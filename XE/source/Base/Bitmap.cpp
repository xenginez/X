#include "Bitmap.h"

static constexpr unsigned char log_2[256] = {
	0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

XE::Bitmap::Bitmap( XE::uint64 count, bool val /*= false */ )
	:_Count( count ), _Data( ( count - 1 ) / _Bitsperword, val ? 0xFF : 0 )
{

}

XE::Bitmap::Bitmap( Bitmap && val )
{
	std::swap( _Count, val._Count );
	std::swap( _Data, val._Data );
}

XE::Bitmap::Bitmap( const Bitmap & val )
	:_Count( val._Count ), _Data( val._Data )
{

}

XE::Bitmap & XE::Bitmap::operator=( Bitmap && val )
{
	Swap( val );

	return *this;
}

XE::Bitmap & XE::Bitmap::operator=( const Bitmap & val )
{
	_Count = val._Count;
	_Data = val._Data;

	return *this;
}

XE::Bitmap & XE::Bitmap::operator>>=( XE::uint64 index )
{
	ptrdiff_t _Words = ( _Count - 1 ) / _Bitsperword;

	const auto _Wordshift = static_cast< ptrdiff_t >( index / _Bitsperword );
	if( _Wordshift != 0 )
	{
		for( ptrdiff_t _Wpos = 0; _Wpos <= _Words; ++_Wpos )
		{
			_Data[_Wpos] = _Wordshift <= _Words - _Wpos ? _Data[_Wpos + _Wordshift] : 0;
		}
	}

	if( ( index %= _Bitsperword ) != 0 )
	{
		for( ptrdiff_t _Wpos = 0; _Wpos < _Words; ++_Wpos )
		{
			_Data[_Wpos] = ( _Data[_Wpos] >> index ) | ( _Data[_Wpos + 1] << ( _Bitsperword - index ) );
		}

		_Data[_Words] >>= index;
	}

	return *this;
}

XE::Bitmap XE::Bitmap::operator>>( XE::uint64 val ) const
{
	return XE::Bitmap( *this ) >>= val;
}

bool XE::Bitmap::operator!=( const XE::Bitmap & val ) const
{
	return _Count != val._Count || _Data != val._Data;
}

bool XE::Bitmap::operator==( const XE::Bitmap & val ) const
{
	return  _Count == val._Count && _Data == val._Data;
}

XE::Bitmap & XE::Bitmap::operator&=( const XE::Bitmap & val )
{
	size_t _Words = ( _Count - 1 ) / _Bitsperword;

	for( size_t _Wpos = 0; _Wpos <= _Words; ++_Wpos )
	{
		_Data[_Wpos] &= val._Data[_Wpos];
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::operator|=( const XE::Bitmap & val )
{
	size_t _Words = ( _Count - 1 ) / _Bitsperword;

	for( size_t _Wpos = 0; _Wpos <= _Words; ++_Wpos )
	{
		_Data[_Wpos] |= val._Data[_Wpos];
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::operator^=( const XE::Bitmap & val )
{
	size_t _Words = ( _Count - 1 ) / _Bitsperword;

	for( size_t _Wpos = 0; _Wpos <= _Words; ++_Wpos )
	{
		_Data[_Wpos] ^= val._Data[_Wpos];
	}

	return *this;
}

XE::Bitmap XE::Bitmap::operator~() const
{
	return XE::Bitmap( *this ).Flip();
}

XE::Bitmap & XE::Bitmap::operator<<=( XE::uint64 index )
{
	ptrdiff_t _Words = ( _Count - 1 ) / _Bitsperword;

	const auto _Wordshift = static_cast< ptrdiff_t >( index / _Bitsperword );
	if( _Wordshift != 0 )
	{
		for( ptrdiff_t _Wpos = _Words; 0 <= _Wpos; --_Wpos )
		{
			_Data[_Wpos] = _Wordshift <= _Wpos ? _Data[_Wpos - _Wordshift] : 0;
		}
	}

	if( ( index %= _Bitsperword ) != 0 )
	{
		for( ptrdiff_t _Wpos = _Words; 0 < _Wpos; --_Wpos )
		{
			_Data[_Wpos] = ( _Data[_Wpos] << index ) | ( _Data[_Wpos - 1] >> ( _Bitsperword - index ) );
		}

		_Data[0] <<= index;
	}

	return *this;
}

XE::Bitmap XE::Bitmap::operator<<( XE::uint64 val ) const
{
	return XE::Bitmap( *this ) <<= val;
}

bool XE::Bitmap::All() const
{
	bool _No_padding = _Count % CHAR_BIT;
	size_t _Words = ( _Count - 1 ) / _Bitsperword;

	for( size_t i = 0; i < _Words + _No_padding; ++i )
	{
		if( _Data[i] != 0xFF )
		{
			return false;
		}
	}

	return _No_padding || _Data[_Words] == ( static_cast< XE::uint8 >( 1 ) << ( _Count % _Bitsperword ) ) - 1;
}

bool XE::Bitmap::Any() const
{
	size_t _Words = ( _Count - 1 ) / _Bitsperword;

	for( size_t _Wpos = 0; _Wpos <= _Words; ++_Wpos )
	{
		if( _Data[_Wpos] != 0 )
		{
			return true;
		}
	}

	return false;
}

bool XE::Bitmap::None() const
{
	return !Any();
}

XE::uint64 XE::Bitmap::Count() const
{
	return _Count;
}

const XE::Array< XE::uint8 > & XE::Bitmap::Data() const
{
	return _Data;
}

bool XE::Bitmap::Test( XE::uint64 index ) const
{
	XE_ASSERT( _Count <= index && "out of range" );

	return ( _Data[index / _Bitsperword] & ( XE::uint8{ 1 } << index % _Bitsperword ) ) != 0;
}

XE::Bitmap & XE::Bitmap::Set( bool val )
{
	for( auto & i : _Data )
	{
		i = val ? 0xFF : 0;
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::Set( XE::uint64 index, bool val )
{
	XE_ASSERT( _Count <= index && "out of range" );

	auto & _Selected_word = _Data[index / _Bitsperword];
	const auto _Bit = XE::uint8{ 1 } << index % _Bitsperword;
	if( val )
	{
		_Selected_word |= _Bit;
	}
	else
	{
		_Selected_word &= ~_Bit;
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::Set( XE::uint64 index, XE::uint64 count, bool val )
{
	for ( size_t i = 0; i < count; i++ )
	{
		Set( index + i, val );
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::Flip()
{
	for( auto & i : _Data )
	{
		i = ~i;
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::Flip( XE::uint64 index )
{
	XE_ASSERT( _Count <= index && "out of range" );

	_Data[index / _Bitsperword] ^= XE::uint8{ 1 } << index % _Bitsperword;

	return *this;
}

XE::Bitmap & XE::Bitmap::Flip( XE::uint64 index, XE::uint64 count )
{
	for ( size_t i = 0; i < count; i++ )
	{
		Flip( index + i );
	}

	return *this;
}

XE::Bitmap & XE::Bitmap::Reset( XE::uint64 count, bool val /*= false */ )
{
	_Count = count;

	_Data.resize( ( count - 1 ) / _Bitsperword, val ? 0xFF : 0 );

	return *this;
}

XE::uint64 XE::Bitmap::TrueCount() const
{
	bool _No_padding = _Count % CHAR_BIT;
	size_t _Words = ( _Count - 1 ) / _Bitsperword;

	int count = 0;

	for( XE::uint64 i = 0; i < _Words + _No_padding; ++i )
	{
		XE::uint8 x = _Data[i];

		while( x != 0 )
		{
			x = x & ( x - 1 );
			count++;
		}
	}

	if( _No_padding )
	{
		XE::uint64 c = _Count - ( _Words * CHAR_BIT );
		XE::uint8 x = _Data[_Words];
		while( c-- )
		{
			count += x & 1;
			x <<= 1;
		}
	}

	return count;
}

XE::uint64 XE::Bitmap::FalseCount() const
{
	return _Count - TrueCount();
}

XE::uint64 XE::Bitmap::FindFirstTrue() const
{
	XE::uint64 count = 0;

	for( XE::uint64 i = 0; i < _Data.size(); ++i )
	{
		XE::uint8 x = _Data[i] & ( -_Data[i] );

		if( x == 0 )
		{
			count += CHAR_BIT;
		}
		else
		{
			return count + log_2[x];
		}
	}

	return npos;
}

XE::uint64 XE::Bitmap::FindFirstFalse() const
{
	XE::uint64 count = 0;

	for( XE::uint64 i = 0; i < _Data.size(); ++i )
	{
		XE::uint8 x = ~_Data[i] & ( _Data[i] + 1 );

		if( x == 0 )
		{
			count += CHAR_BIT;
		}
		else
		{
			return count + log_2[x];
		}
	}

	return npos;
}

XE::uint64 XE::Bitmap::FindLastTrue() const
{
	for( XE::int64 i = _Data.size() - 1; i >= 0; --i )
	{
		if( _Data[i] != 0 )
		{
			XE::uint8 d = _Data[i];
			d |= ( d >> 1 );
			d |= ( d >> 2 );
			d |= ( d >> 4 );
			d ^= ( d >> 1 );
			switch( d )
			{
			case 1:
				return _Count - 8;
				break;
			case 2:
				return _Count - 7;
				break;
			case 4:
				return _Count - 6;
				break;
			case 8:
				return _Count - 5;
				break;
			case 16:
				return _Count - 4;
				break;
			case 32:
				return _Count - 3;
				break;
			case 64:
				return _Count - 2;
				break;
			case 128:
				return _Count - 1;
				break;
			default:
				break;
			}
		}
	}

	return npos;
}

XE::uint64 XE::Bitmap::FindLastFalse() const
{
	for ( XE::int64 i = _Data.size() - 1; i >= 0; --i )
	{
		if ( _Data[i] != 0 )
		{
			XE::uint8 d = ~_Data[i];
			d |= ( d >> 1 );
			d |= ( d >> 2 );
			d |= ( d >> 4 );
			d ^= ( d >> 1 );
			switch ( d )
			{
			case 1:
				return _Count - 8;
				break;
			case 2:
				return _Count - 7;
				break;
			case 4:
				return _Count - 6;
				break;
			case 8:
				return _Count - 5;
				break;
			case 16:
				return _Count - 4;
				break;
			case 32:
				return _Count - 3;
				break;
			case 64:
				return _Count - 2;
				break;
			case 128:
				return _Count - 1;
				break;
			default:
				break;
			}
		}
	}

	return npos;
}

XE::uint64 XE::Bitmap::FindSeriesTrue( XE::uint64 count ) const
{
	if ( ( count % CHAR_BIT ) != 0 )
	{
		auto i = ( count / CHAR_BIT );
		auto f = CHAR_BIT - ( count % CHAR_BIT );
		XE::uint8 x = _Data[i] & ( -_Data[i] );

		for ( size_t j = 1; j <= f; j++ )
		{
			x &= ~( XE::uint8( 1 ) << j );
		}

		if ( x == 0 )
		{
			count += CHAR_BIT;
		}
		else
		{
			return count + log_2[x];
		}
	}

	for ( XE::uint64 i = ( count / CHAR_BIT ) + 1; i < _Data.size(); ++i )
	{
		XE::uint8 x = _Data[i] & ( -_Data[i] );

		if ( x == 0 )
		{
			count += CHAR_BIT;
		}
		else
		{
			return count + log_2[x];
		}
	}

	return npos;
}

XE::uint64 XE::Bitmap::FindSeriesFalse( XE::uint64 count ) const
{
	if ( ( count % CHAR_BIT ) != 0 )
	{
		auto i = ( count / CHAR_BIT );
		auto f = CHAR_BIT - ( count % CHAR_BIT );
		XE::uint8 x = ~_Data[i] & ( _Data[i] + 1 );

		for ( size_t j = 1; j <= f; j++ )
		{
			x |= ( XE::uint8( 1 ) << j );
		}

		if ( x == 0 )
		{
			count += CHAR_BIT;
		}
		else
		{
			return count + log_2[x];
		}
	}

	for ( XE::uint64 i = ( count / CHAR_BIT ) + 1; i < _Data.size(); ++i )
	{
		XE::uint8 x = ~_Data[i] & ( _Data[i] + 1 );

		if ( x == 0 )
		{
			count += CHAR_BIT;
		}
		else
		{
			return count + log_2[x];
		}
	}

	return npos;
}

XE::Bitmap operator&( const XE::Bitmap & Left, const XE::Bitmap & Right )
{
	XE::Bitmap res = Left;

	return res &= Right;
}

XE::Bitmap operator|( const XE::Bitmap & Left, const XE::Bitmap & Right )
{
	XE::Bitmap res = Left;

	return res |= Right;
}

XE::Bitmap operator^( const XE::Bitmap & Left, const XE::Bitmap & Right )
{
	XE::Bitmap res = Left;

	return res ^= Right;
}
