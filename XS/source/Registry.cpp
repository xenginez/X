#include "Registry.h"

void XS::Registry::Register( const QMetaObject * meta, const std::function< QWidget * ( QWidget * ) > & construct )
{
	_p()->_Constructs.insert( { meta->className(), { meta, construct } } );
}

QWidget * XS::Registry::Construct( const QString & name, QWidget * parent )
{
	auto it = _p()->_Constructs.find( name );

	if( it != _p()->_Constructs.end() )
	{
		return std::get<1>( it->second )( parent );
	}

	return nullptr;
}

QList< const QMetaObject * > XS::Registry::GetDerivedClass( const QMetaObject * super )
{
	QList< const QMetaObject * > metas;

	for ( auto it : _p()->_Constructs )
	{
		if ( std::get<0>( it.second )->inherits( super ) )
		{
			metas.push_back( std::get<0>( it.second ) );
		}
	}

	return metas;
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
