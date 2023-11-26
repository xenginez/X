/*!
 * \file	Bitmap.h
 *
 * \author	ZhengYuanQing
 * \date	2021/03/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef BITMAP_H__A00C8A25_4E51_4160_A93F_859031F92B48
#define BITMAP_H__A00C8A25_4E51_4160_A93F_859031F92B48

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Bitmap
{
	static constexpr ptrdiff_t _Bitsperword = CHAR_BIT;

public:
	static constexpr size_t npos = std::numeric_limits< size_t >::max();

public:
	Bitmap() = default;

	Bitmap( XE::uint64 count, bool val = false );

	Bitmap( Bitmap && val );

	Bitmap( const Bitmap & val );

	~Bitmap() = default;

public:
	Bitmap & operator =( Bitmap && val );

	Bitmap & operator =( const Bitmap & val );

public:
	Bitmap & operator<<=( XE::uint64 val );

	Bitmap & operator>>=( XE::uint64 val );

	Bitmap & operator&=( const Bitmap & val );

	Bitmap & operator|=( const Bitmap & val );

	Bitmap & operator^=( const Bitmap & val );

public:
	Bitmap operator~() const;

	Bitmap operator<<( XE::uint64 val ) const;

	Bitmap operator>>( XE::uint64 val ) const;

	bool operator==( const Bitmap & val ) const;

	bool operator!=( const Bitmap & val ) const;

public:
	void Swap( Bitmap & val )
	{
		std::swap( _Count, val._Count );
		std::swap( _Data, val._Data );
	}

public:
	bool All() const;

	bool Any() const;

	bool None() const;

public:
	XE::uint64 Count() const;

	const XE::Array< XE::uint8 > & Data() const;

public:
	bool Test( XE::uint64 index ) const;

public:
	Bitmap & Set( bool val );

	Bitmap & Set( XE::uint64 index, bool val );

	Bitmap & Set( XE::uint64 index, XE::uint64 count, bool val );

	Bitmap & Flip();

	Bitmap & Flip( XE::uint64 index );

	Bitmap & Flip( XE::uint64 index, XE::uint64 count );

public:
	Bitmap & Reset( XE::uint64 count, bool val = false );

public:
	XE::uint64 TrueCount() const;

	XE::uint64 FalseCount() const;

	XE::uint64 FindFirstTrue() const;

	XE::uint64 FindFirstFalse() const;

	XE::uint64 FindLastTrue() const;

	XE::uint64 FindLastFalse() const;

	XE::uint64 FindSeriesTrue( XE::uint64 count ) const;

	XE::uint64 FindSeriesFalse( XE::uint64 count ) const;

private:
	XE::uint64 _Count = 0;
	XE::Array< XE::uint8 > _Data;
};

XE::Bitmap operator&( const XE::Bitmap & Left, const XE::Bitmap & Right );

XE::Bitmap operator|( const XE::Bitmap & Left, const XE::Bitmap & Right );

XE::Bitmap operator^( const XE::Bitmap & Left, const XE::Bitmap & Right );

END_XE_NAMESPACE

namespace std
{
	XE_INLINE void swap( XE::Bitmap & left, XE::Bitmap & right )
	{
		left.Swap( right );
	}
}

#endif // BITMAP_H__A00C8A25_4E51_4160_A93F_859031F92B48
