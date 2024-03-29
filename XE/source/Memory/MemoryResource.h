#ifndef MEMORYRESOURCE_H__84074448_45AA_41EC_A082_4E1DA59DFEC6
#define MEMORYRESOURCE_H__84074448_45AA_41EC_A082_4E1DA59DFEC6

#include "GCMemoryResource.h"
#include "FrameMemoryResource.h"
#include "ObjectMemoryResource.h"
#include "DefaultMemoryResource.h"

BEG_XE_NAMESPACE

class XE_API MemoryResource
{
public:
	static void * Alloc( XE::uint64 size );

	static void Free( void * ptr );

	static XE::GCMemoryResource * GetGCMemoryResource();

	static XE::FrameMemoryResource * GetFrameMemoryResource();

	static XE::ObjectMemoryResource * GetObjectMemoryResource();

	static XE::DefaultMemoryResource * GetDefaultMemoryResource();
};

END_XE_NAMESPACE

#endif // MEMORYRESOURCE_H__84074448_45AA_41EC_A082_4E1DA59DFEC6
