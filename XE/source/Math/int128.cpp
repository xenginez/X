#include "int128.h"
#include "uint128.h"

constexpr XE::int128::int128( XE::int8 val ) noexcept
    : low( static_cast<XE::uint64>( val ) ), high( val < 0 ? XE::int64( -1 ) : XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::int16 val ) noexcept
    : low( static_cast<XE::uint64>( val ) ), high( val < 0 ? XE::int64( -1 ) : XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::int32 val ) noexcept
    : low( static_cast<XE::uint64>( val ) ), high( val < 0 ? XE::int64( -1 ) : XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::int64 val ) noexcept
    : low( static_cast<XE::uint64>( val ) ), high( val < 0 ? XE::int64( -1 ) : XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::uint8 val ) noexcept
    : low( val ), high( XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::uint16 val ) noexcept
    : low( val ), high( XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::uint32 val ) noexcept
    : low( val ), high( XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( XE::uint64 val ) noexcept
    : low( val ), high( XE::int64( 0 ) )
{
}

constexpr XE::int128::int128( const XE::uint128 & val ) noexcept
    : low( val.low ), high( static_cast<XE::int64>( val.high ) )
{
}

constexpr XE::int128::int128( XE::int64 high, XE::uint64 low ) noexcept
    : low( low ), high( high )
{
}

constexpr XE::int128 & XE::int128::operator=( XE::int8 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::int16 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::int32 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::int64 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::uint8 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::uint16 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::uint32 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( XE::uint64 val ) noexcept
{
    return *this = XE::int128( val );
}

constexpr XE::int128 & XE::int128::operator=( const XE::uint128 & val ) noexcept
{
    return *this = XE::int128( val );
}
