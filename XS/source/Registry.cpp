#include "Registry.h"

void XS::Registry::Register( const QString & name, const std::function< QWidget * ( QWidget * ) > & construct )
{
	_p()->_Constructs.insert( { name, construct } );
}

QWidget * XS::Registry::Construct( const QString & name, QWidget * parent )
{
	auto it = _p()->_Constructs.find( name );

	if( it != _p()->_Constructs.end() )
	{
		return it->second( parent );
	}

	return nullptr;
}

XS::Registry::Registry()
{

}

XS::Registry::~Registry()
{

}

XS::Registry * XS::Registry::_p()
{
	static Registry registry;

	return &registry;
}
