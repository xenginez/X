#include "AssetsService.h"

#include <fstream>

#include <tbb/concurrent_hash_map.h>
#define TBB_PREVIEW_CONCURRENT_LRU_CACHE 1
#include <tbb/concurrent_lru_cache.h>

#include "Utils/UUID.h"
#include "Utils/Zipper.h"
#include "Utils/Logger.h"

#include "CoreFramework.h"
#include "TimerService.h"
#include "ThreadService.h"

BEG_META( XE::AssetsService )
END_META()

struct XE::AssetsService::Private
{
	XE::Zipper _Zipper;
	tbb::concurrent_hash_map< XE::UUID, XE::ObjectWPtr, tbb::hash_compare< XE::UUID > > _ObjectCache;
	tbb::concurrent_lru_cache< XE::UUID, XE::SharedPtr< XE::MemoryStream > > _MemoryCache = { []( XE::UUID ) { return XE::MakeShared< XE::MemoryStream >(); }, 1000 };
};

XE::AssetsService::AssetsService()
	:_p( XE::New< Private >() )
{

}

XE::AssetsService::~AssetsService()
{
	XE::Delete( _p );
}

void XE::AssetsService::Prepare()
{
}

bool XE::AssetsService::Startup()
{
	std::filesystem::path path = std::filesystem::u8path( GetFramework()->GetString( "System/AssetsService/AssetsPath" ).c_str() );

	_p->_Zipper.Open( path );

	if( !_p->_Zipper.IsOpen() )
	{
		XE_LOG( LoggerLevel::Error, "{%0} open failed!", path );
	}

	return true;
}

void XE::AssetsService::Update()
{

}

void XE::AssetsService::Clearup()
{
	_p->_Zipper.Close();
	_p->_ObjectCache.clear();
}

XE::MemoryView XE::AssetsService::Load( const XE::UUID & uuid, const XE::String & path )
{
	auto handle = _p->_MemoryCache[uuid];
	if( handle && handle.value()->view() )
	{
		return handle.value()->view();
	}

	auto fullpath( GetFramework()->GetCachesPath() / path.c_str() );
	if( std::filesystem::exists( fullpath ) )
	{
		std::ifstream ifs( fullpath );
		if( ifs.is_open() )
		{
			handle.value()->clear();
			char buf[2048];

			while( !ifs.eof() )
			{
				auto sz = ifs.readsome( buf, 2048 );
				handle.value()->write( buf, sz );
			}

			return handle.value()->view();
		}
	}

	auto suuid = XE::ToString( uuid );

	if( _p->_Zipper.ExtractEntry( suuid ) )
	{
		handle.value()->clear();
		_p->_Zipper.GetEntry( suuid, *( handle.value() ) );
		return handle.value()->view();
	}

	XE_LOG( LoggerLevel::Error, "not find file {%0}.", path );

	return nullptr;
}

void XE::AssetsService::AsyncLoad( const XE::UUID & uuid, const XE::String & path, const LoadFinishCallback & callback )
{
	if( CHECK_THREAD( ThreadType::IO ) )
		callback( Load( uuid, path ) );
	else GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( ThreadType::IO, [=]()
																		 {
																			 callback( Load( uuid, path ) );
																		 } );
}

XE::ObjectPtr XE::AssetsService::LoadObject( const XE::UUID & uuid, const XE::String & path )
{
	tbb::concurrent_hash_map< XE::UUID, XE::ObjectWPtr, tbb::hash_compare< XE::UUID > >::accessor it;
	if( _p->_ObjectCache.find( it, uuid ) )
	{
		if( !it->second.expired() )
		{
			return it->second.lock();
		}
	}

	if( auto mem = Load( uuid, path ) )
	{
		XE::BinaryIArchive load( *( _p->_MemoryCache[uuid].value() ) );

		XE::ObjectPtr obj;

		load & obj;

		_p->_ObjectCache.insert( { uuid, obj } );

		GetFramework()->GetServiceT< XE::TimerService >()->StartUnscaleTimer
		( std::chrono::minutes( 1 ), [this, uuid]() -> bool
		  {
			  tbb::concurrent_hash_map< XE::UUID, XE::ObjectWPtr, tbb::hash_compare< XE::UUID > >::accessor accessor;

			  if( _p->_ObjectCache.find( accessor, uuid ) )
			  {
				  return ( !accessor->second.expired() ? true : !_p->_ObjectCache.erase( accessor->first ) );
			  }

			  return false;
		  } );

		return obj;
	}

	return nullptr;
}

void XE::AssetsService::AsyncLoadObject( const XE::UUID & uuid, const XE::String & path, const LoadObjectFinishCallback & callback )
{
	if( CHECK_THREAD( ThreadType::IO ) )
		callback( LoadObject( uuid, path ) );
	else GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( ThreadType::IO, [=]()
																		 {
																			 callback( LoadObject( uuid, path ) );
																		 } );
}

bool XE::AssetsService::LoadStream( const XE::UUID & uuid, const XE::String & path, std::ostream & stream, XE::uint64 offset, XE::uint64 size )
{
	{
		auto handle = _p->_MemoryCache[uuid];
		if( handle && handle.value()->view() )
		{
			auto v = handle.value()->view();

			stream.write( v.data() + offset, std::min( v.size() - offset, size ) );

			return true;
		}
	}

	{
		auto fullpath( GetFramework()->GetCachesPath() / path.c_str() );
		if( std::filesystem::exists( fullpath ) )
		{
			std::ifstream ifs( fullpath, std::ios::binary );
			if( ifs.is_open() )
			{
				XE::Array< char > buf( size, XE::MemoryResource::GetFrameMemoryResource() );

				ifs.seekg( offset, std::ios::beg );
				size = ifs.readsome( buf.data(), size );
				stream.write( buf.data(), size );

				return true;
			}
		}
	}

	{
		auto suuid = XE::ToString( uuid );
		if( _p->_Zipper.ExtractEntry( suuid ) )
		{
			return _p->_Zipper.GetEntryStream( suuid, stream, offset, size );
		}
	}

	XE_LOG( LoggerLevel::Error, "not find file {%0}.", path );

	return false;
}

void XE::AssetsService::AsyncLoadStream( const XE::UUID & uuid, const XE::String & path, XE::uint64 offset, XE::uint64 size, const LoadStreamFinishCallback & callback )
{
	if( CHECK_THREAD( ThreadType::IO ) )
	{
		XE::MemoryStream stream( XE::MemoryResource::GetFrameMemoryResource() );

		if ( LoadStream( uuid, path, stream, offset, size ) )
		{
			callback( stream.view() );
		}
	}
	else
	{
		GetFramework()->GetServiceT< XE::ThreadService >()->PostTask
		( ThreadType::IO, [this, uuid, path, offset, size, callback]()
		  {
			  XE::MemoryStream stream( XE::MemoryResource::GetFrameMemoryResource() );

			  if( LoadStream( uuid, path, stream, offset, size ) )
			  {
				  callback( stream.view() );
			  }
		  } );
	}
}
