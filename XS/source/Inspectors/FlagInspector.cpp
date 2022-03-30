#include "FlagInspector.h"

#include <QEvent>
#include <QStylePainter>

REG_WIDGET( XS::FlagInspector );

XS::FlagInspector::FlagInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QComboBox = new QComboBox( this );

	_QComboBox->installEventFilter( this );

	SetContentWidget( _QComboBox );
}

XS::FlagInspector::~FlagInspector()
{

}

void XS::FlagInspector::Refresh()
{
	disconnect( _QComboBox, nullptr );

	if ( auto meta = SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() ) )
	{
		_QComboBox->clear();

		auto value = GetObjectProxy()->GetValue().ToUInt64();

		_QComboBox->addItem( tr( "NONE" ) );
		_QComboBox->addItem( tr( "ALL" ) );

		QString tool_tip; XE::uint64 all = 0;
		meta->Visit( [&]( const XE::String & name, const XE::Variant & val )
			{
				if ( val.ToUInt64() == 0 )
				{
					return;
				}

				all |= val.ToUInt64();

				if ( ( value & val.ToUInt64() ) != 0 )
				{
					tool_tip += ( name + "\n" ).c_str();

					_QComboBox->addItem( QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ), tr( name.c_str() ), true );
				}
				else
				{
					_QComboBox->addItem( QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ), tr( name.c_str() ), false );
				}
			} );
		if ( tool_tip.isEmpty() )
		{
			tool_tip = tr( "NONE" );
		}
		else if ( all == value )
		{
			tool_tip = tr( "ALL" );
		}
		else
		{
			tool_tip.remove( tool_tip.size() - 1, 1 );
		}

		_QComboBox->setToolTip( tool_tip );
	}

	connect( _QComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), [this]( int index )
		{
			QString tool_tip;
			XE::VariantEnumData data;

			if ( index == 0 )
			{
				for ( size_t i = 2; i < _QComboBox->count(); i++ )
				{
					_QComboBox->setItemData( i, false );
					_QComboBox->setItemIcon( i, QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ) );
				}
				tool_tip = "NONE";
			}
			else if ( index == 1 )
			{
				for ( size_t i = 2; i < _QComboBox->count(); i++ )
				{
					_QComboBox->setItemData( i, true );
					_QComboBox->setItemIcon( i, QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ) );
				}
				tool_tip = "ALL";
			}
			else
			{
				auto meta = SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() );
				auto value = GetObjectProxy()->GetValue().ToUInt64();

				if ( _QComboBox->itemData( index ).toBool() )
				{
					value &= ~meta->FindValue( _QComboBox->itemText( index ).toStdString() ).ToUInt64();

					_QComboBox->setItemData( index, false );
					_QComboBox->setItemIcon( index, QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ) );
				}
				else
				{
					value |= meta->FindValue( _QComboBox->itemText( index ).toStdString() ).ToUInt64();

					_QComboBox->setItemData( index, true );
					_QComboBox->setItemIcon( index, QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ) );
				}

				data = XE::VariantEnumData( value, GetObjectProxy()->GetType() );

				XE::uint64 all = 0;
				meta->Visit( [&]( const XE::String & name, const XE::Variant & val )
					{
						all |= val.ToUInt64();

						if ( ( value & val.ToUInt64() ) != 0 )
						{
							tool_tip += ( name + "\n" ).c_str();
						}
					} );

				if ( tool_tip.isEmpty() )
				{
					tool_tip = tr( "NONE" );
				}
				else if ( all == value )
				{
					tool_tip = tr( "ALL" );
				}
				else
				{
					tool_tip.remove( tool_tip.size() - 1, 1 );
				}
			}

			PushUndoCommand( GetObjectProxy()->GetName().c_str(),
				[this, proxy = GetObjectProxy(), value = data, tool_tip]()
			{
				proxy->SetValue( value );
				_QComboBox->setToolTip( tool_tip );
			},
				[this, proxy = GetObjectProxy(), value = GetObjectProxy()->GetValue(), tool_tip = _QComboBox->toolTip()]()
			{
				proxy->SetValue( value );
				_QComboBox->setToolTip( tool_tip );
			} );
		} );
}

bool XS::FlagInspector::eventFilter( QObject * watched, QEvent * event )
{
	if ( watched == _QComboBox && event->type() == QEvent::Paint )
	{
		QStylePainter painter( _QComboBox );
		painter.setPen( palette().color( QPalette::Text ) );

		QStyleOptionComboBox opt;
		{
			opt.initFrom( _QComboBox );
			opt.editable = false;
			opt.frame = _QComboBox->hasFrame();
			if ( _QComboBox->hasFocus() && !opt.editable )
			{
				opt.state |= QStyle::State_Selected;
			}

			opt.subControls = QStyle::SC_All;
			opt.activeSubControls = QStyle::SC_ComboBoxArrow;
			opt.state |= QStyle::State_Sunken;

			bool all = true;
			for ( size_t i = 2; i < _QComboBox->count(); i++ )
			{
				if ( _QComboBox->itemData( i ).toBool() )
				{
					opt.currentText += _QComboBox->itemText( i ) + ";";
				}
				else
				{
					all = false;
				}
			}

			if ( opt.currentText.isEmpty() )
			{
				opt.currentText = "NONE";
			}
			else if( all )
			{
				opt.currentText = "ALL";
			}
			else
			{
				opt.currentText.remove( opt.currentText.size() - 1, 1 );
			}
		}
		painter.drawComplexControl( QStyle::CC_ComboBox, opt );

		painter.drawControl( QStyle::CE_ComboBoxLabel, opt );

		return true;
	}

	return Inspector::eventFilter( watched, event );
}
