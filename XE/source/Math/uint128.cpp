#include "uint128.h"
#include "int128.h"

#include <intrin.h>

constexpr XE::uint128::uint128( XE::int8 val ) noexcept
	: low( val ), high( val < 0 ? std::numeric_limits<XE::uint64>::max() : 0 )
{
}

constexpr XE::uint128::uint128( XE::int16 val ) noexcept
	: low( val ), high( val < 0 ? std::numeric_limits<XE::uint64>::max() : 0 )
{
}

constexpr XE::uint128::uint128( XE::int32 val ) noexcept
	: low( val ), high( val < 0 ? std::numeric_limits<XE::uint64>::max() : 0 )
{
}

constexpr XE::uint128::uint128( XE::int64 val ) noexcept
	: low( val ), high( val < 0 ? std::numeric_limits<XE::uint64>::max() : 0 )
{
}

constexpr XE::uint128::uint128( XE::uint8 val ) noexcept
	: low( val ), high( 0 )
{
}

constexpr XE::uint128::uint128( XE::uint16 val ) noexcept
	: low( val ), high( 0 )
{
}

constexpr XE::uint128::uint128( XE::uint32 val ) noexcept
	: low( val ), high( 0 )
{
}

constexpr XE::uint128::uint128( XE::uint64 val ) noexcept
	: low( val ), high( 0 )
{
}

constexpr XE::uint128::uint128( const XE::int128 & val ) noexcept
	: low( val.low ), high( static_cast<XE::uint64>( val.high ) )
{
}

constexpr XE::uint128::uint128( XE::uint64 high, XE::uint64 low ) noexcept
	: low( low ), high( high )
{
}

constexpr XE::uint128::operator bool() const noexcept
{
	return static_cast<bool>( low ) || static_cast<bool>( high );
}

constexpr XE::uint128::operator XE::int8() const noexcept
{
	return static_cast<XE::int8>( low );
}

constexpr XE::uint128::operator XE::int16() const noexcept
{
	return static_cast<XE::int16>( low );
}

constexpr XE::uint128::operator XE::int32() const noexcept
{
	return static_cast<XE::int32>( low );
}

constexpr XE::uint128::operator XE::int64() const noexcept
{
	return static_cast<XE::int64>( low );
}

constexpr XE::uint128::operator XE::uint8() const noexcept
{
	return static_cast<XE::uint8>( low );
}

constexpr XE::uint128::operator XE::uint16() const noexcept
{
	return static_cast<XE::uint16>( low );
}

constexpr XE::uint128::operator XE::uint32() const noexcept
{
	return static_cast<XE::uint32>( low );
}

constexpr XE::uint128::operator XE::uint64() const noexcept
{
	return static_cast<XE::uint64>( low );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::int8 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::int16 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::int32 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::int64 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::uint8 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::uint16 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::uint32 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( XE::uint64 val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr XE::uint128 & XE::uint128::operator=( const XE::int128 & val ) noexcept
{
	return *this = XE::uint128( val );
}

constexpr bool XE::uint128::operator< ( XE::uint128 val ) noexcept
{
	return this->high == val.high ? this->low < val.low : this->high < val.high;
}

constexpr bool XE::uint128::operator> ( XE::uint128 val ) noexcept
{
	return val < ( *this );
}

constexpr bool XE::uint128::operator<=( XE::uint128 val ) noexcept
{
	return !( ( *this ) > val );
}

constexpr bool XE::uint128::operator>=( XE::uint128 val ) noexcept
{
	return !( ( *this ) < val );
}

constexpr bool XE::uint128::operator==( XE::uint128 val ) noexcept
{
	return this->low == val.low && this->high == val.high;
}

constexpr bool XE::uint128::operator!=( XE::uint128 val ) noexcept
{
	return !( ( *this ) == val );;
}

constexpr XE::uint128 XE::uint128::operator+ ( XE::uint128 val ) noexcept
{
	XE::uint64 Carry = ( std::numeric_limits<XE::uint64>::max() - this->low ) < val.low ? 1 : 0;
	return XE::uint128( this->high + val.high + Carry, this->low + val.low );
}

constexpr XE::uint128 XE::uint128::operator- ( XE::uint128 val ) noexcept
{
	XE::uint64 Carry = this->low < val.low ? 1 : 0;
	return XE::uint128( this->high - val.high - Carry, this->low - val.low );
}

constexpr XE::uint128 XE::uint128::operator* ( XE::uint128 val ) noexcept
{
	XE::uint64 A32 = this->low >> 32;
	XE::uint64 A00 = this->low & XE::uint64( 0xffffffff );
	XE::uint64 B32 = val.low >> 32;
	XE::uint64 B00 = val.low & XE::uint64( 0xffffffff );
	XE::uint128 Result = XE::uint128( this->high * val.low + this->low * val.high + A32 * B32, A00 * B00 );
	Result += XE::uint128( A32 * B00 ) << 32U;
	Result += XE::uint128( A00 * B32 ) << 32U;
	return Result;
}

constexpr XE::uint128 XE::uint128::operator/ ( XE::uint128 val ) noexcept
{
	if ( val > ( *this ) )
	{
		return 0;
	}
	if ( val == ( *this ) )
	{
		return 1;
	}
	XE::uint128 Denominator = val;
	XE::uint128 Quotient = 0;
	const unsigned int shift = this->clz() - val.clz();
	Denominator <<= shift;
	for ( unsigned int I = 0; I <= shift; ++I )
	{
		Quotient <<= 1U;
		if ( ( *this ) >= Denominator )
		{
			( *this ) -= Denominator;
			Quotient |= 1U;
		}
		Denominator >>= 1U;
	}
	return Quotient;
}

constexpr XE::uint128 XE::uint128::operator% ( XE::uint128 val ) noexcept
{
	if ( val > ( *this ) )
	{
		return ( *this );
	}
	if ( val == ( *this ) )
	{
		return 0;
	}
	XE::uint128 Denominator = val;
	XE::uint128 Quotient = 0;
	const unsigned int shift = this->clz() - val.clz();
	Denominator <<= shift;
	for ( unsigned int I = 0; I <= shift; ++I )
	{
		Quotient <<= 1U;
		if ( ( *this ) >= Denominator )
		{
			( *this ) -= Denominator;
			Quotient |= 1U;
		}
		Denominator >>= 1U;
	}
	return Denominator;
}

constexpr XE::uint128 XE::uint128::operator& ( XE::uint128 val ) noexcept
{
	return XE::uint128( this->high & val.high, this->low & val.low );
}

constexpr XE::uint128 XE::uint128::operator| ( XE::uint128 val ) noexcept
{
	return XE::uint128( this->high | val.high, this->low | val.low );
}

constexpr XE::uint128 XE::uint128::operator^ ( XE::uint128 val ) noexcept
{
	return XE::uint128( this->high ^ val.high, this->low ^ val.low );
}

constexpr XE::uint128 XE::uint128::operator<<( XE::uint32 val ) noexcept
{
	if ( val < 64 )
	{
		if ( val != 0 )
		{
			return XE::uint128( ( this->high << val ) | ( this->low >> ( 64 - val ) ), this->low << val );
		}
		return ( *this );
	}
	return XE::uint128( this->low << ( val - 64 ), 0 );
}

constexpr XE::uint128 XE::uint128::operator>>( XE::uint32 val ) noexcept
{
	if ( val < 64 )
	{
		if ( val != 0 )
		{
			return XE::uint128( ( this->high >> val ), this->low >> val | ( this->low << ( 64 - val ) ) );
		}
		return ( *this );
	}
	return XE::uint128( 0, this->high >> ( val - 64 ) );
}

constexpr XE::uint128 & XE::uint128::operator+=( XE::uint128 val ) noexcept
{
	return *this = *this + val;
}

constexpr XE::uint128 & XE::uint128::operator-=( XE::uint128 val ) noexcept
{
	return *this = *this - val;
}

constexpr XE::uint128 & XE::uint128::operator*=( XE::uint128 val ) noexcept
{
	return *this = *this * val;
}

constexpr XE::uint128 & XE::uint128::operator/=( XE::uint128 val ) noexcept
{
	return *this = *this / val;
}

constexpr XE::uint128 & XE::uint128::operator%=( XE::uint128 val ) noexcept
{
	return *this = *this % val;
}

constexpr XE::uint128 & XE::uint128::operator&=( XE::uint128 val ) noexcept
{
	return *this = *this & val;
}

constexpr XE::uint128 & XE::uint128::operator|=( XE::uint128 val ) noexcept
{
	return *this = *this | val;
}

constexpr XE::uint128 & XE::uint128::operator^=( XE::uint128 val ) noexcept
{
	return *this = *this ^ val;
}

constexpr XE::uint128 & XE::uint128::operator<<=( XE::uint32 val ) noexcept
{
	return *this = *this << val;
}

constexpr XE::uint128 & XE::uint128::operator>>=( XE::uint32 val ) noexcept
{
	return *this = *this >> val;
}

constexpr XE::uint32 XE::uint128::clz() const noexcept
{
	if ( high )
	{
#if PLATFORM_OS == OS_WINDOWS
		return __lzcnt64( high );
#else
		return __builtin_clzll( high );
#endif // PLATFORM_OS == OS_WINDOWS
	}
	if ( low )
	{
#if PLATFORM_OS == OS_WINDOWS
		return __lzcnt64( low ) + 64;
#else
		return __builtin_clzll( low ) + 64;
#endif // PLATFORM_OS == OS_WINDOWS
	}
	return 128;
}
