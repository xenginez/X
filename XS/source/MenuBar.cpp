#include "MenuBar.h"

#include <QDebug>
#include <QStyle>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QStyleOption>

#include "Skin.h"

XS::MenuBar::MenuBar( QWidget * parent /*= nullptr */ )
	:QMenuBar( parent )
{
	// close
	{
		_Close = new QToolButton( this );
		_Close->setObjectName( "closeButton" );
		_Close->setIcon( QIcon( "SkinIcons:/images/window/icon_window_close.png" ) );
		_Close->setObjectName( QString::fromUtf8( "closeButton" ) );
		_Close->setStyleSheet( QString::fromUtf8( "#closeButton{\n"
												  "  background-color:none;\n"
												  "  border:none;\n"
												  "}\n"
												  "#closeButton:hover{\n"
												  "  background-color:palette(alternate-base);\n"
												  "}\n"
												  "##closeButton:pressed{\n"
												  "  background-color:palette(highlight);\n"
												  "}" ) );

		connect( _Close, &QToolButton::clicked, this, &MenuBar::CloseButtonClicked );
	}

	// restore
	{
		_Restore = new QToolButton( this );
		_Restore->setObjectName( QString::fromUtf8( "restoreButton" ) );
		_Restore->setIcon( QIcon( "SkinIcons:/images/window/icon_window_restore.png" ) );
		_Restore->setStyleSheet( QString::fromUtf8( "#restoreButton{\n"
													"  background-color:none;\n"
													"  border:none;\n"
													"}\n"
													"#restoreButton:hover{\n"
													"  background-color:palette(alternate-base);\n"
													"}\n"
													"#restoreButton:pressed{\n"
													"  background-color:palette(highlight);\n"
													"}" ) );
		setRestoreButtonVisable( true );

		connect( _Restore, &QToolButton::clicked, this, &MenuBar::RestoreButtonClicked );
	}

	// minimize
	{
		_Minimize = new QToolButton( this );
		_Minimize->setObjectName( QString::fromUtf8( "minimizeButton" ) );
		_Minimize->setIcon( QIcon( "SkinIcons:/images/window/icon_window_minimize.png" ) );
		_Minimize->setStyleSheet( QString::fromUtf8( "#minimizeButton{\n"
													 "  background-color:none;\n"
													 "  border:none;\n"
													 "}\n"
													 "#minimizeButton:hover{\n"
													 "  background-color:palette(alternate-base);\n"
													 "}\n"
													 "#minimizeButton:pressed{\n"
													 "  background-color:palette(highlight);\n"
													 "}" ) );
		setMinimizeButtonVisable( true );

		connect( _Minimize, &QToolButton::clicked, this, &MenuBar::MinimizeButtonClicked );
	}

	// maximize
	{
		_Maximize = new QToolButton( this );
		_Maximize->setObjectName( QString::fromUtf8( "maximizeButton" ) );
		_Maximize->setIcon( QIcon( "SkinIcons:/images/window/icon_window_maximize.png" ) );
		_Maximize->setStyleSheet( QString::fromUtf8( "#maximizeButton{\n"
													 "  background-color:none;\n"
													 "  border:none;\n"
													 "}\n"
													 "#maximizeButton:hover{\n"
													 "  background-color:palette(alternate-base);\n"
													 "}\n"
													 "##maximizeButton:pressed{\n"
													 "  background-color:palette(highlight);\n"
													 "}" ) );
		setMaximizeButtonVisable( true );

		connect( _Maximize, &QToolButton::clicked, this, &MenuBar::MaximizeButtonClicked );
	}

	// title
	{
		_Title = new QLabel( this );
		_Title->setObjectName( QString::fromUtf8( "Title" ) );
	}
}

XS::MenuBar::~MenuBar()
{
	setRestoreButtonVisable( false );
	setMinimizeButtonVisable( false );
	setMaximizeButtonVisable( false );
}

void XS::MenuBar::showRestoreButton()
{
	_Restore->show();
}

void XS::MenuBar::hideRestoreButton()
{
	_Restore->hide();
}

bool XS::MenuBar::RestoreButtonVisable() const
{
	return _Restore->userData( Qt::UserRole + 101 ) != nullptr;
}

void XS::MenuBar::setRestoreButtonVisable( bool val )
{
	_Restore->setUserData( Qt::UserRole + 101, val ? (QObjectUserData *)0xFFFFFFFFFFFFFFFF : nullptr );
}

void XS::MenuBar::showMinimizeButton()
{
	_Minimize->show();
}

void XS::MenuBar::hideMinimizeButton()
{
	_Minimize->hide();
}

bool XS::MenuBar::MinimizeButtonVisable() const
{
	return _Minimize->userData( Qt::UserRole + 101 ) != nullptr;
}

void XS::MenuBar::setMinimizeButtonVisable( bool val )
{
	_Minimize->setUserData( Qt::UserRole + 101, val ? (QObjectUserData *)0xFFFFFFFFFFFFFFFF : nullptr );
}

void XS::MenuBar::showMaximizeButton()
{
	_Maximize->show();
}

void XS::MenuBar::hideMaximizeButton()
{
	_Maximize->hide();
}

bool XS::MenuBar::MaximizeButtonVisable() const
{
	return _Maximize->userData( Qt::UserRole + 101 ) != nullptr;
}

void XS::MenuBar::setMaximizeButtonVisable( bool val )
{
	_Maximize->setUserData( Qt::UserRole + 101, val ? (QObjectUserData *)0xFFFFFFFFFFFFFFFF : nullptr );
}

void XS::MenuBar::paintEvent( QPaintEvent * event )
{
	QMenuBar::paintEvent( event );

	QRect rrect = rect();
	int right = rrect.width();

	if ( !_Close->isHidden() )
	{
		_Close->move( right - rrect.height(), 0 );
		_Close->resize( rrect.height(), rrect.height() );
		right -= rrect.height();
	}
	if ( !_Restore->isHidden() )
	{
		_Restore->move( right - rrect.height(), 0 );
		_Restore->resize( rrect.height(), rrect.height() );
		right -= rrect.height();
	}
	if ( !_Maximize->isHidden() )
	{
		_Maximize->move( right - rrect.height(), 0 );
		_Maximize->resize( rrect.height(), rrect.height() );
		right -= rrect.height();
	}
	if ( !_Minimize->isHidden() )
	{
		_Minimize->move( right - rrect.height(), 0 );
		_Minimize->resize( rrect.height(), rrect.height() );
		right -= rrect.height();
	}

	{
		int left = 0;
		auto fm = fontMetrics();
		int itemSpacing = style()->pixelMetric( QStyle::PM_MenuBarItemSpacing, 0, this );
		for ( const auto & it : actions() )
		{
			if ( it->isVisible() )
			{
				if ( !it->isSeparator() )
				{
					QStyleOptionMenuItem opt;
					initStyleOption( &opt, it );
					left += style()->sizeFromContents( QStyle::CT_MenuBarItem, &opt, fm.size( Qt::TextShowMnemonic, it->text() ), this ).width();
				}

				left += itemSpacing;
			}
		}

		QPainter p( this );
		QRect r( left + ( itemSpacing * 2 ), 0, fm.size( Qt::TextShowMnemonic, windowTitle() ).width(), rrect.height() );
		QStyleOptionMenuItem opt;
		{
			opt.palette = palette();
			opt.palette.setColor( QPalette::ButtonText, opt.palette.color( QPalette::Disabled, QPalette::ButtonText ) );
			opt.state = QStyle::State_None;
			opt.state |= QStyle::State_Enabled;
			opt.fontMetrics = fontMetrics();
			opt.menuRect = rect();
			opt.menuItemType = QStyleOptionMenuItem::Normal;
			opt.checkType = QStyleOptionMenuItem::NotCheckable;
			opt.text = windowTitle();
			opt.rect = r;
		}
		p.setClipRect( r );
		style()->drawControl( QStyle::CE_MenuBarItem, &opt, &p, this );
	}
}

void XS::MenuBar::mousePressEvent( QMouseEvent * event )
{
	if ( !window()->isMaximized() && actionAt( event->pos() ) == nullptr )
	{
		_MoveFlag = true;
		_MovePos = event->globalPos();
	}
	else
	{
		_MoveFlag = false;
		QMenuBar::mousePressEvent( event );
	}
}

void XS::MenuBar::mouseMoveEvent( QMouseEvent * event )
{
	if ( !window()->isMaximized() && actionAt( event->pos() ) == nullptr && _MoveFlag )
	{
		window()->move( window()->pos() + ( event->globalPos() - _MovePos ) );
		_MovePos = event->globalPos();
	}
	else
	{
		_MoveFlag = false;
		QMenuBar::mousePressEvent( event );
	}
}

void XS::MenuBar::mouseReleaseEvent( QMouseEvent * event )
{
	_MoveFlag = false;
	QMenuBar::mousePressEvent( event );
}

void XS::MenuBar::mouseDoubleClickEvent( QMouseEvent * event )
{
	if ( actionAt( event->pos() ) == nullptr && ( !_Maximize->isHidden() || !_Restore->isHidden() ) )
	{
		emit MenuBarDoubleClicked();
	}
}
