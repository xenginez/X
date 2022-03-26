/*!
 * \file   Plane.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __PLANE_H__56D96450_7C65_4A4A_ABC2_AF8D392BB432
#define __PLANE_H__56D96450_7C65_4A4A_ABC2_AF8D392BB432

#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Plane
{
public:
	Vec3f normal;
	XE::float32 distance;

public:
	Plane();

	Plane( const Plane& val );

	Plane( const Vec3f& normal, XE::float32 d );

	Plane( XE::float32 a, XE::float32 b, XE::float32 c, XE::float32 d );

	Plane( const Vec3f& normal, const Vec3f& point );

	Plane( const Vec3f& point0, const Vec3f& point1, const Vec3f& point2 );

public:
	Plane& operator=( const Plane& val );

	bool operator ==( const Plane& val ) const;

	bool operator !=( const Plane& val ) const;

public:
	SideType GetSide( const Vec3f& val ) const;

	SideType GetSide( const AABB& val ) const;

	SideType GetSide( const Sphere& val ) const;

public:
	XE::float32 Normalize();

	XE::float32 GetDistance( const Vec3f& val ) const;

public:
	Vec3f Project( const Vec3f& val ) const;

public:
	bool Overlaps( const XE::Plane & val ) const;

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
DECL_XE_CLASS( Plane );

END_XE_NAMESPACE

#endif // __PLANE_H__56D96450_7C65_4A4A_ABC2_AF8D392BB432
