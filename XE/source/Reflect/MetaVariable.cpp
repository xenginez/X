#include "MetaVariable.h"

XE::MetaVariable::MetaVariable( const XE::String & Name, bool IsConst, XE::TypeFlag Flag, XE::MetaTypeCPtr Value, XE::MetaModuleCPtr Module )
	: XE::MetaInfo( Name, XE::MetaInfoType::VARIABLE, nullptr, Module ), _IsConst( IsConst ), _Flag( Flag ), _Value( Value )
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
	return _Flag == XE::TypeFlag::VALUE;
}

bool XE::MetaVariable::IsPointer() const
{
	return _Flag == XE::TypeFlag::POINTER;
}

bool XE::MetaVariable::IsSharedPtr() const
{
	return _Flag == XE::TypeFlag::SHAREDPTR;
}

bool XE::MetaVariable::IsReference() const
{
	return _Flag == XE::TypeFlag::REFERENCE;
}

XE::TypeFlag XE::MetaVariable::GetFlag() const
{
	return _Flag;
}

XE::MetaTypeCPtr XE::MetaVariable::GetValueType() const
{
	return _Value.lock();
}

XE::Variant XE::MetaVariable::Get() const
{
	return Getter();
}

void XE::MetaVariable::Set( const XE::Variant & val ) const
{
	Setter( val );
}
