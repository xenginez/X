#include "ASTExecutor.h"

#include "ASTNode.h"

namespace
{
	enum class GotoType
	{
		NONE,
		BREAK,
		RETURN,
		CONTINUE,
	};

	struct Frame
	{
		XE::uint64 SP = 0;
		XE::uint64 FP = 0;
		GotoType Go = GotoType::NONE;
		XE::Stack< const XE::WhileStatNode * > Loop;
		XE::Map< XE::String, XE::uint64 > Variables;
		std::variant< XE::ASTMethod *, XE::ASTFunction * > AST;
	};
}

struct XE::ASTExecutor::Private
{
	XE::Array< XE::Variant > _Stack;
	XE::Array< Frame * > _FrameStack;
};

XE::ASTExecutor * XE::ASTExecutor::GetInstance()
{
	thread_local XE::ASTExecutor exec;
	return &exec;
}

XE::ASTExecutor::ASTExecutor()
	:_p( XE::New< Private >() )
{

}

XE::ASTExecutor::~ASTExecutor()
{
	XE::Delete( _p );
}

XE::Variant XE::ASTExecutor::Invoke( const XE::SharedPtr< XE::ASTMethod > & method, XE::InvokeStack * args )
{
	Frame frame;
	{
		frame.FP = 0;
		frame.SP = GetInstance()->_p->_Stack.size();
		frame.AST = method.get();
	}

	GetInstance()->_p->_FrameStack.push_back( &frame );
	{
		while ( !args->Empty() )
		{
			GetInstance()->Push( args->Pop< XE::Variant >() );
		}

		for ( const auto & it : method->LocalVariables )
		{
			frame.Variables.insert( { it.first, GetInstance()->_p->_Stack.size() } );

			GetInstance()->Push( it.second );
		}

		GetInstance()->Exec();
	}
	GetInstance()->_p->_FrameStack.pop_back();

	return method->Result.empty() ? XE::Variant() : GetInstance()->Pop();
}

XE::Variant XE::ASTExecutor::Invoke( const XE::SharedPtr< XE::ASTFunction > & function, XE::InvokeStack * args )
{
	Frame frame;
	{
		frame.FP = 0;
		frame.SP = GetInstance()->_p->_Stack.size();
		frame.AST = function.get();
	}

	GetInstance()->_p->_FrameStack.push_back( &frame );
	{
		while ( !args->Empty() )
		{
			GetInstance()->Push( args->Pop< XE::Variant >() );
		}

		for ( const auto & it : function->LocalVariables )
		{
			frame.Variables.insert( { it.first, GetInstance()->_p->_Stack.size() } );
			GetInstance()->Push( it.second );
		}

		GetInstance()->Exec();
	}
	GetInstance()->_p->_FrameStack.pop_back();

	return function->Result.empty() ? XE::Variant() : GetInstance()->Pop();
}

void XE::ASTExecutor::Exec()
{
	std::visit( XE::Overloaded
				{
					[this]( XE::ASTMethod * func )
					{
						while ( _p->_FrameStack.back()->FP < func->StatementBody.size() )
						{
							func->StatementBody[_p->_FrameStack.back()->FP++]->Visit( this );

							if ( _p->_FrameStack.back()->Go == GotoType::RETURN )
							{
								_p->_FrameStack.back()->Go = GotoType::NONE;
								return;
							}
						}
					},
					[this]( XE::ASTFunction * func )
					{
						while ( _p->_FrameStack.back()->FP < func->StatementBody.size() )
						{
							func->StatementBody[_p->_FrameStack.back()->FP++]->Visit( this );

							if ( _p->_FrameStack.back()->Go == GotoType::RETURN )
							{
								_p->_FrameStack.back()->Go = GotoType::NONE;
								return;
							}
						}
					},
				}, _p->_FrameStack.back()->AST );
}

void XE::ASTExecutor::Push( const XE::Variant & val )
{
	_p->_Stack.push_back( val );
}

XE::Variant XE::ASTExecutor::Pop()
{
	XE::Variant val = _p->_Stack.back();
	_p->_Stack.pop_back();
	return val;
}

XE::Variant & XE::ASTExecutor::Top()
{
	return _p->_Stack.back();
}

XE::Variant & XE::ASTExecutor::Get( XE::uint64 val )
{
	return _p->_Stack[val];
}

XE::uint64 XE::ASTExecutor::Index() const
{
	return _p->_Stack.size();
}

void XE::ASTExecutor::Reset( XE::uint64 val )
{
	_p->_Stack.resize( val );
}

void XE::ASTExecutor::Visit( const XE::ASTNode * val )
{
	XE_ASSERT( false );
}

void XE::ASTExecutor::Visit( const XE::IfStatNode * val )
{
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
	_p->_FrameStack.back()->Go = GotoType::BREAK;
}

void XE::ASTExecutor::Visit( const XE::WhileStatNode * val )
{
	_p->_FrameStack.back()->Loop.push( val );
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

			if ( _p->_FrameStack.back()->Go == GotoType::BREAK )
			{
				_p->_FrameStack.back()->Go = GotoType::NONE;
				break;
			}
			else if ( _p->_FrameStack.back()->Go == GotoType::RETURN )
			{
				break;
			}
			else if ( _p->_FrameStack.back()->Go != GotoType::CONTINUE )
			{
				_p->_FrameStack.back()->Go = GotoType::NONE;
				continue;
			}
		}
	}
	_p->_FrameStack.back()->Loop.pop();
}

void XE::ASTExecutor::Visit( const XE::SwitchStatNode * val )
{
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
	val->GetResult()->Visit( this );
	_p->_FrameStack.back()->Go = GotoType::RETURN;
}

void XE::ASTExecutor::Visit( const XE::ContinueStatNode * val )
{
	_p->_FrameStack.back()->Go = GotoType::CONTINUE;
}

void XE::ASTExecutor::Visit( const XE::ValueExprNode * val )
{
	Push( val->GetValue() );
}

void XE::ASTExecutor::Visit( const XE::UnaryExprNode * val )
{
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
	Push( XE::Reflection::FindVariable( val->GetName() )->Get() );
}
