/*!
 * \file	ConcurrentHashSet.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/01/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTHASHSET_HPP__B92C270B_9FAF_43C3_98EF_F6149ED6B414
#define CONCURRENTHASHSET_HPP__B92C270B_9FAF_43C3_98EF_F6149ED6B414

#include <shared_mutex>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T, typename H = XE::Hasher< T >, typename E = std::equal_to< T >, typename A = std::pmr::polymorphic_allocator< T > > class ConcurrentHashSet
{
private:
	class HashSetBucket
	{
		friend class ConcurrentHashSet;

	public:
		using value_type = T;
		using equal_type = E;
		using size_type = XE::uint64;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type *;
		using const_pointer = const value_type *;
		using reference = value_type &;
		using const_reference = const value_type &;
		using list_type = XE::List< value_type >;

	public:
		HashSetBucket( const std::pmr::polymorphic_allocator< value_type > & alloc = {} )
			:_list( alloc )
		{

		}

	public:
		void insert( value_type & val )
		{
			std::unique_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), _equal );
			if( it != _list.end() )
			{
				*it = val;
			}
			else
			{
				_list.push_back( val );
			}
		}

		void erase( const value_type & val )
		{
			std::unique_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), _equal );
			if( it != _list.end() )
			{
				_list.erase( it );
			}
		}

		bool find( value_type & val ) const
		{
			std::shared_lock< std::shared_mutex > lock( _mutex );

			auto it = std::find_if( _list.begin(), _list.end(), _equal );
			if( it != _list.end() )
			{
				val = *it;
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
		equal_type _equal;
		std::shared_mutex _mutex;
	};

public:
	using hasher = H;
	using value_type = T;
	using equal_type = E;
	using bucket_type = HashSetBucket;
	using allocator_type = A;
	using size_type = XE::uint64;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using reference = value_type &;
	using const_reference = const value_type &;
	using vector_type = XE::Array< bucket_type >;

public:
	ConcurrentHashSet( size_type bucket_size = 61, const allocator_type & alloc = allocator_type() )
		:_buckets( bucket_size, alloc.resource() )
	{

	}

public:
	void insert( value_type & val )
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		_buckets[_hash( val ) & _buckets.size()].insert( val );
	}

	void erase( const value_type & val )
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		_buckets[_hash( val ) & _buckets.size()].erase( val );
	}

	bool find( value_type & val ) const
	{
		std::shared_lock< std::shared_mutex > lock( _mutex );

		return _buckets[_hash( val ) & _buckets.size()].find( val );
	}

	void clear()
	{
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
				new_buckets[_hash( it ) & new_buckets.size()].emplace_back( std::move( it ) );
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

#endif//CONCURRENTHASHSET_HPP__B92C270B_9FAF_43C3_98EF_F6149ED6B414
