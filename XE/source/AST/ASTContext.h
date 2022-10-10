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

	Inst * CreateCondBr( Value * Cond, BasicBlock * True, BasicBlock * False, MDNode * BranchWeights = nullptr, MDNode * Unpredictable = nullptr );

	Inst * CreateCondBr( Value * Cond, BasicBlock * True, BasicBlock * False, Inst * MDSrc );

	Inst * CreateSwitch( Value * V, BasicBlock * Dest, unsigned NumCases = 10, MDNode * BranchWeights = nullptr, MDNode * Unpredictable = nullptr );

	Inst * CreateIndirectBr( Value * Addr, unsigned NumDests = 10 );

	Inst * CreateInvoke( FunctionType * Ty, Value * Callee, BasicBlock * NormalDest, BasicBlock * UnwindDest, XE::Array<Value *> Args, const XE::String & Name = "" );

	Inst * CreateInvoke( FunctionType * Ty, Value * Callee, BasicBlock * NormalDest, BasicBlock * UnwindDest, XE::Array<Value *> Args = {}, const XE::String & Name = "" );

	Inst * CreateInvoke( FunctionCallee Callee, BasicBlock * NormalDest, BasicBlock * UnwindDest, XE::Array<Value *> Args, const XE::String & Name = "" );

	Inst * CreateInvoke( FunctionCallee Callee, BasicBlock * NormalDest, BasicBlock * UnwindDest, XE::Array<Value *> Args = {}, const XE::String & Name = "" );

	Inst * CreateCallBr( FunctionType * Ty, Value * Callee, BasicBlock * DefaultDest, XE::Array<BasicBlock *> IndirectDests, XE::Array<Value *> Args = {}, const XE::String & Name = "" );

	Inst * CreateCallBr( FunctionType * Ty, Value * Callee, BasicBlock * DefaultDest, XE::Array<BasicBlock *> IndirectDests,  XE::Array<Value *> Args, const XE::String & Name = "" );

	Inst * CreateCallBr( FunctionCallee Callee, BasicBlock * DefaultDest, XE::Array<BasicBlock *> IndirectDests, XE::Array<Value *> Args = {}, const XE::String & Name = "" );

	Inst * CreateCallBr( FunctionCallee Callee, BasicBlock * DefaultDest, XE::Array<BasicBlock *> IndirectDests, XE::Array<Value *> Args, const XE::String & Name = "" );

	Inst * CreateResume( Value * Exn );

	Inst * CreateCleanupRet( CleanupPadInst * CleanupPad, BasicBlock * UnwindBB = nullptr );

	Inst * CreateCatchSwitch( Value * ParentPad, BasicBlock * UnwindBB, unsigned NumHandlers, const XE::String & Name = "" );

	Inst * CreateCatchPad( Value * ParentPad, XE::Array<Value *> Args, const XE::String & Name = "" );

	Inst * CreateCleanupPad( Value * ParentPad, XE::Array<Value *> Args = {}, const XE::String & Name = "" );

	Inst * CreateCatchRet( Inst * CatchPad, BasicBlock * BB );

	Inst * CreateUnreachable();

public:
	Value * CreateAdd( Value * LHS, Value * RHS, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateNSWAdd( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateNUWAdd( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateSub( Value * LHS, Value * RHS, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateNSWSub( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateNUWSub( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateMul( Value * LHS, Value * RHS, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateNSWMul( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateNUWMul( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateUDiv( Value * LHS, Value * RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateExactUDiv( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateSDiv( Value * LHS, Value * RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateExactSDiv( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateURem( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateSRem( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateShl( Value * LHS, Value * RHS, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateShl( Value * LHS, Value * RHS, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateShl( Value * LHS, uint64_t RHS, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateLShr( Value * LHS, Value * RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateLShr( Value * LHS, Value * RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateLShr( Value * LHS, uint64_t RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateAShr( Value * LHS, Value * RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateAShr( Value * LHS, Value * RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateAShr( Value * LHS, uint64_t RHS, const XE::String & Name = "", bool isExact = false );

	Value * CreateAnd( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateAnd( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateAnd( Value * LHS, uint64_t RHS, const XE::String & Name = "" );

	Value * CreateAnd( const XE::Array< Value *> & Ops );

	Value * CreateOr( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateOr( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateOr( Value * LHS, uint64_t RHS, const XE::String & Name = "" );

	Value * CreateOr( const XE::Array< Value *> & Ops );

	Value * CreateXor( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateXor( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateXor( Value * LHS, uint64_t RHS, const XE::String & Name = "" );

	Value * CreateFAdd( Value * L, Value * R, const XE::String & Name = "", MDNode * FPMD = nullptr );

	Value * CreateFAddFMF( Value * L, Value * R, Inst * FMFSource, const XE::String & Name = "" );

	Value * CreateFSub( Value * L, Value * R, const XE::String & Name = "", MDNode * FPMD = nullptr );

	Value * CreateFSubFMF( Value * L, Value * R, Inst * FMFSource, const XE::String & Name = "" );

	Value * CreateFMul( Value * L, Value * R, const XE::String & Name = "", MDNode * FPMD = nullptr );

	Value * CreateFMulFMF( Value * L, Value * R, Inst * FMFSource, const XE::String & Name = "" );

	Value * CreateFDiv( Value * L, Value * R, const XE::String & Name = "", MDNode * FPMD = nullptr );

	Value * CreateFDivFMF( Value * L, Value * R, Inst * FMFSource, const XE::String & Name = "" );

	Value * CreateFRem( Value * L, Value * R, const XE::String & Name = "", MDNode * FPMD = nullptr );

	Value * CreateFRemFMF( Value * L, Value * R, Inst * FMFSource, const XE::String & Name = "" );

	Value * CreateBinOp( XE::BinaryExprType Opc, Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateLogicalAnd( Value * Cond1, Value * Cond2, const XE::String & Name = "" );

	Value * CreateLogicalOr( Value * Cond1, Value * Cond2, const XE::String & Name = "" );

	Value * CreateLogicalOr( const XE::Array< Value *> & Ops );

	Inst * CreateConstrainedFPBinOp( Intrinsic::ID ID, Value * L, Value * R, Inst * FMFSource = nullptr, const XE::String & Name = "", MDNode * FPMathTag = nullptr, Optional<RoundingMode> Rounding = {}, Optional<fp::ExceptionBehavior> Except = {} );

	Value * CreateNeg( Value * V, const XE::String & Name = "", bool HasNUW = false, bool HasNSW = false );

	Value * CreateNSWNeg( Value * V, const XE::String & Name = "" );

	Value * CreateNUWNeg( Value * V, const XE::String & Name = "" );

	Value * CreateFNeg( Value * V, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFNegFMF( Value * V, Inst * FMFSource, const XE::String & Name = "" );

	Value * CreateNot( Value * V, const XE::String & Name = "" );

	Value * CreateUnOp( XE::UnaryExprType Opc, Value * V, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateNAryOp( unsigned Opc, const XE::Array< Value *> & Ops, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Inst * CreateAlloca( Type * Ty, unsigned AddrSpace, Value * ArraySize = nullptr, const XE::String & Name = "" );

	Inst * CreateAlloca( Type * Ty, Value * ArraySize = nullptr, const XE::String & Name = "" );

	Inst * CreateLoad( Type * Ty, Value * Ptr, const char * Name );

	Inst * CreateLoad( Type * Ty, Value * Ptr, const XE::String & Name = "" );

	Inst * CreateLoad( Type * Ty, Value * Ptr, bool isVolatile, const XE::String & Name = "" );

	Inst * CreateStore( Value * Val, Value * Ptr, bool isVolatile = false );

	Inst * CreateAlignedLoad( Type * Ty, Value * Ptr, MaybeAlign Align, const char * Name );

	Inst * CreateAlignedLoad( Type * Ty, Value * Ptr, MaybeAlign Align, const XE::String & Name = "" );

	Inst * CreateAlignedLoad( Type * Ty, Value * Ptr, MaybeAlign Align, bool isVolatile, const XE::String & Name = "" );

	Inst * CreateAlignedStore( Value * Val, Value * Ptr, MaybeAlign Align, bool isVolatile = false );

	Inst * CreateFence( AtomicOrdering Ordering, SyncScope::ID SSID = SyncScope::System, const XE::String & Name = "" );

	Inst * CreateAtomicCmpXchg( Value * Ptr, Value * Cmp, Value * New, MaybeAlign Align, AtomicOrdering SuccessOrdering, AtomicOrdering FailureOrdering, SyncScope::ID SSID = SyncScope::System );

	Inst * CreateAtomicRMW( Inst::BinOp Op, Value * Ptr, Value * Val, MaybeAlign Align, AtomicOrdering Ordering, SyncScope::ID SSID = SyncScope::System );

	Value * CreateGEP( Type * Ty, Value * Ptr, const XE::Array< Value *> & IdxList, const XE::String & Name = "", bool IsInBounds = false );

	Value * CreateInBoundsGEP( Type * Ty, Value * Ptr, const XE::Array< Value *> & IdxList, const XE::String & Name = "" );

	Value * CreateConstGEP1_32( Type * Ty, Value * Ptr, unsigned Idx0, const XE::String & Name = "" );

	Value * CreateConstInBoundsGEP1_32( Type * Ty, Value * Ptr, unsigned Idx0, const XE::String & Name = "" );

	Value * CreateConstGEP2_32( Type * Ty, Value * Ptr, unsigned Idx0, unsigned Idx1, const XE::String & Name = "" );

	Value * CreateConstInBoundsGEP2_32( Type * Ty, Value * Ptr, unsigned Idx0, unsigned Idx1, const XE::String & Name = "" );

	Value * CreateConstGEP1_64( Type * Ty, Value * Ptr, uint64_t Idx0, const XE::String & Name = "" );

	Value * CreateConstInBoundsGEP1_64( Type * Ty, Value * Ptr, uint64_t Idx0, const XE::String & Name = "" );

	Value * CreateConstGEP2_64( Type * Ty, Value * Ptr, uint64_t Idx0, uint64_t Idx1, const XE::String & Name = "" );

	Value * CreateConstInBoundsGEP2_64( Type * Ty, Value * Ptr, uint64_t Idx0, uint64_t Idx1, const XE::String & Name = "" );

	Value * CreateStructGEP( Type * Ty, Value * Ptr, unsigned Idx, const XE::String & Name = "" );

	Constant * CreateGlobalStringPtr( StringRef Str, const XE::String & Name = "", unsigned AddressSpace = 0, Module * M = nullptr );

	Value * CreateTrunc( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateZExt( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateSExt( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateZExtOrTrunc( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateSExtOrTrunc( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateFPToUI( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateFPToSI( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateUIToFP( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateSIToFP( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateFPTrunc( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateFPExt( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreatePtrToInt( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateIntToPtr( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateBitCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateAddrSpaceCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateZExtOrBitCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateSExtOrBitCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateTruncOrBitCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreatePointerCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreatePointerBitCastOrAddrSpaceCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateIntCast( Value * V, Type * DestTy, bool isSigned, const XE::String & Name = "" );

	Value * CreateBitOrPointerCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Value * CreateFPCast( Value * V, Type * DestTy, const XE::String & Name = "" );

	Inst * CreateConstrainedFPCast( Intrinsic::ID ID, Value * V, Type * DestTy, Inst * FMFSource = nullptr, const XE::String & Name = "", MDNode * FPMathTag = nullptr, Optional<RoundingMode> Rounding = {}, Optional<fp::ExceptionBehavior> Except = {} );

	Value * CreateIntCast( Value *, Type *, const char * ) = delete;

	Value * CreateICmpEQ( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpNE( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpUGT( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpUGE( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpULT( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpULE( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpSGT( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpSGE( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpSLT( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateICmpSLE( Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateFCmpOEQ( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpOGT( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpOGE( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpOLT( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpOLE( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpONE( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpORD( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpUNO( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpUEQ( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpUGT( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpUGE( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpULT( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpULE( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpUNE( Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateICmp( CmpInst::Predicate P, Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateFCmp( CmpInst::Predicate P, Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateCmp( CmpInst::Predicate Pred, Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Value * CreateFCmpS( CmpInst::Predicate P, Value * LHS, Value * RHS, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

public:
	Inst * CreateConstrainedFPCmp( Intrinsic::ID ID, CmpInst::Predicate P, Value * L, Value * R, const XE::String & Name = "", Optional<fp::ExceptionBehavior> Except = {} );

	PHINode * CreatePHI( Type * Ty, unsigned NumReservedValues, const XE::String & Name = "" );

public:
	Inst * CreateCall( FunctionType * FTy, Value * Callee, const XE::Array< Value *> & Args = {}, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Inst * CreateCall( FunctionType * FTy, Value * Callee, const XE::Array< Value *> & Args, ArrayRef<OperandBundleDef> OpBundles, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Inst * CreateCall( FunctionCallee Callee, const XE::Array< Value *> & Args = {}, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Inst * CreateCall( FunctionCallee Callee, const XE::Array< Value *> & Args, ArrayRef<OperandBundleDef> OpBundles, const XE::String & Name = "", MDNode * FPMathTag = nullptr );

	Inst * CreateConstrainedFPCall( Function * Callee, const XE::Array< Value *> & Args, const XE::String & Name = "", Optional<RoundingMode> Rounding = {}, Optional<fp::ExceptionBehavior> Except = {} );

	Value * CreateSelect( Value * C, Value * True, Value * False, const XE::String & Name = "", Inst * MDFrom = nullptr );

	Inst * CreateVAArg( Value * List, Type * Ty, const XE::String & Name = "" );

	Value * CreateExtractElement( Value * Vec, Value * Idx, const XE::String & Name = "" );

	Value * CreateExtractElement( Value * Vec, uint64_t Idx, const XE::String & Name = "" );

	Value * CreateInsertElement( Type * VecTy, Value * NewElt, Value * Idx, const XE::String & Name = "" );

	Value * CreateInsertElement( Type * VecTy, Value * NewElt, uint64_t Idx, const XE::String & Name = "" );

	Value * CreateInsertElement( Value * Vec, Value * NewElt, Value * Idx, const XE::String & Name = "" );

	Value * CreateInsertElement( Value * Vec, Value * NewElt, uint64_t Idx, const XE::String & Name = "" );

	Value * CreateShuffleVector( Value * V1, Value * V2, Value * Mask, const XE::String & Name = "" );

	Value * CreateShuffleVector( Value * V1, Value * V2, ArrayRef<int> Mask, const XE::String & Name = "" );

	Value * CreateShuffleVector( Value * V, ArrayRef<int> Mask, const XE::String & Name = "" );

	Value * CreateExtractValue( Value * Agg, ArrayRef<unsigned> Idxs, const XE::String & Name = "" );

	Value * CreateInsertValue( Value * Agg, Value * Val, ArrayRef<unsigned> Idxs, const XE::String & Name = "" );

	Inst * CreateLandingPad( Type * Ty, unsigned NumClauses, const XE::String & Name = "" );

	Value * CreateFreeze( Value * V, const XE::String & Name = "" );

	Value * CreateIsNull( Value * Arg, const XE::String & Name = "" );

	Value * CreateIsNotNull( Value * Arg, const XE::String & Name = "" );

	Value * CreateIsNeg( Value * Arg, const XE::String & Name = "" );

	Value * CreateIsNotNeg( Value * Arg, const XE::String & Name = "" );

	Value * CreatePtrDiff( Type * ElemTy, Value * LHS, Value * RHS, const XE::String & Name = "" );

	Value * CreateLaunderInvariantGroup( Value * Ptr );

	Value * CreateStripInvariantGroup( Value * Ptr );

	Value * CreateVectorReverse( Value * V, const XE::String & Name = "" );

	Value * CreateVectorSplice( Value * V1, Value * V2, int64_t Imm, const XE::String & Name = "" );

	Value * CreateVectorSplat( unsigned NumElts, Value * V, const XE::String & Name = "" );

	Value * CreateVectorSplat( ElementCount EC, Value * V, const XE::String & Name = "" );

	Value * CreateExtractInteger( const DataLayout & DL, Value * From, IntegerType * ExtractedTy, uint64_t Offset, const XE::String & Name );

	Value * CreatePreserveArrayAccessIndex( Type * ElTy, Value * Base, unsigned Dimension, unsigned LastIndex, MDNode * DbgInfo );

	Value * CreatePreserveUnionAccessIndex( Value * Base, unsigned FieldIndex, MDNode * DbgInfo );

	Value * CreatePreserveStructAccessIndex( Type * ElTy, Value * Base, unsigned Index, unsigned FieldIndex, MDNode * DbgInfo );

public:
	Inst * CreateAlignmentAssumption( const DataLayout & DL, Value * PtrValue, unsigned Alignment, Value * OffsetValue = nullptr );

	Inst * CreateAlignmentAssumption( const DataLayout & DL, Value * PtrValue, Value * Alignment, Value * OffsetValue = nullptr );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//ASTCONTEXT_H__B06A7DDF_7987_49F5_A3B6_78DB4D10A9C5
