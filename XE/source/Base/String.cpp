#include "String.h"

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
#include <Windows.h>
#else
#include <iconv.h>
#endif

XE::CodePoint::CodePoint()
	: _unicode( 0 )
{
}

XE::CodePoint::CodePoint( char c )
	: _unicode( static_cast< unsigned char >( c ) )
{

}

XE::CodePoint::CodePoint( wchar_t c )
	: _unicode( c )
{

}

#ifdef __cpp_char8_t
XE::CodePoint::CodePoint( char8_t c )
	: _unicode( c )
{

}
#endif

XE::CodePoint::CodePoint( char16_t c )
	: _unicode( c )
{
}

XE::CodePoint::CodePoint( char32_t c )
	: _unicode( c )
{
}

XE::CodePoint::CodePoint( XE::uint32 c )
	: _unicode( c )
{
}

XE::CodePoint::CodePoint( const XE::CodePoint & other )
	: _unicode( other._unicode )
{
}

bool XE::CodePoint::operator!=( const XE::CodePoint & other ) const
{
	return _unicode != other._unicode;
}

bool XE::CodePoint::operator==( const XE::CodePoint & other ) const
{
	return _unicode == other._unicode;
}

bool XE::CodePoint::operator< ( const XE::CodePoint & other ) const
{
	return _unicode < other._unicode;
}

bool XE::CodePoint::operator<=( const XE::CodePoint & other ) const
{
	return _unicode <= other._unicode;
}

bool XE::CodePoint::operator >( const XE::CodePoint & other ) const
{
	return _unicode > other._unicode;
}

bool XE::CodePoint::operator>=( const XE::CodePoint & other ) const
{
	return _unicode >= other._unicode;
}

XE::CodePoint & XE::CodePoint::operator=( const XE::CodePoint & c )
{
	_unicode = c._unicode;
	return *this;
}

uint32_t XE::CodePoint::unicode() const
{
	return _unicode;
}


XE::CodePoint XE::AnsiEncode::get_code_point( const XE::AnsiEncode::storage_unit * iter )
{
	wchar_t value = *iter;
	unsigned char tmp = static_cast< unsigned char >( *iter );

	if( tmp >= 0x80 && tmp <= 0xFF )
	{
		value = ( wchar_t( tmp ) << 8 ) | static_cast< unsigned char >( iter[1] );
	}

	return CodePoint( value );
}

XE::AnsiEncode::size_type XE::AnsiEncode::num_of_bytes( XE::AnsiEncode::storage_unit value )
{
	if( static_cast< unsigned char >( value ) >= 0x80 && static_cast< unsigned char >( value ) <= 0xFF )
	{
		return 2;
	}

	return 1;
}


XE::CodePoint XE::WideEncode::get_code_point( const XE::WideEncode::storage_unit * iter )
{
	return CodePoint( *iter );
}

XE::WideEncode::size_type XE::WideEncode::num_of_bytes( XE::WideEncode::storage_unit value )
{
	return 1;
}


XE::CodePoint XE::Utf8Encode::get_code_point( const XE::Utf8Encode::storage_unit * iter )
{
	char32_t value = *iter;
	unsigned char tmp = static_cast< unsigned char >( *iter );

	if( ( tmp & 0x80 ) == 0 )
	{
		value = tmp;
	}
	else if( ( tmp & 0xE0 ) == 0xC0 )
	{
		value = ( tmp & 0x1F ) << 6;

		tmp = iter[1];
		value |= ( tmp & 0x3F );
	}
	else if( ( tmp & 0xF0 ) == 0xE0 )
	{
		value = ( tmp & 0x0F ) << 12;

		tmp = iter[1];
		value |= ( tmp & 0x3F ) << 6;

		tmp = iter[2];
		value |= ( tmp & 0x3F );
	}
	else
	{
		value = ( tmp & 0x07 ) << 18;

		tmp = iter[1];
		value |= ( tmp & 0x3F ) << 12;

		tmp = iter[2];
		value |= ( tmp & 0x3F ) << 6;

		tmp = iter[3];
		value |= ( tmp & 0x3F );
	}

	return CodePoint( value );
}

XE::Utf8Encode::size_type XE::Utf8Encode::num_of_bytes( XE::Utf8Encode::storage_unit value )
{
	unsigned char val = static_cast< unsigned char >( value );
	if( ( val & 0x80 ) == 0 )
	{
		return 1;
	}
	else if( ( val & 0xE0 ) == 0xC0 )
	{
		return 2;
	}
	else if( ( val & 0xF0 ) == 0xE0 )
	{
		return 3;
	}
	else if( ( val & 0xF8 ) == 0xF0 )
	{
		return 4;
	}

	return 1;
}


std::wstring XE::StdStringConvert::ansi_2_wide( const std::string & str )
{
	std::wstring result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	auto len = MultiByteToWideChar( CP_ACP, 0, str.c_str(), static_cast< int >( str.size() ), nullptr, 0 );
	if( len > 0 )
	{
		result.resize( len );

		int size = MultiByteToWideChar( CP_ACP, 0, str.c_str(), static_cast<int>( str.size() ), (wchar_t *)result.data(), len );
		if( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::string XE::StdStringConvert::wide_2_ansi( const std::wstring & wstr )
{
	std::string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	auto len = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), static_cast< int >( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), static_cast<int>( wstr.size() ), (char *)result.data(), len, nullptr, nullptr );
		if( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

#ifdef __cpp_char8_t
std::u8string XE::StdStringConvert::ansi_2_utf8( const std::string & str )
{
	std::u8string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	std::wstring wstr = ansi_2_wide( str );

	auto len = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if ( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), reinterpret_cast<char *>( result.data() ), len, nullptr, nullptr );
		if ( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::string XE::StdStringConvert::utf8_2_ansi( const std::u8string & str )
{
	std::string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	std::wstring wstr = utf8_2_wide( str );

	auto len = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if ( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), static_cast<int>( wstr.size() ), result.data(), len, nullptr, nullptr );
		if ( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::u8string XE::StdStringConvert::wide_2_utf8( const std::wstring & wstr )
{
	std::u8string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	auto len = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if ( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), reinterpret_cast<char *>( result.data() ), len, nullptr, nullptr );
		if ( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::wstring XE::StdStringConvert::utf8_2_wide( const std::u8string & str )
{
	std::wstring result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	auto len = MultiByteToWideChar( CP_UTF8, 0, reinterpret_cast<const char *>( str.c_str() ), static_cast<int>( str.size() ), nullptr, 0 );
	if ( len > 0 )
	{
		result.resize( len );

		int size = MultiByteToWideChar( CP_UTF8, 0, reinterpret_cast<const char *>( str.c_str() ), static_cast<int>( str.size() ), reinterpret_cast<wchar_t *>( result.data() ), len );
		if ( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}
#else
std::string XE::StdStringConvert::ansi_2_utf8( const std::string & str )
{
	std::string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	std::wstring wstr = ansi_2_wide( str );

	auto len = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), result.data(), len, nullptr, nullptr );
		if( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::string XE::StdStringConvert::utf8_2_ansi( const std::string & str )
{
	std::string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	std::wstring wstr = utf8_2_wide( str );

	auto len = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), static_cast<int>( wstr.size() ), result.data(), len, nullptr, nullptr );
		if( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::string XE::StdStringConvert::wide_2_utf8( const std::wstring & wstr )
{
	std::string result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	auto len = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
	if( len > 0 )
	{
		result.resize( len );

		int size = WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), static_cast<int>( wstr.size() ), reinterpret_cast<char *>( result.data() ), len, nullptr, nullptr );
		if( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}

std::wstring XE::StdStringConvert::utf8_2_wide( const std::string & str )
{
	std::wstring result;

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	auto len = MultiByteToWideChar( CP_UTF8, 0, str.c_str(), static_cast<int>( str.size() ), nullptr, 0 );
	if( len > 0 )
	{
		result.resize( len );

		int size = MultiByteToWideChar( CP_UTF8, 0, str.c_str(), static_cast<int>( str.size() ), reinterpret_cast<wchar_t *>( result.data() ), len );
		if( size == 0 )
		{
			result.clear();
		}
		else
		{
			result.resize( size );
		}
	}
#else
	// TODO: 
#endif

	return result;
}
#endif