#include "MetaVariable.h"

#include "MetaAttribute.h"

XE::MetaVariable::MetaVariable( const String & Name, bool IsConst, bool IsObject, bool IsPointer, bool IsSharedPtr, bool IsReference, MetaTypeCPtr Value, MetaModuleCPtr Module )
	:MetaInfo( Name, MetaInfoType::VARIABLE, nullptr, Module ), _IsConst( IsConst ), _IsObject( IsObject ), _IsPointer( IsPointer ), _IsSharedPtr( IsSharedPtr ), _IsReference( IsReference ), _Value( Value )
{

}

XE::MetaVariable::~MetaVariable()
{

}

bool XE::MetaVariable::IsConst() const
{
	return _IsConst;
}

bool XE::MetaVariable::IsObject() const
{
	return _IsObject;
}

bool XE::MetaVariable::IsPointer() const
{
	return _IsPointer;
}

bool XE::MetaVariable::IsSharedPtr() const
{
	return _IsSharedPtr;
}

bool XE::MetaVariable::IsReference() const
{
	return _IsReference;
}

XE::MetaTypeCPtr XE::MetaVariable::GetValueType() const
{
	return _Value.lock();
}

const XE::Array< XE::MetaAttributeCPtr > & XE::MetaVariable::GetAttributes() const
{
	return _Attributes;
}

XE::MetaAttributeCPtr XE::MetaVariable::FindAttribute( const XE::MetaClassCPtr & val ) const
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

XE::Variant XE::MetaVariable::Get() const
{
	return Getter();
}

void XE::MetaVariable::Set( const XE::Variant & val ) const
{
	Setter( val );
}
