#include "MetaFunction.h"

XE::MetaFunction::MetaFunction( const String & Name, MetaInfoCPtr Result, ParameterType && Parameter, MetaModuleCPtr Module )
	:MetaInfo( Name, MetaInfoType::FUNCTION, nullptr, Module ), _Result( Result ), _Parameter( Parameter )
{

}

XE::MetaFunction::~MetaFunction()
{

}

XE::MetaInfoCPtr XE::MetaFunction::GetResultType() const
{
	return _Result.lock();
}

const XE::ParameterType & XE::MetaFunction::GetParameterType() const
{
	return _Parameter;
}
