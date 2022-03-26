/*!
 * \file	Flags.hpp
 *
 * \author	ZhengYuanQing
 * \date	2020/04/04
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FLAGS_HPP__C8E5511D_71B9_46C1_A4BF_35B5E8ADBDF3
#define FLAGS_HPP__C8E5511D_71B9_46C1_A4BF_35B5E8ADBDF3

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T > class Flags
{
	static_assert( std::is_enum_v< T >, "must be an enum type" );

public:
	using enum_type = T;

public:
	Flags()
		:_Value( 0 )
	{

	}

	Flags( enum_type val )
		:_Value( static_cast< XE::uint64 >( val ) )
	{

	}

	Flags( XE::uint64 val )
		:_Value( val )
	{

	}

	Flags( const Flags & val )
		:_Value( val._Value )
	{

	}

public:
	operator bool() const
	{
		return _Value != 0;
	}

public:
	Flags operator ~() const
	{
		return ~_Value;
	}

public:
	Flags operator |( enum_type val ) const
	{
		return _Value | static_cast< XE::uint64 >( val );
	}

	Flags operator |( const Flags & val ) const
	{
		return _Value | val._Value;
	}

	Flags operator &( enum_type val ) const
	{
		return _Value & static_cast< XE::uint64 >( val );
	}

	Flags operator &( const Flags & val ) const
	{
		return _Value & val._Value;
	}

	Flags operator << ( enum_type val ) const
	{
		return _Value << static_cast< XE::uint64 >( val );
	}

	Flags operator << ( const Flags & val ) const
	{
		return _Value << val._Value;
	}

	Flags operator >> ( enum_type val ) const
	{
		return _Value >> static_cast< XE::uint64 >( val );
	}

	Flags operator >> ( const Flags & val ) const
	{
		return _Value >> val._Value;
	}

public:
	Flags & operator =( enum_type val )
	{
		_Value = static_cast< XE::uint64 >( val );

		return *this;
	}

	Flags & operator =( const Flags & val )
	{
		_Value = val._Value;

		return *this;
	}

	Flags & operator |=( enum_type val )
	{
		_Value |= static_cast< XE::uint64 >( val );

		return *this;
	}

	Flags & operator |=( const Flags & val )
	{
		_Value |= val._Value;

		return *this;
	}

	Flags & operator &=( enum_type val )
	{
		_Value &= static_cast< XE::uint64 >( val );

		return *this;
	}

	Flags & operator &=( const Flags & val )
	{
		_Value &= val._Value;

		return *this;
	}

public:
	bool operator ||( enum_type val ) const
	{
		return ( _Value | static_cast< XE::uint64 >( val ) ) != 0;
	}

	bool operator ||( const Flags & val ) const
	{
		return ( _Value | val._Value ) != 0;
	}

	bool operator &&( enum_type val ) const
	{
		return ( _Value & static_cast< XE::uint64 >( val ) ) != 0;
	}

	bool operator &&( const Flags & val ) const
	{
		return ( _Value & val._Value ) != 0;
	}

	bool operator ==( enum_type val ) const
	{
		return _Value == static_cast< XE::uint64 >( val );
	}

	bool operator ==( const Flags & val ) const
	{
		return _Value == val._Value;
	}

	bool operator !=( enum_type val ) const
	{
		return _Value != static_cast< XE::uint64 >( val );
	}

	bool operator !=( const Flags & val ) const
	{
		return _Value != val._Value;
	}

public:
	XE::uint64 GetValue() const
	{
		return _Value;
	}

private:
	XE::uint64 _Value;
};

template< typename T > XE::Flags< T > MakeFlags( T arg0 )
{
	XE::Flags< T > flags( arg0 );

	return flags;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3, T arg4 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3 | arg4;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3, T arg4, T arg5 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3 | arg4 | arg5;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3 | arg4 | arg5 | arg6;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3 | arg4 | arg5 | arg6 | arg7;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3 | arg4 | arg5 | arg6 | arg7 | arg8;
}

template< typename T > XE::Flags< T > MakeFlags( T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8, T arg9 )
{
	XE::Flags< T > flags( arg0 );

	return flags | arg1 | arg2 | arg3 | arg4 | arg5 | arg6 | arg7 | arg8 | arg9;
}

template< typename T > XE_INLINE XE::Flags< T > operator |( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) | right;
}

template< typename T > XE_INLINE XE::Flags< T > operator &( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) & right;
}

template< typename T > XE_INLINE XE::Flags< T > operator <<( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) << right;
}

template< typename T > XE_INLINE XE::Flags< T > operator >>( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) >> right;
}

template< typename T > XE_INLINE bool operator ||( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) || right;
}

template< typename T > XE_INLINE bool operator &&( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) && right;
}

template< typename T > XE_INLINE bool operator ==( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) == right;
}

template< typename T > XE_INLINE bool operator !=( const T & left, const XE::Flags< T > & right )
{
	return XE::MakeFlags( left ) != right;
}


IMPLEMENT_META_TEMPLATE_CLASS( XE, Flags );


END_XE_NAMESPACE

#define DECL_FLAGS_2( TYPE, NAME ) using NAME = XE::Flags< TYPE >;
#define DECL_FLAGS_1( TYPE ) using TYPE##Flags = XE::Flags< TYPE >;
#define DECL_FLAGS( ... ) MACRO_EXP_(MACRO_GLUE(DECL_FLAGS_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))


#endif // FLAGS_HPP__C8E5511D_71B9_46C1_A4BF_35B5E8ADBDF3
