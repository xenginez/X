#include "ObjectProxy.h"

XS::ObjectProxy::ObjectProxy( QObject * parent /*= nullptr */ )
	: QObject( parent )
{

}

XS::ObjectProxy::ObjectProxy( const ObjectProxy & val, QObject * parent /*= nullptr */ )
	: QObject( parent ), _Object( val._Object ), _Property( val._Property )
{

}

XS::ObjectProxy::ObjectProxy( const XE::Variant & obj, QObject * parent /*= nullptr */ )
	: QObject( parent ), _Object( obj )
{

}

XS::ObjectProxy::ObjectProxy( const XE::Variant & obj, const XE::MetaPropertyCPtr & prop, QObject * parent /*= nullptr */ )
	: QObject( parent ), _Object( obj ), _Property( prop )
{

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
