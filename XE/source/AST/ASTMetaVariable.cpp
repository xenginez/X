#include "ASTMetaVariable.h"

#include "Core/CoreFramework.h"

#include "ASTService.h"

XE::ASTMetaVariable::ASTMetaVariable( const XE::SharedPtr< XE::ASTVariable > & val )
	: XE::MetaVariable( val->Name, false, false, false, true, false,
						XE::Reflection::FindType( val->Type ),
						XE::Reflection::FindModule( val->Module ) )
	, _Variable( val )
{

}

XE::ASTMetaVariable::~ASTMetaVariable()
{

}

XE::Variant XE::ASTMetaVariable::Getter() const
{
	return XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->GetVariable( GetFullName() );
}

void XE::ASTMetaVariable::Setter( const XE::Variant & val ) const
{
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ASTService >()->SetVariable( GetFullName(), val );
}
