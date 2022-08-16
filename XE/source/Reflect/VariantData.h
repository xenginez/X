/*!
 * \file	VariantData.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/09
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef VARIANTDATA_H__34B101A5_C0A5_400F_BA21_E293E1EE2C91
#define VARIANTDATA_H__34B101A5_C0A5_400F_BA21_E293E1EE2C91

#include <variant>
#include <functional>

#include "Base/NonCopyable.hpp"

#include "TypeID.hpp"
#include "MetaType.h"

BEG_XE_NAMESPACE

XE_API bool __VariantDataIsCanConvert( const XE::MetaInfo * type1, const XE::MetaInfo * type2 );

struct XE_API VariantEnumData
{
	VariantEnumData() = default;

	VariantEnumData( VariantEnumData && ) = default;

	VariantEnumData( const VariantEnumData & ) = default;

	VariantEnumData & operator = ( VariantEnumData && ) = default;

	VariantEnumData & operator = ( const VariantEnumData & ) = default;

	VariantEnumData( XE::uint64 val, const XE::MetaType * type )
		:Value( val ), Type( type )
	{

	}

	template< typename T > VariantEnumData( T val )
		:Value( XE::BitCast< XE::uint64 >( val ) ), Type( TypeID< T >::Get().get() )
	{

	}


	bool operator < ( const VariantEnumData & val ) const
	{
		return Type == val.Type && Value < val.Value;
	}

	bool operator >( const VariantEnumData & val ) const
	{
		return Type == val.Type && Value > val.Value;
	}

	bool operator <=( const VariantEnumData & val ) const
	{
		return Type == val.Type && Value <= val.Value;
	}

	bool operator >=( const VariantEnumData & val ) const
	{
		return Type == val.Type && Value >= val.Value;
	}

	bool operator ==( const VariantEnumData & val ) const
	{
		return Type == val.Type && Value == val.Value;
	}

	bool operator !=( const VariantEnumData & val ) const
	{
		return Type != val.Type || Value != val.Value;
	}


	XE::uint64 Value = 0;
	const XE::MetaType * Type = nullptr;
};

struct XE_API VariantSmallData
{
	VariantSmallData() = default;

	VariantSmallData( VariantSmallData && ) = default;

	VariantSmallData( const VariantSmallData & ) = default;

	VariantSmallData & operator = ( VariantSmallData && ) = default;

	VariantSmallData & operator = ( const VariantSmallData & ) = default;

	template< typename T > VariantSmallData( T val )
		:Value( XE::BitCast< XE::uint64 >( val ) ), Type( TypeID< T >::Get().get() )
	{

	}

	bool operator < ( const VariantSmallData & val ) const
	{
		return Type == val.Type && Value < val.Value;
	}

	bool operator >( const VariantSmallData & val ) const
	{
		return Type == val.Type && Value > val.Value;
	}

	bool operator <=( const VariantSmallData & val ) const
	{
		return Type == val.Type && Value <= val.Value;
	}

	bool operator >=( const VariantSmallData & val ) const
	{
		return Type == val.Type && Value >= val.Value;
	}

	bool operator ==( const VariantSmallData & val ) const
	{
		return Type == val.Type && Value == val.Value;
	}

	bool operator !=( const VariantSmallData & val ) const
	{
		return Type != val.Type || Value != val.Value;
	}

	XE::uint64 Value = 0;
	const XE::MetaType * Type = nullptr;
};

struct XE_API VariantPointerData
{
	VariantPointerData();

	VariantPointerData( VariantPointerData && ) = default;

	VariantPointerData( const VariantPointerData & ) = default;

	VariantPointerData & operator = ( VariantPointerData && ) = default;

	VariantPointerData & operator = ( const VariantPointerData & ) = default;

	template< typename T > VariantPointerData( const T * val )
		:Value( static_cast< void * >( const_cast< T * >( val ) ) ) , Type( TypeID< T >::Get( val ).get() )
	{

	}

	VariantPointerData( const void * val, const XE::MetaType * type )
		:Value( const_cast< void * >( val ) ), Type( type )
	{

	}

	bool operator < ( const VariantPointerData & val ) const
	{
		return Type == val.Type && Value < val.Value;
	}

	bool operator >( const VariantPointerData & val ) const
	{
		return Type == val.Type && Value > val.Value;
	}

	bool operator <=( const VariantPointerData & val ) const
	{
		return Type == val.Type && Value <= val.Value;
	}

	bool operator >=( const VariantPointerData & val ) const
	{
		return Type == val.Type && Value >= val.Value;
	}

	bool operator ==( const VariantPointerData & val ) const
	{
		return Type == val.Type && Value == val.Value;
	}

	bool operator !=( const VariantPointerData & val ) const
	{
		return Type != val.Type || Value != val.Value;
	}

	void * Value = nullptr;
	const XE::MetaType * Type = nullptr;
};

struct XE_API VariantInterfaceData : public XE::NonCopyable
{
public:
	VariantInterfaceData() = default;

	virtual ~VariantInterfaceData() = default;

public:
	virtual bool IsContainer() const = 0;

	virtual bool IsSharedPtr() const = 0;

	virtual const XE::MetaType * GetMetaType() const = 0;

public:
	virtual void * ValuePointer() = 0;

	virtual XE::SharedPtr< void > SharedPointer() = 0;

public:
	virtual XE::Array< XE::Variant > ToArray() const = 0;

	virtual bool FromArray( const XE::Array< XE::Variant > & val ) = 0;
};

template< typename T > struct VariantClassData : public VariantInterfaceData
{
public:
	VariantClassData() = default;

	VariantClassData( const T & val )
		:_Value( val )
	{
	}

public:
	bool IsContainer() const override
	{
		return std::is_container_v< T >;
	}

	bool IsSharedPtr() const override
	{
		return std::is_shared_ptr_v< T >;
	}

	const XE::MetaType * GetMetaType() const override
	{
		return TypeID< T >::Get( &_Value ).get();
	}

public:
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
		return {};
	}

	bool FromArray( const XE::Array< XE::Variant > & val ) override
	{
		return false;
	}

private:
	T _Value;
};

template< typename T > struct VariantSharedPtrData : public VariantInterfaceData
{
public:
	VariantSharedPtrData() = default;

	VariantSharedPtrData( const XE::SharedPtr< T > & val )
		:_Value( val )
	{

	}

public:
	bool IsContainer() const override
	{
		return std::is_container_v< T >;
	}

	bool IsSharedPtr() const override
	{
		return true;
	}

	const XE::MetaType * GetMetaType() const override
	{
		return TypeID< T >::Get( _Value.get() ).get();
	}

public:
	void * ValuePointer() override
	{
		return _Value.get();
	}

	XE::SharedPtr< void > SharedPointer() override
	{
		return _Value;
	}

public:
	XE::Array< XE::Variant > ToArray() const override
	{
		return {};
	}

	bool FromArray( const XE::Array< XE::Variant > & val ) override
	{
		return false;
	}

private:
	XE::SharedPtr< T > _Value;
};

struct VariantVoidSharedPtrData : public VariantInterfaceData
{
public:
	VariantVoidSharedPtrData() = default;

	VariantVoidSharedPtrData( const XE::SharedPtr< void > & val, const XE::MetaType * type )
		: _Value( val ), _Type( type )
	{

	}

public:
	bool IsContainer() const override
	{
		return false;
	}

	bool IsSharedPtr() const override
	{
		return true;
	}

	const XE::MetaType * GetMetaType() const override
	{
		return _Type;
	}

public:
	void * ValuePointer() override
	{
		return _Value.get();
	}

	XE::SharedPtr< void > SharedPointer() override
	{
		return _Value;
	}

public:
	XE::Array< XE::Variant > ToArray() override
	{
		return {};
	}

private:
	const XE::MetaType * _Type;
	XE::SharedPtr< void > _Value;
};

struct XE_API VariantWarpperData
{
public:
	VariantWarpperData() = default;

	VariantWarpperData( VariantWarpperData && val ) = default;

	VariantWarpperData( const VariantWarpperData & val ) = default;

	VariantWarpperData & operator = ( VariantWarpperData && val ) = default;

	VariantWarpperData & operator = ( const VariantWarpperData & val ) = default;

	template< typename T > VariantWarpperData( const T & val )
		: Pointer( XE::MakeShared< VariantClassData< T > >( val ) )
	{

	}

	template< typename T > VariantWarpperData( const XE::SharedPtr< T > & val )
		: Pointer( XE::MakeShared< VariantSharedPtrData< T > >( val ) )
	{

	}

	VariantWarpperData( const XE::SharedPtr< void > & val, const XE::MetaType * type )
		: Pointer( XE::MakeShared< VariantVoidSharedPtrData >( val, type ) )
	{

	}

	~VariantWarpperData() = default;

public:
	bool operator < ( const VariantWarpperData & val ) const
	{
		return Pointer < val.Pointer;
	}

	bool operator >( const VariantWarpperData & val ) const
	{
		return Pointer > val.Pointer;
	}

	bool operator <=( const VariantWarpperData & val ) const
	{
		return Pointer <= val.Pointer;
	}

	bool operator >=( const VariantWarpperData & val ) const
	{
		return Pointer >= val.Pointer;
	}

	bool operator ==( const VariantWarpperData & val ) const
	{
		return Pointer == val.Pointer;
	}

	bool operator !=( const VariantWarpperData & val ) const
	{
		return Pointer != val.Pointer;
	}

public:
	XE::SharedPtr< VariantInterfaceData > Pointer = nullptr;
};

using VariantData = std::variant< std::monostate, bool, XE::int8, XE::int16, XE::int32, XE::int64, XE::uint8, XE::uint16, XE::uint32, XE::uint64, XE::float32, XE::float64, VariantEnumData, VariantSmallData, VariantPointerData, VariantWarpperData >;

struct XE_API VariantDataIsNull
{
	bool operator()( const std::monostate & ) const;
	bool operator()( const bool & ) const;
	bool operator()( const XE::int8 & ) const;
	bool operator()( const XE::int16 & ) const;
	bool operator()( const XE::int32 & ) const;
	bool operator()( const XE::int64 & ) const;
	bool operator()( const XE::uint8 & ) const;
	bool operator()( const XE::uint16 & ) const;
	bool operator()( const XE::uint32 & ) const;
	bool operator()( const XE::uint64 & ) const;
	bool operator()( const XE::float32 & ) const;
	bool operator()( const XE::float64 & ) const;
	bool operator()( const XE::VariantEnumData & ) const;
	bool operator()( const XE::VariantSmallData & ) const;
	bool operator()( const XE::VariantPointerData & val ) const;
	bool operator()( const XE::VariantWarpperData & val ) const;
};

struct XE_API VariantDataGetMetaType
{
	XE::MetaTypeCPtr operator()( const std::monostate & ) const;
	XE::MetaTypeCPtr operator()( const bool & ) const;
	XE::MetaTypeCPtr operator()( const XE::int8 & ) const;
	XE::MetaTypeCPtr operator()( const XE::int16 & ) const;
	XE::MetaTypeCPtr operator()( const XE::int32 & ) const;
	XE::MetaTypeCPtr operator()( const XE::int64 & ) const;
	XE::MetaTypeCPtr operator()( const XE::uint8 & ) const;
	XE::MetaTypeCPtr operator()( const XE::uint16 & ) const;
	XE::MetaTypeCPtr operator()( const XE::uint32 & ) const;
	XE::MetaTypeCPtr operator()( const XE::uint64 & ) const;
	XE::MetaTypeCPtr operator()( const XE::float32 & ) const;
	XE::MetaTypeCPtr operator()( const XE::float64 & ) const;
	XE::MetaTypeCPtr operator()( const XE::VariantEnumData & val ) const;
	XE::MetaTypeCPtr operator()( const XE::VariantSmallData & val ) const;
	XE::MetaTypeCPtr operator()( const XE::VariantPointerData & val ) const;
	XE::MetaTypeCPtr operator()( const XE::VariantWarpperData & val ) const;
};

struct XE_API VariantDataIsContainer
{
	bool operator()( const std::monostate & ) const;
	bool operator()( const bool & ) const;
	bool operator()( const XE::int8 & ) const;
	bool operator()( const XE::int16 & ) const;
	bool operator()( const XE::int32 & ) const;
	bool operator()( const XE::int64 & ) const;
	bool operator()( const XE::uint8 & ) const;
	bool operator()( const XE::uint16 & ) const;
	bool operator()( const XE::uint32 & ) const;
	bool operator()( const XE::uint64 & ) const;
	bool operator()( const XE::float32 & ) const;
	bool operator()( const XE::float64 & ) const;
	bool operator()( const XE::VariantEnumData & ) const;
	bool operator()( const XE::VariantSmallData & ) const;
	bool operator()( const XE::VariantPointerData & ) const;
	bool operator()( const XE::VariantWarpperData & val ) const;
};

struct XE_API VariantDataGetRawPointer
{
	void * operator()( std::monostate & ) const;
	void * operator()( bool & val ) const;
	void * operator()( XE::int8 & val ) const;
	void * operator()( XE::int16 & val ) const;
	void * operator()( XE::int32 & val ) const;
	void * operator()( XE::int64 & val ) const;
	void * operator()( XE::uint8 & val ) const;
	void * operator()( XE::uint16 & val ) const;
	void * operator()( XE::uint32 & val ) const;
	void * operator()( XE::uint64 & val ) const;
	void * operator()( XE::float32 & val ) const;
	void * operator()( XE::float64 & val ) const;
	void * operator()( XE::VariantEnumData & val ) const;
	void * operator()( XE::VariantSmallData & val ) const;
	void * operator()( XE::VariantPointerData & val ) const;
	void * operator()( XE::VariantWarpperData & val ) const;
};

struct XE_API VariantDataToArray
{
	XE::Array< XE::Variant > operator()( const std::monostate & ) const;
	XE::Array< XE::Variant > operator()( const bool & val ) const;
	XE::Array< XE::Variant > operator()( const XE::int8 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::int16 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::int32 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::int64 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::uint8 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::uint16 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::uint32 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::uint64 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::float32 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::float64 & val ) const;
	XE::Array< XE::Variant > operator()( const XE::VariantEnumData & val ) const;
	XE::Array< XE::Variant > operator()( const XE::VariantSmallData & val ) const;
	XE::Array< XE::Variant > operator()( const XE::VariantPointerData & val ) const;
	XE::Array< XE::Variant > operator()( const XE::VariantWarpperData & val ) const;
};

struct XE_API VariantDataFromArray
{
	VariantDataFromArray( const XE::Array< XE::Variant > & val );

	bool operator()( const std::monostate & ) const;
	bool operator()( const bool & val ) const;
	bool operator()( const XE::int8 & val ) const;
	bool operator()( const XE::int16 & val ) const;
	bool operator()( const XE::int32 & val ) const;
	bool operator()( const XE::int64 & val ) const;
	bool operator()( const XE::uint8 & val ) const;
	bool operator()( const XE::uint16 & val ) const;
	bool operator()( const XE::uint32 & val ) const;
	bool operator()( const XE::uint64 & val ) const;
	bool operator()( const XE::float32 & val ) const;
	bool operator()( const XE::float64 & val ) const;
	bool operator()( const XE::VariantEnumData & val ) const;
	bool operator()( const XE::VariantSmallData & val ) const;
	bool operator()( const XE::VariantPointerData & val ) const;
	bool operator()( const XE::VariantWarpperData & val ) const;

	const XE::Array< XE::Variant > & Array;
};


template< typename ... T > struct VariantDataIsType
{
	template< typename Uy > static bool constexpr IsSameType()
	{
		return false;
	}

	template< typename Uy, typename Ty, typename ... ARGS > static bool constexpr IsSameType()
	{
		if( std::is_same_v< Uy, Ty > == true )
		{
			return true;
		}

		return IsSameType< Uy, ARGS... >();
	}

	template< typename U > bool operator()( const U & ) const
	{
		return IsSameType< U, T... >();
	}
};
using VariantDataIsSignedType = VariantDataIsType< XE::int8, XE::int16, XE::int32, XE::int64 >;
using VariantDataIsUnsignedType = VariantDataIsType< XE::uint8, XE::uint16, XE::uint32, XE::uint64 >;
using VariantDataIsIntegerType = VariantDataIsType< XE::int8, XE::int16, XE::int32, XE::int64, XE::uint8, XE::uint16, XE::uint32, XE::uint64 >;
using VariantDataIsFloatingType = VariantDataIsType< XE::float32, XE::float64 >;

template< typename T > struct VariantDataIsCanConvert
{
	bool operator()( const std::monostate & ) const
	{
		return false;
	}
	bool operator()( const bool & ) const
	{
		return TypeID< T >::Get() == TypeID< bool >::Get();
	}
	bool operator()( const XE::int8 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::int8 >::Get();
	}
	bool operator()( const XE::int16 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::int16 >::Get();
	}
	bool operator()( const XE::int32 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::int32 >::Get();
	}
	bool operator()( const XE::int64 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::int64 >::Get();
	}
	bool operator()( const XE::uint8 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::uint8 >::Get();
	}
	bool operator()( const XE::uint16 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::uint16 >::Get();
	}
	bool operator()( const XE::uint32 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::uint32 >::Get();
	}
	bool operator()( const XE::uint64 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::uint64 >::Get();
	}
	bool operator()( const XE::float32 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::float32 >::Get();
	}
	bool operator()( const XE::float64 & ) const
	{
		return TypeID< T >::Get() == TypeID< XE::float64 >::Get();
	}
	bool operator()( const VariantEnumData & val ) const
	{
		return TypeID< T >::Get().get() == val.Type;
	}
	bool operator()( const VariantSmallData & val ) const
	{
		return val.Type != nullptr && __VariantDataIsCanConvert( TypeID< T >::Get().get(), val.Type );
	}
	bool operator()( const VariantPointerData & val ) const
	{
		return val.Type != nullptr && __VariantDataIsCanConvert( TypeID< T >::Get().get(), val.Type );
	}
	bool operator()( const VariantWarpperData & val ) const
	{
		return val.Pointer != nullptr && __VariantDataIsCanConvert( TypeID< T >::Get().get(), val.Pointer->GetMetaType() );
	}
};

template< typename T > struct VariantDataGetFundamental
{
	T operator()( const std::monostate & ) const
	{
		throw std::bad_variant_access();
	}
	T operator()( const bool & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::int8 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::int16 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::int32 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::int64 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::uint8 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::uint16 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::uint32 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::uint64 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::float32 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::float64 & val ) const
	{
		return static_cast< T >( val );
	}
	T operator()( const XE::VariantEnumData & val ) const
	{
		return val.Value;
	}
	T operator()( const XE::VariantSmallData & val ) const
	{
		throw std::bad_variant_access();
	}
	T operator()( const XE::VariantPointerData & val ) const
	{
		throw std::bad_variant_access();
	}
	T operator()( const XE::VariantWarpperData & val ) const
	{
		throw std::bad_variant_access();
	}
};

template< typename ... T > struct VariantDataWarpperIsType
{
	template< typename U > static bool IsSameType( XE::MetaType * type )
	{
		return type == TypeID< U >::Get().get();
	}

	template< typename U, typename ... ARGS > static bool IsSameType( XE::MetaType * type )
	{
		if( IsSameType< U >( type ) )
		{
			return true;
		}

		return IsSameType< ARGS... >( type );
	}

	bool operator()( const std::monostate & ) const
	{
		return false;
	}
	bool operator()( const bool & ) const
	{
		return false;
	}
	bool operator()( const XE::int8 & ) const
	{
		return false;
	}
	bool operator()( const XE::int16 & ) const
	{
		return false;
	}
	bool operator()( const XE::int32 & ) const
	{
		return false;
	}
	bool operator()( const XE::int64 & ) const
	{
		return false;
	}
	bool operator()( const XE::uint8 & ) const
	{
		return false;
	}
	bool operator()( const XE::uint16 & ) const
	{
		return false;
	}
	bool operator()( const XE::uint32 & ) const
	{
		return false;
	}
	bool operator()( const XE::uint64 & ) const
	{
		return false;
	}
	bool operator()( const XE::float32 & ) const
	{
		return false;
	}
	bool operator()( const XE::float64 & ) const
	{
		return false;
	}
	bool operator()( const VariantEnumData & val ) const
	{
		return false;
	}
	bool operator()( const VariantSmallData & val ) const
	{
		return false;
	}
	bool operator()( const VariantPointerData & val ) const
	{
		return false;
	}
	bool operator()( const VariantWarpperData & val ) const
	{
		return val.Pointer != nullptr ? IsSameType< T... >( val.Pointer->GetMetaType() ) : false;
	}
};

END_XE_NAMESPACE

namespace std
{
	XE_INLINE void swap( XE::VariantEnumData & left, XE::VariantEnumData & right )
	{
		std::swap( left.Type, right.Type );
		std::swap( left.Value, right.Value );
	}
	XE_INLINE void swap( XE::VariantSmallData & left, XE::VariantSmallData & right )
	{
		std::swap( left.Type, right.Type );
		std::swap( left.Value, right.Value );
	}
	XE_INLINE void swap( XE::VariantPointerData & left, XE::VariantPointerData & right )
	{
		std::swap( left.Type, right.Type );
		std::swap( left.Value, right.Value );
	}
	XE_INLINE void swap( XE::VariantWarpperData & left, XE::VariantWarpperData & right )
	{
		std::swap( left.Pointer, right.Pointer );
	}
}

#endif // VARIANTDATA_H__34B101A5_C0A5_400F_BA21_E293E1EE2C91
