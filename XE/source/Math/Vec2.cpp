#include "Vec2.h"

BEG_META( XE::Vec2i )
REG_PROPERTY( "x", &Vec2i::x );
REG_PROPERTY( "y", &Vec2i::y );
END_META()

BEG_META( XE::Vec2f )
REG_PROPERTY( "x", &Vec2f::x );
REG_PROPERTY( "y", &Vec2f::y );
END_META()

BEG_META( XE::Vec2d )
REG_PROPERTY( "x", &Vec2d::x );
REG_PROPERTY( "y", &Vec2d::y );
END_META()
