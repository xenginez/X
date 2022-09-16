#include "ASTVisitor.h"

#include "Core/CoreFramework.h"

#include "ASTNode.h"
#include "ASTContext.h"
#include "ASTService.h"

#define VISIT_META( TYPE ) \
BEG_META( TYPE ) \
type->Method( "Visit", &TYPE::Visit ); \
END_META()

IMPLEMENT_META( XE::ASTExecuteVisitor );
VISIT_META( XE::MacroIfNodeExecuteVisitor );
VISIT_META( XE::MacroElseNodeExecuteVisitor );
VISIT_META( XE::MacroElifNodeExecuteVisitor );
VISIT_META( XE::MacroEndNodeExecuteVisitor );
VISIT_META( XE::IfStatNodeExecuteVisitor );
VISIT_META( XE::BreakStatNodeExecuteVisitor );
VISIT_META( XE::WhileStatNodeExecuteVisitor );
VISIT_META( XE::SwitchStatNodeExecuteVisitor );
VISIT_META( XE::ReturnStatNodeExecuteVisitor );
VISIT_META( XE::ContinueStatNodeExecuteVisitor );
VISIT_META( XE::ValueExprNodeExecuteVisitor );
VISIT_META( XE::UnaryExprNodeExecuteVisitor );
VISIT_META( XE::BinaryExprNodeExecuteVisitor );
VISIT_META( XE::InvokeExprNodeExecuteVisitor );
VISIT_META( XE::VariableExprNodeExecuteVisitor );

IMPLEMENT_META( XE::ASTCompileVisitor );
VISIT_META( XE::MacroIfNodeCompileVisitor );
VISIT_META( XE::MacroElseNodeCompileVisitor );
VISIT_META( XE::MacroElifNodeCompileVisitor );
VISIT_META( XE::MacroEndNodeCompileVisitor );
VISIT_META( XE::IfStatNodeCompileVisitor );
VISIT_META( XE::BreakStatNodeCompileVisitor );
VISIT_META( XE::WhileStatNodeCompileVisitor );
VISIT_META( XE::SwitchStatNodeCompileVisitor );
VISIT_META( XE::ReturnStatNodeCompileVisitor );
VISIT_META( XE::ContinueStatNodeCompileVisitor );
VISIT_META( XE::ValueExprNodeCompileVisitor );
VISIT_META( XE::UnaryExprNodeCompileVisitor );
VISIT_META( XE::BinaryExprNodeCompileVisitor );
VISIT_META( XE::InvokeExprNodeCompileVisitor );
VISIT_META( XE::VariableExprNodeCompileVisitor );

#undef VISIT_META


XE::ASTServicePtr XE::ASTVisitor::GetService() const
{
	return XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >();
}


void XE::MacroIfNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::MacroIfNode * node ) const
{
	if ( context->MacroSkip() ) return;

	if ( context->HasMacro( node->GetName() ) )
	{
		context->PushMacroGotoType( XE::MacroGotoType::THEN );
	}
	else
	{
		context->PushMacroGotoType( XE::MacroGotoType::ELSE );
	}
}

void XE::MacroElseNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::MacroElseNode * node ) const
{
	if ( context->TopMacroGotoType() == XE::MacroGotoType::THEN_END ) return;

	context->TopMacroGotoType() &= XE::MacroGotoType::END;
}

void XE::MacroElifNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::MacroElifNode * node ) const
{
	if ( context->TopMacroGotoType() == XE::MacroGotoType::THEN_END ) return;

	if ( context->HasMacro( node->GetName() ) )
	{
		context->TopMacroGotoType() = XE::MacroGotoType::THEN;
	}
	else
	{
		context->TopMacroGotoType() = XE::MacroGotoType::ELSE;
	}
}

void XE::MacroEndNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::MacroEndNode * node ) const
{
	if ( context->TopMacroGotoType() == XE::MacroGotoType::ELSE ) return;

	context->PopMacroGotoType();
}

void XE::IfStatNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::IfStatNode * node ) const
{
	if ( context->MacroSkip() ) return;
	
	GetService()->Visit( context, node->GetCondition() );

	if ( context->Pop().ToBool() )
	{
		GetService()->Visit( context, node->GetTrue() );
	}
	else
	{
		GetService()->Visit( context, node->GetFalse() );
	}
}

void XE::BreakStatNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::BreakStatNode * node ) const
{
	if ( context->MacroSkip() ) return;

	context->SetExecGotoType( XE::ExecuteGotoType::BREAK );
}

void XE::WhileStatNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::WhileStatNode * node ) const
{
	if ( context->MacroSkip() ) return;

	context->PushLoopNode( node );
	while ( true )
	{
		XE::uint64 i = context->Index();

		GetService()->Visit( context, node->GetCondition() );

		if ( context->Pop().ToBool() )
		{
			GetService()->Visit( context, node->GetStatement() );
		}
		else
		{
			break;
		}

		context->Reset( i );

		if ( context->GetExecGotoType() == XE::ExecuteGotoType::BREAK )
		{
			context->SetExecGotoType( XE::ExecuteGotoType::NONE );
			break;
		}
		else if ( context->GetExecGotoType() == XE::ExecuteGotoType::RETURN )
		{
			break;
		}
		else if ( context->GetExecGotoType() != XE::ExecuteGotoType::CONTINUE )
		{
			context->SetExecGotoType( XE::ExecuteGotoType::NONE );
			continue;
		}
	}
	context->PopLoopNode();
}

void XE::SwitchStatNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::SwitchStatNode * node ) const
{
	if ( context->MacroSkip() ) return;

	GetService()->Visit( context, node->GetExpress() );

	auto value = context->Pop();

	for ( const auto & it : node->GetCases() )
	{
		if ( it.first == value )
		{
			GetService()->Visit( context, it.second );

			return;
		}
	}

	if ( node->GetDefault() )
	{
		GetService()->Visit( context, node->GetDefault() );
	}
}

void XE::ReturnStatNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::ReturnStatNode * node ) const
{
	if ( context->MacroSkip() ) return;

	GetService()->Visit( context, node->GetResult() );

	context->SetExecGotoType( XE::ExecuteGotoType::RETURN );
}

void XE::ContinueStatNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::ContinueStatNode * node ) const
{
	if ( context->MacroSkip() ) return;

	context->SetExecGotoType( XE::ExecuteGotoType::CONTINUE );
}

void XE::ValueExprNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::ValueExprNode * node ) const
{
	if ( context->MacroSkip() ) return;

	context->Push( node->GetValue() );
}

void XE::UnaryExprNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::UnaryExprNode * node ) const
{
	if ( context->MacroSkip() ) return;

	GetService()->Visit( context, node->GetExpress() );

	switch ( node->GetType() )
	{
	case XE::UnaryExprType::NOT:
		context->Push( !context->Pop() );
		break;
	case XE::UnaryExprType::NEG:
		context->Push( -context->Pop() );
		break;
	case XE::UnaryExprType::RMO:
		context->Push( ~context->Pop() );
		break;
	}
}

void XE::BinaryExprNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::BinaryExprNode * node ) const
{
	if ( context->MacroSkip() ) return;

	GetService()->Visit( context, node->GetLeftExpress() );
	auto left = context->Pop();

	GetService()->Visit( context, node->GetRightExpress() );
	auto right = context->Pop();

	switch ( node->GetType() )
	{
	case XE::BinaryExprType::ADD:
		context->Push( left + right );
		break;
	case XE::BinaryExprType::SUB:
		context->Push( left - right );
		break;
	case XE::BinaryExprType::MUL:
		context->Push( left * right );
		break;
	case XE::BinaryExprType::DIV:
		context->Push( left / right );
		break;
	case XE::BinaryExprType::MOD:
		context->Push( left % right );
		break;
	case XE::BinaryExprType::L_OR:
		context->Push( left || right );
		break;
	case XE::BinaryExprType::L_AND:
		context->Push( left && right );
		break;
	case XE::BinaryExprType::B_OR:
		context->Push( left | right );
		break;
	case XE::BinaryExprType::B_XOR:
		context->Push( left ^ right );
		break;
	case XE::BinaryExprType::B_AND:
		context->Push( left & right );
		break;
	case XE::BinaryExprType::L_SHIFT:
		context->Push( left << right );
		break;
	case XE::BinaryExprType::R_SHIFT:
		context->Push( left >> right );
		break;
	case XE::BinaryExprType::LT:
		context->Push( left < right );
		break;
	case XE::BinaryExprType::GT:
		context->Push( left > right );
		break;
	case XE::BinaryExprType::EQUAL:
		context->Push( left == right );
		break;
	case XE::BinaryExprType::LT_EQUAL:
		context->Push( left <= right );
		break;
	case XE::BinaryExprType::GT_EQUAL:
		context->Push( left >= right );
		break;
	case XE::BinaryExprType::NOT_EQUAL:
		context->Push( left != right );
		break;
	case XE::BinaryExprType::INDEX:
		context->Push( left.ToArray()[right.ToUInt64()] );
		break;
	}
}

void XE::InvokeExprNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::InvokeExprNode * node ) const
{
	if ( context->MacroSkip() ) return;

	XE::InvokeStack args;

	auto meta = XE::Reflection::FindMeta( node->GetName() );
	if ( meta->GetType() == XE::MetaInfoType::METHOD )
	{
		args.Push( context->Pop() );
	}

	for ( const auto & it : node->GetArguments() )
	{
		GetService()->Visit( context, it );

		args.Push( context->Pop() );
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

void XE::VariableExprNodeExecuteVisitor::Visit( XE::ASTExecuteContext * context, const XE::VariableExprNode * node ) const
{
	if ( context->MacroSkip() ) return;

	context->Push( XE::Reflection::FindVariable( node->GetName() )->Get() );
}


void XE::MacroIfNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::MacroIfNode * node ) const
{

}

void XE::MacroElseNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::MacroElseNode * node ) const
{

}

void XE::MacroElifNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::MacroElifNode * node ) const
{

}

void XE::MacroEndNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::MacroEndNode * node ) const
{

}

void XE::IfStatNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::IfStatNode * node ) const
{

}

void XE::BreakStatNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::BreakStatNode * node ) const
{

}

void XE::WhileStatNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::WhileStatNode * node ) const
{

}

void XE::SwitchStatNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::SwitchStatNode * node ) const
{

}

void XE::ReturnStatNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::ReturnStatNode * node ) const
{

}

void XE::ContinueStatNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::ContinueStatNode * node ) const
{

}

void XE::ValueExprNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::ValueExprNode * node ) const
{

}

void XE::UnaryExprNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::UnaryExprNode * node ) const
{

}

void XE::BinaryExprNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::BinaryExprNode * node ) const
{

}

void XE::InvokeExprNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::InvokeExprNode * node ) const
{

}

void XE::VariableExprNodeCompileVisitor::Visit( XE::ASTCompileContext * context, const XE::VariableExprNode * node ) const
{

}
