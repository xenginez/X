#include "ObjectProxy.h"

XS::ObjectProxy::ObjectProxy( ObjectProxy * parent /*= nullptr */ )
	:_Parent( parent )
{

}

XS::ObjectProxy * XS::ObjectProxy::GetParent() const
{
	return _Parent;
}

XS::VariantObjectProxy::VariantObjectProxy( const XE::Variant & obj )
	:XS::ObjectProxy( nullptr ), _Object( obj )
{

}

XE::MetaTypeCPtr XS::VariantObjectProxy::GetType() const
{
	return _Object.GetType();
}

XE::MetaAttributeCPtr XS::VariantObjectProxy::FindAttribute( const XE::MetaClassCPtr & type ) const
{
	return _Object.GetType()->FindAttribute( type );
}

XE::Variant XS::VariantObjectProxy::GetValue() const
{
	return _Object;
}

void XS::VariantObjectProxy::SetValue( const XE::Variant & val )
{
	_Object = val;
}

const XE::String & XS::VariantObjectProxy::GetName() const
{
	return GetType()->GetFullName();
}
