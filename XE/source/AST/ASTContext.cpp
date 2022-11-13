#include "ASTContext.h"

#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTNode.h"
#include "ASTVisitor.h"
#include "ASTService.h"
#include "ASTInstance.h"

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


XE::MetaClassCPtr XE::ASTExecuteContext::GetVisitorBaseClass()
{
	return XE::ASTExecuteVisitor::GetMetaClassStatic();
}

XE::Variant XE::ASTExecuteContext::Invoke( const XE::ASTInstancePtr & env, const XE::ASTInfoFunctionPtr & func, XE::InvokeStack * args )
{
	_Frame.FP = 0;
	_Frame.SP = env->GetStackPoint();
	_Frame.AST = func;
	_Frame.ExecGo = XE::ExecuteGotoType::NONE;

	{
		while ( !args->Empty() )
		{
			Push( args->Pop< XE::Variant >() );
		}

		for ( const auto & it : func->LocalVariables )
		{
			_Frame.Variables.insert( { it.first, env->PushStack( it.second ) } );
		}

		Exec();
	}

	return func->Result->Name.empty() ? XE::Variant() : Pop();
}

void XE::ASTExecuteContext::Push( const XE::Variant & val )
{
	_Instance->PushStack( val );
}

XE::Variant XE::ASTExecuteContext::Pop()
{
	return _Instance->PopStack();
}

XE::Variant & XE::ASTExecuteContext::Top()
{
	return _Instance->TopStack();
}

XE::Variant & XE::ASTExecuteContext::Get( XE::uint64 val )
{
	return _Instance->GetStack( val );
}

XE::uint64 XE::ASTExecuteContext::Index() const
{
	return _Instance->GetStackPoint();
}

void XE::ASTExecuteContext::Reset( XE::uint64 i )
{
	_Instance->ResetStack( i );
}

void XE::ASTExecuteContext::PushLoopNode( const XE::WhileStatNode * node )
{
	_Frame.Loop.push( node );
}

void XE::ASTExecuteContext::PopLoopNode()
{
	_Frame.Loop.pop();
}

XE::ExecuteGotoType XE::ASTExecuteContext::GetExecGotoType() const
{
	return _Frame.ExecGo;
}

void XE::ASTExecuteContext::SetExecGotoType( XE::ExecuteGotoType type )
{
	_Frame.ExecGo = type;
}

void XE::ASTExecuteContext::Exec()
{
	if ( auto service = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >() )
	{
		while ( _Frame.FP < _Frame.AST->StatementBody.size() )
		{
			service->Visit( this, _Frame.AST->StatementBody[_Frame.FP++] );

			if ( _Frame.ExecGo == ExecuteGotoType::RETURN )
			{
				_Frame.ExecGo = ExecuteGotoType::NONE;
				return;
			}
		}
	}
}


XE::ASTCompileContext::ASTCompileContext()
	:_Bytecodes( XE::MemoryResource::GetFrameMemoryResource() )
{

}

XE::ASTCompileContext::~ASTCompileContext()
{

}

XE::MetaClassCPtr XE::ASTCompileContext::GetVisitorBaseClass()
{
	return XE::ASTCompileVisitor::GetMetaClassStatic();
}

XE::MemoryView XE::ASTCompileContext::Compile( const XE::ASTInfoModulePtr & module )
{
	_Bytecodes.clear();
	{

	}
	return _Bytecodes.view();
}
