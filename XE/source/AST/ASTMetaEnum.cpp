#include "ASTMetaEnum.h"

#include "ASTInfo.h"

XE::ASTMetaEnum::ASTMetaEnum( const XE::ASTInfoEnumPtr & val, const XE::MetaModuleCPtr & Module )
	: XE::MetaEnum( val->Name, sizeof( XE::uint64 ), nullptr, Module ), _Enum( val )
{
	for ( size_t i = 0; i < _Enum->Elements.size(); i++ )
	{
		XE::uint64 value = i;

		if ( _Enum->Flag )
		{
			value = XE::uint64( 1 ) << i;
		}

		_RegisterValue( _Enum->Elements[i], XE::VariantEnumData( value, this ) );
	}
}

XE::ASTMetaEnum::~ASTMetaEnum()
{

}

void XE::ASTMetaEnum::Serialize( XE::OArchive & arc, const XE::Variant & val ) const
{

}

void XE::ASTMetaEnum::Deserialize( XE::IArchive & arc, XE::Variant & obj ) const
{

}
