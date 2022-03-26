#include "Vec4.h"

BEG_META( XE::Vec4i )
REG_PROPERTY( "x", &Vec4i::x );
REG_PROPERTY( "y", &Vec4i::y );
REG_PROPERTY( "z", &Vec4i::z );
REG_PROPERTY( "w", &Vec4i::w );
END_META()

BEG_META( XE::Vec4f )
REG_PROPERTY( "x", &Vec4f::x );
REG_PROPERTY( "y", &Vec4f::y );
REG_PROPERTY( "z", &Vec4f::z );
REG_PROPERTY( "w", &Vec4f::w );
END_META()

BEG_META( XE::Vec4d )
REG_PROPERTY( "x", &Vec4d::x );
REG_PROPERTY( "y", &Vec4d::y );
REG_PROPERTY( "z", &Vec4d::z );
REG_PROPERTY( "w", &Vec4d::w );
END_META()
