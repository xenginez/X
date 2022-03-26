#include "UUID.h"

#if PLATFORM_OS == OS_WINDOWS
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

BEG_META( XE::UUID )
END_META()

XE::UUID::UUID()
{
	std::memset( _Datas, 0, 16 );
}

XE::UUID::UUID( const XE::UUID & val )
	:_GUID( val._GUID )
{

}

XE::UUID::UUID( const XE::String & val )
{
	FromString( val );
}

XE::UUID XE::UUID::Create()
{
	XE::UUID result;

	::uuid_t uuid;

#if defined(_WIN32)
	CoCreateGuid( &uuid );
#else
	uuid_generate( uuid );
#endif

	std::memcpy( result._Datas, ( void * )&uuid, 16 );

	return result;
}

XE::UUID & XE::UUID::operator=( const XE::UUID & val )
{
	_GUID = val._GUID;

	return *this;
}

bool XE::UUID::operator!=( const XE::UUID & val ) const
{
	return std::memcmp( _Datas, val._Datas, 16 ) != 0;
}

bool XE::UUID::operator==( const XE::UUID & val ) const
{
	return std::memcmp( _Datas, val._Datas, 16 ) == 0;
}

bool XE::UUID::operator>=( const XE::UUID & val ) const
{
	return std::memcmp( _Datas, val._Datas, 16 ) >= 0;
}

bool XE::UUID::operator >( const XE::UUID & val ) const
{
	return std::memcmp( _Datas, val._Datas, 16 ) > 0;
}

bool XE::UUID::operator<=( const XE::UUID & val ) const
{
	return std::memcmp( _Datas, val._Datas, 16 ) <= 0;
}

bool XE::UUID::operator< ( const XE::UUID & val ) const
{
	return std::memcmp( _Datas, val._Datas, 16 ) < 0;
}

XE::String XE::UUID::ToString( std::string_view fmt /*= "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" */ ) const
{
	static constexpr char _str[] = "0123456789abcdef";

	std::string result;

	auto beg = std::begin( fmt );
	auto end = std::end( fmt );

	for( XE::uint64 i = 0; beg != end; ++beg )
	{
		switch( *beg )
		{
		case 'x':
			std::inserter( result, std::end( result ) ) = _str[( i % 2 ) == 0 ? ( _Datas[( i ) / 2] & 0x0F ) : ( ( _Datas[( i ) / 2] & 0xF0 ) >> 4 )];
			++i;
			break;
		case 'X':
			std::inserter( result, std::end( result ) ) = std::toupper( _str[( i % 2 ) == 0 ? ( _Datas[( i ) / 2] & 0x0F ) : ( ( _Datas[( i ) / 2] & 0xF0 ) >> 4 )] );
			++i;
			break;
		default:
			std::inserter( result, std::end( result ) ) = *beg;
			break;
		}
	}

	return result;
}

void XE::UUID::FromString( const XE::String & container, std::string_view fmt /*= "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" */ )
{
	std::memset( _Datas, 0, 16 );

	std::string result = container.c_str();

	auto constexpr _get = []( char c )->XE::uint8
	{
		static constexpr char _str[] = "0123456789abcdef";

		return std::distance( std::begin( _str ), std::find( std::begin( _str ), std::end( _str ), std::tolower( c ) ) );
	};


	auto beg = std::begin( fmt );
	auto end = std::end( fmt );

	for( XE::uint64 i = 0; beg != end; ++beg )
	{
		switch( *beg )
		{
		case 'x':
		case 'X':
			
			_Datas[i / 2] = ( i % 2 ) == 0 ?
				( _Datas[i / 2] | ( _get( *( result.begin() + std::distance( std::begin( fmt ), beg ) ) ) ) ) :
				( _Datas[i / 2] | ( _get( *( result.begin() + std::distance( std::begin( fmt ), beg ) ) ) << 4 ) );
			++i;
			break;
		default:
			break;
		}
	}
}