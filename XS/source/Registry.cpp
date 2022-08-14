#include "Registry.h"

void XS::Registry::Register( const QMetaObject * meta, const std::function< QObject * ( QObject * ) > & construct )
{
	auto it = _p()->_Deriveds.find( meta->superClass() );
	if ( it != _p()->_Deriveds.end() )
	{
		it.value().push_back( meta );
	}
	else
	{
		_p()->_Deriveds.insert( meta->superClass(), { meta } );
	}

	_p()->_Constructs.insert( meta->className(), construct );
}

QObject * XS::Registry::Construct( const QString & name, QObject * parent )
{
	auto it = _p()->_Constructs.find( name );

	if( it != _p()->_Constructs.end() )
	{
		return it.value()( parent );
	}

	return nullptr;
}

QList< const QMetaObject * > XS::Registry::GetDerivedClass( const QMetaObject * super )
{
	auto it = _p()->_Deriveds.find( super );

	return it != _p()->_Deriveds.end() ? it.value() : QList< const QMetaObject * >{};
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
