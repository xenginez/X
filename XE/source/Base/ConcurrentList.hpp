/*!
 * \file	ConcurrentList.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/06/07
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTLIST_HPP__4D6C0B5D_BFB4_4FD7_984E_8CCF2C9130D5
#define CONCURRENTLIST_HPP__4D6C0B5D_BFB4_4FD7_984E_8CCF2C9130D5

#include <atomic>

#include "Type.h"

BEG_XE_NAMESPACE

template <typename _Mynode> class ConcurrentListConstIterator
{
public:
	using iterator_category = std::bidirectional_iterator_tag;

	using _Nodeptr = typename _Mynode::_Nodeptr;
	using value_type = typename _Mynode::value_type;
	using pointer = typename _Mynode::const_pointer;
	using reference = const value_type &;

	ConcurrentListConstIterator()
		:_Ptr( nullptr )
	{
	}

	ConcurrentListConstIterator( _Nodeptr _Ptr )
		:_Ptr( _Ptr )
	{ }

	ConcurrentListConstIterator( const ConcurrentListConstIterator & _Iter )
		:_Ptr( _Iter._Ptr )
	{
	}

	ConcurrentListConstIterator & operator= ( const ConcurrentListConstIterator & _Iter )
	{
		_Ptr = _Iter._Ptr;
		return *this;
	}

	reference operator*() const noexcept
	{
		return *_Ptr->_Myval;
	}

	pointer operator->() const noexcept
	{
		return _Ptr->_Myval;
	}

	ConcurrentListConstIterator & operator++() noexcept
	{
		_Ptr = _Ptr->_Next;
		return *this;
	}

	ConcurrentListConstIterator operator++( int ) noexcept
	{
		ConcurrentListConstIterator _Tmp = *this;
		++ * this;
		return _Tmp;
	}

	ConcurrentListConstIterator & operator--() noexcept
	{
		_Ptr = _Ptr->_Prev;
		return *this;
	}

	ConcurrentListConstIterator operator--( int ) noexcept
	{
		ConcurrentListConstIterator _Tmp = *this;
		-- * this;
		return _Tmp;
	}

	bool operator==( const ConcurrentListConstIterator & _Right ) const noexcept
	{
		return _Ptr == _Right._Ptr;
	}

	bool operator!=( const ConcurrentListConstIterator & _Right ) const noexcept
	{
		return !( *this == _Right );
	}

	_Nodeptr _Ptr;
};

template <typename _Mynode> class ConcurrentListIterator : public ConcurrentListConstIterator<_Mynode>
{
public:
	using _Mybase = ConcurrentListConstIterator<_Mynode>;
	using iterator_category = std::bidirectional_iterator_tag;

	using _Nodeptr = typename _Mynode::_Nodeptr;
	using value_type = typename _Mynode::value_type;
	using pointer = typename _Mynode::pointer;
	using reference = value_type &;

	using _Mybase::_Mybase;

	reference operator*() const noexcept
	{
		return const_cast<reference>( _Mybase::operator*() );
	}

	pointer operator->() const noexcept
	{
		return const_cast<pointer>( _Mybase::operator->() );
	}

	ConcurrentListIterator & operator++() noexcept
	{
		_Mybase::operator++();
		return *this;
	}

	ConcurrentListIterator operator++( int ) noexcept
	{
		ConcurrentListIterator _Tmp = *this;
		_Mybase::operator++();
		return _Tmp;
	}

	ConcurrentListIterator & operator--() noexcept
	{
		_Mybase::operator--();
		return *this;
	}

	ConcurrentListIterator operator--( int ) noexcept
	{
		ConcurrentListIterator _Tmp = *this;
		_Mybase::operator--();
		return _Tmp;
	}
};

template <typename _Value_type> class ConcurrentListNode
{
public:
	using value_type = _Value_type;
	using _Nodeptr = ConcurrentListNode<_Value_type> *;
	using pointer = value_type *;
	using const_pointer = const value_type *;

	ConcurrentListNode() = default;

	pointer _Myval = nullptr;
	std::atomic<_Nodeptr> _Next = nullptr;
	std::atomic<_Nodeptr> _Prev = nullptr;

	ConcurrentListNode( const ConcurrentListNode & ) = delete;
	ConcurrentListNode & operator=( const ConcurrentListNode & ) = delete;
};

template < typename _Ty, typename _Alloc = std::pmr::polymorphic_allocator< _Ty > > class ConcurrentList
{
private:
	using _Alty = typename std::allocator_traits<_Alloc>::template rebind_alloc<_Ty>;
	using _Alty_traits = std::allocator_traits<_Alty>;
	using _Node = ConcurrentListNode<_Ty>;
	using _Alnode = typename std::allocator_traits<_Alloc>::template rebind_alloc<_Node>;
	using _Alnode_traits = std::allocator_traits<_Alnode>;
	using _Nodeptr = typename _Alnode_traits::pointer;

public:
	using value_type = _Ty;
	using allocator_type = _Alloc;
	using size_type = typename _Alty_traits::size_type;
	using pointer = typename _Alty_traits::pointer;
	using const_pointer = typename _Alty_traits::const_pointer;
	using reference = value_type &;
	using const_reference = const value_type &;

	using iterator = ConcurrentListIterator<_Node>;
	using const_iterator = ConcurrentListConstIterator<_Node>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	ConcurrentList( const _Alloc & _Al = {} )
		: _Al( _Al )
	{
		_Construct();
	}

	ConcurrentList( size_type _Count, const _Alloc & _Al = {} )
		: _Al( _Al )
	{
		_Construct();

		assign( _Count, {} );
	}

	ConcurrentList( size_type _Count, const _Ty & _Val, const _Alloc & _Al = {} )
		: _Al( _Al )
	{
		_Construct();

		assign( _Count, _Val );
	}

	ConcurrentList( const ConcurrentList & _Right, const _Alloc & _Al = {} )
		: _Al( _Al )
	{
		_Construct();

		assign( _Right.begin(), _Right.end() );
	}

	template <typename _Iter> ConcurrentList( _Iter _First, _Iter _Last, const _Alloc & _Al = {} )
		: _Al( _Al )
	{
		_Construct();

		assign( _First, _Last );
	}

	ConcurrentList( std::initializer_list<_Ty> _Ilist, const _Alloc & _Al = {} )
		: _Al( _Al )
	{
		_Construct();

		assign( _Ilist.begin(), _Ilist.end() );
	}

	~ConcurrentList() noexcept
	{
		clear();
	}

public:
	ConcurrentList & operator=( const ConcurrentList & _Right )
	{
		if ( this != std::addressof( _Right ) )
		{
			assign( _Right.begin(), _Right.end() );
		}

		return *this;
	}

	ConcurrentList & operator=( std::initializer_list<_Ty> _Ilist )
	{
		assign( _Ilist.begin(), _Ilist.end() );
		return *this;
	}

public:
	iterator begin() noexcept
	{
		return iterator( _Head->_Next.load() );
	}

	const_iterator begin() const noexcept
	{
		return const_iterator( _Head->_Next );
	}

	iterator end() noexcept
	{
		return iterator( _Tail );
	}

	const_iterator end() const noexcept
	{
		return const_iterator( _Tail );
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator( end() );
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator( end() );
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator( begin() );
	}

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator( begin() );
	}

	const_iterator cbegin() const noexcept
	{
		return begin();
	}

	const_iterator cend() const noexcept
	{
		return end();
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return rbegin();
	}

	const_reverse_iterator crend() const noexcept
	{
		return rend();
	}

public:
	size_type size() const noexcept
	{
		return _Size.load();
	}

	size_type max_size() const noexcept
	{
		return std::numeric_limits<size_type>::max();
	}

	bool empty() const noexcept
	{
		return _Head._Next.load() == nullptr;
	}

	allocator_type get_allocator() const noexcept
	{
		return { _Al.resource() };
	}

public:
	reference front() noexcept
	{
		return *_Head->_Next.load()->_Myval;
	}

	const_reference front() const noexcept
	{
		return *_Head->_Next.load()->_Myval;
	}

	reference back() noexcept
	{
		return *_Tail->_Prev.load()->_Myval;
	}

	const_reference back() const noexcept
	{
		return *_Tail->_Prev.load()->_Myval;
	}

public:
	void push_front( _Ty && _Val )
	{
		_Emplace( _Head, std::move( _Val ) );
	}

	void push_front( const _Ty & _Val )
	{
		_Emplace( _Head, _Val );
	}

	template <typename... _Valty> iterator emplace_front( _Valty&&... _Val )
	{
		return _Emplace( _Head, std::forward<_Valty>( _Val )... );
	}

	void pop_front() noexcept
	{
		_Erase( _Head->_Next.load() );
	}

	void push_back( _Ty && _Val )
	{
		_Emplace( _Tail->_Prev.load(), std::move( _Val ) );
	}

	void push_back( const _Ty & _Val )
	{
		_Emplace( _Tail->_Prev.load(), _Val );
	}

	template <typename... _Valty> iterator emplace_back( _Valty&&... _Val )
	{
		return _Emplace( _Tail->_Prev, std::forward<_Valty>( _Val )... );
	}

	void pop_back() noexcept
	{
		_Erase( _Tail->_Prev.load() );
	}

public:
	void assign( std::initializer_list<_Ty> _Ilist )
	{
		assign( _Ilist.begin(), _Ilist.end() );
	}

	void assign( size_type _Count, const _Ty & _Val )
	{
		clear();

		auto node = _Head;
		for ( size_t i = 0; i < _Count; i++ )
		{
			node = _Emplace( node, _Val );
		}
	}

	template <typename _Iter> void assign( _Iter _First, _Iter _Last )
	{
		clear();

		auto node = _Head;
		for ( ; _First != _Last; ++_First )
		{
			node = _Emplace( node, *_First );
		}
	}

public:
	iterator insert( const_iterator _Where, _Ty && _Val )
	{
		return emplace( _Where, std::move( _Val ) );
	}

	iterator insert( const_iterator _Where, const _Ty & _Val )
	{
		return { _Emplace( _Where._Ptr, _Val ) };
	}

public:
	iterator erase( const const_iterator _Where ) noexcept
	{
		const auto _Result = _Where._Ptr->_Next.load();
		
		_Erase( _Where._Ptr );
		--_Size;

		return { _Result };
	}

public:
	void clear() noexcept
	{
		auto node = _Head;

		_Construct();

		while ( node != nullptr )
		{
			node->_Prev = nullptr;

			_Nodeptr next = node->_Next.load();
			while ( !node->_Next.compare_exchange_weak( next, nullptr ) );

			_Erase( node );

			node = next;
		}
	}

public:
	void swap( ConcurrentList & _Right ) noexcept
	{
		if ( this != std::addressof( _Right ) )
		{
			std::swap( _Al, _Right._Al );
			std::swap( _Head, _Right._Head );
			std::swap( _Tail, _Right._Tail );
			std::swap( _Size, _Right._Size );
		}
	}

public:
	template <typename... _Valty> iterator emplace( const const_iterator _Where, _Valty&&... _Val ) noexcept
	{
		return { _Emplace( _Where._Ptr, std::move( _Val )... ) };
	}

private:
	template< typename _Ty1, typename ... _Ty2> _Nodeptr _Emplace( _Nodeptr _NPtr, _Ty1 && _Val1, _Ty2 &&... _Val ) noexcept
	{
		_Nodeptr head = _Alnode_traits::allocate( _Al, 1 );
		_Alnode_traits::construct( _Al, head );

		{
			_Alty _ValAlloc( _Al.resource() );
			head->_Myval = _Alty_traits::allocate( _ValAlloc, 1 );
			_Alty_traits::construct( _ValAlloc, head->_Myval );
		}

		_Nodeptr tail = _Emplace( head, std::move( _Val )... );

		head->_Prev = _NPtr;
		_Nodeptr next = _NPtr->_Next.load();
		while ( !_NPtr->_Next.compare_exchange_weak( next, head ) );
		tail->_Next = next;

		return tail;
	}

	template< typename _Ty1> _Nodeptr _Emplace( _Nodeptr _NPtr, _Ty1 && _Val ) noexcept
	{
		_Nodeptr node = _Alnode_traits::allocate( _Al, 1 );
		_Alnode_traits::construct( _Al, node );

		{
			_Alty _ValAlloc( _Al.resource() );
			node->_Myval = _Alty_traits::allocate( _ValAlloc, 1 );
			_Alty_traits::construct( _ValAlloc, node->_Myval );
		}

		node->_Prev = _NPtr;
		_Nodeptr next = _NPtr->_Next.load();
		while ( !_NPtr->_Next.compare_exchange_weak( next, node ) );
		node->_Next = next;

		return node;
	}

	void _Erase( _Nodeptr _NPtr ) noexcept
	{
		_Nodeptr prev = _NPtr->_Prev.load();
		_Nodeptr next = _NPtr->_Next.load();

		if ( prev != nullptr )
		{
			_Nodeptr p_next = prev->_Next.load();
			while ( !prev->_Next.compare_exchange_weak( p_next, next ) );
		}

		if ( next != nullptr )
		{
			_Nodeptr p_prev = next->_Prev.load();
			while ( !next->_Prev.compare_exchange_weak( p_prev, prev ) );
		}

		if ( _NPtr->_Myval != nullptr )
		{
			_Alty _ValAlloc( _Al.resource() );
			_Alty_traits::destroy( _ValAlloc, _NPtr->_Myval );
			_Alty_traits::deallocate( _ValAlloc, _NPtr->_Myval, 1 );
		}

		_Alnode_traits::destroy( _Al, _NPtr );
		_Alnode_traits::deallocate( _Al, _NPtr, 1 );
	}

	void _Construct() noexcept
	{
		_Head = _Alnode_traits::allocate( _Al, 1 );
		_Alnode_traits::construct( _Al, _Head );

		_Tail = _Alnode_traits::allocate( _Al, 1 );
		_Alnode_traits::construct( _Al, _Tail );

		_Head->_Next = _Tail;
		_Tail->_Prev = _Head;

		_Size = 0;
	}

private:
	_Alnode _Al;
	_Nodeptr _Head = nullptr;
	_Nodeptr _Tail = nullptr;
	XE::uint64 _Size = 0;
};

END_XE_NAMESPACE

#endif//CONCURRENTLIST_HPP__4D6C0B5D_BFB4_4FD7_984E_8CCF2C9130D5
