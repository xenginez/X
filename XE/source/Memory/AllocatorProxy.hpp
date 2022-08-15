/*!
 * \file	AllocatorProxy.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/01/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ALLOCATORPROXY_HPP__D906995B_F0E7_4FAB_97FB_BA844CD27E7F
#define ALLOCATORPROXY_HPP__D906995B_F0E7_4FAB_97FB_BA844CD27E7F

#include "MemoryResource.h"

BEG_XE_NAMESPACE

template< typename T > class AllocatorProxy
{
public:
	static std::pmr::polymorphic_allocator< T > GetAllocator()
	{
		return { GetResource() };
	}

	static std::pmr::memory_resource * GetResource()
	{
		return XE::MemoryResource::GetDefaultMemoryResource();
	}
};

template< typename Ty, typename ... Args > Ty * New( Args &&... args )
{
	auto allocator = AllocatorProxy< Ty >::GetAllocator();

	auto p = allocator.allocate( 1 ); allocator.construct( p, std::forward< Args >( args )... );

	return p;
}

template< typename Ty > void Delete( Ty * ptr )
{
	auto allocator = AllocatorProxy< Ty >::GetAllocator();

	allocator.destroy( ptr ); allocator.deallocate( ptr, 1 );
}

template <typename Ty, typename Alloc, typename... Types > XE::SharedPtr< Ty > AllocateShared( const Alloc & _Al, Types &&... args )
{
	return std::allocate_shared< Ty >( _Al, std::forward< Types >( args )... );
}

template< typename Ty, typename ... Types > XE::SharedPtr< Ty > MakeShared( Types && ...args )
{
	return XE::AllocateShared< Ty >( XE::AllocatorProxy< Ty >::GetAllocator(), std::forward< Types >( args )... );
}

template< typename Ty, typename ... Types > XE::UniquePtr< Ty > MakeUnique( Types && ...args )
{
	return { XE::New< Ty >( std::forward< Types >( args )... ), XE::Deleter< Ty >() };
}

template<typename Ty, typename ... Types > XE::GCPtr< Ty > MakeGC( _Types &&... args )
{
	return new ( XE::MemoryResource::GetGCMemoryResource()->allocate( sizeof( Ty ) ) ) Ty( std::forward< Types >( args )... );
}

END_XE_NAMESPACE

#endif//ALLOCATORPROXY_HPP__D906995B_F0E7_4FAB_97FB_BA844CD27E7F
