/*!
 * \file	ASTExecutor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTEXECUTOR_H__CB058406_A354_46F4_8BDB_D28365A80C4D
#define ASTEXECUTOR_H__CB058406_A354_46F4_8BDB_D28365A80C4D

#include "ASTVisitor.h"

BEG_XE_NAMESPACE

class XE_API ASTExecutor : public XE::ASTVisitor
{
private:
	struct Private;

private:
	ASTExecutor();

	~ASTExecutor();

public:
	static XE::Variant Invoke( const XE::SharedPtr< XE::ASTMethod > & method, XE::InvokeStack * args );

	static XE::Variant Invoke( const XE::SharedPtr< XE::ASTFunction > & function, XE::InvokeStack * args );

public:
	void Visit( const XE::ASTNode * val ) override;

	void Visit( const XE::TypeASTNode * val ) override;

	void Visit( const XE::IfStatNode * val ) override;

	void Visit( const XE::BreakStatNode * val ) override;

	void Visit( const XE::WhileStatNode * val ) override;

	void Visit( const XE::SwitchStatNode * val ) override;

	void Visit( const XE::ReturnStatNode * val ) override;

	void Visit( const XE::ContinueStatNode * val ) override;

	void Visit( const XE::ValueExprNode * val ) override;

	void Visit( const XE::UnaryExprNode * val ) override;

	void Visit( const XE::BinaryExprNode * val ) override;

	void Visit( const XE::SizeofExprNode * val ) override;

	void Visit( const XE::TypeofExprNode * val ) override;

	void Visit( const XE::InvokeExprNode * val ) override;

	void Visit( const XE::VariableExprNode * val ) override;

private:
	static XE::ASTExecutor * GetInstance();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//ASTEXECUTOR_H__CB058406_A354_46F4_8BDB_D28365A80C4D
