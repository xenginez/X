#include "ASTMetaMethod.h"

#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"
#include "ASTService.h"

namespace
{
	XE::ParameterType GetMethodParameter( const XE::ASTInfoMethodPtr & val )
	{
		XE::ParameterType result;

		for ( const auto & it : val->Parameters )
		{
			result.push_back( XE::Reflection::FindType( it.first ) );
		}

		return result;
	}
}

XE::ASTMetaMethod::ASTMetaMethod( const XE::ASTInfoMethodPtr & val, const XE::MetaClassCPtr Owner, const XE::MetaModuleCPtr & Module )
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
#if HAS_JIT
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->Execute( XE::ASTJITCompileContext::ThreadInstance(), _Method, params );
#else
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->Execute( XE::ASTExecuteContext::ThreadInstance(), _Method, params );
#endif
}
