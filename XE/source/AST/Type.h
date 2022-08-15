/*!
 * \file	Type.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/09
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528
#define TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528

#include "Declare.h"
#include "Utils/Flags.hpp"

BEG_XE_NAMESPACE

enum class UnaryExprType
{
	NOT, // !x
	NEG, // -x
	RMO, // ~x
};
DECL_XE_ENUM( UnaryExprType );

enum class BinaryExprType
{
	ADD,            // x+y
	SUB,            // x-y
	MUL,            // x*y
	DIV,            // x/y
	MOD,            // x%y
	L_OR,           // x||y
	L_AND,          // x&&y
	B_OR,           // x|y
	B_XOR,          // x^y
	B_AND,          // x&y
	L_SHIFT,        // x<<y
	R_SHIFT,        // x>>y
	LT,             // x<y
	GT,             // x>y
	EQUAL,          // x==y
	LT_EQUAL,       // x<=y
	GT_EQUAL,       // x>=y
	NOT_EQUAL,      // x!=y
	INDEX,          // x[y]
};
DECL_XE_ENUM( BinaryExprType );

enum class MacroGotoType
{
	NONE = 0,
	THEN = 1 << 0,
	ELSE = 1 << 1,
	END = 1 << 2,
	THEN_END = THEN | END,
	ELSE_END = ELSE | END,
};
DECL_FLAGS( MacroGotoType );

enum class ExecutorGotoType
{
	NONE,
	BREAK,
	RETURN,
	CONTINUE,
};

class XE_API ASTEnum : public XE::EnableSharedFromThis< XE::ASTEnum >
{
public:
	XE::String Name;
	XE::String Module;
	bool Flag = false;
	XE::Array< XE::String > Elements;
};
DECL_XE_CLASS( ASTEnum );

class XE_API ASTClass : public XE::EnableSharedFromThis < XE::ASTClass >
{
public:
	XE::String Name;
	XE::String Super;
	XE::String Module;
	XE::SharedPtr< XE::ASTMethod > Destruct;
	XE::SharedPtr< XE::ASTMethod > Construct;
	XE::Array< XE::SharedPtr< XE::ASTMethod > > Methods;
	XE::Array< XE::SharedPtr< XE::ASTProperty > > Propertys;
};
DECL_XE_CLASS( ASTClass );

class XE_API ASTModule : public XE::EnableSharedFromThis < XE::ASTModule >
{
public:
	XE::String Name;
	XE::String Owner;
	XE::Array< XE::SharedPtr< XE::ASTEnum > > Enums;
	XE::Array< XE::SharedPtr< XE::ASTClass > > Clsses;
	XE::Array< XE::SharedPtr< XE::ASTModule > > Modules;
	XE::Array< XE::SharedPtr< XE::ASTFunction > > Functions;
	XE::Array< XE::SharedPtr< XE::ASTVariable > > Variables;
};
DECL_XE_CLASS( ASTModule );

class XE_API ASTMethod : public XE::EnableSharedFromThis < XE::ASTMethod >
{
public:
	XE::String Name;
	XE::String Owner;
	XE::String Module;
	XE::String Result;
	XE::Array< XE::ASTNodePtr > StatementBody;
	XE::Array< XE::Pair< XE::String, XE::String > > Parameters;
	XE::Array< XE::Pair< XE::String, XE::Variant > > LocalVariables;
};
DECL_XE_CLASS( ASTMethod );

class XE_API ASTProperty : public XE::EnableSharedFromThis < XE::ASTProperty >
{
public:
	XE::String Name;
	XE::String Owner;
	XE::String Module;
	XE::String Type;
	XE::TypeFlag Flag;
	XE::uint64 Offset;
};
DECL_XE_CLASS( ASTProperty );

class XE_API ASTFunction : public XE::EnableSharedFromThis < XE::ASTFunction >
{
public:
	XE::String Name;
	XE::String Module;
	XE::String Result;
	XE::Array< XE::ASTNodePtr > StatementBody;
	XE::Array< XE::Pair< XE::String, XE::String > > Parameters;
	XE::Array< XE::Pair< XE::String, XE::Variant > > LocalVariables;
};
DECL_XE_CLASS( ASTFunction );

class XE_API ASTVariable : public XE::EnableSharedFromThis < XE::ASTVariable >
{
public:
	XE::String Name;
	XE::String Module;
	XE::String Type;
	XE::TypeFlag Flag;
};
DECL_XE_CLASS( ASTVariable );

struct XE_API ASTFrame
{
	XE::uint64 SP = 0;
	XE::uint64 FP = 0;
	XE::Stack< XE::MacroGotoTypeFlags > MacroGo;
	XE::ExecutorGotoType ExecGo = XE::ExecutorGotoType::NONE;
	XE::Stack< const XE::WhileStatNode * > Loop;
	XE::Map< XE::String, XE::uint64 > Variables;
	std::variant< XE::ASTMethod *, XE::ASTFunction * > AST;
};

END_XE_NAMESPACE

#endif//TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528
