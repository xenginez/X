#include "ASTMetaClass.h"

#include "Core/CacheService.h"
#include "Core/ThreadService.h"
#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"
#include "ASTService.h"

namespace
{
	XE::uint64 ClassSize( const XE::ASTInfoClassPtr & val )
	{
		auto sz = XE::Reflection::FindClass( val->SuperClass )->GetSize();

		sz += sizeof( XE::Variant ) * val->Propertys.size();

		return sz;
	}
}

XE::ASTMetaClass::ASTMetaClass( const XE::ASTInfoClassPtr & val, const XE::MetaInfoCPtr Owner, const XE::MetaModuleCPtr & Module )
	: XE::MetaClass( val->Name,
					 ClassSize( val ),
					 false,
					 false,
					 XE::Reflection::FindClass( val->SuperClass ),
					 Owner,
					 Module,
					 {},
					 nullptr ),
	_Class( val )
{

}

XE::ASTMetaClass::~ASTMetaClass()
{

}

void XE::ASTMetaClass::Destruct( void * ptr ) const
{
	XE::InvokeStack args;

	args.Push( XE::Variant( ptr, this ) );

#if HAS_JIT
	if ( auto cache = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::CacheService >() )
	{
		auto hash = XE::Hash( GetFullName() + "." + "Destruct" );

		XE::MemoryView view = cache->FindCache( hash );

		if ( view.data() == nullptr )
		{
			if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
			{
				thread->PostTask( XE::ThreadType::WORKS, [this, hash, cache]()
				{
					cache->InsertCache( hash, XE::ASTJITCompileContext::ThreadInstance()->Compile( _Class->Destruct ), true );
				} );
			}
		}
		else
		{
			XE::ASTJITCompileContext::ThreadInstance()->Invoke( (const void *)( view.data() ), &args );
			return;
		}
	}
#endif

	XE::ASTExecuteContext::ThreadInstance()->Invoke( _Class->Destruct, &args );
}

XE::Variant XE::ASTMetaClass::Construct( void * ptr ) const
{
	if ( ptr == nullptr )
	{
		ptr = XE::MemoryResource::Alloc( GetSize() );
	}

	XE::InvokeStack args( XE::Variant( ptr, this ) );

#if HAS_JIT
	if ( auto cache = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::CacheService >() )
	{
		auto hash = XE::Hash( GetFullName() + "." + "Construct" );

		XE::MemoryView view = cache->FindCache( hash );

		if ( view.data() == nullptr )
		{
			if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
			{
				thread->PostTask( XE::ThreadType::WORKS, [this, hash, cache]()
				{
					cache->InsertCache( hash, XE::ASTJITCompileContext::ThreadInstance()->Compile( _Class->Construct ), true );
				} );
			}
		}
		else
		{
			return XE::ASTJITCompileContext::ThreadInstance()->Invoke( (const void *)( view.data() ), &args );
		}
	}
#endif

	return XE::ASTExecuteContext::ThreadInstance()->Invoke( _Class->Construct, &args );
}

void XE::ASTMetaClass::Clone( const XE::Variant & from, XE::Variant & to ) const
{

}

void XE::ASTMetaClass::Serialize( XE::OArchive & arc, const XE::Variant & val ) const
{

}

void XE::ASTMetaClass::Deserialize( XE::IArchive & arc, XE::Variant & val ) const
{

}
