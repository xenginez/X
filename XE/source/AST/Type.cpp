#include "Type.h"


BEG_META( XE::ASTEnum )
type->Property( "Name", &XE::ASTEnum::Name );
type->Property( "Flag", &XE::ASTEnum::Flag );
type->Property( "Module", &XE::ASTEnum::Module );
type->Property( "Elements", &XE::ASTEnum::Elements );
END_META()
