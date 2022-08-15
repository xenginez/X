/*!
 * \file	GCPtr.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/08/15
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GCPTR_HPP__8FE23E75_0743_4CD9_89B7_BA8B2EAD3106
#define GCPTR_HPP__8FE23E75_0743_4CD9_89B7_BA8B2EAD3106

#include "GCObject.h"
#include "GCMemoryResource.h"

BEG_XE_NAMESPACE

template< typename _Ty > class GCPtr
{
	static_assert( std::is_same_v< std::remove_all_extents_t<_Ty>, XE::GCObject >, "" );

private:
	template< typename _Yty > friend class GCPtr;

public:
	using element_type = std::remove_extent_t<_Ty>;

public:
	GCPtr() noexcept = default;

	GCPtr( std::nullptr_t ) noexcept = default;

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

		_Ptr = static_cast< element_type * >( _Right._Ptr );

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
	template<typename Ty, typename ... Types > friend XE::GCPtr< Ty > MakeGC( _Types &&... args );

private:
	element_type * _Ptr{ nullptr };
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

#endif//GCPTR_HPP__8FE23E75_0743_4CD9_89B7_BA8B2EAD3106
