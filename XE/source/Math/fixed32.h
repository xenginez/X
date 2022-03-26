/*!
 * \file	fixed32.h
 *
 * \author	ZhengYuanQing
 * \date	2021/09/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FIXED32_H__3CBB27B9_631D_4168_8812_77E016F24779
#define FIXED32_H__3CBB27B9_631D_4168_8812_77E016F24779

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API fixed32
{
public:
	static const XE::fixed32 Min;
	static const XE::fixed32 Max;

public:
	XE::int32 value;

public:
	fixed32();

	fixed32( XE::int32 val );

	fixed32( XE::float32 val );

	fixed32( const fixed32 & val );

public:
	fixed32 & operator= ( XE::int32 val );

	fixed32 & operator= ( XE::float32 val );

	fixed32 & operator= ( const fixed32 & val );

public:
	fixed32 & operator+= ( const fixed32 & val );

	fixed32 & operator-= ( const fixed32 & val );

	fixed32 & operator*= ( const fixed32 & val );

	fixed32 & operator/= ( const fixed32 & val );

public:
	operator XE::int32() const;

	operator XE::float32() const;
};
DECL_XE_CLASS( fixed32 );

END_XE_NAMESPACE

bool operator < ( const XE::fixed32 & left, const XE::fixed32 & right );

bool operator > ( const XE::fixed32 & left, const XE::fixed32 & right );

bool operator <= ( const XE::fixed32 & left, const XE::fixed32 & right );

bool operator >= ( const XE::fixed32 & left, const XE::fixed32 & right );

bool operator == ( const XE::fixed32 & left, const XE::fixed32 & right );

bool operator != ( const XE::fixed32 & left, const XE::fixed32 & right );

#endif // FIXED32_H__3CBB27B9_631D_4168_8812_77E016F24779
