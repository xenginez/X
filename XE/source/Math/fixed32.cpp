#include "fixed32.h"

IMPLEMENT_META( XE::fixed32 );

#define PREC 16
#define COMP 65536.0f

const XE::fixed32 XE::fixed32::Min = []()
{
	XE::fixed32 fixed;
	fixed.value = static_cast< XE::int32 >( -1 << 31 );
	return fixed;
}( );


const XE::fixed32 XE::fixed32::Max = []()
{
	XE::fixed32 fixed;
	fixed.value = static_cast< XE::int32 >( 0x7FFFFFFF );
	return fixed;
}( );

XE::fixed32::fixed32()
	: value( 0 )
{

}

XE::fixed32::fixed32( XE::int32 val )
	: value( val << PREC )
{

}

XE::fixed32::fixed32( XE::float32 val )
	: value( val * COMP )
{

}

XE::fixed32::fixed32( const fixed32 & val )
	: value( val.value )
{

}

XE::fixed32 & XE::fixed32::operator=( XE::int32 val )
{
	value = val << PREC;

	return *this;
}

XE::fixed32 & XE::fixed32::operator=( XE::float32 val )
{
	value = val * COMP;

	return *this;
}

XE::fixed32 & XE::fixed32::operator=( const fixed32 & val )
{
	value = val.value;

	return *this;
}

XE::fixed32 & XE::fixed32::operator+=( const fixed32 & val )
{
	value += val.value;

	return *this;
}

XE::fixed32 & XE::fixed32::operator-=( const fixed32 & val )
{
	value -= val.value;

	return *this;
}

XE::fixed32 & XE::fixed32::operator*=( const fixed32 & val )
{
	XE::int64 _value = value;

	value = static_cast< XE::int32 >( ( _value * val.value ) >> PREC );

	return *this;
}

XE::fixed32 & XE::fixed32::operator/=( const fixed32 & val )
{
	XE::int64 _value = value;

	value = static_cast< XE::int32 >( ( _value << PREC ) / val.value );

	return *this;
}

XE::fixed32::operator XE::int32() const
{
	return XE::int32( value >> PREC );
}

XE::fixed32::operator XE::float32() const
{
	return XE::float32( value / COMP );
}


bool operator<( const XE::fixed32 & left, const XE::fixed32 & right )
{
	return left.value < right.value;
}

bool operator>( const XE::fixed32 & left, const XE::fixed32 & right )
{
	return left.value > right.value;
}

bool operator<=( const XE::fixed32 & left, const XE::fixed32 & right )
{
	return left.value <= right.value;
}

bool operator>=( const XE::fixed32 & left, const XE::fixed32 & right )
{
	return left.value >= right.value;
}

bool operator==( const XE::fixed32 & left, const XE::fixed32 & right )
{
	return left.value == right.value;
}

bool operator!=( const XE::fixed32 & left, const XE::fixed32 & right )
{
	return left.value != right.value;
}
