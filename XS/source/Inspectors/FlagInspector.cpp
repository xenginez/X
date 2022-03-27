#include "FlagInspector.h"

#include <QLineEdit>

REG_WIDGET( XS::FlagInspector );

XS::FlagInspector::FlagInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QComboBox = new QComboBox( this );

	connect( _QComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), [this]( int index )
		{
			if ( auto enm = SP_CAST< const XE::MetaEnum >( GetObjectProxy().GetType() ) )
			{
				auto u64 = GetObjectProxy().GetValue().ToUInt64();

				if ( _QComboBox->itemData( index ).toBool() )
				{
					u64 &= ~enm->FindValue( _QComboBox->itemText( index ).toStdString() ).ToUInt64();

					_QComboBox->setItemData( index, false );
					_QComboBox->setItemIcon( index, QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ) );
				}
				else
				{
					u64 |= enm->FindValue( _QComboBox->itemText( index ).toStdString() ).ToUInt64();

					_QComboBox->setItemData( index, true );
					_QComboBox->setItemIcon( index, QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ) );
				}

				GetObjectProxy().SetValue( XE::VariantData( XE::VariantEnumData( u64, GetObjectProxy().GetValue().GetType() ) ) );
			}
		} );
}

XS::FlagInspector::~FlagInspector()
{

}

void XS::FlagInspector::Refresh()
{
	if ( auto enm = SP_CAST< const XE::MetaEnum >( GetObjectProxy().GetType() ) )
	{
		auto value = GetObjectProxy().GetValue().ToUInt64();

		_QComboBox->clear();

		QString list;
		enm->Visit( [&]( const XE::String & name, const XE::Variant & val )
			{
				if ( ( value & val.ToUInt64() ) != 0 )
				{
					list += name.c_str();
					list += ";";
					_QComboBox->addItem( QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ), name.c_str(), true );
				}
				else
				{
					_QComboBox->addItem( QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ), name.c_str(), false );
				}
			} );

		_QComboBox->lineEdit()->setText( list );
		_QComboBox->setToolTip( list.replace( ';', "\r\n" ) );
	}
}
