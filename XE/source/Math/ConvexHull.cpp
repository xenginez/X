#include "ConvexHull.h"

#include "Mathf.h"

XE::ConvexHull::ConvexHull()
{

}

XE::ConvexHull::ConvexHull( const XE::ConvexHull & val )
	:points( val.points )
{

}

XE::ConvexHull::ConvexHull( const XE::Array< XE::Vec3f > & val )
	:points( val )
{

}

XE::ConvexHull & XE::ConvexHull::operator=( const XE::ConvexHull & val )
{
	points = val.points;

	return *this;
}

XE::Vec3f & XE::ConvexHull::operator[]( XE::uint64 val )
{
	return points[val];
}

const XE::Vec3f & XE::ConvexHull::operator[]( XE::uint64 val ) const
{
	return points[val];
}

bool XE::ConvexHull::operator!=( const XE::ConvexHull & val ) const
{
	if( points.size() == val.points.size() )
	{
		for( XE::uint64 i = 0; i < points.size(); ++i )
		{
			if( points[i] != val[i] )
			{
				return true;
			}
		}
	}

	return false;
}

bool XE::ConvexHull::operator==( const XE::ConvexHull & val ) const
{
	if( points.size() == val.points.size() )
	{
		for( XE::uint64 i = 0; i < points.size(); ++i )
		{
			if( points[i] != val[i] )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool XE::ConvexHull::Overlaps( const XE::ConvexHull & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::OBB & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::AABB & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::Line & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::Plane & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::Sphere & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::Capsule & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::Frustum & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::Triangle & val ) const
{
	return false;
}

bool XE::ConvexHull::Intersect( const XE::ConvexHull & val ) const
{
	return false;
}

XE::Pair< bool, XE::float32 > XE::ConvexHull::Intersect( const XE::Ray & val ) const
{
	return { false, 0 };
}

bool XE::ConvexHull::Contains( const XE::Vec3f & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::OBB & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::AABB & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::Line & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::Plane & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::Sphere & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::Capsule & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::Frustum & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::Triangle & val ) const
{
	return false;
}

bool XE::ConvexHull::Contains( const XE::ConvexHull & val ) const
{
	return false;
}
