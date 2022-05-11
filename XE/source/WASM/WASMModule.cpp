#include "WASMModule.h"

namespace
{
	XE::uint64 read_str( const void * buffer, XE::Utf8String & str )
	{
		XE::uint64 res = 0;
		XE::uint64 size = 0;
		res += XE::DecodeLeb128( buffer, size );

		str.resize( size );

		auto src = reinterpret_cast< const char * >( buffer ) + res;
		std::copy( src, src + size, str.data() );

		return res + size;
	}

	XE::uint64 read_init_expr( const void * buffer, XE::InitExpr & expr )
	{
		XE::uint64 idx = 0;

		XE::uint8 opcode = 0;
		idx += XE::DecodeLeb128( buffer, opcode );
		expr.Type = XE::ExprType::VALUE;

		switch( ( XE::Opcode ) opcode )
		{
		case XE::Opcode::OP_I32_CONST:
			idx += XE::DecodeLeb128( reinterpret_cast< const XE::uint8 * >( buffer ) + idx, expr.Val.i32 );
			break;
		case XE::Opcode::OP_I64_CONST:
			idx += XE::DecodeLeb128( reinterpret_cast< const XE::uint8 * >( buffer ) + idx, expr.Val.i64 );
			break;
		case XE::Opcode::OP_F32_CONST:
			idx += XE::ReadLittleEndian( reinterpret_cast< const XE::uint8 * >( buffer ) + idx, expr.Val.f32 );
			break;
		case XE::Opcode::OP_F64_CONST:
			idx += XE::ReadLittleEndian( reinterpret_cast< const XE::uint8 * >( buffer ) + idx, expr.Val.f64 );
			break;
		case XE::Opcode::OP_GET_GLOBAL:
			idx += XE::DecodeLeb128( reinterpret_cast< const XE::uint8 * >( buffer ) + idx, expr.Val.i32 );
			expr.Type = XE::ExprType::INDEX;
			break;
		default:
			break;
		}

		return idx;
	}
}

XE::WASMModule::WASMModule()
{

}

XE::WASMModule::~WASMModule()
{

}

void XE::WASMModule::Load( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 magic = 0, version = 0;

	data += XE::ReadLittleEndian( data, magic );
	data += XE::ReadLittleEndian( data, version );

	while( data < ( view.data() + view.size() ) )
	{
		XE::SectionType s_type = SectionType::CUSTOM;
		XE::uint64 s_size = 0;

		data += XE::ReadLittleEndian( data, reinterpret_cast< XE::uint8 & >( s_type ) );
		data += XE::DecodeLeb128( data, s_size );

		switch( s_type )
		{
		case XE::SectionType::CUSTOM:
			LoadCustomSecions( { data, s_size } );
			break;
		case XE::SectionType::TYPE:
			LoadTypeSecions( { data, s_size } );
			break;
		case XE::SectionType::IMPORT:
			LoadImportSecions( { data, s_size } );
			break;
		case XE::SectionType::FUNC:
			LoadFuncSecions( { data, s_size } );
			break;
		case XE::SectionType::TABLE:
			LoadTableSecions( { data, s_size } );
			break;
		case XE::SectionType::MEMORY:
			LoadMemorySecions( { data, s_size } );
			break;
		case XE::SectionType::GLOBAL:
			LoadGlobalSecions( { data, s_size } );
			break;
		case XE::SectionType::EXPORT:
			LoadExportSecions( { data, s_size } );
			break;
		case XE::SectionType::START:
			LoadStartSecions( { data, s_size } );
			break;
		case XE::SectionType::ELEM:
			LoadElemSecions( { data, s_size } );
			break;
		case XE::SectionType::CODE:
			LoadCodeSecions( { data, s_size } );
			break;
		case XE::SectionType::DATA:
			LoadDataSecions( { data, s_size } );
			break;
		default:
			break;
		}

		data += s_size;
	}
}

void XE::WASMModule::LoadCustomSecions( XE::MemoryView view )
{

}

void XE::WASMModule::LoadTypeSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Types.resize( size );

	for( size_t i = 0; i < size; i++ )
	{
		{
			XE::uint64 flag = 0;
			data += XE::DecodeLeb128( data, flag );
		}

		{
			XE::uint64 p_size = 0;
			data += XE::DecodeLeb128( data, p_size );
			_Types[i].Parameter.resize( p_size );
			for( size_t j = 0; j < p_size; j++ )
			{
				XE::uint64 t = 0;
				data += XE::DecodeLeb128( data, t );
				_Types[i].Parameter[j] = static_cast< XE::ValueType >( t );
			}
		}

		{
			XE::uint64 r_size = 0;
			data += XE::DecodeLeb128( data, r_size );
			_Types[i].Results.resize( r_size );
			for( size_t j = 0; j < r_size; j++ )
			{
				XE::uint64 t = 0;
				data += XE::DecodeLeb128( data, t );
				_Types[i].Results[j] = static_cast< XE::ValueType >( t );
			}
		}
	}
}

void XE::WASMModule::LoadImportSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Imports.resize( size );

	for( size_t i = 0; i < size; i++ )
	{
		data += read_str( data, _Imports[i].Module );
		data += read_str( data, _Imports[i].Field );

		data += XE::ReadLittleEndian( data, reinterpret_cast< XE::uint8 & >( _Imports[i].Desc ) );

		switch( _Imports[i].Desc )
		{
		case XE::DescType::FUNC:
		{
			XE::uint64 type_idx = 0;
			data += XE::DecodeLeb128( data, _Imports[i].TypeIdx );
			_ImportFuncSize++;
		}
			break;
		case XE::DescType::TABLE:
		{
			XE::uint32 elem_t = 0;
			data += XE::DecodeLeb128( data, elem_t );
			XE_ASSERT( elem_t == 0x70 && "" );

			XE::uint32 flag = 0;
			data += XE::DecodeLeb128( data, flag );

			data += XE::DecodeLeb128( data, _Imports[i].Limits.Min );

			if( flag == 0 )
			{
				_Imports[i].Limits.Max = KBYTE( 64 );
			}
			else
			{
				data += XE::DecodeLeb128( data, _Imports[i].Limits.Max );
			}

			_ImportTableSize++;
		}
			break;
		case XE::DescType::MEM:
		{
			XE::uint32 flag = 0;
			data += XE::DecodeLeb128( data, flag );

			data += XE::DecodeLeb128( data, _Imports[i].Limits.Min );

			if( flag == 0 )
			{
				_Imports[i].Limits.Max = KBYTE( 64 );
			}
			else
			{
				data += XE::DecodeLeb128( data, _Imports[i].Limits.Max );
			}

			_ImportMemorySize++;
		}
			break;
		case XE::DescType::GLOBAL:
		{
			data += XE::DecodeLeb128( data, reinterpret_cast< XE::uint8 & >( _Imports[i].Global.Type ) );
			data += XE::DecodeLeb128( data, _Imports[i].Global.Mutable );

			_ImportGlobalSize++;
		}
			break;
		default:
			break;
		}
	}
}

void XE::WASMModule::LoadFuncSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Funcs.resize( size );

	for( size_t i = _ImportFuncSize; i < size; i++ )
	{
		data += XE::DecodeLeb128( data, _Funcs[i].TypeIdx );
	}
}

void XE::WASMModule::LoadTableSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Tables.resize( size );
	XE_ASSERT( size == 1 && "More than 1 table not supported" );

	XE::uint32 elem_t = 0;
	data += XE::DecodeLeb128( data, elem_t );
	XE_ASSERT( elem_t == 0x70 && "" );

	XE::uint32 flag = 0;
	data += XE::DecodeLeb128( data, flag );

	data += XE::DecodeLeb128( data, _Tables[0].Min );

	if( flag == 0 )
	{
		_Tables[0].Max = KBYTE( 64 );
	}
	else
	{
		data += XE::DecodeLeb128( data, _Tables[0].Max );
	}
}

void XE::WASMModule::LoadMemorySecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Memorys.resize( size );
	XE_ASSERT( size == 1 && "More than 1 memory not supported" );

	XE::uint32 flag = 0;
	data += XE::DecodeLeb128( data, flag );

	data += XE::DecodeLeb128( data, _Memorys[0].Min );

	if( flag == 0 )
	{
		_Memorys[0].Max = KBYTE( 64 );
	}
	else
	{
		data += XE::DecodeLeb128( data, _Memorys[0].Max );
	}
}

void XE::WASMModule::LoadGlobalSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Globals.resize( size );

	for( size_t i = 0; i < size; i++ )
	{
		data += XE::DecodeLeb128( data, reinterpret_cast< XE::uint8 & >( _Globals[i].Type ) );
		data += XE::DecodeLeb128( data, _Globals[i].Mutable );
		data += read_init_expr( data, _Globals[i].Init );
	}
}

void XE::WASMModule::LoadExportSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Exports.resize( size );

	for( size_t i = 0; i < size; i++ )
	{
		data += read_str( data, _Exports[i].Field );
		data += XE::DecodeLeb128( data, reinterpret_cast< XE::uint8 & >( _Exports[i].Desc ) );
		data += XE::DecodeLeb128( data, _Exports[i].DescIdx );
	}
}

void XE::WASMModule::LoadStartSecions( XE::MemoryView view )
{
	XE::DecodeLeb128( view.data(), _Start );
}

void XE::WASMModule::LoadElemSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Elems.resize( size );

	for( size_t i = 0; i < size; i++ )
	{
		data += XE::DecodeLeb128( data, _Elems[i].TableIdx );
		data += read_init_expr( data, _Elems[i].Offset );
		XE::uint32 f_size = 0;
		data += XE::DecodeLeb128( data, f_size );
		_Elems[i].FuncIdxs.resize( f_size );

		for( size_t j = 0; j < f_size; j++ )
		{
			data += XE::DecodeLeb128( data, _Elems[i].FuncIdxs[j] );
		}
	}
}

void XE::WASMModule::LoadCodeSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );

	for( size_t i = 0; i < size; i++ )
	{
		auto & func = _Funcs[i + _ImportFuncSize];

		XE::uint64 c_size = 0;
		data += XE::DecodeLeb128( data, c_size );

		auto payload = data;

		XE::uint32 l_size = 0;
		data += XE::DecodeLeb128( data, l_size );

		for( size_t j = 0; j < l_size; j++ )
		{
			XE::uint32 count = 0; XE::ValueType type = ValueType::NONE;
			data += XE::DecodeLeb128( data, count );
			data += XE::DecodeLeb128( data, reinterpret_cast< XE::uint8 & >( type ) );
			func.Locals.insert( func.Locals.end(), count, type );
		}

		func.Codes.assign( data, payload + c_size - 1 );
		
		data = payload + c_size - 1;

		XE_ASSERT( *data == 0x0b && "code section did not end with 0x0b" );

		data++;
	}
}

void XE::WASMModule::LoadDataSecions( XE::MemoryView view )
{
	auto data = view.data();

	XE::uint64 size = 0;
	data += XE::DecodeLeb128( data, size );
	_Datas.resize( size );

	for( size_t i = 0; i < size; i++ )
	{
		data += XE::DecodeLeb128( data, _Datas[i].MemIdx );

		data += read_init_expr( data, _Datas[i].Offset );

		XE::uint64 d_size = 0;
		data += XE::DecodeLeb128( data, d_size );
		_Datas[i].Init.assign( data, data + d_size );
		data += d_size;
	}
}
