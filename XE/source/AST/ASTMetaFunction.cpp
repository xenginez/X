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
						_Callback = ast->JITCompile( GetFullName(), XE::ASTCompileContext::ThreadInstance()->Compile( _Function ) );
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

	return XE::ASTExecuteContext::ThreadInstance()->Invoke( _Function, params );
}
