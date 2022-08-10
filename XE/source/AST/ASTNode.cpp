#include "ASTNode.h"

#include "ASTVisitor.h"

IMPLEMENT_META( XE::ASTNode );
IMPLEMENT_META( XE::StatASTNode );
IMPLEMENT_META( XE::ExprStatNode );
BEG_META( XE::TypeASTNode )
END_META()
BEG_META( XE::IfStatNode )
END_META()
BEG_META( XE::BreakStatNode )
END_META()
BEG_META( XE::WhileStatNode )
END_META()
BEG_META( XE::SwitchStatNode )
END_META()
BEG_META( XE::ReturnStatNode )
END_META()
BEG_META( XE::ContinueStatNode )
END_META()
BEG_META( XE::ValueExprNode )
END_META()
BEG_META( XE::UnaryExprNode )
END_META()
BEG_META( XE::BinaryExprNode )
END_META()
BEG_META( XE::SizeofExprNode )
END_META()
BEG_META( XE::TypeofExprNode )
END_META()
BEG_META( XE::InvokeExprNode )
END_META()
BEG_META( XE::VariableExprNode )
END_META()

#define VISIT( TYPE ) void TYPE::Visit( XE::ASTVisitor * val ) const { val->Visit( this ); }
VISIT( XE::ASTNode );
VISIT( XE::StatASTNode );
VISIT( XE::ExprStatNode );
VISIT( XE::TypeASTNode )
VISIT( XE::IfStatNode )
VISIT( XE::BreakStatNode )
VISIT( XE::WhileStatNode )
VISIT( XE::SwitchStatNode )
VISIT( XE::ReturnStatNode )
VISIT( XE::ContinueStatNode )
VISIT( XE::ValueExprNode )
VISIT( XE::UnaryExprNode )
VISIT( XE::BinaryExprNode )
VISIT( XE::SizeofExprNode )
VISIT( XE::TypeofExprNode )
VISIT( XE::InvokeExprNode )
VISIT( XE::VariableExprNode )

const XE::String & XE::TypeASTNode::GetType() const
{
	return _Type;
}

void XE::TypeASTNode::SetType( const XE::String & val )
{
	_Type = val;
}

const XE::StatASTNodePtr & XE::IfStatNode::GetCondition() const
{
	return _Condition;
}

void XE::IfStatNode::SetCondition( const XE::StatASTNodePtr & val )
{
	_Condition = val;
}

const XE::StatASTNodePtr & XE::IfStatNode::GetTrue() const
{
	return _True;
}

void XE::IfStatNode::SetTrue( const XE::StatASTNodePtr & val )
{
	_True = val;
}

const XE::StatASTNodePtr & XE::IfStatNode::GetFalse() const
{
	return _False;
}

void XE::IfStatNode::SetFalse( const XE::StatASTNodePtr & val )
{
	_False = val;
}

const XE::StatASTNodePtr & XE::WhileStatNode::GetCondition() const
{
	return _Condition;
}

void XE::WhileStatNode::SetCondition( const XE::StatASTNodePtr & val )
{
	_Condition = val;
}

const XE::StatASTNodePtr & XE::WhileStatNode::GetStatement() const
{
	return _Statement;
}

void XE::WhileStatNode::SetStatement( const XE::StatASTNodePtr & val )
{
	_Statement = val;
}

const XE::ExprStatNodePtr & XE::SwitchStatNode::GetExpress() const
{
	return _Express;
}

void XE::SwitchStatNode::SetExpress( const XE::ExprStatNodePtr & val )
{
	_Express = val;
}

const XE::StatASTNodePtr & XE::SwitchStatNode::GetDefault() const
{
	return _Default;
}

void XE::SwitchStatNode::SetDefault( const XE::StatASTNodePtr & val )
{
	_Default = val;
}

const XE::Array< XE::StatASTNodePtr > & XE::SwitchStatNode::GetCases() const
{
	return _Cases;
}

void XE::SwitchStatNode::SetCases( const XE::Array< XE::StatASTNodePtr > & val )
{
	_Cases = val;
}

void XE::SwitchStatNode::AddCase( const XE::StatASTNodePtr & val )
{
	_Cases.push_back( val );
}

const XE::StatASTNodePtr & XE::ReturnStatNode::GetResult() const
{
	return _Result;
}

void XE::ReturnStatNode::SetResult( const XE::StatASTNodePtr & val )
{
	_Result = val;
}

const XE::Variant & XE::ValueExprNode::GetValue() const
{
	return _Value;
}

void XE::ValueExprNode::SetValue( const XE::Variant & val )
{
	_Value = val;
}

XE::UnaryExprType XE::UnaryExprNode::GetType() const
{
	return _Type;
}

void XE::UnaryExprNode::SetType( XE::UnaryExprType val )
{
	_Type = val;
}

const XE::ExprStatNodePtr & XE::UnaryExprNode::GetExpress() const
{
	return _Express;
}

void XE::UnaryExprNode::SetExpress( const XE::ExprStatNodePtr & val )
{
	_Express = val;
}

XE::BinaryExprType XE::BinaryExprNode::GetType() const
{
	return _Type;
}

void XE::BinaryExprNode::SetType( XE::BinaryExprType val )
{
	_Type = val;
}

const XE::ExprStatNodePtr & XE::BinaryExprNode::GetLeftExpress() const
{
	return _LeftExpress;
}

void XE::BinaryExprNode::SetLeftExpress( const XE::ExprStatNodePtr & val )
{
	_LeftExpress = val;
}

const XE::ExprStatNodePtr & XE::BinaryExprNode::GetRightExpress() const
{
	return _RightExpress;
}

void XE::BinaryExprNode::SetRightExpress( const XE::ExprStatNodePtr & val )
{
	_RightExpress = val;
}

const XE::TypeASTNodePtr & XE::SizeofExprNode::GetType() const
{
	return _Type;
}

void XE::SizeofExprNode::SetType( const XE::TypeASTNodePtr & val )
{
	_Type = val;
}

const XE::TypeASTNodePtr & XE::TypeofExprNode::GetType() const
{
	return _Type;
}

void XE::TypeofExprNode::SetType( const XE::TypeASTNodePtr & val )
{
	_Type = val;
}

const XE::String & XE::InvokeExprNode::GetName() const
{
	return _Name;
}

void XE::InvokeExprNode::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::Array< XE::ExprStatNodePtr > & XE::InvokeExprNode::GetArguments() const
{
	return _Arguments;
}

void XE::InvokeExprNode::SetArguments( const XE::Array< XE::ExprStatNodePtr > & val )
{
	_Arguments = val;
}

void XE::InvokeExprNode::AddArguments( const XE::ExprStatNodePtr & val )
{
	_Arguments.push_back( val );
}

const XE::String & XE::VariableExprNode::GetName() const
{
	return _Name;
}

void XE::VariableExprNode::SetName( const XE::String & val )
{
	_Name = val;
}
