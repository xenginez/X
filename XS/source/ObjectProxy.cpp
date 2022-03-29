#include "ObjectProxy.h"

XS::ObjectProxy::ObjectProxy()
{

}

XS::ObjectProxy::ObjectProxy( const ObjectProxy & val )
	:_Object( val._Object ), _Property( val._Property )
{

}

XS::ObjectProxy::ObjectProxy( const XE::Variant & obj, const XE::MetaPropertyCPtr & prop /*= nullptr */ )
	: _Object( obj ), _Property( prop )
{

}

XS::ObjectProxy & XS::ObjectProxy::operator=( const ObjectProxy & val )
{
	_Object = val._Object;
	_Property = val._Property;

	return *this;
}

XE::MetaTypeCPtr XS::ObjectProxy::GetType() const
{
	if ( _Property )
	{
		return _Property->GetValueType();
	}

	return _Object.GetType();
}

XE::MetaAttributeCPtr XS::ObjectProxy::FindAttribute( const XE::MetaClassCPtr & type ) const
{
	if ( _Property )
	{
		return _Property->FindAttribute( type );
	}

	return _Object.GetType()->FindAttribute( type );
}

XE::Variant XS::ObjectProxy::GetValue() const
{
	if ( _Property )
	{
		return _Property->Get( _Object );
	}

	return _Object;
}

void XS::ObjectProxy::SetValue( const XE::Variant & val )
{
	if ( _Property )
	{
		return _Property->Set( _Object, val );
	}

	_Object = val;
}

const XE::String & XS::ObjectProxy::GetName() const
{
	if ( _Property )
	{
		return _Property->GetFullName();
	}

	return XE::String::empty_string;
}
