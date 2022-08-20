#include "ASTMetaVariable.h"

#include "Core/CoreFramework.h"

#include "ASTInfo.h"
#include "ASTService.h"

XE::ASTMetaVariable::ASTMetaVariable( const XE::ASTInfoVariablePtr & val, const XE::MetaModuleCPtr & Module )
	: XE::MetaVariable( val->Name, false, val->Flag,
						XE::Reflection::FindType( val->Type->Name ),
						Module )
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
