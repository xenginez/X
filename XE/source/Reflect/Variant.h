/*!
 * \file   Variant.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __VARIANT_H__87E44F7B_D250_41E0_AEEE_7D7ECB2AAF35
#define __VARIANT_H__87E44F7B_D250_41E0_AEEE_7D7ECB2AAF35

#include "MetaType.h"
#include "VariantData.h"

BEG_XE_NAMESPACE

class XE_API Variant
{
public:
	Variant();

	Variant( std::nullptr_t );

	Variant( bool val );

	Variant( XE::int8 val );

	Variant( XE::int16 val );

	Variant( XE::int32 val );

	Variant( XE::int64 val );

	Variant( XE::uint8 val );

	Variant( XE::uint16 val );

	Variant( XE::uint32 val );

	Variant( XE::uint64 val );

	Variant( XE::float32 val );

	Variant( XE::float64 val );

	Variant( const XE::Variant * val );

	Variant( const XE::Variant & val );

	Variant( const XE::VariantData & val );

	Variant( const XE::VariantEnumData & val );

	Variant( const XE::VariantSmallData & val );

	Variant( const XE::VariantPointerData & val );

	Variant( const XE::VariantWarpperData & val );

	Variant( const void * ptr, const XE::MetaType * meta );

	Variant( const void * ptr, const XE::MetaTypeCPtr & meta );

	Variant( const XE::SharedPtr< void > & ptr, const XE::MetaType * meta );

	Variant( const XE::SharedPtr< void > & ptr, const XE::MetaTypeCPtr & meta );

	template< typename T > Variant( const T & val )
		:_Data( XE::VariantCreate< typename XE::TypeTraits< T >::remove_const_volatile_reference_t >::Create( val ) )
	{
	}

	~Variant() = default;

public:
	XE::Variant & operator=( std::nullptr_t );

	XE::Variant & operator=( bool val );

	XE::Variant & operator=( XE::int8 val );

	XE::Variant & operator=( XE::int16 val );

	XE::Variant & operator=( XE::int32 val );

	XE::Variant & operator=( XE::int64 val );

	XE::Variant & operator=( XE::uint8 val );

	XE::Variant & operator=( XE::uint16 val );

	XE::Variant & operator=( XE::uint32 val );

	XE::Variant & operator=( XE::uint64 val );

	XE::Variant & operator=( XE::float32 val );

	XE::Variant & operator=( XE::float64 val );

	XE::Variant & operator=( const XE::Variant * val );

	XE::Variant & operator=( const XE::Variant & val );

	XE::Variant & operator=( const XE::VariantData & val );

	XE::Variant & operator=( const XE::VariantEnumData & val );

	XE::Variant & operator=( const XE::VariantSmallData & val );

	XE::Variant & operator=( const XE::VariantPointerData & val );

	XE::Variant & operator=( const XE::VariantWarpperData & val );

	template< typename T > Variant & operator=( const T & val )
	{
		_Data = XE::VariantCreate< typename XE::TypeTraits< T >::remove_const_volatile_reference_t >::Create( val );

		return *this;
	}

public:
	XE::Variant operator!() const;

	XE::Variant operator~() const;

	XE::Variant operator-() const;

public:
	bool operator < ( const XE::Variant & val ) const;

	bool operator >( const XE::Variant & val ) const;

	bool operator <=( const XE::Variant & val ) const;

	bool operator >=( const XE::Variant & val ) const;

	bool operator ==( const XE::Variant & val ) const;

	bool operator !=( const XE::Variant & val ) const;

	bool operator && ( const XE::Variant & val ) const;

	bool operator || ( const XE::Variant & val ) const;

public:
	XE::Variant operator+( const XE::Variant & val ) const;

	XE::Variant operator-( const XE::Variant & val ) const;

	XE::Variant operator*( const XE::Variant & val ) const;

	XE::Variant operator/( const XE::Variant & val ) const;

	XE::Variant operator%( const XE::Variant & val ) const;

	XE::Variant operator|( const XE::Variant & val ) const;

	XE::Variant operator^( const XE::Variant & val ) const;

	XE::Variant operator&( const XE::Variant & val ) const;

	XE::Variant operator<<( const XE::Variant & val ) const;

	XE::Variant operator>>( const XE::Variant & val ) const;

public:
	bool IsNull() const;

	bool IsEnum() const;

	bool IsInvalid() const;

	bool IsPointer() const;

	bool IsContainer() const;

	bool IsSharedPtr() const;

	bool IsFundamental() const;

	bool IsCanConvert( const XE::MetaClassCPtr & val ) const;

public:
	bool ToBool() const;

	XE::int8 ToInt8() const;

	XE::int16 ToInt16() const;

	XE::int32 ToInt32() const;

	XE::int64 ToInt64() const;

	XE::uint8 ToUInt8() const;

	XE::uint16 ToUInt16() const;

	XE::int32 ToUInt32() const;

	XE::uint64 ToUInt64() const;

	XE::float32 ToFloat32() const;

	XE::float64 ToFloat64() const;

	XE::String ToString() const;

	void * ToPointer() const;

public:
	XE::Array< XE::Variant > ToArray() const;

	bool FromArray( const XE::Array< XE::Variant > & val );

public:
	XE::MetaTypeCPtr GetType() const;

	const XE::VariantData & GetData() const;

public:
	void Clear();

	void Clone( XE::Variant * val ) const;

	void swap( XE::Variant & val );

public:
	template< typename T > T Value() const
	{
		using raw_t = typename XE::TypeTraits< T >::raw_t;

		if constexpr( std::is_same_v< raw_t, XE::Variant > )
		{
			if constexpr( std::is_pointer_v< T > )
			{
				return const_cast< XE::Variant * >( this );
			}
			else
			{
				return *( const_cast< XE::Variant * >( this ) );
			}
		}
		else
		{
			return XE::VariantCast< std::remove_const_t< T > >::Cast( _Data );
		}
	}

private:
	XE::VariantData _Data;
};


using VariantSet = XE::Set< XE::Variant >;
using VariantMap = XE::Map< XE::Variant, XE::Variant >;
using VariantPair = XE::Pair< XE::Variant, XE::Variant >;
using VariantList = XE::List< XE::Variant >;
using VariantDeque = XE::Deque< XE::Variant >;
using VariantStack = XE::Stack< XE::Variant >;
using VariantQueue = XE::Queue< XE::Variant >;
using VariantArray = XE::Array< XE::Variant >;
using VariantMultiSet = XE::MultiSet< XE::Variant >;
using VariantMultiMap = XE::MultiMap< XE::Variant, XE::Variant >;
using VariantUnorderedSet = XE::UnorderedSet< XE::Variant >;
using VariantUnorderedMap = XE::UnorderedMap< XE::Variant, XE::Variant >;
using VariantUnorderedMultiSet = XE::UnorderedMultiSet< XE::Variant >;
using VariantUnorderedMultiMap = XE::UnorderedMultiMap< XE::Variant, XE::Variant >;

template <> struct Hasher< XE::Variant >
{
	XE::uint64 operator()( const XE::Variant & _Keyval ) const noexcept
	{
		return std::visit( XE::VariantDataGetMetaType(), _Keyval.GetData() )->GetHashCode();
	}
};

END_XE_NAMESPACE

#define VARIANT_CONTAINER_DATA( TYPE ) \
template< typename ... T > struct XE::VariantClassData< TYPE< T... > > : public XE::VariantInterfaceData \
{ \
public: \
	using ContainerType = TYPE< T... >; \
public: \
	VariantClassData() = default; \
	VariantClassData( ContainerType && val ) \
	{ \
		std::swap( _Value, val ); \
	} \
	VariantClassData( const ContainerType & val ) \
		:_Value( val ) \
	{ } \
	bool IsContainer() const override \
	{ \
		return true; \
	} \
	bool IsSharedPtr() const override \
	{ \
		return false; \
	} \
	const XE::MetaType * GetMetaType() const override \
	{ \
		return TypeID< ContainerType >::Get().get(); \
	} \
	void * ValuePointer() override \
	{ \
		return &_Value; \
	} \
	XE::SharedPtr< void > SharedPointer() override \
	{ \
		return nullptr; \
	} \
public: \
	XE::Array< XE::Variant > ToArray() const override \
	{ \
		XE::Array< XE::Variant > result; \
		for( const auto & it : _Value ) \
		{ \
			result.push_back( it ); \
		} \
		return result; \
	} \
	bool FromArray( const XE::Array< XE::Variant > & val ) \
	{ \
		_Value.clear(); \
		for (const auto & it : val ) \
		{ \
			std::inserter( _Value, std::end( _Value ) ) = it.Value< typename ContainerType::value_type >(); \
		} \
		return true; \
	} \
	XE::SharedPtr< XE::VariantInterfaceData > Clone() const override \
	{ \
		return XE::MakeShared< XE::VariantClassData< TYPE< T... > > >( _Value ); \
	} \
private: \
	ContainerType _Value; \
}; \
template< typename ... T > struct XE::VariantSharedPtrData< XE::SharedPtr< TYPE< T... > > > : public XE::VariantInterfaceData \
{ \
public: \
	using ContainerType = TYPE< T... >; \
	using SharedPtrType = XE::SharedPtr< ContainerType >; \
public: \
	VariantSharedPtrData() = default; \
	VariantSharedPtrData( const SharedPtrType & val ) : _Value( val ) { } \
public: \
	bool IsContainer() const override { return true; } \
	bool IsSharedPtr() const override  { return true; } \
	const XE::MetaType * GetMetaType() const override { return TypeID< ContainerType >::Get( _Value.get() ).get(); } \
public: \
	void * ValuePointer() override { return _Value.get(); } \
	XE::SharedPtr< void > SharedPointer() override { return _Value; } \
public: \
	XE::Array< XE::Variant > ToArray() const override \
	{ \
		XE::Array< XE::Variant > result; \
		if ( _Value != nullptr ) \
		{ \
			for ( const auto & it : *_Value ) \
			{ \
				result.push_back( it ); \
			} \
		} \
		return result; \
	} \
	bool FromArray( const XE::Array< XE::Variant > & val ) \
	{ \
		_Value.clear(); \
		for (const auto & it : val ) \
		{ \
			std::inserter( *_Value, std::end( *_Value ) ) = it.Value< typename ContainerType::value_type >(); \
		} \
		return true; \
	} \
	XE::SharedPtr< XE::VariantInterfaceData > Clone() const override \
	{ \
		return XE::MakeShared< XE::VariantSharedPtrData< XE::SharedPtr< TYPE< T... > > > >( _Value ); \
	} \
private: \
	SharedPtrType _Value; \
};

VARIANT_CONTAINER_DATA( std::set );
VARIANT_CONTAINER_DATA( std::map );
VARIANT_CONTAINER_DATA( std::list );
VARIANT_CONTAINER_DATA( std::deque );
VARIANT_CONTAINER_DATA( std::vector );
VARIANT_CONTAINER_DATA( std::multiset );
VARIANT_CONTAINER_DATA( std::multimap );
VARIANT_CONTAINER_DATA( std::unordered_set );
VARIANT_CONTAINER_DATA( std::unordered_map );
VARIANT_CONTAINER_DATA( std::unordered_multiset );
VARIANT_CONTAINER_DATA( std::unordered_multimap );

template< typename K, typename V > struct XE::VariantClassData< std::pair< K, V > > : public XE::VariantInterfaceData
{
public:
	using ContainerType = std::pair< std::remove_const_t< K >, V >;
public:
	VariantClassData() = default;
	VariantClassData( ContainerType && val )
	{
		std::swap( _Value, val );
	}
	VariantClassData( const ContainerType & val )
		:_Value( val )
	{
	}
	bool IsContainer() const override
	{
		return true;
	}
	bool IsSharedPtr() const override
	{
		return false;
	}
	const XE::MetaType * GetMetaType() const override
	{
		return TypeID< ContainerType >::Get().get();
	}
	void * ValuePointer() override
	{
		return &_Value;
	}
	XE::SharedPtr< void > SharedPointer() override
	{
		return nullptr;
	}
public:
	XE::Array< XE::Variant > ToArray() const override
	{
		XE::VariantArray result;
		result.push_back( _Value.first );
		result.push_back( _Value.second );
		return result;
	}
	bool FromArray( const XE::Array< XE::Variant > & val )
	{
		if ( val.size() == 2 )
		{
			_Value.first = val[0].Value< ContainerType::first_type >();
			_Value.second = val[1].Value< ContainerType::second_type >();

			return true;
		}

		return false;
	}
	XE::SharedPtr< XE::VariantInterfaceData > Clone() const override
	{
		return XE::MakeShared< XE::VariantClassData< ContainerType > >( _Value );
	}
private:
	ContainerType _Value;
};
template< typename K, typename V > struct XE::VariantSharedPtrData< XE::SharedPtr< std::pair<  K, V > > > : public XE::VariantInterfaceData
{
public:
	using ContainerType = std::pair< K, V >;
	using SharedPtrType = XE::SharedPtr< ContainerType >;
public:
	VariantSharedPtrData() = default;
	VariantSharedPtrData( const SharedPtrType & val ) : _Value( val ) {}
public:
	bool IsContainer() const override { return true; }
	bool IsSharedPtr() const override { return true; }
	const XE::MetaType * GetMetaType() const override { return TypeID< ContainerType >::Get( _Value.get() ).get(); }
public:
	void * ValuePointer() override { return _Value.get(); }
	XE::SharedPtr< void > SharedPointer() override { return _Value; }
public:
	XE::Array< XE::Variant > ToArray() const override
	{
		XE::VariantArray result;
		if ( _Value != nullptr )
		{
			result.push_back( _Value->first );
			result.push_back( _Value->second );
		}
		return result;
	}
	bool FromArray( const XE::Array< XE::Variant > & val )
	{
		if ( val.size() == 2 )
		{
			_Value->first = val[0].Value< ContainerType::first_type >();
			_Value->second = val[1].Value< ContainerType::second_type >();

			return true;
		}

		return false;
	}
	XE::SharedPtr< XE::VariantInterfaceData > Clone() const override
	{
		return XE::MakeShared< XE::VariantSharedPtrData< SharedPtrType > >( _Value );
	}
private:
	SharedPtrType _Value;
};

#endif // __VARIANT_H__87E44F7B_D250_41E0_AEEE_7D7ECB2AAF35