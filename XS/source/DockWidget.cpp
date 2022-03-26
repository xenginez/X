#include "DockWidget.h"

#include <QLabel>
#include <QDebug>
#include <QFrame>
#include <QEvent>
#include <QMainWindow>
#include <QToolButton>
#include <QSpacerItem>
#include <QHBoxLayout>

#include "Skin.h"
#include "FramelessWindow.h"

XS::DockWidget::DockWidget( QWidget * parent /*= nullptr */ )
	:QDockWidget( parent )
{
	_UndoStack = new QUndoStack( this );
	QString window_name( "XS::FramelessWindow" );
	while ( parent != nullptr )
	{
		if (parent->metaObject()->className() == window_name )
		{
			dynamic_cast<XS::FramelessWindow *>( parent )->AddUndoStack( _UndoStack );
			break;
		}
		else
		{
			parent = parent->parentWidget();
		}
	}

	QDockWidget::setFeatures( AllDockWidgetFeatures );
	QDockWidget::setAllowedAreas( Qt::AllDockWidgetAreas );

	connect( this, &QDockWidget::topLevelChanged, this, &XS::DockWidget::OnTopLevelChanged );
}

XS::DockWidget::~DockWidget()
{
	QWidget * parent = parentWidget();
	QString window_name( "XS::FramelessWindow" );
	while ( parent != nullptr )
	{
		if ( parent->metaObject()->className() == window_name )
		{
			dynamic_cast<XS::FramelessWindow *>( parent )->RemoveUndoStack( _UndoStack );
			break;
		}
		else
		{
			parent = parent->parentWidget();
		}
	}
}

void XS::DockWidget::setTitleBar( QWidget * title_bar )
{
	QFrame * bar = new QFrame( this );
	bar->setObjectName( "titleBar" );
	bar->setStyleSheet( "#titleBar{ background-color:palette(alternate-base); }" );

	QLabel * icon = new QLabel( bar );
	icon->setPixmap( windowIcon().pixmap( 16, 16 ) );
	icon->setFixedSize( 20, 20 );

	QLabel * title = new QLabel( bar );
	title->setText( windowTitle() );
	auto font = title->font();
	font.setBold( true );
	font.setPointSize( 8 );
	title->setFont( font );

	QSpacerItem * line = new QSpacerItem( 10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed );
	QSpacerItem * space = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );

	QToolButton * fixed = new QToolButton( bar );
	fixed->setCheckable( true );
	fixed->setFixedSize( 20, 20 );
	fixed->setIcon( QIcon( "SkinIcons:/images/window/icon_window_fixed.png" ) );

	QToolButton * flaot = new QToolButton( bar );
	flaot->setFixedSize( 20, 20 );
	flaot->setIcon( QIcon( "SkinIcons:/images/window/icon_window_restore.png" ) );

	QToolButton * close = new QToolButton( bar );
	close->setFixedSize( 20, 20 );
	close->setIcon( QIcon( "SkinIcons:/images/window/icon_window_close.png" ) );

	QHBoxLayout * layout = new QHBoxLayout( bar );
	layout->setContentsMargins( 0, 0, 0, 0 );
	layout->setSpacing( 1 );

	layout->addWidget( icon );
	layout->addWidget( title );
	layout->addSpacerItem( line );
	layout->addWidget( title_bar );
	layout->addSpacerItem( space );
	layout->addWidget( fixed );
	layout->addWidget( flaot );
	layout->addWidget( close );
	bar->setLayout( layout );

	bar->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

	setTitleBarWidget( bar );

	_IconLabel = icon;
	_FixedButton = fixed;
	_FloatButton = flaot;
	connect( fixed, &QToolButton::clicked, this, &DockWidget::OnFixedClicked );
	connect( flaot, &QToolButton::clicked, this, &DockWidget::OnFloatClicked );
	connect( close, &QToolButton::clicked, this, &DockWidget::OnCloseClicked );

	bar->installEventFilter( this );
}

void XS::DockWidget::OnTopLevelChanged( bool top )
{
	if( !top && _FixedButton != nullptr )
	{
		_FixedButton->show();
		_FloatButton->show();
	}
	else
	{
		_FloatButton->hide();
	}
}

void XS::DockWidget::OnFixedClicked( bool checked )
{
	if( checked )
	{
		QDockWidget::setFeatures( features() & ( ~DockWidgetMovable ) );

		_FloatButton->setEnabled( false );
		_FixedButton->setIcon( QIcon( "SkinIcons:/images/window/icon_window_unfixed.png" ) );
	}
	else
	{
		_FloatButton->setEnabled( true );
		QDockWidget::setFeatures( features() | DockWidgetMovable );
		_FixedButton->setIcon( QIcon( "SkinIcons:/images/window/icon_window_fixed.png" ) );
	}
}

void XS::DockWidget::OnFloatClicked( bool checked )
{
	QDockWidget::setFloating( true );
	_FixedButton->hide();
}

void XS::DockWidget::OnCloseClicked( bool checked )
{
	close();
}

void XS::DockWidget::Save( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		settings.setValue( "fixed", _FixedButton->isChecked() );
	}
	settings.endGroup();
}

void XS::DockWidget::Load( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		_FixedButton->clicked( settings.value( "fixed" ).toBool() );
	}
	settings.endGroup();
}

bool XS::DockWidget::eventFilter( QObject * watched, QEvent * event )
{
	if( watched == titleBarWidget() && event->type() == QEvent::MouseButtonDblClick )
	{
		if( isFloating() )
		{
			if( windowState().testFlag( Qt::WindowNoState ) )
			{
				setWindowState( Qt::WindowMaximized );

				showMaximized();

				return true;
			}
			else
			{
				setWindowState( Qt::WindowNoState );

				show();

				return true;
			}
		}
		else if( _FixedButton->isChecked() )
		{
			return true;
		}
	}

	return QDockWidget::eventFilter( watched, event );
}

void XS::DockWidget::PushUndoCommand( QUndoCommand * command )
{
	_UndoStack->push( command );
}
