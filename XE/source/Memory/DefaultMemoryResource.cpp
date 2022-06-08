#include "DefaultMemoryResource.h"

XE::DefaultMemoryResource::DefaultMemoryResource()
{

}

XE::DefaultMemoryResource::~DefaultMemoryResource() noexcept
{

}

void * XE::DefaultMemoryResource::do_allocate( size_t _Bytes, size_t _Align )
{
	
	return std::pmr::get_default_resource()->allocate( _Bytes, _Align );
}

void XE::DefaultMemoryResource::do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align )
{
	std::pmr::get_default_resource()->deallocate( _Ptr, _Bytes, _Align );
}

bool XE::DefaultMemoryResource::do_is_equal( const std::pmr::memory_resource & _That ) const noexcept
{
	return std::pmr::get_default_resource()->is_equal( _That );
}
