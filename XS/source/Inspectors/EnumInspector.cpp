#include "EnumInspector.h"

REG_WIDGET( XS::EnumInspector );

XS::EnumInspector::EnumInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QComboBox = new QComboBox( this );
	SetContentWidget( _QComboBox );
}

XS::EnumInspector::~EnumInspector()
{

}

void XS::EnumInspector::Refresh()
{
	disconnect( _QComboBox, nullptr );

	auto type = GetObjectProxy()->GetType();
	if ( auto enm = SP_CAST< const XE::MetaEnum >( type ) )
	{
		auto name = enm->FindName( GetObjectProxy()->GetValue() );

		_QComboBox->clear();

		enm->Visit( [this]( const XE::String & name, const XE::Variant & val )
			{
				_QComboBox->addItem( name.c_str(), val.ToUInt64() );
			} );

		_QComboBox->setCurrentText( name.c_str() );
	}

	connect( _QComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), [this]( int val )
		{
			GetObjectProxy()->SetValue( XE::VariantEnumData( _QComboBox->itemData( Qt::UserRole + 1 ).toULongLong(), GetObjectProxy()->GetType() ) );
		} );
}
