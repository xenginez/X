#include "Frustum.h"

BEG_META( XE::Frustum )
REG_PROPERTY( "top", &Frustum::top );
REG_PROPERTY( "bottom", &Frustum::bottom );
REG_PROPERTY( "left", &Frustum::left );
REG_PROPERTY( "right", &Frustum::right );
REG_PROPERTY( "near", &Frustum::near );
REG_PROPERTY( "far", &Frustum::far );
END_META()

XE::Frustum::Frustum()
{

}

XE::Frustum::Frustum( const Frustum & val )
	:top( val.top ), bottom( val.bottom ), left( val.left ), right( val.right ), far( val.far ), near( val.near )
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
