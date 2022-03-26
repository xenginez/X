#include "SHA.h"


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

			if constexpr( sizeof( T ) == 4 )
			{
				return  ( ( static_cast<T>( *( ptr + 0 ) ) << 24 )
						  | ( static_cast<T>( *( ptr + 1 ) ) << 16 )
						  | ( static_cast<T>( *( ptr + 2 ) ) << 8 )
						  | ( static_cast<T>( *( ptr + 3 ) ) << 0 ) );
			}
			else
			{
				return  ( ( static_cast<T>( *( ptr + 0 ) ) << 56 )
						  | ( static_cast<T>( *( ptr + 1 ) ) << 48 )
						  | ( static_cast<T>( *( ptr + 2 ) ) << 40 )
						  | ( static_cast<T>( *( ptr + 3 ) ) << 32 )
						  | ( static_cast<T>( *( ptr + 4 ) ) << 24 )
						  | ( static_cast<T>( *( ptr + 5 ) ) << 16 )
						  | ( static_cast<T>( *( ptr + 6 ) ) << 8 )
						  | ( static_cast<T>( *( ptr + 7 ) ) << 0 ) );
			}
		}

	private:
		const uint8_t * _ptr;
	};

	template <typename T> class loader2
	{
	public:
		loader2( const uint8_t * ptr )
			: _ptr( ptr )
		{}

		T operator[]( const size_t i ) const
		{
			const uint8_t * ptr = _ptr + ( sizeof( T ) * i );

			if constexpr( sizeof( T ) == 4 )
			{
				return  ( ( static_cast<T>( *( ptr + 0 ) ) << 0 )
						  | ( static_cast<T>( *( ptr + 1 ) ) << 8 )
						  | ( static_cast<T>( *( ptr + 2 ) ) << 16 )
						  | ( static_cast<T>( *( ptr + 3 ) ) << 24 ) );
			}
			else
			{
				return  ( ( static_cast<T>( *( ptr + 0 ) ) << 0 )
						  | ( static_cast<T>( *( ptr + 1 ) ) << 8 )
						  | ( static_cast<T>( *( ptr + 2 ) ) << 16 )
						  | ( static_cast<T>( *( ptr + 3 ) ) << 24 )
						  | ( static_cast<T>( *( ptr + 4 ) ) << 32 )
						  | ( static_cast<T>( *( ptr + 5 ) ) << 40 )
						  | ( static_cast<T>( *( ptr + 6 ) ) << 48 )
						  | ( static_cast<T>( *( ptr + 7 ) ) << 56 ) );
			}
		}

	private:
		const uint8_t * _ptr;
	};

	static constexpr XE::uint32 sha2_k_table[64] =
	{
		0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
		0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
		0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
		0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
		0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
		0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
		0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
		0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
		0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
		0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
		0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
		0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
		0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
		0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
		0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
		0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
	};
	static constexpr XE::uint64 sha2_64_k_table[80] =
	{
		0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
		0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
		0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
		0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
		0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
		0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
		0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
		0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
		0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
		0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
		0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
		0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
		0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
		0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
		0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
		0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
		0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
		0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
		0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
		0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
	};
	static constexpr XE::uint64 sha3_round_table[24] =
	{
		0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
		0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009, 
		0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
		0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
		0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A, 
		0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
	};

	template <typename T> constexpr T rotr( const T x, const XE::uint32 s )
	{
		return( s == 0 ) ? x : ( ( x >> s ) | ( x << ( ( sizeof( T ) * 8 ) - s ) ) );
	}

	template <typename T> constexpr T rotl( const T x, const XE::uint32 s )
	{
		return( s == 0 ) ? x : ( ( x << s ) | ( x >> ( ( sizeof( T ) * 8 ) - s ) ) );
	}

	template <typename R, typename T> constexpr R ror( const T x, const XE::uint32 s )
	{
		return static_cast<R>( x >> s );
	}

	void sha1_add( const XE::uint8 * buf, XE::uint64 buf_sz, XE::uint64 block_sz, XE::uint32 * hash )
	{
		if( buf_sz == 0 )
			return;

		XE::uint32 w[80] = {};
		XE::uint32 a, b, c, d, e;

		loader< XE::uint32 > m( buf );

		a = hash[0];
		b = hash[1];
		c = hash[2];
		d = hash[3];
		e = hash[4];

		// round1
		{
#define round1(a, b, c, d, e, t) \
			w[t] = m[t]; \
			e = rotl(a, 5) + ((b & (c ^ d)) ^ d) + e + w[t] + 0x5A827999; \
			b = rotl(b, 30);

			round1( a, b, c, d, e, 0 );
			round1( e, a, b, c, d, 1 );
			round1( d, e, a, b, c, 2 );
			round1( c, d, e, a, b, 3 );
			round1( b, c, d, e, a, 4 );
			round1( a, b, c, d, e, 5 );
			round1( e, a, b, c, d, 6 );
			round1( d, e, a, b, c, 7 );
			round1( c, d, e, a, b, 8 );
			round1( b, c, d, e, a, 9 );
			round1( a, b, c, d, e, 10 );
			round1( e, a, b, c, d, 11 );
			round1( d, e, a, b, c, 12 );
			round1( c, d, e, a, b, 13 );
			round1( b, c, d, e, a, 14 );
			round1( a, b, c, d, e, 15 );
#undef round1
		}

		// round1a
		{
#define round1a(a, b, c, d, e, t) \
			w[t] = rotl((w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16]), 1); \
			e = rotl(a, 5) + ((b & (c ^ d)) ^ d) + e + w[t] + 0x5A827999; \
			b = rotl(b, 30);

			round1a( e, a, b, c, d, 16 );
			round1a( d, e, a, b, c, 17 );
			round1a( c, d, e, a, b, 18 );
			round1a( b, c, d, e, a, 19 );
#undef round1a
		}

		// round2
		{
#define round2(a, b, c, d, e, t) \
			w[t] = rotl((w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16]), 1); \
			e = rotl(a, 5) + (b ^ c ^ d) + e + w[t] + 0x6ED9EBA1; \
			b = rotl(b, 30);

			round2( a, b, c, d, e, 20 );
			round2( e, a, b, c, d, 21 );
			round2( d, e, a, b, c, 22 );
			round2( c, d, e, a, b, 23 );
			round2( b, c, d, e, a, 24 );
			round2( a, b, c, d, e, 25 );
			round2( e, a, b, c, d, 26 );
			round2( d, e, a, b, c, 27 );
			round2( c, d, e, a, b, 28 );
			round2( b, c, d, e, a, 29 );
			round2( a, b, c, d, e, 30 );
			round2( e, a, b, c, d, 31 );
#undef round2
		}

		// round2a
		{
#define round2a(a, b, c, d, e, t) \
			w[t] = rotl((w[t - 6] ^ w[t - 16] ^ w[t - 28] ^ w[t - 32]), 2); \
			e = rotl(a, 5) + (b ^ c ^ d) + e + w[t] + 0x6ED9EBA1; \
			b = rotl(b, 30);

			round2a( d, e, a, b, c, 32 );
			round2a( c, d, e, a, b, 33 );
			round2a( b, c, d, e, a, 34 );
			round2a( a, b, c, d, e, 35 );
			round2a( e, a, b, c, d, 36 );
			round2a( d, e, a, b, c, 37 );
			round2a( c, d, e, a, b, 38 );
			round2a( b, c, d, e, a, 39 );
#undef round2a
		}

		// round3
		{
#define round3(a, b, c, d, e, t) \
			w[t] = rotl((w[t - 6] ^ w[t - 16] ^ w[t - 28] ^ w[t - 32]), 2); \
			e = rotl(a, 5) + ((b & c) | (d & (b | c))) + e + w[t] + 0x8F1BBCDC; \
			b = rotl(b, 30);

			round3( a, b, c, d, e, 40 );
			round3( e, a, b, c, d, 41 );
			round3( d, e, a, b, c, 42 );
			round3( c, d, e, a, b, 43 );
			round3( b, c, d, e, a, 44 );
			round3( a, b, c, d, e, 45 );
			round3( e, a, b, c, d, 46 );
			round3( d, e, a, b, c, 47 );
			round3( c, d, e, a, b, 48 );
			round3( b, c, d, e, a, 49 );
			round3( a, b, c, d, e, 50 );
			round3( e, a, b, c, d, 51 );
			round3( d, e, a, b, c, 52 );
			round3( c, d, e, a, b, 53 );
			round3( b, c, d, e, a, 54 );
			round3( a, b, c, d, e, 55 );
			round3( e, a, b, c, d, 56 );
			round3( d, e, a, b, c, 57 );
			round3( c, d, e, a, b, 58 );
			round3( b, c, d, e, a, 59 );
#undef round3
		}

		// round4
		{
#define round4(a, b, c, d, e, t) \
			w[t] = rotl((w[t - 6] ^ w[t - 16] ^ w[t - 28] ^ w[t - 32]), 2); \
			e = rotl(a, 5) + (b ^ c ^ d) + e + w[t] + 0xCA62C1D6; \
			b = rotl(b, 30);

			round4( a, b, c, d, e, 60 );
			round4( e, a, b, c, d, 61 );
			round4( d, e, a, b, c, 62 );
			round4( c, d, e, a, b, 63 );
			round4( b, c, d, e, a, 64 );
			round4( a, b, c, d, e, 65 );
			round4( e, a, b, c, d, 66 );
			round4( d, e, a, b, c, 67 );
			round4( c, d, e, a, b, 68 );
			round4( b, c, d, e, a, 69 );
			round4( a, b, c, d, e, 70 );
			round4( e, a, b, c, d, 71 );
			round4( d, e, a, b, c, 72 );
			round4( c, d, e, a, b, 73 );
			round4( b, c, d, e, a, 74 );
			round4( a, b, c, d, e, 75 );
			round4( e, a, b, c, d, 76 );
			round4( d, e, a, b, c, 77 );
			round4( c, d, e, a, b, 78 );
			round4( b, c, d, e, a, 79 );
#undef round4
		}

		hash[0] += a;
		hash[1] += b;
		hash[2] += c;
		hash[3] += d;
		hash[4] += e;

		sha1_add( buf + block_sz, buf_sz - block_sz, block_sz, hash );
	}

	void sha2_add( const XE::uint8 * buf, XE::uint64 buf_sz, XE::uint64 block_sz, XE::uint32 * hash )
	{
		if( buf_sz == 0 )
			return;

		XE::uint32 w[64]{};
		loader< XE::uint32 > m( buf );

		const auto ssig0 = []( const XE::uint32 x ) -> XE::uint32
		{
			return ( rotr( x, 7 ) ^ rotr( x, 18 ) ^ ror<XE::uint32>( x, 3 ) );
		};
		const auto ssig1 = []( const XE::uint32 x ) -> XE::uint32
		{
			return ( rotr( x, 17 ) ^ rotr( x, 19 ) ^ ror<XE::uint32>( x, 10 ) );
		};
		const auto round = [&w]( XE::uint32 & a, XE::uint32 & b, XE::uint32 & c, XE::uint32 & d, XE::uint32 & e, XE::uint32 & f, XE::uint32 & g, XE::uint32 & h, XE::uint32 t ) -> void
		{
			const auto ch = []( const XE::uint32 x, const XE::uint32 y, const XE::uint32 z ) -> XE::uint32
			{
				return ( ( x & ( y ^ z ) ) ^ z );
			};
			const auto maj = []( const XE::uint32 x, const XE::uint32 y, const XE::uint32 z ) -> XE::uint32
			{
				return ( ( x & ( y | z ) ) | ( y & z ) );
			};
			const auto bsig0 = []( const XE::uint32 x ) -> XE::uint32
			{
				return ( rotr( x, 2 ) ^ rotr( x, 13 ) ^ rotr( x, 22 ) );
			};
			const auto bsig1 = []( const XE::uint32 x ) -> XE::uint32
			{
				return ( rotr( x, 6 ) ^ rotr( x, 11 ) ^ rotr( x, 25 ) );
			};

			const XE::uint32 t1 = h + bsig1( e ) + ch( e, f, g ) + sha2_k_table[t] + w[t];
			const XE::uint32 t2 = bsig0( a ) + maj( a, b, c );

			h = t1;
			d += h;
			h += t2;
		};

		for( size_t i = 0; i < 16; ++i )
		{
			w[i] = m[i];
		}
		for( size_t i = 16; i < 64; ++i )
		{
			w[i] = ssig1( w[i - 2] ) + w[i - 7] + ssig0( w[i - 15] ) + w[i - 16];
		}

		XE::uint32 a = hash[0];
		XE::uint32 b = hash[1];
		XE::uint32 c = hash[2];
		XE::uint32 d = hash[3];
		XE::uint32 e = hash[4];
		XE::uint32 f = hash[5];
		XE::uint32 g = hash[6];
		XE::uint32 h = hash[7];

		for( size_t i = 0; i < 8; ++i )
		{
			round( a, b, c, d, e, f, g, h, ( 8 * i ) + 0 );
			round( h, a, b, c, d, e, f, g, ( 8 * i ) + 1 );
			round( g, h, a, b, c, d, e, f, ( 8 * i ) + 2 );
			round( f, g, h, a, b, c, d, e, ( 8 * i ) + 3 );
			round( e, f, g, h, a, b, c, d, ( 8 * i ) + 4 );
			round( d, e, f, g, h, a, b, c, ( 8 * i ) + 5 );
			round( c, d, e, f, g, h, a, b, ( 8 * i ) + 6 );
			round( b, c, d, e, f, g, h, a, ( 8 * i ) + 7 );
		}

		hash[0] += a;
		hash[1] += b;
		hash[2] += c;
		hash[3] += d;
		hash[4] += e;
		hash[5] += f;
		hash[6] += g;
		hash[7] += h;

		sha2_add( buf + block_sz, buf_sz - block_sz, block_sz, hash );
	}

	void sha2_64_add( const XE::uint8 * buf, XE::uint64 buf_sz, XE::uint64 block_sz, XE::uint64 * hash )
	{
		if( buf_sz == 0 )
			return;

		XE::uint64 w[80]{};
		loader< XE::uint64 > m( buf );

		const auto ssig0 = []( const XE::uint64 x ) -> XE::uint64
		{
			return ( rotr( x, 1 ) ^ rotr( x, 8 ) ^ ror<XE::uint64>( x, 7 ) );
		};
		const auto ssig1 = []( const XE::uint64 x ) -> XE::uint64
		{
			return ( rotr( x, 19 ) ^ rotr( x, 61 ) ^ ror<XE::uint64>( x, 6 ) );
		};
		const auto round = [&w]( XE::uint64 & a, XE::uint64 & b, XE::uint64 & c, XE::uint64 & d, XE::uint64 & e, XE::uint64 & f, XE::uint64 & g, XE::uint64 & h, XE::uint32 t ) -> void
		{
			const auto ch = []( const XE::uint64 x, const XE::uint64 y, const XE::uint64 z ) -> XE::uint64
			{
				return ( ( x & ( y ^ z ) ) ^ z );
			};
			const auto maj = []( const XE::uint64 x, const XE::uint64 y, const XE::uint64 z ) -> XE::uint64
			{
				return ( ( x & ( y | z ) ) | ( y & z ) );
			};
			const auto bsig0 = []( const XE::uint64 x ) -> XE::uint64
			{
				return ( rotr( x, 28 ) ^ rotr( x, 34 ) ^ rotr( x, 39 ) );
			};
			const auto bsig1 = []( const XE::uint64 x ) -> XE::uint64
			{
				return ( rotr( x, 14 ) ^ rotr( x, 18 ) ^ rotr( x, 41 ) );
			};

			const XE::uint64 t1 = h + bsig1( e ) + ch( e, f, g ) + sha2_64_k_table[t] + w[t];
			const XE::uint64 t2 = bsig0( a ) + maj( a, b, c );

			h = t1;
			d += h;
			h += t2;
		};

		for( int t = 0; t < 16; ++t )
		{
			w[t] = m[t];
		}
		for( int t = 16; t < 80; ++t )
		{
			w[t] = ssig1( w[t - 2] ) + w[t - 7] + ssig0( w[t - 15] ) + w[t - 16];
		}

		XE::uint64 a = hash[0];
		XE::uint64 b = hash[1];
		XE::uint64 c = hash[2];
		XE::uint64 d = hash[3];
		XE::uint64 e = hash[4];
		XE::uint64 f = hash[5];
		XE::uint64 g = hash[6];
		XE::uint64 h = hash[7];

		for( int t = 0; t < 10; ++t )
		{
			round( a, b, c, d, e, f, g, h, ( 8 * t ) + 0 );
			round( h, a, b, c, d, e, f, g, ( 8 * t ) + 1 );
			round( g, h, a, b, c, d, e, f, ( 8 * t ) + 2 );
			round( f, g, h, a, b, c, d, e, ( 8 * t ) + 3 );
			round( e, f, g, h, a, b, c, d, ( 8 * t ) + 4 );
			round( d, e, f, g, h, a, b, c, ( 8 * t ) + 5 );
			round( c, d, e, f, g, h, a, b, ( 8 * t ) + 6 );
			round( b, c, d, e, f, g, h, a, ( 8 * t ) + 7 );
		}

		hash[0] += a;
		hash[1] += b;
		hash[2] += c;
		hash[3] += d;
		hash[4] += e;
		hash[5] += f;
		hash[6] += g;
		hash[7] += h;

		sha2_64_add( buf + block_sz, buf_sz - block_sz, block_sz, hash );
	}

	void sha3_add( const XE::uint8 * buf, XE::uint64 buf_sz, XE::uint64 block_sz, XE::uint64 * hash )
	{
		if( buf_sz == 0 )
			return;

		loader2< XE::uint64 > m( buf );

		for( int i = 0; i < ( block_sz / 8 ); ++i )
		{
			hash[i] ^= m[i];
		}

		auto round = [&hash]( const int index )
		{
			const auto theta = [&hash]()
			{
				const XE::uint64 c[5] =
				{
					( hash[0] ^ hash[5 * 1 + 0] ^ hash[5 * 2 + 0] ^ hash[5 * 3 + 0] ^ hash[5 * 4 + 0] ),
					( hash[1] ^ hash[5 * 1 + 1] ^ hash[5 * 2 + 1] ^ hash[5 * 3 + 1] ^ hash[5 * 4 + 1] ),
					( hash[2] ^ hash[5 * 1 + 2] ^ hash[5 * 2 + 2] ^ hash[5 * 3 + 2] ^ hash[5 * 4 + 2] ),
					( hash[3] ^ hash[5 * 1 + 3] ^ hash[5 * 2 + 3] ^ hash[5 * 3 + 3] ^ hash[5 * 4 + 3] ),
					( hash[4] ^ hash[5 * 1 + 4] ^ hash[5 * 2 + 4] ^ hash[5 * 3 + 4] ^ hash[5 * 4 + 4] )
				};
				const XE::uint64 d[5] =
				{
					c[4] ^ rotl( c[1], 1 ),
					c[0] ^ rotl( c[2], 1 ),
					c[1] ^ rotl( c[3], 1 ),
					c[2] ^ rotl( c[4], 1 ),
					c[3] ^ rotl( c[0], 1 )
				};

				for( size_t i = 0; i < 5; ++i )
				{
					hash[5 * i + 0] ^= d[0];
					hash[5 * i + 1] ^= d[1];
					hash[5 * i + 2] ^= d[2];
					hash[5 * i + 3] ^= d[3];
					hash[5 * i + 4] ^= d[4];
				}
			};
			const auto rho_pi_chi = [&hash]()
			{
				const XE::uint64 tmp[5 * 5] =
				{
					rotl( hash[5 * 0 + 0], ( 0 % 64 ) ),
					rotl( hash[5 * 1 + 1], ( 300 % 64 ) ),
					rotl( hash[5 * 2 + 2], ( 171 % 64 ) ),
					rotl( hash[5 * 3 + 3], ( 21 % 64 ) ),
					rotl( hash[5 * 4 + 4], ( 78 % 64 ) ),

					rotl( hash[5 * 0 + 3], ( 28 % 64 ) ),
					rotl( hash[5 * 1 + 4], ( 276 % 64 ) ),
					rotl( hash[5 * 2 + 0], ( 3 % 64 ) ),
					rotl( hash[5 * 3 + 1], ( 45 % 64 ) ),
					rotl( hash[5 * 4 + 2], ( 253 % 64 ) ),

					rotl( hash[5 * 0 + 1], ( 1 % 64 ) ),
					rotl( hash[5 * 1 + 2], ( 6 % 64 ) ),
					rotl( hash[5 * 2 + 3], ( 153 % 64 ) ),
					rotl( hash[5 * 3 + 4], ( 136 % 64 ) ),
					rotl( hash[5 * 4 + 0], ( 210 % 64 ) ),

					rotl( hash[5 * 0 + 4], ( 91 % 64 ) ),
					rotl( hash[5 * 1 + 0], ( 36 % 64 ) ),
					rotl( hash[5 * 2 + 1], ( 10 % 64 ) ),
					rotl( hash[5 * 3 + 2], ( 15 % 64 ) ),
					rotl( hash[5 * 4 + 3], ( 120 % 64 ) ),

					rotl( hash[5 * 0 + 2], ( 190 % 64 ) ),
					rotl( hash[5 * 1 + 3], ( 55 % 64 ) ),
					rotl( hash[5 * 2 + 4], ( 231 % 64 ) ),
					rotl( hash[5 * 3 + 0], ( 105 % 64 ) ),
					rotl( hash[5 * 4 + 1], ( 66 % 64 ) )
				};

				for( size_t i = 0; i < 5; ++i )
				{
					hash[5 * i + 0] = tmp[5 * i + 0] ^ ( ( ~tmp[5 * i + 1] ) & tmp[5 * i + 2] );
					hash[5 * i + 1] = tmp[5 * i + 1] ^ ( ( ~tmp[5 * i + 2] ) & tmp[5 * i + 3] );
					hash[5 * i + 2] = tmp[5 * i + 2] ^ ( ( ~tmp[5 * i + 3] ) & tmp[5 * i + 4] );
					hash[5 * i + 3] = tmp[5 * i + 3] ^ ( ( ~tmp[5 * i + 4] ) & tmp[5 * i + 0] );
					hash[5 * i + 4] = tmp[5 * i + 4] ^ ( ( ~tmp[5 * i + 0] ) & tmp[5 * i + 1] );
				}
			};
			const auto iota = [&hash]( const int index )
			{
				hash[0] ^= sha3_round_table[index];
			};

			theta();
			rho_pi_chi();
			iota( index );
		};

		for( size_t i = 0; i < 24; ++i )
		{
			round( i );
		}

		sha3_add( buf + block_sz, buf_sz - block_sz, block_sz, hash );
	}
}


XE::SHA1::SHA1()
{
	Reset();
}

XE::SHA1::SHA1( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA1::SHA1( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA1::operator==( const XE::SHA1 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA1::operator!=( const XE::SHA1 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA1 & XE::SHA1::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha1_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA1 & XE::SHA1::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA1 & XE::SHA1::Finalize()
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
		_Buffer[_Index - 8 + i] = ror<XE::uint8>( sizeCounterBitsH, ( 8 * ( 3 - i ) ) );
		_Buffer[_Index - 4 + i] = ror<XE::uint8>( sizeCounterBitsL, ( 8 * ( 3 - i ) ) );
	}

	sha1_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

	_Index = 0;

	return *this;
}

void XE::SHA1::Reset()
{
	_Index = 0;
	_Count = 0;
	_Buffer.fill( 0 );
	_Hash[0] = 0x67452301;
	_Hash[1] = 0xEFCDAB89;
	_Hash[2] = 0x98BADCFE;
	_Hash[3] = 0x10325476;
	_Hash[4] = 0xC3D2E1F0;
}

void XE::SHA1::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA1::Reset( const XE::String & val )
{
	Reset( XE::MemoryView( val.c_str(), val.size() ) );
}

XE::MemoryView XE::SHA1::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint32 ) };
}

XE::String XE::SHA1::ToString( bool up /*= true*/ ) const
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

std::array<XE::uint8, XE::SHA1::HASH_SIZE> XE::SHA1::ToArray() const
{
	std::array<XE::uint8, XE::SHA1::HASH_SIZE> result;

	auto ptr = result.data();

	for( auto h : _Hash )
	{
		for( int i = ( sizeof( XE::uint32 ) - 1 ); i >= 0; --i )
		{
			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}


XE::SHA2_224::SHA2_224()
{
	Reset();
}

XE::SHA2_224::SHA2_224( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA2_224::SHA2_224( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA2_224::operator==( const SHA2_224 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA2_224::operator!=( const SHA2_224 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA2_224 & XE::SHA2_224::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha2_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA2_224 & XE::SHA2_224::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA2_224 & XE::SHA2_224::Finalize()
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
		_Buffer[_Index - 8 + i] = ror<XE::uint8>( sizeCounterBitsH, ( 8 * ( 3 - i ) ) );
		_Buffer[_Index - 4 + i] = ror<XE::uint8>( sizeCounterBitsL, ( 8 * ( 3 - i ) ) );
	}

	sha2_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

	_Index = 0;

	return *this;
}

void XE::SHA2_224::Reset()
{
	_Index = 0;
	_Count = 0;
	_Buffer.fill( 0 );

	_Hash[0] = 0xC1059ED8;
	_Hash[1] = 0x367CD507;
	_Hash[2] = 0x3070DD17;
	_Hash[3] = 0xF70E5939;
	_Hash[4] = 0xFFC00B31;
	_Hash[5] = 0x68581511;
	_Hash[6] = 0x64F98FA7;
	_Hash[7] = 0xBEFA4FA4;
}

void XE::SHA2_224::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA2_224::Reset( const XE::String & val )
{
	Reset( XE::MemoryView( val.c_str(), val.size() ) );
}

XE::MemoryView XE::SHA2_224::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint32 ) };
}

XE::String XE::SHA2_224::ToString( bool up /*= true*/ ) const
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

std::array<XE::uint8, XE::SHA2_224::HASH_SIZE> XE::SHA2_224::ToArray() const
{
	std::array<XE::uint8, XE::SHA2_224::HASH_SIZE> result;

	auto ptr = result.data();

	for( auto h : _Hash )
	{
		for( int i = ( sizeof( XE::uint32 ) - 1 ); i >= 0; --i )
		{
			if( XE::uint64( ptr - result.data() ) >= result.size() )
			{
				break;
			}

			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}


XE::SHA2_256::SHA2_256()
{
	Reset();
}

XE::SHA2_256::SHA2_256( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA2_256::SHA2_256( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA2_256::operator==( const SHA2_256 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA2_256::operator!=( const SHA2_256 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA2_256 & XE::SHA2_256::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha2_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA2_256 & XE::SHA2_256::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA2_256 & XE::SHA2_256::Finalize()
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
		_Buffer[_Index - 8 + i] = ror<XE::uint8>( sizeCounterBitsH, ( 8 * ( 3 - i ) ) );
		_Buffer[_Index - 4 + i] = ror<XE::uint8>( sizeCounterBitsL, ( 8 * ( 3 - i ) ) );
	}

	sha2_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

	_Index = 0;

	return *this;
}

void XE::SHA2_256::Reset()
{
	_Index = 0;
	_Count = 0;
	_Buffer.fill( 0 );
	_Hash[0] = 0x6A09E667;
	_Hash[1] = 0xBB67AE85;
	_Hash[2] = 0x3C6EF372;
	_Hash[3] = 0xA54FF53A;
	_Hash[4] = 0x510E527F;
	_Hash[5] = 0x9B05688C;
	_Hash[6] = 0x1F83D9AB;
	_Hash[7] = 0x5BE0CD19;
}

void XE::SHA2_256::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA2_256::Reset( const XE::String & val )
{
	Reset( XE::MemoryView( val.c_str(), val.size() ) );
}

XE::MemoryView XE::SHA2_256::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint32 ) };
}

XE::String XE::SHA2_256::ToString( bool up /*= true*/ ) const
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

std::array<XE::uint8, XE::SHA2_256::HASH_SIZE> XE::SHA2_256::ToArray() const
{
	std::array<XE::uint8, XE::SHA2_256::HASH_SIZE> result;

	auto ptr = result.data();

	for( auto h : _Hash )
	{
		for( int i = ( sizeof( XE::uint32 ) - 1 ); i >= 0; --i )
		{
			if( XE::uint64( ptr - result.data() ) >= result.size() )
			{
				break;
			}

			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}


XE::SHA2_384::SHA2_384()
{
	Reset();
}

XE::SHA2_384::SHA2_384( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA2_384::SHA2_384( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA2_384::operator==( const SHA2_384 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA2_384::operator!=( const SHA2_384 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA2_384 & XE::SHA2_384::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha2_64_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA2_384 & XE::SHA2_384::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA2_384 & XE::SHA2_384::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = 1 << 7;

	const auto len = static_cast<int>( ( BUFFER_SIZE - ( _Index + 16 ) ) % ( BUFFER_SIZE / 2 ) ) + 16;
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}

	XE::uint64 sizeCounterBitsL = _Count * 8;
	XE::uint64 sizeCounterBitsH = 0;
	for( int i = 0; i < 8; ++i )
	{
		_Buffer[_Index - 16 + i] = ror<XE::uint8>( sizeCounterBitsH, ( 8 * ( 7 - i ) ) );
		_Buffer[_Index -  8 + i] = ror<XE::uint8>( sizeCounterBitsL, ( 8 * ( 7 - i ) ) );
	}

	sha2_64_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

	_Index = 0;

	return *this;
}

void XE::SHA2_384::Reset()
{
	_Index = 0;
	_Count = 0;
	_Buffer.fill( 0 );
	_Hash[0] = 0xCBBB9D5DC1059ED8;
	_Hash[1] = 0x629A292A367CD507;
	_Hash[2] = 0x9159015A3070DD17;
	_Hash[3] = 0x152FECD8F70E5939;
	_Hash[4] = 0x67332667FFC00B31;
	_Hash[5] = 0x8EB44A8768581511;
	_Hash[6] = 0xDB0C2E0D64F98FA7;
	_Hash[7] = 0x47B5481DBEFA4FA4;
}

void XE::SHA2_384::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA2_384::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::SHA2_384::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint64 ) };
}

XE::String XE::SHA2_384::ToString( bool up /*= true*/ ) const
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

std::array<XE::uint8, XE::SHA2_384::HASH_SIZE> XE::SHA2_384::ToArray() const
{
	std::array<XE::uint8, XE::SHA2_384::HASH_SIZE> result;

	auto ptr = result.data();

	for( size_t i = 0; i < _Hash.size() - 2; i++ )
	{
		for( int j = ( sizeof( XE::uint64 ) - 1 ); j >= 0; --j )
		{
			*( ptr++ ) = ror<XE::uint8>( _Hash[i], ( j * 8 ) );
		}
	}

	return result;
}


XE::SHA2_512::SHA2_512()
{
	Reset();
}

XE::SHA2_512::SHA2_512( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA2_512::SHA2_512( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA2_512::operator==( const SHA2_512 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA2_512::operator!=( const SHA2_512 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA2_512 & XE::SHA2_512::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha2_64_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA2_512 & XE::SHA2_512::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA2_512 & XE::SHA2_512::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = 1 << 7;

	const auto len = static_cast<int>( ( BUFFER_SIZE - ( _Index + 16 ) ) % ( BUFFER_SIZE / 2 ) ) + 16;
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}

	XE::uint64 sizeCounterBitsL = _Count * 8;
	XE::uint64 sizeCounterBitsH = 0;
	for( int i = 0; i < 8; ++i )
	{
		_Buffer[_Index - 16 + i] = ror<XE::uint8>( sizeCounterBitsH, ( 8 * ( 7 - i ) ) );
		_Buffer[_Index - 8 + i] = ror<XE::uint8>( sizeCounterBitsL, ( 8 * ( 7 - i ) ) );
	}

	sha2_64_add( _Buffer.data(), _Index, BUFFER_SIZE / 2, _Hash.data() );

	_Index = 0;

	return *this;
}

void XE::SHA2_512::Reset()
{
	_Index = 0;
	_Count = 0;
	_Buffer.fill( 0 );
	_Hash[0] = 0x6A09E667F3BCC908;
	_Hash[1] = 0xBB67AE8584CAA73B;
	_Hash[2] = 0x3C6EF372FE94F82B;
	_Hash[3] = 0xA54FF53A5F1D36F1;
	_Hash[4] = 0x510E527FADE682D1;
	_Hash[5] = 0x9B05688C2B3E6C1F;
	_Hash[6] = 0x1F83D9ABFB41BD6B;
	_Hash[7] = 0x5BE0CD19137E2179;
}

void XE::SHA2_512::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA2_512::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::SHA2_512::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint64 ) };
}

XE::String XE::SHA2_512::ToString( bool up /*= true*/ ) const
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

std::array<XE::uint8, XE::SHA2_512::HASH_SIZE> XE::SHA2_512::ToArray() const
{
	std::array<XE::uint8, XE::SHA2_512::HASH_SIZE> result;

	auto ptr = result.data();

	for( size_t i = 0; i < _Hash.size(); i++ )
	{
		for( int j = ( sizeof( XE::uint64 ) - 1 ); j >= 0; --j )
		{
			*( ptr++ ) = ror<XE::uint8>( _Hash[i], ( j * 8 ) );
		}
	}

	return result;
}


XE::SHA3_224::SHA3_224()
{
	Reset();
}

XE::SHA3_224::SHA3_224( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA3_224::SHA3_224( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA3_224::operator==( const SHA3_224 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA3_224::operator!=( const SHA3_224 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA3_224 & XE::SHA3_224::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA3_224 & XE::SHA3_224::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA3_224 & XE::SHA3_224::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = FLAG;

	const auto len = static_cast<int>( ( ( 2 * BUFFER_SIZE ) - _Index ) % BUFFER_SIZE );
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}
	_Buffer[_Index - 1] |= ( 1 << 7 );

	sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

	std::array<XE::uint8, BUFFER_SIZE> empty; empty.fill( 0 );
	for( size_t i = BUFFER_SIZE; i < HASH_SIZE; i+= BUFFER_SIZE )
	{
		sha3_add( empty.data(), empty.size(), BUFFER_SIZE, _Hash.data() );
	}

	_Index = 0;

	return ( *this );
}

void XE::SHA3_224::Reset()
{
	_Index = 0;
	_Count = 0;
	_Hash.fill( 0 );
	_Buffer.fill( 0 );
}

void XE::SHA3_224::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA3_224::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::SHA3_224::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint64 ) };
}

XE::String XE::SHA3_224::ToString( bool up /*= true */ ) const
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

std::array<XE::uint8, XE::SHA3_224::HASH_SIZE> XE::SHA3_224::ToArray() const
{
	std::array<XE::uint8, XE::SHA3_224::HASH_SIZE> result;

	auto ptr = result.data();
	for( const auto h : _Hash )
	{
		for( size_t i = 0; i < sizeof( XE::uint64 ); i++ )
		{
			if( size_t( ptr - result.data() ) >= result.size() )
			{
				break;
			}

			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}


XE::SHA3_256::SHA3_256()
{
	Reset();
}

XE::SHA3_256::SHA3_256( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA3_256::SHA3_256( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA3_256::operator==( const SHA3_256 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA3_256::operator!=( const SHA3_256 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA3_256 & XE::SHA3_256::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA3_256 & XE::SHA3_256::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA3_256 & XE::SHA3_256::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = FLAG;

	const auto len = static_cast<int>( ( ( 2 * BUFFER_SIZE ) - _Index ) % BUFFER_SIZE );
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}
	_Buffer[_Index - 1] |= ( 1 << 7 );

	sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

	std::array<XE::uint8, BUFFER_SIZE> empty; empty.fill( 0 );
	for( size_t i = BUFFER_SIZE; i < HASH_SIZE; i += BUFFER_SIZE )
	{
		sha3_add( empty.data(), empty.size(), BUFFER_SIZE, _Hash.data() );
	}

	_Index = 0;

	return ( *this );
}

void XE::SHA3_256::Reset()
{
	_Index = 0;
	_Count = 0;
	_Hash.fill( 0 );
	_Buffer.fill( 0 );
}

void XE::SHA3_256::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA3_256::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::SHA3_256::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint64 ) };
}

XE::String XE::SHA3_256::ToString( bool up /*= true */ ) const
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

std::array<XE::uint8, XE::SHA3_256::HASH_SIZE> XE::SHA3_256::ToArray() const
{
	std::array<XE::uint8, XE::SHA3_256::HASH_SIZE> result;

	auto ptr = result.data();
	for( const auto h : _Hash )
	{
		for( size_t i = 0; i < sizeof( XE::uint64 ); i++ )
		{
			if( size_t( ptr - result.data() ) >= result.size() )
			{
				break;
			}

			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}


XE::SHA3_384::SHA3_384()
{
	Reset();
}

XE::SHA3_384::SHA3_384( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA3_384::SHA3_384( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA3_384::operator==( const SHA3_384 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA3_384::operator!=( const SHA3_384 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA3_384 & XE::SHA3_384::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA3_384 & XE::SHA3_384::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA3_384 & XE::SHA3_384::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = FLAG;

	const auto len = static_cast<int>( ( ( 2 * BUFFER_SIZE ) - _Index ) % BUFFER_SIZE );
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}
	_Buffer[_Index - 1] |= ( 1 << 7 );

	sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

	std::array<XE::uint8, BUFFER_SIZE> empty; empty.fill( 0 );
	for( size_t i = BUFFER_SIZE; i < HASH_SIZE; i += BUFFER_SIZE )
	{
		sha3_add( empty.data(), empty.size(), BUFFER_SIZE, _Hash.data() );
	}

	_Index = 0;

	return ( *this );
}

void XE::SHA3_384::Reset()
{
	_Index = 0;
	_Count = 0;
	_Hash.fill( 0 );
	_Buffer.fill( 0 );
}

void XE::SHA3_384::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA3_384::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::SHA3_384::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint64 ) };
}

XE::String XE::SHA3_384::ToString( bool up /*= true */ ) const
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

std::array<XE::uint8, XE::SHA3_384::HASH_SIZE> XE::SHA3_384::ToArray() const
{
	std::array<XE::uint8, XE::SHA3_384::HASH_SIZE> result;

	auto ptr = result.data();
	for( const auto h : _Hash )
	{
		for( size_t i = 0; i < sizeof( XE::uint64 ); i++ )
		{
			if( size_t( ptr - result.data() ) >= result.size() )
			{
				break;
			}

			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}


XE::SHA3_512::SHA3_512()
{
	Reset();
}

XE::SHA3_512::SHA3_512( XE::MemoryView val )
{
	Reset( val );
}

XE::SHA3_512::SHA3_512( const XE::String & val )
{
	Reset( val );
}

bool XE::SHA3_512::operator==( const SHA3_512 & val ) const
{
	return std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

bool XE::SHA3_512::operator!=( const SHA3_512 & val ) const
{
	return !std::equal( _Hash.begin(), _Hash.end(), val._Hash.begin() );
}

XE::SHA3_512 & XE::SHA3_512::Add( XE::MemoryView val )
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
			_Count += BUFFER_SIZE;

			sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

			_Index = 0;
		}
	}

	return *this;
}

XE::SHA3_512 & XE::SHA3_512::Add( const XE::String & val )
{
	return Add( XE::MemoryView{ val.c_str(), val.size() } );
}

XE::SHA3_512 & XE::SHA3_512::Finalize()
{
	_Count += _Index;

	_Buffer[_Index++] = FLAG;

	const auto len = static_cast<int>( ( ( 2 * BUFFER_SIZE ) - _Index ) % BUFFER_SIZE );
	for( size_t i = 0; i < len; i++ )
	{
		_Buffer[_Index++] = 0;
	}
	_Buffer[_Index - 1] |= ( 1 << 7 );

	sha3_add( _Buffer.data(), _Buffer.size(), BUFFER_SIZE, _Hash.data() );

	std::array<XE::uint8, BUFFER_SIZE> empty; empty.fill( 0 );
	for( size_t i = BUFFER_SIZE; i < HASH_SIZE; i += BUFFER_SIZE )
	{
		sha3_add( empty.data(), empty.size(), BUFFER_SIZE, _Hash.data() );
	}

	_Index = 0;

	return ( *this );
}

void XE::SHA3_512::Reset()
{
	_Index = 0;
	_Count = 0;
	_Hash.fill( 0 );
	_Buffer.fill( 0 );
}

void XE::SHA3_512::Reset( XE::MemoryView val )
{
	Reset();

	Add( val );
}

void XE::SHA3_512::Reset( const XE::String & val )
{
	Reset();

	Add( val );
}

XE::MemoryView XE::SHA3_512::ToView() const
{
	return { reinterpret_cast<const char *>( _Hash.data() ), _Hash.size() * sizeof( XE::uint64 ) };
}

XE::String XE::SHA3_512::ToString( bool up /*= true */ ) const
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

std::array<XE::uint8, XE::SHA3_512::HASH_SIZE> XE::SHA3_512::ToArray() const
{
	std::array<XE::uint8, XE::SHA3_512::HASH_SIZE> result;

	auto ptr = result.data();
	for( const auto h : _Hash )
	{
		for( size_t i = 0; i < sizeof( XE::uint64 ); i++ )
		{
			if( size_t( ptr - result.data() ) >= result.size() )
			{
				break;
			}

			*( ptr++ ) = ror<XE::uint8>( h, ( i * 8 ) );
		}
	}

	return result;
}
