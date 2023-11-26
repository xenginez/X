/*!
 * \file	GCObject.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/15
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GCOBJECT_H__358986EA_20A0_43CF_975E_6D0BCBEF1AB4
#define GCOBJECT_H__358986EA_20A0_43CF_975E_6D0BCBEF1AB4

#include "Base/Type.h"

#include "GCMemoryResource.h"

namespace std
{
	template< typename T > struct is_gc_ptr
	{
		using type = T;
		static constexpr bool value = false;
	};
	template< typename T > struct is_gc_ptr< XE::GCPtr< T > >
	{
		using type = T;
		static constexpr bool value = true;
	};
	template< typename T > inline constexpr bool is_gc_ptr_v = is_gc_ptr< T >::value;
}

BEG_XE_NAMESPACE

class XE_API GCObject
{
private:
	friend class XE::GCMemoryResource;
	template< typename T > friend struct GCMarker;

public:
	GCObject();

	virtual ~GCObject();

protected:
	virtual void Mark() const;

	virtual XE::uint64 Size() const;

private:
	GCStatus _Status;
};

class XE_API GCRootObject : public GCObject
{
public:
	GCRootObject();

	~GCRootObject() override;
};

template< typename _Ty > class GCPtr
{
	static_assert( std::is_same_v< std::remove_all_extents_t<_Ty>, XE::GCObject >, "" );

private:
	template< typename _Yty > friend class GCPtr;

public:
	using element_type = std::remove_extent_t<_Ty>;

public:
	GCPtr() = default;

	GCPtr( std::nullptr_t ) noexcept
		:_Ptr( nullptr )
	{
	}

	GCPtr( GCPtr && _Right ) noexcept
	{
		swap( *this, std::forward< GCPtr >( _Right ) );
	}

	GCPtr( const GCPtr & _Other ) noexcept
		:_Ptr( _Other._Ptr )
	{
		if ( _Ptr )
		{
			XE::GCMemoryResource::Barrier( _Ptr );
		}
	}

	template <typename _Ty2, std::enable_if_t<std::is_convertible< _Ty2 *, _Ty * >::value, int> = 0> GCPtr( GCPtr<_Ty2> && _Right ) noexcept
	{
		swap( *this, static_cast<GCPtr<_Ty> &>( std::forward<GCPtr<_Ty2>>( _Right ) ) );
	}

	template <typename _Ty2, std::enable_if_t<std::is_convertible< _Ty2 *, _Ty * >::value, int> = 0> GCPtr( const GCPtr<_Ty2> & _Other ) noexcept
		:_Ptr( static_cast<element_type *>( _Other._Ptr ) )
	{
		if ( _Ptr )
		{
			XE::GCMemoryResource::Barrier( _Ptr );
		}
	}

	~GCPtr() noexcept
	{

	}

private:
	template <typename _Ty2, std::enable_if_t<std::is_convertible< _Ty2 *, _Ty * >::value, int> = 0> GCPtr( _Ty2 * _Px )
		:_Ptr( static_cast<_Ty *>( _Px ) )
	{
		XE::GCMemoryResource::Barrier( _Ptr );
	}

public:
	GCPtr & operator=( GCPtr && _Right ) noexcept
	{
		GCPtr( std::move( _Right ) ).swap( *this );

		return *this;
	}

	GCPtr & operator=( const GCPtr & _Right ) noexcept
	{
		if ( _Right._Ptr )
		{
			XE::GCMemoryResource::Barrier( _Right._Ptr );
		}

		_Ptr = _Right._Ptr;

		return *this;
	}

	template <typename _Ty2> GCPtr & operator=( GCPtr<_Ty2> && _Right ) noexcept
	{
		GCPtr( std::move( _Right ) ).swap( *this );

		return *this;
	}

	template <typename _Ty2> GCPtr & operator=( const GCPtr<_Ty2> & _Right ) noexcept
	{
		if ( _Right._Ptr )
		{
			XE::GCMemoryResource::Barrier( _Right._Ptr );
		}

		_Ptr = static_cast<element_type *>( _Right._Ptr );

		return *this;
	}

public:
	void swap( GCPtr & _Other ) noexcept
	{
		if ( _Ptr )
		{
			XE::GCMemoryResource::Barrier( _Ptr );
		}
		if ( _Other._Ptr )
		{
			XE::GCMemoryResource::Barrier( _Other._Ptr );
		}

		std::swap( _Ptr, _Other._Ptr );
	}

public:
	element_type * get() const noexcept
	{
		return _Ptr;
	}

public:
	void reset() noexcept
	{
		GCPtr().swap( *this );
	}

public:
	template <typename _Ty2 = _Ty, std::enable_if_t<!std::is_array_v<_Ty2>, int> = 0> _Ty2 * operator->() const noexcept
	{
		return get();
	}

	template <typename _Ty2 = _Ty, std::enable_if_t<!std::disjunction_v<std::is_array<_Ty2>, std::is_void<_Ty2>>, int> = 0> _Ty2 & operator*() const noexcept
	{
		return *get();
	}

public:
	template <typename _Ty2 = _Ty, typename _Elem = element_type, std::enable_if_t<std::is_array_v<_Ty2>, int> = 0> _Elem & operator[]( ptrdiff_t _Idx ) const noexcept
	{
		return get()[_Idx];
	}

public:
	operator bool() const noexcept
	{
		return get() != nullptr;
	}

private:
	template<typename Ty, typename ... Types > friend XE::GCPtr< Ty > MakeGC( Types &&... args );

private:
	element_type * _Ptr{ nullptr };
};

template< typename _Ty > struct GCMarker
{
public:
	template< typename U > struct HasMemberMark
	{
	private:
		template < typename Ty, void( Ty:: * )() const = &Ty::Mark >
		static constexpr auto check( Ty * ) { return true; };

		static constexpr bool check( ... ) { return false; };

	public:
		static constexpr bool value = check( static_cast<U *>( nullptr ) );
	};

public:
	static void Mark( _Ty * val )
	{
		if constexpr ( HasMemberMark< _Ty >::value )
		{
			val->Mark();
		}
	}
};
template< typename _Ty > struct GCMarker< XE::GCPtr< _Ty > >
{
	static void Mark( const XE::GCPtr< _Ty > * val )
	{
		if ( val != nullptr )
		{
			GCMarker< XE::TypeTraits< _Ty >::raw_t >::Mark( val->get() );
		}
	}
};
template< typename ... Types > struct GCMarker< std::list< Types... > >
{
	using value_type = typename std::list< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::list< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::deque< Types... > >
{
	using value_type = typename std::deque< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::deque< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::vector< Types... > >
{
	using value_type = typename std::vector< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::vector< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::pair< Types... > >
{
	using key_type = typename std::pair< Types... >::first_type;
	using raw_key_type = typename XE::TypeTraits< key_type >::raw_t;
	using value_type = typename std::pair< Types... >::second_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::pair< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_key_type, XE::GCObject > || std::is_same_v< raw_key_type, XE::GCObject > )
		{
			if constexpr ( std::is_pointer_v< key_type > )
				GCMarker< raw_key_type >::Mark( val->first );
			else
				GCMarker< raw_key_type >::Mark( &val.first );
		}

		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			if constexpr ( std::is_pointer_v< value_type > )
				GCMarker< raw_value_type >::Mark( val->second );
			else
				GCMarker< raw_value_type >::Mark( &val.second );
		}
	}
};
template< typename ... Types > struct GCMarker< std::set< Types... > >
{
	using value_type = typename std::set< Types... >::key_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::set< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::map< Types... > >
{
	using value_type = typename std::map< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::map< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::multiset< Types... > >
{
	using value_type = typename std::multiset< Types... >::key_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::multiset< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::multimap< Types... > >
{
	using value_type = typename std::multimap< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::multimap< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::unordered_set< Types... > >
{
	using value_type = typename std::unordered_set< Types... >::key_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::unordered_set< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::unordered_map< Types... > >
{
	using value_type = typename std::unordered_multimap< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::unordered_multimap< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::unordered_multiset< Types... > >
{
	using value_type = typename std::unordered_multiset< Types... >::key_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::unordered_multiset< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};
template< typename ... Types > struct GCMarker< std::unordered_multimap< Types... > >
{
	using value_type = typename std::unordered_multimap< Types... >::value_type;
	using raw_value_type = typename XE::TypeTraits< value_type >::raw_t;

	static void Mark( const std::unordered_multimap< Types... > * val )
	{
		if constexpr ( std::is_base_of_v< raw_value_type, XE::GCObject > || std::is_gc_ptr_v< raw_value_type > )
		{
			for ( const auto & it : *val )
			{
				if constexpr ( std::is_pointer_v< value_type > )
				{
					GCMarker< raw_value_type >::Mark( it );
				}
				else
				{
					GCMarker< raw_value_type >::Mark( &it );
				}
			}
		}
	}
};

template <typename _Ty1, typename _Ty2> bool operator!=( const GCPtr<_Ty1> & _Left, const GCPtr<_Ty2> & _Right ) noexcept
{
	return _Left.get() != _Right.get();
}
template <typename _Ty1, typename _Ty2> bool operator<( const GCPtr<_Ty1> & _Left, const GCPtr<_Ty2> & _Right ) noexcept
{
	return _Left.get() < _Right.get();
}
template <typename _Ty1, typename _Ty2> bool operator>=( const GCPtr<_Ty1> & _Left, const GCPtr<_Ty2> & _Right ) noexcept
{
	return _Left.get() >= _Right.get();
}
template <typename _Ty1, typename _Ty2> bool operator>( const GCPtr<_Ty1> & _Left, const GCPtr<_Ty2> & _Right ) noexcept
{
	return _Left.get() > _Right.get();
}
template <typename _Ty1, typename _Ty2> bool operator<=( const GCPtr<_Ty1> & _Left, const GCPtr<_Ty2> & _Right ) noexcept
{
	return _Left.get() <= _Right.get();
}
template <typename _Ty> bool operator==( std::nullptr_t, const GCPtr<_Ty> & _Right ) noexcept
{
	return nullptr == _Right.get();
}
template <typename _Ty> bool operator!=( const GCPtr<_Ty> & _Left, std::nullptr_t ) noexcept
{
	return _Left.get() != nullptr;
}
template <typename _Ty> bool operator!=( std::nullptr_t, const GCPtr<_Ty> & _Right ) noexcept
{
	return nullptr != _Right.get();
}
template <typename _Ty> bool operator<( const GCPtr<_Ty> & _Left, std::nullptr_t ) noexcept
{
	return _Left.get() < static_cast<typename GCPtr<_Ty>::element_type *>( nullptr );
}
template <typename _Ty> bool operator<( std::nullptr_t, const GCPtr<_Ty> & _Right ) noexcept
{
	return static_cast<typename GCPtr<_Ty>::element_type *>( nullptr ) < _Right.get();
}
template <typename _Ty> bool operator>=( const GCPtr<_Ty> & _Left, std::nullptr_t ) noexcept
{
	return _Left.get() >= static_cast<typename GCPtr<_Ty>::element_type *>( nullptr );
}
template <typename _Ty> bool operator>=( std::nullptr_t, const GCPtr<_Ty> & _Right ) noexcept
{
	return static_cast<typename GCPtr<_Ty>::element_type *>( nullptr ) >= _Right.get();
}
template <typename _Ty> bool operator>( const GCPtr<_Ty> & _Left, std::nullptr_t ) noexcept
{
	return _Left.get() > static_cast<typename GCPtr<_Ty>::element_type *>( nullptr );
}
template <typename _Ty> bool operator>( std::nullptr_t, const GCPtr<_Ty> & _Right ) noexcept
{
	return static_cast<typename GCPtr<_Ty>::element_type *>( nullptr ) > _Right.get();
}
template <typename _Ty> bool operator<=( const GCPtr<_Ty> & _Left, std::nullptr_t ) noexcept
{
	return _Left.get() <= static_cast<typename GCPtr<_Ty>::element_type *>( nullptr );
}
template <typename _Ty> bool operator<=( std::nullptr_t, const GCPtr<_Ty> & _Right ) noexcept
{
	return static_cast<typename GCPtr<_Ty>::element_type *>( nullptr ) <= _Right.get();
}
template <typename _Ty> void swap( GCPtr<_Ty> & _Left, GCPtr<_Ty> & _Right ) noexcept
{
	_Left.swap( _Right );
}

END_XE_NAMESPACE

namespace std
{
	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> static_pointer_cast( const XE::GCPtr<_Ty2> & _Other ) noexcept
	{
		const auto _Ptr = static_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );
		return XE::GCPtr<_Ty1>( _Other, _Ptr );
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> static_pointer_cast( XE::GCPtr<_Ty2> && _Other ) noexcept
	{
		const auto _Ptr = static_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );
		return XE::GCPtr<_Ty1>( std::move( _Other ), _Ptr );
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> const_pointer_cast( const XE::GCPtr<_Ty2> & _Other ) noexcept
	{
		const auto _Ptr = const_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );
		return XE::GCPtr<_Ty1>( _Other, _Ptr );
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> const_pointer_cast( XE::GCPtr<_Ty2> && _Other ) noexcept
	{
		const auto _Ptr = const_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );
		return XE::GCPtr<_Ty1>( std::move( _Other ), _Ptr );
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> reinterpret_pointer_cast( const XE::GCPtr<_Ty2> & _Other ) noexcept
	{
		const auto _Ptr = reinterpret_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );
		return XE::GCPtr<_Ty1>( _Other, _Ptr );
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> reinterpret_pointer_cast( XE::GCPtr<_Ty2> && _Other ) noexcept
	{
		const auto _Ptr = reinterpret_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );
		return XE::GCPtr<_Ty1>( std::move( _Other ), _Ptr );
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> dynamic_pointer_cast( const XE::GCPtr<_Ty2> & _Other ) noexcept
	{
		const auto _Ptr = dynamic_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );

		if ( _Ptr )
		{
			return XE::GCPtr<_Ty1>( _Other, _Ptr );
		}

		return {};
	}

	template <typename _Ty1, typename _Ty2> XE::GCPtr<_Ty1> dynamic_pointer_cast( XE::GCPtr<_Ty2> && _Other ) noexcept
	{
		const auto _Ptr = dynamic_cast<typename XE::GCPtr<_Ty1>::element_type *>( _Other.get() );

		if ( _Ptr )
		{
			return XE::GCPtr<_Ty1>( std::move( _Other ), _Ptr );
		}

		return {};
	}
}

#endif//GCOBJECT_H__358986EA_20A0_43CF_975E_6D0BCBEF1AB4
