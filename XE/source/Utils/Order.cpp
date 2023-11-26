#include "Order.h"

#include <array>

#include "CRC32.h"

struct OrderInfo
{
	XE::uint64 id;
	XE::String name;
	XE::String desc;
	XE::MetaInfoCPtr info;
};

struct XE::Order::Private
{
	std::array< XE::Map< XE::uint64, OrderInfo >, 256 > Groups;
};

XE::Order::Order()
	:_p( XE::New< Private >() )
{
}

XE::Order::~Order()
{
	XE::Delete( _p );
}

#define _p XE::Order::Instance()->_p

XE::String XE::Order::FindOrderName( XE::uint64 id )
{
	XE::uint64 group = id >> 56;

	auto it = _p->Groups[group].find( id );
	if( it != _p->Groups[group].end() )
	{
		return it->second.name;
	}

	return String();
}

XE::String XE::Order::FindOrderDesc( XE::uint64 id )
{
	XE::uint64 group = id >> 56;

	auto it = _p->Groups[group].find( id );
	if( it != _p->Groups[group].end() )
	{
		return it->second.desc;
	}

	return XE::String();
}

XE::MetaInfoCPtr XE::Order::FindOrderPatameter( XE::uint64 id )
{
	XE::uint64 group = id >> 56;

	auto it = _p->Groups[group].find( id );
	if( it != _p->Groups[group].end() )
	{
		return it->second.info;
	}

	return nullptr;
}

XE::uint64 XE::Order::RegisterOrder( XE::uint8 group, const XE::String & name, const XE::String & desc, const XE::MetaInfoCPtr & parameter )
{
	XE::uint64 hash = ( XE::uint64( group ) << 56 ) | ( XE::Hash( name ) >> 8 );

	auto it = _p->Groups[group].find( hash );
	if( it != _p->Groups[group].end() )
	{
		if( it->second.name == name )
		{
			return hash;
		}

		return Invalid;
	}

	OrderInfo info;

	info.id = hash;
	info.name = name;
	info.desc = desc;
	info.info = parameter;

	_p->Groups[group].insert( std::make_pair( hash, info ) );

	return hash;
}

XE::uint64 XE::Order::FindOrderID( XE::uint8 group, const XE::String & name )
{
	XE::uint64 hash = ( XE::uint64( group ) << 56 ) | ( XE::Hash( name ) >> 8 );

	auto it = _p->Groups[group].find( hash );
	if( it != _p->Groups[group].end() )
	{
		if( it->second.name == name )
		{
			return hash;
		}
	}

	return Invalid;
}

void XE::Order::VisitOrder( XE::uint8 group, XE::Delegate< void( XE::uint64, XE::String, XE::String, const XE::MetaInfoCPtr & ) > val )
{
	for( const auto & it : _p->Groups[group] )
	{
		val( it.second.id, it.second.name, it.second.desc, it.second.info );
	}
}
