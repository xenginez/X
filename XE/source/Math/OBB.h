/*!
 * \file   OBB.h
 *
 * \author ZhengYuanQing
 * \date   2019/02/28
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __OBB_H__C092424D_239B_4C6E_857D_2CEC52810C85
#define __OBB_H__C092424D_239B_4C6E_857D_2CEC52810C85

#include "Vec3.h"
#include "Quat.h"

BEG_XE_NAMESPACE

class XE_API alignas( 16 ) OBB
{
public:
	Vec3f center;
	Vec3f extent;
	Quat rotation;

public:
	OBB();

	OBB( const OBB& val );

	OBB( const AABB& box, const Quat& orient );

	OBB( const Vec3f& center, const Vec3f& extent, const Quat& orient );

	~OBB();

public:
	OBB& operator =( const OBB& val );

public:
	bool Overlaps( const XE::OBB & val ) const;

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
DECL_XE_CLASS( OBB );

END_XE_NAMESPACE

#endif // __OBB_H__C092424D_239B_4C6E_857D_2CEC52810C85
