#include "WASMInterpreter.h"

#include "WASMContext.h"
#include "WASMService.h"

XE::WASMInterpreter::WASMInterpreter( XE::WASMContext * ctx, XE::WASMService * service )
	:_Context( ctx ), _Service( service )
{

}

XE::int32 XE::WASMInterpreter::Exec( XE::MemoryView view )
{
	while ( true )
	{
		XE::WASMOpcode code = *reinterpret_cast<const XE::WASMOpcode *>( view.data() + _Context->GetFrame().PC++ );
		switch ( code )
		{
		case XE::WASMOpcode::OP_UNREACHABLE:
			break;
		case XE::WASMOpcode::OP_NOP:
		case XE::WASMOpcode::OP_BLOCK:
		case XE::WASMOpcode::OP_LOOP:
			break;
		case XE::WASMOpcode::OP_IF:
		{
			if ( _Context->Pop().i32 != 0 )
			{
				_Context->GetFrame().PC += sizeof( XE::uint32 );
			}
			else
			{
				XE::DecodeLeb128( view.data() + _Context->GetFrame().PC, _Context->GetFrame().PC );
			}
		}
			break;
		case XE::WASMOpcode::OP_ELSE:
		{
			XE::DecodeLeb128( view.data() + _Context->GetFrame().PC, _Context->GetFrame().PC );
		}
			break;
		case XE::WASMOpcode::OP_UNUSED_0x06:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x07:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x08:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x09:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x0A:
			break;
		case XE::WASMOpcode::OP_END:
		{
			if ( _Context->GetFrame().PC >= view.size() )
			{
				goto __END;
			}
		}
			break;
		case XE::WASMOpcode::OP_BR:
		case XE::WASMOpcode::OP_BR_IF:
		case XE::WASMOpcode::OP_RETURN:
		{
			XE::uint32 arity = 0;
			_Context->GetFrame().PC += XE::DecodeLeb128( view.data() + _Context->GetFrame().PC, arity );

			if ( code == WASMOpcode::OP_BR_IF && _Context->Pop().i32 == 0 )
			{
				_Context->GetFrame().PC += 8;
				break;
			}


		}
			break;
		case XE::WASMOpcode::OP_BR_TABLE:
			break;
		case XE::WASMOpcode::OP_CALL:
			break;
		case XE::WASMOpcode::OP_CALL_INDIRECT:
			break;
		case XE::WASMOpcode::OP_RETURN_CALL:
			break;
		case XE::WASMOpcode::OP_RETURN_CALL_INDIRECT:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x14:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x15:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x16:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x17:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x18:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x19:
			break;
		case XE::WASMOpcode::OP_DROP:
			break;
		case XE::WASMOpcode::OP_SELECT:
			break;
		case XE::WASMOpcode::OP_SELECT_T:
			break;
		case XE::WASMOpcode::OP_GET_GLOBAL_64:
			break;
		case XE::WASMOpcode::OP_SET_GLOBAL_64:
			break;
		case XE::WASMOpcode::OP_SET_GLOBAL_AUX_STACK:
			break;
		case XE::WASMOpcode::OP_GET_LOCAL:
			break;
		case XE::WASMOpcode::OP_SET_LOCAL:
			break;
		case XE::WASMOpcode::OP_TEE_LOCAL:
			break;
		case XE::WASMOpcode::OP_GET_GLOBAL:
			break;
		case XE::WASMOpcode::OP_SET_GLOBAL:
			break;
		case XE::WASMOpcode::OP_TABLE_GET:
			break;
		case XE::WASMOpcode::OP_TABLE_SET:
			break;
		case XE::WASMOpcode::OP_UNUSED_0x27:
			break;
		case XE::WASMOpcode::OP_I32_LOAD:
			break;
		case XE::WASMOpcode::OP_I64_LOAD:
			break;
		case XE::WASMOpcode::OP_F32_LOAD:
			break;
		case XE::WASMOpcode::OP_F64_LOAD:
			break;
		case XE::WASMOpcode::OP_I32_LOAD8_S:
			break;
		case XE::WASMOpcode::OP_I32_LOAD8_U:
			break;
		case XE::WASMOpcode::OP_I32_LOAD16_S:
			break;
		case XE::WASMOpcode::OP_I32_LOAD16_U:
			break;
		case XE::WASMOpcode::OP_I64_LOAD8_S:
			break;
		case XE::WASMOpcode::OP_I64_LOAD8_U:
			break;
		case XE::WASMOpcode::OP_I64_LOAD16_S:
			break;
		case XE::WASMOpcode::OP_I64_LOAD16_U:
			break;
		case XE::WASMOpcode::OP_I64_LOAD32_S:
			break;
		case XE::WASMOpcode::OP_I64_LOAD32_U:
			break;
		case XE::WASMOpcode::OP_I32_STORE:
			break;
		case XE::WASMOpcode::OP_I64_STORE:
			break;
		case XE::WASMOpcode::OP_F32_STORE:
			break;
		case XE::WASMOpcode::OP_F64_STORE:
			break;
		case XE::WASMOpcode::OP_I32_STORE8:
			break;
		case XE::WASMOpcode::OP_I32_STORE16:
			break;
		case XE::WASMOpcode::OP_I64_STORE8:
			break;
		case XE::WASMOpcode::OP_I64_STORE16:
			break;
		case XE::WASMOpcode::OP_I64_STORE32:
			break;
		case XE::WASMOpcode::OP_MEMORY_SIZE:
			break;
		case XE::WASMOpcode::OP_MEMORY_GROW:
			break;
		case XE::WASMOpcode::OP_I32_CONST:
			break;
		case XE::WASMOpcode::OP_I64_CONST:
			break;
		case XE::WASMOpcode::OP_F32_CONST:
			break;
		case XE::WASMOpcode::OP_F64_CONST:
			break;
		case XE::WASMOpcode::OP_I32_EQZ:
			break;
		case XE::WASMOpcode::OP_I32_EQ:
			break;
		case XE::WASMOpcode::OP_I32_NE:
			break;
		case XE::WASMOpcode::OP_I32_LT_S:
			break;
		case XE::WASMOpcode::OP_I32_LT_U:
			break;
		case XE::WASMOpcode::OP_I32_GT_S:
			break;
		case XE::WASMOpcode::OP_I32_GT_U:
			break;
		case XE::WASMOpcode::OP_I32_LE_S:
			break;
		case XE::WASMOpcode::OP_I32_LE_U:
			break;
		case XE::WASMOpcode::OP_I32_GE_S:
			break;
		case XE::WASMOpcode::OP_I32_GE_U:
			break;
		case XE::WASMOpcode::OP_I64_EQZ:
			break;
		case XE::WASMOpcode::OP_I64_EQ:
			break;
		case XE::WASMOpcode::OP_I64_NE:
			break;
		case XE::WASMOpcode::OP_I64_LT_S:
			break;
		case XE::WASMOpcode::OP_I64_LT_U:
			break;
		case XE::WASMOpcode::OP_I64_GT_S:
			break;
		case XE::WASMOpcode::OP_I64_GT_U:
			break;
		case XE::WASMOpcode::OP_I64_LE_S:
			break;
		case XE::WASMOpcode::OP_I64_LE_U:
			break;
		case XE::WASMOpcode::OP_I64_GE_S:
			break;
		case XE::WASMOpcode::OP_I64_GE_U:
			break;
		case XE::WASMOpcode::OP_F32_EQ:
			break;
		case XE::WASMOpcode::OP_F32_NE:
			break;
		case XE::WASMOpcode::OP_F32_LT:
			break;
		case XE::WASMOpcode::OP_F32_GT:
			break;
		case XE::WASMOpcode::OP_F32_LE:
			break;
		case XE::WASMOpcode::OP_F32_GE:
			break;
		case XE::WASMOpcode::OP_F64_EQ:
			break;
		case XE::WASMOpcode::OP_F64_NE:
			break;
		case XE::WASMOpcode::OP_F64_LT:
			break;
		case XE::WASMOpcode::OP_F64_GT:
			break;
		case XE::WASMOpcode::OP_F64_LE:
			break;
		case XE::WASMOpcode::OP_F64_GE:
			break;
		case XE::WASMOpcode::OP_I32_CLZ:
			break;
		case XE::WASMOpcode::OP_I32_CTZ:
			break;
		case XE::WASMOpcode::OP_I32_POPCNT:
			break;
		case XE::WASMOpcode::OP_I32_ADD:
			break;
		case XE::WASMOpcode::OP_I32_SUB:
			break;
		case XE::WASMOpcode::OP_I32_MUL:
			break;
		case XE::WASMOpcode::OP_I32_DIV_S:
			break;
		case XE::WASMOpcode::OP_I32_DIV_U:
			break;
		case XE::WASMOpcode::OP_I32_REM_S:
			break;
		case XE::WASMOpcode::OP_I32_REM_U:
			break;
		case XE::WASMOpcode::OP_I32_AND:
			break;
		case XE::WASMOpcode::OP_I32_OR:
			break;
		case XE::WASMOpcode::OP_I32_XOR:
			break;
		case XE::WASMOpcode::OP_I32_SHL:
			break;
		case XE::WASMOpcode::OP_I32_SHR_S:
			break;
		case XE::WASMOpcode::OP_I32_SHR_U:
			break;
		case XE::WASMOpcode::OP_I32_ROTL:
			break;
		case XE::WASMOpcode::OP_I32_ROTR:
			break;
		case XE::WASMOpcode::OP_I64_CLZ:
			break;
		case XE::WASMOpcode::OP_I64_CTZ:
			break;
		case XE::WASMOpcode::OP_I64_POPCNT:
			break;
		case XE::WASMOpcode::OP_I64_ADD:
			break;
		case XE::WASMOpcode::OP_I64_SUB:
			break;
		case XE::WASMOpcode::OP_I64_MUL:
			break;
		case XE::WASMOpcode::OP_I64_DIV_S:
			break;
		case XE::WASMOpcode::OP_I64_DIV_U:
			break;
		case XE::WASMOpcode::OP_I64_REM_S:
			break;
		case XE::WASMOpcode::OP_I64_REM_U:
			break;
		case XE::WASMOpcode::OP_I64_AND:
			break;
		case XE::WASMOpcode::OP_I64_OR:
			break;
		case XE::WASMOpcode::OP_I64_XOR:
			break;
		case XE::WASMOpcode::OP_I64_SHL:
			break;
		case XE::WASMOpcode::OP_I64_SHR_S:
			break;
		case XE::WASMOpcode::OP_I64_SHR_U:
			break;
		case XE::WASMOpcode::OP_I64_ROTL:
			break;
		case XE::WASMOpcode::OP_I64_ROTR:
			break;
		case XE::WASMOpcode::OP_F32_ABS:
			break;
		case XE::WASMOpcode::OP_F32_NEG:
			break;
		case XE::WASMOpcode::OP_F32_CEIL:
			break;
		case XE::WASMOpcode::OP_F32_FLOOR:
			break;
		case XE::WASMOpcode::OP_F32_TRUNC:
			break;
		case XE::WASMOpcode::OP_F32_NEAREST:
			break;
		case XE::WASMOpcode::OP_F32_SQRT:
			break;
		case XE::WASMOpcode::OP_F32_ADD:
			break;
		case XE::WASMOpcode::OP_F32_SUB:
			break;
		case XE::WASMOpcode::OP_F32_MUL:
			break;
		case XE::WASMOpcode::OP_F32_DIV:
			break;
		case XE::WASMOpcode::OP_F32_MIN:
			break;
		case XE::WASMOpcode::OP_F32_MAX:
			break;
		case XE::WASMOpcode::OP_F32_COPYSIGN:
			break;
		case XE::WASMOpcode::OP_F64_ABS:
			break;
		case XE::WASMOpcode::OP_F64_NEG:
			break;
		case XE::WASMOpcode::OP_F64_CEIL:
			break;
		case XE::WASMOpcode::OP_F64_FLOOR:
			break;
		case XE::WASMOpcode::OP_F64_TRUNC:
			break;
		case XE::WASMOpcode::OP_F64_NEAREST:
			break;
		case XE::WASMOpcode::OP_F64_SQRT:
			break;
		case XE::WASMOpcode::OP_F64_ADD:
			break;
		case XE::WASMOpcode::OP_F64_SUB:
			break;
		case XE::WASMOpcode::OP_F64_MUL:
			break;
		case XE::WASMOpcode::OP_F64_DIV:
			break;
		case XE::WASMOpcode::OP_F64_MIN:
			break;
		case XE::WASMOpcode::OP_F64_MAX:
			break;
		case XE::WASMOpcode::OP_F64_COPYSIGN:
			break;
		case XE::WASMOpcode::OP_I32_WRAP_I64:
			break;
		case XE::WASMOpcode::OP_I32_TRUNC_S_F32:
			break;
		case XE::WASMOpcode::OP_I32_TRUNC_U_F32:
			break;
		case XE::WASMOpcode::OP_I32_TRUNC_S_F64:
			break;
		case XE::WASMOpcode::OP_I32_TRUNC_U_F64:
			break;
		case XE::WASMOpcode::OP_I64_EXTEND_S_I32:
			break;
		case XE::WASMOpcode::OP_I64_EXTEND_U_I32:
			break;
		case XE::WASMOpcode::OP_I64_TRUNC_S_F32:
			break;
		case XE::WASMOpcode::OP_I64_TRUNC_U_F32:
			break;
		case XE::WASMOpcode::OP_I64_TRUNC_S_F64:
			break;
		case XE::WASMOpcode::OP_I64_TRUNC_U_F64:
			break;
		case XE::WASMOpcode::OP_F32_CONVERT_S_I32:
			break;
		case XE::WASMOpcode::OP_F32_CONVERT_U_I32:
			break;
		case XE::WASMOpcode::OP_F32_CONVERT_S_I64:
			break;
		case XE::WASMOpcode::OP_F32_CONVERT_U_I64:
			break;
		case XE::WASMOpcode::OP_F32_DEMOTE_F64:
			break;
		case XE::WASMOpcode::OP_F64_CONVERT_S_I32:
			break;
		case XE::WASMOpcode::OP_F64_CONVERT_U_I32:
			break;
		case XE::WASMOpcode::OP_F64_CONVERT_S_I64:
			break;
		case XE::WASMOpcode::OP_F64_CONVERT_U_I64:
			break;
		case XE::WASMOpcode::OP_F64_PROMOTE_F32:
			break;
		case XE::WASMOpcode::OP_I32_REINTERPRET_F32:
			break;
		case XE::WASMOpcode::OP_I64_REINTERPRET_F64:
			break;
		case XE::WASMOpcode::OP_F32_REINTERPRET_I32:
			break;
		case XE::WASMOpcode::OP_F64_REINTERPRET_I64:
			break;
		case XE::WASMOpcode::OP_I32_EXTEND8_S:
			break;
		case XE::WASMOpcode::OP_I32_EXTEND16_S:
			break;
		case XE::WASMOpcode::OP_I64_EXTEND8_S:
			break;
		case XE::WASMOpcode::OP_I64_EXTEND16_S:
			break;
		case XE::WASMOpcode::OP_I64_EXTEND32_S:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xC5:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xC6:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xC7:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xC8:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xC9:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xCA:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xCB:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xCC:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xCD:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xCE:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xCF:
			break;
		case XE::WASMOpcode::OP_REF_NULL:
			break;
		case XE::WASMOpcode::OP_REF_IS_NULL:
			break;
		case XE::WASMOpcode::OP_REF_FUNC:
			break;
		case XE::WASMOpcode::OP_REF_AS_NON_NULL:
			break;
		case XE::WASMOpcode::OP_BR_ON_NULL:
			break;
		case XE::WASMOpcode::OP_REF_EQ:
			break;
		case XE::WASMOpcode::OP_BR_ON_NON_NULL:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xD7:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xD8:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xD9:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xDA:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xDB:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xDC:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xDD:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xDE:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xDF:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE0:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE1:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE2:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE3:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE4:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE5:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE6:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE7:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE8:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xE9:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xEA:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xEB:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xEC:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xED:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xEE:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xEF:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF0:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF1:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF2:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF3:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF4:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF5:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF6:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF7:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF8:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xF9:
			break;
		case XE::WASMOpcode::OP_UNUSED_0xFA:
			break;
		case XE::WASMOpcode::OP_GC_PREFIX:
		{
			XE::GCExtOpcode code = *reinterpret_cast<const XE::GCExtOpcode *>( view.data() + _Context->GetFrame().PC++ );
			switch ( code )
			{
			case XE::GCExtOpcode::OP_UNUSED_0x00:
				break;
			case XE::GCExtOpcode::OP_STRUCT_NEW_CANON:
				break;
			case XE::GCExtOpcode::OP_STRUCT_NEW_CANON_DEFAULT:
				break;
			case XE::GCExtOpcode::OP_STRUCT_GET:
				break;
			case XE::GCExtOpcode::OP_STRUCT_GET_S:
				break;
			case XE::GCExtOpcode::OP_STRUCT_GET_U:
				break;
			case XE::GCExtOpcode::OP_STRUCT_SET:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x07:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x08:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x09:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x0A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x0B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x0C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x0D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x0E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x0F:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x10:
				break;
			case XE::GCExtOpcode::OP_ARRAY_NEW_CANON:
				break;
			case XE::GCExtOpcode::OP_ARRAY_NEW_CANON_DEFAULT:
				break;
			case XE::GCExtOpcode::OP_ARRAY_GET:
				break;
			case XE::GCExtOpcode::OP_ARRAY_GET_S:
				break;
			case XE::GCExtOpcode::OP_ARRAY_GET_U:
				break;
			case XE::GCExtOpcode::OP_ARRAY_SET:
				break;
			case XE::GCExtOpcode::OP_ARRAY_LEN:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x18:
				break;
			case XE::GCExtOpcode::OP_ARRAY_NEW_CANON_FIXED:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x1A:
				break;
			case XE::GCExtOpcode::OP_ARRAY_NEW_CANON_DATA:
				break;
			case XE::GCExtOpcode::OP_ARRAY_NEW_CANON_ELEM:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x1D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x1E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x1F:
				break;
			case XE::GCExtOpcode::OP_I31_NEW:
				break;
			case XE::GCExtOpcode::OP_I31_GET_S:
				break;
			case XE::GCExtOpcode::OP_I31_GET_U:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x23:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x24:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x25:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x26:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x27:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x28:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x29:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x2A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x2B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x2C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x2D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x2E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x2F:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x30:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x31:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x32:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x33:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x34:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x35:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x36:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x37:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x38:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x39:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x3A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x3B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x3C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x3D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x3E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x3F:
				break;
			case XE::GCExtOpcode::OP_REF_TEST:
				break;
			case XE::GCExtOpcode::OP_REF_CAST:
				break;
			case XE::GCExtOpcode::OP_BR_ON_CAST:
				break;
			case XE::GCExtOpcode::OP_BR_ON_CAST_FAIL:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x44:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x45:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x46:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x47:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x48:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x49:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x4A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x4B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x4C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x4D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x4E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x4F:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x50:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x51:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x52:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x53:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x54:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x55:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x56:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x57:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x58:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x59:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x5A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x5B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x5C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x5D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x5E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x5F:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x60:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x61:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x62:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x63:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x64:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x65:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x66:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x67:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x68:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x69:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x6A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x6B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x6C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x6D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x6E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x6F:
				break;
			case XE::GCExtOpcode::OP_EXTERN_INTERNALIZE:
				break;
			case XE::GCExtOpcode::OP_EXTERN_EXTERNALIZE:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x72:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x73:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x74:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x75:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x76:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x77:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x78:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x79:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x7A:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x7B:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x7C:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x7D:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x7E:
				break;
			case XE::GCExtOpcode::OP_UNUSED_0x7F:
				break;
			default:
				break;
			}
		}
			break;
		case XE::WASMOpcode::OP_MISC_PREFIX:
		{
			XE::MiscExtOpcode code = *reinterpret_cast<const XE::MiscExtOpcode *>( view.data() + _Context->GetFrame().PC++ );
			switch ( code )
			{
			case XE::MiscExtOpcode::OP_I32_TRUNC_SAT_S_F32:
				break;
			case XE::MiscExtOpcode::OP_I32_TRUNC_SAT_U_F32:
				break;
			case XE::MiscExtOpcode::OP_I32_TRUNC_SAT_S_F64:
				break;
			case XE::MiscExtOpcode::OP_I32_TRUNC_SAT_U_F64:
				break;
			case XE::MiscExtOpcode::OP_I64_TRUNC_SAT_S_F32:
				break;
			case XE::MiscExtOpcode::OP_I64_TRUNC_SAT_U_F32:
				break;
			case XE::MiscExtOpcode::OP_I64_TRUNC_SAT_S_F64:
				break;
			case XE::MiscExtOpcode::OP_I64_TRUNC_SAT_U_F64:
				break;
			case XE::MiscExtOpcode::OP_MEMORY_INIT:
				break;
			case XE::MiscExtOpcode::OP_DATA_DROP:
				break;
			case XE::MiscExtOpcode::OP_MEMORY_COPY:
				break;
			case XE::MiscExtOpcode::OP_MEMORY_FILL:
				break;
			case XE::MiscExtOpcode::OP_TABLE_INIT:
				break;
			case XE::MiscExtOpcode::OP_ELEM_DROP:
				break;
			case XE::MiscExtOpcode::OP_TABLE_COPY:
				break;
			case XE::MiscExtOpcode::OP_TABLE_GROW:
				break;
			case XE::MiscExtOpcode::OP_TABLE_SIZE:
				break;
			case XE::MiscExtOpcode::OP_TABLE_FILL:
				break;
			default:
				break;
			}
		}
			break;
		case XE::WASMOpcode::OP_SIMD_PREFIX:
		{
			XE::SimdExtOpcode code = *reinterpret_cast<const XE::SimdExtOpcode *>( view.data() + _Context->GetFrame().PC++ );
			switch ( code )
			{
			case XE::SimdExtOpcode::OP_V128_LOAD:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD8X8_S:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD8X8_U:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD16X4_S:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD16X4_U:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD32X2_S:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD32X2_U:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD8_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD16_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD32_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD64_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_V128_STORE:
				break;
			case XE::SimdExtOpcode::OP_V128_CONST:
				break;
			case XE::SimdExtOpcode::OP_V8X16_SHUFFLE:
				break;
			case XE::SimdExtOpcode::OP_V8X16_SWIZZLE:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_I64X2_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_F32X4_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_F64X2_SPLAT:
				break;
			case XE::SimdExtOpcode::OP_I8X16_EXTRACT_LANE_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_EXTRACT_LANE_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_REPLACE_LANE:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTRACT_LANE_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTRACT_LANE_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_REPLACE_LANE:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTRACT_LANE:
				break;
			case XE::SimdExtOpcode::OP_I32X4_REPLACE_LANE:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTRACT_LANE:
				break;
			case XE::SimdExtOpcode::OP_I64X2_REPLACE_LANE:
				break;
			case XE::SimdExtOpcode::OP_F32X4_EXTRACT_LANE:
				break;
			case XE::SimdExtOpcode::OP_F32X4_REPLACE_LANE:
				break;
			case XE::SimdExtOpcode::OP_F64X2_EXTRACT_LANE:
				break;
			case XE::SimdExtOpcode::OP_F64X2_REPLACE_LANE:
				break;
			case XE::SimdExtOpcode::OP_I8X16_EQ:
				break;
			case XE::SimdExtOpcode::OP_I8X16_NE:
				break;
			case XE::SimdExtOpcode::OP_I8X16_LT_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_LT_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_GT_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_GT_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_LE_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_LE_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_GE_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_GE_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EQ:
				break;
			case XE::SimdExtOpcode::OP_I16X8_NE:
				break;
			case XE::SimdExtOpcode::OP_I16X8_LT_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_LT_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_GT_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_GT_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_LE_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_LE_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_GE_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_GE_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EQ:
				break;
			case XE::SimdExtOpcode::OP_I32X4_NE:
				break;
			case XE::SimdExtOpcode::OP_I32X4_LT_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_LT_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_GT_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_GT_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_LE_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_LE_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_GE_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_GE_U:
				break;
			case XE::SimdExtOpcode::OP_F32X4_EQ:
				break;
			case XE::SimdExtOpcode::OP_F32X4_NE:
				break;
			case XE::SimdExtOpcode::OP_F32X4_LT:
				break;
			case XE::SimdExtOpcode::OP_F32X4_GT:
				break;
			case XE::SimdExtOpcode::OP_F32X4_LE:
				break;
			case XE::SimdExtOpcode::OP_F32X4_GE:
				break;
			case XE::SimdExtOpcode::OP_F64X2_EQ:
				break;
			case XE::SimdExtOpcode::OP_F64X2_NE:
				break;
			case XE::SimdExtOpcode::OP_F64X2_LT:
				break;
			case XE::SimdExtOpcode::OP_F64X2_GT:
				break;
			case XE::SimdExtOpcode::OP_F64X2_LE:
				break;
			case XE::SimdExtOpcode::OP_F64X2_GE:
				break;
			case XE::SimdExtOpcode::OP_V128_NOT:
				break;
			case XE::SimdExtOpcode::OP_V128_AND:
				break;
			case XE::SimdExtOpcode::OP_V128_ANDNOT:
				break;
			case XE::SimdExtOpcode::OP_V128_OR:
				break;
			case XE::SimdExtOpcode::OP_V128_XOR:
				break;
			case XE::SimdExtOpcode::OP_V128_BITSELECT:
				break;
			case XE::SimdExtOpcode::OP_V128_ANY_TRUE:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD8_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD16_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD32_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD64_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_STORE8_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_STORE16_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_STORE32_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_STORE64_LANE:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD32_ZERO:
				break;
			case XE::SimdExtOpcode::OP_V128_LOAD64_ZERO:
				break;
			case XE::SimdExtOpcode::OP_F32X4_DEMOTE_F64X2_ZERO:
				break;
			case XE::SimdExtOpcode::OP_F64X2_PROMOTE_LOW_F32X4_ZERO:
				break;
			case XE::SimdExtOpcode::OP_I8X16_ABS:
				break;
			case XE::SimdExtOpcode::OP_I8X16_NEG:
				break;
			case XE::SimdExtOpcode::OP_I8X16_POPCNT:
				break;
			case XE::SimdExtOpcode::OP_I8X16_ALL_TRUE:
				break;
			case XE::SimdExtOpcode::OP_I8X16_BITMASK:
				break;
			case XE::SimdExtOpcode::OP_I8X16_NARROW_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_NARROW_I16X8_U:
				break;
			case XE::SimdExtOpcode::OP_F32X4_CEIL:
				break;
			case XE::SimdExtOpcode::OP_F32X4_FLOOR:
				break;
			case XE::SimdExtOpcode::OP_F32X4_TRUNC:
				break;
			case XE::SimdExtOpcode::OP_F32X4_NEAREST:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SHL:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SHR_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SHR_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_ADD:
				break;
			case XE::SimdExtOpcode::OP_I8X16_ADD_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_ADD_SAT_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SUB:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SUB_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_SUB_SAT_U:
				break;
			case XE::SimdExtOpcode::OP_F64X2_CEIL:
				break;
			case XE::SimdExtOpcode::OP_F64X2_FLOOR:
				break;
			case XE::SimdExtOpcode::OP_I8X16_MIN_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_MIN_U:
				break;
			case XE::SimdExtOpcode::OP_I8X16_MAX_S:
				break;
			case XE::SimdExtOpcode::OP_I8X16_MAX_U:
				break;
			case XE::SimdExtOpcode::OP_F64X2_TRUNC:
				break;
			case XE::SimdExtOpcode::OP_I8X16_AVGR_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTADD_PAIRWISE_I8X16_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTADD_PAIRWISE_I8X16_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTADD_PAIRWISE_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTADD_PAIRWISE_I16X8_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_ABS:
				break;
			case XE::SimdExtOpcode::OP_I16X8_NEG:
				break;
			case XE::SimdExtOpcode::OP_I16X8_Q15MULR_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_ALL_TRUE:
				break;
			case XE::SimdExtOpcode::OP_I16X8_BITMASK:
				break;
			case XE::SimdExtOpcode::OP_I16X8_NARROW_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_NARROW_I32X4_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTEND_LOW_I8X16_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTEND_HIGH_I8X16_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTEND_LOW_I8X16_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTEND_HIGH_I8X16_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SHL:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SHR_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SHR_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_ADD:
				break;
			case XE::SimdExtOpcode::OP_I16X8_ADD_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_ADD_SAT_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SUB:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SUB_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_SUB_SAT_U:
				break;
			case XE::SimdExtOpcode::OP_F64X2_NEAREST:
				break;
			case XE::SimdExtOpcode::OP_I16X8_MUL:
				break;
			case XE::SimdExtOpcode::OP_I16X8_MIN_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_MIN_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_MAX_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_MAX_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_AVGR_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTMUL_LOW_I8X16_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTMUL_HIGH_I8X16_S:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTMUL_LOW_I8X16_U:
				break;
			case XE::SimdExtOpcode::OP_I16X8_EXTMUL_HIGH_I8X16_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_ABS:
				break;
			case XE::SimdExtOpcode::OP_I32X4_NEG:
				break;
			case XE::SimdExtOpcode::OP_I32X4_ALL_TRUE:
				break;
			case XE::SimdExtOpcode::OP_I32X4_BITMASK:
				break;
			case XE::SimdExtOpcode::OP_I32X4_NARROW_I64X2_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_NARROW_I64X2_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTEND_LOW_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTEND_HIGH_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTEND_LOW_I16X8_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTEND_HIGH_I16X8_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SHL:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SHR_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SHR_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_ADD:
				break;
			case XE::SimdExtOpcode::OP_I32X4_ADD_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_ADD_SAT_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SUB:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SUB_SAT_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_SUB_SAT_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_MUL:
				break;
			case XE::SimdExtOpcode::OP_I32X4_MIN_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_MIN_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_MAX_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_MAX_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_DOT_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_AVGR_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTMUL_LOW_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTMUL_HIGH_I16X8_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTMUL_LOW_I16X8_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_EXTMUL_HIGH_I16X8_U:
				break;
			case XE::SimdExtOpcode::OP_I64X2_ABS:
				break;
			case XE::SimdExtOpcode::OP_I64X2_NEG:
				break;
			case XE::SimdExtOpcode::OP_I64X2_ALL_TRUE:
				break;
			case XE::SimdExtOpcode::OP_I64X2_BITMASK:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTEND_LOW_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTEND_HIGH_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTEND_LOW_I32X4_U:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTEND_HIGH_I32X4_U:
				break;
			case XE::SimdExtOpcode::OP_I64X2_SHL:
				break;
			case XE::SimdExtOpcode::OP_I64X2_SHR_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_SHR_U:
				break;
			case XE::SimdExtOpcode::OP_I64X2_ADD:
				break;
			case XE::SimdExtOpcode::OP_I64X2_SUB:
				break;
			case XE::SimdExtOpcode::OP_I64X2_MUL:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EQ:
				break;
			case XE::SimdExtOpcode::OP_I64X2_NE:
				break;
			case XE::SimdExtOpcode::OP_I64X2_LT_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_GT_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_LE_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_GE_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTMUL_LOW_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTMUL_HIGH_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTMUL_LOW_I32X4_U:
				break;
			case XE::SimdExtOpcode::OP_I64X2_EXTMUL_HIGH_I32X4_U:
				break;
			case XE::SimdExtOpcode::OP_F32X4_ABS:
				break;
			case XE::SimdExtOpcode::OP_F32X4_NEG:
				break;
			case XE::SimdExtOpcode::OP_F32X4_ROUND:
				break;
			case XE::SimdExtOpcode::OP_F32X4_SQRT:
				break;
			case XE::SimdExtOpcode::OP_F32X4_ADD:
				break;
			case XE::SimdExtOpcode::OP_F32X4_SUB:
				break;
			case XE::SimdExtOpcode::OP_F32X4_MUL:
				break;
			case XE::SimdExtOpcode::OP_F32X4_DIV:
				break;
			case XE::SimdExtOpcode::OP_F32X4_MIN:
				break;
			case XE::SimdExtOpcode::OP_F32X4_MAX:
				break;
			case XE::SimdExtOpcode::OP_F32X4_PMIN:
				break;
			case XE::SimdExtOpcode::OP_F32X4_PMAX:
				break;
			case XE::SimdExtOpcode::OP_F64X2_ABS:
				break;
			case XE::SimdExtOpcode::OP_F64X2_NEG:
				break;
			case XE::SimdExtOpcode::OP_F64X2_ROUND:
				break;
			case XE::SimdExtOpcode::OP_F64X2_SQRT:
				break;
			case XE::SimdExtOpcode::OP_F64X2_ADD:
				break;
			case XE::SimdExtOpcode::OP_F64X2_SUB:
				break;
			case XE::SimdExtOpcode::OP_F64X2_MUL:
				break;
			case XE::SimdExtOpcode::OP_F64X2_DIV:
				break;
			case XE::SimdExtOpcode::OP_F64X2_MIN:
				break;
			case XE::SimdExtOpcode::OP_F64X2_MAX:
				break;
			case XE::SimdExtOpcode::OP_F64X2_PMIN:
				break;
			case XE::SimdExtOpcode::OP_F64X2_PMAX:
				break;
			case XE::SimdExtOpcode::OP_I32X4_TRUNC_SAT_F32X4_S:
				break;
			case XE::SimdExtOpcode::OP_I32X4_TRUNC_SAT_F32X4_U:
				break;
			case XE::SimdExtOpcode::OP_F32X4_CONVERT_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_F32X4_CONVERT_I32X4_U:
				break;
			case XE::SimdExtOpcode::OP_I32X4_TRUNC_SAT_F64X2_S_ZERO:
				break;
			case XE::SimdExtOpcode::OP_I32X4_TRUNC_SAT_F64X2_U_ZERO:
				break;
			case XE::SimdExtOpcode::OP_F64X2_CONVERT_LOW_I32X4_S:
				break;
			case XE::SimdExtOpcode::OP_F64X2_CONVERT_LOW_I32X4_U:
				break;
			default:
				break;
			}
		}
			break;
		case XE::WASMOpcode::OP_THREAD_PREFIX:
		{
			XE::AtomicExtOpcode code = *reinterpret_cast<const XE::AtomicExtOpcode *>( view.data() + _Context->GetFrame().PC++ );
			switch ( code )
			{
			case XE::AtomicExtOpcode::OP_ATOMIC_NOTIFY:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_WAIT32:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_WAIT64:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_FENCE:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I32_LOAD:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_LOAD:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I32_LOAD8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I32_LOAD16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_LOAD8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_LOAD16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_LOAD32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I32_STORE:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_STORE:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I32_STORE8:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I32_STORE16:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_STORE8:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_STORE16:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_I64_STORE32:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_ADD:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_ADD:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_ADD8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_ADD16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_ADD8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_ADD16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_ADD32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_SUB:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_SUB:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_SUB8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_SUB16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_SUB8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_SUB16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_SUB32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_AND:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_AND:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_AND8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_AND16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_AND8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_AND16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_AND32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_OR:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_OR:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_OR8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_OR16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_OR8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_OR16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_OR32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_XOR:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XOR:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_XOR8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_XOR16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XOR8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XOR16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XOR32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_XCHG:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XCHG:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_XCHG8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_XCHG16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XCHG8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XCHG16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_XCHG32_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_CMPXCHG:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_CMPXCHG:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_CMPXCHG8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I32_CMPXCHG16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_CMPXCHG8_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_CMPXCHG16_U:
				break;
			case XE::AtomicExtOpcode::OP_ATOMIC_RMW_I64_CMPXCHG32_U:
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}
	}

__END:


	return -1;
}
