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

enum class ASTOpcode : XE::uint8
{
	/* control instructions */
	OP_UNREACHABLE = 0x00, /* unreachable */
	OP_NOP = 0x01,         /* nop */
	OP_BLOCK = 0x02,       /* block */
	OP_LOOP = 0x03,        /* loop */
	OP_IF = 0x04,          /* if */
	OP_ELSE = 0x05,        /* else */

	OP_UNUSED_0x06 = 0x06,
	OP_UNUSED_0x07 = 0x07,
	OP_UNUSED_0x08 = 0x08,
	OP_UNUSED_0x09 = 0x09,
	OP_UNUSED_0x0a = 0x0a,

	OP_END = 0x0b,                  /* end */
	OP_BR = 0x0c,                   /* br */
	OP_BR_IF = 0x0d,                /* br if */
	OP_BR_TABLE = 0x0e,             /* br table */
	OP_RETURN = 0x0f,               /* return */
	OP_CALL = 0x10,                 /* call */
	OP_CALL_INDIRECT = 0x11,        /* call_indirect */
	OP_RETURN_CALL = 0x12,          /* return_call */
	OP_RETURN_CALL_INDIRECT = 0x13, /* return_call_indirect */

	OP_UNUSED_0x14 = 0x14,
	OP_UNUSED_0x15 = 0x15,
	OP_UNUSED_0x16 = 0x16,
	OP_UNUSED_0x17 = 0x17,
	OP_UNUSED_0x18 = 0x18,
	OP_UNUSED_0x19 = 0x19,

	/* parametric instructions */
	OP_DROP = 0x1a,     /* drop */
	OP_SELECT = 0x1b,   /* select */
	OP_SELECT_T = 0x1c, /* select t */

	OP_GET_GLOBAL_64 = 0x1d,
	OP_SET_GLOBAL_64 = 0x1e,
	OP_SET_GLOBAL_AUX_STACK = 0x1f,

	/* variable instructions */
	OP_GET_LOCAL = 0x20,  /* get_local */
	OP_SET_LOCAL = 0x21,  /* set_local */
	OP_TEE_LOCAL = 0x22,  /* tee_local */
	OP_GET_GLOBAL = 0x23, /* get_global */
	OP_SET_GLOBAL = 0x24, /* set_global */

	OP_TABLE_GET = 0x25, /* table.get */
	OP_TABLE_SET = 0x26, /* table.set */
	OP_UNUSED_0x27 = 0x27,

	/* memory instructions */
	OP_I32_LOAD = 0x28,     /* i32.load */
	OP_I64_LOAD = 0x29,     /* i64.load */
	OP_F32_LOAD = 0x2a,     /* f32.load */
	OP_F64_LOAD = 0x2b,     /* f64.load */
	OP_I32_LOAD8_S = 0x2c,  /* i32.load8_s */
	OP_I32_LOAD8_U = 0x2d,  /* i32.load8_u */
	OP_I32_LOAD16_S = 0x2e, /* i32.load16_s */
	OP_I32_LOAD16_U = 0x2f, /* i32.load16_u */
	OP_I64_LOAD8_S = 0x30,  /* i64.load8_s */
	OP_I64_LOAD8_U = 0x31,  /* i64.load8_u */
	OP_I64_LOAD16_S = 0x32, /* i64.load16_s */
	OP_I64_LOAD16_U = 0x33, /* i64.load16_u */
	OP_I64_LOAD32_S = 0x34, /* i32.load32_s */
	OP_I64_LOAD32_U = 0x35, /* i32.load32_u */
	OP_I32_STORE = 0x36,    /* i32.store */
	OP_I64_STORE = 0x37,    /* i64.store */
	OP_F32_STORE = 0x38,    /* f32.store */
	OP_F64_STORE = 0x39,    /* f64.store */
	OP_I32_STORE8 = 0x3a,   /* i32.store8 */
	OP_I32_STORE16 = 0x3b,  /* i32.store16 */
	OP_I64_STORE8 = 0x3c,   /* i64.store8 */
	OP_I64_STORE16 = 0x3d,  /* i64.sotre16 */
	OP_I64_STORE32 = 0x3e,  /* i64.store32 */
	OP_MEMORY_SIZE = 0x3f,  /* memory.size */
	OP_MEMORY_GROW = 0x40,  /* memory.grow */

	/* constant instructions */
	OP_I32_CONST = 0x41, /* i32.const */
	OP_I64_CONST = 0x42, /* i64.const */
	OP_F32_CONST = 0x43, /* f32.const */
	OP_F64_CONST = 0x44, /* f64.const */

	/* comparison instructions */
	OP_I32_EQZ = 0x45,  /* i32.eqz */
	OP_I32_EQ = 0x46,   /* i32.eq */
	OP_I32_NE = 0x47,   /* i32.ne */
	OP_I32_LT_S = 0x48, /* i32.lt_s */
	OP_I32_LT_U = 0x49, /* i32.lt_u */
	OP_I32_GT_S = 0x4a, /* i32.gt_s */
	OP_I32_GT_U = 0x4b, /* i32.gt_u */
	OP_I32_LE_S = 0x4c, /* i32.le_s */
	OP_I32_LE_U = 0x4d, /* i32.le_u */
	OP_I32_GE_S = 0x4e, /* i32.ge_s */
	OP_I32_GE_U = 0x4f, /* i32.ge_u */

	OP_I64_EQZ = 0x50,  /* i64.eqz */
	OP_I64_EQ = 0x51,   /* i64.eq */
	OP_I64_NE = 0x52,   /* i64.ne */
	OP_I64_LT_S = 0x53, /* i64.lt_s */
	OP_I64_LT_U = 0x54, /* i64.lt_u */
	OP_I64_GT_S = 0x55, /* i64.gt_s */
	OP_I64_GT_U = 0x56, /* i64.gt_u */
	OP_I64_LE_S = 0x57, /* i64.le_s */
	OP_I64_LE_U = 0x58, /* i64.le_u */
	OP_I64_GE_S = 0x59, /* i64.ge_s */
	OP_I64_GE_U = 0x5a, /* i64.ge_u */

	OP_F32_EQ = 0x5b, /* f32.eq */
	OP_F32_NE = 0x5c, /* f32.ne */
	OP_F32_LT = 0x5d, /* f32.lt */
	OP_F32_GT = 0x5e, /* f32.gt */
	OP_F32_LE = 0x5f, /* f32.le */
	OP_F32_GE = 0x60, /* f32.ge */

	OP_F64_EQ = 0x61, /* f64.eq */
	OP_F64_NE = 0x62, /* f64.ne */
	OP_F64_LT = 0x63, /* f64.lt */
	OP_F64_GT = 0x64, /* f64.gt */
	OP_F64_LE = 0x65, /* f64.le */
	OP_F64_GE = 0x66, /* f64.ge */

	/* numeric operators */
	OP_I32_CLZ = 0x67,    /* i32.clz */
	OP_I32_CTZ = 0x68,    /* i32.ctz */
	OP_I32_POPCNT = 0x69, /* i32.popcnt */
	OP_I32_ADD = 0x6a,    /* i32.add */
	OP_I32_SUB = 0x6b,    /* i32.sub */
	OP_I32_MUL = 0x6c,    /* i32.mul */
	OP_I32_DIV_S = 0x6d,  /* i32.div_s */
	OP_I32_DIV_U = 0x6e,  /* i32.div_u */
	OP_I32_REM_S = 0x6f,  /* i32.rem_s */
	OP_I32_REM_U = 0x70,  /* i32.rem_u */
	OP_I32_AND = 0x71,    /* i32.and */
	OP_I32_OR = 0x72,     /* i32.or */
	OP_I32_XOR = 0x73,    /* i32.xor */
	OP_I32_SHL = 0x74,    /* i32.shl */
	OP_I32_SHR_S = 0x75,  /* i32.shr_s */
	OP_I32_SHR_U = 0x76,  /* i32.shr_u */
	OP_I32_ROTL = 0x77,   /* i32.rotl */
	OP_I32_ROTR = 0x78,   /* i32.rotr */

	OP_I64_CLZ = 0x79,    /* i64.clz */
	OP_I64_CTZ = 0x7a,    /* i64.ctz */
	OP_I64_POPCNT = 0x7b, /* i64.popcnt */
	OP_I64_ADD = 0x7c,    /* i64.add */
	OP_I64_SUB = 0x7d,    /* i64.sub */
	OP_I64_MUL = 0x7e,    /* i64.mul */
	OP_I64_DIV_S = 0x7f,  /* i64.div_s */
	OP_I64_DIV_U = 0x80,  /* i64.div_u */
	OP_I64_REM_S = 0x81,  /* i64.rem_s */
	OP_I64_REM_U = 0x82,  /* i64.rem_u */
	OP_I64_AND = 0x83,    /* i64.and */
	OP_I64_OR = 0x84,     /* i64.or */
	OP_I64_XOR = 0x85,    /* i64.xor */
	OP_I64_SHL = 0x86,    /* i64.shl */
	OP_I64_SHR_S = 0x87,  /* i64.shr_s */
	OP_I64_SHR_U = 0x88,  /* i64.shr_u */
	OP_I64_ROTL = 0x89,   /* i64.rotl */
	OP_I64_ROTR = 0x8a,   /* i64.rotr */

	OP_F32_ABS = 0x8b,      /* f32.abs */
	OP_F32_NEG = 0x8c,      /* f32.neg */
	OP_F32_CEIL = 0x8d,     /* f32.ceil */
	OP_F32_FLOOR = 0x8e,    /* f32.floor */
	OP_F32_TRUNC = 0x8f,    /* f32.trunc */
	OP_F32_NEAREST = 0x90,  /* f32.nearest */
	OP_F32_SQRT = 0x91,     /* f32.sqrt */
	OP_F32_ADD = 0x92,      /* f32.add */
	OP_F32_SUB = 0x93,      /* f32.sub */
	OP_F32_MUL = 0x94,      /* f32.mul */
	OP_F32_DIV = 0x95,      /* f32.div */
	OP_F32_MIN = 0x96,      /* f32.min */
	OP_F32_MAX = 0x97,      /* f32.max */
	OP_F32_COPYSIGN = 0x98, /* f32.copysign */

	OP_F64_ABS = 0x99,      /* f64.abs */
	OP_F64_NEG = 0x9a,      /* f64.neg */
	OP_F64_CEIL = 0x9b,     /* f64.ceil */
	OP_F64_FLOOR = 0x9c,    /* f64.floor */
	OP_F64_TRUNC = 0x9d,    /* f64.trunc */
	OP_F64_NEAREST = 0x9e,  /* f64.nearest */
	OP_F64_SQRT = 0x9f,     /* f64.sqrt */
	OP_F64_ADD = 0xa0,      /* f64.add */
	OP_F64_SUB = 0xa1,      /* f64.sub */
	OP_F64_MUL = 0xa2,      /* f64.mul */
	OP_F64_DIV = 0xa3,      /* f64.div */
	OP_F64_MIN = 0xa4,      /* f64.min */
	OP_F64_MAX = 0xa5,      /* f64.max */
	OP_F64_COPYSIGN = 0xa6, /* f64.copysign */

	/* conversions */
	OP_I32_WRAP_I64 = 0xa7,    /* i32.wrap/i64 */
	OP_I32_TRUNC_S_F32 = 0xa8, /* i32.trunc_s/f32 */
	OP_I32_TRUNC_U_F32 = 0xa9, /* i32.trunc_u/f32 */
	OP_I32_TRUNC_S_F64 = 0xaa, /* i32.trunc_s/f64 */
	OP_I32_TRUNC_U_F64 = 0xab, /* i32.trunc_u/f64 */

	OP_I64_EXTEND_S_I32 = 0xac, /* i64.extend_s/i32 */
	OP_I64_EXTEND_U_I32 = 0xad, /* i64.extend_u/i32 */
	OP_I64_TRUNC_S_F32 = 0xae,  /* i64.trunc_s/f32 */
	OP_I64_TRUNC_U_F32 = 0xaf,  /* i64.trunc_u/f32 */
	OP_I64_TRUNC_S_F64 = 0xb0,  /* i64.trunc_s/f64 */
	OP_I64_TRUNC_U_F64 = 0xb1,  /* i64.trunc_u/f64 */

	OP_F32_CONVERT_S_I32 = 0xb2, /* f32.convert_s/i32 */
	OP_F32_CONVERT_U_I32 = 0xb3, /* f32.convert_u/i32 */
	OP_F32_CONVERT_S_I64 = 0xb4, /* f32.convert_s/i64 */
	OP_F32_CONVERT_U_I64 = 0xb5, /* f32.convert_u/i64 */
	OP_F32_DEMOTE_F64 = 0xb6,    /* f32.demote/f64 */

	OP_F64_CONVERT_S_I32 = 0xb7, /* f64.convert_s/i32 */
	OP_F64_CONVERT_U_I32 = 0xb8, /* f64.convert_u/i32 */
	OP_F64_CONVERT_S_I64 = 0xb9, /* f64.convert_s/i64 */
	OP_F64_CONVERT_U_I64 = 0xba, /* f64.convert_u/i64 */
	OP_F64_PROMOTE_F32 = 0xbb,   /* f64.promote/f32 */

	/* reinterpretations */
	OP_I32_REINTERPRET_F32 = 0xbc, /* i32.reinterpret/f32 */
	OP_I64_REINTERPRET_F64 = 0xbd, /* i64.reinterpret/f64 */
	OP_F32_REINTERPRET_I32 = 0xbe, /* f32.reinterpret/i32 */
	OP_F64_REINTERPRET_I64 = 0xbf, /* f64.reinterpret/i64 */

	OP_I32_EXTEND8_S = 0xc0,  /* i32.extend8_s */
	OP_I32_EXTEND16_S = 0xc1, /* i32.extend16_s */
	OP_I64_EXTEND8_S = 0xc2,  /* i64.extend8_s */
	OP_I64_EXTEND16_S = 0xc3, /* i64.extend16_s */
	OP_I64_EXTEND32_S = 0xc4, /* i64.extend32_s */

	/* drop/select specified types*/
	OP_DROP_64 = 0xc5,
	OP_SELECT_64 = 0xc6,

	/* extend op code */
	EXT_OP_GET_LOCAL_FAST = 0xc7,
	EXT_OP_SET_LOCAL_FAST_I64 = 0xc8,
	EXT_OP_SET_LOCAL_FAST = 0xc9,
	EXT_OP_TEE_LOCAL_FAST = 0xca,
	EXT_OP_TEE_LOCAL_FAST_I64 = 0xcb,
	EXT_OP_COPY_STACK_TOP = 0xcc,
	EXT_OP_COPY_STACK_TOP_I64 = 0xcd,
	EXT_OP_COPY_STACK_VALUES = 0xce,

	OP_IMPDEP = 0xcf,

	OP_REF_NULL = 0xd0,    /* ref.null */
	OP_REF_IS_NULL = 0xd1, /* ref.is_null */
	OP_REF_FUNC = 0xd2,    /* ref.func */

	EXT_OP_BLOCK = 0xd3,          /* block with blocktype */
	EXT_OP_LOOP = 0xd4,           /* loop with blocktype */
	EXT_OP_IF = 0xd5,             /* if with blocktype */
	EXT_OP_BR_TABLE_CACHE = 0xd6, /* br_table from cache */

	DEBUG_OP_BREAK = 0xd7, /* debug break point */

	/* Post-MVP extend op prefix */
	OP_MISC_PREFIX = 0xfc,
	OP_SIMD_PREFIX = 0xfd,
	OP_ATOMIC_PREFIX = 0xfe,
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

struct XE_API ASTExeFrame
{
	XE::uint64 SP = 0;
	XE::uint64 FP = 0;
	XE::ASTInfoFunctionPtr AST;
	XE::ExecuteGotoType ExecGo;
	XE::Stack< const XE::WhileStatNode * > Loop;
	XE::Map< XE::String, XE::uint64 > Variables;
};

END_XE_NAMESPACE

#endif//TYPE_H__133957F1_3C08_4A8E_BD52_68923E0CF528
