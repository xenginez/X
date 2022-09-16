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

class XE_API ASTVisitor : public XE::Object
{
	OBJECT( ASTVisitor, XE::Object );

public:
	ASTVisitor() = default;

	virtual ~ASTVisitor() = default;

protected:
	XE::ASTServicePtr GetService() const;
};

class XE_API ASTExecuteVisitor : public ASTVisitor
{
	OBJECT( ASTExecuteVisitor, XE::ASTVisitor );

public:
	ASTExecuteVisitor() = default;

	~ASTExecuteVisitor() override = default;
};

class XE_API MacroIfNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( MacroIfNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::MacroIfNode * node ) const;
};

class XE_API MacroElseNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( MacroElseNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::MacroElseNode * node ) const;
};

class XE_API MacroElifNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( MacroElifNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::MacroElifNode * node ) const;
};

class XE_API MacroEndNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( MacroEndNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::MacroEndNode * node ) const;
};

class XE_API IfStatNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( IfStatNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::IfStatNode * node ) const;
};

class XE_API BreakStatNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( BreakStatNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::BreakStatNode * node ) const;
};

class XE_API WhileStatNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( WhileStatNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::WhileStatNode * node ) const;
};

class XE_API SwitchStatNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( SwitchStatNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::SwitchStatNode * node ) const;
};

class XE_API ReturnStatNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( ReturnStatNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::ReturnStatNode * node ) const;
};

class XE_API ContinueStatNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( ContinueStatNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::ContinueStatNode * node ) const;
};

class XE_API ValueExprNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( ValueExprNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::ValueExprNode * node ) const;
};

class XE_API UnaryExprNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( UnaryExprNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::UnaryExprNode * node ) const;
};

class XE_API BinaryExprNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( BinaryExprNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::BinaryExprNode * node ) const;
};

class XE_API InvokeExprNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( InvokeExprNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::InvokeExprNode * node ) const;
};

class XE_API VariableExprNodeExecuteVisitor : public ASTExecuteVisitor
{
	OBJECT( VariableExprNodeExecuteVisitor, XE::ASTExecuteVisitor );

public:
	void Visit( XE::ASTExecuteContext * context, const XE::VariableExprNode * node ) const;
};


class XE_API ASTCompileVisitor : public ASTVisitor
{
	OBJECT( ASTCompileVisitor, XE::ASTVisitor );

public:
	ASTCompileVisitor() = default;

	~ASTCompileVisitor() override = default;
};

class XE_API MacroIfNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( MacroIfNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::MacroIfNode * node ) const;
};

class XE_API MacroElseNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( MacroElseNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::MacroElseNode * node ) const;
};

class XE_API MacroElifNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( MacroElifNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::MacroElifNode * node ) const;
};

class XE_API MacroEndNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( MacroEndNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::MacroEndNode * node ) const;
};

class XE_API IfStatNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( IfStatNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::IfStatNode * node ) const;
};

class XE_API BreakStatNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( BreakStatNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::BreakStatNode * node ) const;
};

class XE_API WhileStatNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( WhileStatNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::WhileStatNode * node ) const;
};

class XE_API SwitchStatNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( SwitchStatNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::SwitchStatNode * node ) const;
};

class XE_API ReturnStatNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( ReturnStatNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::ReturnStatNode * node ) const;
};

class XE_API ContinueStatNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( ContinueStatNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::ContinueStatNode * node ) const;
};

class XE_API ValueExprNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( ValueExprNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::ValueExprNode * node ) const;
};

class XE_API UnaryExprNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( UnaryExprNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::UnaryExprNode * node ) const;
};

class XE_API BinaryExprNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( BinaryExprNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::BinaryExprNode * node ) const;
};

class XE_API InvokeExprNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( InvokeExprNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::InvokeExprNode * node ) const;
};

class XE_API VariableExprNodeCompileVisitor : public ASTCompileVisitor
{
	OBJECT( VariableExprNodeCompileVisitor, XE::ASTCompileVisitor );

public:
	void Visit( XE::ASTCompileContext * context, const XE::VariableExprNode * node ) const;
};

END_XE_NAMESPACE

#endif//ASTVISITOR_H__04D86451_6498_44D2_811C_C8AF4856F26D
