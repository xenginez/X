#include "Vec3.h"

BEG_META( XE::Vec3i )
REG_PROPERTY( "x", &Vec3i::x );
REG_PROPERTY( "y", &Vec3i::y );
REG_PROPERTY( "z", &Vec3i::z );
END_META()

BEG_META( XE::Vec3f )
REG_PROPERTY( "x", &Vec3f::x );
REG_PROPERTY( "y", &Vec3f::y );
REG_PROPERTY( "z", &Vec3f::z );
END_META()

BEG_META( XE::Vec3d )
REG_PROPERTY( "x", &Vec3d::x );
REG_PROPERTY( "y", &Vec3d::y );
REG_PROPERTY( "z", &Vec3d::z );
END_META()
