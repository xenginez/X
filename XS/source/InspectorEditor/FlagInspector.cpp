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

		_QComboBox->addItem( tr( "NONE" ) );
		_QComboBox->addItem( tr( "ALL" ) );

		meta->Visit( [&]( const XE::String & name, const XE::Variant & val )
			{
				if ( val.ToUInt64() == 0 )
				{
					return;
				}

				_QComboBox->addItem( tr( name.c_str() ) );
			} );
	}

	RefreshComboBox( GetObjectProxy()->GetValue().ToUInt64() );

	connect( _QComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ), [this]( int index )
		{
			if ( auto meta = SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() ) )
			{
				XE::VariantEnumData data( GetObjectProxy()->GetValue().ToUInt64(), meta.get() );

				// NONE
				if ( index == 0 )
				{
					data.Value = 0;
				}
				// ALL
				else if ( index == 1 )
				{
					data.Value = meta->GetFlags();
				}
				// OTHER
				else
				{
					if ( _QComboBox->itemData( index ).toBool() )
					{
						data.Value &= ~meta->FindValue( _QComboBox->itemText( index ).toStdString() ).ToUInt64();
					}
					else
					{
						data.Value |= meta->FindValue( _QComboBox->itemText( index ).toStdString() ).ToUInt64();
					}
				}

				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), value = data]()
				{
					proxy->SetValue( value );
					RefreshComboBox( value.Value );
				},
					[this, proxy = GetObjectProxy(), value = GetObjectProxy()->GetValue()]()
				{
					proxy->SetValue( value );
					RefreshComboBox( value.ToUInt64() );
				} );
			}
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

void XS::FlagInspector::RefreshComboBox( XE::uint64 flag )
{
	auto meta = SP_CAST< const XE::MetaEnum >( GetObjectProxy()->GetType() );

	if ( flag == 0 ) // NONE
	{
		for ( size_t i = 2; i < _QComboBox->count(); i++ )
		{
			_QComboBox->setItemData( i, false );
			_QComboBox->setItemIcon( i, QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ) );
		}
		_QComboBox->setToolTip( tr( "NONE" ) );
	}
	else if( flag == meta->GetFlags() ) // ALL
	{
		for ( size_t i = 2; i < _QComboBox->count(); i++ )
		{
			_QComboBox->setItemData( i, true );
			_QComboBox->setItemIcon( i, QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ) );
		}
		_QComboBox->setToolTip( tr( "ALL" ) );
	}
	else
	{
		QString tool_tip;
		for ( size_t i = 2; i < _QComboBox->count(); i++ )
		{
			if ( ( flag & meta->FindValue( _QComboBox->itemText( i ).toStdString() ).ToUInt64() ) != 0 )
			{
				tool_tip += ( _QComboBox->itemText( i ) + "\n" );

				_QComboBox->setItemData( i, true );
				_QComboBox->setItemIcon( i, QIcon( "SkinIcons:/images/common/icon_checkbox_checked.png" ) );
			}
			else
			{
				_QComboBox->setItemData( i, false );
				_QComboBox->setItemIcon( i, QIcon( "SkinIcons:/images/common/icon_checkbox_unchecked.png" ) );
			}
		}
		tool_tip.remove( tool_tip.size() - 1, 1 );
		_QComboBox->setToolTip( tool_tip );
	}
}
