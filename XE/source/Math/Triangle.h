/*!
 * \file	Triangle.h
 *
 * \author	ZhengYuanQing
 * \date	2021/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TRIANGLE_H__7ABA7652_48B3_4767_A5D5_A8AF588A79B4
#define TRIANGLE_H__7ABA7652_48B3_4767_A5D5_A8AF588A79B4

#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Triangle
{

public:
	union
	{
		struct
		{
			XE::Vec3f p1;
			XE::Vec3f p2;
			XE::Vec3f p3;
		};

		XE::Vec3f point[3];
	};

public:
	Triangle();

	Triangle( const XE::Triangle & val );

	Triangle( const XE::Vec3f & p1, const XE::Vec3f & p2, const XE::Vec3f & p3 );

	~Triangle();

public:
	Triangle & operator=( const XE::Triangle & val );

public:
	bool operator ==( const XE::Triangle & val ) const;

	bool operator !=( const XE::Triangle & val ) const;

public:
	bool Overlaps( const XE::Triangle & val ) const;

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
DECL_XE_CLASS( Triangle );

END_XE_NAMESPACE

#endif // TRIANGLE_H__7ABA7652_48B3_4767_A5D5_A8AF588A79B4
