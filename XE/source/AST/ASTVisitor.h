/*!
 * \file	ASTVisitor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTVISITOR_H__04D86451_6498_44D2_811C_C8AF4856F26D
#define ASTVISITOR_H__04D86451_6498_44D2_811C_C8AF4856F26D

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTVisitor : public XE::NonCopyable
{
public:
	virtual void Visit( const XE::ASTNode * val ) = 0;

	virtual void Visit( const XE::IfStatNode * val ) = 0;

	virtual void Visit( const XE::BreakStatNode * val ) = 0;

	virtual void Visit( const XE::WhileStatNode * val ) = 0;

	virtual void Visit( const XE::SwitchStatNode * val ) = 0;

	virtual void Visit( const XE::ReturnStatNode * val ) = 0;

	virtual void Visit( const XE::ContinueStatNode * val ) = 0;

	virtual void Visit( const XE::ValueExprNode * val ) = 0;

	virtual void Visit( const XE::UnaryExprNode * val ) = 0;

	virtual void Visit( const XE::BinaryExprNode * val ) = 0;

	virtual void Visit( const XE::InvokeExprNode * val ) = 0;

	virtual void Visit( const XE::VariableExprNode * val ) = 0;
};

END_XE_NAMESPACE

#endif//ASTVISITOR_H__04D86451_6498_44D2_811C_C8AF4856F26D
