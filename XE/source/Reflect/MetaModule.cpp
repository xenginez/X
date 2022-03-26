#include "MetaModule.h"

#include "Reflection.h"

XE::MetaModule::MetaModule( const String & Name, MetaInfoPtr Owner )
	:MetaInfo( Name, MetaInfoType::MODULE, Owner, nullptr )
{

}

XE::MetaModule::~MetaModule()
{

}

const XE::Array< XE::MetaInfoPtr > & XE::MetaModule::GetMetaInfos() const
{
	return _MetaInfos;
}

XE::MetaInfoPtr XE::MetaModule::FindMetaInfo( XE::uint64 hash ) const
{
	for( const auto & it : _MetaInfos )
	{
		if( it->GetHashCode() == hash )
		{
			return it;
		}
	}

	return nullptr;
}

XE::MetaInfoPtr XE::MetaModule::FindMetaInfo( const String & val ) const
{
	for( const auto & it : _MetaInfos )
	{
		if( it->GetName() == val )
		{
			return it;
		}
		else if( it->GetFullName() == val )
		{
			return it;
		}
	}

	return nullptr;
}

void XE::MetaModule::Visit( const XE::Delegate< void( const  XE::MetaInfoPtr & ) > & val ) const
{
	for( const auto & it : _MetaInfos )
	{
		val( it );
	}
}

void XE::MetaModule::RegisterMetaInfo( MetaInfoPtr val )
{
	_MetaInfos.push_back( val );
}
