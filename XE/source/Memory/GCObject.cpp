#include "GCObject.h"

#include "GCMemoryResource.h"

XE::GCObject::GCObject()
{

}

XE::GCObject::~GCObject()
{

}

void XE::GCObject::Mark()
{

}

XE::GCRootObject::GCRootObject()
{
	XE::GCMemoryResource::Register( this );
}

XE::GCRootObject::~GCRootObject()
{
	XE::GCMemoryResource::Unregister( this );
}
