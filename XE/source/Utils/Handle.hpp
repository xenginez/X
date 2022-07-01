/*!
 * \file   Handle.hpp
 *
 * \author ZhengYuanQing
 * \date   2019/02/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __HANDLE_HPP__7920C1A9_2044_45F0_84C4_221961C006AF
#define __HANDLE_HPP__7920C1A9_2044_45F0_84C4_221961C006AF

#include <atomic>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename To > XE::Handle< To > HandleCast( const XE::uint64 & val );
template< typename To, typename From > XE::Handle< To > HandleCast( const XE::Handle< From > & val );


template< typename T > class Handle
{
	template< typename T > friend struct Serializable;

	template< typename T > friend class HandleAllocator;

	template< typename T, XE::uint64 _Max > friend class QueueHandleAllocator;

	template< typename T, XE::uint64 _Max > friend class ConcurrentHandleAllocator;

	template< typename To > friend XE::Handle< To > HandleCast( const XE::uint64 & val );

	template< typename To, typename From > friend XE::Handle< To > HandleCast( const XE::Handle< From > & val );

public:
	using Type = T;

public:
	static const Handle< T > Invalid;

public:
	Handle()
		:_value( std::numeric_limits< XE::uint64 >::max() )
	{
	}

	Handle( const Handle & val )
		:_value( val._value )
	{
	}

private:
	Handle( XE::uint64 val )
		:_value( val )
	{
	}

public:
	Handle & operator=( const Handle & val )
	{
		_value = val._value;
		return *this;
	}

	Handle & operator=( std::nullptr_t )
	{
		_value = Invalid;

		return *this;
	}

public:
	operator bool() const
	{
		return _value != std::numeric_limits< XE::uint64 >::max();
	}

	operator XE::uint64() const
	{
		return GetValue();
	}

public:
	bool operator< ( XE::uint64 val ) const
	{
		return _value < val;
	}

	bool operator >( XE::uint64 val ) const
	{
		return _value > val;
	}

	bool operator<=( XE::uint64 val ) const
	{
		return _value <= val;
	}

	bool operator>=( XE::uint64 val ) const
	{
		return _value >= val;
	}

	bool operator!=( XE::uint64 val ) const
	{
		return _value != val;
	}

	bool operator==( XE::uint64 val ) const
	{
		return _value == val;
	}

public:
	bool operator< ( const Handle & val ) const
	{
		return _value < val._value;
	}

	bool operator >( const Handle & val ) const
	{
		return _value > val._value;
	}

	bool operator<=( const Handle & val ) const
	{
		return _value <= val._value;
	}

	bool operator>=( const Handle & val ) const
	{
		return _value >= val._value;
	}

	bool operator!=( const Handle & val ) const
	{
		return _value != val._value;
	}

	bool operator==( const Handle & val ) const
	{
		return _value == val._value;
	}

public:
	void Reset()
	{
		_value = Invalid._value;
	}

	XE::uint64 GetValue() const
	{
		return _value;
	}

private:
	XE::uint64 _value;
};


template< typename To > XE::Handle< To > HandleCast( const XE::uint64 & val )
{
	return { val };
}

template< typename To, typename From > XE::Handle< To > HandleCast( const XE::Handle< From > & val )
{
	return { val.GetValue() };
}


template< typename T > class HandleAllocator< Handle< T > >
{
public:
	HandleAllocator()
		:_Value( 0 )
	{ }

	HandleAllocator( XE::uint64 val )
		:_Value( val )
	{ }

public:
	XE::Handle< T > Alloc()
	{
		return _Value++;
	}

	void Reset( XE::uint64 val = 0 )
	{
		_Value = val;
	}

	XE::uint64 GetValue() const
	{
		return _Value;
	}

private:
	XE::uint64 _Value;
};

template< typename T, XE::uint64 _Max > class RingHandleAllocator< Handle< T >, _Max >
{
public:
	static constexpr XE::uint64 MAX_SIZE = _Max;

public:
	RingHandleAllocator()
	{
		Reset();
	}

public:
	XE::Handle< T > Alloc()
	{
		XE::uint64 index = _Index;

		while( _Index.compare_exchange_weak( index, ( index + 1 ) % MAX_SIZE ) );

		return XE::HandleCast< T >( index );
	}

	XE::Handle< T > Current() const
	{
		return XE::HandleCast< T >( _Index );
	}

	void Reset()
	{
		_Index = 0;
	}

private:
	std::atomic< XE::uint64 > _Index;
};

template< typename T, XE::uint64 _Max > class QueueHandleAllocator< Handle< T >, _Max >
{
public:
	static constexpr XE::uint64 MAX_SIZE = _Max;

public:
	QueueHandleAllocator() = default;

public:
	XE::Handle< T > Alloc()
	{
		XE::uint64 id = 0;
		if ( _Queue.try_pop( id ) )
		{
			return HandleCast< T >( id );
		}
		else if ( _Cur < MAX_SIZE )
		{
			id = _Cur.load();

			while ( !_Cur.compare_exchange_weak( id, id + 1 ) )
			{
				if ( id >= MAX_SIZE )
					return Handle< T >::Invalid;
			}

			return HandleCast< T >( id );
		}

		return Handle< T >::Invalid;
	}

	void Free( XE::Handle< T > handle )
	{
		_Queue.push( handle.GetValue() );
	}

	void Reset()
	{
		_Cur = 0;
		_Queue = {};
	}

private:
	std::atomic<XE::uint64> _Cur = 0;
	XE::ConcurrentQueue< XE::uint64 > _Queue;
};


IMPLEMENT_XE_TEMPLATE_CLASS( Handle );
IMPLEMENT_XE_TEMPLATE_CLASS( HandleAllocator );


template< typename T > struct Serializable< XE::Handle< T > >
{
public:
	static void Serialize( XE::OArchive & arc, const XE::Handle< T > & val )
	{
		arc << val.GetValue();
	}

	static void Deserialize( XE::IArchive & arc, XE::Handle< T > & val )
	{
		XE::uint64 value = 0;
		arc >> value;
		val = XE::HandleCast< T >( value );
	}
};

template< typename T > struct Serializable< XE::HandleAllocator< T > >
{
public:
	static void Serialize( XE::OArchive & arc, const XE::HandleAllocator< T > & val )
	{
		arc << val.GetValue();
	}

	static void Deserialize( XE::IArchive & arc, XE::HandleAllocator< T > & val )
	{
		XE::uint64 value = 0;
		arc >> value;
		val.Reset( value );
	}
};

END_XE_NAMESPACE

template< typename T > bool operator< ( XE::uint64 left, XE::Handle< T > right )
{
	return left < right.GetValue();
}

template< typename T > bool operator >( XE::uint64 left, XE::Handle< T > right )
{
	return left > right.GetValue();
}

template< typename T > bool operator<=( XE::uint64 left, XE::Handle< T > right )
{
	return left <= right.GetValue();
}

template< typename T > bool operator>=( XE::uint64 left, XE::Handle< T > right )
{
	return left >= right.GetValue();
}

template< typename T > bool operator!=( XE::uint64 left, XE::Handle< T > right )
{
	return left != right.GetValue();
}

template< typename T > bool operator==( XE::uint64 left, XE::Handle< T > right )
{
	return left == right.GetValue();
}

template< typename T > const XE::Handle< T > XE::Handle< T >::Invalid( std::numeric_limits< XE::uint64 >::max() );



#define DECL_HANDLE( NAME ) class NAME; using NAME##Handle = XE::Handle< NAME >; using NAME##HandleAllocator = XE::HandleAllocator< XE::Handle< NAME > >;

#endif // __HANDLE_HPP__7920C1A9_2044_45F0_84C4_221961C006AF
