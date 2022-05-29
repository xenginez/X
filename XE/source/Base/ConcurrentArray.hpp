/*!
* \file   ConcurrentArray.hpp
*
* \author ZhengYuanQing
* \date   2022/05/28
* \email  zhengyuanqing.95@gmail.com
*
*/
#ifndef CONCURRENTARRAY_HPP__8B6F212F_8323_40BB_9456_D69C77F6AE77
#define CONCURRENTARRAY_HPP__8B6F212F_8323_40BB_9456_D69C77F6AE77

#include <shared_mutex>

#include "Type.h"

BEG_XE_NAMESPACE

template < typename _Myvec > class ConcurrentArrayConstIterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename _Myvec::value_type;
    using difference_type = typename _Myvec::difference_type;
    using pointer = typename _Myvec::const_pointer;
    using reference = const value_type &;

    using _Tptr = typename _Myvec::pointer;
    using _Container_base = _Myvec;

    constexpr ConcurrentArrayConstIterator() noexcept
        : _Ptr()
    {
    }

    constexpr ConcurrentArrayConstIterator( _Tptr _Parg, const _Container_base * _PConcurrentArray ) noexcept
        : _Ptr( _Parg ), _Container( _PConcurrentArray )
    {
    }

    constexpr reference operator*() const noexcept
    {
        return *_Ptr;
    }

    constexpr pointer operator->() const noexcept
    {
        return _Ptr;
    }

    constexpr ConcurrentArrayConstIterator & operator++() noexcept
    {
        ++_Ptr;
        return *this;
    }

    constexpr ConcurrentArrayConstIterator operator++( int ) noexcept
    {
        ConcurrentArrayConstIterator _Tmp = *this;
        ++ * this;
        return _Tmp;
    }

    constexpr ConcurrentArrayConstIterator & operator--() noexcept
    {
        --_Ptr;
        return *this;
    }

    constexpr ConcurrentArrayConstIterator operator--( int ) noexcept
    {
        ConcurrentArrayConstIterator _Tmp = *this;
        -- * this;
        return _Tmp;
    }

    constexpr ConcurrentArrayConstIterator & operator+=( const difference_type _Off ) noexcept
    {
        _Ptr += _Off;
        return *this;
    }

    constexpr ConcurrentArrayConstIterator operator+( const difference_type _Off ) const noexcept
    {
        ConcurrentArrayConstIterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    constexpr ConcurrentArrayConstIterator & operator-=( const difference_type _Off ) noexcept
    {
        return *this += -_Off;
    }

    constexpr ConcurrentArrayConstIterator operator-( const difference_type _Off ) const noexcept
    {
        ConcurrentArrayConstIterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    constexpr difference_type operator-( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return _Ptr - _Right._Ptr;
    }

    constexpr reference operator[]( const difference_type _Off ) const noexcept
    {
        return *( *this + _Off );
    }

    constexpr bool operator==( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return _Ptr == _Right._Ptr;
    }

    bool operator!=( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return !( *this == _Right );
    }

    bool operator<( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return _Ptr < _Right._Ptr;
    }

    bool operator>( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return _Right < *this;
    }

    bool operator<=( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return !( _Right < *this );
    }

    bool operator>=( const ConcurrentArrayConstIterator & _Right ) const noexcept
    {
        return !( *this < _Right );
    }

    _Tptr _Ptr;
    _Container_base * _Container;
};

template < typename _Myvec > class ConcurrentArrayIterator : public ConcurrentArrayConstIterator<_Myvec>
{
public:
    using _Mybase = ConcurrentArrayConstIterator<_Myvec>;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename _Myvec::value_type;
    using difference_type = typename _Myvec::difference_type;
    using pointer = typename _Myvec::pointer;
    using reference = value_type &;

    using _Mybase::_Mybase;

    constexpr reference operator*() const noexcept
    {
        return const_cast<reference>( _Mybase::operator*() );
    }

    constexpr pointer operator->() const noexcept
    {
        return const_cast<pointer>( _Mybase::operator->() );
    }

    constexpr ConcurrentArrayIterator & operator++() noexcept
    {
        _Mybase::operator++();
        return *this;
    }

    constexpr ConcurrentArrayIterator operator++( int ) noexcept
    {
        ConcurrentArrayIterator _Tmp = *this;
        _Mybase::operator++();
        return _Tmp;
    }

    constexpr ConcurrentArrayIterator & operator--() noexcept
    {
        _Mybase::operator--();
        return *this;
    }

    constexpr ConcurrentArrayIterator operator--( int ) noexcept
    {
        ConcurrentArrayIterator _Tmp = *this;
        _Mybase::operator--();
        return _Tmp;
    }

    constexpr ConcurrentArrayIterator & operator+=( const difference_type _Off ) noexcept
    {
        _Mybase::operator+=( _Off );
        return *this;
    }

    constexpr ConcurrentArrayIterator operator+( const difference_type _Off ) const noexcept
    {
        ConcurrentArrayIterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    constexpr ConcurrentArrayIterator & operator-=( const difference_type _Off ) noexcept
    {
        _Mybase::operator-=( _Off );
        return *this;
    }

    using _Mybase::operator-;

    constexpr ConcurrentArrayIterator operator-( const difference_type _Off ) const noexcept
    {
        ConcurrentArrayIterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    constexpr reference operator[]( const difference_type _Off ) const noexcept
    {
        return const_cast<reference>( _Mybase::operator[]( _Off ) );
    }
};

template < typename _Ty, typename _Alloc = std::pmr::polymorphic_allocator< _Ty > > class ConcurrentArray
{
    friend class ConcurrentArrayConstIterator< ConcurrentArray< _Ty, _Alloc > >;

private:
    using _Alty_traits = std::allocator_traits< _Alloc >;

public:
    using value_type = _Ty;
    using allocator_type = _Alloc;
    using pointer = typename _Alty_traits::pointer;
    using const_pointer = typename _Alty_traits::const_pointer;
    using reference = _Ty &;
    using const_reference = const _Ty &;
    using size_type = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;

public:
    using iterator = ConcurrentArrayIterator<_Ty>;
    using const_iterator = ConcurrentArrayConstIterator<_Ty>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr ConcurrentArray()
        : _Mypair()
    {

    }

    constexpr explicit ConcurrentArray( const _Alloc & _Al ) noexcept
        : _Mypair( _Al )
    {

    }

    constexpr explicit ConcurrentArray( const size_type _Count, const _Alloc & _Al = _Alloc() )
        : _Mypair( _Count, _Al )
    {

    }

    constexpr ConcurrentArray( const size_type _Count, const _Ty & _Val, const _Alloc & _Al = _Alloc() )
        : _Mypair( _Count, _Val, _Al )
    {

    }

    template < typename _Iter > constexpr ConcurrentArray( _Iter _First, _Iter _Last, const _Alloc & _Al = _Alloc() )
        : _Mypair( _First, _Last, _Al )
    {
    }

    constexpr ConcurrentArray( std::initializer_list<_Ty> _Ilist, const _Alloc & _Al = _Alloc() )
        : _Mypair( _Ilist, _Al )
    {

    }

    constexpr ConcurrentArray( const ConcurrentArray & _Right )
        : _Mypair( _Right._Mypair )
    {

    }

    constexpr ConcurrentArray( const ConcurrentArray & _Right, const _Alloc & _Al )
        : _Mypair( _Right._Mypair, _Al )
    {

    }

    constexpr ConcurrentArray( ConcurrentArray && _Right ) noexcept
        : _Mypair( std::move( _Right._Mypair ) )
    {

    }

    constexpr ConcurrentArray( ConcurrentArray && _Right, const _Alloc & _Al_ ) noexcept
        : _Mypair( std::move( _Right._Mypair ), _Al_ )
    {
    }

    constexpr ConcurrentArray & operator=( ConcurrentArray && _Right ) noexcept
    {
        _Mypair = std::move( _Right._Mypair );

        return *this;
    }

    ~ConcurrentArray() noexcept
    {
    }

public:
    template < typename ... _Valty > constexpr decltype( auto ) emplace_back( _Valty&&... _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.emplace_back( _Val );
    }

    constexpr void push_back( const _Ty & _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.emplace_back( _Val );
    }

    constexpr void push_back( _Ty && _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.emplace_back( std::move( _Val ) );
    }

    template < typename ... _Valty > constexpr iterator emplace( const_iterator _Where, _Valty&&... _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.emplace( _Where, _Val... );
    }

    constexpr iterator insert( const_iterator _Where, const _Ty & _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.emplace( _Where, _Val );
    }

    constexpr iterator insert( const_iterator _Where, _Ty && _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.emplace( _Where, std::move( _Val ) );
    }

    constexpr iterator insert( const_iterator _Where, const size_type _Count, const _Ty & _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.insert( _Where, _Count, _Val );
    }

public:
    template < typename _Iter > constexpr iterator insert( const_iterator _Where, _Iter _First, _Iter _Last )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.insert( _Where, _First, _Last );
    }

    constexpr iterator insert( const_iterator _Where, std::initializer_list<_Ty> _Ilist )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.insert( _Where, _Ilist.begin(), _Ilist.end() );
    }

    constexpr void assign( const size_type _Newsize, const _Ty & _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.assign( _Newsize, _Val );
    }

public:
    template < typename _Iter > constexpr void assign( _Iter _First, _Iter _Last )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.assign( _First, _Last );
    }

    constexpr void assign( std::initializer_list<_Ty> _Ilist )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.assign( _Ilist.begin(), _Ilist.end() );
    }

public:
    constexpr ConcurrentArray & operator=( const ConcurrentArray & _Right )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair = _Right._Mypair;

        return *this;
    }

    constexpr ConcurrentArray & operator=( std::initializer_list<_Ty> _Ilist )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair = _Ilist;

        return *this;
    }

public:
    constexpr void resize( const size_type _Newsize )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.resize( _Newsize );
    }

    constexpr void resize( const size_type _Newsize, const _Ty & _Val )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.resize( _Newsize, _Val );
    }

public:
    constexpr void reserve( const size_type _Newcapacity )
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.reserve( _Newcapacity );
    }

    constexpr void shrink_to_fit()
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.shrink_to_fit();
    }

    constexpr void pop_back() noexcept
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.pop_back();
    }

    constexpr void clear() noexcept
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.clear();
    }

    constexpr void swap( ConcurrentArray & _Right ) noexcept
    {
        std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair.swap( _Right._Mypair );
    }

    constexpr _Ty * unsafe_data() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.data();
    }

    constexpr const _Ty * unsafe_data() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.data();
    }

    constexpr iterator begin() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.begin();
    }

    constexpr const_iterator begin() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.begin();
    }

    constexpr iterator end() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.end();
    }

    constexpr const_iterator end() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.end();
    }

    constexpr reverse_iterator rbegin() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.rbegin();
    }

    constexpr const_reverse_iterator rbegin() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.rbegin();
    }

    constexpr reverse_iterator rend() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.rend();
    }

    constexpr const_reverse_iterator rend() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.rend();
    }

    constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }

    constexpr const_iterator cend() const noexcept
    {
        return end();
    }

    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    constexpr const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    constexpr bool empty() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.empty();
    }

    constexpr size_type size() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.size();
    }

    constexpr size_type max_size() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.max_size();
    }

    constexpr size_type capacity() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.capacity();
    }

    constexpr const _Ty & operator[]( const size_type _Pos ) const
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair[_Pos];
    }

    constexpr const _Ty & at( const size_type _Pos ) const
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.at( _Pos );
    }

    constexpr void set( const size_type _Pos, const value_type & _Val )
	{
		std::unique_lock< std::shared_mutex > lock( _Lock );

        _Mypair[_Pos] = _Val;
    }

    constexpr _Ty & front() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.front();
    }

    constexpr const _Ty & front() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.front();
    }

    constexpr _Ty & back() noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.back();
    }

    constexpr const _Ty & back() const noexcept
    {
        std::shared_lock< std::shared_mutex > lock( _Lock );

        return _Mypair.back();
    }

    constexpr allocator_type get_allocator() const noexcept
    {
        return _Mypair.get_allocator();
    }

private:
    mutable std::shared_mutex _Lock;
    std::vector< _Ty, _Alloc > _Mypair;
};

END_XE_NAMESPACE

#endif//CONCURRENTARRAY_HPP__8B6F212F_8323_40BB_9456_D69C77F6AE77