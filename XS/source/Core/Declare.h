/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/16
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef DECLARE_H__E18D69DE_667C_43EF_A8D7_DDFC362820D4
#define DECLARE_H__E18D69DE_667C_43EF_A8D7_DDFC362820D4

#include "Global.h"

BEG_XS_NAMESPACE

DECL_PTR( GraphCompiler );
DECL_PTR( GraphExecutor );

DECL_PTR( GraphNode );
DECL_PTR( GraphNodeMeta );
DECL_PTR( GraphNodeEnum );
DECL_PTR( GraphNodeClass );
DECL_PTR( GraphNodeMethod );
DECL_PTR( GraphNodeProperty );
DECL_PTR( GraphNodeFunction );
DECL_PTR( GraphNodeVariable );
DECL_PTR( GraphNodeMacroIfDef );
DECL_PTR( GraphNodeStatement );
DECL_PTR( GraphNodeIf );
DECL_PTR( GraphNodeFor );
DECL_PTR( GraphNodeWhile );
DECL_PTR( GraphNodeSwitch );
DECL_PTR( GraphNodeBreak );
DECL_PTR( GraphNodeContinue );
DECL_PTR( GraphNodeReturn );
DECL_PTR( GraphNodeDebugBreakPoint );
DECL_PTR( GraphNodeExpression );
DECL_PTR( GraphNodeNew );
DECL_PTR( GraphNodeNot );
DECL_PTR( GraphNodeRev );
DECL_PTR( GraphNodeAdd );
DECL_PTR( GraphNodeSub );
DECL_PTR( GraphNodeMul );
DECL_PTR( GraphNodeDiv );
DECL_PTR( GraphNodeMod );
DECL_PTR( GraphNodeAnd );
DECL_PTR( GraphNodeXor );
DECL_PTR( GraphNodeOr );
DECL_PTR( GraphNodeShiftLeft );
DECL_PTR( GraphNodeShiftRight );
DECL_PTR( GraphNodeLogicAnd );
DECL_PTR( GraphNodeLogicOr );
DECL_PTR( GraphNodeEqual );
DECL_PTR( GraphNodeNotEqual );
DECL_PTR( GraphNodeLess );
DECL_PTR( GraphNodeLessEqual );
DECL_PTR( GraphNodeGreater );
DECL_PTR( GraphNodeGreaterEqual );
DECL_PTR( GraphNodeConstBool );
DECL_PTR( GraphNodeConstInt );
DECL_PTR( GraphNodeConstFloat );
DECL_PTR( GraphNodeConstString );
DECL_PTR( GraphNodeCallMethod );
DECL_PTR( GraphNodeCallFunction );
DECL_PTR( GraphNodeGetProperty );
DECL_PTR( GraphNodeSetProperty );
DECL_PTR( GraphNodeGetVariable );
DECL_PTR( GraphNodeSetVariable );

END_XS_NAMESPACE

#endif//DECLARE_H__E18D69DE_667C_43EF_A8D7_DDFC362820D4