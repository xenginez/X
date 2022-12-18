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

class GCRoot;
class GCObject;
class GCRootObject;
class MemoryResource;
class GCMemoryResource;
class FrameMemoryResource;
class ObjectMemoryResource;
class DefaultMemoryResource;

template< typename _Ty > struct GCMarker;

template< typename _Ty > class GCPtr;
template< typename _Ty > class GCWeakPtr;
template< typename _Ty > class EnableGCFromThis;
template< XE::uint64 S > class StackMemoryResource;

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

class MMapFile;
class MemoryResource;
template< typename T > class GCPtr;
template< typename T > class ObjectPage;
template< typename T > class AllocatorProxy;
template< class _Elem > class BasicMemoryView;
template< typename Pt, typename At > class PageAllocator;
template< class _Elem, class _Traits, class _Alloc > class BasicMemoryStream;
template< class _Elem, class _Traits, class _Alloc > class BasicIMemoryStream;
template< class _Elem, class _Traits, class _Alloc > class BasicOMemoryStream;

END_XE_NAMESPACE

#define DECL_PTR( TYPE ) \
class TYPE; \
using TYPE##Ptr = XE::SharedPtr< TYPE >; \
using TYPE##WPtr = XE::WeakPtr< TYPE >; \
using TYPE##UPtr = XE::UniquePtr< TYPE >; \
using TYPE##GPtr = XE::GCPtr< TYPE >; \
using TYPE##GWPtr = XE::GCWeakPtr< TYPE >; \
using TYPE##CPtr = XE::SharedPtr< const TYPE >; \
using TYPE##CWPtr = XE::WeakPtr< const TYPE >; \
using TYPE##CUPtr = XE::UniquePtr< const TYPE >; \
using TYPE##CGPtr = XE::GCPtr< const TYPE >; \
using TYPE##CGWPtr = XE::GCWeakPtr< const TYPE >;

#define CP_CAST std::const_pointer_cast
#define SP_CAST std::static_pointer_cast
#define DP_CAST std::dynamic_pointer_cast
#define RP_CAST std::reinterpret_pointer_cast

#define XE_THIS( TYPE ) std::static_pointer_cast< TYPE >( shared_from_this() )

#define OBJECT_POOL_ALLOCATOR( TYPE ) \
template<> class XE::AllocatorProxy< TYPE > \
{ \
public: \
	static std::pmr::polymorphic_allocator< TYPE > GetAllocator() \
	{ \
		return { GetResource() }; \
	} \
	static std::pmr::memory_resource * GetResource() \
	{ \
		if constexpr( sizeof( TYPE ) <= 512 ) \
		{ \
			return XE::MemoryResource::GetObjectMemoryResource(); \
		} \
		else \
		{ \
			return XE::MemoryResource::GetDefaultMemoryResource(); \
		} \
	} \
};

#endif // DECLARE_H__FFE474C0_FA49_4631_A284_A4861F8FB635
