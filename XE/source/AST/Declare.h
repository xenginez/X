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

class ASTContext;
class ASTExecuteContext;
class ASTCompileContext;

DECL_PTR( ASTService );
DECL_PTR( ASTVisitor );
DECL_PTR( ASTInstance );

DECL_PTR( ASTInfoMeta );
DECL_PTR( ASTInfoType );
DECL_PTR( ASTInfoEnum );
DECL_PTR( ASTInfoClass );
DECL_PTR( ASTInfoModule );
DECL_PTR( ASTInfoProperty );
DECL_PTR( ASTInfoFunction );
DECL_PTR( ASTInfoVariable );

DECL_PTR( ASTMetaEnum );
DECL_PTR( ASTMetaClass );
DECL_PTR( ASTMetaMethod );
DECL_PTR( ASTMetaProperty );
DECL_PTR( ASTMetaFunction );
DECL_PTR( ASTMetaVariable );

DECL_PTR( ASTNode );
DECL_PTR( MacroIfNode );
DECL_PTR( MacroElseNode );
DECL_PTR( MacroElifNode );
DECL_PTR( MacroEndNode );
DECL_PTR( StatASTNode );
DECL_PTR( IfStatNode );
DECL_PTR( BreakStatNode );
DECL_PTR( WhileStatNode );
DECL_PTR( SwitchStatNode );
DECL_PTR( ReturnStatNode );
DECL_PTR( ContinueStatNode );
DECL_PTR( ExprStatNode );
DECL_PTR( ValueExprNode );
DECL_PTR( UnaryExprNode );
DECL_PTR( BinaryExprNode );
DECL_PTR( InvokeExprNode );
DECL_PTR( VariableExprNode );

END_XE_NAMESPACE

#endif//DECLARE_H__1D034124_9106_4ECA_8D26_F66C24BC7CDD
