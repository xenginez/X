#include "MemoryResource.h"

void * XE::MemoryResource::Alloc( XE::uint64 size )
{
	return ::malloc( size );
}

void XE::MemoryResource::Free( void * ptr )
{
	::free( ptr );
}

XE::GCMemoryResource * XE::MemoryResource::GetGCMemoryResource()
{
	static XE::GCMemoryResource resource;

	return &resource;
}

XE::FrameMemoryResource * XE::MemoryResource::GetFrameMemoryResource()
{
	thread_local XE::FrameMemoryResource resource;

	return &resource;
}

XE::ObjectMemoryResource * XE::MemoryResource::GetObjectMemoryResource()
{
	static XE::ObjectMemoryResource resource;
	
	return &resource;
}

XE::DefaultMemoryResource * XE::MemoryResource::GetDefaultMemoryResource()
{
	static XE::DefaultMemoryResource resource;

	return &resource;
}
