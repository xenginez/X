/*!
 * \file	ConcurrentQueue.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/01/04
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTQUEUE_HPP__6063D922_4E58_47CE_897D_E3E790DA8D23
#define CONCURRENTQUEUE_HPP__6063D922_4E58_47CE_897D_E3E790DA8D23

#include <atomic>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T, typename A = std::pmr::polymorphic_allocator< T > > class ConcurrentQueue
{
public:
	using size_type = XE::uint64;
	using value_type = T;
	using allocator_type = A;
	using reference_type = T &;
	using const_reference_type = const T &;
	using difference_type = std::ptrdiff_t;
	using allocator_traits = std::allocator_traits< allocator_type >;

private:
	struct node
	{
		T value;
		std::atomic < node * > next = nullptr;
	};

	using node_allocator_type = typename std::allocator_traits< allocator_type >::template rebind_alloc< node >;
	using node_allocator_traits = std::allocator_traits< node_allocator_type >;

public:
	ConcurrentQueue( const allocator_type & alloc = allocator_type() )
		:_Alloc( alloc.resource() )
	{
		node * p = node_allocator_traits::allocate( _Alloc, 1 ); node_allocator_traits::construct( _Alloc, p );

		_Head = p;
		_Tail = p;
	}

	~ConcurrentQueue()
	{
		node * head = _Head.load();

		while ( !_Head.compare_exchange_weak( head, nullptr ) );

		while( head != nullptr )
		{
			auto next = head->next.load();

			node_allocator_traits::destroy( _Alloc, head );
			node_allocator_traits::deallocate( _Alloc, head, 1 );

			head = next;
		}
	}

	ConcurrentQueue( ConcurrentQueue && ) = delete;

	ConcurrentQueue( const ConcurrentQueue & ) = delete;

public:
	void push( const T & val )
	{
		node * t = nullptr;
		node * q = node_allocator_traits::allocate( _Alloc, 1 );
		node_allocator_traits::construct( _Alloc, q );

		q->value = val;

		while( true )
		{
			if( t == nullptr )
			{
				t = _Tail.load();
				continue;
			}

			if( !_Tail.compare_exchange_weak( t, nullptr ) )
			{
				continue;
			}

			break;
		}

		t->next.store( q );
		++_Size;

		node * expected = nullptr;
		_Tail.compare_exchange_weak( expected, t->next );
	}

	bool try_pop( T & val )
	{
		node * head = nullptr, * head_next = nullptr;
		while( true )
		{
			if( head == nullptr )
			{
				head = _Head.load();
				continue;
			}

			if( !_Head.compare_exchange_weak( head, NULL ) )
			{
				continue;
			}

			head_next = head->next.load();

			if( head_next != nullptr )
			{
				while( _Size.load() == 0 )
				{
					std::this_thread::yield();
				}
			}

			break;
		}

		node * expected = nullptr;
		node * desired = head;

		if( head_next != nullptr )
		{
			val = head_next->value;

			node_allocator_traits::destroy( _Alloc, head );
			node_allocator_traits::deallocate( _Alloc, head, 1 );

			desired = head_next;
			--_Size;
		}

		_Head.compare_exchange_weak( expected, desired );

		return head_next != nullptr;
	}

	void clear()
	{
		node * head = _Head.load();
		node * p = node_allocator_traits::allocate( _Alloc, 1 ); node_allocator_traits::construct( _Alloc, p );

		while ( !_Head.compare_exchange_weak( head, p ) );

		while ( head != nullptr )
		{
			node * next = head->next.load();

			node_allocator_traits::destroy( _Alloc, head );
			node_allocator_traits::deallocate( _Alloc, head, 1 );

			head = next;
		}
	}

public:
	bool empty() const
	{
		return _Size == 0;
	}

	XE::uint64 unsafe_size() const
	{
		return _Size.load();
	}

private:
	node_allocator_type _Alloc;
	std::atomic< node * > _Head;
	std::atomic< node * > _Tail;
	std::atomic < size_type > _Size = 0;
};

END_XE_NAMESPACE

#endif//CONCURRENTQUEUE_HPP__6063D922_4E58_47CE_897D_E3E790DA8D23
