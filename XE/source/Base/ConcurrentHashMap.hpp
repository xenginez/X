/*!
 * \file	ConcurrentHashMap.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/01/04
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTHASHMAP_HPP__96FBB2FD_5627_4FC8_9884_9FC2F1CE3275
#define CONCURRENTHASHMAP_HPP__96FBB2FD_5627_4FC8_9884_9FC2F1CE3275

#include <shared_mutex>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename K, typename V, typename H = XE::Hasher< K >, typename E = std::equal_to< K >, typename A = std::pmr::polymorphic_allocator< std::pair< const K, V > > > class ConcurrentHashMap
{
private:
	class HashMapBucket
	{
		friend class ConcurrentHashMap;

	public:
		using key_type = K;
		using mapped_type = V;
		using key_equal = E;
		using value_type = XE::Pair< const K, V >;
		using size_type = XE::uint64;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type *;
		using const_pointer = const value_type *;
		using reference = value_type &;
		using const_reference = const value_type &;
		using list_type = XE::List< value_type >;

	public:
		HashMapBucket( const HashMapBucket & val )
			:_list( val._list )
		{

		}

		HashMapBucket( const std::pmr::polymorphic_allocator< value_type > & alloc = {} )
			:_list( alloc )
		{

		}

	public:
		void insert( const key_type & key, const mapped_type & val )
		{
			std::unique_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), [this, key]( const auto & val ) { return _equal( key, val.first ); } );
			if( it != _list.end() )
			{
				it->second = val;
			}
			else
			{
				_list.push_back( { key, val } );
			}
		}

		void erase( const key_type & key )
		{
			std::unique_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), [this, key]( const auto & val ) { return _equal( key, val.first ); } );
			if( it != _list.end() )
			{
				_list.erase( it );
			}
		}

		bool find( const key_type & key, mapped_type & val )
		{
			std::shared_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), [this, key]( const auto & val ) { return _equal( key, val.first ); } );
			if( it != _list.end() )
			{
				val = it->second;
				return true;
			}

			return false;
		}

		bool find( const key_type & key, mapped_type & val ) const
		{
			std::shared_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), _equal );
			if( it != _list.end() )
			{
				val = it->second;
				return true;
			}

			return false;
		}

		void clear()
		{
			std::unique_lock< std::shared_mutex > lock( _mutex );

			_list.clear();
		}

	private:
		list_type _list;
		key_equal _equal;
		std::shared_mutex _mutex;
	};

public:
	using hasher = H;
	using key_type = K;
	using mapped_type = V;
	using key_equal = E;
	using bucket_type = HashMapBucket;
	using value_type = XE::Pair< const K, V >;
	using allocator_type = A;
	using size_type = XE::uint64;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using vector_type = XE::Array< bucket_type >;

public:
	ConcurrentHashMap( size_type bucket_size = 61, const allocator_type & alloc = allocator_type() )
		:_buckets( bucket_size, alloc.resource() )
	{

	}

	ConcurrentHashMap( const ConcurrentHashMap & val )
		:_buckets( val._buckets )
	{

	}

public:
	void insert( const key_type & key, const mapped_type & val )
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		_buckets[_hash( key ) & _buckets.size()].insert( key, val );
	}

	void erase( const key_type & key )
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		_buckets[_hash( key ) & _buckets.size()].erase( key );
	}

	bool find( const key_type & key, mapped_type & val )
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		return _buckets[_hash( key ) & _buckets.size()].find( key, val );
	}

	bool find( const key_type & key, mapped_type & val ) const
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		return _buckets[_hash( key ) & _buckets.size()].find( key, val );
	}

	void clear()
	{
		for( auto & it : _buckets )
		{
			it.clear();
		}

		std::unique_lock< std::shared_mutex > lock( _mutex );

		_buckets.clear();
	}

	void rebucket( size_type bucket_size )
	{
		std::unique_lock< std::shared_mutex > lock( _mutex );

		vector_type new_buckets( bucket_size, _buckets.get_allocator() );

		for( auto & bucket : _buckets )
		{
			for( auto & it : bucket._list )
			{
				new_buckets[_hash( it.first ) & bucket_size].emplace_back( std::move( it ) );
			}
		}

		std::swap( _buckets, new_buckets );
	}

private:
	hasher _hash;
	vector_type _buckets;
	std::shared_mutex _mutex;
};

END_XE_NAMESPACE

#endif//CONCURRENTHASHMAP_HPP__96FBB2FD_5627_4FC8_9884_9FC2F1CE3275
