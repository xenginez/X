#include "MetaProperty.h"

#include "MetaClass.h"
#include "MetaAttribute.h"

XE::MetaProperty::MetaProperty( const String & Name, bool IsStatic, bool IsConst, bool IsObject, bool IsPointer, bool IsSharedPtr, bool IsReference, MetaTypePtr Value, MetaClassPtr Owner, MetaModulePtr Module )
	:MetaInfo( Name, MetaInfoType::PROPERTY, SP_CAST< MetaInfo >( Owner ), Module ), _IsStatic( IsStatic ), _IsConst( IsConst ), _IsObject( IsObject ), _IsPointer( IsPointer ), _IsSharedPtr( IsSharedPtr ), _IsReference( IsReference ), _Value( Value )
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
	return _IsObject;
}

bool XE::MetaProperty::IsPointer() const
{
	return _IsPointer;
}

bool XE::MetaProperty::IsSharedPtr() const
{
	return _IsSharedPtr;
}

bool XE::MetaProperty::IsReference() const
{
	return _IsReference;
}

XE::MetaTypePtr XE::MetaProperty::GetValueType() const
{
	return _Value.lock();
}

const XE::Array< XE::MetaAttributeCPtr > & XE::MetaProperty::GetAttributes() const
{
	return _Attributes;
}

XE::MetaAttributeCPtr XE::MetaProperty::FindAttribute( const XE::MetaClassPtr & val ) const
{
	for( const auto & it : _Attributes )
	{
		if( it->GetMetaClass() == val )
		{
			return it;
		}
	}

	return nullptr;
}

XE::Variant XE::MetaProperty::Get( const XE::Variant & obj ) const
{
	return Getter( obj );
}

void XE::MetaProperty::Set( const XE::Variant & obj, const XE::Variant & val ) const
{
	Setter( obj, val );
}
