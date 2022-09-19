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

enum class Instcode
{

};

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

enum class ExecuteGotoType
{
	NONE,
	BREAK,
	RETURN,
	CONTINUE,
};

struct XE_API ASTFrame
{
	XE::uint64 SP = 0;
	XE::uint64 FP = 0;
	XE::Stack< XE::MacroGotoTypeFlags > MacroGo;
	XE::ExecuteGotoType ExecGo = XE::ExecuteGotoType::NONE;
	XE::Stack< const XE::WhileStatNode * > Loop;
	XE::Map< XE::String, XE::uint64 > Variables;
	std::variant< XE::ASTInfoMethodPtr, XE::ASTInfoFunctionPtr > AST;
};

END_XE_NAMESPACE

#endif//TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528
