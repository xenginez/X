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
	if ( _JITCode.data() == nullptr )
	{
		if ( auto cache = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::CacheService >() )
		{
			auto hash = GetHashCode();

			XE::MemoryView view = cache->FindCache( hash );
			if ( view.data() == nullptr )
			{
				if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
				{
					thread->PostTask( XE::ThreadType::WORKS, [this, hash, cache]()
					{
						cache->InsertCache( hash, XE::ASTJITCompileContext::ThreadInstance()->Compile( _Function ) );
					} );
				}
			}
			else
			{
				return XE::ASTJITCompileContext::ThreadInstance()->Invoke( (const void *)( view.data() ), params );
			}
	}
}
#endif

	return XE::ASTExecuteContext::ThreadInstance()->Invoke( _Function, params );
}
