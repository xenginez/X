#include "ASTMetaClass.h"

#include "ASTExecutor.h"

namespace
{
	XE::uint64 ClassSize( const XE::SharedPtr< XE::ASTClass > & val )
	{
		return 0;
	}
}

XE::ASTMetaClass::ASTMetaClass( const XE::SharedPtr< XE::ASTClass > & val )
	: XE::MetaClass( val->Name,
					 ClassSize( val ),
					 false,
					 XE::Reflection::FindClass( val->Super ),
					 val->Super.empty() ? nullptr : XE::Reflection::FindClass( val->Super ),
					 val->Module.empty() ? nullptr : XE::Reflection::FindModule( val->Module ), {} ),
	_Class( val )
{

}

XE::ASTMetaClass::~ASTMetaClass()
{

}

void XE::ASTMetaClass::Destruct( void * ptr ) const
{
	XE::ASTExecutor exec;

	XE::InvokeStack args;

	args.Push( XE::Variant( ptr, this ) );

	exec.Invoke( _Class->Destruct, &args );
}

XE::Variant XE::ASTMetaClass::Construct( void * ptr ) const
{
	XE::ASTExecutor exec;

	XE::InvokeStack args;

	args.Push( XE::Variant( ptr, this ) );

	return exec.Invoke( _Class->Construct, &args );
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
