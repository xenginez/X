#include "MemoryResource.h"

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
