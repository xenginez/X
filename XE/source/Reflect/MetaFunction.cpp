#include "MetaFunction.h"

XE::MetaFunction::MetaFunction( const XE::String & Name, XE::MetaInfoCPtr Result, XE::ParameterType && Parameter, XE::MetaModuleCPtr Module )
	: XE::MetaInfo( Name, XE::MetaInfoType::FUNCTION, nullptr, Module ), _Result( Result ), _Parameter( Parameter )
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
