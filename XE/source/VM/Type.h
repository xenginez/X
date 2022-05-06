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
#include "Opcodes.inc"

BEG_XE_NAMESPACE

enum class Opcode : XE::uint16
{
#define VISIT_OP( ID, NAME, STR ) NAME = ID,

	CONTROL_OPCODES( VISIT_OP )
	PARAMETRIC_OPCODES( VISIT_OP )
	OVERLOADED_OPCODES( VISIT_OP )
	INDEX_POLYMORPHIC_OPCODES( VISIT_OP )
	NONCONTROL_NONPARAMETRIC_OPCODES( VISIT_OP )

#undef VISIT_OP
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

enum class ConstType : XE::uint8
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

enum class ElemModeType
{
	NONE = 0,
	PASSIVE,
	ACTIVE,
	DECLARATIVE,
};

enum class DataModeType
{
	NONE = 0,
	PASSIVE,
	ACTIVE,
};


struct XE_API Value
{
	ValueType type;
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

struct XE_API FuncDecl
{
	XE::Array< ValueType > Parameter;
	XE::Array< ValueType > Results;
};

struct XE_API ConstExpr
{
	ConstType Type;
	Value Val;
};

struct XE_API ImportSection
{
	XE::Utf8String Module;
	XE::Utf8String Name;
	DescType Desc;
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
			bool Mutable;
			ValueType Type;
		} Global;
	};
};

struct XE_API FuncSection
{
	XE::uint32 TypeIdx;
	XE::Array< XE::uint8 > Codes;
	XE::Array< ValueType > Locals;
};

struct XE_API TableSection
{
	RefType Type;
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
	bool Mutable;
	ValueType Type;
	ConstExpr Innit;
};

struct XE_API ExportSection
{
	XE::Utf8String Name;
	DescType Desc;
	XE::uint32 DescIdx;
};

struct XE_API ElemSection
{
	ValueType Type;
	XE::Array< ConstExpr > Init;
	struct
	{
		ElemModeType Type;
		XE::uint32 TableIdx;
		ConstExpr Offset;
	} Mode;
};

struct XE_API DataSection
{
	XE::Array< XE::uint8 > Init;
	struct  
	{
		DataModeType Type;
		XE::uint32 MemIdx;
		ConstExpr Offset;
	} Mode;
};

struct XE_API Module
{
	XE::Array< XE::FuncDecl > Types;
	XE::Array< XE::ImportSection > Imports;
	XE::Array< XE::FuncSection > Funcs;
	XE::Array< XE::TableSection > Tables;
	XE::Array< XE::MemorySection > Memorys;
	XE::Array< XE::GlobalSection > Globals;
	XE::Array< XE::ExportSection > Exports;
	XE::uint32 Start;
	XE::Array< XE::ElemSection > Elems;
	XE::Array< XE::DataSection > Datas;
};

END_XE_NAMESPACE

#endif // TYPE_H__7E436181_F13C_40BF_ADA5_29233BD1218E
