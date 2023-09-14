#include "VariantData.h"

#include "Variant.h"
#include "CXXMetaEnum.hpp"
#include "CXXMetaClass.hpp"

bool XE::__VariantDataIsCanConvert( const XE::MetaInfo * type1, const XE::MetaInfo * type2 )
{
	if( type1 == type2 )
	{
		return true;
	}

	if( type1->GetType() == XE::MetaInfoType::CLASS && type2->GetType() == XE::MetaInfoType::CLASS )
	{
		return static_cast<const XE::MetaClass *>( type1 )->CanConvert( static_cast<const XE::MetaClass *>( type2 ) );
	}

	return false;
}


bool XE::VariantDataIsNull::operator()( const std::monostate & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const bool & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::int8 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::int16 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::int32 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::int64 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::uint8 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::uint16 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::uint32 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::uint64 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::float32 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::float64 & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::VariantEnumData & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::VariantSmallData & ) const
{
	return false;
}
bool XE::VariantDataIsNull::operator()( const XE::VariantPointerData & val ) const
{
	return val.Value == nullptr;
}
bool XE::VariantDataIsNull::operator()( const XE::VariantWarpperData & val ) const
{
	return val.Pointer == nullptr || val.Pointer->ValuePointer() == nullptr;
}

XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const std::monostate & ) const
{
	return nullptr;
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const bool & ) const
{
	return ::TypeID< bool >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::int8 & ) const
{
	return ::TypeID< XE::int8 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::int16 & ) const
{
	return ::TypeID< XE::int16 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::int32 & ) const
{
	return ::TypeID< XE::int32 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::int64 & ) const
{
	return ::TypeID< XE::int64 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::uint8 & ) const
{
	return ::TypeID< XE::uint8 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::uint16 & ) const
{
	return ::TypeID< XE::uint16 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::uint32 & ) const
{
	return ::TypeID< XE::uint32 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::uint64 & ) const
{
	return ::TypeID< XE::uint64 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::float32 & ) const
{
	return ::TypeID< XE::float32 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::float64 & ) const
{
	return ::TypeID< XE::float64 >::Get();
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::VariantEnumData & val ) const
{
	return SP_CAST< const XE::MetaType >( val.Type->shared_from_this() );
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::VariantSmallData & val ) const
{
	return SP_CAST< const XE::MetaType >( val.Type->shared_from_this() );
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::VariantPointerData & val ) const
{
	return SP_CAST< const XE::MetaType >( val.Type->shared_from_this() );
}
XE::MetaTypeCPtr XE::VariantDataGetMetaType::operator()( const XE::VariantWarpperData & val ) const
{
	return val.Pointer != nullptr ? SP_CAST< const XE::MetaType >( val.Pointer->GetMetaType()->shared_from_this() ) : nullptr;
}

bool XE::VariantDataIsContainer::operator()( const std::monostate & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const bool & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::int8 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::int16 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::int32 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::int64 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::uint8 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::uint16 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::uint32 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::uint64 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::float32 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::float64 & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::VariantEnumData & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::VariantSmallData & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::VariantPointerData & ) const
{
	return false;
}
bool XE::VariantDataIsContainer::operator()( const XE::VariantWarpperData & val ) const
{
	return val.Pointer != nullptr && val.Pointer->IsContainer();
}

void * XE::VariantDataGetRawPointer::operator()( std::monostate & ) const
{
	return nullptr;
}
void * XE::VariantDataGetRawPointer::operator()( bool & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::int8 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::int16 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::int32 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::int64 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::uint8 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::uint16 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::uint32 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::uint64 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::float32 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::float64 & val ) const
{
	return &val;
}
void * XE::VariantDataGetRawPointer::operator()( XE::VariantEnumData & val ) const
{
	return &val.Value;
}
void * XE::VariantDataGetRawPointer::operator()( XE::VariantSmallData & val ) const
{
	return &val.Value;
}
void * XE::VariantDataGetRawPointer::operator()( XE::VariantPointerData & val ) const
{
	return val.Value;
}
void * XE::VariantDataGetRawPointer::operator()( XE::VariantWarpperData & val ) const
{
	return val.Pointer != nullptr ? val.Pointer->ValuePointer() : nullptr;
}


XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const std::monostate & ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const bool & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::int8 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::int16 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::int32 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::int64 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::uint8 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::uint16 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::uint32 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::uint64 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::float32 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::float64 & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::VariantEnumData & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::VariantSmallData & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::VariantPointerData & val ) const
{
	return {};
}
XE::Array< XE::Variant > XE::VariantDataToArray::operator()( const XE::VariantWarpperData & val ) const
{
	return ( val.Pointer && val.Pointer->IsContainer() ) ? val.Pointer->ToArray() : XE::Array< XE::Variant >();
}

XE::VariantDataFromArray::VariantDataFromArray( const XE::Array< XE::Variant > & val )
	:Array( val )
{

}
bool XE::VariantDataFromArray::operator()( const std::monostate & ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const bool & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::int8 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::int16 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::int32 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::int64 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::uint8 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::uint16 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::uint32 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::uint64 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::float32 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::float64 & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::VariantEnumData & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::VariantSmallData & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::VariantPointerData & val ) const
{
	return false;
}
bool XE::VariantDataFromArray::operator()( const XE::VariantWarpperData & val ) const
{
	return ( val.Pointer && val.Pointer->IsContainer() ) ? val.Pointer->FromArray( Array ) : false;
}

XE::VariantData XE::VariantDataCloneVariantData::operator()( const std::monostate & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const bool & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::int8 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::int16 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::int32 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::int64 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::uint8 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::uint16 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::uint32 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::uint64 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::float32 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::float64 & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::VariantEnumData & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::VariantSmallData & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::VariantPointerData & val ) const
{
	return val;
}
XE::VariantData XE::VariantDataCloneVariantData::operator()( const XE::VariantWarpperData & val ) const
{
	XE::VariantWarpperData warpper;
	warpper.Pointer = val.Pointer ? val.Pointer->Clone() : nullptr;
	return warpper;
}
