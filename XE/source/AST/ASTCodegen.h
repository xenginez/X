/*!
 * \file	ASTCodegen.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/09
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTCODEGEN_H__78C8CE24_4D87_4566_A4A9_88555BDE768F
#define ASTCODEGEN_H__78C8CE24_4D87_4566_A4A9_88555BDE768F

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTCodegen : public XE::EnableSharedFromThis< XE::ASTCodegen >
{
public:
	virtual void Visit( const ASTNode * val ) = 0;
	virtual void Visit( const TypeASTNode * val ) = 0;
	virtual void Visit( const EnumASTNode * val ) = 0;
	virtual void Visit( const StructASTNode * val ) = 0;
	virtual void Visit( const FunctionASTNode * val ) = 0;
	virtual void Visit( const StatASTNode * val ) = 0;
	virtual void Visit( const IfStatNode * val ) = 0;
	virtual void Visit( const BlockStatNode * val ) = 0;
	virtual void Visit( const BreakStatNode * val ) = 0;
	virtual void Visit( const WhileStatNode * val ) = 0;
	virtual void Visit( const SwitchStatNode * val ) = 0;
	virtual void Visit( const ReturnStatNode * val ) = 0;
	virtual void Visit( const ContinueStatNode * val ) = 0;
	virtual void Visit( const ExprASTNode * val ) = 0;
	virtual void Visit( const ValueExprNode * val ) = 0;
	virtual void Visit( const UnaryExprNode * val ) = 0;
	virtual void Visit( const BinaryExprNode * val ) = 0;
	virtual void Visit( const SizeofExprNode * val ) = 0;
	virtual void Visit( const TypeofExprNode * val ) = 0;
	virtual void Visit( const InvokeExprNode * val ) = 0;
	virtual void Visit( const VariableExprNode * val ) = 0;
};

END_XE_NAMESPACE

#endif//ASTCODEGEN_H__78C8CE24_4D87_4566_A4A9_88555BDE768F
