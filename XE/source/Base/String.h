/*!
* \file	String.hpp
*
* \author	ZhengYuanQing
* \date	2021/02/19
* \email	zhengyuanqing.95@gmail.com
*
*/
#ifndef STRING_HPP__2D116B84_5AD0_4FFF_A245_8EC14E092378
#define STRING_HPP__2D116B84_5AD0_4FFF_A245_8EC14E092378

#include <regex>

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API CodePoint
{
public:
	CodePoint();

	CodePoint( char c );

	CodePoint( wchar_t c );

	CodePoint( char16_t c );

	CodePoint( char32_t c );

	CodePoint( XE::uint32 c );

	CodePoint( const CodePoint & other );

public:
	CodePoint & operator=( const CodePoint & c );

public:
	bool operator!=( const CodePoint & other ) const;

	bool operator==( const CodePoint & other ) const;

	bool operator< ( const CodePoint & other ) const;

	bool operator<=( const CodePoint & other ) const;

	bool operator >( const CodePoint & other ) const;

	bool operator>=( const CodePoint & other ) const;

public:
	XE::uint32 unicode() const;

private:
	union
	{
		char c;
		wchar_t wc;
		char16_t c16;
		char32_t c32;
		XE::uint32 _unicode;
	};
};

class XE_API AnsiEncode
{
public:
	using size_type = std::size_t;
	using storage_unit = char;

public:
	template <typename Iterator >
	static Iterator advance( Iterator iter_begin, Iterator iter_end, size_type count )
	{
		auto iter = iter_begin;
		storage_unit value = 0;

		while( iter != iter_end && count != 0 )
		{
			value = *iter;
			if( value >= 0x80 && value <= 0xFF )
			{
				++iter;
			}

			++iter;
			--count;
		}

		return iter;
	}

	template <typename Iterator >
	static size_type distance( Iterator iter_begin, Iterator iter_end )
	{
		size_type retval = 0;

		for( auto iter = iter_begin; iter != iter_end; )
		{
			storage_unit value = *iter;

			if( value >= 0x80 && value <= 0xFF )
			{
				++iter;
			}

			++iter;
			++retval;
		}

		return retval;
	}

	template <typename Container >
	static typename Container::const_iterator insert( Container & str1, typename Container::const_iterator iter, CodePoint c, size_type count = 1 )
	{
		uint32_t value = c.unicode();

		for( size_type x = 0; x < count; ++x )
		{
			if( value <= 0x7F )
			{
				iter = str1.insert( iter, value );
			}
			else
			{
				iter = str1.insert( iter, value & 0xFF );
				iter = str1.insert( iter, ( value >> 8 ) & 0xFF );
			}
		}

		return iter;
	}

	template< typename storage_iterator >
	static size_type walk( size_type len, storage_iterator iter )
	{
		size_type retval = 0;
		size_type count = 0;

		if( len >= 0 )
		{
			for( size_type x = 0; x < len; ++x )
			{
				storage_unit value = *iter;

				count = num_of_bytes( value );
				iter += count;

				retval += count;
			}
		}
		else
		{
			for( size_type x = 0; x > len; --x )
			{
				uint16_t value = *iter;

				if( value >= 0x80 && value <= 0xFF )
				{
					--iter;
					--retval;
				}

				--iter;
				--retval;
			}
		}

		return retval;
	}

	static CodePoint get_code_point( const storage_unit * iter );

	static size_type num_of_bytes( storage_unit value );
};

class XE_API WideEncode
{
public:
	using size_type = std::size_t;
	using storage_unit = wchar_t;

public:
	template <typename Iterator >
	static Iterator advance( Iterator iter_begin, Iterator iter_end, size_type count )
	{
		auto iter = iter_begin;
		storage_unit value = 0;

		while( iter != iter_end && count != 0 )
		{
			--count;
			++iter;
		}

		return iter;
	}

	template <typename Iterator >
	static size_type distance( Iterator iter_begin, Iterator iter_end )
	{
		return iter_end - iter_begin;
	}

	template <typename Container >
	static typename Container::const_iterator insert( Container & str1, typename Container::const_iterator iter, CodePoint c, size_type count = 1 )
	{
		uint32_t value = c.unicode();

		for( size_type x = 0; x < count; ++x )
		{
			iter = str1.insert( iter, value );
		}

		return iter;
	}

	template< typename storage_iterator >
	static size_type walk( size_type len, storage_iterator iter )
	{
		iter += len;

		return len;
	}

	static CodePoint get_code_point( const storage_unit * iter );

	static size_type num_of_bytes( storage_unit value );
};

class XE_API Utf8Encode
{
public:
	using size_type = std::size_t;
	using storage_unit = char;

public:
	template <typename Iterator >
	static Iterator advance( Iterator iter_begin, Iterator iter_end, size_type count )
	{
		auto iter = iter_begin;
		unsigned char value = 0;

		while( iter != iter_end && count != 0 )
		{
			value = static_cast< unsigned char >( *iter );
			if( value < 0x80 || value > 0xBf )
			{
				--count;
			}

			++iter;
		}

		if( value >= 0xBF )
		{
			while( iter != iter_end && *iter >= 0x80 && *iter <= 0xBF )
			{
				++iter;
			}
		}

		return iter;
	}

	template <typename Iterator >
	static size_type distance( Iterator iter_begin, Iterator iter_end )
	{
		size_type retval = 0;

		for( auto iter = iter_begin; iter != iter_end; ++iter )
		{
			unsigned char value = static_cast< unsigned char >( *iter );

			if( value < 0x80 || value > 0xBF )
			{
				++retval;
			}
		}

		return retval;
	}

	template <typename Container >
	static typename Container::const_iterator insert( Container & str1, typename Container::const_iterator iter, CodePoint c, size_type count = 1 )
	{
		uint32_t value = c.unicode();

		for( size_type x = 0; x < count; ++x )
		{
			if( value <= 0x007F )
			{
				iter = str1.insert( iter, value );
			}
			else if( value <= 0x07FF )
			{
				iter = str1.insert( iter, ( ( value ) & 0x3F ) | 0x80 );
				iter = str1.insert( iter, ( ( value >> 6 ) & 0x1F ) | 0xC0 );
			}
			else if( value <= 0xFFFF )
			{
				iter = str1.insert( iter, ( ( value ) & 0x3F ) | 0x80 );
				iter = str1.insert( iter, ( ( value >> 6 ) & 0x3F ) | 0x80 );
				iter = str1.insert( iter, ( ( value >> 12 ) & 0x0F ) | 0xE0 );
			}
			else
			{
				iter = str1.insert( iter, ( ( value ) & 0x3F ) | 0x80 );
				iter = str1.insert( iter, ( ( value >> 6 ) & 0x3F ) | 0x80 );
				iter = str1.insert( iter, ( ( value >> 12 ) & 0x3F ) | 0x80 );
				iter = str1.insert( iter, ( ( value >> 18 ) & 0x07 ) | 0xF0 );
			}
		}

		return iter;
	}

	template< typename storage_iterator >
	static size_type walk( size_type len, storage_iterator iter )
	{
		size_type retval = 0;
		size_type count = 0;

		if( len >= 0 )
		{
			for( size_type x = 0; x < len; ++x )
			{
				unsigned char value = static_cast< unsigned char >( *iter );

				count = num_of_bytes( value );
				iter += count;

				retval += count;
			}
		}
		else
		{
			for( size_type x = 0; x > len; --x )
			{
				while( true )
				{
					--iter;
					--retval;

					unsigned char value = static_cast< unsigned char >( *iter );

					if( ( value & 0xC0 ) != 0x80 )
					{
						break;
					}
				}
			}
		}

		return retval;
	}

	static CodePoint get_code_point( const storage_unit * iter );

	static size_type num_of_bytes( storage_unit value );
};

class XE_API StdStringConvert
{
public:
	// local multi byte code point to local wide char code point
	static std::wstring ansi_2_wide( const std::string & str );

	// local wide char code point to local multi byte code point
	static std::string wide_2_ansi( const std::wstring & str );

	// local multi byte code point to unicode utf-8
	static std::string ansi_2_utf8( const std::string & str );

	// unicode utf-8 to local multi byte code point
	static std::string utf8_2_ansi( const std::string & str );

	// local wide char code point to unicode utf-8
	static std::string wide_2_utf8( const std::wstring & str );

	// unicode utf-8 code point to local wide char
	static std::wstring utf8_2_wide( const std::string & str );
};

template< typename Unit > class CodePointArrow
{
public:
	CodePointArrow( Unit * p )
		:_Ptr( p )
	{}

public:
	const Unit * operator->() const
	{
		return _Ptr;
	}

private:
	Unit * _Ptr;
};

template< typename String > class CodePointReference
{
	friend String;

public:
	using encode_type = typename String::encode_type;
	using difference_type = typename String::difference_type;
	using size_type = typename String::size_type;
	using value_type = typename String::value_type;
	using storage_unit = typename String::storage_unit;
	using iterator = typename String::iterator;

private:
	CodePointReference( String * _Str, iterator _Iter )
		:_Str( _Str ), _Iter( _Iter )
	{

	}

public:
	operator CodePoint() const
	{
		return *_Iter;
	}

	CodePointReference & operator=( const CodePointReference & _Other )
	{
		_Str = _Other._Str;
		_Iter = _Other._Iter;
		return *this;
	}

	CodePointReference & operator=( const CodePoint & _Other )
	{
		_Str->replace( _Iter - _Str->begin(), 1, _Other );

		return *this;
	}

private:
	String * _Str;
	iterator _Iter;
};

template<> class StringConvert< AnsiEncode, AnsiEncode >
{
public:
	static std::basic_string< typename AnsiEncode::storage_unit > convert( const std::basic_string< typename AnsiEncode::storage_unit > & str )
	{
		return str;
	}
};

template<> class StringConvert< AnsiEncode, WideEncode >
{
public:
	static std::basic_string< typename WideEncode::storage_unit > convert( const std::basic_string< typename AnsiEncode::storage_unit > & str )
	{
		return StdStringConvert::ansi_2_wide( str );
	}
};

template<> class StringConvert< AnsiEncode, Utf8Encode >
{
public:
	static std::basic_string< typename Utf8Encode::storage_unit > convert( const std::basic_string< typename AnsiEncode::storage_unit > & str )
	{
		return StdStringConvert::ansi_2_utf8( str );
	}
};

template<> class StringConvert< WideEncode, AnsiEncode >
{
public:
	static std::basic_string< typename AnsiEncode::storage_unit > convert( const std::basic_string< typename WideEncode::storage_unit > & str )
	{
		return StdStringConvert::wide_2_ansi( str );
	}
};

template<> class StringConvert< WideEncode, WideEncode >
{
public:
	static std::basic_string< typename WideEncode::storage_unit > convert( const std::basic_string< typename WideEncode::storage_unit > & str )
	{
		return str;
	}
};

template<> class StringConvert< WideEncode, Utf8Encode >
{
public:
	static std::basic_string< typename Utf8Encode::storage_unit > convert( const std::basic_string< typename WideEncode::storage_unit > & str )
	{
		return StdStringConvert::wide_2_utf8( str );
	}
};

template<> class StringConvert< Utf8Encode, AnsiEncode >
{
public:
	static std::basic_string< typename AnsiEncode::storage_unit > convert( const std::basic_string< typename Utf8Encode::storage_unit > & str )
	{
		return StdStringConvert::utf8_2_ansi( str );
	}
};

template<> class StringConvert< Utf8Encode, WideEncode >
{
public:
	static std::basic_string< typename WideEncode::storage_unit > convert( const std::basic_string< typename Utf8Encode::storage_unit > & str )
	{
		return StdStringConvert::utf8_2_wide( str );
	}
};

template<> class StringConvert< Utf8Encode, Utf8Encode >
{
public:
	static std::basic_string< typename Utf8Encode::storage_unit > convert( const std::basic_string< typename Utf8Encode::storage_unit > & str )
	{
		return str;
	}
};

template < typename Encode, typename Alloc > class BasicStringIterator
{
private:
	friend class BasicString< Encode, Alloc >;

	using v_iter = typename std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc >::const_iterator;

public:
	using difference_type = std::ptrdiff_t;
	using pointer = CodePoint *;
	using reference = CodePoint;
	using size_type = std::size_t;
	using value_type = CodePoint;
	using iterator_category = std::random_access_iterator_tag;

public:
	BasicStringIterator() = default;

public:
	CodePoint operator*() const
	{
		return CodePoint( *_iter );
	}

	CodePointArrow< typename Encode::storage_unit > operator->() const
	{
		return { _iter.template operator->() };
	}

public:
	CodePoint operator[]( size_type n ) const
	{
		return *( *this + n );
	}

public:
	bool operator!=( const BasicStringIterator & other ) const
	{
		return _iter != other._iter;
	}

	bool operator==( const BasicStringIterator & other ) const
	{
		return _iter == other._iter;
	}

	bool operator< ( const BasicStringIterator & other ) const
	{
		return _iter < other._iter;
	}

	bool operator<=( const BasicStringIterator & other ) const
	{
		return _iter <= other._iter;
	}

	bool operator >( const BasicStringIterator & other ) const
	{
		return _iter > other._iter;
	}

	bool operator>=( const BasicStringIterator & other ) const
	{
		return _iter >= other._iter;
	}

public:
	BasicStringIterator & operator+=( size_type n )
	{
		_iter += Encode::walk( n, _iter );
		return *this;
	}

	BasicStringIterator & operator-=( size_type n )
	{
		_iter += Encode::walk( -n, _iter );
		return *this;
	}

	BasicStringIterator operator+( size_type n ) const
	{
		auto iter = _iter + Encode::walk( n, _iter );
		return BasicStringIterator( iter );
	}

	BasicStringIterator & operator++()
	{
		_iter += Encode::walk( 1, _iter );
		return *this;
	}

	BasicStringIterator operator++( int )
	{
		BasicStringIterator retval = *this;
		_iter += Encode::walk( 1, _iter );

		return retval;
	}

	BasicStringIterator operator-( size_type n ) const
	{
		auto iter = _iter + Encode::walk( -(int)( n ), _iter );
		return BasicStringIterator( iter );
	}

	size_type operator-( BasicStringIterator other ) const
	{
		BasicStringIterator a = *this;
		BasicStringIterator b = other;

		if( a < b )
		{
			return 0 - Encode::distance( a._iter, b._iter );
		}
		else
		{
			return Encode::distance( b._iter, a._iter );
		}
	}

	BasicStringIterator & operator--()
	{
		_iter += Encode::walk( -1, _iter );
		return *this;
	}

	BasicStringIterator operator--( int )
	{
		BasicStringIterator retval = *this;
		_iter += Encode::walk( -1, _iter );

		return retval;
	}

public:
	std::pair< v_iter, v_iter > code_point_range() const
	{
		return { _iter, _iter + Encode::walk( 1, _iter ) };
	}

	v_iter code_point_begin() const
	{
		return _iter;
	}

	v_iter code_point_end() const
	{
		return _iter + Encode::walk( 1, _iter );
	}

private:
	explicit BasicStringIterator( v_iter data )
	{
		_iter = data;
	}

	v_iter _iter;
};

template < typename StringIterator > class BasicStringReverseIterator : public std::reverse_iterator< StringIterator >
{
public:
	BasicStringReverseIterator() = default;

	BasicStringReverseIterator( StringIterator iter )
		: std::reverse_iterator< StringIterator >( iter )
	{
	}

	template <typename U >
	BasicStringReverseIterator( BasicStringReverseIterator< U > iter )
		: std::reverse_iterator< StringIterator >( iter.base() )
	{
	}

public:
	decltype( std::declval< StringIterator >().operator*() )  operator*() const
	{
		auto tmp = this->base();
		return ( --tmp ).operator*();
	}

	decltype( std::declval< StringIterator >().operator->() ) operator->() const
	{
		auto tmp = this->base();
		return ( --tmp ).operator->();
	}
};

template < typename Encode, typename Alloc = std::pmr::polymorphic_allocator< typename Encode::storage_unit > > class BasicString
{
public:
	static const BasicString< Encode, Alloc > empty_string;

public:
	using encode_type = Encode;
	using allocator_type = Alloc;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;
	using value_type = CodePoint;
	using const_value_type = const CodePoint;
	using reference_value_type = CodePointReference< BasicString< Encode, Alloc > >;
	using storage_unit = typename Encode::storage_unit;

	using iterator = BasicStringIterator< Encode, Alloc >;
	using const_iterator = BasicStringIterator< Encode, Alloc >;
	using reverse_iterator = BasicStringReverseIterator< iterator >;
	using const_reverse_iterator = BasicStringReverseIterator< const_iterator >;

	using storage_type = std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc >;
	using storage_iterator = typename std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc >::iterator;
	using storage_const_iterator = typename std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc >::const_iterator;
	using storage_reverse_iterator = typename std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc >::reverse_iterator;
	using storage_const_reverse_iterator = typename std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc >::const_reverse_iterator;

public:
	static constexpr const size_type npos = -1;

public:
	BasicString()
	{
	}

	BasicString( const Alloc & a )
		: _string( a )
	{
	}

	BasicString( CodePoint code, const Alloc & a = Alloc() )
		: _string( a )
	{
		Encode::insert( _string, _string.end(), code );
	}

	BasicString( const BasicString & str, const Alloc & a = Alloc() )
		: _string( str._string, a )
	{
	}

	BasicString( BasicString && str, const Alloc & a = Alloc() )
		: _string( std::move( str._string ), a )
	{
	}

	BasicString( const storage_unit * str, const Alloc & a = Alloc() )
		: _string( str, a )
	{
	}

	BasicString( const storage_unit * str, XE::uint64 size, const Alloc & a = Alloc() )
		: _string( str, size, a )
	{
	}

	BasicString( const_iterator begin, const_iterator end, const Alloc & a = Alloc() )
		: _string( a )
	{
		for( const_iterator item = begin; item != end; ++item )
		{
			Encode::insert( _string, _string.end(), *item );
		}
	}

	template< typename UEncode, typename UAlloc > BasicString( const BasicString< UEncode, UAlloc > & val )
		: _string( XE::StringConvert< UEncode, Encode >::convert( val.c_str() ) )
	{

	}

	template< typename UAlloc > BasicString( const std::basic_string< storage_unit, std::char_traits< storage_unit >, UAlloc > & str, const Alloc & a = Alloc() )
		: _string( str.begin(), str.end(), a )
	{
	}

public:
	BasicString & operator=( CodePoint c )
	{
		_string.clear();

		append( c );
		return *this;
	}

	BasicString & operator=( BasicString && str ) = default;

	BasicString & operator=( const BasicString & str ) = default;

	BasicString & operator=( const storage_unit * str )
	{
		_string.clear();

		append( str );
		return *this;
	}

	template< typename UEncode, typename UAlloc > BasicString & operator =( const BasicString< UEncode, UAlloc > & val )
	{
		_string = XE::StringConvert< UEncode, Encode >::convert( val.c_str() );

		return *this;
	}

	template< typename UAlloc > BasicString & operator=( const std::basic_string< storage_unit, std::char_traits< storage_unit >, UAlloc > & str )
	{
		_string.clear();
		_string.insert( _string.end(), str.begin(), str.end() );

		return *this;
	}

public:
	BasicString operator+( CodePoint c ) const
	{
		BasicString result = *this;
		result.append( c );
		return result;
	}

	BasicString operator+( const BasicString & str ) const
	{
		BasicString result = *this;
		result.append( str );
		return result;
	}

	BasicString operator+( const storage_unit * str ) const
	{
		BasicString result = *this;
		result.append( str );
		return result;
	}

	BasicString operator+( const std::basic_string< typename Encode::storage_unit > & str ) const
	{
		BasicString result = *this;
		result.append( str );
		return result;
	}

public:
	BasicString & operator+=( CodePoint c )
	{
		append( c );
		return *this;
	}

	BasicString & operator+=( const BasicString & str )
	{
		append( str );
		return *this;
	}

	BasicString & operator+=( const storage_unit * str )
	{
		append( str );
		return *this;;
	}

	BasicString & operator+=( const std::basic_string< typename Encode::storage_unit > & str )
	{
		append( str );
		return *this;
	}

public:
	bool operator == ( const BasicString & val ) const
	{
		return _string == val._string;
	}

	bool operator != ( const BasicString & val ) const
	{
		return _string != val._string;
	}

	bool operator < ( const BasicString & val ) const
	{
		return _string < val._string;
	}

	bool operator <= ( const BasicString & val ) const
	{
		return _string <= val._string;
	}

	bool operator > ( const BasicString & val ) const
	{
		return _string > val._string;
	}

	bool operator >= ( const BasicString & val ) const
	{
		return _string >= val._string;
	}

public:
	Alloc get_allocator() const
	{
		return _string.get_allocator();
	}

public:
	reference_value_type operator[]( size_type index )
	{
		const_iterator iter = begin();
		std::advance( iter, index );

		return { this, iter };
	}

	const_value_type operator[]( size_type index ) const
	{
		const_iterator iter = begin();
		std::advance( iter, index );

		return *iter;
	}

	reference_value_type at( size_type index )
	{
		const_iterator iter = begin();
		std::advance( iter, index );

		return { this, iter };
	}

	const_value_type at( size_type index ) const
	{
		const_iterator iter = begin();
		std::advance( iter, index );

		return *iter;
	}

public:
	iterator advance( iterator begin, size_type count )
	{
		return iterator( Encode::advance( begin.code_point_begin(), end().code_point_begin(), count ) );
	}

	const_iterator advance( const_iterator begin, size_type count ) const
	{
		return const_iterator( Encode::advance( begin.code_point_begin(), end().code_point_begin(), count ) );
	}

public:
	BasicString & append( CodePoint c )
	{
		Encode::insert( _string, _string.end(), c );

		return *this;
	}

	BasicString & append( size_type size, CodePoint c )
	{
		Encode::insert( _string, _string.end(), c, size );

		return *this;
	}

	BasicString & append( const BasicString & str )
	{
		_string.append( str._string );

		return *this;
	}

	BasicString & append( const_iterator begin, const_iterator end )
	{
		for( const_iterator item = begin; item != end; ++item )
		{
			Encode::insert( _string, _string.end(), *item );
		}

		return *this;
	}

	BasicString & append( const storage_unit * str )
	{
		_string.append( str );

		return *this;
	}

	BasicString & append( const storage_unit * str, size_type size )
	{
		_string.append( str, size );

		return *this;
	}

	BasicString & append( const std::basic_string< typename Encode::storage_unit > & str )
	{
		_string.append( str );

		return *this;
	}

public:
	BasicString & assign( size_type count, CodePoint c )
	{
		clear();
		append( count, c );

		return *this;
	}

	BasicString & assign( BasicString && str )
	{
		clear();
		append( std::move( str ) );

		return *this;
	}

	BasicString & assign( const BasicString & str )
	{
		clear();
		append( str );

		return *this;
	}

	BasicString & assign( const storage_unit * str )
	{
		clear();
		append( str, std::strlen( str ) );

		return *this;
	}

	BasicString & assign( const storage_unit * str, size_type size )
	{
		clear();
		append( str, size );

		return *this;
	}

	template <int N >
	BasicString & assign( const storage_unit * ( &str )[N] )
	{
		clear();
		append( str, N );

		return *this;
	}

	BasicString & assign( const_iterator begin, const_iterator end )
	{
		clear();
		append( begin, end );

		return *this;
	}

	BasicString & assign( const std::basic_string< typename Encode::storage_unit > & str )
	{
		clear();
		append( str );

		return *this;
	}

public:
	void clear()
	{
		_string.clear();
	}

	bool empty() const
	{
		return _string.empty();
	}

	int strcmp( const BasicString & str ) const
	{
		if( str.size() == size() )
		{
			auto it1 = begin();
			auto it2 = str.begin();
			auto it1end = end();
			auto it2end = str.end();

			for( ; it1 != it1end && it2 != it2end; ++it1, ++it2 )
			{
				if( *it1 < *it2 )
				{
					return -1;
				}
				else if( *it1 > *it2 )
				{
					return 1;
				}
			}

			return 0;
		}

		return size() > str.size() ? 1 : -1;
	}

public:
	iterator erase( const_iterator iter )
	{
		auto retval = _string.erase( iter.code_point_begin() );
		return const_iterator( retval );
	}

	iterator erase( const_iterator iter_begin, const_iterator iter_end )
	{
		auto retval = _string.erase( iter_begin.code_point_begin(), iter_end.code_point_begin() );
		return const_iterator( retval );
	}

	BasicString & erase( size_type indexStart = 0, size_type size = npos )
	{
		const_iterator iter_begin = begin() + indexStart;
		const_iterator iter_end;

		if( iter_begin == end() )
		{
			return *this;
		}

		if( size >= 0 )
		{
			iter_end = iter_begin;

			for( size_type i = 0; i < size && iter_end != end(); ++i )
			{
				++iter_end;
			}
		}
		else
		{
			iter_end = end();
		}

		erase( iter_begin, iter_end );

		return *this;
	}

public:
	const_iterator find_fast( const BasicString & str ) const
	{
		return find_fast( str, begin() );
	}

	const_iterator find_fast( const BasicString & str, const_iterator iter_begin ) const
	{
		const_iterator iter_end = end();

		if( iter_begin == iter_end )
		{
			return iter_end;
		}

		if( str.empty() )
		{
			return iter_begin;
		}

		auto iter = iter_begin;
		auto ch = str[0];

		while( iter != iter_end )
		{
			if( *iter == ch )
			{
				auto text_iter = iter + 1;
				auto pattern_iter = str.begin() + 1;

				while( text_iter != iter_end && pattern_iter != str.end() )
				{

					if( *text_iter == *pattern_iter )
					{
						++text_iter;
						++pattern_iter;
					}
					else
					{
						break;
					}
				}

				if( pattern_iter == str.end() )
				{
					return iter;
				}
			}

			++iter;
		}

		return iter_end;
	}

public:
	const_iterator rfind_fast( const BasicString & str ) const
	{
		return rfind_fast( str, end() );
	}

	const_iterator rfind_fast( const BasicString & str, const_iterator iter_end ) const
	{
		const_iterator iter_begin = begin();

		if( iter_begin == iter_end )
		{
			return end();
		}

		if( str.empty() )
		{
			return iter_end;
		}

		auto iter = iter_end;
		auto str_end = str.end();
		auto ch = str[0];

		while( iter != begin() )
		{
			--iter;

			if( *iter == ch )
			{
				auto text_iter = iter + 1;
				auto pattern_iter = str.begin() + 1;

				while( text_iter != end() && pattern_iter != str_end )
				{
					if( *text_iter == *pattern_iter )
					{
						++text_iter;
						++pattern_iter;
					}
					else
					{
						break;
					}
				}

				if( pattern_iter == str_end )
				{
					return iter;
				}
			}

		}

		return end();
	}

public:
	size_type find( CodePoint c, size_type indexStart = 0 ) const
	{
		const_iterator iter_begin = begin();

		for( size_type i = 0; i < indexStart && iter_begin != end(); ++i )
		{
			++iter_begin;
		}

		if( iter_begin == end() )
		{
			return -1;
		}

		size_type retval = indexStart;

		while( iter_begin != end() )
		{
			if( *iter_begin == c )
			{
				return retval;
			}

			++iter_begin;
			++retval;
		}

		return -1;
	}

	size_type find( const BasicString & str, size_type indexStart = 0 ) const
	{
		size_type stringLen = this->size();

		if( str.empty() )
		{
			if( indexStart > stringLen )
			{
				return -1;
			}
			else
			{
				return indexStart;
			}
		}

		if( indexStart >= stringLen )
		{
			return -1;
		}

		size_type retval = indexStart;
		auto iter = begin() + indexStart;
		auto ch = str[0];

		while( iter != end() )
		{
			if( *iter == ch )
			{
				auto text_iter = iter + 1;
				auto pattern_iter = str.begin() + 1;

				while( text_iter != end() && pattern_iter != str.end() )
				{
					if( *text_iter == *pattern_iter )
					{
						++text_iter;
						++pattern_iter;
					}
					else
					{
						break;
					}
				}

				if( pattern_iter == str.end() )
				{
					return retval;
				}
			}

			++iter;
			++retval;
		}

		return -1;
	}

public:
	size_type rfind( CodePoint c, size_type indexStart = npos ) const
	{
		size_type stringLen = this->size();

		size_type retval;
		const_iterator iter;

		if( indexStart >= 0 && indexStart < stringLen )
		{
			retval = indexStart + 1;
			iter = begin() + indexStart + 1;
		}
		else
		{
			retval = stringLen;
			iter = end();
		}

		while( iter != begin() )
		{
			--iter;
			--retval;

			if( *iter == c )
			{
				return retval;
			}
		}

		return -1;
	}

	size_type rfind( const BasicString & str, size_type indexStart = npos ) const
	{
		size_type stringLen = this->size();

		if( str.empty() )
		{
			if( indexStart > stringLen || indexStart == -1 )
			{
				return stringLen;
			}
			else
			{
				return indexStart;
			}
		}

		size_type retval;
		const_iterator iter;

		if( indexStart >= 0 && indexStart < stringLen )
		{
			retval = indexStart + 1;
			iter = begin() + indexStart + 1;
		}
		else
		{
			retval = stringLen;
			iter = end();
		}

		const_iterator str_end = str.end();
		auto ch = str[0];

		while( iter != begin() )
		{
			--iter;
			--retval;

			if( *iter == ch )
			{
				auto text_iter = iter + 1;
				auto pattern_iter = str.begin() + 1;

				while( text_iter != end() && pattern_iter != str_end )
				{
					if( *text_iter == *pattern_iter )
					{
						++text_iter;
						++pattern_iter;
					}
					else
					{
						break;
					}
				}

				if( pattern_iter == str_end )
				{
					return retval;
				}
			}

		}

		return -1;
	}

public:
	size_type find_first_of( CodePoint c, size_type indexStart = 0 ) const
	{
		return find( c, indexStart );
	}

	size_type find_first_of( const BasicString & str, size_type indexStart = 0 ) const
	{
		if( str.empty() || indexStart >= this->size() )
		{
			return -1;
		}

		size_type retval = indexStart;
		auto iter = begin() + indexStart;

		while( iter != end() )
		{
			for( auto c : str )
			{
				if( *iter == c )
				{
					return retval;
				}
			}

			++iter;
			++retval;
		}

		return -1;
	}

public:
	size_type find_last_of( CodePoint c, size_type indexStart = npos ) const
	{
		return rfind( c, indexStart );
	}

	size_type find_last_of( const BasicString & str, size_type indexStart = npos ) const
	{
		size_type stringLen = this->size();

		if( str.empty() || indexStart >= stringLen )
		{
			return -1;
		}

		size_type retval;
		const_iterator iter;

		if( indexStart >= 0 && indexStart < stringLen )
		{
			retval = indexStart + 1;
			iter = begin() + indexStart + 1;
		}
		else
		{
			retval = stringLen;
			iter = end();
		}

		while( iter != begin() )
		{
			--iter;
			--retval;

			for( CodePoint c : str )
			{
				if( *iter == c )
				{
					return retval;
				}
			}
		}

		return -1;
	}

public:
	size_type find_first_not_of( CodePoint c, size_type indexStart = 0 ) const
	{
		if( indexStart >= this->size() )
		{
			return -1;
		}

		size_type retval = indexStart;
		auto iter = begin() + indexStart;

		while( iter != end() )
		{
			if( *iter != c )
			{
				return retval;
			}

			++iter;
			++retval;
		}

		return -1;
	}

	size_type find_first_not_of( const BasicString & str, size_type indexStart = 0 ) const
	{
		size_type stringLen = this->size();

		if( str.empty() )
		{
			if( indexStart >= stringLen )
			{
				return -1;
			}
			else
			{
				return indexStart;
			}
		}

		if( indexStart >= stringLen )
		{
			return -1;
		}

		size_type retval = indexStart;
		auto iter = begin() + indexStart;

		auto str_end = str.end();

		while( iter != end() )
		{
			const_iterator pattern_iter = str.begin();

			while( pattern_iter != str_end )
			{
				if( *iter == *pattern_iter )
				{
					break;
				}

				++pattern_iter;
			}

			if( pattern_iter == str_end )
			{
				return retval;
			}

			++iter;
			++retval;
		}

		return -1;
	}

public:
	size_type find_last_not_of( CodePoint c, size_type indexStart = npos ) const
	{
		size_type stringLen = this->size();

		if( indexStart >= stringLen )
		{
			return -1;
		}

		size_type retval;
		const_iterator iter;

		if( indexStart >= 0 && indexStart < stringLen )
		{
			retval = indexStart + 1;
			iter = begin() + indexStart + 1;
		}
		else
		{
			retval = stringLen;
			iter = end();
		}

		while( iter != begin() )
		{
			--iter;
			--retval;

			if( *iter != c )
			{
				return retval;
			}
		}

		return -1;
	}

	size_type find_last_not_of( const BasicString & str, size_type indexStart = npos ) const
	{
		size_type stringLen = this->size();

		if( str.empty() )
		{
			if( indexStart > stringLen || indexStart == -1 )
			{
				return stringLen - 1;
			}
			else
			{
				return indexStart;
			}
		}

		size_type retval;
		const_iterator iter;

		if( indexStart >= 0 && indexStart < stringLen )
		{
			retval = indexStart + 1;
			iter = begin() + indexStart + 1;
		}
		else
		{
			retval = stringLen;
			iter = end();
		}

		const_iterator str_end = str.end();

		while( iter != begin() )
		{
			--iter;
			--retval;

			const_iterator pattern_iter = str.begin();

			while( pattern_iter != str_end )
			{
				if( *iter == *pattern_iter )
				{
					break;
				}

				++pattern_iter;
			}

			if( pattern_iter == str_end )
			{
				return retval;
			}
		}

		return -1;
	}

public:
	CodePoint front() const
	{
		return *begin();
	}

	CodePoint back() const
	{
		return *( --end() );
	}

public:
	void pop_back()
	{
		if( empty() )
		{
			return;
		}

		const_iterator iter = --end();
		erase( iter );
	}

	void push_back( CodePoint c )
	{
		append( c );
	}

public:
	BasicString & insert( size_type indexStart, CodePoint c )
	{
		const_iterator iter_begin = begin() + indexStart;

		Encode::insert( _string, iter_begin.code_point_begin(), c );

		return *this;
	}

	BasicString & insert( size_type indexStart, const BasicString & str )
	{
		const_iterator iter_begin = begin() + indexStart;
		
		for( CodePoint c : str )
		{
			storage_const_iterator iter_tmp = Encode::insert( _string, iter_begin.code_point_begin(), c );

			iter_begin = const_iterator( iter_tmp );
		}

		return *this;
	}

	iterator insert( const_iterator posStart, CodePoint c )
	{
		storage_const_iterator iter_tmp = Encode::insert( _string, posStart.code_point_begin(), c );
		return BasicStringIterator< Encode, Alloc >( iter_tmp );
	}

	iterator insert( const_iterator posStart, const BasicString & str )
	{
		const_iterator iter = posStart;

		for( auto c : str )
		{
			storage_const_iterator iter_tmp = Encode::insert( _string, iter.code_point_begin(), c );

			iter = const_iterator( iter_tmp );
		}

		return ( iter );
	}

	iterator insert( const_iterator posStart, size_type count, CodePoint c )
	{
		storage_const_iterator iter_tmp = posStart.code_point_begin();
		iter_tmp = Encode::insert( _string, iter_tmp, c, count );
		return BasicStringIterator< Encode, Alloc >( iter_tmp );
	}

	iterator insert( const_iterator posStart, const_iterator begin, const_iterator end )
	{
		const_iterator iter = posStart;

		for( auto item = begin; item != end; item++ )
		{
			storage_const_iterator iter_tmp = Encode::insert( _string, iter.code_point_begin(), *item );

			iter = const_iterator( iter_tmp );
		}

		return ( iter );
	}

public:
	BasicString & replace( size_type indexStart, size_type size, const BasicString & str )
	{
		const_iterator iter_begin = begin() + indexStart;
		const_iterator iter_end;

		if( size >= 0 )
		{
			iter_end = iter_begin;

			for( size_type j = 0; j < size && iter_end != end(); ++j )
			{
				++iter_end;
			}
		}
		else
		{
			iter_end = end();
		}

		auto iter = erase( iter_begin, iter_end );
		insert( iter, str );

		return *this;
	}

	BasicString & replace( size_type indexStart, size_type size, const storage_unit * str )
	{
		auto iter = erase( begin() + indexStart, begin() + indexStart + size );
		insert( iter, str );

		return *this;
	}

	template <int N >
	BasicString & replace( size_type indexStart, size_type size, const storage_unit( &str )[N] )
	{
		auto iter = erase( begin() + indexStart, begin() + indexStart + size );
		insert( iter, str );

		return *this;
	}

	BasicString & replace( size_type indexStart, size_type size, size_type count, CodePoint c )
	{
		const_iterator iter_begin = begin() + indexStart;
		const_iterator iter_end;

		if( size >= 0 )
		{
			iter_end = iter_begin;

			for( size_type j = 0; j < size && iter_end != end(); ++j )
			{
				++iter_end;
			}
		}
		else
		{
			iter_end = end();
		}

		auto iter = erase( iter_begin, iter_end );
		insert( iter, count, c );

		return *this;
	}

	BasicString & replace( const_iterator first, const_iterator last, const BasicString & str )
	{
		auto iter = erase( first, last );
		insert( iter, str );

		return *this;
	}

	BasicString & replace( const_iterator first, const_iterator last, const storage_unit * str )
	{
		auto iter = erase( first, last );
		insert( iter, str );

		return *this;
	}

	template <int N >
	BasicString & replace( const_iterator first, const_iterator last, const storage_unit( &str )[N] )
	{
		auto iter = erase( first, last );
		insert( iter, str );

		return *this;
	}

	BasicString & replace( const_iterator first, const_iterator last, size_type count, CodePoint c )
	{
		auto iter = erase( first, last );
		insert( iter, count, c );

		return *this;
	}

	BasicString & replace( const_iterator first1, const_iterator last1, const_iterator first2, const_iterator last2 )
	{
		auto iter = erase( first1, last1 );
		insert( iter, first2, last2 );

		return *this;
	}

public:
	void resize( size_type size )
	{
		if( size < 0 )
		{
			size = 0;
		}

		size_type stringLen = this->size();
		size_type count = size - stringLen;

		CodePoint c;

		if( count > 0 )
		{
			append( count, c );
		}
		else if( count < 0 )
		{
			auto end = this->end();
			auto begin = end + count;

			erase( begin, end );
		}
	}

	void resize( size_type size, CodePoint c )
	{
		if( size < 0 )
		{
			size = 0;
		}

		size_type stringLen = this->size();
		size_type count = size - stringLen;

		if( count > 0 )
		{
			append( count, c );
		}
		else if( count < 0 )
		{
			erase( size, -count );
		}
	}

	void reserve( size_type size )
	{
		CodePoint c;
		append( size, c );
	}

public:
	void shrink_to_fit()
	{
		_string.shrink_to_fit();
	}

public:
	size_type size() const
	{
		return _string.size();
	}

	size_type length() const
	{
		return Encode::distance( _string.begin(), _string.end() );
	}

public:
	BasicString substr( size_type indexStart = 0, size_type size = npos ) const
	{
		const_iterator iter_begin = begin();
		const_iterator iter_end;

		for( size_type i = 0; i < indexStart && iter_begin != end(); ++i )
		{
			++iter_begin;
		}

		if( iter_begin == end() )
		{
			return BasicString();
		}

		if( size >= 0 )
		{
			iter_end = iter_begin;

			for( size_type i = 0; i < size && iter_end != end(); ++i )
			{
				++iter_end;
			}
		}
		else
		{
			iter_end = end();
		}

		return BasicString( iter_begin, iter_end );
	}

public:
	void swap( BasicString & str )
	{
		_string.swap( str._string );
	}

public:
	const_iterator begin() const
	{
		return BasicStringIterator< Encode, Alloc >( _string.begin() );
	}

	const_iterator end() const
	{

		return BasicStringIterator< Encode, Alloc >( _string.end() );
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator( end() );
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator( begin() );
	}

	storage_const_iterator storage_begin() const
	{
		return _string.begin();
	}

	storage_const_iterator storage_end() const
	{
		return _string.end();
	}

	storage_const_reverse_iterator storage_rbegin() const
	{
		return _string.crbegin() + 1;
	}

	storage_const_reverse_iterator storage_rend() const
	{
		return _string.crend();
	}

public:
	storage_unit * data()
	{
		return _string.data();
	}

	const storage_unit * data() const
	{
		return _string.data();
	}

	const storage_unit * c_str() const
	{
		return _string.c_str();
	}

	const storage_type & std_str() const
	{
		return _string;
	}

	std::vector< BasicString > split( const BasicString & sep ) const
	{
		using string_type = std::basic_string< typename Encode::storage_unit >;

		std::basic_regex< typename Encode::storage_unit > re{ sep.c_str() };

		std::vector< BasicString > result;
		std::regex_token_iterator< typename string_type::const_pointer > beg( _string.data(), _string.data() + _string.size(), re, -1 );
		std::regex_token_iterator< typename string_type::const_pointer > end;
		for( ; beg != end; ++beg )
		{
			string_type s( beg->first, beg->second );
			result.push_back( s );
		}

		return result;
	}

public:
	BasicString & replace( const BasicString & sep, const BasicString & str )
	{
		std::basic_regex< typename Encode::storage_unit > re{ sep.c_str() };

		_string = std::regex_replace( _string, re, str.c_str() );

		return *this;
	}

private:
	storage_type _string;
};
template< typename Encode, typename Alloc > const BasicString< Encode, Alloc > BasicString< Encode, Alloc >::empty_string = {};

using AnsiString = XE::BasicString< XE::AnsiEncode, std::pmr::polymorphic_allocator< XE::AnsiEncode::storage_unit > >;
using WideString = XE::BasicString< XE::WideEncode, std::pmr::polymorphic_allocator< XE::WideEncode::storage_unit > >;
using Utf8String = XE::BasicString< XE::Utf8Encode, std::pmr::polymorphic_allocator< XE::Utf8Encode::storage_unit > >;

using String = XE::AnsiString;


XE_INLINE XE::String ToString( bool _Val )
{
	return _Val ? "true" : "false";
}

XE_INLINE XE::String ToString( char _Val )
{
	return { CodePoint( _Val ) };
}

XE_INLINE XE::String ToString( uint8 _Val )
{
	return std::to_string( ( int32 ) _Val );
}

XE_INLINE XE::String ToString( int16 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( uint16 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( int32 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( long _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( uint32 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( int64 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( uint64 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( float32 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( float64 _Val )
{
	return std::to_string( _Val );
}

XE_INLINE XE::String ToString( char * _Val )
{
	return _Val;
}

XE_INLINE XE::String ToString( const char * _Val )
{
	return _Val;
}

XE_INLINE XE::String ToString( const XE::String & _Val )
{
	return _Val;
}

XE_INLINE XE::String ToString( const std::filesystem::path & _Val )
{
	return _Val.string();
}

XE_INLINE XE::String ToString( const std::chrono::system_clock::time_point & _Val )
{
	std::tm _tm;
	std::ostringstream _oss;
	const time_t _tt = std::chrono::system_clock::to_time_t( _Val );

#if PLATFORM_OS == OS_WINDOWS
	localtime_s( &_tm, &_tt );
#else
	localtime_r( &_tt, &_tm );
#endif

	_oss << std::put_time( &_tm, "%Y-%m-%d %X" );

	return _oss.str();
}


XE_INLINE bool FromString( const XE::String & _Str, bool & _Val )
{
	if( _Str == "true" || _Str == "false" )
	{
		_Val = _Str == "true";
		return true;
	}

	return false;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::int8 & _Val )
{
	_Val = std::atol( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::int16 & _Val )
{
	_Val = std::atol( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::int32 & _Val )
{
	_Val = std::stol( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, long & _Val )
{
	_Val = std::stol( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::int64 & _Val )
{
	_Val = std::stol( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::uint8 & _Val )
{
	_Val = std::stoul( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::uint16 & _Val )
{
	_Val = std::stoul( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::uint32 & _Val )
{
	_Val = std::stoul( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::uint64 & _Val )
{
	_Val = std::stoul( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::float32 & _Val )
{
	_Val = std::atof( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::float64 & _Val )
{
	_Val = std::stod( _Str.c_str() );

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, XE::String & _Val )
{
	_Val = _Str;

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, std::filesystem::path & _Val )
{
	_Val = _Str.c_str();

	return true;
}

XE_INLINE bool FromString( const XE::String & _Str, std::chrono::system_clock::time_point & _Val )
{
	if( _Str.size() == 18 )
	{
		std::tm _tm;

		_tm.tm_year = std::atoi( _Str.substr( 0, 4 ).c_str() ) - 1900;
		_tm.tm_mon =  std::atoi( _Str.substr( 5, 2 ).c_str() ) - 1;
		_tm.tm_mday = std::atoi( _Str.substr( 8, 2 ).c_str() );
		_tm.tm_hour = std::atoi( _Str.substr( 11, 2 ).c_str() );
		_tm.tm_min =  std::atoi( _Str.substr( 14, 2 ).c_str() );
		_tm.tm_sec =  std::atoi( _Str.substr( 17, 2 ).c_str() );

		_Val = std::chrono::system_clock::from_time_t( std::mktime( &_tm ) );

		return true;
	}

	return false;
}


template < typename ... T > struct Hasher< XE::BasicString< T... > >
{
	using argument_type = XE::BasicString< T... >;
	using result_type = XE::uint64;

	result_type operator()( const argument_type & val ) const
	{
		return XE::Hash( val.c_str() );
	}
};


END_XE_NAMESPACE

template <typename Encode, typename Alloc >
XE_INLINE bool operator==( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1, str2.c_str() ) == 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator==( const XE::BasicString< Encode, Alloc > & str1, const typename Encode::storage_unit * str2 )
{
	return std::strcmp( str1.c_str(), str2 ) == 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator==( const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc1 > & str1, const XE::BasicString< Encode, Alloc2 > & str2 )
{
	return std::strcmp( str2.c_str(), str1.c_str() ) == 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator==( const XE::BasicString< Encode, Alloc1 > & str1, const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc2 > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) == 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator==( const XE::BasicString< Encode, Alloc > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) == 0;
}

template <typename Encode1, typename Alloc1, typename Encode2, typename Alloc2 >
XE_INLINE bool operator==( const XE::BasicString< Encode1, Alloc1 > & str1, const XE::BasicString< Encode2, Alloc2 > & str2 )
{
	return str1.strcmp( str2 ) == 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator!=( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return !( str1 == str2 );
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator!=( const XE::BasicString< Encode, Alloc > & str1, const typename Encode::storage_unit * str2 )
{
	return !( str1 == str2 );
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator!=( const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc1 > & str1, const XE::BasicString< Encode, Alloc2 > & str2 )
{
	return !( str1 == str2 );
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator!=( const XE::BasicString< Encode, Alloc1 > & str1, const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc2 > & str2 )
{
	return !( str1 == str2 );
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator!=( const XE::BasicString< Encode, Alloc > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return !( str1 == str2 );
}

template <typename Encode1, typename Alloc1, typename Encode2, typename Alloc2 >
XE_INLINE bool operator!=( const XE::BasicString< Encode1, Alloc1 > & str1, const XE::BasicString< Encode2, Alloc2 > & str2 )
{
	return str1.strcmp( str2 ) != 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator< ( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1, str2.c_str() ) < 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator< ( const XE::BasicString< Encode, Alloc > & str1, const typename Encode::storage_unit * str2 )
{
	return std::strcmp( str1.c_str(), str2 ) < 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator< ( const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc1 > & str1, const XE::BasicString< Encode, Alloc2 > & str2 )
{
	return std::strcmp( str2.c_str(), str1.c_str() ) < 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator< ( const XE::BasicString< Encode, Alloc1 > & str1, const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc2 > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) < 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator< ( const XE::BasicString< Encode, Alloc > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return str1.strcmp( str2 ) < 0;
}

template <typename Encode1, typename Alloc1, typename Encode2, typename Alloc2 >
XE_INLINE bool operator< ( const XE::BasicString< Encode1, Alloc1 > & str1, const XE::BasicString< Encode2, Alloc2 > & str2 )
{
	return str1.strcmp( str2 ) < 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator<=( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1, str2.c_str() ) <= 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator<=( const XE::BasicString< Encode, Alloc > & str1, const typename Encode::storage_unit * str2 )
{
	return std::strcmp( str1.c_str(), str2 ) <= 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator<=( const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc1 > & str1, const XE::BasicString< Encode, Alloc2 > & str2 )
{
	return std::strcmp( str2.c_str(), str1.c_str() ) <= 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator<=( const XE::BasicString< Encode, Alloc1 > & str1, const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc2 > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) <= 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator<=( const XE::BasicString< Encode, Alloc > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) <= 0;
}

template <typename Encode1, typename Alloc1, typename Encode2, typename Alloc2 >
XE_INLINE bool operator<=( const XE::BasicString< Encode1, Alloc1 > & str1, const XE::BasicString< Encode2, Alloc2 > & str2 )
{
	return str1.strcmp( str2 ) <= 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator> ( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1, str2.c_str() ) > 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator> ( const XE::BasicString< Encode, Alloc > & str1, const typename Encode::storage_unit * str2 )
{
	return std::strcmp( str1.c_str(), str2 ) > 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator> ( const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc1 > & str1, const XE::BasicString< Encode, Alloc2 > & str2 )
{
	return std::strcmp( str2.c_str(), str1.c_str() ) > 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator> ( const XE::BasicString< Encode, Alloc1 > & str1, const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc2 > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) > 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator> ( const XE::BasicString< Encode, Alloc > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) > 0;
}

template <typename Encode1, typename Alloc1, typename Encode2, typename Alloc2 >
XE_INLINE bool operator> ( const XE::BasicString< Encode1, Alloc1 > & str1, const XE::BasicString< Encode2, Alloc2 > & str2 )
{
	return str1.strcmp( str2 ) > 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator>=( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1, str2.c_str() ) >= 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator>=( const XE::BasicString< Encode, Alloc > & str1, const typename Encode::storage_unit * str2 )
{
	return std::strcmp( str1.c_str(), str2 ) >= 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator>=( const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc1 > & str1, const XE::BasicString< Encode, Alloc2 > & str2 )
{
	return std::strcmp( str2.c_str(), str1.c_str() ) >= 0;
}

template <typename Encode, typename Alloc1, typename Alloc2 >
XE_INLINE bool operator>=( const XE::BasicString< Encode, Alloc1 > & str1, const std::basic_string< typename Encode::storage_unit, std::char_traits< typename Encode::storage_unit >, Alloc2 > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) >= 0;
}

template <typename Encode, typename Alloc >
XE_INLINE bool operator>=( const XE::BasicString< Encode, Alloc > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	return std::strcmp( str1.c_str(), str2.c_str() ) >= 0;
}

template <typename Encode1, typename Alloc1, typename Encode2, typename Alloc2 >
XE_INLINE bool operator>=( const XE::BasicString< Encode1, Alloc1 > & str1, const XE::BasicString< Encode2, Alloc2 > & str2 )
{
	return str1.strcmp( str2 ) >= 0;
}

template <typename Encode, typename Alloc >
XE_INLINE XE::BasicString< Encode, Alloc > operator+( const typename Encode::storage_unit * str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	XE::BasicString< Encode, Alloc > retval = str1;
	retval.append( str2 );

	return retval;
}

template <typename Encode, typename Alloc >
XE_INLINE XE::BasicString< Encode, Alloc > operator+( const std::basic_string< typename Encode::storage_unit > & str1, const XE::BasicString< Encode, Alloc > & str2 )
{
	XE::BasicString< Encode, Alloc > retval = str1;
	retval.append( str2 );

	return retval;
}


template <typename Encode, typename Alloc >
void swap( XE::BasicString< Encode, Alloc > & str1, XE::BasicString< Encode, Alloc > & str2 )
{
	str1.swap( str2 );
}


namespace std
{
	template <typename Encode, typename Alloc > struct equal_to< XE::BasicString< Encode, Alloc > >
	{
		using first_argument_type = XE::BasicString< Encode, Alloc >;
		using second_argument_type = XE::BasicString< Encode, Alloc >;
		using result_type = bool;

		bool operator()( const XE::BasicString< Encode, Alloc > & _Left, const XE::BasicString< Encode, Alloc > & _Right ) const
		{
			return ( _Left == _Right );
		}
	};

	template <typename Encode, typename Alloc > struct hash< XE::BasicString< Encode, Alloc > >
	{
		typedef XE::BasicString< Encode, Alloc > argument_type;
		typedef uint64_t result_type;

		uint64_t operator()( const XE::BasicString< Encode, Alloc > & _Keyval ) const noexcept
		{
			return XE::Hash( _Keyval.c_str() );
		}
	};
}

#endif // STRING_HPP__2D116B84_5AD0_4FFF_A245_8EC14E092378
