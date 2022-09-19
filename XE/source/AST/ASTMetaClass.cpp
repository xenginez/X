#include "ASTMetaClass.h"

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
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->Execute( XE::ASTJITCompileContext::ThreadInstance(), _Class->Destruct, &args );
#else
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->Execute( XE::ASTExecuteContext::ThreadInstance(), _Class->Destruct, &args );
#endif
}

XE::Variant XE::ASTMetaClass::Construct( void * ptr ) const
{
	if ( ptr == nullptr )
	{
		ptr = XE::MemoryResource::Alloc( GetSize() );
	}

	XE::InvokeStack args( XE::Variant( ptr, this ) );

#if HAS_JIT
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->Execute( XE::ASTJITCompileContext::ThreadInstance(), _Class->Construct, &args );
#else
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->Execute( XE::ASTExecuteContext::ThreadInstance(), _Class->Construct, &args );
#endif
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
