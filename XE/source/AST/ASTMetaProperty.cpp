#include "ASTMetaProperty.h"

#include "ASTInfo.h"

XE::ASTMetaProperty::ASTMetaProperty( const XE::ASTInfoPropertyPtr & val, const XE::MetaClassCPtr Owner, const XE::MetaModuleCPtr & Module )
	: XE::MetaProperty( val->Name, false, false, val->Flag,
						XE::Reflection::FindType( val->Type->Name ),
						Owner,
						Module )
	, _Property( val )
{

}

XE::ASTMetaProperty::~ASTMetaProperty()
{

}

XE::Variant XE::ASTMetaProperty::Getter( const XE::Variant & obj ) const
{
	return reinterpret_cast<XE::Variant *>( reinterpret_cast<XE::uint8 *>( obj.ToPointer() ) + _Property->Offset );
}

void XE::ASTMetaProperty::Setter( const XE::Variant & obj, const XE::Variant & val ) const
{
	*( reinterpret_cast<XE::Variant *>( reinterpret_cast<XE::uint8 *>( obj.ToPointer() ) + _Property->Offset ) ) = val;
}
