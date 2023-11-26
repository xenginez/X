/*!
 * \file	GraphNodes.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/16
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef GRAPHNODES_H__E44E92AD_5D7D_4A4E_9A73_29C14E352D3A
#define GRAPHNODES_H__E44E92AD_5D7D_4A4E_9A73_29C14E352D3A

#include "GraphExecutor.h"
#include "GraphCompiler.h"

BEG_XS_NAMESPACE

class XS_API GraphNode : public XE::Object
{
	OBJECT( GraphNode, XE::Object )

public:
	GraphNode();

	~GraphNode() override;

public:
	XE::uint64 GetID() const;

	void SetID( XE::uint64 val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

public:
	virtual void Compile( const XS::GraphCompilerPtr & ctx ) = 0;

	virtual void Execute( const XS::GraphExecutorPtr & ctx ) = 0;

private:
	XE::uint64 _ID;
	XE::String _Name;
};

class GraphNodeMeta : public XS::GraphNode
{
	OBJECT( GraphNodeMeta, XS::GraphNode )

public:
	GraphNodeMeta();

	~GraphNodeMeta() override;

public:
	const XE::String & GetModuleName() const;

	void SetModuleName( const XE::String & val );

private:
	XE::String _ModuleName;
};
class GraphNodeEnum : public XS::GraphNodeMeta
{
	OBJECT( GraphNodeEnum, XS::GraphNodeMeta )

public:
	GraphNodeEnum();

	~GraphNodeEnum() override;

public:
	const XE::Array< XE::String > & GetValues() const;

	void SetValues( const XE::Array< XE::String > & val );

private:
	XE::Array< XE::String > _Values;
};
class GraphNodeClass : public XS::GraphNodeMeta
{
	OBJECT( GraphNodeClass, XS::GraphNodeMeta )

public:
	GraphNodeClass();

	~GraphNodeClass() override;

public:
	const XE::String & GetSuperClass() const;

	void SetSuperClass( const XE::String & val );

	const XE::Array< XS::GraphNodeMethodPtr > & GetMethods() const;

	void SetMethods( const XE::Array< XS::GraphNodeMethodPtr > & val );

	const XE::Array< XS::GraphNodePropertyPtr > & GetPropertys() const;

	void SetPropertys( const XE::Array< XS::GraphNodePropertyPtr > & val );

	const XE::Array< XS::GraphNodeFunctionPtr > & GetFunctions() const;

	void SetFunctions( const XE::Array< XS::GraphNodeFunctionPtr > & val );

	const XE::Array< XS::GraphNodeVariablePtr > & GetVariables() const;

	void SetVariables( const XE::Array< XS::GraphNodeVariablePtr > & val );

private:
	XE::String _SuperClass;
	XE::Array< XS::GraphNodeMethodPtr > _Methods;
	XE::Array< XS::GraphNodePropertyPtr > _Propertys;
	XE::Array< XS::GraphNodeFunctionPtr > _Functions;
	XE::Array< XS::GraphNodeVariablePtr > _Variables;
};
class GraphNodeMethod : public XS::GraphNodeMeta
{
	OBJECT( GraphNodeMethod, XS::GraphNodeMeta )

public:
	GraphNodeMethod();

	~GraphNodeMethod() override;

public:
	const XE::String & GetResult() const;

	void SetResult( const XE::String & val );

	const XS::GraphNodeStatementPtr & GetBody() const;

	void SetBody( const XS::GraphNodeStatementPtr & val );

	const XE::Array< XE::Pair< XE::String, XE::String > > & GetParameters() const;

	void SetParameters( const XE::Array< XE::Pair< XE::String, XE::String > > & val );

private:
	XE::String _Result;
	XS::GraphNodeStatementPtr _Body;
	XE::Array< XE::Pair< XE::String, XE::String > > _Parameters;
};
class GraphNodeProperty : public XS::GraphNodeMeta
{
	OBJECT( GraphNodeProperty, XS::GraphNodeMeta )

public:
	GraphNodeProperty();

	~GraphNodeProperty() override;

public:
	const XE::String & GetType() const;

	void SetType( const XE::String & val );

private:
	XE::String _Type;
};
class GraphNodeFunction : public XS::GraphNodeMeta
{
	OBJECT( GraphNodeFunction, XS::GraphNodeMeta )

public:
	GraphNodeFunction();

	~GraphNodeFunction() override;

public:
	const XE::String & GetResult() const;

	void SetResult( const XE::String & val );

	const XS::GraphNodeStatementPtr & GetBody() const;

	void SetBody( const XS::GraphNodeStatementPtr & val );

	const XE::Array< XE::Pair< XE::String, XE::String > > & GetParameters() const;

	void SetParameters( const XE::Array< XE::Pair< XE::String, XE::String > > & val );

private:
	XE::String _Result;
	XS::GraphNodeStatementPtr _Body;
	XE::Array< XE::Pair< XE::String, XE::String > > _Parameters;
};
class GraphNodeVariable : public XS::GraphNodeMeta
{
	OBJECT( GraphNodeVariable, XS::GraphNodeMeta )

public:
	GraphNodeVariable();

	~GraphNodeVariable() override;

public:
	const XE::String & GetType() const;

	void SetType( const XE::String & val );

private:
	XE::String _Type;
};

class GraphNodeStatement : public XS::GraphNode
{
	OBJECT( GraphNodeStatement, XS::GraphNode )

public:
	GraphNodeStatement();

	~GraphNodeStatement() override;
};
class GraphNodeMacroIfDef : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeMacroIfDef, XS::GraphNodeStatement )

public:
	GraphNodeMacroIfDef();

	~GraphNodeMacroIfDef() override;

public:
	const XE::String & GetMacro() const;

	void SetMacro( const XE::String & val );

	const XS::GraphNodeStatementPtr & GetThen() const;

	void SetThen( const XS::GraphNodeStatementPtr & val );

	const XS::GraphNodeStatementPtr & GetElse() const;

	void SetElse( const XS::GraphNodeStatementPtr & val );

	const XS::GraphNodeStatementPtr & GetOutput() const;

	void SetOutput( const XS::GraphNodeStatementPtr & val );

private:
	XE::String _Macro;
	XS::GraphNodeStatementPtr _Then;
	XS::GraphNodeStatementPtr _Else;
	XS::GraphNodeStatementPtr _Output;
};
class GraphNodeIf : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeIf, XS::GraphNodeStatement )

public:
	GraphNodeIf();

	~GraphNodeIf() override;

public:
	const XS::GraphNodeStatementPtr & GetThen() const;

	void SetThen( const XS::GraphNodeStatementPtr & val );

	const XS::GraphNodeStatementPtr & GetElse() const;

	void SetElse( const XS::GraphNodeStatementPtr & val );

	const XS::GraphNodeExpressionPtr & GetCondition() const;

	void SetCondition( const XS::GraphNodeExpressionPtr & val );

private:
	XS::GraphNodeStatementPtr _Then;
	XS::GraphNodeStatementPtr _Else;
	XS::GraphNodeExpressionPtr _Condition;
};
class GraphNodeFor : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeFor, XS::GraphNodeStatement )

public:
	GraphNodeFor();

	~GraphNodeFor() override;

public:
	const XS::GraphNodeStatementPtr & GetBody() const;

	void SetBody( const XS::GraphNodeStatementPtr & val );

	const XS::GraphNodeExpressionPtr & GetIndex() const;

	void SetIndex( const XS::GraphNodeExpressionPtr & val );

	const XS::GraphNodeExpressionPtr & GetCondition() const;

	void SetCondition( const XS::GraphNodeExpressionPtr & val );

	const XS::GraphNodeExpressionPtr & GetControlStep() const;

	void SetControlStep( const XS::GraphNodeExpressionPtr & val );

private:
	XS::GraphNodeStatementPtr _Body;
	XS::GraphNodeExpressionPtr _Index;
	XS::GraphNodeExpressionPtr _Condition;
	XS::GraphNodeExpressionPtr _ControlStep;
};
class GraphNodeWhile : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeWhile, XS::GraphNodeStatement )

public:
	GraphNodeWhile();

	~GraphNodeWhile() override;

public:
	const XS::GraphNodeStatementPtr & GetBody() const;

	void SetBody( const XS::GraphNodeStatementPtr & val );

	const XS::GraphNodeExpressionPtr & GetCondition() const;

	void SetCondition( const XS::GraphNodeExpressionPtr & val );

private:
	XS::GraphNodeStatementPtr _Body;
	XS::GraphNodeExpressionPtr _Condition;
};
class GraphNodeSwitch : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeSwitch, XS::GraphNodeStatement )

public:
	GraphNodeSwitch();

	~GraphNodeSwitch() override;

public:
	const XS::GraphNodeExpressionPtr & GetCondition() const;

	void SetCondition( const XS::GraphNodeExpressionPtr & val );

	const XS::GraphNodeStatementPtr & GetDefault() const;

	void SetDefault( const XS::GraphNodeStatementPtr & val );

	const XE::Array< XE::Pair< XE::Variant, XS::GraphNodeStatementPtr > > & GetCases() const;

	void SetCases( const XE::Array< XE::Pair< XE::Variant, XS::GraphNodeStatementPtr > > & val );

private:
	XS::GraphNodeExpressionPtr _Condition;
	XS::GraphNodeStatementPtr _Default;
	XE::Array< XE::Pair< XE::Variant, XS::GraphNodeStatementPtr > > _Cases;
};
class GraphNodeBreak : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeBreak, XS::GraphNodeStatement )

public:
	GraphNodeBreak();

	~GraphNodeBreak() override;
};
class GraphNodeContinue : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeContinue, XS::GraphNodeStatement )

public:
	GraphNodeContinue();

	~GraphNodeContinue() override;
};
class GraphNodeReturn : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeReturn, XS::GraphNodeStatement )

public:
	GraphNodeReturn();

	~GraphNodeReturn() override;

public:
	const XS::GraphNodeExpressionPtr & GetValue() const;

	void SetValue( const XS::GraphNodeExpressionPtr & val );

private:
	XS::GraphNodeExpressionPtr _Value;
};

class GraphNodeDebugBreakPoint : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeDebugBreakPoint, XS::GraphNodeStatement )

public:
	GraphNodeDebugBreakPoint();

	~GraphNodeDebugBreakPoint() override;
};

class GraphNodeExpression : public XS::GraphNodeStatement
{
	OBJECT( GraphNodeExpression, XS::GraphNodeStatement )

public:
	GraphNodeExpression();

	~GraphNodeExpression() override;

public:
	const XS::GraphNodeStatementPtr & GetOutput() const;

	void SetOutput( const XS::GraphNodeStatementPtr & val );

private:
	XS::GraphNodeStatementPtr _Output;
};
class GraphNodeNew : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeNew, XS::GraphNodeExpression )

public:
	GraphNodeNew();

	~GraphNodeNew() override;

public:
	const XE::String & GetType() const;

	void SetType( const XE::String & val );

	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::String _Type;
	XE::Variant _Value;
};
class GraphNodeNot : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeNot, XS::GraphNodeExpression )

public:
	GraphNodeNot();

	~GraphNodeNot() override;

public:
	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::Variant _Value;
};
class GraphNodeRev : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeRev, XS::GraphNodeExpression )

public:
	GraphNodeRev();

	~GraphNodeRev() override;

public:
	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::Variant _Value;
};
class GraphNodeAdd : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeAdd, XS::GraphNodeExpression )

public:
	GraphNodeAdd();

	~GraphNodeAdd() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeSub : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeSub, XS::GraphNodeExpression )

public:
	GraphNodeSub();

	~GraphNodeSub() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeMul : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeMul, XS::GraphNodeExpression )

public:
	GraphNodeMul();

	~GraphNodeMul() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeDiv : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeDiv, XS::GraphNodeExpression )

public:
	GraphNodeDiv();

	~GraphNodeDiv() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeMod : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeMod, XS::GraphNodeExpression )

public:
	GraphNodeMod();

	~GraphNodeMod() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeAnd : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeAnd, XS::GraphNodeExpression )

public:
	GraphNodeAnd();

	~GraphNodeAnd() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeXor : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeXor, XS::GraphNodeExpression )

public:
	GraphNodeXor();

	~GraphNodeXor() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeOr : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeOr, XS::GraphNodeExpression )

public:
	GraphNodeOr();

	~GraphNodeOr() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeShiftLeft : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeShiftLeft, XS::GraphNodeExpression )

public:
	GraphNodeShiftLeft();

	~GraphNodeShiftLeft() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeShiftRight : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeShiftRight, XS::GraphNodeExpression )

public:
	GraphNodeShiftRight();

	~GraphNodeShiftRight() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeLogicAnd : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeLogicAnd, XS::GraphNodeExpression )

public:
	GraphNodeLogicAnd();

	~GraphNodeLogicAnd() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeLogicOr : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeLogicOr, XS::GraphNodeExpression )

public:
	GraphNodeLogicOr();

	~GraphNodeLogicOr() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeEqual : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeEqual, XS::GraphNodeExpression )

public:
	GraphNodeEqual();

	~GraphNodeEqual() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeNotEqual : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeNotEqual, XS::GraphNodeExpression )

public:
	GraphNodeNotEqual();

	~GraphNodeNotEqual() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeLess : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeLess, XS::GraphNodeExpression )

public:
	GraphNodeLess();

	~GraphNodeLess() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeLessEqual : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeLessEqual, XS::GraphNodeExpression )

public:
	GraphNodeLessEqual();

	~GraphNodeLessEqual() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeGreater : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeGreater, XS::GraphNodeExpression )

public:
	GraphNodeGreater();

	~GraphNodeGreater() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeGreaterEqual : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeGreaterEqual, XS::GraphNodeExpression )

public:
	GraphNodeGreaterEqual();

	~GraphNodeGreaterEqual() override;

public:
	const XE::Variant & GetLeft() const;

	void SetLeft( const XE::Variant & val );

	const XE::Variant & GetRight() const;

	void SetRight( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::Variant _Left;
	XE::Variant _Right;
	XE::Variant _Result;
};
class GraphNodeConstBool : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeConstBool, XS::GraphNodeExpression )

public:
	GraphNodeConstBool();

	~GraphNodeConstBool() override;

public:
	bool GetValue() const;

	void SetValue( bool val );

private:
	bool _Value;
};
class GraphNodeConstInt : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeConstInt, XS::GraphNodeExpression )

public:
	GraphNodeConstInt();

	~GraphNodeConstInt() override;

public:
	int GetValue() const;

	void SetValue( int val );

private:
	int _Value;
};
class GraphNodeConstFloat : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeConstFloat, XS::GraphNodeExpression )

public:
	GraphNodeConstFloat();

	~GraphNodeConstFloat() override;

public:
	XE::float32 GetValue() const;

	void SetValue( XE::float32 val );

private:
	XE::float32 _Value;
};
class GraphNodeConstString : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeConstString, XS::GraphNodeExpression )

public:
	GraphNodeConstString();

	~GraphNodeConstString() override;

public:
	const XE::String & GetValue() const;

	void SetValue( const XE::String & val );

private:
	XE::String _Value;
};

class GraphNodeCallMethod : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeCallMethod, XS::GraphNodeExpression )

public:
	GraphNodeCallMethod();

	~GraphNodeCallMethod() override;

public:
	const XE::String & GetMethod() const;

	void SetMethod( const XE::String & val );

	const XE::Variant & GetThis() const;

	void SetThis( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

	const XE::Array< XE::Variant > & GetParameters() const;

	void SetParameters( const XE::Array< XE::Variant > & val );

private:
	XE::String _Method;
	XE::Variant _This;
	XE::Variant _Result;
	XE::Array< XE::Variant > _Parameters;
};
class GraphNodeCallFunction : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeCallFunction, XS::GraphNodeExpression )

public:
	GraphNodeCallFunction();

	~GraphNodeCallFunction() override;

public:
	const XE::String & GetFunction() const;

	void SetFunction( const XE::String & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

	const XE::Array< XE::Variant > & GetParameters() const;

	void SetParameters( const XE::Array< XE::Variant > & val );

private:
	XE::String _Function;
	XE::Variant _This;
	XE::Variant _Result;
	XE::Array< XE::Variant > _Parameters;
};
class GraphNodeGetProperty : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeGetProperty, XS::GraphNodeExpression )

public:
	GraphNodeGetProperty();

	~GraphNodeGetProperty() override;

public:
	const XE::String & GetProperty() const;

	void SetProperty( const XE::String & val );

	const XE::Variant & GetThis() const;

	void SetThis( const XE::Variant & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::String _Property;
	XE::Variant _This;
	XE::Variant _Result;
};
class GraphNodeSetProperty : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeSetProperty, XS::GraphNodeExpression )

public:
	GraphNodeSetProperty();

	~GraphNodeSetProperty() override;

public:
	const XE::String & GetProperty() const;

	void SetProperty( const XE::String & val );

	const XE::Variant & GetThis() const;

	void SetThis( const XE::Variant & val );

	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::String _Property;
	XE::Variant _This;
	XE::Variant _Value;
};
class GraphNodeGetVariable : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeGetVariable, XS::GraphNodeExpression )

public:
	GraphNodeGetVariable();

	~GraphNodeGetVariable() override;

public:
	const XE::String & GetVariable() const;

	void SetVariable( const XE::String & val );

	const XE::Variant & GetResult() const;

	void SetResult( const XE::Variant & val );

private:
	XE::String _Variable;
	XE::Variant _Result;
};
class GraphNodeSetVariable : public XS::GraphNodeExpression
{
	OBJECT( GraphNodeSetVariable, XS::GraphNodeExpression )

public:
	GraphNodeSetVariable();

	~GraphNodeSetVariable() override;

public:
	const XE::String & GetVariable() const;

	void SetVariable( const XE::String & val );

	const XE::Variant & GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::String _Variable;
	XE::Variant _Value;
};

END_XS_NAMESPACE

#endif//GRAPHNODES_H__E44E92AD_5D7D_4A4E_9A73_29C14E352D3A