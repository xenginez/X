#include "ObjectMemoryResource.h"

#include "MemoryResource.h"

XE::ObjectMemoryResource::ObjectMemoryResource()
	:std::pmr::synchronized_pool_resource( std::pmr::pool_options{ 0, 512 }, XE::MemoryResource::GetDefaultMemoryResource() )
{
}

XE::ObjectMemoryResource::~ObjectMemoryResource()
{

}

void * XE::ObjectMemoryResource::do_allocate( size_t _Bytes, size_t _Align )
{
	return std::pmr::synchronized_pool_resource::do_allocate( _Bytes, _Align );
}

void XE::ObjectMemoryResource::do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align )
{
	std::pmr::synchronized_pool_resource::do_deallocate( _Ptr, _Bytes, _Align );
}

bool XE::ObjectMemoryResource::do_is_equal( const std::pmr::memory_resource & _That ) const noexcept
{
	return std::pmr::synchronized_pool_resource::do_is_equal( _That );
}
