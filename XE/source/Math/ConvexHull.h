/*!
 * \file	ConvexHull.h
 *
 * \author	ZhengYuanQing
 * \date	2021/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONVEXHULL_H__56C6937D_F4EF_43B6_81F0_55F999943598
#define CONVEXHULL_H__56C6937D_F4EF_43B6_81F0_55F999943598

#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API ConvexHull
{
public:
	XE::Array< XE::Vec3f > points;

public:
	ConvexHull();

	ConvexHull( const ConvexHull & val );

	ConvexHull( const XE::Array< XE::Vec3f > & val );

public:
	ConvexHull & operator=( const XE::ConvexHull & val );

public:
	XE::Vec3f & operator[]( XE::uint64 val );

	const XE::Vec3f & operator[]( XE::uint64 val ) const;
	
public:
	bool operator ==( const XE::ConvexHull & val ) const;

	bool operator !=( const XE::ConvexHull & val ) const;

public:
	bool Overlaps( const XE::ConvexHull & val ) const;

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

END_XE_NAMESPACE

#endif // CONVEXHULL_H__56C6937D_F4EF_43B6_81F0_55F999943598
