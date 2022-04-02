#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>

#include "DockWidget.h"

XS::MainWindow::MainWindow( QWidget * parent )
	: QMainWindow( parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint ), ui( new Ui::MainWindow )
{
	ui->setupUi( this );

	_UndoGroup = new QUndoGroup( this );

	ui->menuBar->hideRestoreButton();
	ui->icon->setPixmap( QPixmap( "SkinIcons:/images/common/icon_title.png" ) );

	connect( ui->menuBar, &MenuBar::CloseButtonClicked, this, &MainWindow::OnCloseButtonClicked );
	connect( ui->menuBar, &MenuBar::MenuBarDoubleClicked, this, &MainWindow::OnMenuBarDoubleClicked );
	connect( ui->menuBar, &MenuBar::RestoreButtonClicked, this, &MainWindow::OnRestoreButtonClicked );
	connect( ui->menuBar, &MenuBar::MinimizeButtonClicked, this, &MainWindow::OnMinimizeButtonClicked );
	connect( ui->menuBar, &MenuBar::MaximizeButtonClicked, this, &MainWindow::OnMaximizeButtonClicked );
}

XS::MainWindow::~MainWindow()
{
	delete ui;
}

void XS::MainWindow::AddUndoStack( QUndoStack * val )
{
	_UndoGroup->addStack( val );
}

void XS::MainWindow::RemoveUndoStack( QUndoStack * val )
{
	_UndoGroup->removeStack( val );
}

bool XS::MainWindow::SetContent( QWidget * w )
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

void XS::MainWindow::SetWindowIcon( const QIcon & ico )
{
	QMainWindow::setWindowIcon( ico );

	ui->menuBar->setWindowIcon( ico );
}

void XS::MainWindow::SetWindowTitle( const QString & text )
{
	QMainWindow::setWindowTitle( text );

	ui->menuBar->setWindowTitle( text );
}

void XS::MainWindow::RemoveCentralwidget()
{
	ui->centralwidget->deleteLater();
	ui->centralwidget = nullptr;
}

void XS::MainWindow::enableRestoreButton()
{
	ui->menuBar->setRestoreButtonVisable( true );
	ui->menuBar->showRestoreButton();
}

void XS::MainWindow::disableRestoreButton()
{
	ui->menuBar->setRestoreButtonVisable( false );
	ui->menuBar->hideRestoreButton();
}

void XS::MainWindow::enableMinimizeButton()
{
	ui->menuBar->setMinimizeButtonVisable( true );
	ui->menuBar->showMinimizeButton();
}

void XS::MainWindow::disableMinimizeButton()
{
	ui->menuBar->setMinimizeButtonVisable( false );
	ui->menuBar->hideMinimizeButton();
}

void XS::MainWindow::enableMaximizeButton()
{
	ui->menuBar->setMaximizeButtonVisable( true );
	ui->menuBar->showMaximizeButton();
}

void XS::MainWindow::disableMaximizeButton()
{
	ui->menuBar->setMaximizeButtonVisable( false );
	ui->menuBar->hideMaximizeButton();
}

void XS::MainWindow::OnCloseButtonClicked()
{
	close();
}

void XS::MainWindow::OnMenuBarDoubleClicked()
{
	if ( QMainWindow::isMaximized() )
	{
		OnRestoreButtonClicked();
	}
	else
	{
		OnMaximizeButtonClicked();
	}
}

void XS::MainWindow::OnRestoreButtonClicked()
{
	show();
}

void XS::MainWindow::OnMinimizeButtonClicked()
{
	showMinimized();
}

void XS::MainWindow::OnMaximizeButtonClicked()
{
	showMaximized();
}

void XS::MainWindow::show()
{
	if( ui->menuBar->MaximizeButtonVisable() )
	{
		ui->menuBar->showMaximizeButton();
	}

	ui->menuBar->hideRestoreButton();

	QMainWindow::showNormal();
}

void XS::MainWindow::showMinimized()
{
	QMainWindow::showMinimized();
}

void XS::MainWindow::showMaximized()
{
	if( ui->menuBar->RestoreButtonVisable() )
	{
		ui->menuBar->showRestoreButton();
	}

	ui->menuBar->hideMaximizeButton();

	QMainWindow::showMaximized();
}

void XS::MainWindow::Save( QSettings & settings )
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
		settings.setValue( "state", saveState() );
		settings.setValue( "geometry", saveGeometry() );

		for( XS::DockWidget * it : docks )
		{
			it->Save( settings );
			settings.setValue( it->objectName() + "/size", it->size() );
		}
	}
	settings.endGroup();
}

void XS::MainWindow::Load( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		XS::DockWidget * docks[255];

		auto types = settings.value( "docks" ).toStringList();
		auto names = settings.value( "names" ).toStringList();

		for ( int i = 0; i < types.count(); i++ )
		{
			XS::DockWidget * dock = XS::Registry::ConstructT<XS::DockWidget>( types[i], this );
			dock->setObjectName( names[i] );
			dock->Load( settings );
			dock->show();

			docks[i] = dock;
		}

		restoreGeometry( settings.value( "geometry" ).toByteArray() );
		restoreState( settings.value( "state" ).toByteArray() );

		QApplication::processEvents();

		for( int i = 0; i < types.count(); i++ )
		{
			QSize size = settings.value( docks[i]->objectName() + "/size" ).toSize();
			docks[i]->setMinimumSize( size );
			docks[i]->setMaximumSize( size );
		} 

		QApplication::processEvents();

		for ( int i = 0; i < types.count(); i++ )
		{
			docks[i]->setMinimumSize( QSize( 0, 0 ) );
			docks[i]->setMaximumSize( QSize( 65536, 65536 ) );
		}

		QApplication::processEvents();

		if ( QMainWindow::isMaximized() )
		{
			if ( ui->menuBar->RestoreButtonVisable() )
			{
				ui->menuBar->showRestoreButton();
			}
			ui->menuBar->hideMaximizeButton();
		}
		else
		{
			if ( ui->menuBar->MaximizeButtonVisable() )
			{
				ui->menuBar->showMaximizeButton();
			}
			ui->menuBar->hideRestoreButton();
		}
	}
	settings.endGroup();
}

void XS::MainWindow::SaveShortcuts( const QString & path )
{
	QJsonObject obj;
	{
		for ( auto it = _Shortcuts.begin(); it != _Shortcuts.end(); ++it )
		{
			obj.insert( it.key(), it.value().toString() );
		}
	}
	QJsonDocument doc( obj );

	QFile file( path );
	if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		file.write( doc.toJson( QJsonDocument::Indented ) );
	}
}

void XS::MainWindow::LoadShortcuts( const QString & path )
{
	QFile file( path );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );

		auto obj = doc.object();

		for ( auto it = obj.begin(); it != obj.end(); ++it )
		{
			_Shortcuts.insert( it.key(), QKeySequence::fromString( it.value().toString() ) );
		}
	}
}

QShortcut * XS::MainWindow::AddShortcuts( const QString & name, const QKeySequence & key, QWidget * widget )
{
	auto it = _Shortcuts.find( name );
	if ( it != _Shortcuts.end() )
	{
		return new QShortcut( it.value(), widget, nullptr, nullptr, Qt::ShortcutContext::WidgetWithChildrenShortcut );
	}
	else
	{
		_Shortcuts.insert( name, key );
		return new QShortcut( key, widget, nullptr, nullptr, Qt::ShortcutContext::WidgetWithChildrenShortcut );
	}
}
