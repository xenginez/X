#include "Triangle.h"

#include "Mathf.h"

BEG_META( XE::Triangle )
REG_PROPERTY( "p1", &XE::Triangle::p1 );
REG_PROPERTY( "p2", &XE::Triangle::p2 );
REG_PROPERTY( "p3", &XE::Triangle::p3 );
END_META()

XE::Triangle::Triangle()
{

}

XE::Triangle::Triangle( const XE::Triangle & val )
	:p1( val.p1 ), p2( val.p2 ), p3( val.p3 )
{

}

XE::Triangle::Triangle( const XE::Vec3f & p1, const XE::Vec3f & p2, const XE::Vec3f & p3 )
	: p1( p1 ), p2( p2 ), p3( p3 )
{

}

XE::Triangle::~Triangle()
{

}

XE::Triangle & XE::Triangle::operator=( const XE::Triangle & val )
{
	p1 = val.p1;
	p2 = val.p2;
	p3 = val.p3;

	return *this;
}

bool XE::Triangle::operator!=( const XE::Triangle & val ) const
{
	return p1 != val.p1 || p2 != val.p2 || p3 != val.p3;
}

bool XE::Triangle::operator==( const XE::Triangle & val ) const
{
	return p1 == val.p1 && p2 == val.p2 && p3 == val.p3;
}

bool XE::Triangle::Intersect( const XE::OBB & val ) const
{
	return false;
}

bool XE::Triangle::Intersect( const XE::AABB & val ) const
{
	return false;
}

bool XE::Triangle::Intersect( const XE::Line & val ) const
{
	return false;
}

bool XE::Triangle::Intersect( const XE::Plane & val ) const
{
	return false;
}

bool XE::Triangle::Intersect( const XE::Sphere & val ) const
{
	return false;
}

bool XE::Triangle::Intersect( const XE::Frustum & val ) const
{
	return false;
}

std::pair< bool, XE::float32 > XE::Triangle::Intersect( const Ray & ray ) const
{
	return { false, 0 };
}
