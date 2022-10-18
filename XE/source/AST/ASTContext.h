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

public:
	Type * CreateType( const XE::MetaClassCPtr & val, XE::TypeFlag flag );

	Value * CreateValue( Type * val );

	Value * CreateConstant( bool val );

	Value * CreateConstant( XE::int8 val );

	Value * CreateConstant( XE::int16 val );

	Value * CreateConstant( XE::int32 val );

	Value * CreateConstant( XE::int64 val );

	Value * CreateConstant( XE::uint8 val );

	Value * CreateConstant( XE::uint16 val );

	Value * CreateConstant( XE::uint32 val );

	Value * CreateConstant( XE::uint64 val );

	Value * CreateConstant( XE::float16 val );

	Value * CreateConstant( XE::float32 val );

	Value * CreateConstant( XE::float64 val );

	Value * CreateConstant( const XE::String & val );

	BasicBlock * CreateBasicBlock( const XE::String & val );

public:
	void PushBasicBlock( BasicBlock * val );

	BasicBlock * TopBasicBlock();

	BasicBlock * PopBasicBlock();

public:
	Inst * CreateFAddReduce( Value * Acc, Value * Src );

	Inst * CreateFMulReduce( Value * Acc, Value * Src );

	Inst * CreateAddReduce( Value * Src );

	Inst * CreateMulReduce( Value * Src );

	Inst * CreateAndReduce( Value * Src );

	Inst * CreateOrReduce( Value * Src );

	Inst * CreateXorReduce( Value * Src );

	Inst * CreateIntMaxReduce( Value * Src, bool IsSigned = false );

	Inst * CreateIntMinReduce( Value * Src, bool IsSigned = false );

	Inst * CreateFPMaxReduce( Value * Src );

	Inst * CreateFPMinReduce( Value * Src );

	Inst * CreateLifetimeStart( Value * Ptr, Inst * Size = nullptr );

	Inst * CreateLifetimeEnd( Value * Ptr, Inst * Size = nullptr );

	Inst * CreateInvariantStart( Value * Ptr, Inst * Size = nullptr );

	Inst * CreateThreadLocalAddress( Value * Ptr );

public:
	Inst * CreateRetVoid();

	Inst * CreateRet( Value * V );

	Inst * CreateAggregateRet( Value * const * retVals, unsigned N );

	Inst * CreateBr( BasicBlock * Dest );

	Inst * CreateCondBr( Value * Cond, BasicBlock * True, BasicBlock * False );

	Inst * CreateSwitch( Value * V, BasicBlock * Dest, unsigned NumCases = 10 );

	Inst * CreateIndirectBr( Value * Addr, unsigned NumDests = 10 );

	Inst * CreateInvoke( Value * Callee, BasicBlock * NormalDest, BasicBlock * UnwindDest, XE::Span<Value *> Args );

	Inst * CreateCallBr( Value * Callee, BasicBlock * DefaultDest, XE::Span<BasicBlock *> IndirectDests, XE::Span<Value *> Args );

	Inst * CreateResume( Value * Exn );

	Inst * CreateCleanupRet( Inst * CleanupPad, BasicBlock * UnwindBB = nullptr );

	Inst * CreateCatchSwitch( Value * ParentPad, BasicBlock * UnwindBB, unsigned NumHandlers );

	Inst * CreateCatchPad( Value * ParentPad, XE::Span<Value *> Args );

	Inst * CreateCleanupPad( Value * ParentPad, XE::Span<Value *> Args = {} );

	Inst * CreateCatchRet( Inst * CatchPad, BasicBlock * BB );

	Inst * CreateUnreachable();

public:
	Value * CreateAdd( Value * LHS, Value * RHS );

	Value * CreateSub( Value * LHS, Value * RHS );

	Value * CreateMul( Value * LHS, Value * RHS );

	Value * CreateUDiv( Value * LHS, Value * RHS );

	Value * CreateSDiv( Value * LHS, Value * RHS );

	Value * CreateURem( Value * LHS, Value * RHS );

	Value * CreateSRem( Value * LHS, Value * RHS );

	Value * CreateShl( Value * LHS, Value * RHS );

	Value * CreateShl( Value * LHS, Value * RHS );

	Value * CreateShl( Value * LHS, uint64_t RHS );

	Value * CreateLShr( Value * LHS, Value * RHS );

	Value * CreateAShr( Value * LHS, Value * RHS );

	Value * CreateAnd( Value * LHS, Value * RHS );

	Value * CreateOr( Value * LHS, Value * RHS );

	Value * CreateXor( Value * LHS, Value * RHS );

	Value * CreateFAdd( Value * L, Value * R );

	Value * CreateFSub( Value * L, Value * R );

	Value * CreateFMul( Value * L, Value * R );

	Value * CreateFDiv( Value * L, Value * R );

	Value * CreateFRem( Value * L, Value * R );

	Value * CreateBinOp( XE::BinaryExprType Opc, Value * LHS, Value * RHS );

	Value * CreateLogicalAnd( Value * Cond1, Value * Cond2 );

	Value * CreateLogicalOr( Value * Cond1, Value * Cond2 );

	Value * CreateLogicalOr( const XE::Span< Value *> & Ops );

	Value * CreateNeg( Value * V );

	Value * CreateNSWNeg( Value * V );

	Value * CreateNUWNeg( Value * V );

	Value * CreateFNeg( Value * V );

	Value * CreateNot( Value * V );

	Value * CreateUnOp( XE::UnaryExprType Opc, Value * V );

	Inst * CreateAlloca( Type * Ty, Value * ArraySize = nullptr );

	Inst * CreateLoad( Type * Ty, Value * Ptr );

	Inst * CreateStore( Value * Val, Value * Ptr );

	Value * CreateGEP( Type * Ty, Value * Ptr, const XE::Span< Value *> & IdxList );

	Value * CreateGlobalStringPtr( const XE::String & Str );

	Value * CreateTrunc( Value * V, Type * DestTy );

	Value * CreateZExt( Value * V, Type * DestTy );

	Value * CreateSExt( Value * V, Type * DestTy );

	Value * CreateZExtOrTrunc( Value * V, Type * DestTy );

	Value * CreateSExtOrTrunc( Value * V, Type * DestTy );

	Value * CreateFPToUI( Value * V, Type * DestTy );

	Value * CreateFPToSI( Value * V, Type * DestTy );

	Value * CreateUIToFP( Value * V, Type * DestTy );

	Value * CreateSIToFP( Value * V, Type * DestTy );

	Value * CreateFPTrunc( Value * V, Type * DestTy );

	Value * CreateFPExt( Value * V, Type * DestTy );

	Value * CreatePtrToInt( Value * V, Type * DestTy );

	Value * CreateIntToPtr( Value * V, Type * DestTy );

	Value * CreateBitCast( Value * V, Type * DestTy );

	Value * CreateAddrSpaceCast( Value * V, Type * DestTy );

	Value * CreateZExtOrBitCast( Value * V, Type * DestTy );

	Value * CreateSExtOrBitCast( Value * V, Type * DestTy );

	Value * CreateTruncOrBitCast( Value * V, Type * DestTy );

	Value * CreateCast( Value * V, Type * DestTy );

	Value * CreatePointerCast( Value * V, Type * DestTy );

	Value * CreatePointerBitCastOrAddrSpaceCast( Value * V, Type * DestTy );

	Value * CreateIntCast( Value * V, Type * DestTy, bool isSigned );

	Value * CreateBitOrPointerCast( Value * V, Type * DestTy );

	Value * CreateFPCast( Value * V, Type * DestTy );

	Value * CreateICmpEQ( Value * LHS, Value * RHS );

	Value * CreateICmpNE( Value * LHS, Value * RHS );

	Value * CreateICmpUGT( Value * LHS, Value * RHS );

	Value * CreateICmpUGE( Value * LHS, Value * RHS );

	Value * CreateICmpULT( Value * LHS, Value * RHS );

	Value * CreateICmpULE( Value * LHS, Value * RHS );

	Value * CreateICmpSGT( Value * LHS, Value * RHS );

	Value * CreateICmpSGE( Value * LHS, Value * RHS );

	Value * CreateICmpSLT( Value * LHS, Value * RHS );

	Value * CreateICmpSLE( Value * LHS, Value * RHS );

	Value * CreateFCmpOEQ( Value * LHS, Value * RHS );

	Value * CreateFCmpOGT( Value * LHS, Value * RHS );

	Value * CreateFCmpOGE( Value * LHS, Value * RHS );

	Value * CreateFCmpOLT( Value * LHS, Value * RHS );

	Value * CreateFCmpOLE( Value * LHS, Value * RHS );

	Value * CreateFCmpONE( Value * LHS, Value * RHS );

	Value * CreateFCmpORD( Value * LHS, Value * RHS );

	Value * CreateFCmpUNO( Value * LHS, Value * RHS );

	Value * CreateFCmpUEQ( Value * LHS, Value * RHS );

	Value * CreateFCmpUGT( Value * LHS, Value * RHS );

	Value * CreateFCmpUGE( Value * LHS, Value * RHS );

	Value * CreateFCmpULT( Value * LHS, Value * RHS );

	Value * CreateFCmpULE( Value * LHS, Value * RHS );

	Value * CreateFCmpUNE( Value * LHS, Value * RHS );

public:
	Inst * CreatePHI( Type * Ty, unsigned NumReservedValues );

public:
	Inst * CreateCall( Value * Callee, const XE::Span< Value *> & Args );

	Value * CreateSelect( Value * C, Value * True, Value * False );

	Value * CreateExtractElement( Value * Vec, Value * Idx );

	Value * CreateExtractElement( Value * Vec, uint64_t Idx );

	Value * CreateInsertElement( Type * VecTy, Value * NewElt, Value * Idx );

	Value * CreateInsertElement( Type * VecTy, Value * NewElt, uint64_t Idx );

	Value * CreateInsertElement( Value * Vec, Value * NewElt, Value * Idx );

	Value * CreateInsertElement( Value * Vec, Value * NewElt, uint64_t Idx );

	Value * CreateShuffleVector( Value * V1, Value * V2, Value * Mask );

	Value * CreateShuffleVector( Value * V1, Value * V2, XE::Span<int> Mask );

	Value * CreateShuffleVector( Value * V, XE::Span<int> Mask );

	Value * CreateExtractValue( Value * Agg, XE::Span<unsigned> Idxs );

	Value * CreateInsertValue( Value * Agg, Value * Val, XE::Span<unsigned> Idxs );

	Inst * CreateLandingPad( Type * Ty, unsigned NumClauses );

	Value * CreateFreeze( Value * V );

	Value * CreateIsNull( Value * Arg );

	Value * CreateIsNotNull( Value * Arg );

	Value * CreateIsNeg( Value * Arg );

	Value * CreateIsNotNeg( Value * Arg );

	Value * CreatePtrDiff( Type * ElemTy, Value * LHS, Value * RHS );

	Value * CreateLaunderInvariantGroup( Value * Ptr );

	Value * CreateStripInvariantGroup( Value * Ptr );

	Value * CreateVectorReverse( Value * V );

	Value * CreateVectorSplice( Value * V1, Value * V2, int64_t Imm );

	Value * CreateVectorSplat( unsigned NumElts, Value * V );

	Value * CreatePreserveArrayAccessIndex( Type * ElTy, Value * Base, unsigned Dimension, unsigned LastIndex );

	Value * CreatePreserveUnionAccessIndex( Value * Base, unsigned FieldIndex );

	Value * CreatePreserveStructAccessIndex( Type * ElTy, Value * Base, unsigned Index, unsigned FieldIndex );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//ASTCONTEXT_H__B06A7DDF_7987_49F5_A3B6_78DB4D10A9C5
