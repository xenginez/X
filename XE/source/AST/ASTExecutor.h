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
public:
	ASTExecutor();

	~ASTExecutor();

public:
	void Visit( const XE::ASTNode * val ) override;

	void Visit( const XE::MacroIfASTNode * val ) override;

	void Visit( const XE::MacroElseASTNode * val ) override;

	void Visit( const XE::MacroElifASTNode * val ) override;

	void Visit( const XE::MacroEndASTNode * val ) override;

	void Visit( const XE::IfStatNode * val ) override;

	void Visit( const XE::BreakStatNode * val ) override;

	void Visit( const XE::WhileStatNode * val ) override;

	void Visit( const XE::SwitchStatNode * val ) override;

	void Visit( const XE::ReturnStatNode * val ) override;

	void Visit( const XE::ContinueStatNode * val ) override;

	void Visit( const XE::ValueExprNode * val ) override;

	void Visit( const XE::UnaryExprNode * val ) override;

	void Visit( const XE::BinaryExprNode * val ) override;

	void Visit( const XE::InvokeExprNode * val ) override;

	void Visit( const XE::VariableExprNode * val ) override;

public:
	void AddMacro( const XE::String & val );

public:
	XE::Variant Invoke( const XE::SharedPtr< XE::ASTMethod > & method, XE::InvokeStack * args );

	XE::Variant Invoke( const XE::SharedPtr< XE::ASTFunction > & function, XE::InvokeStack * args );

public:
	void Push( const XE::Variant & val );

	XE::Variant Pop();

	XE::Variant & Top();

	XE::Variant & Get( XE::uint64 val );

public:
	XE::uint64 Index() const;

	void Reset( XE::uint64 val );

public:
	bool MacroSkip() const;

public:
	XE::ASTFrame * GetFrame() const;

private:
	void Exec();

private:
	XE::Array< XE::String > _Macros;
	XE::Array< XE::Variant > _ValStack;
	XE::Array< XE::ASTFrame * > _FrameStack;
};

END_XE_NAMESPACE

#endif//ASTEXECUTOR_H__CB058406_A354_46F4_8BDB_D28365A80C4D
