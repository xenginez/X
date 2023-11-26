/*!
 * \file   Type.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/05
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__7E436181_F13C_40BF_ADA5_29233BD1218E
#define TYPE_H__7E436181_F13C_40BF_ADA5_29233BD1218E

#include "Reflect/Reflection.h"
#include "Reflect/Object.h"

#include "Declare.h"

BEG_XE_NAMESPACE

DECL_PTR( WASI );
DECL_PTR( WASMModule );
DECL_PTR( WASMService );
DECL_PTR( WASMInterpreter );

enum class WASMOpcode : XE::uint8
{
	/* CONTROL INSTRUCTIONS */
	OP_UNREACHABLE = 0x00, /* UNREACHABLE */
	OP_NOP = 0x01,         /* NOP */
	OP_BLOCK = 0x02,       /* BLOCK */
	OP_LOOP = 0x03,        /* LOOP */
	OP_IF = 0x04,          /* IF */
	OP_ELSE = 0x05,        /* ELSE */

	OP_UNUSED_0x06 = 0x06,
	OP_UNUSED_0x07 = 0x07,
	OP_UNUSED_0x08 = 0x08,
	OP_UNUSED_0x09 = 0x09,
	OP_UNUSED_0x0A = 0x0A,

	OP_END = 0x0B,                  /* END */
	OP_BR = 0x0C,                   /* BR */
	OP_BR_IF = 0x0D,                /* BR IF */
	OP_BR_TABLE = 0x0E,             /* BR TABLE */
	OP_RETURN = 0x0F,               /* RETURN */
	OP_CALL = 0x10,                 /* CALL */
	OP_CALL_INDIRECT = 0x11,        /* CALL_INDIRECT */
	OP_RETURN_CALL = 0x12,          /* RETURN_CALL */
	OP_RETURN_CALL_INDIRECT = 0x13, /* RETURN_CALL_INDIRECT */

	OP_UNUSED_0x14 = 0x14,
	OP_UNUSED_0x15 = 0x15,
	OP_UNUSED_0x16 = 0x16,
	OP_UNUSED_0x17 = 0x17,
	OP_UNUSED_0x18 = 0x18,
	OP_UNUSED_0x19 = 0x19,

	/* PARAMETRIC INSTRUCTIONS */
	OP_DROP = 0x1A,     /* DROP */
	OP_SELECT = 0x1B,   /* SELECT */
	OP_SELECT_T = 0x1C, /* SELECT T */

	OP_GET_GLOBAL_64 = 0x1D,
	OP_SET_GLOBAL_64 = 0x1E,
	OP_SET_GLOBAL_AUX_STACK = 0x1F,

	/* VARIABLE INSTRUCTIONS */
	OP_GET_LOCAL = 0x20,  /* GET_LOCAL */
	OP_SET_LOCAL = 0x21,  /* SET_LOCAL */
	OP_TEE_LOCAL = 0x22,  /* TEE_LOCAL */
	OP_GET_GLOBAL = 0x23, /* GET_GLOBAL */
	OP_SET_GLOBAL = 0x24, /* SET_GLOBAL */

	OP_TABLE_GET = 0x25, /* TABLE.GET */
	OP_TABLE_SET = 0x26, /* TABLE.SET */
	OP_UNUSED_0x27 = 0x27,

	/* MEMORY INSTRUCTIONS */
	OP_I32_LOAD = 0x28,     /* I32.LOAD */
	OP_I64_LOAD = 0x29,     /* I64.LOAD */
	OP_F32_LOAD = 0x2A,     /* F32.LOAD */
	OP_F64_LOAD = 0x2B,     /* F64.LOAD */
	OP_I32_LOAD8_S = 0x2C,  /* I32.LOAD8_S */
	OP_I32_LOAD8_U = 0x2D,  /* I32.LOAD8_U */
	OP_I32_LOAD16_S = 0x2E, /* I32.LOAD16_S */
	OP_I32_LOAD16_U = 0x2F, /* I32.LOAD16_U */
	OP_I64_LOAD8_S = 0x30,  /* I64.LOAD8_S */
	OP_I64_LOAD8_U = 0x31,  /* I64.LOAD8_U */
	OP_I64_LOAD16_S = 0x32, /* I64.LOAD16_S */
	OP_I64_LOAD16_U = 0x33, /* I64.LOAD16_U */
	OP_I64_LOAD32_S = 0x34, /* I32.LOAD32_S */
	OP_I64_LOAD32_U = 0x35, /* I32.LOAD32_U */
	OP_I32_STORE = 0x36,    /* I32.STORE */
	OP_I64_STORE = 0x37,    /* I64.STORE */
	OP_F32_STORE = 0x38,    /* F32.STORE */
	OP_F64_STORE = 0x39,    /* F64.STORE */
	OP_I32_STORE8 = 0x3A,   /* I32.STORE8 */
	OP_I32_STORE16 = 0x3B,  /* I32.STORE16 */
	OP_I64_STORE8 = 0x3C,   /* I64.STORE8 */
	OP_I64_STORE16 = 0x3D,  /* I64.SOTRE16 */
	OP_I64_STORE32 = 0x3E,  /* I64.STORE32 */
	OP_MEMORY_SIZE = 0x3F,  /* MEMORY.SIZE */
	OP_MEMORY_GROW = 0x40,  /* MEMORY.GROW */

	/* CONSTANT INSTRUCTIONS */
	OP_I32_CONST = 0x41, /* I32.CONST */
	OP_I64_CONST = 0x42, /* I64.CONST */
	OP_F32_CONST = 0x43, /* F32.CONST */
	OP_F64_CONST = 0x44, /* F64.CONST */

	/* COMPARISON INSTRUCTIONS */
	OP_I32_EQZ = 0x45,  /* I32.EQZ */
	OP_I32_EQ = 0x46,   /* I32.EQ */
	OP_I32_NE = 0x47,   /* I32.NE */
	OP_I32_LT_S = 0x48, /* I32.LT_S */
	OP_I32_LT_U = 0x49, /* I32.LT_U */
	OP_I32_GT_S = 0x4A, /* I32.GT_S */
	OP_I32_GT_U = 0x4B, /* I32.GT_U */
	OP_I32_LE_S = 0x4C, /* I32.LE_S */
	OP_I32_LE_U = 0x4D, /* I32.LE_U */
	OP_I32_GE_S = 0x4E, /* I32.GE_S */
	OP_I32_GE_U = 0x4F, /* I32.GE_U */

	OP_I64_EQZ = 0x50,  /* I64.EQZ */
	OP_I64_EQ = 0x51,   /* I64.EQ */
	OP_I64_NE = 0x52,   /* I64.NE */
	OP_I64_LT_S = 0x53, /* I64.LT_S */
	OP_I64_LT_U = 0x54, /* I64.LT_U */
	OP_I64_GT_S = 0x55, /* I64.GT_S */
	OP_I64_GT_U = 0x56, /* I64.GT_U */
	OP_I64_LE_S = 0x57, /* I64.LE_S */
	OP_I64_LE_U = 0x58, /* I64.LE_U */
	OP_I64_GE_S = 0x59, /* I64.GE_S */
	OP_I64_GE_U = 0x5A, /* I64.GE_U */

	OP_F32_EQ = 0x5B, /* F32.EQ */
	OP_F32_NE = 0x5C, /* F32.NE */
	OP_F32_LT = 0x5D, /* F32.LT */
	OP_F32_GT = 0x5E, /* F32.GT */
	OP_F32_LE = 0x5F, /* F32.LE */
	OP_F32_GE = 0x60, /* F32.GE */

	OP_F64_EQ = 0x61, /* F64.EQ */
	OP_F64_NE = 0x62, /* F64.NE */
	OP_F64_LT = 0x63, /* F64.LT */
	OP_F64_GT = 0x64, /* F64.GT */
	OP_F64_LE = 0x65, /* F64.LE */
	OP_F64_GE = 0x66, /* F64.GE */

	/* NUMERIC OPERATORS */
	OP_I32_CLZ = 0x67,    /* I32.CLZ */
	OP_I32_CTZ = 0x68,    /* I32.CTZ */
	OP_I32_POPCNT = 0x69, /* I32.POPCNT */
	OP_I32_ADD = 0x6A,    /* I32.ADD */
	OP_I32_SUB = 0x6B,    /* I32.SUB */
	OP_I32_MUL = 0x6C,    /* I32.MUL */
	OP_I32_DIV_S = 0x6D,  /* I32.DIV_S */
	OP_I32_DIV_U = 0x6E,  /* I32.DIV_U */
	OP_I32_REM_S = 0x6F,  /* I32.REM_S */
	OP_I32_REM_U = 0x70,  /* I32.REM_U */
	OP_I32_AND = 0x71,    /* I32.AND */
	OP_I32_OR = 0x72,     /* I32.OR */
	OP_I32_XOR = 0x73,    /* I32.XOR */
	OP_I32_SHL = 0x74,    /* I32.SHL */
	OP_I32_SHR_S = 0x75,  /* I32.SHR_S */
	OP_I32_SHR_U = 0x76,  /* I32.SHR_U */
	OP_I32_ROTL = 0x77,   /* I32.ROTL */
	OP_I32_ROTR = 0x78,   /* I32.ROTR */

	OP_I64_CLZ = 0x79,    /* I64.CLZ */
	OP_I64_CTZ = 0x7A,    /* I64.CTZ */
	OP_I64_POPCNT = 0x7B, /* I64.POPCNT */
	OP_I64_ADD = 0x7C,    /* I64.ADD */
	OP_I64_SUB = 0x7D,    /* I64.SUB */
	OP_I64_MUL = 0x7E,    /* I64.MUL */
	OP_I64_DIV_S = 0x7F,  /* I64.DIV_S */
	OP_I64_DIV_U = 0x80,  /* I64.DIV_U */
	OP_I64_REM_S = 0x81,  /* I64.REM_S */
	OP_I64_REM_U = 0x82,  /* I64.REM_U */
	OP_I64_AND = 0x83,    /* I64.AND */
	OP_I64_OR = 0x84,     /* I64.OR */
	OP_I64_XOR = 0x85,    /* I64.XOR */
	OP_I64_SHL = 0x86,    /* I64.SHL */
	OP_I64_SHR_S = 0x87,  /* I64.SHR_S */
	OP_I64_SHR_U = 0x88,  /* I64.SHR_U */
	OP_I64_ROTL = 0x89,   /* I64.ROTL */
	OP_I64_ROTR = 0x8A,   /* I64.ROTR */

	OP_F32_ABS = 0x8B,      /* F32.ABS */
	OP_F32_NEG = 0x8C,      /* F32.NEG */
	OP_F32_CEIL = 0x8D,     /* F32.CEIL */
	OP_F32_FLOOR = 0x8E,    /* F32.FLOOR */
	OP_F32_TRUNC = 0x8F,    /* F32.TRUNC */
	OP_F32_NEAREST = 0x90,  /* F32.NEAREST */
	OP_F32_SQRT = 0x91,     /* F32.SQRT */
	OP_F32_ADD = 0x92,      /* F32.ADD */
	OP_F32_SUB = 0x93,      /* F32.SUB */
	OP_F32_MUL = 0x94,      /* F32.MUL */
	OP_F32_DIV = 0x95,      /* F32.DIV */
	OP_F32_MIN = 0x96,      /* F32.MIN */
	OP_F32_MAX = 0x97,      /* F32.MAX */
	OP_F32_COPYSIGN = 0x98, /* F32.COPYSIGN */

	OP_F64_ABS = 0x99,      /* F64.ABS */
	OP_F64_NEG = 0x9A,      /* F64.NEG */
	OP_F64_CEIL = 0x9B,     /* F64.CEIL */
	OP_F64_FLOOR = 0x9C,    /* F64.FLOOR */
	OP_F64_TRUNC = 0x9D,    /* F64.TRUNC */
	OP_F64_NEAREST = 0x9E,  /* F64.NEAREST */
	OP_F64_SQRT = 0x9F,     /* F64.SQRT */
	OP_F64_ADD = 0xA0,      /* F64.ADD */
	OP_F64_SUB = 0xA1,      /* F64.SUB */
	OP_F64_MUL = 0xA2,      /* F64.MUL */
	OP_F64_DIV = 0xA3,      /* F64.DIV */
	OP_F64_MIN = 0xA4,      /* F64.MIN */
	OP_F64_MAX = 0xA5,      /* F64.MAX */
	OP_F64_COPYSIGN = 0xA6, /* F64.COPYSIGN */

	/* CONVERSIONS */
	OP_I32_WRAP_I64 = 0xA7,    /* I32.WRAP/I64 */
	OP_I32_TRUNC_S_F32 = 0xA8, /* I32.TRUNC_S/F32 */
	OP_I32_TRUNC_U_F32 = 0xA9, /* I32.TRUNC_U/F32 */
	OP_I32_TRUNC_S_F64 = 0xAA, /* I32.TRUNC_S/F64 */
	OP_I32_TRUNC_U_F64 = 0xAB, /* I32.TRUNC_U/F64 */

	OP_I64_EXTEND_S_I32 = 0xAC, /* I64.EXTEND_S/I32 */
	OP_I64_EXTEND_U_I32 = 0xAD, /* I64.EXTEND_U/I32 */
	OP_I64_TRUNC_S_F32 = 0xAE,  /* I64.TRUNC_S/F32 */
	OP_I64_TRUNC_U_F32 = 0xAF,  /* I64.TRUNC_U/F32 */
	OP_I64_TRUNC_S_F64 = 0xB0,  /* I64.TRUNC_S/F64 */
	OP_I64_TRUNC_U_F64 = 0xB1,  /* I64.TRUNC_U/F64 */

	OP_F32_CONVERT_S_I32 = 0xB2, /* F32.CONVERT_S/I32 */
	OP_F32_CONVERT_U_I32 = 0xB3, /* F32.CONVERT_U/I32 */
	OP_F32_CONVERT_S_I64 = 0xB4, /* F32.CONVERT_S/I64 */
	OP_F32_CONVERT_U_I64 = 0xB5, /* F32.CONVERT_U/I64 */
	OP_F32_DEMOTE_F64 = 0xB6,    /* F32.DEMOTE/F64 */

	OP_F64_CONVERT_S_I32 = 0xB7, /* F64.CONVERT_S/I32 */
	OP_F64_CONVERT_U_I32 = 0xB8, /* F64.CONVERT_U/I32 */
	OP_F64_CONVERT_S_I64 = 0xB9, /* F64.CONVERT_S/I64 */
	OP_F64_CONVERT_U_I64 = 0xBA, /* F64.CONVERT_U/I64 */
	OP_F64_PROMOTE_F32 = 0xBB,   /* F64.PROMOTE/F32 */

	/* REINTERPRETATIONS */
	OP_I32_REINTERPRET_F32 = 0xBC, /* I32.REINTERPRET/F32 */
	OP_I64_REINTERPRET_F64 = 0xBD, /* I64.REINTERPRET/F64 */
	OP_F32_REINTERPRET_I32 = 0xBE, /* F32.REINTERPRET/I32 */
	OP_F64_REINTERPRET_I64 = 0xBF, /* F64.REINTERPRET/I64 */

	OP_I32_EXTEND8_S = 0xC0,  /* I32.EXTEND8_S */
	OP_I32_EXTEND16_S = 0xC1, /* I32.EXTEND16_S */
	OP_I64_EXTEND8_S = 0xC2,  /* I64.EXTEND8_S */
	OP_I64_EXTEND16_S = 0xC3, /* I64.EXTEND16_S */
	OP_I64_EXTEND32_S = 0xC4, /* I64.EXTEND32_S */
	
	OP_UNUSED_0xC5 = 0xC5,
	OP_UNUSED_0xC6 = 0xC6,
	OP_UNUSED_0xC7 = 0xC7,
	OP_UNUSED_0xC8 = 0xC8,
	OP_UNUSED_0xC9 = 0xC9,
	OP_UNUSED_0xCA = 0xCA,
	OP_UNUSED_0xCB = 0xCB,
	OP_UNUSED_0xCC = 0xCC,
	OP_UNUSED_0xCD = 0xCD,
	OP_UNUSED_0xCE = 0xCE,
	OP_UNUSED_0xCF = 0xCF,

	OP_REF_NULL = 0xD0,    /* REF.NULL */
	OP_REF_IS_NULL = 0xD1, /* REF.IS_NULL */
	OP_REF_FUNC = 0xD2,    /* REF.FUNC */
	OP_REF_AS_NON_NULL = 0xD3, /* REF.AS_NON_NULL */
	OP_BR_ON_NULL = 0xD4,      /* BR_ON_NULL */
	OP_REF_EQ = 0xD5, /* REF.EQ */
	OP_BR_ON_NON_NULL = 0xD6, /* BR_ON_NON_NULL */

	OP_UNUSED_0xD7 = 0xD7,
	OP_UNUSED_0xD8 = 0xD8,
	OP_UNUSED_0xD9 = 0xD9,
	OP_UNUSED_0xDA = 0xDA,
	OP_UNUSED_0xDB = 0xDB,
	OP_UNUSED_0xDC = 0xDC,
	OP_UNUSED_0xDD = 0xDD,
	OP_UNUSED_0xDE = 0xDE,
	OP_UNUSED_0xDF = 0xDF,

	OP_UNUSED_0xE0 = 0xE0,
	OP_UNUSED_0xE1 = 0xE1,
	OP_UNUSED_0xE2 = 0xE2,
	OP_UNUSED_0xE3 = 0xE3,
	OP_UNUSED_0xE4 = 0xE4,
	OP_UNUSED_0xE5 = 0xE5,
	OP_UNUSED_0xE6 = 0xE6,
	OP_UNUSED_0xE7 = 0xE7,
	OP_UNUSED_0xE8 = 0xE8,
	OP_UNUSED_0xE9 = 0xE9,
	OP_UNUSED_0xEA = 0xEA,
	OP_UNUSED_0xEB = 0xEB,
	OP_UNUSED_0xEC = 0xEC,
	OP_UNUSED_0xED = 0xED,
	OP_UNUSED_0xEE = 0xEE,
	OP_UNUSED_0xEF = 0xEF,

	OP_UNUSED_0xF0 = 0xF0,
	OP_UNUSED_0xF1 = 0xF1,
	OP_UNUSED_0xF2 = 0xF2,
	OP_UNUSED_0xF3 = 0xF3,
	OP_UNUSED_0xF4 = 0xF4,
	OP_UNUSED_0xF5 = 0xF5,
	OP_UNUSED_0xF6 = 0xF6,
	OP_UNUSED_0xF7 = 0xF7,
	OP_UNUSED_0xF8 = 0xF8,
	OP_UNUSED_0xF9 = 0xF9,
	OP_UNUSED_0xFA = 0xFA,

	/* EXTEND OP PREFIX */
	OP_GC_PREFIX = 0xFB,
	OP_STR_PREFIX = 0xFB,
	OP_MISC_PREFIX = 0xFC,
	OP_SIMD_PREFIX = 0xFD,
	OP_THREAD_PREFIX = 0xFE,
};

enum class GCExtOpcode : XE::uint8
{
	OP_UNUSED_0x00 = 0x00,
	OP_STRUCT_NEW_CANON = 0x01,
	OP_STRUCT_NEW_CANON_DEFAULT = 0x02,
	OP_STRUCT_GET = 0x03,
	OP_STRUCT_GET_S = 0x04,
	OP_STRUCT_GET_U = 0x05,
	OP_STRUCT_SET = 0x06,
	OP_UNUSED_0x07 = 0x07,
	OP_UNUSED_0x08 = 0x08,
	OP_UNUSED_0x09 = 0x09,
	OP_UNUSED_0x0A = 0x0A,
	OP_UNUSED_0x0B = 0x0B,
	OP_UNUSED_0x0C = 0x0C,
	OP_UNUSED_0x0D = 0x0D,
	OP_UNUSED_0x0E = 0x0E,
	OP_UNUSED_0x0F = 0x0F,
	OP_UNUSED_0x10 = 0x10,

	OP_ARRAY_NEW_CANON = 0x11,
	OP_ARRAY_NEW_CANON_DEFAULT = 0x12,
	OP_ARRAY_GET = 0x13,
	OP_ARRAY_GET_S = 0x14,
	OP_ARRAY_GET_U = 0x15,
	OP_ARRAY_SET = 0x16,
	OP_ARRAY_LEN = 0x17,
	OP_UNUSED_0x18 = 0x18,
	OP_ARRAY_NEW_CANON_FIXED = 0x19,
	OP_UNUSED_0x1A = 0x1A,
	OP_ARRAY_NEW_CANON_DATA = 0x1B,
	OP_ARRAY_NEW_CANON_ELEM = 0x1C,
	OP_UNUSED_0x1D = 0x1D,
	OP_UNUSED_0x1E = 0x1E,
	OP_UNUSED_0x1F = 0x1F,

	OP_I31_NEW = 0x20,
	OP_I31_GET_S = 0x21,
	OP_I31_GET_U = 0x22,
	OP_UNUSED_0x23 = 0x23,
	OP_UNUSED_0x24 = 0x24,
	OP_UNUSED_0x25 = 0x25,
	OP_UNUSED_0x26 = 0x26,
	OP_UNUSED_0x27 = 0x27,
	OP_UNUSED_0x28 = 0x28,
	OP_UNUSED_0x29 = 0x29,
	OP_UNUSED_0x2A = 0x2A,
	OP_UNUSED_0x2B = 0x2B,
	OP_UNUSED_0x2C = 0x2C,
	OP_UNUSED_0x2D = 0x2D,
	OP_UNUSED_0x2E = 0x2E,
	OP_UNUSED_0x2F = 0x2F,
	OP_UNUSED_0x30 = 0x30,
	OP_UNUSED_0x31 = 0x31,
	OP_UNUSED_0x32 = 0x32,
	OP_UNUSED_0x33 = 0x33,
	OP_UNUSED_0x34 = 0x34,
	OP_UNUSED_0x35 = 0x35,
	OP_UNUSED_0x36 = 0x36,
	OP_UNUSED_0x37 = 0x37,
	OP_UNUSED_0x38 = 0x38,
	OP_UNUSED_0x39 = 0x39,
	OP_UNUSED_0x3A = 0x3A,
	OP_UNUSED_0x3B = 0x3B,
	OP_UNUSED_0x3C = 0x3C,
	OP_UNUSED_0x3D = 0x3D,
	OP_UNUSED_0x3E = 0x3E,
	OP_UNUSED_0x3F = 0x3F,

	OP_REF_TEST = 0x40,
	OP_REF_CAST = 0x41,
	OP_BR_ON_CAST = 0x42,
	OP_BR_ON_CAST_FAIL = 0x43,
	OP_UNUSED_0x44 = 0x44,
	OP_UNUSED_0x45 = 0x45,
	OP_UNUSED_0x46 = 0x46,
	OP_UNUSED_0x47 = 0x47,
	OP_UNUSED_0x48 = 0x48,
	OP_UNUSED_0x49 = 0x49,
	OP_UNUSED_0x4A = 0x4A,
	OP_UNUSED_0x4B = 0x4B,
	OP_UNUSED_0x4C = 0x4C,
	OP_UNUSED_0x4D = 0x4D,
	OP_UNUSED_0x4E = 0x4E,
	OP_UNUSED_0x4F = 0x4F,

	OP_UNUSED_0x50 = 0x50,
	OP_UNUSED_0x51 = 0x51,
	OP_UNUSED_0x52 = 0x52,
	OP_UNUSED_0x53 = 0x53,
	OP_UNUSED_0x54 = 0x54,
	OP_UNUSED_0x55 = 0x55,
	OP_UNUSED_0x56 = 0x56,
	OP_UNUSED_0x57 = 0x57,
	OP_UNUSED_0x58 = 0x58,
	OP_UNUSED_0x59 = 0x59,
	OP_UNUSED_0x5A = 0x5A,
	OP_UNUSED_0x5B = 0x5B,
	OP_UNUSED_0x5C = 0x5C,
	OP_UNUSED_0x5D = 0x5D,
	OP_UNUSED_0x5E = 0x5E,
	OP_UNUSED_0x5F = 0x5F,

	OP_UNUSED_0x60 = 0x60,
	OP_UNUSED_0x61 = 0x61,
	OP_UNUSED_0x62 = 0x62,
	OP_UNUSED_0x63 = 0x63,
	OP_UNUSED_0x64 = 0x64,
	OP_UNUSED_0x65 = 0x65,
	OP_UNUSED_0x66 = 0x66,
	OP_UNUSED_0x67 = 0x67,
	OP_UNUSED_0x68 = 0x68,
	OP_UNUSED_0x69 = 0x69,
	OP_UNUSED_0x6A = 0x6A,
	OP_UNUSED_0x6B = 0x6B,
	OP_UNUSED_0x6C = 0x6C,
	OP_UNUSED_0x6D = 0x6D,
	OP_UNUSED_0x6E = 0x6E,
	OP_UNUSED_0x6F = 0x6F,

	OP_EXTERN_INTERNALIZE = 0x70,
	OP_EXTERN_EXTERNALIZE = 0x71,
	OP_UNUSED_0x72 = 0x72,
	OP_UNUSED_0x73 = 0x73,
	OP_UNUSED_0x74 = 0x74,
	OP_UNUSED_0x75 = 0x75,
	OP_UNUSED_0x76 = 0x76,
	OP_UNUSED_0x77 = 0x77,
	OP_UNUSED_0x78 = 0x78,
	OP_UNUSED_0x79 = 0x79,
	OP_UNUSED_0x7A = 0x7A,
	OP_UNUSED_0x7B = 0x7B,
	OP_UNUSED_0x7C = 0x7C,
	OP_UNUSED_0x7D = 0x7D,
	OP_UNUSED_0x7E = 0x7E,
	OP_UNUSED_0x7F = 0x7F,
};

enum class StrExtOpcode
{
	OP_STRING_NEW_UTF8 = 0x80,
	OP_STRING_NEW_WTF16 = 0x81,
	OP_STRING_CONST = 0x82,
	OP_STRING_MEASURE_UTF8 = 0x83,
	OP_STRING_MEASURE_WTF8 = 0x84,
	OP_STRING_MEASURE_WTF16 = 0x85,
	OP_STRING_ENCODE_UTF8 = 0x86,
	OP_STRING_ENCODE_WTF16 = 0x87,
	OP_STRING_CONCAT = 0x88,
	OP_STRING_EQ = 0x89,
	OP_STRING_IS_USV_SEQUENCE = 0x8A,
	OP_STRING_NEW_LOSSY_UTF8 = 0x8B,
	OP_STRING_NEW_WTF8 = 0x8C,
	OP_STRING_ENCODE_LOSSY_UTF8 = 0x8D,
	OP_STRING_ENCODE_WTF8 = 0x8E,
	OP_UNUSED_0x8F = 0x8F,
	OP_STRING_AS_WTF8 = 0x90,
	OP_STRINGVIEW_WTF8_ADVANCE = 0x91,
	OP_STRINGVIEW_WTF8_ENCODE_UTF8 = 0x92,
	OP_STRINGVIEW_WTF8_SLICE = 0x93,
	OP_STRINGVIEW_WTF8_ENCODE_LOSSY_UTF8 = 0x94,
	OP_STRINGVIEW_WTF8_ENCODE_WTF8 = 0x95,
	OP_UNUSED_0x96 = 0x96,
	OP_UNUSED_0x97 = 0x97,
	OP_STRING_AS_WTF16 = 0x98,
	OP_STRINGVIEW_WTF16_LENGTH = 0x99,
	OP_STRINGVIEW_WTF16_GET_CODEUNIT = 0x9A,
	OP_STRINGVIEW_WTF16_ENCODE = 0x9B,
	OP_STRINGVIEW_WTF16_SLICE = 0x9C,
	OP_UNUSED_0x9D = 0x9D,
	OP_UNUSED_0x9E = 0x9E,
	OP_UNUSED_0x9F = 0x9F,

	OP_STRING_AS_ITER = 0xA0,
	OP_STRINGVIEW_ITER_NEXT = 0xA1,
	OP_STRINGVIEW_ITER_ADVANCE = 0xA2,
	OP_STRINGVIEW_ITER_REWIND = 0xA3,
	OP_STRINGVIEW_ITER_SLICE = 0xA4,
	OP_UNUSED_0xA5 = 0xA5,
	OP_UNUSED_0xA6 = 0xA6,
	OP_UNUSED_0xA7 = 0xA7,
	OP_UNUSED_0xA8 = 0xA8,
	OP_UNUSED_0xA9 = 0xA9,
	OP_UNUSED_0xAA = 0xAA,
	OP_UNUSED_0xAB = 0xAB,
	OP_UNUSED_0xAC = 0xAC,
	OP_UNUSED_0xAD = 0xAD,
	OP_UNUSED_0xAE = 0xAE,
	OP_UNUSED_0xAF = 0xAF,

	OP_STRING_NEW_UTF8_ARRAY = 0xB0,
	OP_STRING_NEW_WTF16_ARRAY = 0xB1,
	OP_STRING_ENCODE_UTF8_ARRAY = 0xB2,
	OP_STRING_ENCODE_WTF16_ARRAY = 0xB3,
	OP_STRING_NEW_LOSSY_UTF8_ARRAY = 0xB4,
	OP_STRING_NEW_WTF8_ARRAY = 0xB5,
	OP_STRING_ENCODE_LOSSY_UTF8_ARRAY = 0xB6,
	OP_STRING_ENCODE_WTF8_ARRAY = 0xB7,
	OP_UNUSED_0xB8 = 0xB8,
	OP_UNUSED_0xB9 = 0xB9,
	OP_UNUSED_0xBA = 0xBA,
	OP_UNUSED_0xBB = 0xBB,
	OP_UNUSED_0xBC = 0xBC,
	OP_UNUSED_0xBD = 0xBD,
	OP_UNUSED_0xBE = 0xBE,
	OP_UNUSED_0xBF = 0xBF,
};

enum class MiscExtOpcode : XE::uint8
{
	OP_I32_TRUNC_SAT_S_F32 = 0x00,
	OP_I32_TRUNC_SAT_U_F32 = 0x01,
	OP_I32_TRUNC_SAT_S_F64 = 0x02,
	OP_I32_TRUNC_SAT_U_F64 = 0x03,
	OP_I64_TRUNC_SAT_S_F32 = 0x04,
	OP_I64_TRUNC_SAT_U_F32 = 0x05,
	OP_I64_TRUNC_SAT_S_F64 = 0x06,
	OP_I64_TRUNC_SAT_U_F64 = 0x07,
	OP_MEMORY_INIT = 0x08,
	OP_DATA_DROP = 0x09,
	OP_MEMORY_COPY = 0x0A,
	OP_MEMORY_FILL = 0x0B,
	OP_TABLE_INIT = 0x0C,
	OP_ELEM_DROP = 0x0D,
	OP_TABLE_COPY = 0x0E,
	OP_TABLE_GROW = 0x0F,
	OP_TABLE_SIZE = 0x10,
	OP_TABLE_FILL = 0x11,
};

enum class SimdExtOpcode : XE::uint8
{
	/* memory instruction */
	OP_V128_LOAD = 0x00,
	OP_V128_LOAD8X8_S = 0x01,
	OP_V128_LOAD8X8_U = 0x02,
	OP_V128_LOAD16X4_S = 0x03,
	OP_V128_LOAD16X4_U = 0x04,
	OP_V128_LOAD32X2_S = 0x05,
	OP_V128_LOAD32X2_U = 0x06,
	OP_V128_LOAD8_SPLAT = 0x07,
	OP_V128_LOAD16_SPLAT = 0x08,
	OP_V128_LOAD32_SPLAT = 0x09,
	OP_V128_LOAD64_SPLAT = 0x0a,
	OP_V128_STORE = 0x0b,

	/* basic operation */
	OP_V128_CONST = 0x0c,
	OP_V8X16_SHUFFLE = 0x0d,
	OP_V8X16_SWIZZLE = 0x0e,

	/* splat operation */
	OP_I8X16_SPLAT = 0x0f,
	OP_I16X8_SPLAT = 0x10,
	OP_I32X4_SPLAT = 0x11,
	OP_I64X2_SPLAT = 0x12,
	OP_F32X4_SPLAT = 0x13,
	OP_F64X2_SPLAT = 0x14,

	/* lane operation */
	OP_I8X16_EXTRACT_LANE_S = 0x15,
	OP_I8X16_EXTRACT_LANE_U = 0x16,
	OP_I8X16_REPLACE_LANE = 0x17,
	OP_I16X8_EXTRACT_LANE_S = 0x18,
	OP_I16X8_EXTRACT_LANE_U = 0x19,
	OP_I16X8_REPLACE_LANE = 0x1a,
	OP_I32X4_EXTRACT_LANE = 0x1b,
	OP_I32X4_REPLACE_LANE = 0x1c,
	OP_I64X2_EXTRACT_LANE = 0x1d,
	OP_I64X2_REPLACE_LANE = 0x1e,
	OP_F32X4_EXTRACT_LANE = 0x1f,
	OP_F32X4_REPLACE_LANE = 0x20,
	OP_F64X2_EXTRACT_LANE = 0x21,
	OP_F64X2_REPLACE_LANE = 0x22,

	/* i8x16 compare operation */
	OP_I8X16_EQ = 0x23,
	OP_I8X16_NE = 0x24,
	OP_I8X16_LT_S = 0x25,
	OP_I8X16_LT_U = 0x26,
	OP_I8X16_GT_S = 0x27,
	OP_I8X16_GT_U = 0x28,
	OP_I8X16_LE_S = 0x29,
	OP_I8X16_LE_U = 0x2a,
	OP_I8X16_GE_S = 0x2b,
	OP_I8X16_GE_U = 0x2c,

	/* i16x8 compare operation */
	OP_I16X8_EQ = 0x2d,
	OP_I16X8_NE = 0x2e,
	OP_I16X8_LT_S = 0x2f,
	OP_I16X8_LT_U = 0x30,
	OP_I16X8_GT_S = 0x31,
	OP_I16X8_GT_U = 0x32,
	OP_I16X8_LE_S = 0x33,
	OP_I16X8_LE_U = 0x34,
	OP_I16X8_GE_S = 0x35,
	OP_I16X8_GE_U = 0x36,

	/* i32x4 compare operation */
	OP_I32X4_EQ = 0x37,
	OP_I32X4_NE = 0x38,
	OP_I32X4_LT_S = 0x39,
	OP_I32X4_LT_U = 0x3a,
	OP_I32X4_GT_S = 0x3b,
	OP_I32X4_GT_U = 0x3c,
	OP_I32X4_LE_S = 0x3d,
	OP_I32X4_LE_U = 0x3e,
	OP_I32X4_GE_S = 0x3f,
	OP_I32X4_GE_U = 0x40,

	/* f32x4 compare operation */
	OP_F32X4_EQ = 0x41,
	OP_F32X4_NE = 0x42,
	OP_F32X4_LT = 0x43,
	OP_F32X4_GT = 0x44,
	OP_F32X4_LE = 0x45,
	OP_F32X4_GE = 0x46,

	/* f64x2 compare operation */
	OP_F64X2_EQ = 0x47,
	OP_F64X2_NE = 0x48,
	OP_F64X2_LT = 0x49,
	OP_F64X2_GT = 0x4a,
	OP_F64X2_LE = 0x4b,
	OP_F64X2_GE = 0x4c,

	/* v128 operation */
	OP_V128_NOT = 0x4d,
	OP_V128_AND = 0x4e,
	OP_V128_ANDNOT = 0x4f,
	OP_V128_OR = 0x50,
	OP_V128_XOR = 0x51,
	OP_V128_BITSELECT = 0x52,
	OP_V128_ANY_TRUE = 0x53,

	/* Load Lane Operation */
	OP_V128_LOAD8_LANE = 0x54,
	OP_V128_LOAD16_LANE = 0x55,
	OP_V128_LOAD32_LANE = 0x56,
	OP_V128_LOAD64_LANE = 0x57,
	OP_V128_STORE8_LANE = 0x58,
	OP_V128_STORE16_LANE = 0x59,
	OP_V128_STORE32_LANE = 0x5a,
	OP_V128_STORE64_LANE = 0x5b,
	OP_V128_LOAD32_ZERO = 0x5c,
	OP_V128_LOAD64_ZERO = 0x5d,

	/* Float conversion */
	OP_F32X4_DEMOTE_F64X2_ZERO = 0x5e,
	OP_F64X2_PROMOTE_LOW_F32X4_ZERO = 0x5f,

	/* i8x16 Operation */
	OP_I8X16_ABS = 0x60,
	OP_I8X16_NEG = 0x61,
	OP_I8X16_POPCNT = 0x62,
	OP_I8X16_ALL_TRUE = 0x63,
	OP_I8X16_BITMASK = 0x64,
	OP_I8X16_NARROW_I16X8_S = 0x65,
	OP_I8X16_NARROW_I16X8_U = 0x66,
	OP_F32X4_CEIL = 0x67,
	OP_F32X4_FLOOR = 0x68,
	OP_F32X4_TRUNC = 0x69,
	OP_F32X4_NEAREST = 0x6a,
	OP_I8X16_SHL = 0x6b,
	OP_I8X16_SHR_S = 0x6c,
	OP_I8X16_SHR_U = 0x6d,
	OP_I8X16_ADD = 0x6e,
	OP_I8X16_ADD_SAT_S = 0x6f,
	OP_I8X16_ADD_SAT_U = 0x70,
	OP_I8X16_SUB = 0x71,
	OP_I8X16_SUB_SAT_S = 0x72,
	OP_I8X16_SUB_SAT_U = 0x73,
	OP_F64X2_CEIL = 0x74,
	OP_F64X2_FLOOR = 0x75,
	OP_I8X16_MIN_S = 0x76,
	OP_I8X16_MIN_U = 0x77,
	OP_I8X16_MAX_S = 0x78,
	OP_I8X16_MAX_U = 0x79,
	OP_F64X2_TRUNC = 0x7a,
	OP_I8X16_AVGR_U = 0x7b,
	OP_I16X8_EXTADD_PAIRWISE_I8X16_S = 0x7c,
	OP_I16X8_EXTADD_PAIRWISE_I8X16_U = 0x7d,
	OP_I32X4_EXTADD_PAIRWISE_I16X8_S = 0x7e,
	OP_I32X4_EXTADD_PAIRWISE_I16X8_U = 0x7f,

	/* i16x8 operation */
	OP_I16X8_ABS = 0x80,
	OP_I16X8_NEG = 0x81,
	OP_I16X8_Q15MULR_SAT_S = 0x82,
	OP_I16X8_ALL_TRUE = 0x83,
	OP_I16X8_BITMASK = 0x84,
	OP_I16X8_NARROW_I32X4_S = 0x85,
	OP_I16X8_NARROW_I32X4_U = 0x86,
	OP_I16X8_EXTEND_LOW_I8X16_S = 0x87,
	OP_I16X8_EXTEND_HIGH_I8X16_S = 0x88,
	OP_I16X8_EXTEND_LOW_I8X16_U = 0x89,
	OP_I16X8_EXTEND_HIGH_I8X16_U = 0x8a,
	OP_I16X8_SHL = 0x8b,
	OP_I16X8_SHR_S = 0x8c,
	OP_I16X8_SHR_U = 0x8d,
	OP_I16X8_ADD = 0x8e,
	OP_I16X8_ADD_SAT_S = 0x8f,
	OP_I16X8_ADD_SAT_U = 0x90,
	OP_I16X8_SUB = 0x91,
	OP_I16X8_SUB_SAT_S = 0x92,
	OP_I16X8_SUB_SAT_U = 0x93,
	OP_F64X2_NEAREST = 0x94,
	OP_I16X8_MUL = 0x95,
	OP_I16X8_MIN_S = 0x96,
	OP_I16X8_MIN_U = 0x97,
	OP_I16X8_MAX_S = 0x98,
	OP_I16X8_MAX_U = 0x99,
	/* placeholder            = 0x9a */
	OP_I16X8_AVGR_U = 0x9b,
	OP_I16X8_EXTMUL_LOW_I8X16_S = 0x9c,
	OP_I16X8_EXTMUL_HIGH_I8X16_S = 0x9d,
	OP_I16X8_EXTMUL_LOW_I8X16_U = 0x9e,
	OP_I16X8_EXTMUL_HIGH_I8X16_U = 0x9f,

	/* i32x4 operation */
	OP_I32X4_ABS = 0xa0,
	OP_I32X4_NEG = 0xa1,
	/* placeholder            = 0xa2 */
	OP_I32X4_ALL_TRUE = 0xa3,
	OP_I32X4_BITMASK = 0xa4,
	OP_I32X4_NARROW_I64X2_S = 0xa5,
	OP_I32X4_NARROW_I64X2_U = 0xa6,
	OP_I32X4_EXTEND_LOW_I16X8_S = 0xa7,
	OP_I32X4_EXTEND_HIGH_I16X8_S = 0xa8,
	OP_I32X4_EXTEND_LOW_I16X8_U = 0xa9,
	OP_I32X4_EXTEND_HIGH_I16X8_U = 0xaa,
	OP_I32X4_SHL = 0xab,
	OP_I32X4_SHR_S = 0xac,
	OP_I32X4_SHR_U = 0xad,
	OP_I32X4_ADD = 0xae,
	OP_I32X4_ADD_SAT_S = 0xaf,
	OP_I32X4_ADD_SAT_U = 0xb0,
	OP_I32X4_SUB = 0xb1,
	OP_I32X4_SUB_SAT_S = 0xb2,
	OP_I32X4_SUB_SAT_U = 0xb3,
	/* placeholder            = 0xb4 */
	OP_I32X4_MUL = 0xb5,
	OP_I32X4_MIN_S = 0xb6,
	OP_I32X4_MIN_U = 0xb7,
	OP_I32X4_MAX_S = 0xb8,
	OP_I32X4_MAX_U = 0xb9,
	OP_I32X4_DOT_I16X8_S = 0xba,
	OP_I32X4_AVGR_U = 0xbb,
	OP_I32X4_EXTMUL_LOW_I16X8_S = 0xbc,
	OP_I32X4_EXTMUL_HIGH_I16X8_S = 0xbd,
	OP_I32X4_EXTMUL_LOW_I16X8_U = 0xbe,
	OP_I32X4_EXTMUL_HIGH_I16X8_U = 0xbf,

	/* i64x2 operation */
	OP_I64X2_ABS = 0xc0,
	OP_I64X2_NEG = 0xc1,
	/* placeholder       = 0xc2 */
	OP_I64X2_ALL_TRUE = 0xc3,
	OP_I64X2_BITMASK = 0xc4,
	/* placeholder       = 0xc5 */
	/* placeholder       = 0xc6 */
	OP_I64X2_EXTEND_LOW_I32X4_S = 0xc7,
	OP_I64X2_EXTEND_HIGH_I32X4_S = 0xc8,
	OP_I64X2_EXTEND_LOW_I32X4_U = 0xc9,
	OP_I64X2_EXTEND_HIGH_I32X4_U = 0xca,
	OP_I64X2_SHL = 0xcb,
	OP_I64X2_SHR_S = 0xcc,
	OP_I64X2_SHR_U = 0xcd,
	OP_I64X2_ADD = 0xce,
	/* placeholder       = 0xcf */
	/* placeholder       = 0xd0 */
	OP_I64X2_SUB = 0xd1,
	/* placeholder       = 0xd2 */
	/* placeholder       = 0xd3 */
	/* placeholder       = 0xd4 */
	OP_I64X2_MUL = 0xd5,
	OP_I64X2_EQ = 0xd6,
	OP_I64X2_NE = 0xd7,
	OP_I64X2_LT_S = 0xd8,
	OP_I64X2_GT_S = 0xd9,
	OP_I64X2_LE_S = 0xda,
	OP_I64X2_GE_S = 0xdb,
	OP_I64X2_EXTMUL_LOW_I32X4_S = 0xdc,
	OP_I64X2_EXTMUL_HIGH_I32X4_S = 0xdd,
	OP_I64X2_EXTMUL_LOW_I32X4_U = 0xde,
	OP_I64X2_EXTMUL_HIGH_I32X4_U = 0xdf,

	/* f32x4 operation */
	OP_F32X4_ABS = 0xe0,
	OP_F32X4_NEG = 0xe1,
	OP_F32X4_ROUND = 0xe2,
	OP_F32X4_SQRT = 0xe3,
	OP_F32X4_ADD = 0xe4,
	OP_F32X4_SUB = 0xe5,
	OP_F32X4_MUL = 0xe6,
	OP_F32X4_DIV = 0xe7,
	OP_F32X4_MIN = 0xe8,
	OP_F32X4_MAX = 0xe9,
	OP_F32X4_PMIN = 0xea,
	OP_F32X4_PMAX = 0xeb,

	/* f64x2 operation */
	OP_F64X2_ABS = 0xec,
	OP_F64X2_NEG = 0xed,
	OP_F64X2_ROUND = 0xee,
	OP_F64X2_SQRT = 0xef,
	OP_F64X2_ADD = 0xf0,
	OP_F64X2_SUB = 0xf1,
	OP_F64X2_MUL = 0xf2,
	OP_F64X2_DIV = 0xf3,
	OP_F64X2_MIN = 0xf4,
	OP_F64X2_MAX = 0xf5,
	OP_F64X2_PMIN = 0xf6,
	OP_F64X2_PMAX = 0xf7,

	/* conversion operation */
	OP_I32X4_TRUNC_SAT_F32X4_S = 0xf8,
	OP_I32X4_TRUNC_SAT_F32X4_U = 0xf9,
	OP_F32X4_CONVERT_I32X4_S = 0xfa,
	OP_F32X4_CONVERT_I32X4_U = 0xfb,
	OP_I32X4_TRUNC_SAT_F64X2_S_ZERO = 0xfc,
	OP_I32X4_TRUNC_SAT_F64X2_U_ZERO = 0xfd,
	OP_F64X2_CONVERT_LOW_I32X4_S = 0xfe,
	OP_F64X2_CONVERT_LOW_I32X4_U = 0xff,
};

enum class AtomicExtOpcode : XE::uint8
{
	/* atomic wait and notify */
	OP_ATOMIC_NOTIFY = 0x00,
	OP_ATOMIC_WAIT32 = 0x01,
	OP_ATOMIC_WAIT64 = 0x02,
	OP_ATOMIC_FENCE = 0x03,
	/* atomic load and store */
	OP_ATOMIC_I32_LOAD = 0x10,
	OP_ATOMIC_I64_LOAD = 0x11,
	OP_ATOMIC_I32_LOAD8_U = 0x12,
	OP_ATOMIC_I32_LOAD16_U = 0x13,
	OP_ATOMIC_I64_LOAD8_U = 0x14,
	OP_ATOMIC_I64_LOAD16_U = 0x15,
	OP_ATOMIC_I64_LOAD32_U = 0x16,
	OP_ATOMIC_I32_STORE = 0x17,
	OP_ATOMIC_I64_STORE = 0x18,
	OP_ATOMIC_I32_STORE8 = 0x19,
	OP_ATOMIC_I32_STORE16 = 0x1a,
	OP_ATOMIC_I64_STORE8 = 0x1b,
	OP_ATOMIC_I64_STORE16 = 0x1c,
	OP_ATOMIC_I64_STORE32 = 0x1d,
	/* atomic add */
	OP_ATOMIC_RMW_I32_ADD = 0x1e,
	OP_ATOMIC_RMW_I64_ADD = 0x1f,
	OP_ATOMIC_RMW_I32_ADD8_U = 0x20,
	OP_ATOMIC_RMW_I32_ADD16_U = 0x21,
	OP_ATOMIC_RMW_I64_ADD8_U = 0x22,
	OP_ATOMIC_RMW_I64_ADD16_U = 0x23,
	OP_ATOMIC_RMW_I64_ADD32_U = 0x24,
	/* atomic sub */
	OP_ATOMIC_RMW_I32_SUB = 0x25,
	OP_ATOMIC_RMW_I64_SUB = 0x26,
	OP_ATOMIC_RMW_I32_SUB8_U = 0x27,
	OP_ATOMIC_RMW_I32_SUB16_U = 0x28,
	OP_ATOMIC_RMW_I64_SUB8_U = 0x29,
	OP_ATOMIC_RMW_I64_SUB16_U = 0x2a,
	OP_ATOMIC_RMW_I64_SUB32_U = 0x2b,
	/* atomic and */
	OP_ATOMIC_RMW_I32_AND = 0x2c,
	OP_ATOMIC_RMW_I64_AND = 0x2d,
	OP_ATOMIC_RMW_I32_AND8_U = 0x2e,
	OP_ATOMIC_RMW_I32_AND16_U = 0x2f,
	OP_ATOMIC_RMW_I64_AND8_U = 0x30,
	OP_ATOMIC_RMW_I64_AND16_U = 0x31,
	OP_ATOMIC_RMW_I64_AND32_U = 0x32,
	/* atomic or */
	OP_ATOMIC_RMW_I32_OR = 0x33,
	OP_ATOMIC_RMW_I64_OR = 0x34,
	OP_ATOMIC_RMW_I32_OR8_U = 0x35,
	OP_ATOMIC_RMW_I32_OR16_U = 0x36,
	OP_ATOMIC_RMW_I64_OR8_U = 0x37,
	OP_ATOMIC_RMW_I64_OR16_U = 0x38,
	OP_ATOMIC_RMW_I64_OR32_U = 0x39,
	/* atomic xor */
	OP_ATOMIC_RMW_I32_XOR = 0x3a,
	OP_ATOMIC_RMW_I64_XOR = 0x3b,
	OP_ATOMIC_RMW_I32_XOR8_U = 0x3c,
	OP_ATOMIC_RMW_I32_XOR16_U = 0x3d,
	OP_ATOMIC_RMW_I64_XOR8_U = 0x3e,
	OP_ATOMIC_RMW_I64_XOR16_U = 0x3f,
	OP_ATOMIC_RMW_I64_XOR32_U = 0x40,
	/* atomic xchg */
	OP_ATOMIC_RMW_I32_XCHG = 0x41,
	OP_ATOMIC_RMW_I64_XCHG = 0x42,
	OP_ATOMIC_RMW_I32_XCHG8_U = 0x43,
	OP_ATOMIC_RMW_I32_XCHG16_U = 0x44,
	OP_ATOMIC_RMW_I64_XCHG8_U = 0x45,
	OP_ATOMIC_RMW_I64_XCHG16_U = 0x46,
	OP_ATOMIC_RMW_I64_XCHG32_U = 0x47,
	/* atomic cmpxchg */
	OP_ATOMIC_RMW_I32_CMPXCHG = 0x48,
	OP_ATOMIC_RMW_I64_CMPXCHG = 0x49,
	OP_ATOMIC_RMW_I32_CMPXCHG8_U = 0x4a,
	OP_ATOMIC_RMW_I32_CMPXCHG16_U = 0x4b,
	OP_ATOMIC_RMW_I64_CMPXCHG8_U = 0x4c,
	OP_ATOMIC_RMW_I64_CMPXCHG16_U = 0x4d,
	OP_ATOMIC_RMW_I64_CMPXCHG32_U = 0x4e,
};


enum class RefType : XE::uint8
{
	NONE = 0,
	FUNC,
	EXTERN,
};

enum class DescType : XE::uint8
{
	NONE = 0,
	FUNC,
	TABLE,
	MEM,
	GLOBAL,
};

enum class ExprType : XE::uint8
{
	NONE = 0,
	VALUE,
	INDEX,
};

enum class ValueType : XE::uint8
{
	NONE = 0,
	I32,
	I64,
	F32,
	F64,
	FUNC,
	EXTERN,
};

enum class BlockType : XE::uint8
{
	IF,
	LOOP,
	BLOCK,
	FUNCTION,
	INIT_EXP,
};

enum class SectionType : XE::uint8
{
	CUSTOM = 0,
	TYPE,
	IMPORT,
	FUNC,
	TABLE,
	MEMORY,
	GLOBAL,
	EXPORT,
	START,
	ELEM,
	CODE,
	DATA,
};


struct XE_API InitExpr
{
	XE::ExprType Type;
	union
	{
		XE::int32 i32;
		XE::int64 i64;
		XE::float32 f32;
		XE::float64 f64;
	} Val;
};

struct XE_API TypeSection
{
	XE::Array< XE::ValueType > Parameter;
	XE::Array< XE::ValueType > Results;
};

struct XE_API ImportSection
{
	XE::String Module;
	XE::String Field;
	XE::DescType Desc;
	union
	{
		XE::uint32 TypeIdx;
		struct
		{
			XE::uint32 Min;
			XE::uint32 Max;
		} Limits;
		struct
		{
			XE::uint32 Mutable;
			XE::ValueType Type;
		} Global;
	};
};

struct XE_API FuncSection
{
	XE::uint32 TypeIdx;
	XE::Array< XE::uint8 > Codes;
	XE::Array< XE::ValueType > Locals;
};

struct XE_API TableSection
{
	XE::uint32 Min;
	XE::uint32 Max;
};

struct XE_API MemorySection
{
	XE::uint32 Min;
	XE::uint32 Max;
};

struct XE_API GlobalSection
{
	XE::uint32 Mutable;
	XE::ValueType Type;
	XE::InitExpr Init;
};

struct XE_API ExportSection
{
	XE::String Field;
	XE::DescType Desc;
	XE::uint32 DescIdx;
};

struct XE_API ElemSection
{
	XE::uint32 TableIdx;
	XE::InitExpr Offset;
	XE::Array< XE::uint32 > FuncIdxs;
};

struct XE_API DataSection
{
	XE::uint32 MemIdx;
	XE::InitExpr Offset;
	XE::Array< XE::uint8 > Init;
};


struct XE_API WASMFunc
{
	struct InternalFunc
	{
		XE::uint64 Index;
	};
	struct ExternalFunc
	{
		XE::uint64 HashCode;
	};

	std::variant< std::monostate, InternalFunc, ExternalFunc > Func;
};

struct XE_API WASMValue
{
	XE::ValueType type;
	union
	{
		XE::int32 i32;
		XE::int64 i64;
		XE::float32 f32;
		XE::float64 f64;
		XE::Object * o;
	};
};

struct XE_API WASMFrame
{
	XE::uint64 SP = 0;
	XE::uint64 FP = 0;
	XE::uint64 PC = 0;
	XE::uint64 FuncIdx;
};


template<> struct VariantCreate< WASMValue >
{
	static XE::VariantData Create( const WASMValue & val )
	{
		switch ( val.type )
		{
		case XE::ValueType::I32:
			return XE::VariantData( val.i32 );
		case XE::ValueType::I64:
			return XE::VariantData( val.i64 );
		case XE::ValueType::F32:
			return XE::VariantData( val.f32 );
		case XE::ValueType::F64:
			return XE::VariantData( val.f64 );
		case XE::ValueType::FUNC:
		case XE::ValueType::EXTERN:
			return XE::VariantData( XE::VariantPointerData( val.o ) );
			break;
		}

		return XE::VariantData();
	}
};

template<> struct VariantCast< WASMValue >
{
	static WASMValue Cast( const VariantData & val )
	{
		return std::visit( XE::Overloaded{
			[]( const std::monostate & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::NONE; return value; },
			[]( const bool & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val ? 1 : 0; return value; },
			[]( const XE::int8 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val; return value; },
			[]( const XE::int16 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val; return value; },
			[]( const XE::int32 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val; return value; },
			[]( const XE::int64 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I64; value.i64 = val; return value; },
			[]( const XE::uint8 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val; return value; },
			[]( const XE::uint16 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val; return value; },
			[]( const XE::uint32 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I64; value.i64 = val; return value; },
			[]( const XE::uint64 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I64; value.i64 = val; return value; },
			[]( const XE::float32 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::F32; value.f32 = val; return value; },
			[]( const XE::float64 & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::F64; value.f64 = val; return value; },
			[]( const XE::VariantEnumData & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::I32; value.i32 = val.Value; return value; },
			[]( const XE::VariantSmallData & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::NONE; return value; },
			[]( const XE::VariantPointerData & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::FUNC; value.o = reinterpret_cast<XE::Object *>( val.Value ); return value; },
			[]( const XE::VariantWarpperData & val ) -> XE::WASMValue { XE::WASMValue value; value.type = ValueType::NONE; return value; } }, val );
	}
};

END_XE_NAMESPACE

#endif // TYPE_H__7E436181_F13C_40BF_ADA5_29233BD1218E
