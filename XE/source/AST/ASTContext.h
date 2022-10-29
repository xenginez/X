/*!
 * \file	ASTContext.h
 *
 * \author	ZhengYuanQing
 * \date	2022/09/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTCONTEXT_H__B06A7DDF_7987_49F5_A3B6_78DB4D10A9C5
#define ASTCONTEXT_H__B06A7DDF_7987_49F5_A3B6_78DB4D10A9C5

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTContext
{
	OBJECT( ASTContext );

public:
	ASTContext() = default;

	virtual ~ASTContext() = default;

public:
	virtual XE::MetaClassCPtr GetVisitorBaseClass() = 0;

public:
	void AddMacro( const XE::String & val );

	bool HasMacro( const XE::String & val ) const;

public:
	bool MacroSkip() const;

public:
	void PushMacroGotoType( XE::MacroGotoTypeFlags type );

	XE::MacroGotoTypeFlags & TopMacroGotoType();

	void PopMacroGotoType();

private:
	XE::Array< XE::String > _Macros;
	XE::Deque< XE::MacroGotoTypeFlags > _MacroGo;
};

class XE_API ASTExecuteContext : public ASTContext
{
	OBJECT( ASTExecuteContext, XE::ASTContext );

public:
	ASTExecuteContext() = default;

	~ASTExecuteContext() override = default;

public:
	static ASTExecuteContext * ThreadInstance();

public:
	XE::MetaClassCPtr GetVisitorBaseClass() override;

public:
	XE::Variant Invoke( const XE::ASTInfoMethodPtr & method, XE::InvokeStack * args );

	XE::Variant Invoke( const XE::ASTInfoFunctionPtr & function, XE::InvokeStack * args );

public:
	void Push( const XE::Variant & val );

	XE::Variant Pop();

	XE::Variant & Top();

	XE::Variant & Get( XE::uint64 val );

public:
	XE::uint64 Index() const;

	void Reset( XE::uint64 val );

public:
	XE::ASTFrame * GetFrame() const;

public:
	void PushLoopNode( const XE::WhileStatNode * node );

	void PopLoopNode();

public:
	XE::ExecuteGotoType GetExecGotoType() const;

	void SetExecGotoType( XE::ExecuteGotoType type );

private:
	void Exec();

public:
	XE::Array< XE::Variant > _ValStack;
	XE::Array< XE::ASTFrame * > _FrameStack;
};

class XE_API ASTCompileContext : public ASTContext
{
	OBJECT( ASTCompileContext, XE::ASTContext );

private:
	struct Private;

public:
	struct Type;
	struct Inst;
	struct Value;
	struct BasicBlock;

public:
	ASTCompileContext();

	~ASTCompileContext() override;

public:
	static ASTCompileContext * ThreadInstance();

public:
	XE::MetaClassCPtr GetVisitorBaseClass() override;

public:
	XE::MemoryView Compile( const XE::ASTInfoMethodPtr & method );

	XE::MemoryView Compile( const XE::ASTInfoFunctionPtr & function );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//ASTCONTEXT_H__B06A7DDF_7987_49F5_A3B6_78DB4D10A9C5
