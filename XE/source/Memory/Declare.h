/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__FFE474C0_FA49_4631_A284_A4861F8FB635
#define DECLARE_H__FFE474C0_FA49_4631_A284_A4861F8FB635

#include "Base/Base.h"

BEG_XE_NAMESPACE

class MemoryResource;
class FrameMemoryResource;
class ObjectMemoryResource;
class DefaultMemoryResource;

template< typename Ty > void Delete( Ty * ptr );
template< typename Ty, typename ... Args > Ty * New( Args &&... args );

template < typename _Ty > struct Deleter
{
	constexpr Deleter() noexcept = default;

	template < typename _Ty2, std::enable_if_t< std::is_convertible_v< _Ty2 *, _Ty * >, int > = 0 > Deleter( const Deleter< _Ty2 > & ) noexcept
	{}

	void operator()( _Ty * _Ptr ) const noexcept
	{
		XE::Delete( _Ptr );
	}
};
template< typename T, typename D = XE::Deleter< T > > using UniquePtr = std::unique_ptr< T, D >;


template< typename ... T >
using Tuple = std::tuple< T... >;

class MMapFile;
class MemoryResource;

template< typename T > class ObjectPage;
template< typename T > class AllocatorProxy;
template< class _Elem > class BasicMemoryView;
template< typename Pt, typename At > class PageAllocator;
template< class _Elem, class _Traits, class _Alloc > class BasicMemoryStream;
template< class _Elem, class _Traits, class _Alloc > class BasicIMemoryStream;
template< class _Elem, class _Traits, class _Alloc > class BasicOMemoryStream;

END_XE_NAMESPACE

#define OBJECT_ALLOCATOR_PROXY( TYPE ) \
template< > class XE::AllocatorProxy< TYPE > \
{ \
public: \
	static std::pmr::polymorphic_allocator< TYPE > GetAllocator() \
	{ \
		return { GetResource() }; \
	} \
	static std::pmr::memory_resource * GetResource() \
	{ \
		return XE::MemoryResource::GetObjectMemoryResource(); \
	} \
};

#endif // DECLARE_H__FFE474C0_FA49_4631_A284_A4861F8FB635
