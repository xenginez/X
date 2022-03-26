/*!
 * \file   Line.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/13
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __LINE_H__525C993C_1BCC_44B9_8AEF_0BE8FDF0474A
#define __LINE_H__525C993C_1BCC_44B9_8AEF_0BE8FDF0474A

#include "Ray.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Line
{
public:
	union
	{
		struct  
		{
			Vec3f start;
			Vec3f end;
		};

		Vec3f point[2];
	};

public:
	Line();

	Line( const Line & val );

	Line( const Ray& ray, XE::float32 distance );

	Line( const Vec3f& start, const Vec3f& end );

	~Line();

public:
	Line& operator=( const Line& val );

public:
	bool operator ==( const Line& val ) const;

	bool operator !=( const Line& val ) const;

public:
	Ray GetRay() const;

	XE::float32 GetDistance() const;

	Vec3f GetDirection() const;

public:
	bool Overlaps( const XE::Line & val ) const;

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

};
DECL_XE_CLASS( Line );

END_XE_NAMESPACE

#endif // __LINE_H__525C993C_1BCC_44B9_8AEF_0BE8FDF0474A
