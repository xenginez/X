#include "FramelessWindow.h"

#include "ui_FramelessWindow.h"

#include "DockWidget.h"

XS::FramelessWindow::FramelessWindow( QWidget * parent )
	: QMainWindow( parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint ), ui( new Ui::FramelessWindow )
{
	ui->setupUi( this );

	_UndoGroup = new QUndoGroup( this );

	ui->menuBar->hideRestoreButton();
	ui->icon->setPixmap( QPixmap( "SkinIcons:/images/common/icon_title.png" ) );

	connect( ui->menuBar, &MenuBar::CloseButtonClicked, this, &FramelessWindow::OnCloseButtonClicked );
	connect( ui->menuBar, &MenuBar::MenuBarDoubleClicked, this, &FramelessWindow::OnMenuBarDoubleClicked );
	connect( ui->menuBar, &MenuBar::RestoreButtonClicked, this, &FramelessWindow::OnRestoreButtonClicked );
	connect( ui->menuBar, &MenuBar::MinimizeButtonClicked, this, &FramelessWindow::OnMinimizeButtonClicked );
	connect( ui->menuBar, &MenuBar::MaximizeButtonClicked, this, &FramelessWindow::OnMaximizeButtonClicked );
}

XS::FramelessWindow::~FramelessWindow()
{
	delete ui;
}

void XS::FramelessWindow::AddUndoStack( QUndoStack * val )
{
	_UndoGroup->addStack( val );
}

void XS::FramelessWindow::RemoveUndoStack( QUndoStack * val )
{
	_UndoGroup->removeStack( val );
}

bool XS::FramelessWindow::setContent( QWidget * w )
{
	if ( ui->centralwidget == nullptr )
	{
		ui->centralwidget = new QWidget( this );
		ui->centralwidget->setAutoFillBackground( true );
		QVBoxLayout * verticalLayout_2 = new QVBoxLayout( ui->centralwidget );
		verticalLayout_2->setSpacing( 0 );
		verticalLayout_2->setContentsMargins( 11, 11, 11, 11 );
		verticalLayout_2->setObjectName( QString::fromUtf8( "verticalLayout_2" ) );
		verticalLayout_2->setContentsMargins( 0, 0, 0, 0 );

		setCentralWidget( ui->centralwidget );
	}

	ui->centralwidget->layout()->addWidget( w );

	return true;
}

void XS::FramelessWindow::removeCentralwidget()
{
	ui->centralwidget->deleteLater();
	ui->centralwidget = nullptr;
}

void XS::FramelessWindow::setWindowIcon( const QIcon & ico )
{
	ui->menuBar->setIcon( ico.pixmap( 16, 16 ) );
}

void XS::FramelessWindow::setWindowTitle( const QString & text )
{
	ui->menuBar->setTitle( text );
}

void XS::FramelessWindow::enableWindowIcon()
{
	ui->menuBar->setIconVisable( true );
	ui->menuBar->showIcon();
}

void XS::FramelessWindow::disableWindowIcon()
{
	ui->menuBar->setIconVisable( false );
	ui->menuBar->hideIcon();
}

void XS::FramelessWindow::enableRestoreButton()
{
	ui->menuBar->setRestoreButtonVisable( true );
	ui->menuBar->showRestoreButton();
}

void XS::FramelessWindow::disableRestoreButton()
{
	ui->menuBar->setRestoreButtonVisable( false );
	ui->menuBar->hideRestoreButton();
}

void XS::FramelessWindow::enableMinimizeButton()
{
	ui->menuBar->setMinimizeButtonVisable( true );
	ui->menuBar->showMinimizeButton();
}

void XS::FramelessWindow::disableMinimizeButton()
{
	ui->menuBar->setMinimizeButtonVisable( false );
	ui->menuBar->hideMinimizeButton();
}

void XS::FramelessWindow::enableMaximizeButton()
{
	ui->menuBar->setMaximizeButtonVisable( true );
	ui->menuBar->showMaximizeButton();
}

void XS::FramelessWindow::disableMaximizeButton()
{
	ui->menuBar->setMaximizeButtonVisable( false );
	ui->menuBar->hideMaximizeButton();
}

void XS::FramelessWindow::OnCloseButtonClicked()
{
	close();
}

void XS::FramelessWindow::OnMenuBarDoubleClicked()
{
	if( windowState().testFlag( Qt::WindowNoState ) )
	{
		OnMaximizeButtonClicked();
	}
	else if( windowState().testFlag( Qt::WindowMaximized ) )
	{
		OnRestoreButtonClicked();
	}
}

void XS::FramelessWindow::OnRestoreButtonClicked()
{
	show();
}

void XS::FramelessWindow::OnMinimizeButtonClicked()
{
	showMinimized();
}

void XS::FramelessWindow::OnMaximizeButtonClicked()
{
	showMaximized();
}

void XS::FramelessWindow::show()
{
	if( ui->menuBar->MaximizeButtonVisable() )
	{
		ui->menuBar->showMaximizeButton();
	}

	ui->menuBar->hideRestoreButton();

	setWindowState( Qt::WindowNoState );

	QMainWindow::show();
}

void XS::FramelessWindow::showMinimized()
{
	QMainWindow::showMinimized();
}

void XS::FramelessWindow::showMaximized()
{
	if( ui->menuBar->RestoreButtonVisable() )
	{
		ui->menuBar->showRestoreButton();
	}

	ui->menuBar->hideMaximizeButton();

	setWindowState( Qt::WindowMaximized );

	QMainWindow::showMaximized();
}

void XS::FramelessWindow::Save( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		QStringList types;
		QStringList names;
		QList<XS::DockWidget *> docks = QWidget::findChildren<XS::DockWidget *>();

		for( XS::DockWidget * it : docks )
		{
			types.push_back( it->metaObject()->className() );
			names.push_back( it->objectName() );
		}

		settings.setValue( "docks", types );
		settings.setValue( "names", names );
		settings.setValue( "geometry", saveGeometry() );
		settings.setValue( "state", saveState() );

		for( XS::DockWidget * it : docks )
		{
			it->Save( settings );
		}
	}
	settings.endGroup();
}

void XS::FramelessWindow::Load( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		auto types = settings.value( "docks" ).toStringList();
		auto names = settings.value( "names" ).toStringList();

		QList<XS::DockWidget *> docks;
		for( int i = 0; i < types.count(); i++ )
		{
			XS::DockWidget * dock = XS::Registry::ConstructT<XS::DockWidget>( types[i], this );
			dock->setObjectName( names[i] );
			docks.push_back( dock );
			dock->show();
		}

		restoreGeometry( settings.value( "geometry" ).toByteArray() );
		restoreState( settings.value( "state" ).toByteArray() );

		for( XS::DockWidget * it : docks )
		{
			it->Load( settings );
		}
	}
	settings.endGroup();
}
