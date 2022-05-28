/*!
 * \file	Span.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/05/28
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SPAN_HPP__54C7CB94_FE5C_45DD_AD7B_D1A840D7C02D
#define SPAN_HPP__54C7CB94_FE5C_45DD_AD7B_D1A840D7C02D

#include "Type.h"

BEG_XE_NAMESPACE

constexpr XE::uint64 SpanDynamicExtent = static_cast<XE::uint64>( -1 );

template <typename _Ty> struct SpanIterator
{
	using iterator_category = std::random_access_iterator_tag;
	using value_type = std::remove_cv_t<_Ty>;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using reference = _Ty &;

	constexpr reference operator*() const noexcept
	{
		return *_Myptr;
	}

	constexpr pointer operator->() const noexcept
	{
		return _Myptr;
	}

	constexpr SpanIterator & operator++() noexcept
	{
		++_Myptr;
		return *this;
	}

	constexpr SpanIterator operator++( int ) noexcept
	{
		SpanIterator _Tmp{ *this };
		++ * this;
		return _Tmp;
	}

	constexpr SpanIterator & operator--() noexcept
	{
		--_Myptr;
		return *this;
	}

	constexpr SpanIterator operator--( int ) noexcept
	{
		SpanIterator _Tmp{ *this };
		-- * this;
		return _Tmp;
	}

	constexpr SpanIterator & operator+=( const difference_type _Off ) noexcept
	{
		_Myptr += _Off;
		return *this;
	}

	constexpr SpanIterator operator+( const difference_type _Off ) const noexcept
	{
		SpanIterator _Tmp{ *this };
		_Tmp += _Off;
		return _Tmp;
	}

	friend constexpr SpanIterator operator+( const difference_type _Off, SpanIterator _Next ) noexcept
	{
		return _Next += _Off;
	}

	constexpr SpanIterator & operator-=( const difference_type _Off ) noexcept
	{
		_Myptr -= _Off;
		return *this;
	}

	constexpr SpanIterator operator-( const difference_type _Off ) const noexcept
	{
		SpanIterator _Tmp{ *this };
		_Tmp -= _Off;
		return _Tmp;
	}

	constexpr difference_type operator-( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr - _Right._Myptr;
	}

	constexpr reference operator[]( const difference_type _Off ) const noexcept
	{
		return *( *this + _Off );
	}

	constexpr bool operator==( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr == _Right._Myptr;
	}

	constexpr bool operator!=( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr != _Right._Myptr;
	}

	constexpr bool operator<=( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr <= _Right._Myptr;
	}

	constexpr bool operator>=( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr >= _Right._Myptr;
	}

	constexpr bool operator<( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr < _Right._Myptr;
	}

	constexpr bool operator>( const SpanIterator & _Right ) const noexcept
	{
		return _Myptr > _Right._Myptr;
	}

	pointer _Myptr = nullptr;
};

template <typename _Ty, XE::uint64 _Extent> struct SpanExtentType
{
	using pointer = _Ty *;

	constexpr SpanExtentType() noexcept = default;

	constexpr SpanExtentType( const pointer _Data, XE::uint64 ) noexcept : _Mydata{ _Data } {}

	pointer _Mydata{ nullptr };
	static constexpr XE::uint64 _Mysize = _Extent;
};

template <typename _Ty> struct SpanExtentType<_Ty, SpanDynamicExtent>
{
	using pointer = _Ty *;

	constexpr SpanExtentType() noexcept = default;

	constexpr SpanExtentType( const pointer _Data, const XE::uint64 _Size ) noexcept
		: _Mydata{ _Data }, _Mysize{ _Size } {}

	pointer _Mydata{ nullptr };
	XE::uint64 _Mysize{ 0 };
};

template <typename> struct IsSpan : std::false_type {};

template <typename _Ty, XE::uint64 _Extent> struct IsSpan<Span<_Ty, _Extent>> : std::true_type {};

template <typename> struct IsStdArray : std::false_type {};

template <typename _Ty, XE::uint64 _Size> struct IsStdArray<std::array<_Ty, _Size>> : std::true_type {};

template <typename _Rng, typename _Ty> struct IsSpanConvertibleRange
	: std::bool_constant<std::is_convertible_v<std::remove_pointer_t<decltype( std::data( std::declval<_Rng &>() ) )>( * )[], _Ty( * )[]>>
{
};

template <typename, typename = void> struct HasContainerInterface : std::false_type {};

template <typename _Rng> struct HasContainerInterface<_Rng, std::void_t<decltype( std::data( std::declval<_Rng &>() ) ), decltype( std::size( std::declval<_Rng &>() ) )>> : std::true_type {};

template <typename _Rng, typename _Ty> inline constexpr bool IsSpanCompatibleRange = std::conjunction_v<std::negation<std::is_array<_Rng>>, std::negation<IsSpan<std::remove_const_t<_Rng>>>, std::negation<IsStdArray<remove_const_t<_Rng>>>, HasContainerInterface<_Rng>, IsSpanConvertibleRange<_Rng, _Ty>>;

template <typename _Ty, XE::uint64 _Extent = SpanDynamicExtent> class Span : private SpanExtentType<_Ty, _Extent>
{
private:
	using _Mybase = SpanExtentType<_Ty, _Extent>;
	using _Mybase::_Mydata;
	using _Mybase::_Mysize;

public:
	using element_type = _Ty;
	using value_type = std::remove_cv_t<_Ty>;
	using size_type = XE::uint64;
	using difference_type = std::ptrdiff_t;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using reference = _Ty &;
	using const_reference = const _Ty &;
	using iterator = SpanIterator<_Ty>;
	using reverse_iterator = std::reverse_iterator<iterator>;

	static constexpr size_type extent = _Extent;

	template <XE::uint64 _Ext = _Extent, std::enable_if_t<_Ext == 0 || _Ext == SpanDynamicExtent, int> = 0>
	constexpr Span() noexcept {}

	constexpr Span( pointer _Ptr, size_type _Count ) noexcept
		: _Mybase( _Ptr, _Count )
	{
	}

	constexpr Span( pointer _First, pointer _Last ) noexcept
		: _Mybase( _First, static_cast<size_type>( _Last - _First ) )
	{

	}

	template <XE::uint64 _Size, std::enable_if_t<_Extent == SpanDynamicExtent || _Extent == _Size, int> = 0> constexpr Span( element_type( &_Arr )[_Size] ) noexcept : _Mybase( _Arr, _Size ) {}

	template <class _OtherTy, XE::uint64 _Size, std::enable_if_t<std::conjunction_v<std::bool_constant<_Extent == SpanDynamicExtent || _Extent == _Size>, std::is_convertible<_OtherTy( * )[], element_type( * )[]>>, int> = 0>
		constexpr Span( std::array<_OtherTy, _Size> & _Arr ) noexcept : _Mybase( _Arr.data(), _Size ) {}

	template <class _OtherTy, XE::uint64 _Size, std::enable_if_t<std::conjunction_v<std::bool_constant<_Extent == SpanDynamicExtent || _Extent == _Size>, std::is_convertible<const _OtherTy( * )[], element_type( * )[]>>, int> = 0>
		constexpr Span( const std::array<_OtherTy, _Size> & _Arr ) noexcept : _Mybase( _Arr.data(), _Size ) {}

	template <class _Rng, std::enable_if_t<IsSpanCompatibleRange<_Rng, element_type> && _Extent != SpanDynamicExtent, int> = 0>
	constexpr Span( _Rng & _Range )
		: _Mybase( std::data( _Range ), static_cast<size_type>( std::size( _Range ) ) )
	{

	}

	template <class _Rng, std::enable_if_t<IsSpanCompatibleRange<const _Rng, element_type> && _Extent != SpanDynamicExtent, int> = 0>
	constexpr Span( const _Rng & _Range )
		: _Mybase( std::data( _Range ), static_cast<size_type>( std::size( _Range ) ) )
	{

	}

	template <class _OtherTy, XE::uint64 _OtherExtent, std::enable_if_t<std::conjunction_v<std::bool_constant<_Extent == SpanDynamicExtent || _OtherExtent == SpanDynamicExtent || _Extent == _OtherExtent>, std::is_convertible<_OtherTy( * )[], element_type( * )[]>> && _Extent != SpanDynamicExtent && _OtherExtent == SpanDynamicExtent, int> = 0>
		constexpr Span( const Span<_OtherTy, _OtherExtent> & _Other ) noexcept
		: _Mybase( _Other.data(), _Other.size() )
	{

	}


	template <XE::uint64 _Count> constexpr auto first() const noexcept
	{
		if constexpr ( _Extent != SpanDynamicExtent )
		{
			static_assert( _Count <= _Extent, "Count out of range in span::first()" );
		}

		return Span<element_type, _Count>{_Mydata, _Count};
	}

	constexpr auto first( const size_type _Count ) const noexcept
	{
		return Span<element_type, SpanDynamicExtent>{_Mydata, _Count};
	}

	template <XE::uint64 _Count> constexpr auto last() const noexcept
	{
		if constexpr ( _Extent != SpanDynamicExtent )
		{
			static_assert( _Count <= _Extent, "Count out of range in span::last()" );
		}

		return Span<element_type, _Count>{_Mydata + ( _Mysize - _Count ), _Count};
	}

	constexpr auto last( const size_type _Count ) const noexcept
	{
		return Span<element_type, SpanDynamicExtent>{_Mydata + ( _Mysize - _Count ), _Count};
	}

	template <XE::uint64 _Offset, XE::uint64 _Count = SpanDynamicExtent> constexpr auto subspan() const noexcept
	{
		if constexpr ( _Extent != SpanDynamicExtent )
		{
			static_assert( _Offset <= _Extent, "Offset out of range in span::subspan()" );
			static_assert(
				_Count == SpanDynamicExtent || _Count <= _Extent - _Offset, "Count out of range in span::subspan()" );
		}

		using _ReturnType = Span<element_type, _Count != SpanDynamicExtent ? _Count : ( _Extent != SpanDynamicExtent ? _Extent - _Offset : SpanDynamicExtent )>;

		return _ReturnType{ _Mydata + _Offset, _Count == SpanDynamicExtent ? _Mysize - _Offset : _Count };
	}

	constexpr auto subspan( const size_type _Offset, const size_type _Count = SpanDynamicExtent ) const noexcept
	{
		using _ReturnType = Span<element_type, SpanDynamicExtent>;

		return _ReturnType{ _Mydata + _Offset, _Count == SpanDynamicExtent ? _Mysize - _Offset : _Count };
	}

	constexpr XE::uint64 size() const noexcept
	{
		return _Mysize;
	}

	constexpr size_type size_bytes() const noexcept
	{
		return _Mysize * sizeof( element_type );
	}

	constexpr bool empty() const noexcept
	{
		return _Mysize == 0;
	}

	constexpr reference operator[]( const size_type _Off ) const noexcept
	{
		return _Mydata[_Off];
	}

	constexpr reference front() const noexcept
	{
		return _Mydata[0];
	}

	constexpr reference back() const noexcept
	{
		return _Mydata[_Mysize - 1];
	}

	constexpr pointer data() const noexcept
	{
		return _Mydata;
	}

	constexpr iterator begin() const noexcept
	{
		return { _Mydata };
	}

	constexpr iterator end() const noexcept
	{
		return { _Mydata + _Mysize };
	}

	constexpr reverse_iterator rbegin() const noexcept
	{
		return reverse_iterator{ end() };
	}

	constexpr reverse_iterator rend() const noexcept
	{
		return reverse_iterator{ begin() };
	}

	constexpr pointer _Unchecked_begin() const noexcept
	{
		return _Mydata;
	}

	constexpr pointer _Unchecked_end() const noexcept
	{
		return _Mydata + _Mysize;
	}
};

END_XE_NAMESPACE

#endif//SPAN_HPP__54C7CB94_FE5C_45DD_AD7B_D1A840D7C02D