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
	if ( _Callback != nullptr )
	{
		return _Callback( params );
	}
	else
	{
		if ( auto ast = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
		{
			switch ( ast->GetJITCompileState( GetFullName() ) )
			{
			case XE::CompileStateType::NONE:
				if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
				{
					thread->PostTask( XE::ThreadType::WORKS, [this, ast]()
					{
						_Callback = ast->JITCompile( GetFullName(), XE::ASTCompileContext::ThreadInstance()->Compile( _Method ) );
					} );
				}
				break;
			case XE::CompileStateType::EXIST:
				_Callback = ast->FindJITFunction( GetFullName() );
				return _Callback( params );
			default:
				break;
			}
		}
	}
#endif

	return XE::ASTExecuteContext::ThreadInstance()->Invoke( _Method, params );
}
