#include "Type.h"

BEG_META( XE::ThreadType )
type->Value( "IO", XE::ThreadType::IO );
type->Value( "GAME", XE::ThreadType::GAME );
type->Value( "AUDIO", XE::ThreadType::AUDIO );
type->Value( "RENDER", XE::ThreadType::RENDER );
type->Value( "PHYSICS", XE::ThreadType::PHYSICS );
type->Value( "WORKS", XE::ThreadType::WORKS );
type->Value( "UNKNOWN", XE::ThreadType::UNKNOWN );
END_META()
