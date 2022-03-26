#include "MetaMethod.h"

#include "MetaClass.h"



XE::MetaMethod::MetaMethod( const String & Name, bool IsStatic, bool IsConst, MetaTypePtr Result, ParameterType && Parameter, MetaClassPtr Owner, MetaModulePtr Module )
	:MetaInfo( Name, MetaInfoType::METHOD, SP_CAST< MetaInfo >( Owner ), Module ), _IsStatic( IsStatic ), _IsConst( IsConst ), _Result( Result ), _Parameter( Parameter )
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

XE::MetaTypePtr XE::MetaMethod::GetResultType() const
{
	return _Result.lock();
}

const XE::ParameterType& XE::MetaMethod::GetParameterType() const
{
	return _Parameter;
}
