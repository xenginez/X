#include "HeightField.h"

BEG_META( XE::HeightField )
type->Property( "min", &XE::HeightField::min );
type->Property( "max", &XE::HeightField::max );
type->Property( "data", &XE::HeightField::data );
END_META()