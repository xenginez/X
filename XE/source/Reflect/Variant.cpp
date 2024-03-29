#include "Variant.h"

#include "CXXMetaClass.hpp"

XE::Variant::Variant()
	:_Data( std::monostate() )
{

}

XE::Variant::Variant( std::nullptr_t )
	: _Data( XE::VariantPointerData() )
{

}

XE::Variant::Variant( bool val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::int8 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::int16 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::int32 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::int64 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::uint8 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::uint16 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::uint32 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::uint64 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::float32 val )
	: _Data( val )
{

}

XE::Variant::Variant( XE::float64 val )
	: _Data( val )
{

}

XE::Variant::Variant( const XE::Variant * val )
	: _Data( val->_Data )
{

}

XE::Variant::Variant( const XE::Variant & val )
	: _Data( val._Data )
{

}

XE::Variant::Variant( const XE::VariantData & val )
	: _Data( val )
{

}

XE::Variant::Variant( const XE::VariantEnumData & val )
	: _Data( val )
{

}

XE::Variant::Variant( const XE::VariantSmallData & val )
	: _Data( val )
{

}

XE::Variant::Variant( const XE::VariantPointerData & val )
	: _Data( val )
{

}

XE::Variant::Variant( const XE::VariantWarpperData & val )
	: _Data( val )
{

}

XE::Variant::Variant( const void * ptr, const XE::MetaType * meta )
	: _Data( XE::VariantPointerData( ptr, meta ) )
{

}

XE::Variant::Variant( const void * ptr, const XE::MetaTypeCPtr & meta )
	: _Data( XE::VariantPointerData( ptr, meta.get() ) )
{

}

XE::Variant::Variant( const XE::SharedPtr< void > & ptr, const XE::MetaType * meta )
	: _Data( XE::VariantWarpperData( ptr, meta ) )
{

}

XE::Variant::Variant( const XE::SharedPtr< void > & ptr, const XE::MetaTypeCPtr & meta )
	: _Data( XE::VariantWarpperData( ptr, meta.get() ) )
{

}

XE::Variant & XE::Variant::operator=( std::nullptr_t )
{
	_Data = XE::VariantPointerData();

	return *this;
}

XE::Variant & XE::Variant::operator=( bool val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::int8 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::int16 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::int32 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::int64 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::uint8 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::uint16 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::uint32 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::uint64 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::float32 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( XE::float64 val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::Variant & val )
{
	_Data = val._Data;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::Variant * val )
{
	_Data = val->_Data;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::VariantData & val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::VariantEnumData & val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::VariantSmallData & val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::VariantPointerData & val )
{
	_Data = val;

	return *this;
}

XE::Variant & XE::Variant::operator=( const XE::VariantWarpperData & val )
{
	_Data = val;

	return *this;
}

XE::Variant XE::Variant::operator!() const
{
	return std::visit( XE::Overloaded{
						[this]( const std::monostate & val ) -> XE::Variant
 {
throw * this;
},
		[this]( const bool & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::int8 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::int16 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::int32 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::int64 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::uint8 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::uint16 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::uint32 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::uint64 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::float32 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const XE::float64 & val ) -> XE::Variant
{
return !val;
},
		 [this]( const VariantEnumData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantSmallData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantPointerData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantWarpperData & val ) -> XE::Variant
{
throw * this;
},
					   }, _Data );
}

XE::Variant XE::Variant::operator~() const
{
	return std::visit( XE::Overloaded{
						[this]( const std::monostate & val ) -> XE::Variant
 {
throw * this;
},
		[this]( const bool & val ) -> XE::Variant
{
throw * this;
},
		[this]( const XE::int8 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::int16 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::int32 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::int64 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::uint8 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::uint16 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::uint32 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::uint64 & val ) -> XE::Variant
{
return ~val;
},
		 [this]( const XE::float32 & val ) -> XE::Variant
{
throw * this;
},
		[this]( const XE::float64 & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantEnumData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantSmallData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantPointerData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantWarpperData & val ) -> XE::Variant
{
throw * this;
},
					   }, _Data );
}

XE::Variant XE::Variant::operator-() const
{
	return std::visit( XE::Overloaded{
						[this]( const std::monostate & val ) -> XE::Variant
 {
throw * this;
},
		[this]( const bool & val ) -> XE::Variant
{
throw * this;
},
		[this]( const XE::int8 & val ) -> XE::Variant
{
return -val;
},
		 [this]( const XE::int16 & val ) -> XE::Variant
{
return -val;
},
		 [this]( const XE::int32 & val ) -> XE::Variant
{
return -val;
},
		 [this]( const XE::int64 & val ) -> XE::Variant
{
return -val;
},
		 [this]( const XE::uint8 & val ) -> XE::Variant
{
return -(XE::int8) val;
},
[this]( const XE::uint16 & val ) -> XE::Variant
{
return -(XE::int16) val;
},
[this]( const XE::uint32 & val ) -> XE::Variant
{
return -(XE::int32) val;
},
[this]( const XE::uint64 & val ) -> XE::Variant
{
return -(XE::int64) val;
},
[this]( const XE::float32 & val ) -> XE::Variant
{
return -val;
},
		 [this]( const XE::float64 & val ) -> XE::Variant
{
return -val;
},
		 [this]( const VariantEnumData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantSmallData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantPointerData & val ) -> XE::Variant
{
throw * this;
},
		[this]( const VariantWarpperData & val ) -> XE::Variant
{
throw * this;
},
					   }, _Data );
}

bool XE::Variant::operator< ( const XE::Variant & val ) const
{
	return _Data < val._Data;
}

bool XE::Variant::operator >( const XE::Variant & val ) const
{
	return _Data > val._Data;
}

bool XE::Variant::operator<=( const XE::Variant & val ) const
{
	return _Data <= val._Data;
}

bool XE::Variant::operator>=( const XE::Variant & val ) const
{
	return _Data >= val._Data;
}

bool XE::Variant::operator==( const XE::Variant & val ) const
{
	return _Data == val._Data;
}

bool XE::Variant::operator!=( const XE::Variant & val ) const
{
	return _Data != val._Data;
}

bool XE::Variant::operator&&( const XE::Variant & val ) const
{
	return !( val.IsInvalid() );
}

bool XE::Variant::operator||( const XE::Variant & val ) const
{
	return !( val.IsInvalid() );
}

XE::Variant XE::Variant::operator+( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_fundamental_v< decltype( left )> && std::is_fundamental_v< decltype( left )> )
							{
								return left + right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator-( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_fundamental_v< decltype( left )> && std::is_fundamental_v< decltype( left )> )
							{
								return left - right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator*( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_fundamental_v< decltype( left )> && std::is_fundamental_v< decltype( left )> )
							{
								return left * right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator/( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_fundamental_v< decltype( left )> && std::is_fundamental_v< decltype( left )> )
							{
								return left / right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator%( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_integral_v< decltype( left )> && std::is_integral_v< decltype( left )> )
							{
								return left % right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator|( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_integral_v< decltype( left )> && std::is_integral_v< decltype( left )> )
							{
								return left | right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator^( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_integral_v< decltype( left )> && std::is_integral_v< decltype( left )> )
							{
								return left ^ right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator&( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_integral_v< decltype( left )> && std::is_integral_v< decltype( left )> )
							{
								return left & right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator<<( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_integral_v< decltype( left )> && std::is_integral_v< decltype( left )> )
							{
								return left << right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

XE::Variant XE::Variant::operator>>( const XE::Variant & val ) const
{
	return std::visit( XE::Overloaded{
						[&]( const auto & left, const auto & right ) -> XE::Variant
						{
							if constexpr( std::is_integral_v< decltype( left )> && std::is_integral_v< decltype( left )> )
							{
								return left >> right;
							}
							else
							{
								throw * this;
							}
						}
					   }, _Data, val._Data );
}

bool XE::Variant::IsNull() const
{
	return std::visit( XE::VariantDataIsNull(), _Data );
}

bool XE::Variant::IsEnum() const
{
	return std::visit( XE::VariantDataIsType< XE::VariantEnumData >(), _Data );
}

bool XE::Variant::IsInvalid() const
{
	return std::visit( XE::VariantDataIsType< std::monostate >(), _Data );
}

bool XE::Variant::IsPointer() const
{
	return std::visit( XE::VariantDataIsType< XE::VariantPointerData >(), _Data );
}

bool XE::Variant::IsContainer() const
{
	return std::visit( XE::VariantDataIsContainer(), _Data );
}

bool XE::Variant::IsSharedPtr() const
{
	return std::visit( XE::VariantDataIsType< XE::VariantWarpperData >(), _Data ) && std::get< XE::VariantWarpperData >( _Data ).Pointer->IsSharedPtr();
}

bool XE::Variant::IsFundamental() const
{
	return std::visit(
		VariantDataIsType< bool, XE::int8, XE::int16, XE::int32, XE::int64, XE::uint8, XE::uint16, XE::uint32, XE::uint64, XE::float32, XE::float64 >(),
		_Data );
}

bool XE::Variant::IsCanConvert( const XE::MetaClassCPtr & val ) const
{
	auto type = std::visit( XE::VariantDataGetMetaType(), _Data );
	if( type && type->GetType() == XE::MetaInfoType::CLASS )
	{
		return SP_CAST< const XE::MetaClass >( type )->CanConvert( val );
	}

	return false;
}

bool XE::Variant::ToBool() const
{
	return std::visit( XE::VariantDataGetFundamental< bool >(), _Data );
}

XE::int8 XE::Variant::ToInt8() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::int8 >(), _Data );
}

XE::int16 XE::Variant::ToInt16() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::int16 >(), _Data );
}

XE::int32 XE::Variant::ToInt32() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::int32 >(), _Data );
}

XE::int64 XE::Variant::ToInt64() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::int64 >(), _Data );
}

XE::uint8 XE::Variant::ToUInt8() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::uint8 >(), _Data );
}

XE::uint16 XE::Variant::ToUInt16() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::uint16 >(), _Data );
}

XE::int32 XE::Variant::ToUInt32() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::uint32 >(), _Data );
}

XE::uint64 XE::Variant::ToUInt64() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::uint64 >(), _Data );
}

XE::float32 XE::Variant::ToFloat32() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::float32 >(), _Data );
}

XE::float64 XE::Variant::ToFloat64() const
{
	return std::visit( XE::VariantDataGetFundamental< XE::float64 >(), _Data );
}

XE::String XE::Variant::ToString() const
{
	return Value< XE::String >();
}

void * XE::Variant::ToPointer() const
{
	return std::visit( XE::VariantDataGetRawPointer(), const_cast<XE::VariantData &>( _Data ) );
}

XE::Array< XE::Variant > XE::Variant::ToArray() const
{
	return std::visit( XE::VariantDataToArray(), _Data );
}

bool XE::Variant::FromArray( const XE::Array< XE::Variant > & val )
{
	return std::visit( XE::VariantDataFromArray( val ), _Data );
}

XE::MetaTypeCPtr XE::Variant::GetType() const
{
	return std::visit( XE::VariantDataGetMetaType(), _Data );
}

const XE::VariantData & XE::Variant::GetData() const
{
	return _Data;
}

void XE::Variant::Clear()
{
	_Data = std::monostate();
}

void XE::Variant::Clone( XE::Variant * val ) const
{
	val->_Data = std::visit( XE::VariantDataCloneVariantData(), _Data );
}

void XE::Variant::swap( XE::Variant & val )
{
	auto tmp = _Data;
	_Data = val._Data;
	val._Data = tmp;
}
