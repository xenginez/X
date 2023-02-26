#include "ObjectProxy.h"

XS::ObjectProxy::ObjectProxy( ObjectProxy * parent /*= nullptr */ )
	:_Parent( parent )
{

}

XS::ObjectProxy * XS::ObjectProxy::GetParent() const
{
	return _Parent;
}

XS::VariantObjectProxy::VariantObjectProxy( const XE::Variant & obj, ObjectProxy * parent /*= nullptr */ )
	: ObjectProxy( parent ), _Value( obj )
{

}

XE::MetaTypeCPtr XS::VariantObjectProxy::GetType() const
{
	return _Value.GetType();
}

XE::MetaAttributeCPtr XS::VariantObjectProxy::FindAttribute( const XE::MetaClassCPtr & type ) const
{
	return _Value.GetType()->FindAttribute( type );
}

XE::Variant XS::VariantObjectProxy::GetValue() const
{
	return _Value;
}

void XS::VariantObjectProxy::SetValue( const XE::Variant & val )
{
	_Value = val;
}

const XE::String & XS::VariantObjectProxy::GetName() const
{
	return GetType()->GetFullName();
}


XS::ElementObjectProxy::ElementObjectProxy( XE::uint64 i, XE::MetaEnumeratorPtr & enumer, XS::ObjectProxy * parent )
	: ObjectProxy( parent ), _Index( i ), _Enumerator( enumer )
{

}

XE::MetaTypeCPtr XS::ElementObjectProxy::GetType() const
{
	return SP_CAST< const XE::MetaClass >( GetParent()->GetType() )->GetElementType();
}

const XE::String & XS::ElementObjectProxy::GetName() const
{
	return GetType()->GetFullName();
}

XE::MetaAttributeCPtr XS::ElementObjectProxy::FindAttribute( const XE::MetaClassCPtr & type ) const
{
	return GetType()->FindAttribute( type );
}

XE::Variant XS::ElementObjectProxy::GetValue() const
{
	_Enumerator->SetIndex( _Index );

	return _Enumerator->GetCurrent();
}

void XS::ElementObjectProxy::SetValue( const XE::Variant & val )
{
	_Enumerator->SetIndex( _Index );

	_Enumerator->SetCurrent( val );
}


XS::PropertyObjectProxy::PropertyObjectProxy( const XE::MetaPropertyCPtr & prop, XS::ObjectProxy * parent )
	: ObjectProxy( parent ), _Property( prop )
{

}

XE::MetaTypeCPtr XS::PropertyObjectProxy::GetType() const
{
	return _Property->GetValueType();
}

const XE::String & XS::PropertyObjectProxy::GetName() const
{
	return _Property->GetName();
}

XE::MetaAttributeCPtr XS::PropertyObjectProxy::FindAttribute( const XE::MetaClassCPtr & type ) const
{
	if ( auto attr = _Property->FindAttribute( type ) )
	{
		return attr;
	}

	return GetType()->FindAttribute( type );
}

XE::Variant XS::PropertyObjectProxy::GetValue() const
{
	return _Property->Get( GetParent()->GetValue() );
}

void XS::PropertyObjectProxy::SetValue( const XE::Variant & val )
{
	_Property->Set( GetParent()->GetValue(), val );
}
