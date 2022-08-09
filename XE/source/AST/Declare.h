/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/09
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__1D034124_9106_4ECA_8D26_F66C24BC7CDD
#define DECLARE_H__1D034124_9106_4ECA_8D26_F66C24BC7CDD

#include "Utils/Declare.h"

BEG_XE_NAMESPACE

DECL_PTR( ASTNode );
DECL_PTR( TypeASTNode );
DECL_PTR( EnumASTNode );
DECL_PTR( StructASTNode );
DECL_PTR( FunctionASTNode );
DECL_PTR( StatASTNode );
DECL_PTR( IfStatNode );
DECL_PTR( BlockStatNode );
DECL_PTR( BreakStatNode );
DECL_PTR( WhileStatNode );
DECL_PTR( SwitchStatNode );
DECL_PTR( ReturnStatNode );
DECL_PTR( ContinueStatNode );
DECL_PTR( ExprASTNode );
DECL_PTR( ValueExprNode );
DECL_PTR( UnaryExprNode );
DECL_PTR( BinaryExprNode );
DECL_PTR( SizeofExprNode );
DECL_PTR( TypeofExprNode );
DECL_PTR( InvokeExprNode );
DECL_PTR( VariableExprNode );

DECL_PTR( SyntaxTree );
DECL_PTR( ASTCodegen );

END_XE_NAMESPACE

#endif//DECLARE_H__1D034124_9106_4ECA_8D26_F66C24BC7CDD
