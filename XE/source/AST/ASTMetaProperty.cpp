#include "ASTMetaProperty.h"

XE::ASTMetaProperty::ASTMetaProperty( const XE::SharedPtr< XE::ASTProperty > & val )
	: XE::MetaProperty( val->Name, false, false, val->Flag,
						XE::Reflection::FindType( val->Type ),
						XE::Reflection::FindClass( val->Owner ),
						XE::Reflection::FindModule( val->Module ) )
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
