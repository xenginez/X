#include "Event.h"

BEG_META( XE::Event )
REG_PROPERTY( "handle", &Event::handle )->Attribute( XE::NonInspectorAttribute() );
REG_PROPERTY( "parameter", &Event::parameter )->Attribute( XE::NonInspectorAttribute() );
END_META()

XE::Event::Event()
	:accept( false )
{

}

XE::Event::Event( XE::EventHandle id, const XE::Variant & parameter /*= Variant() */ )
	: accept( false ), handle( id ), parameter( parameter )
	
{

}

XE::EventPtr XE::Event::Create( XE::EventHandle handle, const XE::Variant & parameter /*= XE::Variant()*/ )
{
	return XE::MakeShared< XE::Event >( handle, parameter );
}
