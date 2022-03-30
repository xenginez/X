#include "MetaMethod.h"

#include "MetaClass.h"

XE::MetaMethod::MetaMethod( const String & Name, bool IsStatic, bool IsConst, MetaTypeCPtr Result, ParameterType && Parameter, MetaClassCPtr Owner, MetaModuleCPtr Module )
	:MetaInfo( Name, MetaInfoType::METHOD, Owner, Module ), _IsStatic( IsStatic ), _IsConst( IsConst ), _Result( Result ), _Parameter( Parameter )
{

}

XE::MetaMethod::~MetaMethod()
{

}

bool XE::MetaMethod::IsConst() const
{
	return _IsConst;
}

bool XE::MetaMethod::IsStatic() const
{
	return _IsStatic;
}

XE::MetaTypeCPtr XE::MetaMethod::GetResultType() const
{
	return _Result.lock();
}

const XE::ParameterType& XE::MetaMethod::GetParameterType() const
{
	return _Parameter;
}
