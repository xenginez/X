/*!
 * \file	ASTNode.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/09
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTNODE_H__9B9C419D_3C89_48EE_A06E_CC3EE8048C89
#define ASTNODE_H__9B9C419D_3C89_48EE_A06E_CC3EE8048C89

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTNode : public XE::Object
{
	OBJECT( ASTNode, XE::Object )

public:
	ASTNode() = default;

	~ASTNode() override = default;

public:
	virtual void Visit( const XE::ASTCodegenPtr & val ) const;
};

class XE_API TypeASTNode : public XE::ASTNode
{
	OBJECT( TypeASTNode, XE::ASTNode )

public:
	TypeASTNode() = default;

	~TypeASTNode() override = default;

public:
	const XE::MetaClassCPtr & GetType() const;

	void SetType( const XE::MetaClassCPtr & val );

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;

private:
	XE::MetaClassCPtr _Type;
};

class XE_API EnumASTNode : public XE::ASTNode
{
	OBJECT( EnumASTNode, XE::ASTNode )

public:
	EnumASTNode() = default;

	~EnumASTNode() override = default;

public:
	bool GetFlag() const;

	void SetFlag( bool val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const XE::Array< XE::String > & GetElements() const;

	void SetElements( const XE::Array< XE::String > & val );

public:
	bool AddElement( const XE::String & val );

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;

private:
	bool _Flag = false;
	XE::String _Name;
	XE::Array< XE::String > _Elements;
};

class XE_API StructASTNode : public XE::ASTNode
{
	OBJECT( StructASTNode, XE::ASTNode )

public:
	StructASTNode() = default;

	~StructASTNode() override = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const XE::Array< XE::Pair< XE::String, XE::Variant > > & GetElements() const;

	void SetElements( const XE::Array< XE::Pair< XE::String, XE::Variant > > & val );

public:
	void AddElement( const XE::String & name, const XE::Variant & val );

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;

private:
	XE::String _Name;
	XE::Array< XE::Pair< XE::String, XE::Variant > > _Elements;
};

class XE_API FunctionASTNode : public XE::ASTNode
{
	OBJECT( FunctionASTNode, XE::ASTNode )

public:
	FunctionASTNode() = default;

	~FunctionASTNode() override = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const XE::StatASTNodePtr & GetStatement() const;

	void SetStatement( const XE::StatASTNodePtr & val );

	const XE::Array< XE::TypeASTNodePtr > & GetResults() const;

	void SetResults( const XE::Array< XE::TypeASTNodePtr > & val );

	const XE::Array< XE::Pair< XE::String, XE::TypeASTNodePtr > > & GetParameter() const;

	void SetParameters( const XE::Array< XE::Pair< XE::String, XE::TypeASTNodePtr > > & val );

public:
	void AddResult( const XE::TypeASTNodePtr & val );

	void AddParameter( const XE::String & name, const XE::TypeASTNodePtr & type );

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;

private:
	XE::String _Name;
	XE::StatASTNodePtr _Statement;
	XE::Array< XE::TypeASTNodePtr > _Results;
	XE::Array< XE::Pair< XE::String, XE::TypeASTNodePtr > > _Parameters;
};

class XE_API StatASTNode : public XE::ASTNode
{
	OBJECT( StatASTNode, XE::ASTNode )

public:
	StatASTNode() = default;

	~StatASTNode() override = default;

public:
	virtual void Visit( const XE::ASTCodegenPtr & val ) const;
};

class XE_API IfStatNode : public XE::StatASTNode
{
	OBJECT( IfStatNode, XE::StatASTNode )

public:
	IfStatNode() = default;

	~IfStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API BlockStatNode : public XE::StatASTNode
{
	OBJECT( BlockStatNode, XE::StatASTNode )

public:
	BlockStatNode() = default;

	~BlockStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API BreakStatNode : public XE::StatASTNode
{
	OBJECT( BreakStatNode, XE::StatASTNode )

public:
	BreakStatNode() = default;

	~BreakStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API WhileStatNode : public XE::StatASTNode
{
	OBJECT( WhileStatNode, XE::StatASTNode )

public:
	WhileStatNode() = default;

	~WhileStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API SwitchStatNode : public XE::StatASTNode
{
	OBJECT( SwitchStatNode, XE::StatASTNode )

public:
	SwitchStatNode() = default;

	~SwitchStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API ReturnStatNode : public XE::StatASTNode
{
	OBJECT( ReturnStatNode, XE::StatASTNode )

public:
	ReturnStatNode() = default;

	~ReturnStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API ContinueStatNode : public XE::StatASTNode
{
	OBJECT( ContinueStatNode, XE::StatASTNode )

public:
	ContinueStatNode() = default;

	~ContinueStatNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API ExprASTNode : public XE::ASTNode
{
	OBJECT( ExprASTNode, XE::ASTNode )

public:
	ExprASTNode() = default;

	~ExprASTNode() override = default;

public:
	virtual void Visit( const XE::ASTCodegenPtr & val ) const;
};

class XE_API ValueExprNode : public XE::ExprASTNode
{
	OBJECT( ValueExprNode, XE::ExprASTNode )

public:
	ValueExprNode() = default;

	~ValueExprNode() override = default;

public:
	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;

private:
	XE::Variant _Value;
};

class XE_API UnaryExprNode : public XE::ExprASTNode
{
	OBJECT( UnaryExprNode, XE::ExprASTNode )

public:
	UnaryExprNode() = default;

	~UnaryExprNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API BinaryExprNode : public XE::ExprASTNode
{
	OBJECT( BinaryExprNode, XE::ExprASTNode )

public:
	BinaryExprNode() = default;

	~BinaryExprNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API SizeofExprNode : public XE::ExprASTNode
{
	OBJECT( SizeofExprNode, XE::ExprASTNode )

public:
	SizeofExprNode() = default;

	~SizeofExprNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API TypeofExprNode : public XE::ExprASTNode
{
	OBJECT( TypeofExprNode, XE::ExprASTNode )

public:
	TypeofExprNode() = default;

	~TypeofExprNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API InvokeExprNode : public XE::ExprASTNode
{
	OBJECT( InvokeExprNode, XE::ExprASTNode )

public:
	InvokeExprNode() = default;

	~InvokeExprNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

class XE_API VariableExprNode : public XE::ExprASTNode
{
	OBJECT( VariableExprNode, XE::ExprASTNode )

public:
	VariableExprNode() = default;

	~VariableExprNode() override = default;

public:
	void Visit( const XE::ASTCodegenPtr & val ) const override;
};

END_XE_NAMESPACE

#endif//ASTNODE_H__9B9C419D_3C89_48EE_A06E_CC3EE8048C89
