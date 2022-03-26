/*!
 * \file	Capsule.h
 *
 * \author	ZhengYuanQing
 * \date	2019/09/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CAPSULE_H__F02E074A_E970_41B4_B55A_01D844A36129
#define CAPSULE_H__F02E074A_E970_41B4_B55A_01D844A36129

#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Capsule
{
public:
	XE::float32 radius;
	XE::float32 height;

public:
	Capsule();

	Capsule( const Capsule & val );

	Capsule( XE::float32 radius, XE::float32 height );

public:
	Capsule & operator=( const Capsule & val );

	bool operator ==( const Capsule & val ) const;

	bool operator !=( const Capsule & val ) const;

public:
	bool Overlaps( const XE::Capsule & val ) const;

public:
	bool Intersect( const XE::OBB & val ) const;

	bool Intersect( const XE::AABB & val ) const;

	bool Intersect( const XE::Line & val ) const;

	bool Intersect( const XE::Plane & val ) const;

	bool Intersect( const XE::Sphere & val ) const;

	bool Intersect( const XE::Capsule & val ) const;

	bool Intersect( const XE::Frustum & val ) const;

	bool Intersect( const XE::Triangle & val ) const;

	bool Intersect( const XE::ConvexHull & val ) const;

	XE::Pair< bool, XE::float32 > Intersect( const XE::Ray & val ) const;

public:
	bool Contains( const XE::Vec3f & val ) const;

	bool Contains( const XE::OBB & val ) const;

	bool Contains( const XE::AABB & val ) const;

	bool Contains( const XE::Line & val ) const;

	bool Contains( const XE::Plane & val ) const;

	bool Contains( const XE::Sphere & val ) const;

	bool Contains( const XE::Capsule & val ) const;

	bool Contains( const XE::Frustum & val ) const;

	bool Contains( const XE::Triangle & val ) const;

	bool Contains( const XE::ConvexHull & val ) const;

};
DECL_XE_CLASS( Capsule );

END_XE_NAMESPACE

#endif // CAPSULE_H__F02E074A_E970_41B4_B55A_01D844A36129
