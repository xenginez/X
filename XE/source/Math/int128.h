/*!
* \file   int128.h
*
* \author ZhengYuanQing
* \date   2022/05/22
* \email  zhengyuanqing.95@gmail.com
*
*/
#ifndef INT128_H__D5BCE39E_2B95_435C_A835_BF510C741AA4
#define INT128_H__D5BCE39E_2B95_435C_A835_BF510C741AA4

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API int128
{
public:
    XE::uint64 low = 0;
    XE::int64 high = 0;

public:
    constexpr int128() noexcept = default;
    constexpr int128( const int128 & ) noexcept = default;
    constexpr int128( int128 && ) noexcept = default;
    constexpr int128 & operator=( const int128 & V ) noexcept = default;
    constexpr int128 & operator=( int128 && V ) noexcept = default;

    constexpr int128( XE::int8 val ) noexcept;
    constexpr int128( XE::int16 val ) noexcept;
    constexpr int128( XE::int32 val ) noexcept;
    constexpr int128( XE::int64 val ) noexcept;
    constexpr int128( XE::uint8 val ) noexcept;
    constexpr int128( XE::uint16 val ) noexcept;
    constexpr int128( XE::uint32 val ) noexcept;
    constexpr int128( XE::uint64 val ) noexcept;
    constexpr int128( const XE::uint128 & val ) noexcept;
    constexpr int128( XE::int64 high, XE::uint64 low ) noexcept;

    constexpr int128 & operator=( XE::int8 val ) noexcept;
    constexpr int128 & operator=( XE::int16 val ) noexcept;
    constexpr int128 & operator=( XE::int32 val ) noexcept;
    constexpr int128 & operator=( XE::int64 val ) noexcept;
    constexpr int128 & operator=( XE::uint8 val ) noexcept;
    constexpr int128 & operator=( XE::uint16 val ) noexcept;
    constexpr int128 & operator=( XE::uint32 val ) noexcept;
    constexpr int128 & operator=( XE::uint64 val ) noexcept;
    constexpr int128 & operator=( const XE::uint128 & val ) noexcept;
};

END_XE_NAMESPACE

namespace std
{
    template <> class numeric_limits< XE::int128 >
    {
    public:
        static constexpr bool is_specialized = true;
        static constexpr bool is_signed = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact = true;
        static constexpr bool has_infinity = false;
        static constexpr bool has_quiet_NaN = false;
        static constexpr bool has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm = denorm_absent;
        static constexpr bool has_denorm_loss = false;
        static constexpr float_round_style round_style = round_toward_zero;
        static constexpr bool is_iec559 = false;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = false;
        static constexpr int digits = 127;
        static constexpr int digits10 = 38;
        static constexpr int max_digits10 = 0;
        static constexpr int radix = 2;
        static constexpr int min_exponent = 0;
        static constexpr int min_exponent10 = 0;
        static constexpr int max_exponent = 0;
        static constexpr int max_exponent10 = 0;
        static constexpr bool traps = std::numeric_limits< XE::uint64 >::traps;
        static constexpr bool tinyness_before = false;

        static constexpr XE::int128 min() { return XE::int128( std::numeric_limits< XE::int64 >::min(), 0 ); }
        static constexpr XE::int128 lowest() { return XE::int128( std::numeric_limits< XE::int64 >::min(), 0 ); }
        static constexpr XE::int128 max() { return XE::int128( std::numeric_limits< XE::int64 >::max(), std::numeric_limits< XE::uint64 >::max() ); }
        static constexpr XE::int128 epsilon() { return 0; }
        static constexpr XE::int128 round_error() { return 0; }
        static constexpr XE::int128 infinity() { return 0; }
        static constexpr XE::int128 quiet_NaN() { return 0; }
        static constexpr XE::int128 signaling_NaN() { return 0; }
        static constexpr XE::int128 denorm_min() { return 0; }
    };
}

#endif//INT128_H__D5BCE39E_2B95_435C_A835_BF510C741AA4
