#include "MetaEnum.h"



XE::MetaEnum::MetaEnum( const String & Name, XE::uint64 Size, MetaInfoCPtr Owner, MetaModuleCPtr Module )
	:MetaType( Name, MetaInfoType::ENUM, Size, Owner, Module )
{

}

XE::MetaEnum::~MetaEnum()
{

}

XE::uint64 XE::MetaEnum::GetFlags() const
{
	XE::uint64 result = 0;
	for ( const auto & it : _Values )
	{
		result |= it.second.ToUInt64();
	}
	return result;
}

XE::uint64 XE::MetaEnum::GetEnumCount() const
{
	return _Values.size();
}

XE::String XE::MetaEnum::FindName( XE::Variant val ) const
{
	for ( auto var : _Values )
	{
		if ( var.second == val )
		{
			return var.first;
		}
	}

	return "";
}

XE::Variant XE::MetaEnum::FindValue( const String& val ) const
{
	for ( auto var : _Values )
	{
		if ( var.first == val )
		{
			return var.second;
		}
	}

	return XE::Variant();
}

const XE::Array< XE::Pair< XE::String, XE::Variant > > & XE::MetaEnum::GetValues() const
{
	return _Values;
}

void XE::MetaEnum::Visit( const XE::Delegate< void( const String &, const XE::Variant & ) > & val ) const
{
	for ( auto var : _Values )
	{
		val( var.first, var.second );
	}
}

void XE::MetaEnum::_RegisterValue( const String& Name, const  XE::Variant & Val )
{
	_Values.push_back( std::make_pair( Name, Val ) );
}
