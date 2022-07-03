/*!
 * \file	RefCount.h
 *
 * \author	ZhengYuanQing
 * \date	2021/11/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef REFCOUNT_H__416B3227_FC29_474C_B2CC_243CC7F99FD1
#define REFCOUNT_H__416B3227_FC29_474C_B2CC_243CC7F99FD1

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RefCount : public XE::NonCopyable
{
	template< typename _Ty > friend class RefPtr;

public:
	RefCount() = default;

	virtual ~RefCount() = default;

public:
	void Reset();

	XE::uint64 Inc();

	XE::uint64 Dec();
	
	XE::uint64 Count() const;

public:
	template< typename _Dx > void SetDeleter( _Dx val )
	{
		_Deleter = val;
	}

private:
	std::atomic< XE::uint64 > _Count = 0;
	XE::Delegate< void( RefCount * ) > _Deleter;
};

template <class _Ty> class RefPtr
{
public:
	using element_type = std::remove_extent_t<_Ty>;

public:
	constexpr RefPtr() noexcept = default;

	constexpr RefPtr( nullptr_t ) noexcept {}

	template <class _Ux, std::enable_if_t<std::is_convertible<_Ux *, _Ty *>::value, int> = 0> explicit RefPtr( _Ux * _Px )
		:_Ptr( dynamic_cast<_Ty *>( _Px ) )
	{
		if ( _Ptr != nullptr )
		{
			_Ptr->Inc();
		}
	}

	RefPtr( RefPtr && _Right ) noexcept
	{
		RefPtr( std::move( _Right ) ).swap( *this );
	}

	RefPtr( const RefPtr & _Other ) noexcept
	{
		reset( _Other._Ptr );
	}

	~RefPtr() noexcept
	{
		if ( _Ptr != nullptr )
		{
			_Ptr->Dec();
		}
	}

public:
	RefPtr & operator=( RefPtr && _Right ) noexcept
	{
		RefPtr( std::move( _Right ) ).swap( *this );
		return *this;
	}

	RefPtr & operator=( const RefPtr & _Right ) noexcept
	{
		reset( _Right._Ptr );

		return *this;
	}

public:
	void swap( RefPtr & _Other ) noexcept
	{
		std::swap( _Ptr, _Other._Ptr );
	}

public:
	void reset() noexcept
	{
		if ( _Ptr != nullptr )
		{
			_Ptr->Dec();
			_Ptr = nullptr;
		}
	}

	template <class _Ux> void reset( _Ux * _Px )
	{
		if ( _Ptr != nullptr )
		{
			_Ptr->Dec();
			_Ptr = nullptr;
		}

		_Ptr = dynamic_cast<_Ty *>( _Px );
		_Ptr->Inc();
	}

public:
	XE::uint64 use_count() const noexcept
	{
		return _Ptr->Count();
	}

	element_type * get() const noexcept
	{
		return _Ptr;
	}

public:
	_Ty & operator*() const noexcept
	{
		return *get();
	}

	_Ty * operator->() const noexcept
	{
		return get();
	}

	explicit operator bool() const noexcept
	{
		return get() != nullptr;
	}

private:
	element_type * _Ptr = nullptr;
};

END_XE_NAMESPACE

#define DECL_REF( TYPE ) class TYPE; using TYPE##RPtr = XE::RefPtr< TYPE >;

#endif // REFCOUNT_H__416B3227_FC29_474C_B2CC_243CC7F99FD1
