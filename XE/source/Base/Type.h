/*!
 * \file   Type.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __TYPE_H__B9857ED9_F6EE_4A0A_8E87_0DAA433A5E40
#define __TYPE_H__B9857ED9_F6EE_4A0A_8E87_0DAA433A5E40

#include "Declare.h"

namespace std
{
	template< class _Ty > struct remove_pointer< weak_ptr< _Ty > >
	{
		using type = typename remove_pointer< _Ty >::type;
	};

	template< class _Ty > struct remove_pointer< shared_ptr< _Ty > >
	{
		using type = typename remove_pointer< _Ty >::type;
	};

	template< typename T > struct is_weak_ptr
	{
		using type = T;
		static constexpr bool value = false;
	};

	template< typename T > struct is_weak_ptr< weak_ptr< T > >
	{
		using type = T;
		static constexpr bool value = true;
	};

	template< typename T > inline constexpr bool is_weak_ptr_v = is_weak_ptr< T >::value;

	template< typename T > struct is_shared_ptr
	{
		using type = T;
		static constexpr bool value = false;
	};

	template< typename T > struct is_shared_ptr< shared_ptr< T > >
	{
		using type = T;
		static constexpr bool value = true;
	};

	template< typename T > inline constexpr bool is_shared_ptr_v = is_shared_ptr< T >::value;

	template< typename T > struct is_container
	{
		using type = T;
		static constexpr bool value = false;
	};
	template< typename T > inline constexpr bool is_container_v = is_container< T >::value;


	template< typename ... T > struct is_container< std::vector< T... > >
	{
		using type = std::vector< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::deque< T... > >
	{
		using type = std::deque< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::stack< T... > >
	{
		using type = std::stack< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::queue< T... > >
	{
		using type = std::queue< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::pair< T... > >
	{
		using type = std::pair< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::set< T... > >
	{
		using type = std::set< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::map< T... > >
	{
		using type = std::map< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::multiset< T... > >
	{
		using type = std::multiset< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::multimap< T... > >
	{
		using type = std::multimap< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::unordered_set< T... > >
	{
		using type = std::unordered_set< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::unordered_map< T... > >
	{
		using type = std::unordered_map< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::unordered_multiset< T... > >
	{
		using type = std::unordered_multiset< T... >;
		static constexpr bool value = true;
	};

	template< typename ... T > struct is_container< std::unordered_multimap< T... > >
	{
		using type = std::unordered_multimap< T... >;
		static constexpr bool value = true;
	};


	template< typename T > void swap( std::pmr::polymorphic_allocator< T > & _Left, std::pmr::polymorphic_allocator< T > & _Right ) noexcept
	{
		XE_ASSERT( _Left == _Right && "polymorphic_allocator incompatible for swap" );
	}


	template <class _Ty1, class _Ty2> _Ty1 * static_pointer_cast( _Ty2 * _Other ) noexcept
	{
		return static_cast<_Ty1 *>( _Other );
	}

	template <class _Ty1, class _Ty2> const _Ty1 * static_pointer_cast( const _Ty2 * _Other ) noexcept
	{
		return static_cast<const _Ty1 *>( _Other );
	}

	template <class _Ty1, class _Ty2> _Ty1 * const_pointer_cast( _Ty2 * _Other ) noexcept
	{
		return const_cast<_Ty1 *>( _Other );
	}

	template <class _Ty1, class _Ty2> _Ty1 * const_pointer_cast( const _Ty2 * _Other ) noexcept
	{
		return const_cast<_Ty1 *>( _Other );
	}

	template <class _Ty1, class _Ty2> _Ty1 * reinterpret_pointer_cast( _Ty2 * _Other ) noexcept
	{
		return reinterpret_cast<_Ty1 *>( _Other );
	}

	template <class _Ty1, class _Ty2> const _Ty1 * reinterpret_pointer_cast( const _Ty2 * _Other ) noexcept
	{
		return reinterpret_cast<const _Ty1 *>( _Other );
	}
};

BEG_XE_NAMESPACE


template < class To, class From > To BitCast( const From & src )
{
	To dst;
	std::memset( &dst, 0, sizeof( To ) );
	std::memcpy( &dst, &src, std::min( sizeof( From ), sizeof( To ) ) );
	return dst;
}

template< typename T > struct TypeTraits
{
	using value_t = T;

	using remove_const_t = typename std::remove_const< value_t >::type;
	using remove_pointer_t = typename std::remove_pointer< value_t >::type;
	using remove_volatile_t = typename std::remove_volatile< value_t >::type;
	using remove_reference_t = typename std::remove_reference< value_t >::type;

	using remove_const_pointer_t = typename std::remove_const< remove_pointer_t >::type;
	using remove_const_volatile_t = typename std::remove_const< remove_volatile_t >::type;
	using remove_const_reference_t = typename std::remove_const< remove_reference_t >::type;
	using remove_pointer_volatile_t = typename std::remove_pointer< remove_volatile_t >::type;
	using remove_pointer_reference_t = typename std::remove_pointer< remove_reference_t >::type;
	using remove_volatile_reference_t = typename std::remove_volatile< remove_reference_t >::type;

	using remove_const_pointer_volatile_t = typename std::remove_const< remove_pointer_volatile_t >::type;
	using remove_const_pointer_reference_t = typename std::remove_const< remove_pointer_reference_t >::type;
	using remove_const_volatile_reference_t = typename std::remove_const< remove_volatile_reference_t >::type;
	using remove_pointer_volatile_reference_t = typename std::remove_pointer< remove_volatile_reference_t >::type;

	using raw_t = typename std::remove_pointer< typename std::remove_const< typename std::remove_volatile< typename std::remove_reference< value_t >::type >::type >::type >::type;
};

template< typename ... T > struct Overloaded : T... { using T::operator()...; };
template< typename ... T > Overloaded( T... ) -> Overloaded< T... >;

template< typename ... T > struct FunctionTraits;
template< typename R, typename ... Types > struct FunctionTraits< R( Types... ) >
{
	static constexpr bool is_callable = true;
	static constexpr size_t args_size = sizeof...( Types );

	using result_type = R;
	using function_type = R( Types... );
	template<size_t I> struct args
	{
		static_assert( I < args_size, "index is out of range, index must less than sizeof Args" );
		using type = typename std::tuple_element< I, std::tuple< Types... > >::type;
	};

	typedef std::tuple< Types... > tuple_type;
	typedef std::tuple< std::remove_cv_t< std::remove_reference_t< Types > >... > pod_tuple_type;
};
template< typename R, typename C, typename ... Types > struct FunctionTraits< C, R( Types... ) >
{
	static constexpr bool is_callable = true;
	static constexpr size_t args_size = sizeof...( Types );

	using result_type = R;
	using class_type = C;
	using function_type = R( Types... );
	template<size_t I> struct args
	{
		static_assert( I < args_size, "index is out of range, index must less than sizeof Args" );
		using type = typename std::tuple_element< I, std::tuple< Types... > >::type;
	};

	typedef std::tuple< Types... > tuple_type;
	typedef std::tuple< std::remove_cv_t< std::remove_reference_t< Types > >... > pod_tuple_type;
};
template< typename R, typename ... Types > struct FunctionTraits< R( * )( Types... ) > : public FunctionTraits< R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) > : public FunctionTraits< C, R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) const > : public FunctionTraits< C, R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) volatile > : public FunctionTraits< C, R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) noexcept > : public FunctionTraits< C, R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) const volatile > : public FunctionTraits< C, R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) const noexcept > : public FunctionTraits< C, R( Types... ) > {};
template< typename R, typename C, typename ... Types > struct FunctionTraits< R( C:: * )( Types... ) const volatile noexcept > : public FunctionTraits< C, R( Types... ) > {};
template< typename Callable > struct FunctionTraits< Callable > : public FunctionTraits< decltype( &Callable::operator() ) > {};
template< typename R, typename ... Types > struct FunctionTraits< std::function< R( Types... ) > > : public FunctionTraits< R( Types... ) > {};

template< bool... > struct __TupleTypeCheck {};
template< typename _Fy, typename... _Ty > constexpr bool TupleHasType( const std::tuple< _Ty... > & )
{
	return !std::is_same< __TupleTypeCheck< false, std::is_same< _Fy, _Ty >::value... >, __TupleTypeCheck< std::is_same< _Fy, _Ty >::value..., false > >::value;
}

template< typename Func, typename Tuple, std::size_t I > void ApplyTupleImpl( Func && f, Tuple & t )
{
	f( std::ref( std::get< I >( t ) ) );

	if constexpr ( I < std::tuple_size< Tuple >::value )
	{
		ApplyTupleImpl< I + 1 >( f, t );
	}
}
template< typename Func, typename Tuple > void ApplyTuple( Func && f, Tuple & t )
{
	return ApplyTupleImpl< 0 >( std::forward< Func >( f ), std::forward< Tuple >( t ) );
}

template< typename T > XE_INLINE static T * RawPointer( T * val )
{
	return val;
}

template< typename T > XE_INLINE static T * RawPointer( T & val )
{
	return &val;
}

template< typename T > XE_INLINE static T * RawPointer( T ** val )
{
	return RawPointer( *val );
}

template< typename T > XE_INLINE static const T * RawPointer( const T * val )
{
	return val;
}

template< typename T > XE_INLINE static const T * RawPointer( const T & val )
{
	return &val;
}

template< typename T > XE_INLINE static T * RawPointer( std::shared_ptr< T > * val )
{
	return val ? val->get() : nullptr;
}

template< typename T > XE_INLINE static T * RawPointer( const std::shared_ptr< T > * val )
{
	return val ? val->get() : nullptr;
}


XE_INLINE constexpr XE::uint64 Hash( const XE::uint8 * ptr, XE::uint64 size, XE::uint64 value = 14695981039346656037ULL )
{
	return size != 0 ? Hash( ++ptr, --size, ( value ^ static_cast< XE::uint64 >( *ptr ) ) * 1099511628211ULL ) : value;
}

XE_INLINE constexpr XE::uint64 Hash( const char * str, XE::uint64 value = 14695981039346656037ULL )
{
	return *str != 0 ? Hash( ++str, ( value ^ static_cast< XE::uint64 >( *str ) ) * 1099511628211ULL ) : value;
}

XE_INLINE constexpr XE::uint64 Hash( const wchar_t * str, XE::uint64 value = 14695981039346656037ULL )
{
	return *str != 0 ? Hash( ++str, ( value ^ static_cast< XE::uint64 >( *str ) ) * 1099511628211ULL ) : value;
}

#ifdef __cpp_char8_t
XE_INLINE constexpr XE::uint64 Hash( const char8_t * str, XE::uint64 value = 14695981039346656037ULL )
{
	return *str != 0 ? Hash( ++str, ( value ^ static_cast< XE::uint64 >( *str ) ) * 1099511628211ULL ) : value;
}
#endif

template< typename T > XE_INLINE constexpr XE::uint64 Hash( const T & val, XE::uint64 value = 14695981039346656037ULL ) noexcept
{
	return XE::Hash( reinterpret_cast<const XE::uint8 *>( &val ), sizeof( T ), value );
}

template <class T, XE::uint64 N > XE_INLINE constexpr XE::uint64 Hash( const T val[], XE::uint64 value = 14695981039346656037ULL )
{
	for( size_t i = 0; i < N; ++i )
	{
		value ^= XE::Hash( val[i] );
	}

	return value;
}

template< typename ... Types > XE_INLINE constexpr XE::uint64 Hash( const std::basic_string< Types... > & val, XE::uint64 value = 14695981039346656037ULL ) noexcept
{
	return XE::Hash( val.c_str(), value );
}

template < typename T > struct Hasher
{
	using argument_type = T;
	using result_type = XE::uint64;

	result_type operator()( const argument_type & val ) const
	{
		return XE::Hash( val );
	}
};


template< typename T > using Set = std::pmr::set< T >;
template< typename T > using List = std::pmr::list< T >;
template< typename T > using Deque = std::pmr::deque< T >;
template< typename T > using Array = std::pmr::vector< T >;
template< typename T > using MultiSet = std::pmr::multiset< T >;
template< typename T > using Stack = std::stack< T, XE::Deque< T > >;
template< typename T > using Queue = std::queue< T, XE::Deque< T > >;
template< typename T > using UnorderedSet = std::pmr::unordered_set< T, XE::Hasher< T > >;
template< typename T > using UnorderedMultiSet = std::pmr::unordered_multiset< T, XE::Hasher< T > >;
template< typename K, typename V > using Pair = std::pair< K, V >;
template< typename K, typename V, typename L = std::less< K > > using Map = std::pmr::map< K, V, L >;
template< typename K, typename V, typename L = std::less< K >  > using MultiMap = std::pmr::multimap< K, V, L >;
template< typename K, typename V, typename H = XE::Hasher< K > > using UnorderedMap = std::pmr::unordered_map< K, V, H >;
template< typename K, typename V, typename H = XE::Hasher< K > > using UnorderedMultiMap = std::pmr::unordered_multimap< K, V, H >;

END_XE_NAMESPACE

#define DECL_PTR( TYPE ) \
class TYPE; \
typedef TYPE * TYPE##RPtr; \
typedef XE::SharedPtr< TYPE > TYPE##Ptr; \
typedef XE::WeakPtr< TYPE > TYPE##WPtr; \
typedef XE::SharedPtr< TYPE > TYPE##SPtr; \
typedef XE::UniquePtr< TYPE > TYPE##UPtr; \
typedef const TYPE * TYPE##CRPtr; \
typedef XE::SharedPtr< const TYPE > TYPE##CPtr; \
typedef XE::WeakPtr< const TYPE > TYPE##CWPtr; \
typedef XE::UniquePtr< const TYPE > TYPE##CUPtr;

#define CP_CAST std::const_pointer_cast
#define SP_CAST std::static_pointer_cast
#define DP_CAST std::dynamic_pointer_cast
#define RP_CAST std::reinterpret_pointer_cast

#define XE_THIS( TYPE ) std::static_pointer_cast< TYPE >( shared_from_this() )

constexpr XE::uint64 operator "" _hash( const char * p, size_t )
{
	return XE::Hash( p );
}

#endif // __TYPE_H__B9857ED9_F6EE_4A0A_8E87_0DAA433A5E40
