#include "AssetsService.h"

#include "Utils/Logger.h"
#include "Utils/Package.h"

#include "CoreFramework.h"
#include "ThreadService.h"

BEG_META( XE::AssetsService )
END_META()

struct XE::AssetsService::Private
{
	XE::Array< XE::SharedPtr< XE::Package > > _Packages;
	XE::ConcurrentLruCache< XE::String, XE::MemoryStream > _LruCache = { 500 };
};

XE::AssetsService::AssetsService()
	:_p( XE::New<Private>() )
{

}

XE::AssetsService::~AssetsService()
{
	XE::Delete( _p );
}

void XE::AssetsService::Prepare()
{

}

void XE::AssetsService::Startup()
{
	auto path = GetFramework()->GetAssetsPath();

	for ( auto & it : std::filesystem::directory_iterator( path ) )
	{
		auto p = it.path();
		if ( p.extension() == ".pak" )
		{
			auto pak = XE::MakeShared< XE::Package >();

			if ( !pak->Open( p ) )
			{
				XE_ERROR( "{%0} failed to open!", p );
				continue;
			}

			_p->_Packages.push_back( pak );
		}
	}

	std::sort( _p->_Packages.begin(), _p->_Packages.end(), []( const auto & left, const auto & right ) { return left->GetVersion() > right->GetVersion(); } );
}

void XE::AssetsService::Update()
{

}

void XE::AssetsService::Clearup()
{
	_p->_LruCache.clear();
	_p->_Packages.clear();
}

XE::MemoryView XE::AssetsService::Load( const XE::UUID & uuid )
{
	return Load( uuid.ToString() );
}

XE::MemoryView XE::AssetsService::Load( const XE::String & name )
{
	auto it = _p->_LruCache.find( name );

	if ( it.value().width() == 0 )
	{
		for ( const auto & pak : _p->_Packages )
		{
			if ( pak->ExistsEntry( name ) )
			{
				if ( pak->GetEntry( name, it.value() ) )
				{
					break;
				}
			}
		}
	}

	return it.value().view();
}

void XE::AssetsService::AsyncLoad( const XE::UUID & uuid, const LoadFinishCallback & callback )
{
	AsyncLoad( uuid.ToString(), callback );
}

void XE::AssetsService::AsyncLoad( const XE::String & name, const LoadFinishCallback & callback )
{
	if ( !CHECK_THREAD( XE::ThreadType::IO ) )
	{
		GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( XE::ThreadType::IO, [=]() { callback( Load( name ) ); } );
	}
	else
	{
		callback( Load( name ) );
	}
}

XE::ObjectPtr XE::AssetsService::LoadObject( const XE::UUID & uuid )
{
	return LoadObject( uuid.ToString() );
}

XE::ObjectPtr XE::AssetsService::LoadObject( const XE::String & name )
{
	auto it = _p->_LruCache.find( name );

	if ( it.value().width() == 0 )
	{
		for ( const auto & pak : _p->_Packages )
		{
			if ( pak->ExistsEntry( name ) )
			{
				if ( pak->GetEntry( name, it.value() ) )
				{
					break;
				}
			}
		}
	}

	XE::ObjectPtr obj;
	XE::BinaryIArchive archive( it.value() );

	archive & obj;

	return obj;
}

void XE::AssetsService::AsyncLoadObject( const XE::UUID & uuid, const LoadObjectFinishCallback & callback )
{
	AsyncLoadObject( uuid.ToString(), callback );
}

void XE::AssetsService::AsyncLoadObject( const XE::String & name, const LoadObjectFinishCallback & callback )
{
	if ( !CHECK_THREAD( XE::ThreadType::IO ) )
	{
		GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( XE::ThreadType::IO, [=]() { callback( LoadObject( name ) ); } );
	}
	else
	{
		callback( LoadObject( name ) );
	}
}
