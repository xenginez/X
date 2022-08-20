#include "ASTExecutor.h"

#include "Core/CoreFramework.h"

#include "ASTNode.h"
#include "ASTInfo.h"
#include "ASTService.h"

XE::ASTExecutor::ASTExecutor()
{

}

XE::ASTExecutor::~ASTExecutor()
{

}

void XE::ASTExecutor::Visit( const XE::ASTNode * val )
{
	XE_ASSERT( XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->ExecuteCustomNode( val->GetMetaClass(), this ) );
}

void XE::ASTExecutor::Visit( const XE::MacroIfASTNode * val )
{
	if ( MacroSkip() ) return;

	if ( std::find( _Macros.begin(), _Macros.end(), val->GetName() ) != _Macros.end() ||
		 XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->HasMacro( val->GetName() ) )
	{
		_FrameStack.back()->MacroGo.push( XE::MacroGotoType::THEN );
	}
	else
	{
		_FrameStack.back()->MacroGo.push( XE::MacroGotoType::ELSE );
	}
}

void XE::ASTExecutor::Visit( const XE::MacroElseASTNode * val )
{
	if ( _FrameStack.back()->MacroGo.top() == XE::MacroGotoType::THEN_END ) return;

	_FrameStack.back()->MacroGo.top() &= XE::MacroGotoType::END;
}

void XE::ASTExecutor::Visit( const XE::MacroElifASTNode * val )
{
	if ( _FrameStack.back()->MacroGo.top() == XE::MacroGotoType::THEN_END ) return;

	if ( std::find( _Macros.begin(), _Macros.end(), val->GetName() ) != _Macros.end() ||
		 XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->HasMacro( val->GetName() ) )
	{
		_FrameStack.back()->MacroGo.top() = XE::MacroGotoType::THEN;
	}
	else
	{
		_FrameStack.back()->MacroGo.top() = XE::MacroGotoType::ELSE;
	}
}

void XE::ASTExecutor::Visit( const XE::MacroEndASTNode * val )
{
	if ( _FrameStack.back()->MacroGo.top() == XE::MacroGotoType::ELSE ) return;

	_FrameStack.back()->MacroGo.pop();
}

void XE::ASTExecutor::Visit( const XE::IfStatNode * val )
{
	if ( MacroSkip() ) return;

	if ( val->GetCondition()->Visit( this ), Pop().ToBool() )
	{
		val->GetTrue()->Visit( this );
	}
	else
	{
		val->GetFalse()->Visit( this );
	}
}

void XE::ASTExecutor::Visit( const XE::BreakStatNode * val )
{
	if ( MacroSkip() ) return;

	_FrameStack.back()->ExecGo = ExecutorGotoType::BREAK;
}

void XE::ASTExecutor::Visit( const XE::WhileStatNode * val )
{
	if ( MacroSkip() ) return;

	_FrameStack.back()->Loop.push( val );
	{
		while ( true )
		{
			XE::uint64 i = Index();

			if ( val->GetCondition()->Visit( this ), Pop().ToBool() )
			{
				val->GetStatement()->Visit( this );
			}
			else
			{
				break;
			}

			Reset( i );

			if ( _FrameStack.back()->ExecGo == ExecutorGotoType::BREAK )
			{
				_FrameStack.back()->ExecGo = ExecutorGotoType::NONE;
				break;
			}
			else if ( _FrameStack.back()->ExecGo == ExecutorGotoType::RETURN )
			{
				break;
			}
			else if ( _FrameStack.back()->ExecGo != ExecutorGotoType::CONTINUE )
			{
				_FrameStack.back()->ExecGo = ExecutorGotoType::NONE;
				continue;
			}
		}
	}
	_FrameStack.back()->Loop.pop();
}

void XE::ASTExecutor::Visit( const XE::SwitchStatNode * val )
{
	if ( MacroSkip() ) return;

	val->GetExpress()->Visit( this );

	auto value = Pop();

	for ( const auto & it : val->GetCases() )
	{
		if ( it.first == value )
		{
			it.second->Visit( this );

			return;
		}
	}

	val->GetDefault()->Visit( this );
}

void XE::ASTExecutor::Visit( const XE::ReturnStatNode * val )
{
	if ( MacroSkip() ) return;

	val->GetResult()->Visit( this );
	_FrameStack.back()->ExecGo = ExecutorGotoType::RETURN;
}

void XE::ASTExecutor::Visit( const XE::ContinueStatNode * val )
{
	if ( MacroSkip() ) return;

	_FrameStack.back()->ExecGo = ExecutorGotoType::CONTINUE;
}

void XE::ASTExecutor::Visit( const XE::ValueExprNode * val )
{
	if ( MacroSkip() ) return;

	Push( val->GetValue() );
}

void XE::ASTExecutor::Visit( const XE::UnaryExprNode * val )
{
	if ( MacroSkip() ) return;

	val->GetExpress()->Visit( this );

	switch ( val->GetType() )
	{
	case XE::UnaryExprType::NOT:
		Push( !Pop() );
		break;
	case XE::UnaryExprType::NEG:
		Push( -Pop() );
		break;
	case XE::UnaryExprType::RMO:
		Push( ~Pop() );
		break;
	}
}

void XE::ASTExecutor::Visit( const XE::BinaryExprNode * val )
{
	if ( MacroSkip() ) return;

	val->GetLeftExpress()->Visit( this );
	auto left = Pop();

	val->GetRightExpress()->Visit( this );
	auto right = Pop();

	switch ( val->GetType() )
	{
	case XE::BinaryExprType::ADD:
		Push( left + right );
		break;
	case XE::BinaryExprType::SUB:
		Push( left - right );
		break;
	case XE::BinaryExprType::MUL:
		Push( left * right );
		break;
	case XE::BinaryExprType::DIV:
		Push( left / right );
		break;
	case XE::BinaryExprType::MOD:
		Push( left % right );
		break;
	case XE::BinaryExprType::L_OR:
		Push( left || right );
		break;
	case XE::BinaryExprType::L_AND:
		Push( left && right );
		break;
	case XE::BinaryExprType::B_OR:
		Push( left | right );
		break;
	case XE::BinaryExprType::B_XOR:
		Push( left ^ right );
		break;
	case XE::BinaryExprType::B_AND:
		Push( left & right );
		break;
	case XE::BinaryExprType::L_SHIFT:
		Push( left << right );
		break;
	case XE::BinaryExprType::R_SHIFT:
		Push( left >> right );
		break;
	case XE::BinaryExprType::LT:
		Push( left < right );
		break;
	case XE::BinaryExprType::GT:
		Push( left > right );
		break;
	case XE::BinaryExprType::EQUAL:
		Push( left == right );
		break;
	case XE::BinaryExprType::LT_EQUAL:
		Push( left <= right );
		break;
	case XE::BinaryExprType::GT_EQUAL:
		Push( left >= right );
		break;
	case XE::BinaryExprType::NOT_EQUAL:
		Push( left != right );
		break;
	case XE::BinaryExprType::INDEX:
		Push( left.ToArray()[right.ToUInt64()] );
		break;
	}
}

void XE::ASTExecutor::Visit( const XE::InvokeExprNode * val )
{
	if ( MacroSkip() ) return;

	XE::InvokeStack args;

	auto meta = XE::Reflection::FindMeta( val->GetName() );
	if ( meta->GetType() == XE::MetaInfoType::METHOD )
	{
		args.Push( Pop() );
	}

	for ( const auto & it : val->GetArguments() )
	{
		it->Visit( this );
		args.Push( Pop() );
	}

	if ( meta->GetType() == XE::MetaInfoType::METHOD )
	{
		SP_CAST< const XE::MetaMethod >( meta )->Invoke( &args );
	}
	else if ( meta->GetType() == XE::MetaInfoType::FUNCTION )
	{
		SP_CAST< const XE::MetaFunction >( meta )->Invoke( &args );
	}
}

void XE::ASTExecutor::Visit( const XE::VariableExprNode * val )
{
	if ( MacroSkip() ) return;

	Push( XE::Reflection::FindVariable( val->GetName() )->Get() );
}

void XE::ASTExecutor::AddMacro( const XE::String & val )
{
	_Macros.push_back( val );
}

XE::Variant XE::ASTExecutor::Invoke( const XE::ASTInfoMethodPtr & method, XE::InvokeStack * args )
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

XE::Variant XE::ASTExecutor::Invoke( const XE::ASTInfoFunctionPtr & function, XE::InvokeStack * args )
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

void XE::ASTExecutor::Push( const XE::Variant & val )
{
	_ValStack.push_back( val );
}

XE::Variant XE::ASTExecutor::Pop()
{
	XE::Variant val = _ValStack.back();
	_ValStack.pop_back();
	return val;
}

XE::Variant & XE::ASTExecutor::Top()
{
	return _ValStack.back();
}

XE::Variant & XE::ASTExecutor::Get( XE::uint64 val )
{
	return _ValStack[val];
}

XE::uint64 XE::ASTExecutor::Index() const
{
	return _ValStack.size();
}

void XE::ASTExecutor::Reset( XE::uint64 val )
{
	_ValStack.resize( val );
}

bool XE::ASTExecutor::MacroSkip() const
{
	if ( !_FrameStack.back()->MacroGo.empty() )
	{
		return _FrameStack.back()->MacroGo.top() == XE::MacroGotoType::ELSE || _FrameStack.back()->MacroGo.top() == XE::MacroGotoType::THEN_END;
	}

	return false;
}

XE::ASTFrame * XE::ASTExecutor::GetFrame() const
{
	return _FrameStack.back();
}

void XE::ASTExecutor::Exec()
{
	std::visit( XE::Overloaded
				{
					[this]( XE::ASTInfoMethodPtr func )
					{
						while ( _FrameStack.back()->FP < func->StatementBody.size() )
						{
							func->StatementBody[_FrameStack.back()->FP++]->Visit( this );

							if ( _FrameStack.back()->ExecGo == ExecutorGotoType::RETURN )
							{
								_FrameStack.back()->ExecGo = ExecutorGotoType::NONE;
								return;
							}
						}
					},
					[this]( XE::ASTInfoFunctionPtr func )
					{
						while ( _FrameStack.back()->FP < func->StatementBody.size() )
						{
							func->StatementBody[_FrameStack.back()->FP++]->Visit( this );

							if ( _FrameStack.back()->ExecGo == ExecutorGotoType::RETURN )
							{
								_FrameStack.back()->ExecGo = ExecutorGotoType::NONE;
								return;
							}
						}
					},
				}, _FrameStack.back()->AST );
}
