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
	ASSIGN,         // x=y
	ADD_ASSIGN,     // x+=y
	SUB_ASSIGN,     // x-=y
	MUL_ASSIGN,     // x*=y
	DIV_ASSIGN,     // x/=y
	MOD_ASSIGN,     // x%=y
	B_OR_ASSIGN,    // x|=y
	B_AND_ASSIGN,   // x&=y
	B_XOR_ASSIGN,   // x^=y
	L_SHIFT_ASSIGN, // x<<=y
	R_SHIFT_ASSIGN, // x>>=y
	ELEMENT,        // x.y
	INDEX_ELEMENT,  // x[y]
};
DECL_XE_ENUM( BinaryExprType );

END_XE_NAMESPACE

#endif//TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528
