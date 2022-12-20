#include "GCObject.h"

XE::GCObject::GCObject()
	:_Status( XE::GCStatus::WHITE )
{

}

XE::GCObject::~GCObject()
{

}

void XE::GCObject::Mark() const
{
	XE::GCMemoryResource::Mark( this );
}

XE::uint64 XE::GCObject::Size() const
{
	return sizeof( XE::GCObject );
}

XE::GCRootObject::GCRootObject()
{
	XE::GCMemoryResource::Register( this );
}

XE::GCRootObject::~GCRootObject()
{
	XE::GCMemoryResource::Unregister( this );
}
