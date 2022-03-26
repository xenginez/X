/*!
 * \file	Delegate.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/02/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DELEGATE_HPP__C1E38B56_5986_4410_A7F5_E846D406BA60
#define DELEGATE_HPP__C1E38B56_5986_4410_A7F5_E846D406BA60

#include "Type.h"

BEG_XE_NAMESPACE

struct XE_API FunctorPODData
{
	void * data[4]{ nullptr, nullptr, nullptr, nullptr };

	void swap( FunctorPODData & val )
	{
		std::swap( data[0], val.data[0] );
		std::swap( data[1], val.data[1] );
		std::swap( data[2], val.data[2] );
		std::swap( data[3], val.data[3] );
	}
};

template< typename ... T > class Functor;
template< typename R, typename ... T > class FunctorBase;

template< typename R, typename ... T > class FunctorBase< R( T... ) >
{
public:
	using result_type = R;
	using function_type = R( T... );
	using arguments_tuple_type = std::tuple< T... >;

public:
	virtual ~FunctorBase() = default;

public:
	XE::int64 Inc()
	{
		return ++_Count;
	}

	XE::int64 Dec()
	{
		return --_Count;
	}

public:
	virtual result_type Invoke( typename arguments_tuple_type && args ) const = 0;

private:
	std::atomic< XE::int64 > _Count = 0;
};

template< typename F > class Functor< F > : public FunctorBase< typename XE::FunctionTraits< std::decay_t< F > >::function_type >
{
public:
	using functor_type = std::decay_t< F >;
	using base_type = FunctorBase< typename XE::FunctionTraits< std::decay_t< F > >::function_type >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type && func )
		:_Func( std::forward< functor_type >( func ) )
	{}

	~Functor() = default;

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v< void, result_type > )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( _Func, std::forward< decltype( args ) >( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( _Func, std::forward< decltype( args ) >( args )... );
			}, std::move( args ) );
		}
	}

private:
	mutable functor_type _Func;
};
template< typename R, typename ... T > class Functor< R( * )( T... ) > : public FunctorBase< R( T... ) >
{
public:
	using functor_type = R( * )( T... );
	using base_type = FunctorBase< R( T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

	~Functor() = default;

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v< void, result_type > )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( _Func, std::forward< decltype( args ) >( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( _Func, std::forward< decltype( args ) >( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) > : public FunctorBase< R( O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... );
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) const > : public FunctorBase< R( const O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... ) const;
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) volatile > : public FunctorBase< R( O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... ) volatile;
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) noexcept > : public FunctorBase< R( O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... ) noexcept;
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) const volatile > : public FunctorBase< R( O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... ) const volatile;
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) const noexcept > : public FunctorBase< R( O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... ) const noexcept;
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) const volatile noexcept > : public FunctorBase< R( O *, T... ) >
{
public:
	using functor_type = R( O:: * )( T... ) const volatile noexcept;
	using base_type = FunctorBase< R( O *, T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func )
		:_Func( func )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	functor_type _Func;
};

template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ), O * > : public FunctorBase< R( T... ) >
{
public:
	using object_type = O *;
	using functor_type = R( O:: * )( T... );
	using base_type = FunctorBase< R( T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func, object_type obj )
		:_Func( func ), _Obj( obj )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), _Obj, std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), _Obj, std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	object_type _Obj;
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) const, const O * > : public FunctorBase< R( T... ) >
{
public:
	using object_type = const O *;
	using functor_type = R( O:: * )( T... ) const;
	using base_type = FunctorBase< R( T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func, object_type obj )
		:_Func( func ), _Obj( obj )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), _Obj, std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), _Obj, std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	object_type _Obj;
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ), XE::SharedPtr< O > > : public FunctorBase< R( T... ) >
{
public:
	using object_type = XE::SharedPtr< O >;
	using functor_type = R( O:: * )( T... );
	using base_type = FunctorBase< R( T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func, object_type obj )
		:_Func( func ), _Obj( obj )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), _Obj.get(), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), _Obj.get(), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	object_type _Obj;
	functor_type _Func;
};
template< typename O, typename R, typename ... T > class Functor< R( O:: * )( T... ) const, XE::SharedPtr< const O > > : public FunctorBase< R( T... ) >
{
public:
	using object_type = XE::SharedPtr< const O >;
	using functor_type = R( O:: * )( T... ) const;
	using base_type = FunctorBase< R( T... ) >;
	using result_type = typename base_type::result_type;
	using arguments_tuple_type = typename base_type::arguments_tuple_type;

public:
	Functor( functor_type func, object_type obj )
		:_Func( func ), _Obj( obj )
	{}

public:
	result_type Invoke( typename arguments_tuple_type && args ) const override
	{
		if constexpr( std::is_same_v<void, result_type> )
		{
			std::apply( [this]( auto &&... args )
			{
				std::invoke( std::mem_fn( _Func ), _Obj.get(), std::move( args )... );
			}, std::move( args ) );
		}
		else
		{
			return std::apply( [this]( auto &&... args )
			{
				return std::invoke( std::mem_fn( _Func ), _Obj.get(), std::move( args )... );
			}, std::move( args ) );
		}
	}

private:
	object_type _Obj;
	functor_type _Func;
};

template< typename F, typename ... T > auto MakeFunctor( FunctorPODData * pod, std::pmr::memory_resource * resource, F && val, T &&... args )
{
	void * p = nullptr;
	if constexpr( std::disjunction_v< std::is_trivial< F >, std::is_trivial< T >... > && sizeof( Functor< F, T... > ) <= sizeof( FunctorPODData ) )
	{
		p = pod;
	}
	else
	{
		p = resource->allocate( sizeof( Functor< F, T... > ) );
	}

	return ( new( p ) Functor< F, T... >( std::move( val ), std::move( args )... ) );
}

template< typename R, typename ... T > class Delegate< R( T... ) >
{
public:
	using poddata_type = FunctorPODData;
	using functor_type = FunctorBase< R( T... ) > *;
	using allocator_type = std::pmr::polymorphic_allocator< FunctorBase< R( T... ) > >;
	using result_type = R;
	static constexpr XE::uint64 argument_size = sizeof...( T );
	template< XE::uint64 N > using argument_type = typename std::tuple_element< N, std::tuple< T... > >::type;

public:
	Delegate( const allocator_type & _Alloc = {} )
		:_Alloc( _Alloc )
	{}

	Delegate( std::nullptr_t, const allocator_type & _Alloc = {} )
		:_Alloc( _Alloc )
	{}

	Delegate( Delegate && _Right )
	{
		swap( std::forward< Delegate & >( _Right ) );
	}

	Delegate( const Delegate & _Right )
		:_POD( _Right._POD ), _Alloc( _Right._Alloc ), _Functor( _Right._Functor )
	{
		if ( _Functor == (const functor_type)( &_Right._POD ) )
		{
			_Functor = (functor_type)( &_POD );
		}
		else
		{
			_Functor->Inc();
		}
	}

	template< typename F, typename ... U, std::enable_if_t< std::is_invocable_r_v<R, std::decay_t<F>, U..., T... >, int > = 0 > Delegate( F _Fx, U ... _Ux )
	{
		_Functor = MakeFunctor( &_POD, _Alloc.resource(), std::move( _Fx ), std::move( _Ux )... );

		if ( _Functor != (functor_type)( &_POD ) )
		{
			_Functor->Inc();
		}
	}

	template< typename F, typename ... U, std::enable_if_t< std::is_invocable_r_v<R, std::decay_t<F>, U..., T... >, int > = 0 > Delegate( std::allocator_arg_t, const allocator_type & _Ax, F _Fx, U ... _Ux )
		: _Alloc( _Ax )
	{
		_Functor = MakeFunctor( &_POD, _Alloc.resource(), std::move( _Fx ), std::move( _Ux )... );

		if ( _Functor != (functor_type)( &_POD ) )
		{
			_Functor->Inc();
		}
	}

	~Delegate()
	{
		reset();
	}

public:
	Delegate & operator= ( std::nullptr_t )
	{
		reset();

		_POD = {};
		_Functor = nullptr;

		return *this;
	}

	Delegate & operator= ( Delegate && _Right )
	{
		swap( std::forward< Delegate & >( _Right ) );

		return *this;
	}

	Delegate & operator= ( const Delegate & _Right )
	{
		reset();

		_POD = _Right._POD;
		_Functor = _Right._Functor;

		if( _Functor == (const functor_type)( &_Right._POD ) )
		{
			_Functor = (functor_type)( &_POD );
		}
		else
		{
			_Functor->Inc();
		}

		return *this;
	}

public:
	operator bool() const
	{
		return _Functor != nullptr;
	}

	bool operator ==( std::nullptr_t ) const
	{
		return _Functor == nullptr;
	}

	bool operator !=( std::nullptr_t ) const
	{
		return _Functor != nullptr;
	}

	result_type operator()( T ... args ) const
	{
		std::tuple< T... > tuple( std::forward< T >( args )... );

		if constexpr( std::is_same_v< void, result_type > )
		{
			_Functor->Invoke( std::move( tuple ) );
		}
		else
		{
			return _Functor->Invoke( std::move( tuple ) );
		}
	}

public:
	void reset()
	{
		if ( _Functor != nullptr && _Functor != (functor_type)( &_POD ) && _Functor->Dec() <= 0 )
		{
			_Alloc.destroy( _Functor );
			_Alloc.deallocate( _Functor, 1 );
		}
	}

	void swap( Delegate & _Right )
	{
		std::swap( _POD, _Right._POD );
		std::swap( _Alloc, _Right._Alloc );
		std::swap( _Functor, _Right._Functor );

		if( _Functor == (const functor_type)( &_Right._POD ) )
		{
			_Functor = (functor_type)( &_POD );
		}
		if ( _Right._Functor == (const functor_type)( &_POD ) )
		{
			_Right._Functor = (functor_type)( &_Right._POD );
		}
	}

	allocator_type get_allocator() const
	{
		return _Alloc;
	}

private:
	poddata_type _POD;
	functor_type _Functor = nullptr;
	allocator_type _Alloc = std::pmr::get_default_resource();
};


template< typename R, typename ... Types > struct XE::FunctionTraits< XE::Delegate< R( Types... ) > > : public XE::FunctionTraits< R( Types... ) > {};

END_XE_NAMESPACE

#endif//DELEGATE_HPP__C1E38B56_5986_4410_A7F5_E846D406BA60
