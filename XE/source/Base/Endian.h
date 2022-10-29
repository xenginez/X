/*!
 * \file   Endian.h
 *
 * \author ZhengYuanQing
 * \date   2022/02/06
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef ENDIAN_H__A7A89663_2D28_4A60_A458_3C31319A94AA
#define ENDIAN_H__A7A89663_2D28_4A60_A458_3C31319A94AA

#include "Type.h"

BEG_XE_NAMESPACE

XE_INLINE bool IsBigEndian()
{
	XE::int32 i = 1;

	XE::uint8 * pointer = ( XE::uint8 * ) &i;

	return ( *pointer ) != 0;
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::int8 & val )
{
	auto buf = reinterpret_cast< const XE::int8 * >( buffer );
	val = *buf;
	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::int16 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int16 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );

	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
	}
	else
	{
		u.d[1] = buf[0];
		u.d[0] = buf[1];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::int32 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int32 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::int64 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int64 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];

		u.d[4] = buf[4];
		u.d[5] = buf[5];
		u.d[6] = buf[6];
		u.d[7] = buf[7];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];

		u.d[7] = buf[4];
		u.d[6] = buf[5];
		u.d[5] = buf[6];
		u.d[4] = buf[7];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::uint8 & val )
{
	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	val = *buf;
	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::uint16 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint16 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
	}
	else
	{
		u.d[1] = buf[0];
		u.d[0] = buf[1];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::uint32 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint32 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::uint64 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint64 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];

		u.d[4] = buf[4];
		u.d[5] = buf[5];
		u.d[6] = buf[6];
		u.d[7] = buf[7];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];

		u.d[7] = buf[4];
		u.d[6] = buf[5];
		u.d[5] = buf[6];
		u.d[4] = buf[7];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::float32 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float32 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadBigEndian( const void * buffer, XE::float64 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float64 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];

		u.d[4] = buf[4];
		u.d[5] = buf[5];
		u.d[6] = buf[6];
		u.d[7] = buf[7];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];

		u.d[7] = buf[4];
		u.d[6] = buf[5];
		u.d[5] = buf[6];
		u.d[4] = buf[7];
	}

	val = u.val;

	return sizeof( val );
}

template< typename T > T ReadBigEndian( const void * buffer )
{
	static_assert( std::is_fundamental_v< T >, "" );

	T result = 0;

	ReadBigEndian( buffer, result );

	return result;
}

XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::int8 val )
{
	auto buf = reinterpret_cast< XE::int8 * >( buffer );
	*buf = val;
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::int16 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int16 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
	}
	else
	{
		buf[0] = u.d[1];
		buf[1] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::int32 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int32 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::int64 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int64 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];

		buf[4] = u.d[4];
		buf[5] = u.d[5];
		buf[6] = u.d[6];
		buf[7] = u.d[7];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];

		buf[4] = u.d[7];
		buf[5] = u.d[6];
		buf[6] = u.d[5];
		buf[7] = u.d[4];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::uint8 val )
{
	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	*buf = val;
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::uint16 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint16 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
	}
	else
	{
		buf[0] = u.d[1];
		buf[1] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::uint32 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint32 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::uint64 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint64 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];

		buf[4] = u.d[4];
		buf[5] = u.d[5];
		buf[6] = u.d[6];
		buf[7] = u.d[7];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];

		buf[4] = u.d[7];
		buf[5] = u.d[6];
		buf[6] = u.d[5];
		buf[7] = u.d[4];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::float32 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float32 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteBigEndian( void * buffer, XE::float64 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float64 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsBigEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];

		buf[4] = u.d[4];
		buf[5] = u.d[5];
		buf[6] = u.d[6];
		buf[7] = u.d[7];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];

		buf[4] = u.d[7];
		buf[5] = u.d[6];
		buf[6] = u.d[5];
		buf[7] = u.d[4];
	}
	return sizeof( val );
}

XE_INLINE bool IsLittleEndian()
{
	XE::int32 i = 1;

	XE::uint8 * pointer = ( XE::uint8 * ) &i;

	return ( *pointer ) == 0;
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::int8 & val )
{
	auto buf = reinterpret_cast< const XE::int8 * >( buffer );
	val = *buf;
	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::int16 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int16 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );

	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
	}
	else
	{
		u.d[1] = buf[0];
		u.d[0] = buf[1];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::int32 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int32 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::int64 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int64 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];

		u.d[4] = buf[4];
		u.d[5] = buf[5];
		u.d[6] = buf[6];
		u.d[7] = buf[7];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];

		u.d[7] = buf[4];
		u.d[6] = buf[5];
		u.d[5] = buf[6];
		u.d[4] = buf[7];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::uint8 & val )
{
	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	val = *buf;
	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::uint16 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint16 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
	}
	else
	{
		u.d[1] = buf[0];
		u.d[0] = buf[1];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::uint32 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint32 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::uint64 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint64 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];

		u.d[4] = buf[4];
		u.d[5] = buf[5];
		u.d[6] = buf[6];
		u.d[7] = buf[7];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];

		u.d[7] = buf[4];
		u.d[6] = buf[5];
		u.d[5] = buf[6];
		u.d[4] = buf[7];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::float32 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float32 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];
	}

	val = u.val;

	return sizeof( val );
}
XE_INLINE XE::uint64 ReadLittleEndian( const void * buffer, XE::float64 & val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float64 val;
	} u;

	auto buf = reinterpret_cast< const XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		u.d[0] = buf[0];
		u.d[1] = buf[1];
		u.d[2] = buf[2];
		u.d[3] = buf[3];

		u.d[4] = buf[4];
		u.d[5] = buf[5];
		u.d[6] = buf[6];
		u.d[7] = buf[7];
	}
	else
	{
		u.d[3] = buf[0];
		u.d[2] = buf[1];
		u.d[1] = buf[2];
		u.d[0] = buf[3];

		u.d[7] = buf[4];
		u.d[6] = buf[5];
		u.d[5] = buf[6];
		u.d[4] = buf[7];
	}

	val = u.val;

	return sizeof( val );
}

template< typename T > T ReadLittleEndian( const void * buffer )
{
	static_assert( std::is_fundamental_v< T >, "" );

	T result = 0;

	ReadLittleEndian( buffer, result );

	return result;
}

XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::int8 val )
{
	auto buf = reinterpret_cast< XE::int8 * >( buffer );
	*buf = val;
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::int16 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int16 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
	}
	else
	{
		buf[0] = u.d[1];
		buf[1] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::int32 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int32 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::int64 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::int64 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];

		buf[4] = u.d[4];
		buf[5] = u.d[5];
		buf[6] = u.d[6];
		buf[7] = u.d[7];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];

		buf[4] = u.d[7];
		buf[5] = u.d[6];
		buf[6] = u.d[5];
		buf[7] = u.d[4];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::uint8 val )
{
	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	*buf = val;
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::uint16 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint16 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
	}
	else
	{
		buf[0] = u.d[1];
		buf[1] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::uint32 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint32 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::uint64 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::uint64 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];

		buf[4] = u.d[4];
		buf[5] = u.d[5];
		buf[6] = u.d[6];
		buf[7] = u.d[7];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];

		buf[4] = u.d[7];
		buf[5] = u.d[6];
		buf[6] = u.d[5];
		buf[7] = u.d[4];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::float32 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float32 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];
	}
	return sizeof( val );
}
XE_INLINE XE::uint64 WriteLittleEndian( void * buffer, XE::float64 val )
{
	union
	{
		XE::uint8 d[sizeof(val)];
		XE::float64 val;
	} u;

	u.val = val;

	auto buf = reinterpret_cast< XE::uint8 * >( buffer );
	if( IsLittleEndian() )
	{
		buf[0] = u.d[0];
		buf[1] = u.d[1];
		buf[2] = u.d[2];
		buf[3] = u.d[3];

		buf[4] = u.d[4];
		buf[5] = u.d[5];
		buf[6] = u.d[6];
		buf[7] = u.d[7];
	}
	else
	{
		buf[0] = u.d[3];
		buf[1] = u.d[2];
		buf[2] = u.d[1];
		buf[3] = u.d[0];

		buf[4] = u.d[7];
		buf[5] = u.d[6];
		buf[6] = u.d[5];
		buf[7] = u.d[4];
	}
	return sizeof( val );
}

END_XE_NAMESPACE

#endif // ENDIAN_H__A7A89663_2D28_4A60_A458_3C31319A94AA
