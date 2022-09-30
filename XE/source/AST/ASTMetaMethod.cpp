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
					cache->InsertCache( hash, XE::ASTJITCompileContext::ThreadInstance()->Compile( _Method ), true );
				} );
			}
		}
		else
		{
			return XE::ASTJITCompileContext::ThreadInstance()->Invoke( (const void *)( view.data() ), params );
		}
}
#endif

	return XE::ASTExecuteContext::ThreadInstance()->Invoke( _Method, params );
}
