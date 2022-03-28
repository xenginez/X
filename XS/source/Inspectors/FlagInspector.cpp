#include "FlagInspector.h"

#include <QDebug>
#include <QEvent>
#include <QAction>
#include <QVBoxLayout>
#include <QResizeEvent>

REG_WIDGET( XS::FlagInspector );

XS::FlagInspector::FlagInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	QWidget * widget = new QWidget( this );
	{
		QVBoxLayout * verticalLayout = new QVBoxLayout( widget );
		{
			verticalLayout->setSpacing( 0 );
			verticalLayout->setObjectName( QString::fromUtf8( "layout" ) );
			verticalLayout->setContentsMargins( 0, 0, 0, 0 );

			{
				_QComboBox = new QComboBox( this );
				_QComboBox->hide();

				_QLineEdit = new QLineEdit( this );
				_QLineEdit->setReadOnly( true );
				_QLineEdit->installEventFilter( this );

				connect( _QLineEdit->addAction( QIcon( "SkinIcons:/images/common/icon_branch_open.png" ), QLineEdit::TrailingPosition ), &QAction::triggered, [this]() { _QComboBox->showPopup(); } );
			}

			verticalLayout->addWidget( _QComboBox );
			verticalLayout->addWidget( _QLineEdit );
		}
	}
	SetContentWidget( widget );
}

XS::FlagInspector::~FlagInspector()
{

}

void XS::FlagInspector::Refresh()
{
	disconnect( _QComboBox, nullptr );

	if ( auto enm = SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() ) )
	{
		auto u64 = GetObjectProxy()->GetValue().ToUInt64();

		_QComboBox->clear();

		QString list; XE::uint64 all = 0;
		enm->Visit( [&]( const XE::String & name, const XE::Variant & val )
			{
				if ( val.ToUInt64() == 0 )
				{
					return;
				}

				all |= val.ToUInt64();

				if ( ( u64 & val.ToUInt64() ) != 0 )
				{
					list += name.c_str();
					list += ";";
					_QComboBox->addItem( QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ), tr( name.c_str() ), true );
				}
				else
				{
					_QComboBox->addItem( QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ), tr( name.c_str() ), false );
				}
			} );
		if ( list.isEmpty() ) list = tr( "NONE;" );
		else if ( all == u64 ) list = tr( "ALL;" );

		list.remove( list.size() - 1, 1 );
		_QLineEdit->setText( list );
		_QLineEdit->setToolTip( list.replace( ';', '\n' ) );
	}

	connect( _QComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), [this]( int index )
		{
			if ( auto enm = SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() ) )
			{
				auto u64 = GetObjectProxy()->GetValue().ToUInt64();

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

				GetObjectProxy()->SetValue( XE::VariantData( XE::VariantEnumData( u64, GetObjectProxy()->GetValue().GetType() ) ) );

				QString list; XE::uint64 all = 0;
				enm->Visit( [&]( const XE::String & name, const XE::Variant & val )
					{
						all |= val.ToUInt64();

						if ( ( u64 & val.ToUInt64() ) != 0 )
						{
							list += name.c_str();
							list += ";";
						}
					} );
				if ( list.isEmpty() ) list = tr( "NONE;" );
				else if ( all == u64 ) list = tr( "ALL;" );

				list.remove( list.size() - 1, 1 );
				_QLineEdit->setText( list );
				_QLineEdit->setToolTip( list.replace( ';', '\n' ) );
			}
		} );
}

bool XS::FlagInspector::eventFilter( QObject * obj, QEvent * event )
{
	if ( obj == _QLineEdit )
	{
		if ( event->type() == QEvent::Resize )
		{
			_QComboBox->resize( static_cast<QResizeEvent *>( event )->size() );
		}
		else if ( event->type() == QEvent::MouseButtonRelease )
		{
			_QComboBox->showPopup();
		}
	}

	return Inspector::eventFilter( obj, event );
}
