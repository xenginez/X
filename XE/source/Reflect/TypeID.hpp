/*!
 * \file	TypeID.hpp
 *
 * \author	ZhengYuanQing
 * \date	2019/12/06
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPEID_HPP__1C7147FB_7591_47E2_B804_CD182F01438A
#define TYPEID_HPP__1C7147FB_7591_47E2_B804_CD182F01438A

#include "Type.h"

template< typename T > struct EnumID
{
	static XE::MetaEnumCPtr Get( const T * val = nullptr )
	{
		return nullptr;
	}
};

template< typename T > struct ClassID
{
	static XE::MetaClassCPtr Get( const T * val = nullptr )
	{
		if( val )
		{
			return val->GetMetaClass();
		}

		return T::GetMetaClassStatic();

		return nullptr;
	}
};

template< typename T > struct ClassID< T * >
{
	static XE::MetaClassCPtr Get( const T * val = nullptr )
	{
		return ::ClassID< T >::Get( val );
	}
};

template< typename T > struct ClassID< T & >
{
	static XE::MetaClassCPtr Get( const T * val = nullptr )
	{
		return ::ClassID< T >::Get( val );
	}
};

template< typename T > struct ClassID< const T >
{
	static XE::MetaClassCPtr Get( const T * val = nullptr )
	{
		return ::ClassID< T >::Get( val );
	}
};

template< typename T > struct ClassID< XE::SharedPtr< T > >
{
	static XE::MetaClassCPtr Get( const XE::SharedPtr< T > & val = nullptr )
	{
		if( val != nullptr )
		{
			return ::ClassID< T >::Get( val.get() );
		}

		return ::ClassID< T >::Get();
	}
};

template< typename T > struct TypeID
{
	static XE::MetaTypeCPtr Get( const T * val = nullptr )
	{
		using raw_t = typename XE::TypeTraits< T >::raw_t;

		return SP_CAST< const XE::MetaType >( std::conditional_t< std::is_enum_v< raw_t >, ::EnumID< raw_t >, ::ClassID< raw_t > >::Get( val ) );
	}
};

template< typename T > struct TypeID< XE::SharedPtr< T > >
{
	static XE::MetaTypeCPtr Get( const XE::SharedPtr< T > * val = nullptr )
	{
		if( val != nullptr )
		{
			return ::TypeID< T >::Get( val->get() );
		}

		return ::TypeID< T >::Get();
	}
};

BEG_XE_NAMESPACE

template< typename T > XE_INLINE XE::String ToString( const T * _Val )
{
	using raw_t = XE::TypeTraits< T >::raw_t;

	return ::TypeID< raw_t >::Get( _Val )->GetFullName();
}

template< typename T > XE_INLINE XE::String ToString( const T & _Val )
{
	using raw_t = XE::TypeTraits< T >::raw_t;

	return ::TypeID< raw_t >::Get( &_Val )->GetFullName();
}

END_XE_NAMESPACE

#endif // TYPEID_HPP__1C7147FB_7591_47E2_B804_CD182F01438A
