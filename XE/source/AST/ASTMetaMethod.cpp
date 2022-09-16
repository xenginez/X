#include "ASTMetaMethod.h"

#include "ASTInfo.h"
#include "ASTVisitor.h"
#include "ASTContext.h"

namespace
{
	XE::ParameterType GetMethodParameter( const XE::ASTInfoMethodPtr & val )
	{
		XE::ParameterType result;

		for ( const auto & it : val->Parameters )
		{
			result.push_back( XE::Reflection::FindType( it.first ) );
		}

		return result;
	}
}

XE::ASTMetaMethod::ASTMetaMethod( const XE::ASTInfoMethodPtr & val, const XE::MetaClassCPtr Owner, const XE::MetaModuleCPtr & Module )
	: XE::MetaMethod( val->Name,
					  false,
					  false,
					  XE::Reflection::FindType( val->Result->Name ),
					  GetMethodParameter( val ),
					  Owner,
					  Module )
	, _Method( val )
{

}

XE::ASTMetaMethod::~ASTMetaMethod()
{

}

XE::Variant XE::ASTMetaMethod::Invoke( XE::InvokeStack * params ) const
{
	return XE::ASTExecuteContext( XE::MemoryResource::GetFrameMemoryResource() ).Invoke( _Method, params );
}
