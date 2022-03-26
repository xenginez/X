#include "MD5.h"

namespace
{
	template <typename T> class loader
	{
	public:
		loader( const uint8_t * ptr )
			: _ptr( ptr )
		{}

		T operator[]( const size_t i ) const
		{
			const uint8_t * ptr = _ptr + ( sizeof( T ) * i );

			return  ( ( static_cast<T>( *( ptr + 0 ) ) << 0 )
					  | ( static_cast<T>( *( ptr + 1 ) ) << 8 )
					  | ( static_cast<T>( *( ptr + 2 ) ) << 16 )
					  | ( static_cast<T>( *( ptr + 3 ) ) << 24 ) );
		}

	private:
		const uint8_t * _ptr;
	};

	template <typename R, typename T> constexpr R ror( const T x, XE::uint32 s )
	{
		return static_cast<R>( x >> s );
	}
	
	template <typename T> constexpr T rotl( const T x, XE::uint32 s )
	{
		return( s == 0 ) ? x : ( ( x << s ) | ( x >> ( ( sizeof( T ) * 8 ) - s ) ) );
	}

	void md5_add( XE::uint8 * buf, XE::uint64 buf_sz, XE::uint32 * hash )
	{
		if( buf_sz <= 0 )
			return;

		loader< XE::uint32 > x( buf );

		auto f = []( const XE::uint32 x, const XE::uint32 y, const XE::uint32 z ) -> XE::uint32
		{
			return ( ( x & ( y ^ z ) ) ^ z );
		};
		auto g = []( const XE::uint32 x, const XE::uint32 y, const XE::uint32 z ) -> XE::uint32
		{
			return ( y ^ ( ( x ^ y ) & z ) );
		};
		auto h = []( const XE::uint32 x, const XE::uint32 y, const XE::uint32 z ) -> XE::uint32
		{
			return ( x ^ y ^ z );
		};
		auto i = []( const XE::uint32 x, const XE::uint32 y, const XE::uint32 z ) -> XE::uint32
		{
			return ( y ^ ( x | ( ~z ) ) );
		};

		XE::uint32 a = hash[0];
		XE::uint32 b = hash[1];
		XE::uint32 c = hash[2];
		XE::uint32 d = hash[3];

		auto round = [x]( XE::uint32 & a, XE::uint32 & b, XE::uint32 & c, XE::uint32 & d, const auto & func, XE::uint32 k, XE::uint32 s, XE::uint32 t ) -> void
		{
			a = b + rotl( ( a + func( b, c, d ) + x[k] + t ), s );
		};

		round( a, b, c, d, f, 0, 7, 0xd76aa478 );
		round( d, a, b, c, f, 1, 12, 0xe8c7b756 );
		round( c, d, a, b, f, 2, 17, 0x242070db );
		round( b, c, d, a, f, 3, 22, 0xc1bdceee );
		round( a, b, c, d, f, 4, 7, 0xf57c0faf );
		round( d, a, b, c, f, 5, 12, 0x4787c62a );
		round( c, d, a, b, f, 6, 17, 0xa8304613 );
		round( b, c, d, a, f, 7, 22, 0xfd469501 );
		round( a, b, c, d, f, 8, 7, 0x698098d8 );
		round( d, a, b, c, f, 9, 12, 0x8b44f7af );
		round( c, d, a, b, f, 10, 17, 0xffff5bb1 );
		round( b, c, d, a, f, 11, 22, 0x895cd7be );
		round( a, b, c, d, f, 12, 7, 0x6b901122 );
		round( d, a, b, c, f, 13, 12, 0xfd987193 );
		round( c, d, a, b, f, 14, 17, 0xa679438e );
		round( b, c, d, a, f, 15, 22, 0x49b40821 );

		round( a, b, c, d, g, 1, 5, 0xf61e2562 );
		round( d, a, b, c, g, 6, 9, 0xc040b340 );
		round( c, d, a, b, g, 11, 14, 0x265e5a51 );
		round( b, c, d, a, g, 0, 20, 0xe9b6c7aa );
		round( a, b, c, d, g, 5, 5, 0xd62f105d );
		round( d, a, b, c, g, 10, 9, 0x02441453 );
		round( c, d, a, b, g, 15, 14, 0xd8a1e681 );
		round( b, c, d, a, g, 4, 20, 0xe7d3fbc8 );
		round( a, b, c, d, g, 9, 5, 0x21e1cde6 );
		round( d, a, b, c, g, 14, 9, 0xc33707d6 );
		round( c, d, a, b, g, 3, 14, 0xf4d50d87 );
		round( b, c, d, a, g, 8, 20, 0x455a14ed );
		round( a, b, c, d, g, 13, 5, 0xa9e3e905 );
		round( d, a, b, c, g, 2, 9, 0xfcefa3f8 );
		round( c, d, a, b, g, 7, 14, 0x676f02d9 );
		round( b, c, d, a, g, 12, 20, 0x8d2a4c8a );

		round( a, b, c, d, h, 5, 4, 0xfffa3942 );
		round( d, a, b, c, h, 8, 11, 0x8771f681 );
		round( c, d, a, b, h, 11, 16, 0x6d9d6122 );
		round( b, c, d, a, h, 14, 23, 0xfde5380c );
		round( a, b, c, d, h, 1, 4, 0xa4beea44 );
		round( d, a, b, c, h, 4, 11, 0x4bdecfa9 );
		round( c, d, a, b, h, 7, 16, 0xf6bb4b60 );
		round( b, c, d, a, h, 10, 23, 0xbebfbc70 );
		round( a, b, c, d, h, 13, 4, 0x289b7ec6 );
		round( d, a, b, c, h, 0, 11, 0xeaa127fa );
		round( c, d, a, b, h, 3, 16, 0xd4ef3085 );
		round( b, c, d, a, h, 6, 23, 0x04881d05 );
		round( a, b, c, d, h, 9, 4, 0xd9d4d039 );
		round( d, a, b, c, h, 12, 11, 0xe6db99e5 );
		round( c, d, a, b, h, 15, 16, 0x1fa27cf8 );
		round( b, c, d, a, h, 2, 23, 0xc4ac5665 );

		round( a, b, c, d, i, 0, 6, 0xf4292244 );
		round( d, a, b, c, i, 7, 10, 0x432aff97 );
		round( c, d, a, b, i, 14, 15, 0xab9423a7 );
		round( b, c, d, a, i, 5, 21, 0xfc93a039 );
		round( a, b, c, d, i, 12, 6, 0x655b59c3 );
		round( d, a, b, c, i, 3, 10, 0x8f0ccc92 );
		round( c, d, a, b, i, 10, 15, 0xffeff47d );
		round( b, c, d, a, i, 1, 21, 0x85845dd1 );
		round( a, b, c, d, i, 8, 6, 0x6fa87e4f );
		round( d, a, b, c, i, 15, 10, 0xfe2ce6e0 );
		round( c, d, a, b, i, 6, 15, 0xa3014314 );
		round( b, c, d, a, i, 13, 21, 0x4e0811a1 );
		round( a, b, c, d, i, 4, 6, 0xf7537e82 );
		round( d, a, b, c, i, 11, 10, 0xbd3af235 );
		round( c, d, a, b, i, 2, 15, 0x2ad7d2bb );
		round( b, c, d, a, i, 9, 21, 0xeb86d391 );

		hash[0] += a;
		hash[1] += b;
		hash[2] += c;
		hash[3] += d;

		md5_add( buf + 64, buf_sz - 64, hash );
	}
}

XE::MD5::MD5()
{
	Reset();
}

XE::MD5::MD5( XE::MemoryView val )
{
	Reset( val );
}

XE::MD5::MD5( const XE::String & val )
{
	Reset( val );
}

bool XE::MD5::operator==( const MD5 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::MD5::operator!=( const MD5 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::MD5 & XE::MD5::Add( XE::MemoryView val )
{
	XE::uint64 i = 0;
	XE::uint64 len = 0;

	while( i < val.size() )
	{
		len = std::min( val.size() - i, BUFFER_SIZE - _Index );

		for( size_t j = 0; j < len; j++ )
		{
			_Buffer[_Index++] = val[i++];
		}

		if( _Index == BUFFER_SIZE )
		{
			_Count += _Index;

			md5_add( _Buffer.data(), _Index, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::MD5 & XE::MD5::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::MD5 & XE::MD5::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = 1 << 7;

	const auto len = static_cast<int>( ( BUFFER_SIZE - ( _Index + 8 ) ) % ( BUFFER_SIZE / 2 ) ) + 8;
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}

	XE::uint64 sizeCounterBits = _Count * 8;
	XE::uint32 sizeCounterBitsL = ror< XE::uint32 >( sizeCounterBits, 0 );
	XE::uint32 sizeCounterBitsH = ror< XE::uint32 >( sizeCounterBits, 32 );
	for( int i = 0; i < 4; ++i )
	{
		_Buffer[_Index - 8 + i] = ror<XE::uint8>( sizeCounterBitsL, ( 8 * i ) );
		_Buffer[_Index - 4 + i] = ror<XE::uint8>( sizeCounterBitsH, ( 8 * i ) );
	}

	md5_add( _Buffer.data(), _Index, _Hash.data() );

	_Index = 0;

	return *this;
}

void XE::MD5::Reset()
{
	_Index = 0;
	_Count = 0;
	_Buffer.fill( 0 );

	_Hash[0] = 0x67452301;
	_Hash[1] = 0xEFCDAB89;
	_Hash[2] = 0x98BADCFE;
	_Hash[3] = 0x10325476;
}

void XE::MD5::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::MD5::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::MD5::ToView() const
{
	return { reinterpret_cast<const XE::int8 *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint32 ) };
}

XE::String XE::MD5::ToString( bool up /*= true */ ) const
{
	XE::String result;

	char c = up ? 'A' : 'a';

	auto arr = ToArray();
	result.resize( arr.size() * 2 );

	for( size_t i = 0; i < arr.size(); i++ )
	{
		XE::uint8 upper = ror<XE::uint8>( arr[i], 4 );
		result[i * 2] = static_cast<char>( ( upper < 10 ) ? ( upper + '0' ) : ( upper - 10 + c ) );

		XE::uint8 lower = arr[i] & 0x0F;
		result[i * 2 + 1] = static_cast<char>( ( lower < 10 ) ? ( lower + '0' ) : ( lower - 10 + c ) );
	}

	return result;
}

std::array<XE::uint8, XE::MD5::HASH_SIZE> XE::MD5::ToArray() const
{
	std::array<XE::uint8, XE::MD5::HASH_SIZE> result;

	auto ptr = result.data();

	for( auto h : _Hash )
	{
		for( size_t i = 0; i < sizeof( XE::uint32 ); ++i )
		{
			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}
