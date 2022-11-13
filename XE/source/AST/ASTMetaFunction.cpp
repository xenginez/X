#include "ASTMetaFunction.h"

#include "Core/CacheService.h"
#include "Core/ThreadService.h"
#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"
#include "ASTService.h"

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
	if ( auto ast = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
	{
#if HAS_JIT
		XE::Variant result;
		if ( ast->JITInvoke( GetFullName(), *params, result ) )
		{
			return result;
		}
#endif

		return XE::ASTExecuteContext().Invoke( ast->GetInstance(), _Function, params );
	}

	return {};
}
