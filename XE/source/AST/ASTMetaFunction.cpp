#include "ASTMetaFunction.h"

#include "ASTExecutor.h"

namespace
{
	XE::ParameterType GetFunctionParameter( const XE::SharedPtr< XE::ASTFunction > & val )
	{
		XE::ParameterType result;

		for ( const auto & it : val->Parameters )
		{
			result.push_back( XE::Reflection::FindType( it.first ) );
		}

		return result;
	}
}

XE::ASTMetaFunction::ASTMetaFunction( const XE::SharedPtr< XE::ASTFunction > & val )
	: XE::MetaFunction( val->Name, XE::Reflection::FindClass( val->Result ), GetFunctionParameter( val ), XE::Reflection::FindModule( val->Module ) )
	, _Function( val )
{

}

XE::ASTMetaFunction::~ASTMetaFunction()
{

}

XE::Variant XE::ASTMetaFunction::Invoke( XE::InvokeStack * params ) const
{
	XE::ASTExecutor exec;

	return exec.Invoke( _Function, params );
}
