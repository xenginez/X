#include "Rect.h"

BEG_META( XE::Recti )
REG_PROPERTY( "x", &XE::Recti::x );
REG_PROPERTY( "y", &XE::Recti::y );
REG_PROPERTY( "width", &XE::Recti::width );
REG_PROPERTY( "height", &XE::Recti::height );
END_META()

BEG_META( XE::Rectf )
REG_PROPERTY( "x", &XE::Rectf::x );
REG_PROPERTY( "y", &XE::Rectf::y );
REG_PROPERTY( "width", &XE::Rectf::width );
REG_PROPERTY( "height", &XE::Rectf::height );
END_META()

BEG_META( XE::Rectd )
REG_PROPERTY( "x", &XE::Rectd::x );
REG_PROPERTY( "y", &XE::Rectd::y );
REG_PROPERTY( "width", &XE::Rectd::width );
REG_PROPERTY( "height", &XE::Rectd::height );
END_META()
