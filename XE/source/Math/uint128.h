/*!
* \file   uint128.h
*
* \author ZhengYuanQing
* \date   2022/05/22
* \email  zhengyuanqing.95@gmail.com
*
*/
#ifndef UINT128_H__A402F01E_3D8F_4A45_AB76_7FBB77DBF138
#define UINT128_H__A402F01E_3D8F_4A45_AB76_7FBB77DBF138

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API uint128
{
public:
    XE::uint64 low = 0;
    XE::uint64 high = 0;

public:
    constexpr uint128() noexcept = default;
    constexpr uint128( const XE::uint128 & ) noexcept = default;
    constexpr uint128( XE::uint128 && ) noexcept = default;
    constexpr uint128 & operator=( const XE::uint128 & ) noexcept = default;
    constexpr uint128 & operator=( XE::uint128 && ) noexcept = default;

    constexpr uint128( XE::int8 val ) noexcept;
    constexpr uint128( XE::int16 val ) noexcept;
    constexpr uint128( XE::int32 val ) noexcept;
    constexpr uint128( XE::int64 val ) noexcept;
    constexpr uint128( XE::uint8 val ) noexcept;
    constexpr uint128( XE::uint16 val ) noexcept;
    constexpr uint128( XE::uint32 val ) noexcept;
    constexpr uint128( XE::uint64 val ) noexcept;
    constexpr uint128( const XE::int128 & val ) noexcept;
    constexpr uint128( XE::uint64 high, XE::uint64 low ) noexcept;

    constexpr operator bool() const noexcept;
    constexpr operator XE::int8() const noexcept;
    constexpr operator XE::int16() const noexcept;
    constexpr operator XE::int32() const noexcept;
    constexpr operator XE::int64() const noexcept;
    constexpr operator XE::uint8() const noexcept;
    constexpr operator XE::uint16() const noexcept;
    constexpr operator XE::uint32() const noexcept;
    constexpr operator XE::uint64() const noexcept;

    constexpr XE::uint128 & operator=( XE::int8 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::int16 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::int32 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::int64 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::uint8 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::uint16 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::uint32 val ) noexcept;
    constexpr XE::uint128 & operator=( XE::uint64 val ) noexcept;
    constexpr XE::uint128 & operator=( const XE::int128 & val ) noexcept;

    constexpr bool operator< ( XE::uint128 val ) noexcept;
    constexpr bool operator> ( XE::uint128 val ) noexcept;
    constexpr bool operator<=( XE::uint128 val ) noexcept;
    constexpr bool operator>=( XE::uint128 val ) noexcept;
    constexpr bool operator==( XE::uint128 val ) noexcept;
    constexpr bool operator!=( XE::uint128 val ) noexcept;

    constexpr XE::uint128 operator+ ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator- ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator* ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator/ ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator% ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator& ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator| ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator^ ( XE::uint128 val ) noexcept;
    constexpr XE::uint128 operator<<( XE::uint32 val ) noexcept;
    constexpr XE::uint128 operator>>( XE::uint32 val ) noexcept;

    constexpr XE::uint128 & operator+=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator-=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator*=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator/=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator%=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator&=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator|=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator^=( XE::uint128 val ) noexcept;
    constexpr XE::uint128 & operator<<=( XE::uint32 val ) noexcept;
    constexpr XE::uint128 & operator>>=( XE::uint32 val ) noexcept;

    constexpr XE::uint32 clz() const noexcept;
};

END_XE_NAMESPACE

namespace std
{
    template <> class numeric_limits<XE::uint128>
    {
    public:
        static constexpr bool is_specialized = true;
        static constexpr bool is_signed = false;
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
        static constexpr bool traps = std::numeric_limits<XE::uint64>::traps;
        static constexpr bool tinyness_before = false;

        static constexpr XE::uint128 min() { return XE::uint128( std::numeric_limits<XE::uint64>::min(), std::numeric_limits<XE::uint64>::min() ); }
        static constexpr XE::uint128 lowest() { return XE::uint128( std::numeric_limits<XE::uint64>::min(), std::numeric_limits<XE::uint64>::min() ); }
        static constexpr XE::uint128 max() { return XE::uint128( std::numeric_limits<XE::uint64>::max(), std::numeric_limits<XE::uint64>::max() ); }
        static constexpr XE::uint128 epsilon() { return 0; }
        static constexpr XE::uint128 round_error() { return 0; }
        static constexpr XE::uint128 infinity() { return 0; }
        static constexpr XE::uint128 quiet_NaN() { return 0; }
        static constexpr XE::uint128 signaling_NaN() { return 0; }
        static constexpr XE::uint128 denorm_min() { return 0; }
    };
}

#endif//UINT128_H__A402F01E_3D8F_4A45_AB76_7FBB77DBF138
