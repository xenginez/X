/*!
 * \file	BuddyAllocator.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/12/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef BUDDYALLOCATOR_HPP__F8E5B605_088A_4A08_8E5E_F8B359744163
#define BUDDYALLOCATOR_HPP__F8E5B605_088A_4A08_8E5E_F8B359744163

#include "Type.h"

BEG_XE_NAMESPACE

/*
* order map
*              [0] [1]     [2] [3]      [4] [5]     [6] [7]     ->order 0
*               |___|       |___|        |___|       |___|
*                 |           |	           |           |
*               [0,1]       [2,3]        [4,5]       [6,7]      ->order 1
*                 |___________|            |___________|
* 				        |                       |
*                   [0,1,2,3]               [4,5,6,7]           ->order 2
*                       |_______________________|
* 				                    |
*                           [0,1,2,3,4,5,6,7]                   ->order 3
*/
template< XE::uint64 MAX_SIZE, XE::uint64 BLOCK_SIZE, std::enable_if_t< ( ( MAX_SIZE & ( MAX_SIZE - 1 ) ) + ( BLOCK_SIZE & ( BLOCK_SIZE - 1 ) ) ) == 0, int > = 0 > class BuddyAllocator
{
private:
	static XE::uint64 constexpr log2( XE::uint64 val )
	{
		XE::uint64 count = 0;

		if( val == 1 )
			return 0;

		return 1 + log2( val >> 1 );
	}

	static XE::uint64 constexpr pow2( XE::uint64 val )
	{
		XE::uint64 r = 1;
		for( ; val > 0; val-- ) r += r;
		return r;
	}

public:
	using size_type = XE::uint64;
	using address_type = XE::uint64;
	static constexpr XE::uint64 max_size = MAX_SIZE;
	static constexpr XE::uint64 block_size = BLOCK_SIZE;
	static constexpr XE::uint64 block_count = MAX_SIZE / BLOCK_SIZE;
	static constexpr XE::uint64 order_count = log2( max_size ) - log2( block_size ) + 1;

public:
	BuddyAllocator()
	{
		_Map.resize( block_count );
		_Orders.back().push_back( 0 );
	}

	BuddyAllocator( BuddyAllocator< MAX_SIZE, BLOCK_SIZE > && val )
	{
		std::swap( _Map, val._Map );
		std::swap( _Orders, val._Orders );
	}

	BuddyAllocator( const BuddyAllocator< MAX_SIZE, BLOCK_SIZE > & val )
		:_Orders( val._Orders ), _Map( val._Map )
	{

	}

public:
	BuddyAllocator & operator =( BuddyAllocator< MAX_SIZE, BLOCK_SIZE > && val )
	{
		std::swap( _Map, val._Map );
		std::swap( _Orders, val._Orders );

		return *this;
	}

	BuddyAllocator & operator =( const BuddyAllocator< MAX_SIZE, BLOCK_SIZE > & val )
	{
		_Map = val._Map;
		_Orders = val._Orders;

		return *this;
	}

public:
	void swap( XE::BuddyAllocator< MAX_SIZE, BLOCK_SIZE > & val )
	{
		std::swap( _Map, val._Map );
		std::swap( _Orders, val._Orders );
	}

public:
	bool HasAllocate( size_type val ) const
	{
		auto size = ALIGNED( val, block_size );
		auto order_index = log2( size ) - log2( block_size );

		while( order_index < order_count )
		{
			if( !_Orders[order_index].empty() )
			{
				return true;
			}
		}

		return false;
	}

	address_type Allocate( size_type val )
	{
		auto size = ALIGNED( val, block_size );
		auto order_index = log2( size ) - log2( block_size );

		auto n = order_index;

		while( _Orders[n].empty() ) n++;

		while( n != order_index )
		{
			address_type node = _Orders[n].front();
			_Orders[n].pop_front();

			_Orders[n - 1].push_back( node );
			_Orders[n - 1].push_back( node + ( ( n + 1 ) * block_size ) / 2 );

			n--;
		}

		address_type node = _Orders[n].front();
		_Orders[n].pop_front();

		_Map[node / block_size] = true;

		return node;
	}

	void Deallocate( address_type val )
	{
		auto block_index = val / block_size;

		XE::uint64 n = 0;

		if( _Map[block_index % 2 == 0 ? block_index + 1 : block_index - 1] == false )
		{
			auto it = std::find( _Orders[n].begin(), _Orders[n].end(), block_index % 2 == 0 ? block_index + 1 : block_index - 1 );
			if( it != _Orders[n].end() )
			{
				_Orders[n].erase( it );
			}

			n++;

			for( ; n < order_count; n++ )
			{
				if( _Map[block_index % 2 == 0 ? block_index + ( n * 2 ) : block_index - ( n * 2 )] == false )
				{
					auto it = std::find( _Orders[n].begin(), _Orders[n].end(), block_index % 2 == 0 ? block_index + ( n * 2 ) : block_index - ( n * 2 ) );
					if( it != _Orders[n].end() )
					{
						_Orders[n].erase( it );
						n++;
					}
				}
				else
				{
					break;
				}
			}
		}
		
		_Map[block_index] = false;
		_Orders[n].push_back( val );
	}

private:
	XE::Array< bool > _Map;
	std::array< XE::List< address_type >, order_count > _Orders;
};

END_XE_NAMESPACE

namespace std
{
	template< XE::uint64 MAX_SIZE, XE::uint64 BLOCK_SIZE > void swap( XE::BuddyAllocator< MAX_SIZE, BLOCK_SIZE > & left, XE::BuddyAllocator< MAX_SIZE, BLOCK_SIZE > & right )
	{
		left.swap( right );
	}
}

#endif//BUDDYALLOCATOR_HPP__F8E5B605_088A_4A08_8E5E_F8B359744163
