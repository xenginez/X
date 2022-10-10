#include "ASTContext.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTNode.h"
#include "ASTVisitor.h"
#include "ASTService.h"

IMPLEMENT_META( XE::ASTContext );
IMPLEMENT_META( XE::ASTExecuteContext );
IMPLEMENT_META( XE::ASTCompileContext );

namespace
{
	class raw_memory_ostream : public llvm::raw_ostream
	{
		XE::OMemoryStream & OS;

		void write_impl( const char * Ptr, size_t Size ) override
		{
			OS.write( Ptr, Size );
		}

		uint64_t current_pos() const override
		{
			return OS.tellp();
		}

	public:
		explicit raw_memory_ostream( XE::OMemoryStream & O ) : OS( O )
		{
			SetUnbuffered();
		}

		void reserveExtraSpace( uint64_t ExtraSize ) override
		{

		}
	};
}

void XE::ASTContext::AddMacro( const XE::String & val )
{
	_Macros.push_back( val );
}

bool XE::ASTContext::HasMacro( const XE::String & val ) const
{
	return std::find( _Macros.begin(), _Macros.end(), val ) != _Macros.end() ||
		XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->HasGlobalMacro( val );
}

bool XE::ASTContext::MacroSkip() const
{
	return !_MacroGo.empty() && _MacroGo.front() != XE::MacroGotoType::ELSE && _MacroGo.front() != XE::MacroGotoType::THEN_END;
}

void XE::ASTContext::PushMacroGotoType( XE::MacroGotoTypeFlags type )
{
	_MacroGo.push_front( type );
}

XE::MacroGotoTypeFlags & XE::ASTContext::TopMacroGotoType()
{
	return _MacroGo.front();
}

void XE::ASTContext::PopMacroGotoType()
{
	_MacroGo.pop_front();
}


XE::ASTExecuteContext * XE::ASTExecuteContext::ThreadInstance()
{
	thread_local XE::ASTExecuteContext context;
	return &context;
}

XE::MetaClassCPtr XE::ASTExecuteContext::GetVisitorBaseClass()
{
	return XE::ASTExecuteVisitor::GetMetaClassStatic();
}

XE::Variant XE::ASTExecuteContext::Invoke( const XE::ASTInfoMethodPtr & method, XE::InvokeStack * args )
{
	XE::ASTFrame frame;
	{
		frame.FP = 0;
		frame.SP = _ValStack.size();
		frame.AST = method;
	}

	_FrameStack.push_back( &frame );
	{
		while ( !args->Empty() )
		{
			Push( args->Pop< XE::Variant >() );
		}

		for ( const auto & it : method->LocalVariables )
		{
			frame.Variables.insert( { it.first, _ValStack.size() } );

			Push( it.second );
		}

		Exec();
	}
	_FrameStack.pop_back();

	return method->Result->Name.empty() ? XE::Variant() : Pop();
}

XE::Variant XE::ASTExecuteContext::Invoke( const XE::ASTInfoFunctionPtr & function, XE::InvokeStack * args )
{
	XE::ASTFrame frame;
	{
		frame.FP = 0;
		frame.SP = _ValStack.size();
		frame.AST = function;
	}

	_FrameStack.push_back( &frame );
	{
		while ( !args->Empty() )
		{
			Push( args->Pop< XE::Variant >() );
		}

		for ( const auto & it : function->LocalVariables )
		{
			frame.Variables.insert( { it.first, _ValStack.size() } );
			Push( it.second );
		}

		Exec();
	}
	_FrameStack.pop_back();

	return function->Result->Name.empty() ? XE::Variant() : Pop();
}

void XE::ASTExecuteContext::Push( const XE::Variant & val )
{
	_ValStack.push_back( val );
}

XE::Variant XE::ASTExecuteContext::Pop()
{
	XE::Variant val = _ValStack.back();
	_ValStack.pop_back();
	return val;
}

XE::Variant & XE::ASTExecuteContext::Top()
{
	return _ValStack.back();
}

XE::Variant & XE::ASTExecuteContext::Get( XE::uint64 val )
{
	return _ValStack[val];
}

XE::uint64 XE::ASTExecuteContext::Index() const
{
	return _ValStack.size();
}

void XE::ASTExecuteContext::Reset( XE::uint64 val )
{
	_ValStack.resize( val );
}

XE::ASTFrame * XE::ASTExecuteContext::GetFrame() const
{
	return _FrameStack.back();
}

void XE::ASTExecuteContext::PushLoopNode( const XE::WhileStatNode * node )
{
	_FrameStack.back()->Loop.push( node );
}

void XE::ASTExecuteContext::PopLoopNode()
{
	_FrameStack.back()->Loop.pop();
}

XE::ExecuteGotoType XE::ASTExecuteContext::GetExecGotoType() const
{
	return _FrameStack.back()->ExecGo;
}

void XE::ASTExecuteContext::SetExecGotoType( XE::ExecuteGotoType type )
{
	_FrameStack.back()->ExecGo = type;
}

void XE::ASTExecuteContext::Exec()
{
	if ( auto service = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
	{
		std::visit( XE::Overloaded
					{
						[&]( XE::ASTInfoMethodPtr func )
						{
							while ( _FrameStack.back()->FP < func->StatementBody.size() )
							{
								service->Visit( this, func->StatementBody[_FrameStack.back()->FP++] );

								if ( _FrameStack.back()->ExecGo == ExecuteGotoType::RETURN )
								{
									_FrameStack.back()->ExecGo = ExecuteGotoType::NONE;
									return;
								}
							}
						},
						[&]( XE::ASTInfoFunctionPtr func )
						{
							while ( _FrameStack.back()->FP < func->StatementBody.size() )
							{
								service->Visit( this, func->StatementBody[_FrameStack.back()->FP++] );

								if ( _FrameStack.back()->ExecGo == ExecuteGotoType::RETURN )
								{
									_FrameStack.back()->ExecGo = ExecuteGotoType::NONE;
									return;
								}
							}
						},
					}, _FrameStack.back()->AST );
	}
}

struct XE::ASTCompileContext::Type
{

};
struct XE::ASTCompileContext::Inst
{

};
struct XE::ASTCompileContext::Value
{

};
struct XE::ASTCompileContext::BasicBlock
{

};
struct XE::ASTCompileContext::Private
{
	llvm::LLVMContext * _Context;
	XE::OMemoryStream _Bytecodes;
	XE::Deque<llvm::BasicBlock *> _BasicBlocks;
};

XE::ASTCompileContext::ASTCompileContext()
	:_p( XE::New<Private>() )
{

}

XE::ASTCompileContext::~ASTCompileContext()
{
	XE::Delete( _p );
}

XE::ASTCompileContext * XE::ASTCompileContext::ThreadInstance()
{
	thread_local XE::ASTCompileContext context;
	return &context;
}

XE::MetaClassCPtr XE::ASTCompileContext::GetVisitorBaseClass()
{
	return XE::ASTCompileVisitor::GetMetaClassStatic();
}

XE::MemoryView XE::ASTCompileContext::Compile( const XE::ASTInfoMethodPtr & method )
{
	_p->_Bytecodes.clear();
	_p->_BasicBlocks.clear();

	llvm::LLVMContext context;
	_p->_Context = &context;

	// TODO: 
	llvm::Module module( "_Module", context );
	llvm::FunctionType * prototype = llvm::FunctionType::get( llvm::IntegerType::get( context, 32 ), llvm::PointerType::get( context, 0 ), false );
	llvm::Function * func = llvm::Function::Create( prototype, llvm::GlobalValue::ExternalLinkage, "_Function", module );
	llvm::BasicBlock * entry = llvm::BasicBlock::Create( *_p->_Context, "entry", func, nullptr );
	llvm::Value * params = func->arg_begin(); params->setName( "params" );

	_p->_BasicBlocks.push_front( entry );
	{
		if ( auto service = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
		{
			for ( auto it : method->StatementBody )
			{
				service->Visit( this, it );
			}
		}

		if ( method->Result == nullptr || method->Result->Name == ::ClassID<void>::Get()->GetFullName() )
		{
			llvm::ReturnInst::Create( context, _p->_BasicBlocks.front() );
		}
	}
	_p->_BasicBlocks.pop_front();

	{
		raw_memory_ostream oss( _p->_Bytecodes );
		llvm::WriteBitcodeToFile( module, oss );
	}

	return _p->_Bytecodes.view();
}

XE::MemoryView XE::ASTCompileContext::Compile( const XE::ASTInfoFunctionPtr & function )
{
	_p->_Bytecodes.clear();
	_p->_BasicBlocks.clear();

	llvm::LLVMContext context;
	_p->_Context = &context;

	// TODO: 
	llvm::Module module( "_Module", context );
	llvm::FunctionType * prototype = llvm::FunctionType::get( llvm::IntegerType::get( context, 32 ), llvm::PointerType::get( context, 0 ), false );
	llvm::Function * func = llvm::Function::Create( prototype, llvm::GlobalValue::ExternalLinkage, "_Function", module );
	llvm::BasicBlock * entry = llvm::BasicBlock::Create( *_p->_Context, "entry", func, nullptr );
	llvm::Value * params = func->arg_begin(); params->setName( "params" );

	_p->_BasicBlocks.push_front( entry );
	{
		if ( auto service = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
		{
			for ( auto it : function->StatementBody )
			{
				service->Visit( this, it );
			}
		}

		if ( function->Result == nullptr || function->Result->Name == ::ClassID<void>::Get()->GetFullName() )
		{
			llvm::ReturnInst::Create( context, _p->_BasicBlocks.front() );
		}
	}
	_p->_BasicBlocks.pop_front();

	{
		raw_memory_ostream oss( _p->_Bytecodes );
		llvm::WriteBitcodeToFile( module, oss );
	}

	return _p->_Bytecodes.view();
}
