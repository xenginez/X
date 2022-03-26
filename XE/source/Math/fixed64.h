/*!
 * \file	fixed64.h
 *
 * \author	ZhengYuanQing
 * \date	2021/09/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FIXED64_H__5657685E_95B2_49E6_96CE_1D29AFB3E033
#define FIXED64_H__5657685E_95B2_49E6_96CE_1D29AFB3E033

#include "fixed32.h"

BEG_XE_NAMESPACE

class XE_API fixed64
{
public:
	static const XE::fixed64 Min;
	static const XE::fixed64 Max;

public:
	XE::int64 value;

public:
	fixed64();

	fixed64( XE::int64 val );

	fixed64( XE::float64 val );

	fixed64( const fixed32 & val );

	fixed64( const fixed64 & val );

public:
	fixed64 & operator= ( XE::int64 val );

	fixed64 & operator= ( XE::float64 val );

	fixed64 & operator= ( const fixed32 & val );

	fixed64 & operator= ( const fixed64 & val );

public:
	fixed64 & operator+= ( const fixed64 & val );

	fixed64 & operator-= ( const fixed64 & val );

	fixed64 & operator*= ( const fixed64 & val );

	fixed64 & operator/= ( const fixed64 & val );

public:
	operator XE::int64() const;

	operator XE::float64() const;

};
DECL_XE_CLASS( fixed64 );

END_XE_NAMESPACE

bool operator < ( const XE::fixed64 & left, const XE::fixed64 & right );

bool operator > ( const XE::fixed64 & left, const XE::fixed64 & right );

bool operator <= ( const XE::fixed64 & left, const XE::fixed64 & right );

bool operator >= ( const XE::fixed64 & left, const XE::fixed64 & right );

bool operator == ( const XE::fixed64 & left, const XE::fixed64 & right );

bool operator != ( const XE::fixed64 & left, const XE::fixed64 & right );

#endif // FIXED64_H__5657685E_95B2_49E6_96CE_1D29AFB3E033
