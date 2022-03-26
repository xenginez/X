/*!
 * \file	ConcurrentLRUCache.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/02/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTLRUCACHE_HPP__D5EF18E0_1895_42C8_BF3A_E46D34A787FE
#define CONCURRENTLRUCACHE_HPP__D5EF18E0_1895_42C8_BF3A_E46D34A787FE

#include <shared_mutex>

#include "NonCopyable.hpp"

BEG_XE_NAMESPACE

template< typename K, typename V, typename L = std::less< K >, typename A = std::pmr::polymorphic_allocator< std::pair< const K, V > > >
class ConcurrentLruCache : public XE::NonCopyable
{
public:
	struct map_value_type;

public:
	using self_type = ConcurrentLruCache;
	using key_type = K;
	using mapped_type = V;
	using less_type = L;
	using value_type = XE::Pair< const K, V >;
	using allocator_type = A;
	using size_type = XE::uint64;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using allocator_traits = std::allocator_traits< allocator_type >;
	using map_storage_type = XE::Map< key_type, map_value_type >;
	using lru_list_type = XE::List< typename map_storage_type::iterator >;

public:
	struct map_value_type
	{
		using value_type = V;

		XE::int64 count;
		value_type value;
	};
	struct handle
	{
		friend class ConcurrentLruCache;

	public:
		using iterator = typename map_storage_type::iterator;

	private:
		handle( self_type * cache, iterator iter )
			:_Iter( iter ), _Cache( cache )
		{
			_Cache->inc( _Iter );
		}

	public:
		handle( handle && val )
		{
			swap( std::forward< handle >( val ) );
		}

		handle( const handle & val )
			:_Iter( val._Iter ), _Cache( val._Cache )
		{
			_Cache->inc( _Iter );
		}

		handle & operator= ( handle && val )
		{
			swap( std::forward< handle >( val ) );

			return *this;
		}

		handle & operator= ( const handle & val )
		{
			if( _Cache != nullptr && _Iter != _Cache->_Map.end() )
			{
				_Cache->dec( _Iter );
			}

			_Iter = val._Iter;
			_Cache = val._Cache;

			_Cache->inc( _Iter );

			return *this;
		}

		~handle()
		{
			if( _Cache != nullptr )
			{
				_Cache->dec( _Iter );
			}
		}

	public:
		operator bool() const
		{
			return _Cache != nullptr && _Iter != _Cache->_Map.end();
		}

	public:
		void swap( handle & val )
		{
			std::swap( _Iter, val._Iter );
			std::swap( _Cache, val._Cache );
		}

		typename iterator::second_type::value_type & value() const
		{
			return _Iter->second.value;
		}

	private:
		iterator _Iter;
		self_type * _Cache = nullptr;
	};

public:
	ConcurrentLruCache( XE::uint64 max_size, const allocator_type & alloc = allocator_type() )
		:_MaxSize( max_size ), _Map( alloc.resource() ), _List( alloc.resource() )
	{

	}

public:
	void clear()
	{
		std::unique_lock< std::shared_mutex > lock( _mutex );

		_Map.clear();
		_List.clear();
	}

	handle find( const key_type & key )
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		return { this, _Map.find( key ) };
	}

	void insert( value_type && val )
	{
		std::unique_lock< std::shared_mutex > lock( _mutex );

		_Map.insert( { val.first, { 0, val.second } } );
	}

	void resize( XE::uint64 max_size )
	{
		_MaxSize = max_size;
	}

private:
	friend handle;

	void inc( typename map_storage_type::iterator iter )
	{
		iter->second.count++;
	}

	void dec( typename map_storage_type::iterator iter )
	{
		if( ( --iter->second.count ) <= 0 )
		{
			std::unique_lock< std::shared_mutex > lock( _mutex );

			if( _List.size() >= _MaxSize )
			{
				_Map.erase( _List.pop_back() );
			}

			_List.push_front( iter );
		}
	}

private:
	XE::uint64 _MaxSize;
	lru_list_type _List;
	map_storage_type _Map;
	std::shared_mutex _mutex;
};

END_XE_NAMESPACE

#endif//CONCURRENTLRUCACHE_HPP__D5EF18E0_1895_42C8_BF3A_E46D34A787FE
