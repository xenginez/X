#include "Type.h"

#include "Base/Format.hpp"

#include "MetaInfo.h"
#include "Reflection.h"

BEG_META( XE::TypeFlag )
type->Value( "VALUE", XE::TypeFlag::VALUE );
type->Value( "POINTER", XE::TypeFlag::POINTER );
type->Value( "REFERENCE", XE::TypeFlag::REFERENCE );
type->Value( "SHAREDPTR", XE::TypeFlag::SHAREDPTR );
END_META()

XE::MetaException::MetaException( XE::MetaInfoCPtr Meta, const XE::String & Msg )
	: XE::RuntimeException( XE::Format( "MetaException: {} {}", Meta->GetFullName(), Msg ) ), _Meta( Meta )
{

}

XE::MetaException::~MetaException()
{

}

XE::MetaInfoCPtr XE::MetaException::GetMetaInfo() const
{
	return _Meta;
}
