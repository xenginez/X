/*!
 * \file	Leb128.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/03/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LEB128_HPP__183EE27C_7FA0_42E6_9098_9295761ABBFE
#define LEB128_HPP__183EE27C_7FA0_42E6_9098_9295761ABBFE

#include "Type.h"

BEG_XE_NAMESPACE

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::int8 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	bool negative = ( val < 0 );
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = val & 0x7F;

		val >>= 7;
		if( negative )
		{
			val |= ( ~0ULL << 57 );
		}
		if( ( ( val == 0 ) && ( !( b & 0x40 ) ) ) || ( ( val == -1 ) && ( b & 0x40 ) ) )
		{
			data[i++] = b;
			return i;
		}
		else
		{
			data[i++] = b | 0x80;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::int16 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	bool negative = ( val < 0 );
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = val & 0x7F;

		val >>= 7;
		if( negative )
		{
			val |= ( ~0ULL << 57 );
		}
		if( ( ( val == 0 ) && ( !( b & 0x40 ) ) ) || ( ( val == -1 ) && ( b & 0x40 ) ) )
		{
			data[i++] = b;
			return i;
		}
		else
		{
			data[i++] = b | 0x80;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::int32 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	bool negative = ( val < 0 );
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = val & 0x7F;

		val >>= 7;
		if( negative )
		{
			val |= ( ~0ULL << 57 );
		}
		if( ( ( val == 0 ) && ( !( b & 0x40 ) ) ) || ( ( val == -1 ) && ( b & 0x40 ) ) )
		{
			data[i++] = b;
			return i;
		}
		else
		{
			data[i++] = b | 0x80;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::int64 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	bool negative = ( val < 0 );
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = val & 0x7F;

		val >>= 7;
		if( negative )
		{
			val |= ( ~0ULL << 57 );
		}
		if( ( ( val == 0 ) && ( !( b & 0x40 ) ) ) || ( ( val == -1 ) && ( b & 0x40 ) ) )
		{
			data[i++] = b;
			return i;
		}
		else
		{
			data[i++] = b | 0x80;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::uint8 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	std::size_t i = 0;
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	do
	{
		XE::uint8 b = val & 0x7F;
		val >>= 7;
		if( val != 0 )
		{
			b |= 0x80;
		}
		data[i++] = b;
	} while( val != 0 );

	return i;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::uint16 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	std::size_t i = 0;
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	do
	{
		XE::uint8 b = val & 0x7F;
		val >>= 7;
		if( val != 0 )
		{
			b |= 0x80;
		}
		data[i++] = b;
	} while( val != 0 );

	return i;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::uint32 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	std::size_t i = 0;
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	do
	{
		XE::uint8 b = val & 0x7F;
		val >>= 7;
		if( val != 0 )
		{
			b |= 0x80;
		}
		data[i++] = b;
	} while( val != 0 );

	return i;
}

XE_INLINE XE::uint64 EncodeLeb128( void * buffer, XE::uint64 val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	std::size_t i = 0;
	XE::uint8 * data = reinterpret_cast< XE::uint8 * >( buffer );

	do
	{
		XE::uint8 b = val & 0x7F;
		val >>= 7;
		if( val != 0 )
		{
			b |= 0x80;
		}
		data[i++] = b;
	} while( val != 0 );

	return i;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::int8 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = data[i++];
		XE::uint64 slice = b & 0x7F;
		res |= slice << shift;
		shift += 7;
		if( !( b & 0x80 ) )
		{
			if( ( shift < 64 ) && ( b & 0x40 ) )
			{
				val = res | ( -1ULL ) << shift;
				return i;
			}

			val = res;
			return i;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::int16 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = data[i++];
		XE::uint64 slice = b & 0x7F;
		res |= slice << shift;
		shift += 7;
		if( !( b & 0x80 ) )
		{
			if( ( shift < 64 ) && ( b & 0x40 ) )
			{
				val = res | ( -1ULL ) << shift;
				return i;
			}

			val = res;
			return i;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::int32 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = data[i++];
		XE::uint64 slice = b & 0x7F;
		res |= slice << shift;
		shift += 7;
		if( !( b & 0x80 ) )
		{
			if( ( shift < 64 ) && ( b & 0x40 ) )
			{
				val = res | ( -1ULL ) << shift;
				return i;
			}

			val = res;
			return i;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::int64 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint8 b = data[i++];
		XE::uint64 slice = b & 0x7F;
		res |= slice << shift;
		shift += 7;
		if( !( b & 0x80 ) )
		{
			if( ( shift < 64 ) && ( b & 0x40 ) )
			{
				val = res | ( -1ULL ) << shift;
				return i;
			}

			val = res;
			return i;
		}
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::uint8 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint64 b = data[i++];
		res |= ( b & 0x7F ) << shift;
		if( !( b & 0x80 ) )
		{
			val = res;
			return i;
		}
		shift += 7;
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::uint16 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint64 b = data[i++];
		res |= ( b & 0x7F ) << shift;
		if( !( b & 0x80 ) )
		{
			val = res;
			return i;
		}
		shift += 7;
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::uint32 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint64 b = data[i++];
		res |= ( b & 0x7F ) << shift;
		if( !( b & 0x80 ) )
		{
			val = res;
			return i;
		}
		shift += 7;
	}

	return 0;
}

XE_INLINE XE::uint64 DecodeLeb128( const void * buffer, XE::uint64 & val )
{
	if( buffer == nullptr )
	{
		return 0;
	}

	XE::uint64 res = 0, shift = 0;
	const XE::uint8 * data = reinterpret_cast< const XE::uint8 * >( buffer );

	for( size_t i = 0; i < 10; i++ )
	{
		XE::uint64 b = data[i++];
		res |= ( b & 0x7F ) << shift;
		if( !( b & 0x80 ) )
		{
			val = res;
			return i;
		}
		shift += 7;
	}

	return 0;
}

END_XE_NAMESPACE

#endif//LEB128_HPP__183EE27C_7FA0_42E6_9098_9295761ABBFE
