#include "EnumInspector.h"

REG_WIDGET( XS::EnumInspector );

XS::EnumInspector::EnumInspector( QWidget * parent /*= nullptr */ )
	:InspectorWidget( parent )
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
			if ( _QComboBox->itemData( val ).toULongLong() != GetObjectProxy()->GetValue().ToUInt64() )
			{
				auto data = XE::VariantEnumData( _QComboBox->itemData( val ).toULongLong(), GetObjectProxy()->GetType().get() );

				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), value = data]()
				{
					proxy->SetValue( value );
					_QComboBox->setCurrentIndex( _QComboBox->findText( SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() )->FindName( value ).c_str() ) );
				},
					[this, proxy = GetObjectProxy(), value = GetObjectProxy()->GetValue()]()
				{
					proxy->SetValue( value );
					_QComboBox->setCurrentIndex( _QComboBox->findText( SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() )->FindName( value ).c_str() ) );
				} );
			}
		} );
}
