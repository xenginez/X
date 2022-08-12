#include "ASTMetaMethod.h"

#include "ASTExecutor.h"

namespace
{
	XE::ParameterType GetMethodParameter( const XE::SharedPtr< XE::ASTMethod > & val )
	{
		XE::ParameterType result;

		for ( const auto & it : val->Parameters )
		{
			result.push_back( XE::Reflection::FindType( it.first ) );
		}

		return result;
	}
}

XE::ASTMetaMethod::ASTMetaMethod( const XE::SharedPtr< XE::ASTMethod > & val )
	: XE::MetaMethod( val->Name,
					  false,
					  false,
					  XE::Reflection::FindType( val->Result ),
					  GetMethodParameter( val ),
					  XE::Reflection::FindClass( val->Owner ),
					  XE::Reflection::FindModule( val->Module ) )
	, _Method( val )
{

}

XE::ASTMetaMethod::~ASTMetaMethod()
{

}

XE::Variant XE::ASTMetaMethod::Invoke( XE::InvokeStack * params ) const
{
	XE::ASTExecutor exec;

	return exec.Invoke( _Method, params );
}
