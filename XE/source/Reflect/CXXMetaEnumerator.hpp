/*!
 * \file	CXXMetaEnumerator.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/08/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CXXMETAENUMERATOR_HPP__13A6391C_9C4A_4AF4_A1CE_8AB180493A63
#define CXXMETAENUMERATOR_HPP__13A6391C_9C4A_4AF4_A1CE_8AB180493A63

#include "MetaEnumerator.h"

BEG_XE_NAMESPACE

template< typename ContainerType > class CXXMetaEnumerator : public XE::MetaEnumerator
{
public:
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		*_Cur = val.Value< ValueType >();
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;

		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		auto beg = _Beg;
		auto end = _Beg;

		std::advance( beg, first );
		std::advance( end, second );

		std::iter_swap( beg, end );

		return true;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::set< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::set< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		*_Cur = val.Value< ValueType >();
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::map< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::map< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		_Cur->second = val.Value< ValueType >().second;
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::multiset< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::multiset< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		*_Cur = val.Value< ValueType >();
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::multimap< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::multimap< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		_Cur->second = val.Value< ValueType >().second;
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::unordered_set< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::unordered_set< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		*_Cur = val.Value< ValueType >();
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::unordered_map< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::unordered_map< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		_Cur->second = val.Value< ValueType >().second;
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::unordered_multiset< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::unordered_multiset< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		*_Cur = val.Value< ValueType >();
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};
template< typename ... T > class CXXMetaEnumerator< std::unordered_multimap< T... > > : public XE::MetaEnumerator
{
public:
	using ContainerType = std::unordered_multimap< T... >;
	using Iterator = ContainerType::iterator;
	using ValueType = ContainerType::value_type;

public:
	CXXMetaEnumerator( ContainerType * ptr )
		: _Ptr( ptr ), _Beg( std::begin( *ptr ) ), _Cur( std::begin( *ptr ) ), _End( std::end( *ptr ) )
	{

	}

	~CXXMetaEnumerator() override = default;

public:
	XE::MetaTypeCPtr GetValueType() override
	{
		return ::TypeID< ValueType >::Get();
	}

	XE::Variant GetCurrent() override
	{
		return _Cur.operator ->();
	}

	void SetCurrent( const XE::Variant & val ) override
	{
		_Cur->second = val.Value< ValueType >().second;
	}

public:
	void MoveNext() override
	{
		_Cur++;
	}

	void Reset() override
	{
		_Cur = _Beg;
	}

	bool IsEnd() override
	{
		return _Cur == _End;
	}

public:
	XE::uint64 GetSize() override
	{
		return _Ptr->size();
	}

	void SetIndex( XE::uint64 val ) override
	{
		_Cur = _Beg;
		if ( (XE::uint64)std::distance( _Beg, _End ) < val )
		{
			std::advance( _Cur, val );
		}
		else
		{
			_Cur = _End;
		}
	}

public:
	void EraseCurrent() override
	{
		_Cur = _Ptr->erase( _Cur );
	}

	void InsertToCurrent( const XE::Variant & val ) override
	{
		_Ptr->insert( _Cur, val.Value< ValueType >() );
	}

public:
	bool Swap( XE::uint64 first, XE::uint64 second ) override
	{
		return false;
	}

protected:
	ContainerType * _Ptr;
	Iterator _Beg, _Cur, _End;
};

END_XE_NAMESPACE

#endif//CXXMETAENUMERATOR_HPP__13A6391C_9C4A_4AF4_A1CE_8AB180493A63
