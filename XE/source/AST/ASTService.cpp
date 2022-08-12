#include "ASTService.h"


struct XE::ASTService::Private
{
	XE::Set< XE::String > _Macros;
	XE::Map< XE::MetaClassCPtr, XE::Delegate< void( XE::ASTVisitor * ) > > _CustomNodeExecutes;
	XE::Map< XE::MetaClassCPtr, XE::Delegate< void( XE::ASTVisitor * ) > > _CustomNodeCompiles;
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

}

void XE::ASTService::Update()
{

}

void XE::ASTService::Clearup()
{

}

void XE::ASTService::AddMacro( const XE::String & val )
{
	_p->_Macros.insert( val );
}

bool XE::ASTService::HasMacro( const XE::String & val ) const
{
	return _p->_Macros.find( val ) != _p->_Macros.end();
}

void XE::ASTService::AddCustomNodeExecute( const XE::MetaClassCPtr & type, const XE::Delegate< void( XE::ASTVisitor * ) > & callback )
{
	_p->_CustomNodeExecutes.insert( { type, callback } );
}

void XE::ASTService::AddCustomNodeCompile( const XE::MetaClassCPtr & type, const XE::Delegate< void( XE::ASTVisitor * ) > & callback )
{
	_p->_CustomNodeCompiles.insert( { type, callback } );
}

bool XE::ASTService::ExecuteCustomNode( const XE::MetaClassCPtr & type, XE::ASTVisitor * visitor ) const
{
	auto it = _p->_CustomNodeExecutes.find( type );
	if ( it != _p->_CustomNodeExecutes.end() )
	{
		it->second( visitor );

		return true;
	}

	return false;
}

bool XE::ASTService::CompileCustomNode( const XE::MetaClassCPtr & type, XE::ASTVisitor * visitor ) const
{
	auto it = _p->_CustomNodeCompiles.find( type );
	if ( it != _p->_CustomNodeCompiles.end() )
	{
		it->second( visitor );

		return true;
	}

	return false;
}
