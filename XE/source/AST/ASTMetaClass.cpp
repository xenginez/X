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
	if ( auto ast = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
	{
		XE::Variant result;
		if ( ast->JITInvoke( GetFullName() + "." + "Destruct", args, result ) )
		{
			return;
		}

		XE::ASTExecuteContext().Invoke( ast->GetInstance(), _Class->Destruct, &args );
	}
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
	if ( auto ast = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
	{
		XE::Variant result;
		if ( ast->JITInvoke( GetFullName() + "." + "Construct", args, result ) )
		{
			return result;
		}

		return XE::ASTExecuteContext().Invoke( ast->GetInstance(), _Class->Construct, &args );
	}
#endif

	return {};
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
