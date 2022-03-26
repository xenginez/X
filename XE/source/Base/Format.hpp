/*!
 * \file	Format.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/11/07
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FORMAT_HPP__6B939898_70E8_4C71_AA12_A456F9F639CA
#define FORMAT_HPP__6B939898_70E8_4C71_AA12_A456F9F639CA

#include "String.h"

BEG_XE_NAMESPACE

XE_INLINE XE::String Format( XE::uint64 index, const XE::String & fmt )
{
	return fmt;
};

template< typename T > XE::String Format( XE::uint64 index, const XE::String & fmt, T && arg )
{
	return std::regex_replace( fmt.std_str(), std::regex( R"(\{\s*%)" + XE::ToString( index ).std_str() + R"(\s*\})" ), XE::ToString( arg ).std_str() );
};

template< typename T, typename ... Types > XE::String Format( XE::uint64 index, const XE::String & fmt, T && arg, Types &&... args )
{
	return Format( index + 1, Format( index, fmt, arg ), std::forward< Types >( args )... );
};


XE_INLINE XE::String Format( const XE::String & fmt )
{
	return fmt;
}

template< typename ... Types > XE::String Format( const XE::String & fmt, Types &&... args )
{
	return XE::Format( XE::uint64( 0 ), fmt, std::forward< Types >( args )... );
}

END_XE_NAMESPACE

#endif // FORMAT_HPP__6B939898_70E8_4C71_AA12_A456F9F639CA
