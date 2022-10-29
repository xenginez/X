#include "ASTContext.h"

#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTNode.h"
#include "ASTVisitor.h"
#include "ASTService.h"

IMPLEMENT_META( XE::ASTContext );
IMPLEMENT_META( XE::ASTExecuteContext );
IMPLEMENT_META( XE::ASTCompileContext );

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

struct XE::ASTCompileContext::Private
{
	XE::OMemoryStream _Bytecodes;
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

	return _p->_Bytecodes.view();
}

XE::MemoryView XE::ASTCompileContext::Compile( const XE::ASTInfoFunctionPtr & function )
{
	_p->_Bytecodes.clear();

	return _p->_Bytecodes.view();
}
