#include "ASTNode.h"

#include "ASTVisitor.h"

IMPLEMENT_META( XE::ASTNode );
BEG_META( XE::MacroIfASTNode )
type->Property( "Name", &XE::MacroIfASTNode::_Name );
END_META()
IMPLEMENT_META( XE::MacroElseASTNode );
BEG_META( XE::MacroElifASTNode )
type->Property( "Name", &XE::MacroElifASTNode::_Name );
END_META()
IMPLEMENT_META( XE::MacroEndASTNode );
IMPLEMENT_META( XE::StatASTNode );
IMPLEMENT_META( XE::ExprStatNode );
BEG_META( XE::IfStatNode )
type->Property( "Condition", &XE::IfStatNode::_Condition );
type->Property( "True", &XE::IfStatNode::_True );
type->Property( "False", &XE::IfStatNode::_False );
END_META()
IMPLEMENT_META( XE::BreakStatNode );
BEG_META( XE::WhileStatNode )
type->Property( "Condition", &XE::WhileStatNode::_Condition );
type->Property( "Statement", &XE::WhileStatNode::_Statement );
END_META()
BEG_META( XE::SwitchStatNode )
type->Property( "Express", &XE::SwitchStatNode::_Express );
type->Property( "Cases", &XE::SwitchStatNode::_Cases );
type->Property( "Default", &XE::SwitchStatNode::_Default );
END_META()
BEG_META( XE::ReturnStatNode )
type->Property( "Result", &XE::ReturnStatNode::_Result );
END_META()
IMPLEMENT_META( XE::ContinueStatNode );
BEG_META( XE::ValueExprNode )
type->Property( "Value", &XE::ValueExprNode::_Value );
END_META()
BEG_META( XE::UnaryExprNode )
type->Property( "Type", &XE::UnaryExprNode::_Type );
type->Property( "Express", &XE::UnaryExprNode::_Express );
END_META()
BEG_META( XE::BinaryExprNode )
type->Property( "Type", &XE::BinaryExprNode::_Type );
type->Property( "Left", &XE::BinaryExprNode::_LeftExpress );
type->Property( "Right", &XE::BinaryExprNode::_RightExpress );
END_META()
BEG_META( XE::InvokeExprNode )
type->Property( "Name", &XE::InvokeExprNode::_Name );
type->Property( "Arguments", &XE::InvokeExprNode::_Arguments );
END_META()
BEG_META( XE::VariableExprNode )
type->Property( "Name", &XE::VariableExprNode::_Name );
END_META()

//////////////////////////////////////////////////////////////////////////

#define VISIT( TYPE ) void TYPE::Visit( XE::ASTVisitor * val ) const { val->Visit( this ); }
VISIT( XE::ASTNode );
VISIT( XE::MacroIfASTNode );
VISIT( XE::MacroElseASTNode );
VISIT( XE::MacroElifASTNode );
VISIT( XE::MacroEndASTNode );
VISIT( XE::StatASTNode );
VISIT( XE::ExprStatNode );
VISIT( XE::IfStatNode );
VISIT( XE::BreakStatNode );
VISIT( XE::WhileStatNode );
VISIT( XE::SwitchStatNode );
VISIT( XE::ReturnStatNode );
VISIT( XE::ContinueStatNode );
VISIT( XE::ValueExprNode );
VISIT( XE::UnaryExprNode );
VISIT( XE::BinaryExprNode );
VISIT( XE::InvokeExprNode );
VISIT( XE::VariableExprNode );
#undef VISIT

//////////////////////////////////////////////////////////////////////////

const XE::String & XE::MacroIfASTNode::GetName() const
{
	return _Name;
}

void XE::MacroIfASTNode::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::String & XE::MacroElifASTNode::GetName() const
{
	return _Name;
}

void XE::MacroElifASTNode::SetName( const XE::String & val )
{
	_Name = val;
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

const XE::Array< XE::Pair< XE::Variant, XE::StatASTNodePtr > > & XE::SwitchStatNode::GetCases() const
{
	return _Cases;
}

void XE::SwitchStatNode::SetCases( const XE::Array< XE::Pair< XE::Variant, XE::StatASTNodePtr > > & val )
{
	_Cases = val;
}

void XE::SwitchStatNode::AddCase( const XE::Variant & val, const XE::StatASTNodePtr & stat )
{
	_Cases.push_back( { val, stat } );
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
