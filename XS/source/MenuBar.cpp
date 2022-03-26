#include "MenuBar.h"

#include <QDebug>
#include <QStyle>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QStyleOption>

#include "Skin.h"

#define ICON_SIZE ( 16 )
#define SPACE_SIZE ( 10 )
#define TITLE_SPACE_SIZE ( 10 )

XS::MenuBar::MenuBar( QWidget * parent /*= nullptr */ )
	:QMenuBar( parent )
{
	// icon
	{
		_Icon = new QLabel( this );
		_Icon->setObjectName( QString::fromUtf8( "icon" ) );
		_Icon->setMinimumSize( QSize( ICON_SIZE, ICON_SIZE ) );
		_Icon->setMaximumSize( QSize( ICON_SIZE, ICON_SIZE ) );
		_Icon->setContextMenuPolicy( Qt::NoContextMenu );
		_Icon->setStyleSheet( QString::fromUtf8( "#icon {background-color:palette(shadow);}" ) );
		_Icon->setPixmap( QApplication::style()->standardIcon( QStyle::SP_DesktopIcon ).pixmap( ICON_SIZE, ICON_SIZE ) );
		setIconVisable( true );
	}

	// title
	{
		_Title = new QLabel( this );
		_Title->setObjectName( QString::fromUtf8( "title" ) );
		QFont font;
		font.setBold( true );
		font.setWeight( 75 );
		_Title->setFont( font );
		_Title->setText( "Welcome X Studio" );
		_Title->setAlignment( Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter );
		_Title->setStyleSheet( QString::fromUtf8( "padding-left:5px;" ) );

		_Title->installEventFilter( this );
	}

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

	// actions list
	{
		for( int i = 0; i < 10; i++ )
		{
			auto tool = new QToolButton( this );
			tool->setObjectName( QString::fromUtf8( "toolButton_%0" ).arg( i + 2 ) );
			tool->setStyleSheet( QString::fromUtf8( "#%0{ background-color:none; border:none; } #%0:hover{ background-color:palette(alternate-base); }" )
								 .arg( tool->objectName() ) );
			_Actions.push_back( tool );
		}
	}
}

XS::MenuBar::~MenuBar()
{
	setIconVisable( false );
	setRestoreButtonVisable( false );
	setMinimizeButtonVisable( false );
	setMaximizeButtonVisable( false );

	for( auto & it : _Actions )
	{
		it->setUserData( 0, nullptr );
	}
	_Actions.clear();
}

QPixmap XS::MenuBar::icon() const
{
	return *_Icon->pixmap();
}

void XS::MenuBar::setIcon( const QPixmap & val )
{
	_Icon->setPixmap( val );
}

QString XS::MenuBar::title() const
{
	return _Title->text();
}

void XS::MenuBar::setTitle( const QString & val )
{
	_Title->setText( val );
}

void XS::MenuBar::showIcon()
{
	_Icon->show();
}

void XS::MenuBar::hideIcon()
{
	_Icon->hide();
}

bool XS::MenuBar::IconVisable() const
{
	return _Icon->userData( Qt::UserRole + 101 ) != nullptr;
}

void XS::MenuBar::setIconVisable( bool val )
{
	_Icon->setUserData( Qt::UserRole + 101, val ? (QObjectUserData *)0xFFFFFFFFFFFFFFFF : nullptr );
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
	const QRect & rrect = rect();
	int left = 0;
	int right = rrect.width();

	if( !_Icon->isHidden() )
	{
		left += SPACE_SIZE;
		_Icon->move( left, ( rrect.height() - _Icon->width() ) / 2 );
		left += _Icon->width() + SPACE_SIZE;
	}
	if( !_Close->isHidden() )
	{
		_Close->move( right - rect().height(), 0 );
		_Close->resize( rect().height(), rect().height() );
		right -= rect().height();
	}
	if( !_Restore->isHidden() )
	{
		_Restore->move( right - rect().height(), 0 );
		_Restore->resize( rect().height(), rect().height() );
		right -= rect().height();
	}
	if( !_Maximize->isHidden() )
	{
		_Maximize->move( right - rect().height(), 0 );
		_Maximize->resize( rect().height(), rect().height() );
		right -= rect().height();
	}
	if( !_Minimize->isHidden() )
	{
		_Minimize->move( right - rect().height(), 0 );
		_Minimize->resize( rect().height(), rect().height() );
		right -= rect().height();
	}

	{
		int i = 0;
		auto list = actions();

		for( ; i < list.count(); i++ )
		{
			if( _Actions.count() <= i )
			{
				QToolButton * tool = new QToolButton( this );
				_Actions.push_back( tool );
			}

			_Actions[i]->show();

			auto action = (QAction *)_Actions[i]->userData( 0 );

			if( action == nullptr || action != list[i] )
			{
				auto tool = _Actions[i];

				if( action != nullptr )
				{
					tool->disconnect();
				}

				tool->setText( list[i]->text() );
				tool->setIcon( list[i]->icon() );
				tool->setUserData( 0, (QObjectUserData *)list[i] );
				connect( tool, &QToolButton::clicked, list[i], [this, i]()
				{ 
					auto action = (QAction *)_Actions[i]->userData( 0 );

					if( activeAction() != action )
					{
						setActiveAction( action );
					}
				} );
			}

			auto rect = actionRect( list[i] );
			_Actions[i]->move( rect.left(), 0 );
			_Actions[i]->resize( rect.width(), rect.height() );

			if( list[i]->menu() != nullptr )
			{
				auto p = mapToGlobal( { rect.left(), rect.bottom() } );
				list[i]->menu()->move( p );
			}

			left = rect.left() + rect.width();
		}

		for( int j = i; j < _Actions.count(); j++ )
		{
			_Actions[j]->hide();
		}
	}

	{
		QPainter p( this );
		QRegion emptyArea( rect() );

		//draw border
		if( int fw = style()->pixelMetric( QStyle::PM_MenuBarPanelWidth, 0, this ) )
		{
			QRegion borderReg;
			borderReg += QRect( 0, 0, fw, height() ); //left
			borderReg += QRect( width() - fw, 0, fw, height() ); //right
			borderReg += QRect( 0, 0, width(), fw ); //top
			borderReg += QRect( 0, height() - fw, width(), fw ); //bottom
			p.setClipRegion( borderReg );
			emptyArea -= borderReg;
			QStyleOptionFrame frame;
			frame.rect = rect();
			frame.palette = palette();
			frame.state = QStyle::State_None;
			frame.lineWidth = style()->pixelMetric( QStyle::PM_MenuBarPanelWidth );
			frame.midLineWidth = 0;
			style()->drawPrimitive( QStyle::PE_PanelMenuBar, &frame, &p, this );
		}
		p.setClipRegion( emptyArea );
		QStyleOptionMenuItem menuOpt;
		menuOpt.palette = palette();
		menuOpt.state = QStyle::State_None;
		menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
		menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
		menuOpt.rect = rect();
		menuOpt.menuRect = rect();
		style()->drawControl( QStyle::CE_MenuBarEmptyArea, &menuOpt, &p, this );
	}

	if( !_Title->text().isEmpty() )
	{
		left += SPACE_SIZE;
		_Title->setToolTip( toolTip() );
		_Title->move( left, 0 );
		_Title->resize( right - left, rrect.height() );
	}
}

void XS::MenuBar::mousePressEvent( QMouseEvent * event )
{
	setActiveAction( actionAt( event->pos() ) );

	QWidget::mousePressEvent( event );
}

void XS::MenuBar::mouseMoveEvent( QMouseEvent * event )
{
	auto active = activeAction();

	if( active != nullptr )
	{
		auto action = actionAt( event->pos() );

		if( active != action )
		{
			setActiveAction( action );
		}

		update();
	}

	QWidget::mouseMoveEvent( event );
}

void XS::MenuBar::mouseReleaseEvent( QMouseEvent * event )
{
	setActiveAction( nullptr );

	QWidget::mouseReleaseEvent( event );
}

QRect XS::MenuBar::actionRect( QAction * action ) const
{
	auto rect = actionGeometry( action );

	int offset = 0;

	if( !_Icon->isHidden() )
	{
		offset += _Icon->width() + SPACE_SIZE;
	}

	return { rect.left() + SPACE_SIZE + offset, rect.top(), rect.width(), rect.height() };
}

QAction * XS::MenuBar::actionAt( const QPoint & val ) const
{
	int offset = 0;

	if( !_Icon->isHidden() )
	{
		offset += _Icon->width() + SPACE_SIZE;
	}

	return QMenuBar::actionAt( QPoint( val.x() - SPACE_SIZE - offset, val.y() ) );
}

bool XS::MenuBar::eventFilter( QObject * obj, QEvent * event )
{
	if( obj == _Title )
	{
		if( parentWidget()->windowState().testFlag( Qt::WindowMaximized ) )
		{
			_MoveFlag = false;
		}
		else if( event->type() == QEvent::MouseButtonPress )
		{
			_MoveFlag = true;
			_Pos = ( (QMouseEvent *)( event ) )->globalPos();
			_WPos = parentWidget()->pos();
		}
		else if( event->type() == QEvent::MouseMove && _MoveFlag )
		{
			parentWidget()->move( _WPos + ( ( (QMouseEvent *)( event ) )->globalPos() - _Pos ) );
		}
		else if( event->type() == QEvent::MouseButtonRelease && _MoveFlag )
		{
			_MoveFlag = false;
		}
		
		if( event->type() == QEvent::MouseButtonDblClick && ( !_Maximize->isHidden() || !_Restore->isHidden() ) )
		{
			emit MenuBarDoubleClicked();
		}
	}

	return QMenuBar::eventFilter( obj, event );
}
