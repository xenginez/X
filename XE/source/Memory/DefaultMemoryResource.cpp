#include "DefaultMemoryResource.h"

#include <tbb/scalable_allocator.h>
#include <tbb/cache_aligned_allocator.h>

XE::DefaultMemoryResource::DefaultMemoryResource()
{
	std::pmr::set_default_resource( this );
}

XE::DefaultMemoryResource::~DefaultMemoryResource() noexcept
{
	std::pmr::set_default_resource( std::pmr::new_delete_resource() );
}

void * XE::DefaultMemoryResource::do_allocate( size_t _Bytes, size_t _Align )
{
	return tbb::scalable_memory_resource()->allocate( _Bytes, _Align );
}

void XE::DefaultMemoryResource::do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align )
{
	tbb::scalable_memory_resource()->deallocate( _Ptr, _Bytes, _Align );
}

bool XE::DefaultMemoryResource::do_is_equal( const std::pmr::memory_resource & _That ) const noexcept
{
	return tbb::scalable_memory_resource()->is_equal( _That );
}
