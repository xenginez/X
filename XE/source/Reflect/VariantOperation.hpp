/*!
 * \file	VariantOperation.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/04/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef VARIANTOPERATION_HPP__341FA70B_D2EA_46D1_98BF_13C605838578
#define VARIANTOPERATION_HPP__341FA70B_D2EA_46D1_98BF_13C605838578

#include "Variant.h"

BEG_XE_NAMESPACE

template<> struct VariantCreate< char * >
{
	static XE::VariantData Create( const char * val )
	{
		return { VariantWarpperData( XE::String( val ) ) };
	}
};

template<> struct VariantCreate< XE::AnsiString >
{
	static XE::VariantData Create( const XE::AnsiString & val )
	{
		return { VariantWarpperData( XE::String( val ) ) };
	}
};

template<> struct VariantCreate< XE::WideString >
{
	static XE::VariantData Create( const XE::WideString & val )
	{
		return { VariantWarpperData( XE::String( val ) ) };
	}
};

template< typename T > struct VariantCreate
{
	static  XE::VariantData Create( const T & val )
	{
		if constexpr( std::is_enum_v< T > )
		{
			return { XE::VariantEnumData( val ) };
		}
		else if constexpr( std::is_pointer_v< T > )
		{
			return { VariantPointerData( val ) };
		}
		else if constexpr( sizeof( T ) <= sizeof( XE::uint64 ) && std::is_trivial_v< T > )
		{
			return { XE::VariantSmallData( val ) };
		}
		else
		{
			return { XE::VariantWarpperData( val ) };
		}
	}
};



template<> struct VariantCast< bool >
{
	static bool Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< bool >(), val );
	}
};

template<> struct VariantCast< XE::int8 >
{
	static XE::int8 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::int8 >(), val );
	}
};

template<> struct VariantCast< XE::int16 >
{
	static XE::int16 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::int16 >(), val );
	}
};

template<> struct VariantCast< XE::int32 >
{
	static XE::int32 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::int32 >(), val );
	}
};

template<> struct VariantCast< XE::int64 >
{
	static XE::int64 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::int64 >(), val );
	}
};

template<> struct VariantCast< XE::uint8 >
{
	static XE::uint8 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::uint8 >(), val );
	}
};

template<> struct VariantCast< XE::uint16 >
{
	static XE::uint16 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::uint16 >(), val );
	}
};

template<> struct VariantCast< XE::uint32 >
{
	static XE::uint32 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::uint32 >(), val );
	}
};

template<> struct VariantCast< XE::uint64 >
{
	static XE::uint64 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::uint64 >(), val );
	}
};

template<> struct VariantCast< XE::float32 >
{
	static XE::float32 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::float32 >(), val );
	}
};

template<> struct VariantCast< XE::float64 >
{
	static XE::float64 Cast( const VariantData & val )
	{
		return std::visit( XE::VariantDataGetFundamental< XE::float64 >(), val );
	}
};

template<> struct VariantCast< XE::AnsiString >
{
	static XE::AnsiString Cast( const XE::VariantData & val )
	{
		if ( std::visit( VariantDataIsCanConvert< XE::String >(), val ) )
		{
			return *reinterpret_cast<XE::String *>( std::visit( VariantDataGetRawPointer(), const_cast<XE::VariantData &>( val ) ) );
		}

		throw std::bad_variant_access();
	}
};

template<> struct VariantCast< XE::WideString >
{
	static XE::WideString Cast( const XE::VariantData & val )
	{
		if( std::visit( VariantDataIsCanConvert< XE::String >(), val ) )
		{
			return *reinterpret_cast< XE::String * >( std::visit( VariantDataGetRawPointer(), const_cast< XE::VariantData & >( val ) ) );
		}

		throw std::bad_variant_access();
	}
};

template<> struct VariantCast< bool & >
{
	static bool & Cast( const VariantData & val )
	{
		return std::get< bool >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::int8 & >
{
	static XE::int8 & Cast( const VariantData & val )
	{
		return std::get< XE::int8 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::int16 & >
{
	static XE::int16 & Cast( const VariantData & val )
	{
		return std::get< XE::int16 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::int32 & >
{
	static XE::int32 & Cast( const VariantData & val )
	{
		return std::get< XE::int32 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::int64 & >
{
	static XE::int64 & Cast( const VariantData & val )
	{
		return std::get< XE::int64 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::uint8 & >
{
	static XE::uint8 & Cast( const VariantData & val )
	{
		return std::get< XE::uint8 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::uint16 & >
{
	static XE::uint16 & Cast( const VariantData & val )
	{
		return std::get< XE::uint16 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::uint32 & >
{
	static XE::uint32 & Cast( const VariantData & val )
	{
		return std::get< XE::uint32 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::uint64 & >
{
	static XE::uint64 & Cast( const VariantData & val )
	{
		return std::get< XE::uint64 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::float32 & >
{
	static XE::float32 & Cast( const VariantData & val )
	{
		return std::get< XE::float32 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< XE::float64 & >
{
	static XE::float64 & Cast( const VariantData & val )
	{
		return std::get< XE::float64 >( const_cast< VariantData & >( val ) );
	}
};

template<> struct VariantCast< const char * >
{
	static char const * Cast( const XE::VariantData & val )
	{
		if( std::visit( VariantDataIsCanConvert< XE::String >(), val ) )
		{
			return reinterpret_cast<XE::String *>( std::visit( VariantDataGetRawPointer(), const_cast<XE::VariantData &>( val ) ) )->c_str();
		}

		return nullptr;
	}
};

template< typename T > struct VariantCast
{
	static T Cast( const XE::VariantData & val )
	{
		if constexpr( std::is_enum_v< T > )
		{
			auto data = std::get< XE::VariantEnumData >( val );
			if( data.Type == TypeID< T >::Get().get() )
			{
				return XE::BitCast< T >( data.Value );
			}
		}
		else
		{
			return *( VariantCast< T * >::Cast( val ) );
		}

		throw std::bad_variant_access();
	}
};

template< typename T > struct VariantCast< T * >
{
	static T * Cast( const XE::VariantData & val )
	{
		if( std::visit( VariantDataIsCanConvert< typename XE::TypeTraits< T >::raw_t >(), val ) )
		{
			return reinterpret_cast<T *>( std::visit( VariantDataGetRawPointer(), const_cast<XE::VariantData &>( val ) ) );
		}

		throw std::bad_variant_access();
	}
};

template< typename T > struct VariantCast< T & >
{
	static T & Cast( const XE::VariantData & val )
	{
		if constexpr( std::is_enum_v< T > )
		{
			auto & data = std::get< XE::VariantEnumData >( const_cast<XE::VariantData &>( val ) );
			if( data.Type == TypeID< T >::Get().get() )
			{
				return *( reinterpret_cast<T *>( &data.Value ) );
			}
		}
		else if( std::visit( VariantDataIsCanConvert< T >(), val ) )
		{
			return *( VariantCast< T * >::Cast( val ) );
		}

		throw std::bad_variant_access();
	}
};

template< typename T > struct VariantCast< XE::SharedPtr< T > >
{
	static XE::SharedPtr< T > Cast( const XE::VariantData & val )
	{
		if( std::visit( VariantDataIsType< VariantWarpperData >(), val ) && std::get< VariantWarpperData >( val ).Pointer->IsSharedPtr() && std::visit( VariantDataIsCanConvert< T >(), val ) )
		{
			return std::reinterpret_pointer_cast<T>( std::get< VariantWarpperData >( val ).Pointer->SharedPointer() );
		}

		throw std::bad_variant_access();
	}
};


template<> struct VariantCreate< XE::VariantData >;
template<> struct VariantCreate< XE::VariantData * >;
template<> struct VariantCreate< XE::VariantData & >;

template<> struct VariantCast< XE::AnsiString & >;
template<> struct VariantCast< XE::WideString & >;
template< typename T > struct VariantCast< XE::SharedPtr< T > * >;
template< typename T > struct VariantCast< XE::SharedPtr< T > & >;

END_XE_NAMESPACE

#endif // VARIANTOPERATION_HPP__341FA70B_D2EA_46D1_98BF_13C605838578
