/*!
 * \file   Frustum.h
 *
 * \author ZhengYuanQing
 * \date   2019/02/28
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __FRUSTUM_H__A9A7B11B_8DB7_4F63_BD17_1B7487E1141A
#define __FRUSTUM_H__A9A7B11B_8DB7_4F63_BD17_1B7487E1141A

#include "Type.h"
#include "Plane.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Frustum
{
public:
	union
	{
		struct  
		{
			Plane top;
			Plane bottom;
			Plane left;
			Plane right;
			Plane near;
			Plane far;
		};
		Plane plane[6];
	};

public:
	Frustum();

	Frustum( const Frustum& val );

	~Frustum();

public:
	Frustum & operator=( const Frustum & val );

public:
	bool Overlaps( const XE::Frustum & val ) const;

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
DECL_XE_CLASS( Frustum );

END_XE_NAMESPACE

#endif // __FRUSTUM_H__A9A7B11B_8DB7_4F63_BD17_1B7487E1141A
