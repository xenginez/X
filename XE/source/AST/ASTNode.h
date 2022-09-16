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
};

class XE_API MacroIfNode : public XE::ASTNode
{
	OBJECT( MacroIfNode, XE::ASTNode )

public:
	MacroIfNode() = default;

	~MacroIfNode() override = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

private:
	XE::String _Name;
};

class XE_API MacroElseNode : public XE::ASTNode
{
	OBJECT( MacroElseNode, XE::ASTNode )

public:
	MacroElseNode() = default;

	~MacroElseNode() override = default;
};

class XE_API MacroElifNode : public XE::ASTNode
{
	OBJECT( MacroElifNode, XE::ASTNode )

public:
	MacroElifNode() = default;

	~MacroElifNode() override = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

private:
	XE::String _Name;
};

class XE_API MacroEndNode : public XE::ASTNode
{
	OBJECT( MacroEndNode, XE::ASTNode )

public:
	MacroEndNode() = default;

	~MacroEndNode() override = default;
};

class XE_API StatASTNode : public XE::ASTNode
{
	OBJECT( StatASTNode, XE::ASTNode )

public:
	StatASTNode() = default;

	~StatASTNode() override = default;
};

class XE_API ExprStatNode : public XE::StatASTNode
{
	OBJECT( ExprStatNode, XE::StatASTNode )

public:
	ExprStatNode() = default;

	~ExprStatNode() override = default;
};

class XE_API IfStatNode : public XE::StatASTNode
{
	OBJECT( IfStatNode, XE::StatASTNode )

public:
	IfStatNode() = default;

	~IfStatNode() override = default;

public:
	const XE::StatASTNodePtr & GetCondition() const;

	void SetCondition( const XE::StatASTNodePtr & val );

	const XE::StatASTNodePtr & GetTrue() const;

	void SetTrue( const XE::StatASTNodePtr & val );

	const XE::StatASTNodePtr & GetFalse() const;

	void SetFalse( const XE::StatASTNodePtr & val );

private:
	XE::StatASTNodePtr _Condition;
	XE::StatASTNodePtr _True;
	XE::StatASTNodePtr _False;
};

class XE_API BreakStatNode : public XE::StatASTNode
{
	OBJECT( BreakStatNode, XE::StatASTNode )

public:
	BreakStatNode() = default;

	~BreakStatNode() override = default;
};

class XE_API WhileStatNode : public XE::StatASTNode
{
	OBJECT( WhileStatNode, XE::StatASTNode )

public:
	WhileStatNode() = default;

	~WhileStatNode() override = default;

public:
	const XE::StatASTNodePtr & GetCondition() const;

	void SetCondition( const XE::StatASTNodePtr & val );

	const XE::StatASTNodePtr & GetStatement() const;

	void SetStatement( const XE::StatASTNodePtr & val );

private:
	XE::StatASTNodePtr _Condition;
	XE::StatASTNodePtr _Statement;
};

class XE_API SwitchStatNode : public XE::StatASTNode
{
	OBJECT( SwitchStatNode, XE::StatASTNode )

public:
	SwitchStatNode() = default;

	~SwitchStatNode() override = default;

public:
	const XE::ExprStatNodePtr & GetExpress() const;

	void SetExpress( const XE::ExprStatNodePtr & val );

	const XE::StatASTNodePtr & GetDefault() const;

	void SetDefault( const XE::StatASTNodePtr & val );

	const XE::Array< XE::Pair< XE::Variant, XE::StatASTNodePtr > > & GetCases() const;

	void SetCases( const XE::Array< XE::Pair< XE::Variant, XE::StatASTNodePtr > > & val );

public:
	void AddCase( const XE::Variant & val, const XE::StatASTNodePtr & stat );

private:
	XE::ExprStatNodePtr _Express;
	XE::StatASTNodePtr _Default;
	XE::Array< XE::Pair< XE::Variant, XE::StatASTNodePtr > > _Cases;
};

class XE_API ReturnStatNode : public XE::StatASTNode
{
	OBJECT( ReturnStatNode, XE::StatASTNode )

public:
	ReturnStatNode() = default;

	~ReturnStatNode() override = default;

public:
	const XE::StatASTNodePtr & GetResult() const;

	void SetResult( const XE::StatASTNodePtr & val );

private:
	XE::StatASTNodePtr _Result;
};

class XE_API ContinueStatNode : public XE::StatASTNode
{
	OBJECT( ContinueStatNode, XE::StatASTNode )

public:
	ContinueStatNode() = default;

	~ContinueStatNode() override = default;
};

class XE_API ValueExprNode : public XE::ExprStatNode
{
	OBJECT( ValueExprNode, XE::ExprStatNode )

public:
	ValueExprNode() = default;

	~ValueExprNode() override = default;

public:
	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::Variant _Value;
};

class XE_API UnaryExprNode : public XE::ExprStatNode
{
	OBJECT( UnaryExprNode, XE::ExprStatNode )

public:
	UnaryExprNode() = default;

	~UnaryExprNode() override = default;

public:
	XE::UnaryExprType GetType() const;

	void SetType( XE::UnaryExprType val );

	const XE::ExprStatNodePtr & GetExpress() const;

	void SetExpress( const XE::ExprStatNodePtr & val );

private:
	XE::UnaryExprType _Type;
	XE::ExprStatNodePtr _Express;
};

class XE_API BinaryExprNode : public XE::ExprStatNode
{
	OBJECT( BinaryExprNode, XE::ExprStatNode )

public:
	BinaryExprNode() = default;

	~BinaryExprNode() override = default;

public:
	XE::BinaryExprType GetType() const;

	void SetType( XE::BinaryExprType val );

	const XE::ExprStatNodePtr & GetLeftExpress() const;

	void SetLeftExpress( const XE::ExprStatNodePtr & val );

	const XE::ExprStatNodePtr & GetRightExpress() const;

	void SetRightExpress( const XE::ExprStatNodePtr & val );

private:
	XE::BinaryExprType _Type;
	XE::ExprStatNodePtr _LeftExpress;
	XE::ExprStatNodePtr _RightExpress;
};

class XE_API InvokeExprNode : public XE::ExprStatNode
{
	OBJECT( InvokeExprNode, XE::ExprStatNode )

public:
	InvokeExprNode() = default;

	~InvokeExprNode() override = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const XE::Array< XE::ExprStatNodePtr > & GetArguments() const;

	void SetArguments( const XE::Array< XE::ExprStatNodePtr > & val );

public:
	void AddArguments( const XE::ExprStatNodePtr & val );

private:
	XE::String _Name;
	XE::Array< XE::ExprStatNodePtr > _Arguments;
};

class XE_API VariableExprNode : public XE::ExprStatNode
{
	OBJECT( VariableExprNode, XE::ExprStatNode )

public:
	VariableExprNode() = default;

	~VariableExprNode() override = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

private:
	XE::String _Name;
};

END_XE_NAMESPACE

#endif//ASTNODE_H__9B9C419D_3C89_48EE_A06E_CC3EE8048C89
