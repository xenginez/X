#include "ASTService.h"

#include "ASTInfo.h"
#include "ASTNode.h"
#include "ASTVisitor.h"
#include "ASTContext.h"

#include "Core/CoreFramework.h"
#include "Core/AssetsService.h"

IMPLEMENT_META( XE::ASTService )

struct XE::ASTService::Private
{
	XE::Set< XE::String > _Macros;
	XE::Map< XE::String, XE::Variant > _Globals;
	XE::MultiMap< XE::MetaClassCPtr, XE::ASTVisitorPtr > _Visitors; // ASTNode Type : ASTVisitor
};

XE::ASTService::ASTService()
	:_p( XE::New< Private >() )
{

}

XE::ASTService::~ASTService()
{
	XE::Delete( _p );
}

void XE::ASTService::Prepare()
{

}

void XE::ASTService::Startup()
{
	if ( auto asset = GetFramework()->GetServiceT< XE::AssetsService >() )
	{
		//asset->
	}
}

void XE::ASTService::Update()
{

}

void XE::ASTService::Clearup()
{

}

void XE::ASTService::Visit( XE::ASTContext * context, const XE::ASTNode * node ) const
{
	auto ranges = _p->_Visitors.equal_range( node->GetMetaClass() );
	for ( auto it = ranges.first; it != ranges.second; ++it )
	{
		if ( it->second->GetMetaClass()->CanConvert( context->GetVisitorBaseClass() ) )
		{
			if ( auto visit = it->second->GetMetaClass()->FindMethod( "Visit" ) )
			{
				if ( context->GetMetaClass()->CanConvert( SP_CAST< const XE::MetaClass >( visit->GetParameterType().front() ) ) )
				{
					XE::InvokeStack args( it->second, context, node );

					visit->Invoke( &args );

					return;
				}
			}
		}
	}
}

void XE::ASTService::Visit( XE::ASTContext * context, const XE::ASTNodePtr & node ) const
{
	Visit( context, node.get() );
}

void XE::ASTService::SetGlobalVariable( const XE::String & name, const XE::Variant & val )
{
	_p->_Globals[name] = val;
}

XE::Variant XE::ASTService::GetGlobalVariable( const XE::String & name )
{
	auto it = _p->_Globals.find( name );

	return it != _p->_Globals.end() ? it->second : XE::Variant();
}

void XE::ASTService::AddGlobalMacro( const XE::String & val )
{
	_p->_Macros.insert( val );
}

bool XE::ASTService::HasGlobalMacro( const XE::String & val ) const
{
	return _p->_Macros.find( val ) != _p->_Macros.end();
}

XE::ASTInstancePtr XE::ASTService::GetInstance() const
{
	thread_local XE::ASTInstancePtr Instance = XE::MakeShared< XE::ASTInstance >();
	return Instance;
}

bool XE::ASTService::JITInvoke( const XE::String & name, XE::InvokeStack & args, XE::Variant & result ) const
{

}
