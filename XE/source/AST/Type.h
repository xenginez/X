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


struct XE_API ASTEnum : public XE::EnableSharedFromThis< XE::ASTEnum >
{
	XE::String Name;
	bool Flag = false;
	XE::Array< XE::String > Elements;
};
DECL_XE_CLASS( ASTEnum );

struct XE_API ASTClass : public XE::EnableSharedFromThis < XE::ASTClass >
{
	XE::String Name;
	XE::String Super;
	XE::String Module;
	XE::SharedPtr< XE::ASTMethod > Destruct;
	XE::SharedPtr< XE::ASTMethod > Construct;
	XE::Array< XE::SharedPtr< XE::ASTMethod > > Methods;
	XE::Array< XE::SharedPtr< XE::ASTProperty > > Propertys;
};
DECL_XE_CLASS( ASTClass );

struct XE_API ASTMethod : public XE::EnableSharedFromThis < XE::ASTMethod >
{
	XE::String Name;
	XE::String Owner;
	XE::String Module;
	XE::String Result;
	XE::Array< XE::ASTNodePtr > StatementBody;
	XE::Array< XE::Pair< XE::String, XE::String > > Parameters;
	XE::Array< XE::Pair< XE::String, XE::Variant > > LocalVariables;
};
DECL_XE_CLASS( ASTMethod );

struct XE_API ASTProperty : public XE::EnableSharedFromThis < XE::ASTProperty >
{
	XE::String Name;
	XE::String Owner;
	XE::String Module;
	XE::String ValueType;
};
DECL_XE_CLASS( ASTProperty );

struct XE_API ASTFunction : public XE::EnableSharedFromThis < XE::ASTFunction >
{
	XE::String Name;
	XE::String Module;
	XE::String Result;
	XE::Array< XE::ASTNodePtr > StatementBody;
	XE::Array< XE::Pair< XE::String, XE::String > > Parameters;
	XE::Array< XE::Pair< XE::String, XE::Variant > > LocalVariables;
};
DECL_XE_CLASS( ASTFunction );

struct XE_API ASTVariable : public XE::EnableSharedFromThis < XE::ASTVariable >
{
	XE::String Name;
	XE::String Module;
	XE::String ValueType;
};
DECL_XE_CLASS( ASTVariable );

END_XE_NAMESPACE

#endif//TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528
