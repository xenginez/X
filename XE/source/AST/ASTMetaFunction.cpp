#include "ASTMetaFunction.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"

namespace
{
	XE::ParameterType GetFunctionParameter( const XE::ASTInfoFunctionPtr & val )
	{
		XE::ParameterType result;

		for ( const auto & it : val->Parameters )
		{
			result.push_back( XE::Reflection::FindType( it.first ) );
		}

		return result;
	}
}

XE::ASTMetaFunction::ASTMetaFunction( const XE::ASTInfoFunctionPtr & val, const XE::MetaModuleCPtr & Module )
	: XE::MetaFunction( val->Name, XE::Reflection::FindClass( val->Result->Name ), GetFunctionParameter( val ), Module )
	, _Function( val )
{

}

XE::ASTMetaFunction::~ASTMetaFunction()
{

}

XE::Variant XE::ASTMetaFunction::Invoke( XE::InvokeStack * params ) const
{
	return XE::ASTExecuteContext( XE::MemoryResource::GetFrameMemoryResource() ).Invoke( _Function, params );
}
