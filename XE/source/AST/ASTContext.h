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

public:
	struct Type;
	struct Value;
	struct Label;

public:
	ASTCompileContext() = default;

	~ASTCompileContext() override = default;

public:
	XE::MetaClassCPtr GetVisitorBaseClass() override;

public:
	XE::MemoryView Compile( const XE::ASTInfoMethodPtr & method );

	XE::MemoryView Compile( const XE::ASTInfoFunctionPtr & function );

public:
	Type * CreateType();

	Value * CreateValue( Type * type );

	Label * CreateLabel();

public:// Unary operators...
	void Ret( Value * value );
	void Br( Value * cond, Label * iftrue, Label * iffalse );
	void Switch( Value * value, Label * def, XE::Span< XE::Pair< Value *, Label * > > cases );
	void IndirectBr( Value * addr, XE::Span< Label * > labels );
	void Invoke();
	void Resume();
	void Unreachable();
	void CleanupRet();
	void CatchRet();
	void CatchSwitch();
	void CallBr();

public:// Binary operators...
	Value * Add( Type * type, Value * left, Value * right );
	Value * FAdd( Type * type, Value * left, Value * right );
	Value * Sub( Type * type, Value * left, Value * right );
	Value * FSub( Type * type, Value * left, Value * right );
	Value * Mul( Type * type, Value * left, Value * right );
	Value * FMul( Type * type, Value * left, Value * right );
	Value * UDiv( Type * type, Value * left, Value * right );
	Value * SDiv( Type * type, Value * left, Value * right );
	Value * FDiv( Type * type, Value * left, Value * right );
	Value * URem( Type * type, Value * left, Value * right );
	Value * SRem( Type * type, Value * left, Value * right );
	Value * FRem( Type * type, Value * left, Value * right );

public:// Logical operators ...
	Value * Shl( Type * type, Value * left, Value * right );
	Value * LShr( Type * type, Value * left, Value * right );
	Value * AShr( Type * type, Value * left, Value * right );
	Value * And( Type * type, Value * left, Value * right );
	Value * Or( Type * type, Value * left, Value * right );
	Value * Xor( Type * type, Value * left, Value * right );

public:// Memory operators ...
	Value * Alloca( Type * type, XE::uint64 element_num = 1, XE::uint64 alignment = sizeof( void * ) );
	Value * Load( Type * type, Value * ptr );
	Value * Store( Value * value, Value * ptr );
	Value * GetElementPtr( Type * type, Value * ptr, XE::Span< XE::uint64 > idx );

public:// Cast operators ...
	Value * Trunc( Value * value, Type * type );
	void Zext();
	void Sext();
	void FpToUI();
	void FpToSI();
	void UiToFP();
	void SiToFP();
	void FpTrunc();
	void FpExt();
	void PtrToInt();
	void IntToPtr();
	void BitCast();
	void AddrSpaceCast();

public:// Other operators...
	Value * ICmpEQ( Value * left, Value * right );
	Value * ICmpNE( Value * left, Value * right );
	Value * ICmpSGE( Value * left, Value * right );
	Value * ICmpSGT( Value * left, Value * right );
	Value * ICmpSLE( Value * left, Value * right );
	Value * ICmpSLT( Value * left, Value * right );
	Value * ICmpUGE( Value * left, Value * right );
	Value * ICmpSGT( Value * left, Value * right );
	Value * ICmpULE( Value * left, Value * right );
	Value * ICmpULT( Value * left, Value * right );

	Value * FCmpOEQ( Value * left, Value * right );
	Value * FCmpOGT( Value * left, Value * right );
	Value * FCmpOGE( Value * left, Value * right );
	Value * FCmpOLT( Value * left, Value * right );
	Value * FCmpOLE( Value * left, Value * right );
	Value * FCmpONE( Value * left, Value * right );
	Value * FCmpORD( Value * left, Value * right );
	Value * FCmpUEQ( Value * left, Value * right );
	Value * FCmpUGT( Value * left, Value * right );
	Value * FCmpUGE( Value * left, Value * right );
	Value * FCmpULT( Value * left, Value * right );
	Value * FCmpULE( Value * left, Value * right );
	Value * FCmpUNE( Value * left, Value * right );
	Value * FCmpUNO( Value * left, Value * right );
	Value * FCmpTRUE( Value * left, Value * right );
	Value * FCmpFALSE( Value * left, Value * right );

	Value * PHI( Type * type, XE::Span< XE::Pair< Value *, Label * > > labels );
	Value * Call( Type * type, const XE::String & name, XE::Span< Value * > args );
	Value * Select( Value * cond, Value * iftrue, Value * iffalse );
	Value * ExtractElement( XE::uint64 n, Type * type, Value * vec, Type * type2, XE::uint64 idx );
	Value * InsertElement( XE::uint64 n, Type * type, Value * vec, Type * type2, XE::uint64 elt, Type * type3, XE::uint64 idx );
	Value * ShuffleVector( XE::uint64 n1, Type * type1, Value * vec1, XE::uint64 n2, Type * type2, Value * vec2, XE::Span< Value * > masks );
	Value * ExtractValue( Type * type, Value * value, XE::Span< XE::uint64 > idx );
	Value * InsertValue( Type * type, Value * value, Value * elt, XE::Span< XE::uint64 > idx );

public:
	void Fence();
	void AtomicCmpXchg();
	void AtomicRMW();

public:
	void CleanupPad();
	void CatchPad();
	void LandingPad();
	void Freeze();

protected:
	virtual void ToTargetCode() = 0;

private:
	XE::MemoryStream _Bytecodes;
};

class XE_API ASTAMD64CompileContext : public ASTCompileContext
{
	OBJECT( ASTAMD64CompileContext, ASTCompileContext );

protected:
	void ToTargetCode() override;
};

class XE_API ASTARM64CompileContext : public ASTCompileContext
{
	OBJECT( ASTARM64CompileContext, ASTCompileContext );

protected:
	void ToTargetCode() override;
};

class XE_API ASTRISCVCompileContext : public ASTCompileContext
{
	OBJECT( ASTRISCVCompileContext, ASTCompileContext );

protected:
	void ToTargetCode() override;
};

END_XE_NAMESPACE

#endif//ASTCONTEXT_H__B06A7DDF_7987_49F5_A3B6_78DB4D10A9C5
