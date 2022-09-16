#include "ASTMetaClass.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"

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

	XE::ASTExecuteContext( XE::MemoryResource::GetFrameMemoryResource() ).Invoke( _Class->Destruct, &args );
}

XE::Variant XE::ASTMetaClass::Construct( void * ptr ) const
{
	XE::InvokeStack args;

	args.Push( XE::Variant( ptr, this ) );

	return XE::ASTExecuteContext( XE::MemoryResource::GetFrameMemoryResource() ).Invoke( _Class->Construct, &args );
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
