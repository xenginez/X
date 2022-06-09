/*!
 * \file	ConcurrentArray.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/06/09
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENTARRAY_HPP__A81B3E56_9155_42CC_99C4_BE5D4856AC8D
#define CONCURRENTARRAY_HPP__A81B3E56_9155_42CC_99C4_BE5D4856AC8D

#include "Type.h"

BEG_XE_NAMESPACE

template<typename _Container, typename _Value> class ConcurrentArrayIterator
{
private:
	template<typename _C, typename _U> friend class ConcurrentArrayIterator;

public:
	typedef ptrdiff_t difference_type;
	typedef _Value value_type;
	typedef _Value * pointer;
	typedef _Value & reference;
	typedef std::random_access_iterator_tag iterator_category;

private:
	ConcurrentArrayIterator( const _Container & _Arr, XE::uint64 _Index, void * _Ptr = nullptr )
		: _Array( const_cast<_Container *>( &_Arr ) ), _Index( _Index ), _Item( static_cast<_Value *>( _Ptr ) )
	{
	}

public:
	ConcurrentArrayIterator()
		: _Array( nullptr ), _Index( ~XE::uint64( 0 ) ), _Item( nullptr )
	{
	}
	ConcurrentArrayIterator( const ConcurrentArrayIterator<_Container, typename _Container::value_type> & _Other )
		: _Array( _Other._Array ), _Index( _Other._Index ), _Item( _Other._Item )
	{
	}

public:
	ConcurrentArrayIterator operator+( ptrdiff_t _Offset ) const
	{
		return ConcurrentArrayIterator( *_Array, _Index + _Offset );
	}
	ConcurrentArrayIterator & operator+=( ptrdiff_t _Offset )
	{
		_Index += _Offset;
		_Item = nullptr;
		return *this;
	}
	ConcurrentArrayIterator operator-( ptrdiff_t _Offset ) const
	{
		return ConcurrentArrayIterator( *_Array, _Index - _Offset );
	}
	ConcurrentArrayIterator & operator-=( ptrdiff_t _Offset )
	{
		_Index -= _Offset;
		_Item = nullptr;
		return *this;
	}

public:
	_Value & operator*() const
	{
		_Value * _Item = _Item;
		if ( !_Item )
		{
			_Item = _Item = &_Array->_Internal_subscript( _Index );
		}

		return *_Item;
	}
	_Value & operator[]( ptrdiff_t _K ) const
	{
		return _Array->_Internal_subscript( _Index + _K );
	}
	_Value * operator->() const
	{
		return &operator*();
	}

public:
	ConcurrentArrayIterator & operator++()
	{
		XE::uint64 _K = ++_Index;
		if ( _Item )
		{
			if ( ( _K & ( _K - 2 ) ) == 0 )
			{
				_Item = nullptr;
			}
			else
			{
				++_Item;
			}
		}
		return *this;
	}
	ConcurrentArrayIterator & operator--()
	{
		XE::uint64 _K = _Index--;
		if ( _Item )
		{
			if ( ( _K & ( _K - 2 ) ) == 0 )
			{
				_Item = nullptr;
			}
			else
			{
				--_Item;
			}
		}
		return *this;
	}
	ConcurrentArrayIterator operator++( int )
	{
		ConcurrentArrayIterator _Result = *this;
		operator++();
		return _Result;
	}
	ConcurrentArrayIterator operator--( int )
	{
		ConcurrentArrayIterator _Result = *this;
		operator--();
		return _Result;
	}

public:
	template<typename _U> bool operator==( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return _Index == _Right._Index && _Array == _Right._Array;
	}
	template<typename _U> bool operator!=( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return !( ( *this ) == _Right );
	}
	template<typename _U> bool operator<( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return _Index < _Right._Index && _Array == _Right._Array;
	}
	template<typename _U> bool operator>( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return _Right < ( *this );
	}
	template<typename _U> bool operator>=( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return !( ( *this ) < _Right );
	}
	template<typename _U> bool operator<=( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return !( _Right < ( *this ) );
	}
	template<typename _U> ptrdiff_t operator-( const ConcurrentArrayIterator<_Container, _U> & _Right ) const
	{
		return ptrdiff_t( _Index ) - ptrdiff_t( _Right._Index );
	}

private:
	_Value * _Item;
	XE::uint64 _Index;
	_Container * _Array;
};

template<typename _Ty, class _Ax = std::pmr::polymorphic_allocator<_Ty>> class ConcurrentArray
{
public:
	using size_type = XE::uint64;
	using allocator_type = typename std::allocator_traits<_Ax>::template rebind_alloc<_Ty>;
	using allocator_traits = std::allocator_traits<allocator_type>;
	using value_type = _Ty;
	using difference_type = std::ptrdiff_t;
	using reference = _Ty &;
	using const_reference = const _Ty &;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using iterator = ConcurrentArrayIterator<ConcurrentArray, _Ty>;
	using const_iterator = ConcurrentArrayIterator<ConcurrentArray, const _Ty>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	template<bool B> class _Is_integer_tag;
	template<typename _C, typename _U> friend class ConcurrentArrayIterator;
	using segment_index_t = XE::uint64;

private:
	static constexpr void * BAD_ALLOC_MARKER = reinterpret_cast<void *>( 63 );
	static constexpr segment_index_t DEFAULT_INITIAL_SEGMENTS = 1;
	static constexpr segment_index_t POINTERS_PER_SHORT_TABLE = 3;
	static constexpr segment_index_t POINTERS_PER_LONG_TABLE = sizeof( segment_index_t ) * 8;

private:
	struct segment_t
	{
		void * _Array;
	};
	class internal_loop_guide
	{
	public:
		const pointer _Array;
		const size_type _N;
		size_type _I;
		internal_loop_guide( size_type _NTrials, void * _Ptr )
			: _Array( static_cast<pointer>( _Ptr ) ), _N( _NTrials ), _I( 0 )
		{
		}

		void _Init()
		{
			for ( ; _I < _N; ++_I )
			{
				new( &_Array[_I] ) _Ty();
			}
		}
		void _Init( const void * _Src )
		{
			for ( ; _I < _N; ++_I )
			{
				new( &_Array[_I] ) _Ty( *static_cast<const _Ty *>( _Src ) );
			}
		}
		void _Copy( const void * _Src )
		{
			for ( ; _I < _N; ++_I )
			{
				new( &_Array[_I] ) _Ty( static_cast<const _Ty *>( _Src )[_I] );
			}
		}
		void _Assign( const void * _Src )
		{
			for ( ; _I < _N; ++_I )
			{
				_Array[_I] = static_cast<const _Ty *>( _Src )[_I];
			}
		}
		template<class _It> void _Iterate( _It & _Src )
		{
			for ( ; _I < _N; ++_I, ++_Src )
			{
				new( &_Array[_I] ) _Ty( *_Src );
			}
		}
		~internal_loop_guide()
		{
			if ( _I < _N )
			{
				std::memset( _Array + _I, 0, ( _N - _I ) * sizeof( value_type ) );
			}
		}

	private:
		void operator=( const internal_loop_guide & ) = delete;
	};
	struct internal_segments_table
	{
		segment_index_t _FirstBlock;
		void * _Table[POINTERS_PER_LONG_TABLE];
	};

private:
	typedef void( *internal_array_op1_t )( void * _Begin, size_type _N );
	typedef void( *internal_array_op2_t )( void * _Dst, const void * _Src, size_type _N );

public:
	ConcurrentArray( const allocator_type & _Al = allocator_type() )
		: _Allocator( _Al )
	{
		_Construct();
	}

	ConcurrentArray( ConcurrentArray && _Arr )
		: _Allocator( _Arr.get_allocator() )
	{
		_Construct();
		InternalSwap( _Arr );
	}
	ConcurrentArray( const ConcurrentArray & _Arr )
		: _Allocator( _Arr.get_allocator() )
	{
		_Construct();
		InternalCopy( _Arr, sizeof( _Ty ), &CopyArray );
	}
	template<class M> ConcurrentArray( const ConcurrentArray<_Ty, M> & _Arr, const allocator_type & _Al = allocator_type() )
		: _Allocator( _Al )
	{
		_Construct();
		InternalCopy( _Arr._Internal_vector_base(), sizeof( _Ty ), &CopyArray );
	}

	ConcurrentArray( size_type _N )
	{
		_Construct();
		if ( _N != 0 )
		{
			InternalReserve( _N, sizeof( _Ty ), max_size() ); _EarlySize = _N;

			InitializeArray( static_cast<_Ty *>( _Segment[0]._Array ), nullptr, _N );
		}
	}
	ConcurrentArray( size_type _N, const_reference _Item, const allocator_type & _Al = allocator_type() )
		: _Allocator( _Al )
	{
		_Construct();
		InternalAssign( _N, _Item );
	}

	template<class _InputIterator> ConcurrentArray( _InputIterator _Begin, _InputIterator _End, const allocator_type & _Al = allocator_type() )
		: _Allocator( _Al )
	{
		_Construct();
		InternalAssign( _Begin, _End, static_cast<_Is_integer_tag<std::numeric_limits<_InputIterator>::is_integer> *>( 0 ) );
	}

	~ConcurrentArray()
	{
		segment_t * _Table = _Segment;
		InternalFreeSegments( reinterpret_cast<void **>( _Table ), InternalClear( &DestroyArray ), _FirstBlock );
	}

public:
	ConcurrentArray & operator=( ConcurrentArray && _Arr )
	{
		if ( static_cast<void *>( this ) != static_cast<const void *>( &_Arr ) )
		{
			InternalSwap( _Arr );
			_Arr.clear();
		}
		return *this;
	}
	ConcurrentArray & operator=( const ConcurrentArray & _Arr )
	{
		if ( this != &_Arr )
			InternalAssign( _Arr, sizeof( _Ty ), &DestroyArray, &AssignArray, &CopyArray );
		return *this;
	}
	template<class M> ConcurrentArray & operator=( const ConcurrentArray<_Ty, M> & _Arr )
	{
		if ( static_cast<void *>( this ) != static_cast<const void *>( &_Arr ) )
		{
			InternalAssign( _Arr._Internal_vector_base(), sizeof( _Ty ), &DestroyArray, &AssignArray, &CopyArray );
		}
		return *this;
	}

public:
	iterator grow_by( size_type _Delta )
	{
		return iterator( *this, _Delta ? InternalGrowBy( _Delta, sizeof( _Ty ), &InitializeArray, nullptr ) : _EarlySize );
	}
	iterator grow_by( size_type _Delta, const_reference _Item )
	{
		return iterator( *this, _Delta ? InternalGrowBy( _Delta, sizeof( _Ty ), &InitializeArrayBy, static_cast<const void *>( &_Item ) ) : _EarlySize );
	}
	iterator grow_to_at_least( size_type _N )
	{
		size_type _M = 0;

		if ( _N )
		{
			_M = InternalGrowToAtLeastWithResult( _N, sizeof( _Ty ), &InitializeArray, nullptr );
			if ( _M > _N )
			{
				_M = _N;
			}
		}

		return iterator( *this, _M );
	};

public:
	iterator push_back( const_reference _Item )
	{
		size_type _K;
		void * _Ptr = InternalPushBack( sizeof( _Ty ), _K );
		internal_loop_guide _Loop( 1, _Ptr );
		_Loop._Init( &_Item );
		return iterator( *this, _K, _Ptr );
	}
	iterator push_back( _Ty && _Item )
	{
		size_type _K;
		void * _Ptr = InternalPushBack( sizeof( _Ty ), _K );
		new ( _Ptr ) _Ty( std::move( _Item ) );
		return iterator( *this, _K, _Ptr );
	}

public:
	reference operator[]( size_type _Index )
	{
		return InternalSubscript( _Index );
	}
	const_reference operator[]( size_type _Index ) const
	{
		return InternalSubscript( _Index );
	}
	reference at( size_type _Index )
	{
		return InternalSubscriptWithExceptions( _Index );
	}
	const_reference at( size_type _Index ) const
	{
		return InternalSubscriptWithExceptions( _Index );
	}

public:
	size_type size() const
	{
		size_type _Sz = _EarlySize;
		size_type _Cp = InternalCapacity();
		return _Cp < _Sz ? _Cp : _Sz;
	}
	bool empty() const
	{
		return !_EarlySize;
	}
	size_type capacity() const
	{
		return InternalCapacity();
	}
	size_type max_size() const
	{
		return ( ~size_type( 0 ) ) / sizeof( _Ty );
	}

public:
	void reserve( size_type _N )
	{
		if ( _N != 0 )
		{
			InternalReserve( _N, sizeof( _Ty ), max_size() );
		}
	}
	void shrink_to_fit()
	{
		internal_segments_table _Old = { 0, nullptr };
		try
		{
			if ( InternalCompact( sizeof( _Ty ), &_Old, &DestroyArray, &CopyArray ) )
			{
				InternalFreeSegments( _Old._Table, POINTERS_PER_LONG_TABLE, _Old._FirstBlock );
			}
		}
		catch ( ... )
		{
			if ( _Old._FirstBlock )
			{
				InternalFreeSegments( _Old._Table, 1, _Old._FirstBlock );
			}
		}
	}
	void resize( size_type _N )
	{
		InternalResize( _N, sizeof( _Ty ), max_size(), DestroyArray, InitializeArray, nullptr );
	}
	void resize( size_type _N, const _Ty & _Val )
	{
		InternalResize( _N, sizeof( _Ty ), max_size(), DestroyArray, InitializeArrayBy, static_cast<const void *>( &_Val ) );
	}

public:
	iterator begin()
	{
		return iterator( *this, 0 );
	}
	iterator end()
	{
		return iterator( *this, size() );
	}
	const_iterator begin() const
	{
		return const_iterator( *this, 0 );
	}
	const_iterator end() const
	{
		return const_iterator( *this, size() );
	}
	reverse_iterator rbegin()
	{
		return reverse_iterator( end() );
	}
	reverse_iterator rend()
	{
		return reverse_iterator( begin() );
	}
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator( end() );
	}
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator( begin() );
	}
	const_iterator cbegin() const
	{
		return begin();
	}
	const_iterator cend() const
	{
		return end();
	}
	const_reverse_iterator crbegin() const
	{
		return rbegin();
	}
	const_reverse_iterator crend() const
	{
		return rend();
	}

public:
	reference front()
	{
		return static_cast<_Ty *>( _Segment[0]._Array )[0];
	}
	const_reference front() const
	{
		return static_cast<_Ty *>( _Segment[0]._Array )[0];
	}
	reference back()
	{
		return InternalSubscript( size() - 1 );
	}
	const_reference back() const
	{
		return InternalSubscript( size() - 1 );
	}

public:
	allocator_type get_allocator() const
	{
		return this->_Allocator;
	}

public:
	void assign( size_type _N, const_reference _Item )
	{
		clear();
		InternalAssign( _N, _Item );
	}
	template<class _InputIterator> void assign( _InputIterator _Begin, _InputIterator _End )
	{
		clear();
		InternalAssign( _Begin, _End, static_cast<_Is_integer_tag<std::numeric_limits<_InputIterator>::is_integer> *>( 0 ) );
	}

public:
	void swap( ConcurrentArray & _Arr )
	{
		if ( this != &_Arr )
		{
			InternalSwap( _Arr );
			std::swap( this->_Allocator, _Arr._Allocator );
		}
	}

public:
	void clear()
	{
		InternalClear( &DestroyArray );
	}

private:
	void _Construct()
	{
		_EarlySize = 0;
		_FirstBlock = 0;
		for ( segment_index_t _I = 0; _I < POINTERS_PER_SHORT_TABLE; _I++ )
		{
			_Storage[_I]._Array = nullptr;
		}
		_Segment = _Storage;
	}

private:
	static segment_index_t SegmentIndexOf( size_type _Index )
	{
		// TODO: 
		return 0;
	}
	static segment_index_t SegmentBase( segment_index_t _K )
	{
		return ( segment_index_t( 1 ) << _K & ~segment_index_t( 1 ) );
	}
	static segment_index_t SegmentBaseIndexOf( segment_index_t & _Index )
	{
		segment_index_t _K = SegmentIndexOf( _Index );
		_Index -= SegmentBase( _K );
		return _K;
	}
	static size_type SegmentSize( segment_index_t _K )
	{
		return segment_index_t( 1 ) << _K;
	}

private:
	void InternalReserve( size_type _N, size_type _Element_size, size_type _Max_size )
	{
		// TODO: 
	}
	size_type InternalCapacity() const
	{
		// TODO: 
		return 0;
	}
	void InternalGrow( size_type _Start, size_type _Finish, size_type _Element_size, internal_array_op2_t _Init, const void * _Src )
	{
		// TODO: 
	}
	size_type InternalGrowSegment( const size_type _Start, size_type _Finish, size_type _Element_size, segment_t ** _PPSegment, size_type * _PSegStart, size_type * _PSegFinish )
	{
		// TODO: 
		return 0;
	}
	size_type InternalGrowBy( size_type _Delta, size_type _Element_size, internal_array_op2_t _Init, const void * _Src )
	{
		// TODO: 
		return 0;
	}
	void * InternalPushBack( size_type _Element_size, size_type & _Index )
	{
		// TODO: 
		return nullptr;
	}
	segment_index_t InternalClear( internal_array_op1_t _Destroy )
	{
		// TODO: 
		return 0;
	}
	void InternalTruncate( size_type _Old_size, size_type _New_size, size_type _Element_size, internal_array_op1_t _Destroy )
	{
		// TODO: 
	}
	void * InternalCompact( size_type _Element_size, void * _Table, internal_array_op1_t _Destroy, internal_array_op2_t _Copy )
	{
		// TODO: 
		return nullptr;
	}
	void InternalCopy( const ConcurrentArray & _Src, size_type _Element_size, internal_array_op2_t _Copy )
	{
		// TODO: 
	}
	void InternalAssign( const ConcurrentArray & _Src, size_type _Element_size, internal_array_op1_t _Destroy, internal_array_op2_t _Assign, internal_array_op2_t _Copy )
	{
		// TODO: 
	}
	void InternalThrowException( size_type ) const
	{
		// TODO: 
	}
	void InternalSwap( ConcurrentArray & )
	{
		// TODO: 
	}
	void InternalResize( size_type _New_size, size_type _Element_size, size_type _Max_size, internal_array_op1_t _Destroy, internal_array_op2_t _Init, const void * _Src )
	{
		// TODO: 
	}
	size_type InternalGrowToAtLeastWithResult( size_type _New_size, size_type _Element_size, internal_array_op2_t _Init, const void * _Src )
	{
		// TODO: 
		return 0;
	}

private:
	static void * InternalAllocator( ConcurrentArray & _Vb, XE::uint64 _K )
	{
		return static_cast<ConcurrentArray &>( _Vb )._Allocator.allocate( _K );
	}
	void InternalFreeSegments( void * _Table[], segment_index_t _K, segment_index_t _First_block )
	{
		while ( _K > _First_block )
		{
			--_K;
			_Ty * _Array = static_cast<_Ty *>( _Table[_K] );
			_Table[_K] = nullptr;
			if ( _Array > BAD_ALLOC_MARKER )
			{
				this->_Allocator.deallocate( _Array, SegmentSize( _K ) );
			}
		}
		_Ty * _Array = static_cast<_Ty *>( _Table[0] );
		if ( _Array > BAD_ALLOC_MARKER )
		{
			while ( _K > 0 )
			{
				_Table[--_K] = nullptr;
			}
			this->_Allocator.deallocate( _Array, SegmentSize( _First_block ) );
		}
	}

private:
	_Ty & InternalSubscript( size_type _Index ) const
	{
		size_type _J = _Index;
		segment_index_t _K = SegmentBaseIndexOf( _J );

		return static_cast<_Ty *>( _Segment[_K]._Array )[_J];
	}
	_Ty & InternalSubscriptWithExceptions( size_type _Index ) const
	{
		if ( _Index >= _EarlySize )
		{
			InternalThrowException( 0 );
		}

		size_type _J = _Index;
		segment_index_t _K = SegmentBaseIndexOf( _J );
		if ( _Segment == (segment_t *)_Storage && _K >= POINTERS_PER_SHORT_TABLE )
		{
			InternalThrowException( 1 );
		}

		void * _Array = _Segment[_K]._Array;
		if ( _Array <= BAD_ALLOC_MARKER )
		{
			InternalThrowException( 2 );
		}

		return static_cast<_Ty *>( _Array )[_J];
	}

private:
	void InternalAssign( size_type _N, const_reference _Item )
	{
		if ( _N == 0 )
		{
			return;
		}

		InternalReserve( _N, sizeof( _Ty ), max_size() );

		_EarlySize = _N;
		segment_index_t _K = 0;
		size_type _Sz = SegmentSize( _FirstBlock );
		while ( _Sz < _N )
		{
			InitializeArrayBy( static_cast<_Ty *>( _Segment[_K]._Array ), static_cast<const void *>( &_Item ), _Sz );
			_N -= _Sz;
			if ( !_K )
			{
				_K = _FirstBlock;
			}
			else
			{
				++_K;
				_Sz <<= 1;
			}
		}
		InitializeArrayBy( static_cast<_Ty *>( _Segment[_K]._Array ), static_cast<const void *>( &_Item ), _N );
	}
	template<class _I> void InternalAssign( _I _First, _I _Last, _Is_integer_tag<true> * )
	{
		InternalAssign( static_cast<size_type>( _First ), static_cast<_Ty>( _Last ) );
	}
	template<class _I> void InternalAssign( _I _First, _I _Last, _Is_integer_tag<false> * )
	{
		internalAssignIterators( _First, _Last );
	}
	template<class _I> void internalAssignIterators( _I _First, _I _Last )
	{
		size_type _N = std::distance( _First, _Last );
		if ( _N == 0 )
		{
			return;
		}

		InternalReserve( _N, sizeof( _Ty ), max_size() );

		_EarlySize = _N;
		segment_index_t _K = 0;
		size_type _Sz = SegmentSize( _FirstBlock );
		while ( _Sz < _N )
		{
			internal_loop_guide _Loop( _Sz, _Segment[_K]._Array );
			_Loop._Iterate( _First );
			_N -= _Sz;
			if ( !_K )
			{
				_K = _FirstBlock;
			}
			else
			{
				++_K;
				_Sz <<= 1;
			}
		}

		internal_loop_guide _Loop( _N, _Segment[_K]._Array );
		_Loop._Iterate( _First );
	}

private:
	static void InitializeArray( void * _Begin, const void *, size_type _N )
	{
		internal_loop_guide _Loop( _N, _Begin ); _Loop._Init();
	}
	static void InitializeArrayBy( void * _Begin, const void * _Src, size_type _N )
	{
		internal_loop_guide _Loop( _N, _Begin ); _Loop._Init( _Src );
	}

private:
	static void CopyArray( void * _Dst, const void * _Src, size_type _N )
	{
		internal_loop_guide _Loop( _N, _Dst ); _Loop._Copy( _Src );
	}
	static void AssignArray( void * _Dst, const void * _Src, size_type _N )
	{
		internal_loop_guide _Loop( _N, _Dst ); _Loop._Assign( _Src );
	}
	static void DestroyArray( void * _Begin, size_type _N )
	{
		_Ty * _Array = static_cast<_Ty *>( _Begin );
		for ( size_type _J = _N; _J > 0; --_J )
		{
			_Array[_J - 1].~_Ty();
		}
	}

private:
	allocator_type _Allocator;
	std::atomic<size_type> _FirstBlock;
	std::atomic<size_type> _EarlySize;
	std::atomic<segment_t *> _Segment;
	segment_t _Storage[POINTERS_PER_SHORT_TABLE];
};

template<typename _Ty, class _Ax> inline void swap( ConcurrentArray<_Ty, _Ax> & _A, ConcurrentArray<_Ty, _Ax> & _B )
{
	_A.swap( _B );
}

template<typename _Ty, class A1, class A2> inline bool operator==( const ConcurrentArray<_Ty, A1> & _A, const ConcurrentArray<_Ty, A2> & _B )
{
	if ( _A.size() != _B.size() )
	{
		return false;
	}
	typename ConcurrentArray<_Ty, A1>::const_iterator _I( _A.begin() );
	typename ConcurrentArray<_Ty, A2>::const_iterator _J( _B.begin() );
	for ( ; _I != _A.end(); ++_I, ++_J )
	{
		if ( !( *_I == *_J ) )
		{
			return false;
		}
	}
	return true;
}

template<typename _Ty, class A1, class A2> inline bool operator!=( const ConcurrentArray<_Ty, A1> & _A, const ConcurrentArray<_Ty, A2> & _B )
{
	return !( _A == _B );
}

template<typename _Ty, class A1, class A2> inline bool operator<( const ConcurrentArray<_Ty, A1> & _A, const ConcurrentArray<_Ty, A2> & _B )
{
	return ( std::lexicographical_compare( _A.begin(), _A.end(), _B.begin(), _B.end() ) );
}

template<typename _Ty, class A1, class A2> inline bool operator>( const ConcurrentArray<_Ty, A1> & _A, const ConcurrentArray<_Ty, A2> & _B )
{
	return _B < _A;
}

template<typename _Ty, class A1, class A2> inline bool operator<=( const ConcurrentArray<_Ty, A1> & _A, const ConcurrentArray<_Ty, A2> & _B )
{
	return !( _B < _A );
}

template<typename _Ty, class A1, class A2> inline bool operator>=( const ConcurrentArray<_Ty, A1> & _A, const ConcurrentArray<_Ty, A2> & _B )
{
	return !( _A < _B );
}

END_XE_NAMESPACE

#endif// CONCURRENTARRAY_HPP__A81B3E56_9155_42CC_99C4_BE5D4856AC8D