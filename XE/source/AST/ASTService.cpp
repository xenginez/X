#include "ASTService.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>

#include "ASTInfo.h"
#include "ASTNode.h"
#include "ASTVisitor.h"
#include "ASTContext.h"

IMPLEMENT_META( XE::ASTService )

namespace
{
	static llvm::Function * COMPILING_FLAG = (llvm::Function *)0x00000001;
}

struct XE::ASTService::Private
{
	XE::Set< XE::String > _Macros;
	XE::Map< XE::String, XE::Variant > _Globals;
	XE::MultiMap< XE::MetaClassCPtr, XE::ASTVisitorPtr > _Visitors; // ASTNode Type : ASTVisitor

	llvm::LLVMContext _Context;
	llvm::ExecutionEngine * _Executor = nullptr;
	XE::ConcurrentHashMap< XE::String, llvm::Function * > _Functions;
};

XE::ASTService::ASTService()
	:_p( XE::New< Private >() )
{

}

XE::ASTService::~ASTService()
{
	XE::Delete( _p );
}

void XE::ASTService::Prepare()
{
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmParser();
	llvm::InitializeNativeTargetAsmPrinter();

	auto buf = std::move( llvm::MemoryBuffer::getMemBuffer( llvm::StringRef( nullptr, 0 ) ) );

	auto module = llvm::parseBitcodeFile( *buf, _p->_Context );

	_p->_Executor = llvm::EngineBuilder( std::move( *module ) ).create();
}

void XE::ASTService::Startup()
{

}

void XE::ASTService::Update()
{

}

void XE::ASTService::Clearup()
{
	delete _p->_Executor; _p->_Executor = nullptr;

	llvm::llvm_shutdown();
}

void XE::ASTService::Visit( XE::ASTContext * context, const XE::ASTNode * node ) const
{
	auto ranges = _p->_Visitors.equal_range( node->GetMetaClass() );
	for ( auto it = ranges.first; it != ranges.second; ++it )
	{
		if ( it->second->GetMetaClass()->CanConvert( context->GetVisitorBaseClass() ) )
		{
			if ( auto visit = it->second->GetMetaClass()->FindMethod( "Visit" ) )
			{
				if ( context->GetMetaClass()->CanConvert( SP_CAST< const XE::MetaClass >( visit->GetParameterType().front() ) ) )
				{
					XE::InvokeStack args( it->second, context, node );

					visit->Invoke( &args );

					return;
				}
			}
		}
	}
}

void XE::ASTService::Visit( XE::ASTContext * context, const XE::ASTNodePtr & node ) const
{
	Visit( context, node.get() );
}

void XE::ASTService::SetVariable( const XE::String & name, const XE::Variant & val )
{
	_p->_Globals[name] = val;
}

XE::Variant XE::ASTService::GetVariable( const XE::String & name )
{
	auto it = _p->_Globals.find( name );

	return it != _p->_Globals.end() ? it->second : XE::Variant();
}

void XE::ASTService::AddGlobalMacro( const XE::String & val )
{
	_p->_Macros.insert( val );
}

bool XE::ASTService::HasGlobalMacro( const XE::String & val ) const
{
	return _p->_Macros.find( val ) != _p->_Macros.end();
}

XE::CompileStateType XE::ASTService::GetJITCompileState( const XE::String & name ) const
{
	llvm::Function * func = nullptr;
	if ( _p->_Functions.find( name, func ) )
	{
		return func == COMPILING_FLAG ? XE::CompileStateType::COMPILING : XE::CompileStateType::EXIST;
	}
	return XE::CompileStateType::NONE;
}

XE::Delegate< XE::Variant( XE::InvokeStack * ) > XE::ASTService::JITCompile( const XE::String & name, XE::MemoryView bitcodes )
{
	_p->_Functions.insert( name, COMPILING_FLAG );

	auto buf = std::move( llvm::MemoryBuffer::getMemBuffer( llvm::StringRef( bitcodes.data(), bitcodes.size() ) ) );

	_p->_Executor->addModule( std::move( *llvm::parseBitcodeFile( *buf, _p->_Context ) ) );

	auto func = _p->_Executor->FindFunctionNamed( name.c_str() );

	if ( func != nullptr )
	{
		_p->_Functions.insert( name, func );

		return [this, func]( XE::InvokeStack * args ) -> XE::Variant
		{
			XE::Array< llvm::GenericValue > values( XE::MemoryResource::GetFrameMemoryResource() );
			{
				// TODO: 
			}
			auto res = _p->_Executor->runFunction( func, { values.data(), values.size() } );

				// TODO: 
			return {};
		};
	}
	else
	{
		_p->_Functions.erase( name );
	}

	return nullptr;
}

XE::Delegate< XE::Variant( XE::InvokeStack * ) > XE::ASTService::FindJITFunction( const XE::String & name ) const
{
	llvm::Function * func = nullptr;

	if ( _p->_Functions.find( name, func ) && func != COMPILING_FLAG )
	{
		return [this, func]( XE::InvokeStack * args ) -> XE::Variant
		{
			XE::Array< llvm::GenericValue > values( XE::MemoryResource::GetFrameMemoryResource() );
			{
				// TODO: 
			}
			auto res = _p->_Executor->runFunction( func, { values.data(), values.size() } );

				// TODO: 
			return {};
		};
	}

	return nullptr;
}
