#include "EnumInspector.h"

Q_DECLARE_METATYPE( XE::Variant );

REG_WIDGET( XS::EnumInspector );

XS::EnumInspector::EnumInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QComboBox = new QComboBox( this );

	connect( _QComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), [this]( int val )
		{
			GetObjectProxy().SetValue( _QComboBox->itemData( Qt::UserRole + 1 ).value<XE::Variant>() );
		} );
}

XS::EnumInspector::~EnumInspector()
{

}

void XS::EnumInspector::Refresh()
{
	auto type = GetObjectProxy().GetType();
	if ( auto enm = SP_CAST< const XE::MetaEnum >( type ) )
	{
		auto name = enm->FindName( GetObjectProxy().GetValue() );

		_QComboBox->clear();

		enm->Visit( [this]( const XE::String & name, const XE::Variant & val )
			{
				_QComboBox->addItem( name.c_str(), QVariant::fromValue( val ) );
			} );

		_QComboBox->setCurrentText( name.c_str() );
	}
}
