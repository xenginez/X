#include "ASTNode.h"

#include "ASTCodegen.h"

IMPLEMENT_META( XE::ASTNode );
IMPLEMENT_META( XE::StatASTNode );
IMPLEMENT_META( XE::ExprASTNode );
BEG_META( XE::TypeASTNode )
END_META()
BEG_META( XE::EnumASTNode )
END_META()
BEG_META( XE::StructASTNode )
END_META()
BEG_META( XE::FunctionASTNode )
END_META()
BEG_META( IfStatNode )
END_META()
BEG_META( BlockStatNode )
END_META()
BEG_META( BreakStatNode )
END_META()
BEG_META( WhileStatNode )
END_META()
BEG_META( SwitchStatNode )
END_META()
BEG_META( ReturnStatNode )
END_META()
BEG_META( ContinueStatNode )
END_META()
BEG_META( ValueExprNode )
END_META()
BEG_META( UnaryExprNode )
END_META()
BEG_META( BinaryExprNode )
END_META()
BEG_META( SizeofExprNode )
END_META()
BEG_META( TypeofExprNode )
END_META()
BEG_META( InvokeExprNode )
END_META()
BEG_META( VariableExprNode )
END_META()


void XE::ASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

const XE::MetaClassCPtr & XE::TypeASTNode::GetType() const
{
	return _Type;
}

void XE::TypeASTNode::SetType( const XE::MetaClassCPtr & val )
{
	_Type = val;
}

void XE::TypeASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

bool XE::EnumASTNode::GetFlag() const
{
	return _Flag;
}

void XE::EnumASTNode::SetFlag( bool val )
{
	_Flag = val;
}

const XE::String & XE::EnumASTNode::GetName() const
{
	return _Name;
}

void XE::EnumASTNode::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::Array< XE::String > & XE::EnumASTNode::GetElements() const
{
	return _Elements;
}

void XE::EnumASTNode::SetElements( const XE::Array< XE::String > & val )
{
	_Elements = val;
}

bool XE::EnumASTNode::AddElement( const XE::String & val )
{
	if ( _Flag && _Elements.size() == 64 )
	{
		return false;
	}

	_Elements.push_back( val );

	return true;
}

void XE::EnumASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

const XE::String & XE::StructASTNode::GetName() const
{
	return _Name;
}

void XE::StructASTNode::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::Array< XE::Pair< XE::String, XE::Variant > > & XE::StructASTNode::GetElements() const
{
	return _Elements;
}

void XE::StructASTNode::SetElements( const XE::Array< XE::Pair< XE::String, XE::Variant > > & val )
{
	_Elements = val;
}

void XE::StructASTNode::AddElement( const XE::String & name, const XE::Variant & val )
{
	_Elements.push_back( { name, val } );
}

void XE::StructASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

const XE::String & XE::FunctionASTNode::GetName() const
{
	return _Name;
}

void XE::FunctionASTNode::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::StatASTNodePtr & XE::FunctionASTNode::GetStatement() const
{
	return _Statement;
}

void XE::FunctionASTNode::SetStatement( const XE::StatASTNodePtr & val )
{
	_Statement = val;
}

const XE::Array< XE::TypeASTNodePtr > & XE::FunctionASTNode::GetResults() const
{
	return _Results;
}

void XE::FunctionASTNode::SetResults( const XE::Array< XE::TypeASTNodePtr > & val )
{
	_Results = val;
}

const XE::Array< XE::Pair< XE::String, XE::TypeASTNodePtr > > & XE::FunctionASTNode::GetParameter() const
{
	return _Parameters;
}

void XE::FunctionASTNode::SetParameters( const XE::Array< XE::Pair< XE::String, XE::TypeASTNodePtr > > & val )
{
	_Parameters = val;
}

void XE::FunctionASTNode::AddResult( const XE::TypeASTNodePtr & val )
{
	_Results.push_back( val );
}

void XE::FunctionASTNode::AddParameter( const XE::String & name, const XE::TypeASTNodePtr & type )
{
	_Parameters.push_back( { name, type } );
}

void XE::FunctionASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::StatASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::IfStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::BlockStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::BreakStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::WhileStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::SwitchStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::ReturnStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::ContinueStatNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::ExprASTNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

const XE::Variant & XE::ValueExprNode::GetValue() const
{
	return _Value;
}

void XE::ValueExprNode::SetValue( const XE::Variant & val )
{
	_Value = val;
}

void XE::ValueExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::UnaryExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::BinaryExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::SizeofExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::TypeofExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::InvokeExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}

void XE::VariableExprNode::Visit( const XE::ASTCodegenPtr & val ) const
{
	val->Visit( this );
}
