/*!
 * \file   Ray.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __RAY_H__5874E305_1881_4FDF_B7EE_44A07BD36195
#define __RAY_H__5874E305_1881_4FDF_B7EE_44A07BD36195

#include "Mat.h"
#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) Ray
{
public:
	Vec3f origin;
	Vec3f direction;

public:
	Ray();

	Ray( const Vec3f& origin, const Vec3f& direction );

public:
	Ray& operator=( const Ray & val );

	bool operator ==( const Ray& val ) const;

	bool operator !=( const Ray& val ) const;

public:
	Vec3f GetPoint( XE::float32 val ) const;

public:
	std::pair< bool, XE::float32 > Intersect( const XE::OBB& val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::AABB& val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Line& val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Plane& val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Sphere& val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Capsule & val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Frustum& val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Triangle & val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::ConvexHull & val ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Ray& ray, bool discardInside = true ) const;

	std::pair< bool, XE::float32 > Intersect( const XE::Vec3f& a, const XE::Vec3f& b, const XE::Vec3f& c, const XE::Vec3f& normal, bool positiveSide = true, bool negativeSide = true ) const;

public:
	void Transform( const Mat4x4f& val );

	void TransformAffine( const Mat4x4f& val );

};
DECL_XE_CLASS( Ray );

END_XE_NAMESPACE

#endif // __RAY_H__5874E305_1881_4FDF_B7EE_44A07BD36195
