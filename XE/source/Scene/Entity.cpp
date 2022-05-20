#include "Entity.h"

#include "Utils/Logger.h"
#include "Core/ThreadService.h"

#include "World.h"

BEG_META( XE::EntitySystem )
END_META()

BEG_META( XE::EntityComponent )
END_META()

BEG_META( XE::EntitySystemGroup )
type->Property( "Graph", &XE::EntitySystemGroup::_Graph );
END_META()

BEG_META( XE::NameEntityComponent )
type->Property( "Name", &XE::NameEntityComponent::Name );
END_META()

void XE::EntitySystemGroup::Execute( const XE::World * world, XE::float32 dt ) const
{

}
