#include "CacheService.h"

#include <fstream>
#include <shared_mutex>

#include "Memory/MMapFile.h"

#include "CoreFramework.h"

IMPLEMENT_META( XE::CacheService );

#define ITEM_FLAG_NONE (0 << 0)
#define ITEM_FLAG_FREE (1 << 0)

struct CacheItem
{
	XE::uint64 Hash = 0;
	XE::uint64 Flag = 0;
	XE::uint64 Size = 0;
	XE::uint64 Capacity = 0;
};

struct XE::CacheService::Private
{
	XE::MMapFile _MMap;
	std::shared_mutex _Mutex;
	XE::Map< XE::uint64, CacheItem * > _Items;
	XE::MultiMap< XE::uint64, CacheItem * > _Frees;
};

XE::CacheService::CacheService()
	:_p( XE::New< Private >() )
{

}

XE::CacheService::~CacheService()
{
	XE::Delete( _p );
}

void XE::CacheService::Prepare()
{
	_p->_MMap.Open( GetFramework()->GetCachesPath() / CACHEDATA_FILE_NAME );

	Reset();
}

void XE::CacheService::Startup()
{

}

void XE::CacheService::Update()
{

}

void XE::CacheService::Clearup()
{
	_p->_Items.clear();
	_p->_Frees.clear();

	_p->_MMap.Close();
}

void XE::CacheService::InsertCache( XE::uint64 hash, XE::MemoryView data )
{
	std::unique_lock< std::shared_mutex > lock( _p->_Mutex );

	auto path = GetFramework()->GetCachesPath() / CACHEDATA_FILE_NAME;

	_p->_MMap.Close();
	{
		CacheItem item{ hash, 0, data.size(), data.size() };

		std::ofstream ofs( path, std::ios::out | std::ios::app | std::ios::binary );

		ofs.write( (const char *)&item, sizeof( item ) );
		ofs.write( data.data(), data.size() );

		ofs.close();
	}
	_p->_MMap.Open( path );

	Reset();
}

void XE::CacheService::RemoveCache( XE::uint64 hash )
{
	auto it = _p->_Items.find( hash );
	if( it != _p->_Items.end() )
	{
		it->second->Flag |= ITEM_FLAG_FREE;
		_p->_Frees.insert( *it );
		_p->_Items.erase( it );
	}
}

void XE::CacheService::UpdateCache( XE::uint64 hash, XE::MemoryView data )
{
	CacheItem * item = nullptr;

	auto it = _p->_Items.find( hash );
	if( it != _p->_Items.end() )
	{
		if( it->second->Capacity >= data.size() )
		{
			item = it->second;
		}
		else
		{
			it->second->Flag |= ITEM_FLAG_FREE;
			_p->_Frees.insert( { it->second->Capacity, it->second } );
			_p->_Items.erase( it );
		}
	}

	if( item == nullptr )
	{
		auto it = _p->_Frees.upper_bound( data.size() );
		if( it != _p->_Frees.end() )
		{
			item = it->second;

			_p->_Items.insert( { item->Hash, item } );

			_p->_Frees.erase( it );
		}
	}
	
	if( item != nullptr )
	{
		item->Hash = hash;
		item->Size = data.size();
		item->Flag = ITEM_FLAG_NONE;

		std::memcpy( (void *)( (XE::uint8 *)item + sizeof( CacheItem ) ), data.data(), data.size() );
	}
	else
	{
		InsertCache( hash, data );
	}
}

XE::MemoryView XE::CacheService::FindCache( XE::uint64 hash )
{
	std::shared_lock< std::shared_mutex > lock( _p->_Mutex );

	auto it = _p->_Items.find( hash );
	if( it != _p->_Items.end() )
	{
		return { ( (XE::int8 *)it->second + sizeof( CacheItem ) ), it->second->Size };
	}

	return {};
}

void XE::CacheService::Reset()
{
	_p->_Items.clear();
	_p->_Frees.clear();

	XE::uint64 size = _p->_MMap.GetSize();
	XE::uint8 * address = _p->_MMap.GetAddress();

	CacheItem * item = (CacheItem *)address;
	while( (XE::uint8 *)item < ( address + size ) )
	{
		if( ( item->Flag & ITEM_FLAG_FREE ) == 0 )
		{
			_p->_Items.insert( { item->Hash, item } );
		}
		else
		{
			_p->_Frees.insert( { item->Capacity, item } );
		}

		item = (CacheItem *)( (XE::uint8 *)item + sizeof( CacheItem ) + item->Capacity );
	}
}
