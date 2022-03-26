/*!
 * \file   Sphere.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __SPHERE_H__06E9A06D_DAE8_4648_8E21_51748C5A3BC7
#define __SPHERE_H__06E9A06D_DAE8_4648_8E21_51748C5A3BC7

#include "Mat.h"
#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Sphere
{
public:
	static const Sphere Zero;

public:
	Vec3f center;
	XE::float32 radius;

public:
	Sphere();

	Sphere( const Sphere& val );

	Sphere( const Vec3f&center, XE::float32 radius );

public:
	Sphere& operator=( const Sphere & val );

	bool operator ==( const Sphere& val ) const;

	bool operator !=( const Sphere& val ) const;

public:
	bool Overlaps( const XE::Sphere & val ) const;

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

public:
	void Merge( const Vec3f& val );

	void Merge( const Sphere& val );

public:
	void Transform( const Mat4x4f& val );
};
DECL_XE_CLASS( Sphere );

END_XE_NAMESPACE

#endif // __SPHERE_H__06E9A06D_DAE8_4648_8E21_51748C5A3BC7
