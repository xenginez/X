#include "fixed64.h"

IMPLEMENT_META( XE::fixed64 );

#define PREC 32
#define COMP 4294967296.0

const XE::fixed64 XE::fixed64::Min = []()
{
	XE::fixed64 fixed;
	fixed.value = static_cast< XE::int64 >( -1 ) << 63;
	return fixed;
}( );


const XE::fixed64 XE::fixed64::Max = []()
{
	XE::fixed64 fixed;
	fixed.value = static_cast< XE::int64 >( 0x7FFFFFFFFFFFFFFF );
	return fixed;
}( );

XE::fixed64::fixed64()
	: value( 0 )
{

}

XE::fixed64::fixed64( XE::int64 val )
	: value( val << PREC )
{

}

XE::fixed64::fixed64( XE::float64 val )
	: value( val * COMP )
{

}

XE::fixed64::fixed64( const fixed32 & val )
	: value( XE::int64( XE::int32( val ) ) << PREC )
{

}

XE::fixed64::fixed64( const fixed64 & val )
	: value( val.value )
{

}

XE::fixed64 & XE::fixed64::operator=( XE::int64 val )
{
	value = val << PREC;

	return *this;
}

XE::fixed64 & XE::fixed64::operator=( XE::float64 val )
{
	value = val * COMP;

	return *this;
}

XE::fixed64 & XE::fixed64::operator=( const fixed32 & val )
{
	value = static_cast< XE::int64 >( XE::int32( val ) ) << PREC;

	return *this;
}

XE::fixed64 & XE::fixed64::operator=( const fixed64 & val )
{
	value = val.value;

	return *this;
}

XE::fixed64 & XE::fixed64::operator+=( const fixed64 & val )
{
	value += val.value;

	return *this;
}

XE::fixed64 & XE::fixed64::operator-=( const fixed64 & val )
{
	value -= val.value;

	return *this;
}

XE::fixed64 & XE::fixed64::operator*=( const fixed64 & val )
{
	XE::float64 _value = value / COMP;

	_value *= XE::float64( val );

	*this = _value;

	return *this;
}

XE::fixed64 & XE::fixed64::operator/=( const fixed64 & val )
{
	XE::float64 _value = value / COMP;

	_value /= XE::float64( val );

	*this = _value;

	return *this;
}

XE::fixed64::operator XE::int64() const
{
	return XE::int64( value >> PREC );
}

XE::fixed64::operator XE::float64() const
{
	return XE::float64( value / COMP );
}


bool operator<( const XE::fixed64 & left, const XE::fixed64 & right )
{
	return left.value < right.value;
}

bool operator>( const XE::fixed64 & left, const XE::fixed64 & right )
{
	return left.value > right.value;
}

bool operator<=( const XE::fixed64 & left, const XE::fixed64 & right )
{
	return left.value <= right.value;
}

bool operator>=( const XE::fixed64 & left, const XE::fixed64 & right )
{
	return left.value >= right.value;
}

bool operator==( const XE::fixed64 & left, const XE::fixed64 & right )
{
	return left.value == right.value;
}

bool operator!=( const XE::fixed64 & left, const XE::fixed64 & right )
{
	return left.value != right.value;
}
