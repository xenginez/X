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

#include "Declare.h"

BEG_XE_NAMESPACE

DECL_PTR( WASI );
DECL_PTR( WASMModule );
DECL_PTR( WASMInterpreter );


enum class WASMOpcode : XE::uint8
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
	OP_MEMORY_COPY = 0x0a,
	OP_MEMORY_FILL = 0x0b,
	OP_TABLE_INIT = 0x0c,
	OP_ELEM_DROP = 0x0d,
	OP_TABLE_COPY = 0x0e,
	OP_TABLE_GROW = 0x0f,
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


struct XE_API WASMValue
{
	XE::ValueType type;
	union
	{
		XE::int32 i32;
		XE::int64 i64;
		XE::uint32 u32;
		XE::uint64 u64;
		XE::float32 f32;
		XE::float64 f64;
		void * p;
	};
};

struct XE_API InitExpr
{
	XE::ExprType Type;
	XE::WASMValue Val;
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
		XE::uint32 TypeIdx; // index of funcType in types
		struct
		{
			XE::uint32 Min;
			XE::uint32 Max;
		} Limits; // table, memory
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

struct XE_API FuncInstance
{

};

struct XE_API TableInstance
{

};

struct XE_API MemoryInstance
{

};

struct XE_API GlobalInstance
{

};

struct XE_API WASMFrame
{
	XE::uint64 SP = 0;
	XE::uint64 FP = 0;
	XE::uint64 PC = 0;
	XE::uint32 FuncIdx;
	XE::WASMModulePtr Module;
};

END_XE_NAMESPACE

#endif // TYPE_H__7E436181_F13C_40BF_ADA5_29233BD1218E
