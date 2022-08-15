#include "MetaVariable.h"

#include "MetaAttribute.h"

XE::MetaVariable::MetaVariable( const String & Name, bool IsConst, XE::TypeFlag Flag, MetaTypeCPtr Value, MetaModuleCPtr Module )
	:MetaInfo( Name, MetaInfoType::VARIABLE, nullptr, Module ), _IsConst( IsConst ), _Flag( Flag ), _Value( Value )
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
	return _Flag == TypeFlag::VALUE;
}

bool XE::MetaVariable::IsPointer() const
{
	return _Flag == TypeFlag::POINTER;
}

bool XE::MetaVariable::IsSharedPtr() const
{
	return _Flag == TypeFlag::SHAREDPTR;
}

bool XE::MetaVariable::IsReference() const
{
	return _Flag == TypeFlag::REFERENCE;
}

XE::TypeFlag XE::MetaVariable::GetFlag() const
{
	return _Flag;
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
