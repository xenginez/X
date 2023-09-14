#include "MetaProperty.h"

#include "MetaClass.h"

XE::MetaProperty::MetaProperty( const XE::String & Name, bool IsStatic, bool IsConst, XE::TypeFlag Flag, XE::MetaTypeCPtr Value, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
	: XE::MetaInfo( Name, XE::MetaInfoType::PROPERTY, Owner, Module ), _IsStatic( IsStatic ), _IsConst( IsConst ), _Flag( Flag ), _Value( Value )
{

}

XE::MetaProperty::~MetaProperty()
{

}

bool XE::MetaProperty::IsConst() const
{
	return _IsConst;
}

bool XE::MetaProperty::IsStatic() const
{
	return _IsStatic;
}

bool XE::MetaProperty::IsObject() const
{
	return _Flag == XE::TypeFlag::VALUE;
}

bool XE::MetaProperty::IsPointer() const
{
	return _Flag == XE::TypeFlag::POINTER;
}

bool XE::MetaProperty::IsSharedPtr() const
{
	return _Flag == XE::TypeFlag::SHAREDPTR;
}

bool XE::MetaProperty::IsReference() const
{
	return _Flag == XE::TypeFlag::REFERENCE;
}

XE::TypeFlag XE::MetaProperty::GetFlag() const
{
	return _Flag;
}

XE::MetaTypeCPtr XE::MetaProperty::GetValueType() const
{
	return _Value.lock();
}

XE::Variant XE::MetaProperty::Get( const XE::Variant & obj ) const
{
	return Getter( obj );
}

void XE::MetaProperty::Set( const XE::Variant & obj, const XE::Variant & val ) const
{
	Setter( obj, val );
}
