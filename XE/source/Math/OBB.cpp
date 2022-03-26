#include "OBB.h"

#include "AABB.h"

BEG_META( XE::OBB );
REG_PROPERTY( "center", &OBB::center );
REG_PROPERTY( "extent", &OBB::extent );
REG_PROPERTY( "rotation", &OBB::rotation );
END_META()

XE::OBB::OBB()
{

}

XE::OBB::OBB( const OBB& val )
	:center( val.center ), rotation( val.rotation ), extent( val.extent )
{

}

XE::OBB::OBB( const AABB& box, const Quat& orient )
	: center( box.GetCenter() ), extent( box.GetSize() ), rotation( orient )
{

}

XE::OBB::OBB( const Vec3f& center, const Vec3f& extent, const Quat& orient )
	: center( center ), rotation( orient ), extent( extent )
{

}

XE::OBB::~OBB()
{

}

XE::OBB& XE::OBB::operator=( const OBB& val )
{
	center = val.center;
	extent = val.extent;
	rotation = val.rotation;
	return *this;
}
