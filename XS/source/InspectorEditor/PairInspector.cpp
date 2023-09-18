#include "PairInspector.h"

REG_INSPECTOR_T( XE::Pair, XS::PairInspector );

XS::PairInspector::PairInspector( QWidget * parent /*= nullptr */ )
	: XS::InspectorWidget( parent )
{
	_Layout = new QFormLayout( this );

	_Layout->setSpacing( 0 );
	_Layout->setObjectName( QString::fromUtf8( "layout" ) );
	_Layout->setContentsMargins( 0, 0, 0, 0 );
}

XS::PairInspector::~PairInspector()
{

}

void XS::PairInspector::Refresh()
{
	if ( auto cls = SP_CAST< const XE::MetaClass>( GetObjectProxy()->GetType() ) )
	{
		for ( int i = 0; i < _Layout->rowCount(); i++ )
		{
			_Layout->removeRow( i );
		}

		auto first = cls->FindProperty( "first" );
		auto second = cls->FindProperty( "second" );

		_Layout->addRow( tr( "key" ), XS::InspectorWidget::Create( new XS::PropertyObjectProxy( first, GetObjectProxy() ), this ) );
		_Layout->addRow( tr( "value" ), XS::InspectorWidget::Create( new XS::PropertyObjectProxy( second, GetObjectProxy() ), this ) );
	}
}
