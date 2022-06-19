#include "Asset.h"

#include "CoreFramework.h"
#include "AssetsService.h"

BEG_META( XE::Asset )
REG_PROPERTY( "UUID", &XE::Asset::GetUUID, &XE::Asset::SetUUID );
END_META()

XE::Asset::Asset()
{

}

XE::Asset::Asset( const Asset & val )
	:_Ptr( val._Ptr ), _UUID( val._UUID )
{

}

XE::Asset::~Asset()
{

}

XE::Asset::operator bool() const
{
	return _Ptr != nullptr;
}

XE::Asset & XE::Asset::operator=( std::nullptr_t )
{
	_Ptr = nullptr;
	_UUID = {};

	return *this;
}

XE::Asset & XE::Asset::operator=( const Asset & val )
{
	_Ptr = val._Ptr;
	_UUID = val._UUID;

	return *this;
}

const XE::UUID & XE::Asset::GetUUID() const
{
	return _UUID;
}

void XE::Asset::SetUUID( const XE::UUID & val )
{
	_UUID = val;
}

void XE::Asset::Load()
{
	if( _Ptr == nullptr )
	{
		LoadFinish( XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::AssetsService >()->LoadObject( _UUID ) );
	}
}

void XE::Asset::AsyncLoad()
{
	if( _Ptr == nullptr )
	{
		XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::AssetsService >()->AsyncLoadObject( _UUID, [this]( XE::ObjectPtr obj ){ LoadFinish( obj ); } );
	}
}

void XE::Asset::LoadFinish( const XE::ObjectPtr & val )
{
	_Ptr = val;
}
