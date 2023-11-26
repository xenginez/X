/*!
 * \file	multi_array.hpp
 *
 * \author	ZhengYuanQing
 * \date	2020/06/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MULTI_ARRAY_HPP__C18A3E92_BC14_4913_A6DA_CCAC92FE3A43
#define MULTI_ARRAY_HPP__C18A3E92_BC14_4913_A6DA_CCAC92FE3A43

#include <array>

#include "Declare.h"

BEG_XE_NAMESPACE

template< std::size_t ... _Size > class _multi_array_size;

template< > class _multi_array_size< >
{
public:
	static constexpr std::size_t size = 0;
	static constexpr std::size_t value = 0;
};

template< std::size_t _Sz > class _multi_array_size< _Sz >
{
public:
	using type = _multi_array_size< >;
	static constexpr std::size_t size = _Sz;
	static constexpr std::size_t value = _Sz;
};

template< std::size_t _Sz, std::size_t ... _Size > class _multi_array_size< _Sz, _Size... >
{
public:
	using type = _multi_array_size< _Size... >;
	static constexpr std::size_t size = _Sz;
	static constexpr std::size_t value = _Sz * type::value;
};

template< std::size_t ... _Size > using _multi_array_size_t = typename _multi_array_size< _Size... >::type;
template< std::size_t ... _Size > static constexpr std::size_t _multi_array_size_v = _multi_array_size< _Size... >::value;


template< typename _Ty > class MultiArrayConstIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = _Ty;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using reference = _Ty &;

	using _Tptr = pointer;

public:
	MultiArrayConstIterator() noexcept
		: _Ptr()
	{
	}

	MultiArrayConstIterator( _Tptr _Parg ) noexcept
		: _Ptr( _Parg )
	{

	}

	reference operator*() const
	{
		return *_Ptr;
	}

	pointer operator->() const
	{
		return _Ptr;
	}

	MultiArrayConstIterator & operator++()
	{
		++_Ptr;
		return *this;
	}

	MultiArrayConstIterator operator++( int )
	{
		MultiArrayConstIterator _Tmp = *this;
		++ * this;
		return _Tmp;
	}

	MultiArrayConstIterator & operator--()
	{
		--_Ptr;
		return *this;
	}

	MultiArrayConstIterator operator--( int )
	{
		MultiArrayConstIterator _Tmp = *this;
		-- * this;
		return _Tmp;
	}

	MultiArrayConstIterator & operator+=( const difference_type _Off )
	{
		_Ptr += _Off;
		return *this;
	}

	MultiArrayConstIterator operator+( const difference_type _Off ) const
	{
		MultiArrayConstIterator _Tmp = *this;
		return _Tmp += _Off;
	}

	MultiArrayConstIterator & operator-=( const difference_type _Off )
	{
		return *this += -_Off;
	}

	MultiArrayConstIterator operator-( const difference_type _Off ) const
	{
		MultiArrayConstIterator _Tmp = *this;
		return _Tmp -= _Off;
	}

	difference_type operator-( const MultiArrayConstIterator & _Right ) const
	{
		return _Ptr - _Right._Ptr;
	}

	reference operator[]( const difference_type _Off ) const
	{
		return *( *this + _Off );
	}

	bool operator==( const MultiArrayConstIterator & _Right ) const
	{
		return _Ptr == _Right._Ptr;
	}

	bool operator!=( const MultiArrayConstIterator & _Right ) const
	{
		return !( *this == _Right );
	}

	bool operator< ( const MultiArrayConstIterator & _Right ) const
	{
		return _Ptr < _Right._Ptr;
	}

	bool operator >( const MultiArrayConstIterator & _Right ) const
	{
		return _Right < *this;
	}

	bool operator<=( const MultiArrayConstIterator & _Right ) const
	{
		return !( _Right < *this );
	}

	bool operator>=( const MultiArrayConstIterator & _Right ) const
	{
		return !( *this < _Right );
	}

	_Tptr _Ptr;
};

template< typename _Ty > class MultiArrayIterator : public MultiArrayConstIterator< _Ty >
{
public:
	using _Mybase = MultiArrayConstIterator< _Ty >;

	using iterator_category = std::random_access_iterator_tag;
	using value_type = _Ty;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using reference = value_type &;

	using _Mybase::_Mybase;

public:
	reference operator*() const
	{
		return const_cast< reference >( _Mybase::operator*() );
	}

	pointer operator->() const
	{
		return const_cast< pointer >( _Mybase::operator->() );
	}

	MultiArrayIterator & operator++()
	{
		_Mybase::operator++();
		return *this;
	}

	MultiArrayIterator operator++( int )
	{
		MultiArrayIterator _Tmp = *this;
		_Mybase::operator++();
		return _Tmp;
	}

	MultiArrayIterator & operator--()
	{
		_Mybase::operator--();
		return *this;
	}

	MultiArrayIterator operator--( int )
	{
		MultiArrayIterator _Tmp = *this;
		_Mybase::operator--();
		return _Tmp;
	}

	MultiArrayIterator & operator+=( const difference_type _Off )
	{
		_Mybase::operator+=( _Off );
		return *this;
	}

	MultiArrayIterator operator+( const difference_type _Off ) const
	{
		MultiArrayIterator _Tmp = *this;
		return _Tmp += _Off;
	}

	MultiArrayIterator & operator-=( const difference_type _Off )
	{
		_Mybase::operator-=( _Off );
		return *this;
	}

	using _Mybase::operator-;

	MultiArrayIterator operator-( const difference_type _Off ) const
	{
		MultiArrayIterator _Tmp = *this;
		return _Tmp -= _Off;
	}

	reference operator[]( const difference_type _Off ) const
	{
		return const_cast< reference >( _Mybase::operator[]( _Off ) );
	}
};


template< typename _Ty, std::size_t _Sz, std::size_t ... _Size > class MultiArrayView< _Ty, _multi_array_size< _Sz, _Size... > >
{
public:
	using value_type = _Ty;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using reference = _Ty &;
	using const_reference = const _Ty &;

	using iterator = MultiArrayIterator< _Ty >;
	using const_iterator = MultiArrayConstIterator< _Ty >;

	using reverse_iterator = std::reverse_iterator< iterator >;
	using const_reverse_iterator = std::reverse_iterator< const_iterator >;

	using view = MultiArrayView< _Ty, _multi_array_size< _Size...>>;
	using const_view = const MultiArrayView< _Ty, _multi_array_size< _Size...>>;

public:
	MultiArrayView()
		:_Elems( nullptr )
	{

	}

	MultiArrayView( _Ty * Elems )
		:_Elems( Elems )
	{

	}

public:
	iterator begin() noexcept
	{
		return iterator( _Elems );
	}

	const_iterator begin() const noexcept
	{
		return const_iterator( _Elems );
	}

	iterator end() noexcept
	{
		return iterator( _Elems + _Sz * _multi_array_size_v< _Size... > );
	}

	const_iterator end() const noexcept
	{
		return const_iterator( _Elems + _Sz * _multi_array_size_v< _Size... > );
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
	constexpr size_type size() const noexcept
	{
		return _Sz;
	}

	constexpr size_type max_size() const noexcept
	{
		return _Sz;
	}

	constexpr bool empty() const noexcept
	{
		return false;
	}

	view at( size_type _Pos )
	{
		if( _Sz <= _Pos )
		{
			_Xran();
		}

		return { _Elems + _Pos * _multi_array_size_v< _Size... > };
	}

	constexpr const_view at( size_type _Pos ) const
	{
		if( _Sz <= _Pos )
		{
			_Xran();
		}

		return { _Elems + _Pos * _multi_array_size_v< _Size... > };
	}

	view operator[]( size_type _Pos ) noexcept
	{
		return { _Elems + _Pos * _multi_array_size_v< _Size... > };
	}

	constexpr const_view operator[]( size_type _Pos ) const noexcept
	{
		return { _Elems + _Pos * _multi_array_size_v< _Size... > };
	}

	reference front() noexcept
	{
		return _Elems[0];
	}

	constexpr const_reference front() const noexcept
	{
		return _Elems[0];
	}

	reference back() noexcept
	{
		return _Elems[_Sz * _multi_array_size_v< _Size... > - 1];
	}

	constexpr const_reference back() const noexcept
	{
		return _Elems[_Sz * _multi_array_size_v< _Size... > - 1];
	}

	_Ty * data() noexcept
	{
		return _Elems;
	}

	const _Ty * data() const noexcept
	{
		return _Elems;
	}

private:
	void _Xran() const
	{
		XE_ASSERT( false && "invalid MultiArrayView< T, N... > subscript" );
	}

private:
	_Ty * _Elems;
};

template< typename _Ty, std::size_t _Sz > class MultiArrayView< _Ty, _multi_array_size< _Sz > >
{
public:
	using value_type = _Ty;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using reference = _Ty &;
	using const_reference = const _Ty &;

	using iterator = MultiArrayIterator< _Ty >;
	using const_iterator = MultiArrayConstIterator< _Ty >;

	using reverse_iterator = std::reverse_iterator< iterator >;
	using const_reverse_iterator = std::reverse_iterator< const_iterator >;

public:
	MultiArrayView()
		:_Elems( nullptr )
	{

	}

	MultiArrayView( _Ty * Elems )
		:_Elems( Elems )
	{

	}

public:
	iterator begin() noexcept
	{
		return iterator( _Elems );
	}

	const_iterator begin() const noexcept
	{
		return const_iterator( _Elems );
	}

	iterator end() noexcept
	{
		return iterator( _Elems + _Sz );
	}

	const_iterator end() const noexcept
	{
		return const_iterator( _Elems + _Sz );
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
	constexpr size_type size() const noexcept
	{
		return _Sz;
	}

	constexpr size_type max_size() const noexcept
	{
		return _Sz;
	}

	constexpr bool empty() const noexcept
	{
		return false;
	}

	reference at( size_type _Pos )
	{
		if( _Sz <= _Pos )
		{
			_Xran();
		}

		return _Elems[_Pos];
	}

	constexpr const_reference at( size_type _Pos ) const
	{
		if( _Sz <= _Pos )
		{
			_Xran();
		}

		return _Elems[_Pos];
	}

	reference operator[]( size_type _Pos ) noexcept
	{
		return _Elems[_Pos];
	}

	constexpr const_reference operator[]( size_type _Pos ) const
		noexcept
	{
		return _Elems[_Pos];
	}

	reference front() noexcept
	{
		return _Elems[0];
	}

	constexpr const_reference front() const noexcept
	{
		return _Elems[0];
	}

	reference back() noexcept
	{
		return _Elems[_Sz - 1];
	}

	constexpr const_reference back() const noexcept
	{
		return _Elems[_Sz - 1];
	}

	_Ty * data() noexcept
	{
		return _Elems;
	}

	const _Ty * data() const noexcept
	{
		return _Elems;
	}

private:
	void _Xran() const
	{
		XE_ASSERT( false && "invalid MultiArrayView< T, N... > subscript" );
	}

private:
	_Ty * _Elems;
};


template< typename _Ty, std::size_t ... _Size > class MultiArray
{
public:
	using value_type = _Ty;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using reference = _Ty &;
	using const_reference = const _Ty &;

	using iterator = MultiArrayIterator< _Ty >;
	using const_iterator = MultiArrayConstIterator< _Ty >;

	using reverse_iterator = std::reverse_iterator< iterator >;
	using const_reverse_iterator = std::reverse_iterator< const_iterator >;

	using view = MultiArrayView< _Ty, typename _multi_array_size< _Size... >::type >;
	using const_view = const MultiArrayView< _Ty, typename _multi_array_size< _Size... >::type >;

public:
	void fill( const _Ty & _Value )
	{
		std::fill_n( _Elems, _multi_array_size_v< _Size... >, _Value );
	}

	void swap( MultiArray & _Other ) noexcept
	{
		auto _First1 = _Elems;
		auto _Last1 = _Elems + _multi_array_size_v< _Size... >;
		auto _First2 = _Other._Elems;

		for( ; _First1 != _Last1; ++_First1, ++_First2 )
		{
			std::iter_swap( _First1, _First2 );
		}
	}

public:
	iterator begin() noexcept
	{
		return iterator( _Elems );
	}

	const_iterator begin() const noexcept
	{
		return const_iterator( _Elems );
	}

	iterator end() noexcept
	{
		return iterator( _Elems + _multi_array_size_v< _Size... > );
	}

	const_iterator end() const noexcept
	{
		return const_iterator( _Elems + _multi_array_size_v< _Size... > );
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
	constexpr size_type size() const noexcept
	{
		return _multi_array_size_v< _Size... >;
	}

	constexpr size_type max_size() const noexcept
	{
		return _multi_array_size_v< _Size... >;
	}

	constexpr bool empty() const noexcept
	{
		return false;
	}

	view at( size_type _Pos )
	{
		if( _multi_array_size< _Size... >::size <= _Pos )
		{
			_Xran();
		}

		return { _Elems + _Pos * _multi_array_size< _Size... >::type::value };
	}

	constexpr const_view at( size_type _Pos ) const
	{
		if( _multi_array_size< _Size... >::size <= _Pos )
		{
			_Xran();
		}

		return { _Elems + _Pos * _multi_array_size< _Size... >::type::value };
	}

	view operator[]( size_type _Pos ) noexcept
	{
		return { _Elems + _Pos * _multi_array_size< _Size... >::type::value };
	}

	constexpr const_view operator[]( size_type _Pos ) const noexcept
	{
		return { _Elems + _Pos * _multi_array_size< _Size... >::type::value };
	}

	reference front() noexcept 
	{
		return _Elems[0];
	}

	constexpr const_reference front() const noexcept 
	{
		return _Elems[0];
	}

	reference back() noexcept 
	{
		return _Elems[_multi_array_size_v< _Size... > - 1];
	}

	constexpr const_reference back() const noexcept
	{
		return _Elems[_multi_array_size_v< _Size... > - 1];
	}

	_Ty * data() noexcept
	{
		return _Elems;
	}

	const _Ty * data() const noexcept
	{
		return _Elems;
	}

private:
	void _Xran() const
	{
		XE_ASSERT( false && "invalid multi_array< T, N... > subscript" );
	}

private:
	_Ty _Elems[_multi_array_size_v< _Size... >];
};

template< typename _Ty, std::size_t _Size > class MultiArray< _Ty, _Size > : public std::array< _Ty, _Size >
{
public:
	using _MyBase = std::array< _Ty, _Size >;

	using _MyBase::_MyBase;
};

END_XE_NAMESPACE

#endif // MULTI_ARRAY_HPP__C18A3E92_BC14_4913_A6DA_CCAC92FE3A43
