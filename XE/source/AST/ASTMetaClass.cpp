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
	if ( _Destructor != nullptr )
	{
		_Destructor( &args );

		return;
	}
	else
	{
		if ( auto ast = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
		{
			switch ( ast->GetJITCompileState( GetFullName() + "." + "Destruct" ) )
			{
			case XE::CompileStateType::NONE:
				if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
				{
					thread->PostTask( XE::ThreadType::WORKS, [this, ast]()
					{
						_Destructor = ast->JITCompile( GetFullName() + "." + "Destruct", XE::ASTCompileContext::ThreadInstance()->Compile( _Class->Destruct ) );
					} );
				}
				break;
			case XE::CompileStateType::EXIST:
				_Destructor = ast->FindJITFunction( GetFullName() + "." + "Destruct" );
				_Destructor( &args );
				return;
			default:
				break;
			}
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
	if ( _Constructor != nullptr )
	{
		return _Constructor( &args );
	}
	else
	{
		if ( auto ast = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
		{
			switch ( ast->GetJITCompileState( GetFullName() + "." + "Construct" ) )
			{
			case XE::CompileStateType::NONE:
				if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
				{
					thread->PostTask( XE::ThreadType::WORKS, [this, ast]()
					{
						_Constructor = ast->JITCompile( GetFullName() + "." + "Construct", XE::ASTCompileContext::ThreadInstance()->Compile( _Class->Construct ) );
					} );
				}
				break;
			case XE::CompileStateType::EXIST:
				_Constructor = ast->FindJITFunction( GetFullName() + "." + "Construct" );
				return _Constructor( &args );
			default:
				break;
			}
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
