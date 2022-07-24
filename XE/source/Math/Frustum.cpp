#include "Frustum.h"

#include "Mathf.h"

BEG_META( XE::Frustum )
REG_PROPERTY( "top", &Frustum::top );
REG_PROPERTY( "bottom", &Frustum::bottom );
REG_PROPERTY( "left", &Frustum::left );
REG_PROPERTY( "right", &Frustum::right );
REG_PROPERTY( "near", &Frustum::near );
REG_PROPERTY( "far", &Frustum::far );
END_META()

namespace
{
	void NormalizePlane( XE::Plane & plane )
	{
		auto magnitude = XE::Mathf::Sqrt( plane.a * plane.a +
										  plane.b * plane.b +
										  plane.c * plane.c );

		plane.a /= magnitude;
		plane.b /= magnitude;
		plane.c /= magnitude;
		plane.d /= magnitude;
	}
}

XE::Frustum::Frustum()
{

}

XE::Frustum::Frustum( const Frustum & val )
	:top( val.top ), bottom( val.bottom ), left( val.left ), right( val.right ), far( val.far ), near( val.near )
{

}

XE::Frustum::Frustum( const XE::Mat4x4f & view_proj )
{
	auto columnMajorViewProjection = XE::Mathf::Transpose( view_proj );
	float const * clip = columnMajorViewProjection.d;

	top.a = clip[3] - clip[1];
	top.b = clip[7] - clip[5];
	top.c = clip[11] - clip[9];
	top.d = clip[15] - clip[13];
	NormalizePlane( top );

	bottom.a = clip[3] + clip[1];
	bottom.b = clip[7] + clip[5];
	bottom.c = clip[11] + clip[9];
	bottom.d = clip[15] + clip[13];
	NormalizePlane( bottom );

	left.a = clip[3] + clip[0];
	left.b = clip[7] + clip[4];
	left.c = clip[11] + clip[8];
	left.d = clip[15] + clip[12];
	NormalizePlane( left );

	right.a = clip[3] - clip[0];
	right.b = clip[7] - clip[4];
	right.c = clip[11] - clip[8];
	right.d = clip[15] - clip[12];
	NormalizePlane( right );

	near.a = clip[3] + clip[2];
	near.b = clip[7] + clip[6];
	near.c = clip[11] + clip[10];
	near.d = clip[15] + clip[14];
	NormalizePlane( near );

	far.a = clip[3] - clip[2];
	far.b = clip[7] - clip[6];
	far.c = clip[11] - clip[10];
	far.d = clip[15] - clip[14];
	NormalizePlane( far );
}

XE::Frustum::Frustum( const Plane & top, const Plane & bottom, const Plane & left, const Plane & right, const Plane & near, const Plane & far )
	:top( top ), bottom( bottom ), left( left ), right( right ), far( far ), near( near )
{

}

XE::Frustum::~Frustum()
{

}

XE::Frustum & XE::Frustum::operator=( const Frustum & val )
{
	top = val.top;
	bottom = val.bottom;
	left = val.left;
	right = val.right;
	far = val.far;
	near = val.near;

	return *this;
}

bool XE::Frustum::Intersect( const OBB & val ) const
{
	return false;
}

bool XE::Frustum::Intersect( const AABB & val ) const
{
	return false;
}

bool XE::Frustum::Intersect( const Line & val ) const
{
	return false;
}

bool XE::Frustum::Intersect( const Plane & val ) const
{
	return false;
}

bool XE::Frustum::Intersect( const Sphere & val ) const
{
	return false;
}

bool XE::Frustum::Intersect( const Frustum & val ) const
{
	return false;
}

std::pair< bool, XE::float32 > XE::Frustum::Intersect( const Ray & ray ) const
{
	return { false, 0.0f };
}
