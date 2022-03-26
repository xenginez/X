/*!
 * \file	Promise.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/02/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PROMISE_HPP__A7662DC0_41C8_42B8_AE41_F0D05E2B3E49
#define PROMISE_HPP__A7662DC0_41C8_42B8_AE41_F0D05E2B3E49

#include <any>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T > struct IsPromise : public std::false_type {};
template< typename T > struct IsPromise< XE::Promise< T > > : public std::true_type {};
template< typename T > inline constexpr bool IsPromiseValue = IsPromise< T >::value;


template< typename T > class Promise final
{
public:
	using value_type = T;

public:
	Promise()
		:_State( XE::MakeShared< State >() )
	{

	}

	Promise( Promise && val )
	{
		std::swap( _State, val._State );
	}

	Promise( const Promise & val )
		:_State( val._State )
	{

	}

	~Promise() = default;

	Promise & operator=( Promise && val )
	{
		std::swap( _State, val._State );
		return *this;
	}

	Promise & operator=( const Promise & val )
	{
		_State = val._State;

		return *this;
	}

public:
	void Resolve( T && val )
	{
		_State->Resolve( std::forward< T >( val ) );
	}

	template< typename U > void Reject( U && val )
	{
		_State->Reject( std::forward< U >( val ) );
	}

public:
	template < typename FulfilledF, typename FulfilledR = std::invoke_result_t< FulfilledF, T >, std::enable_if_t< IsPromiseValue< FulfilledR >, int > = 0 >
	Promise< typename FulfilledR::value_type > Then( FulfilledF && on_fulfilled )
	{
		Promise< typename FulfilledR::value_type > next;

		Then( [n = next, f = std::forward< FulfilledF >( on_fulfilled )]( auto && arg ) mutable
		{
			auto np = std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( arg )>( arg ) );

			np.Then( [n]( auto && next_arg ) mutable
			{
				n.Resolve( std::forward<decltype( next_arg )>( next_arg ) );
			} );

			np.Except( [n]( const std::any & e ) mutable
			{
				n.Reject( e );
			} );
		} )
			.Except( [n = next]( const std::any & e ) mutable
		{
			n.Reject( e );
		} );

		return next;
	}

	template < typename FulfilledF, typename FulfilledR = std::invoke_result_t< FulfilledF, T >, std::enable_if_t< !IsPromiseValue< FulfilledR >, int > = 0 >
	Promise< FulfilledR > Then( FulfilledF && on_fulfilled )
	{
		Promise< FulfilledR > next;

		_State->Add( next, std::forward< FulfilledF >( on_fulfilled ), []( const std::any & e ) -> FulfilledR { throw e; }, false );

		return next;
	}

	template < typename FulfilledF, typename RejectedF, typename FulfilledR = std::invoke_result_t< FulfilledF, T >, std::enable_if_t< !IsPromiseValue< FulfilledR >, int > = 0 >
	Promise< FulfilledR > Then( FulfilledF && on_fulfilled, RejectedF && on_rejected )
	{
		Promise< FulfilledR > next;

		_State->Add( next, std::forward< FulfilledF >( on_fulfilled ), std::forward< RejectedF >( on_rejected ), true );

		return next;
	}

public:
	template < typename FulfilledF > auto ThenAll( FulfilledF && on_fulfilled )
	{
		return Then( [f = std::forward<FulfilledF>( on_fulfilled )]( auto && v ) mutable
		{
			return MakeAllPromise( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( v )>( v ) ) );
		} );
	}

	template < typename FulfilledF > auto ThenAny( FulfilledF && on_fulfilled )
	{
		return then( [f = std::forward<FulfilledF>( on_fulfilled )]( auto && v ) mutable
		{
			return MakeAnyPromise( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( v )>( v ) ) );
		} );
	}

	template < typename FulfilledF > auto ThenRace( FulfilledF && on_fulfilled )
	{
		return then( [f = std::forward<FulfilledF>( on_fulfilled )]( auto && v ) mutable
		{
			return MakeRacePromise( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( v )>( v ) ) );
		} );
	}

public:
	template < typename RejectedF > Promise< T > Except( RejectedF && on_rejected )
	{
		return Then( []( auto && arg ) { return std::forward<decltype( arg )>( arg ); }, std::forward<RejectedF>( on_rejected ) );
	}

	template < typename FinallyF > Promise< T > Finally( FinallyF && on_finally )
	{
		return Then(
			[on_finally]( auto && arg ) { std::invoke( std::move( on_finally ) ); return std::forward<decltype( arg )>( arg ); },
			[on_finally]( const std::any & e ) { std::invoke( std::move( on_finally ) ); std::rethrow_exception( e ); } );
	}

private:
	struct State : public XE::NonCopyable
	{
		struct Handler
		{
			XE::Delegate< void( const T & ) > fulfilled;
			XE::Delegate< void( const std::any & ) > rejected;
		};

		template < typename U > bool Resolve( U && value )
		{
			std::unique_lock< std::mutex > lock( _Lock );

			if( _Status != XE::PromiseStatus::PENDING )
			{
				return false;
			}

			_Fulfull = value;

			_Status = PromiseStatus::FULFILLED;

			InvokeFulfilledHandlers();

			return true;
		}

		template< typename U > bool Reject( U && value ) noexcept
		{
			std::unique_lock< std::mutex > lock( _Lock );

			if( _Status != XE::PromiseStatus::PENDING )
			{
				return false;
			}

			_Reject = value;

			_Status = PromiseStatus::REJECTED;

			InvokeRejectedHandlers();

			return true;
		}

		template < typename U, typename FulfilledF, typename RejectedF >
		void Add( Promise< U > & next, FulfilledF && on_fulfilled, RejectedF && on_rejected, bool has_reject )
		{
			auto fulfilled = [n = next, f = std::forward<FulfilledF>( on_fulfilled )]( const T & arg ) mutable -> void
			{
				try
				{
					if constexpr( std::is_void_v<U> )
					{
						std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( arg )>( arg ) );

						n.Resolve();
					}
					else
					{
						n.Resolve( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( arg )>( arg ) ) );
					}
				}
				catch( ... )
				{
					n.Reject( std::current_exception() );
				}
			};

			auto rejected = [n = next, f = std::forward<RejectedF>( on_rejected ), has_reject]( const std::any & arg ) mutable -> void
			{
				if( has_reject )
				{
					try
					{
						if constexpr( std::is_void_v<U> )
						{
							std::invoke( std::forward<decltype( f )>( f ), std::any_cast< typename XE::FunctionTraits< decltype( f ) >::args< 0 >::type >( arg ) );

							n.Resolve();
						}
						else
						{
							n.Resolve( std::invoke( std::forward<decltype( f )>( f ), std::any_cast< typename XE::FunctionTraits< decltype( f ) >::args< 0 >::type >( arg ) ) );
						}
					}
					catch( ... )
					{
						n.Reject( std::current_exception() );
					}
				}
				else
				{
					n.Reject( arg );
				}
			};

			AddHandlers( std::move( fulfilled ), std::move( rejected ) );
		}

		template < typename FulfilledF, typename RejectedF > void AddHandlers( FulfilledF && fulfilled, RejectedF && rejected )
		{
			std::unique_lock< std::mutex > lock( _Lock );

			switch( _Status )
			{
			case XE::PromiseStatus::PENDING:
				_Handlers.push_back( Handler{ std::forward< FulfilledF >( fulfilled ), std::forward< RejectedF >( rejected ) } );
				break;
			case XE::PromiseStatus::FULFILLED:
				std::invoke( fulfilled, _Fulfull );
				break;
			case XE::PromiseStatus::REJECTED:
				std::invoke( rejected, _Reject );
				break;
			}
		}

		void InvokeFulfilledHandlers() noexcept
		{
			for( const auto & h : _Handlers )
			{
				h.fulfilled( _Fulfull );
			}
			_Handlers.clear();
		}

		void InvokeRejectedHandlers() noexcept
		{
			for( const auto & h : _Handlers )
			{
				h.rejected( _Reject );
			}
			_Handlers.clear();
		}

		std::mutex _Lock;
		std::any _Reject;
		T _Fulfull;
		XE::Array< Handler > _Handlers;
		XE::PromiseStatus _Status = XE::PromiseStatus::PENDING;
	};

private:
	XE::SharedPtr< State > _State;
};

template<> class Promise< void > final
{
public:
	using value_type = void;

public:
	Promise()
		:_State( XE::MakeShared< State >() )
	{}

	Promise( Promise && val )
	{
		std::swap( _State, val._State );
	}

	Promise( const Promise & val )
		:_State( val._State )
	{

	}

	~Promise() = default;

	Promise & operator=( Promise && val )
	{
		std::swap( _State, val._State );
		return *this;
	}

	Promise & operator=( const Promise & val )
	{
		_State = val._State;

		return *this;
	}

public:
	void Resolve()
	{
		_State->Resolve();
	}

	template< typename U > void Reject( U && val )
	{
		_State->Reject( std::forward< U >( val ) );
	}

public:
	template < typename FulfilledF, typename FulfilledR = std::invoke_result_t< FulfilledF >, std::enable_if_t< IsPromiseValue< FulfilledR >, int > = 0 >
	Promise< typename FulfilledR::value_type > Then( FulfilledF && on_fulfilled )
	{
		Promise< typename FulfilledR::value_type > next;

		Then( [n = next, f = std::forward< FulfilledF >( on_fulfilled )]( auto && arg ) mutable
		{
			auto np = std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( arg )>( arg ) );

			np.Then( [n]( auto && next_arg ) mutable
			{
				n.Resolve( std::forward<decltype( next_arg )>( next_arg ) );
			} );

			np.Except( [n]( const std::any & e ) mutable
			{
				n.Reject( e );
			} );
		} )
			.Except( [n = next]( const std::any & e ) mutable
		{
			n.Reject( e );
		} );

		return next;
	}

	template < typename FulfilledF, typename FulfilledR = std::invoke_result_t< FulfilledF >, std::enable_if_t< !IsPromiseValue< FulfilledR >, int > = 0 >
	Promise< FulfilledR > Then( FulfilledF && on_fulfilled )
	{
		Promise< FulfilledR > next;

		_State->Add( next, std::forward< FulfilledF >( on_fulfilled ), []( const std::any & e ) -> FulfilledR { throw e; }, false );

		return next;
	}

	template < typename FulfilledF, typename RejectedF, typename FulfilledR = std::invoke_result_t< FulfilledF >, std::enable_if_t< !IsPromiseValue< FulfilledR >, int > = 0 >
	Promise< FulfilledR > Then( FulfilledF && on_fulfilled, RejectedF && on_rejected )
	{
		Promise< FulfilledR > next;

		_State->Add( next, std::forward< FulfilledF >( on_fulfilled ), std::forward< RejectedF >( on_rejected ), true );

		return next;
	}

public:
	template < typename FulfilledF > auto ThenAll( FulfilledF && on_fulfilled )
	{
		return Then( [f = std::forward<FulfilledF>( on_fulfilled )]( auto && v ) mutable
		{
			return MakeAllPromise( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( v )>( v ) ) );
		} );
	}

	template < typename FulfilledF > auto ThenAny( FulfilledF && on_fulfilled )
	{
		return then( [f = std::forward<FulfilledF>( on_fulfilled )]( auto && v ) mutable
		{
			return MakeAnyPromise( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( v )>( v ) ) );
		} );
	}

	template < typename FulfilledF > auto ThenRace( FulfilledF && on_fulfilled )
	{
		return then( [f = std::forward<FulfilledF>( on_fulfilled )]( auto && v ) mutable
		{
			return MakeRacePromise( std::invoke( std::forward<decltype( f )>( f ), std::forward<decltype( v )>( v ) ) );
		} );
	}

public:
	template < typename RejectedF > Promise< void > Except( RejectedF && on_rejected )
	{
		return Then( []( auto && arg ) { return std::forward<decltype( arg )>( arg ); }, std::forward<RejectedF>( on_rejected ) );
	}

	template < typename FinallyF > Promise< void > Finally( FinallyF && on_finally )
	{
		return Then(
			[on_finally]( auto && arg ) { std::invoke( std::move( on_finally ) ); return std::forward<decltype( arg )>( arg ); },
			[on_finally]( const std::any & e ) { std::invoke( std::move( on_finally ) ); std::rethrow_exception( e ); } );
	}

private:
	struct State : public XE::NonCopyable
	{
		struct Handler
		{
			XE::Delegate< void() > fulfilled;
			XE::Delegate< void( const std::any & ) > rejected;
		};

		bool Resolve()
		{
			std::unique_lock< std::mutex > lock( _Lock );

			if( _Status != XE::PromiseStatus::PENDING )
			{
				return false;
			}

			_Status = PromiseStatus::FULFILLED;

			InvokeFulfilledHandlers();

			return true;
		}

		template< typename U > bool Reject( U && value ) noexcept
		{
			std::unique_lock< std::mutex > lock( _Lock );

			if( _Status != XE::PromiseStatus::PENDING )
			{
				return false;
			}

			_Reject = value;

			_Status = PromiseStatus::REJECTED;

			InvokeRejectedHandlers();

			return true;
		}

		template < typename U, typename FulfilledF, typename RejectedF >
		void Add( Promise< U > & next, FulfilledF && on_fulfilled, RejectedF && on_rejected, bool has_reject )
		{
			auto fulfilled = [n = next, f = std::forward<FulfilledF>( on_fulfilled )]() mutable -> void
			{
				try
				{
					if constexpr( std::is_void_v< U > )
					{
						std::invoke( std::forward<decltype( f )>( f ) );

						n.Resolve();
					}
					else
					{
						n.Resolve( std::invoke( std::forward<decltype( f )>( f ) ) );
					}
				}
				catch( ... )
				{
					n.Reject( std::current_exception() );
				}
			};

			auto rejected = [n = next, f = std::forward<RejectedF>( on_rejected ), has_reject]( const std::any & arg ) mutable -> void
			{
				if( has_reject )
				{
					try
					{
						if constexpr( std::is_void_v< U > )
						{
							std::invoke( std::forward<decltype( f )>( f ), std::any_cast< typename XE::FunctionTraits< decltype( f ) >::args< 0 >::type >( arg ) );

							n.Resolve();
						}
						else
						{
							n.Resolve( std::invoke( std::forward<decltype( f )>( f ), std::any_cast< typename XE::FunctionTraits< decltype( f ) >::args< 0 >::type >( arg ) ) );
						}
					}
					catch( ... )
					{
						n.Reject( std::current_exception() );
					}
				}
				else
				{
					n.Reject( arg );
				}
			};

			AddHandlers( std::move( fulfilled ), std::move( rejected ) );
		}

		template < typename FulfilledF, typename RejectedF > void AddHandlers( FulfilledF && fulfilled, RejectedF && rejected )
		{
			std::unique_lock< std::mutex > lock( _Lock );

			switch( _Status )
			{
			case XE::PromiseStatus::PENDING:
				_Handlers.push_back( Handler{ std::forward< FulfilledF >( fulfilled ), std::forward< RejectedF >( rejected ) } );
				break;
			case XE::PromiseStatus::FULFILLED:
				std::invoke( fulfilled );
				break;
			case XE::PromiseStatus::REJECTED:
				std::invoke( rejected, _Reject );
				break;
			}
		}


		void InvokeFulfilledHandlers() noexcept
		{
			for( const auto & h : _Handlers )
			{
				h.fulfilled();
			}
			_Handlers.clear();
		}

		void InvokeRejectedHandlers() noexcept
		{
			for( const auto & h : _Handlers )
			{
				h.rejected( _Reject );
			}
			_Handlers.clear();
		}

		std::mutex _Lock;
		std::any _Reject;
		XE::Array< Handler > _Handlers;
		XE::PromiseStatus _Status = XE::PromiseStatus::PENDING;
	};

private:
	XE::SharedPtr< State > _State;
};


template < typename R > Promise< R > MakePromise()
{
	return Promise<R>();
}

template < typename R, typename F > Promise< R > MakePromise( F && f )
{
	Promise<R> result;

	if constexpr( std::is_void_v< R > )
	{
		auto resolver = [result]() mutable
		{
			return result.Resolve();
		};

		auto rejector = [result]( auto && e ) mutable
		{
			return result.Reject( std::forward<decltype( e )>( e ) );
		};

		try
		{
			std::invoke( std::forward<F>( f ), std::move( resolver ), std::move( rejector ) );
		}
		catch( ... )
		{
			result.Reject( std::current_exception() );
		}
	}
	else
	{
		auto resolver = [result]( auto && v ) mutable
		{
			return result.Resolve( std::forward<decltype( v )>( v ) );
		};

		auto rejector = [result]( auto && e ) mutable
		{
			return result.Reject( std::forward<decltype( e )>( e ) );
		};

		try
		{
			std::invoke( std::forward<F>( f ), std::move( resolver ), std::move( rejector ) );
		}
		catch( ... )
		{
			result.Reject( std::current_exception() );
		}
	}

	return result;
}

inline Promise< void > MakeResolvedPromise()
{
	Promise<void> result;
	result.Resolve();
	return result;
}

template < typename R > Promise< std::decay_t< R > > MakeResolvedPromise( R && v )
{
	Promise<std::decay_t<R>> result;
	result.Resolve( std::forward<R>( v ) );
	return result;
}

template < typename U > Promise< void > MakeRejectedPromise( U && e )
{
	Promise<void> result;
	result.Reject( std::forward<U>( e ) );
	return result;
}

template < typename R, typename U > Promise< R > MakeRejectedPromise( U && e )
{
	Promise<R> result;
	result.Reject( std::forward<U>( e ) );
	return result;
}

template < typename Iter,
	typename SubPromise = typename std::iterator_traits< Iter >::value_type,
	typename SubPromiseResult = typename SubPromise::value_type,
	typename ResultPromiseValueType = XE::Array< SubPromiseResult > > Promise< ResultPromiseValueType > MakeAllPromise( Iter begin, Iter end )
{
	if( begin == end )
	{
		return MakeResolvedPromise( ResultPromiseValueType() );
	}

	struct context_t
	{
		std::atomic< size_t > success_counter{ 0u };
		std::vector< SubPromiseResult > results;
		context_t( std::size_t count )
			: success_counter( count )
			, results( count )
		{}
	};

	return MakePromise< ResultPromiseValueType >( [begin, end]( auto && resolver, auto && rejector )
	{
		std::size_t result_index = 0;
		auto context = XE::MakeShared<context_t>( std::distance( begin, end ) );
		for( Iter iter = begin; iter != end; ++iter, ++result_index )
		{
			( *iter )
				.Then( [context, resolver, result_index]( auto && v ) mutable
			{
				context->results[result_index] = std::forward<decltype( v )>( v );
				if( !--context->success_counter )
				{
					XE::Array<SubPromiseResult> results;
					results.reserve( context->results.size() );
					for( auto && r : context->results )
					{
						results.push_back( std::move( *r ) );
					}
					resolver( std::move( results ) );
				}
			} )
				.Except( [rejector]( const std::any & e ) mutable
			{
				rejector( e );
			} );
		}
	} );
}

template < typename Container > auto MakeAllPromise( Container && container )
{
	return MakeAllPromise( std::begin( container ), std::end( container ) );
}

template < typename Iter,
	typename SubPromise = typename std::iterator_traits< Iter >::value_type,
	typename SubPromiseResult = typename SubPromise::value_type,
	typename ResultPromiseValueType = SubPromiseResult > Promise< ResultPromiseValueType > MakeAnyPromise( Iter begin, Iter end )
{
	if( begin == end )
	{
		return MakeRejectedPromise<ResultPromiseValueType>( std::out_of_range( "out of range" ) );
	}

	struct context_t
	{
		std::atomic< size_t > failure_counter{ 0u };
		std::vector< std::any > exceptions;
		context_t( std::size_t count )
			: failure_counter( count )
			, exceptions( count )
		{}
	};

	return MakePromise<ResultPromiseValueType>( [begin, end]( auto && resolver, auto && rejector )
	{
		std::size_t exception_index = 0;
		auto context = XE::MakeShared<context_t>( std::distance( begin, end ) );
		for( Iter iter = begin; iter != end; ++iter, ++exception_index )
		{
			( *iter )
				.Then( [resolver]( auto && v ) mutable
			{
				resolver( std::forward<decltype( v )>( v ) );
			} )
				.Except( [context, rejector, exception_index]( const std::any & e ) mutable
			{
				context->exceptions[exception_index] = e;
				if( !--context->failure_counter )
				{
					rejector( std::move( context->exceptions ) );
				}
			} );
		}
	} );
}

template < typename Container > auto MakeAnyPromise( Container && container )
{
	return MakeAnyPromise( std::begin( container ), std::end( container ) );
}

template < typename Iter,
	typename SubPromise = typename std::iterator_traits<Iter>::value_type,
	typename SubPromiseResult = typename SubPromise::value_type,
	typename ResultPromiseValueType = SubPromiseResult > Promise< ResultPromiseValueType > MakeRacePromise( Iter begin, Iter end )
{
	return MakePromise<ResultPromiseValueType>( [begin, end]( auto && resolver, auto && rejector )
	{
		for( Iter iter = begin; iter != end; ++iter )
		{
			( *iter )
				.Then( resolver )
				.Except( rejector );
		}
	} );
}

template < typename Container > auto MakeRacePromise( Container && container )
{
	return MakeRacePromise( std::begin( container ), std::end( container ) );
}

END_XE_NAMESPACE

#endif//PROMISE_HPP__A7662DC0_41C8_42B8_AE41_F0D05E2B3E49
