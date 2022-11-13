#include "ASTMetaMethod.h"

#include "Core/CacheService.h"
#include "Core/ThreadService.h"
#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"
#include "ASTService.h"

namespace
{
	XE::ParameterType GetMethodParameter( const XE::ASTInfoFunctionPtr & val )
	{
		XE::ParameterType result;

		for ( const auto & it : val->Parameters )
		{
			result.push_back( XE::Reflection::FindType( it.first ) );
		}

		return result;
	}
}

XE::ASTMetaMethod::ASTMetaMethod( const XE::ASTInfoFunctionPtr & val, const XE::MetaClassCPtr Owner, const XE::MetaModuleCPtr & Module )
	: XE::MetaMethod( val->Name,
					  false,
					  false,
					  XE::Reflection::FindType( val->Result->Name ),
					  GetMethodParameter( val ),
					  Owner,
					  Module )
	, _Method( val )
{

}

XE::ASTMetaMethod::~ASTMetaMethod()
{

}

XE::Variant XE::ASTMetaMethod::Invoke( XE::InvokeStack * params ) const
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

		return XE::ASTExecuteContext().Invoke( ast->GetInstance(), _Method, params );
	}

	return {};
}
