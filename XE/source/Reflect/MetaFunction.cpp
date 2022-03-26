#include "MetaFunction.h"

XE::MetaFunction::MetaFunction( const String & Name, MetaInfoPtr Result, ParameterType && Parameter, MetaModulePtr Module )
	:MetaInfo( Name, MetaInfoType::FUNCTION, nullptr, Module ), _Result( Result ), _Parameter( Parameter )
{

}

XE::MetaFunction::~MetaFunction()
{

}

XE::MetaInfoPtr XE::MetaFunction::GetResultType() const
{
	return _Result.lock();
}

const XE::ParameterType & XE::MetaFunction::GetParameterType() const
{
	return _Parameter;
}
