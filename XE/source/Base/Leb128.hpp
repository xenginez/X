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

template< typename T, std::enable_if_t< std::is_arithmetic_v< T >, int > = 0 > XE::uint64 EncodeLeb128( T val, std::ostream & stream )
{
	bool negative = ( val < 0 );

	for ( XE::uint64 i = 0; true; i++ )
	{
		XE::uint8 b = val & 0x7F;

		val >>= 7;

		if ( negative )
		{
			val |= ( ~0ULL << 57 );
		}

		if ( ( ( val == 0 ) && ( !( b & 0x40 ) ) ) || ( ( val == -1 ) && ( b & 0x40 ) ) )
		{
			stream.put( XE::BitCast<char>( b ) );

			return i;
		}
		else
		{
			stream.put( XE::BitCast<char>( b | 0x80 ) );
		}
	}
}

template< typename T, std::enable_if_t< std::is_arithmetic_v< T >, int > = 0 > XE::uint64 DecodeLeb128( std::istream & stream, T & val )
{
	XE::uint64 res = 0;
	XE::uint64 shift = 0;
	XE::uint64 i = 0;

	for ( XE::uint64 i = 0; true; i++ )
	{
		XE::uint8 b = XE::BitCast<XE::uint8>( stream.get() );
		XE::uint64 slice = b & 0x7F;

		res |= slice << shift;
		shift += 7;

		if ( !( b & 0x80 ) )
		{
			if ( ( shift < 64 ) && ( b & 0x40 ) )
			{
				val = res | std::numeric_limits<XE::uint64>::max() << shift;
			}
			else
			{
				val = res;
			}

			return i;
		}
	}
}

END_XE_NAMESPACE

#endif//LEB128_HPP__183EE27C_7FA0_42E6_9098_9295761ABBFE
