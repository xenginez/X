#include "MainWindow.h"

#include <QFile>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QJsonDocument>

#include "MenuBar.h"
#include "StatusBar.h"
#include "DockWidget.h"

namespace Ui
{
	class MainWindow
	{
	public:
		QWidget * centralwidget;
		QVBoxLayout * verticalLayout_2;
		QLabel * icon;
		XS::StatusBar * statusBar;
		XS::MenuBar * menuBar;

		void setupUi( QMainWindow * MainWindow )
		{
			if ( MainWindow->objectName().isEmpty() )
				MainWindow->setObjectName( QString::fromUtf8( "MainWindow" ) );
			MainWindow->resize( 659, 558 );
			MainWindow->setMouseTracking( true );
			MainWindow->setStyleSheet( QString::fromUtf8( "#FramelessWindow\n"
														  "{\n"
														  "	border-radius:5px 5px 5px 5px;\n"
														  "}" ) );
			MainWindow->setDocumentMode( true );
			MainWindow->setTabShape( QTabWidget::Rounded );
			MainWindow->setDockNestingEnabled( true );
			MainWindow->setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks );
			centralwidget = new QWidget( MainWindow );
			centralwidget->setObjectName( QString::fromUtf8( "centralwidget" ) );
			centralwidget->setAutoFillBackground( true );
			verticalLayout_2 = new QVBoxLayout( centralwidget );
			verticalLayout_2->setSpacing( 0 );
			verticalLayout_2->setContentsMargins( 11, 11, 11, 11 );
			verticalLayout_2->setObjectName( QString::fromUtf8( "verticalLayout_2" ) );
			verticalLayout_2->setContentsMargins( 0, 0, 0, 0 );
			icon = new QLabel( centralwidget );
			icon->setObjectName( QString::fromUtf8( "icon" ) );
			icon->setScaledContents( false );
			icon->setAlignment( Qt::AlignCenter );

			verticalLayout_2->addWidget( icon );

			MainWindow->setCentralWidget( centralwidget );
			statusBar = new XS::StatusBar( MainWindow );
			statusBar->setObjectName( QString::fromUtf8( "statusBar" ) );
			statusBar->setStyleSheet( QString::fromUtf8( "#statusBar\n"
														 "{\n"
														 "	border: 0px none palette(base);\n"
														 "	background-color:palette(shadow);\n"
														 "}" ) );
			MainWindow->setStatusBar( statusBar );
			menuBar = new XS::MenuBar( MainWindow );
			menuBar->setObjectName( QString::fromUtf8( "menuBar" ) );
			menuBar->setGeometry( QRect( 0, 0, 659, 23 ) );
			QSizePolicy sizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
			sizePolicy.setHorizontalStretch( 0 );
			sizePolicy.setVerticalStretch( 0 );
			sizePolicy.setHeightForWidth( menuBar->sizePolicy().hasHeightForWidth() );
			menuBar->setSizePolicy( sizePolicy );
			menuBar->setMinimumSize( QSize( 0, 23 ) );
			menuBar->setMaximumSize( QSize( 16777215, 23 ) );
			menuBar->setFocusPolicy( Qt::StrongFocus );
			menuBar->setStyleSheet( QString::fromUtf8( "#menuBar\n"
													   "{\n"
													   "	border: 0px none palette(base);\n"
													   "	background-color:palette(dark);\n"
													   "}" ) );
			MainWindow->setMenuBar( menuBar );

			retranslateUi( MainWindow );

			QMetaObject::connectSlotsByName( MainWindow );
		} // setupUi

		void retranslateUi( QMainWindow * MainWindow )
		{
			icon->setText( QString() );
			Q_UNUSED( MainWindow );
		} // retranslateUi

	};
}

XS::MainWindow::MainWindow( QWidget * parent )
	: QMainWindow( parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint ), ui( new Ui::MainWindow ), _MainThreadID( std::this_thread::get_id() )
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

std::thread::id XS::MainWindow::GetEditorThreadId() const
{
	return _MainThreadID;
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

void XS::MainWindow::Save()
{
	SaveLayout();
	SaveShortcuts();
}

void XS::MainWindow::Load()
{
	LoadLayout();
	LoadShortcuts();
}

void XS::MainWindow::SaveLayout()
{
	QSettings settings( QApplication::applicationDirPath() + "/layout.ini", QSettings::IniFormat );

	settings.clear();

	settings.beginGroup( objectName() );
	{
		QStringList types;
		QList<XS::DockWidget *> docks = QWidget::findChildren<XS::DockWidget *>();

		for ( XS::DockWidget * it : docks )
		{
			types.push_back( it->metaObject()->className() );
		}

		settings.setValue( "docks", types );
		settings.setValue( "state", saveState() );
		settings.setValue( "geometry", saveGeometry() );
		settings.setValue( "maximized", QMainWindow::isMaximized() );

		for ( XS::DockWidget * it : docks )
		{
			it->SaveLayout( settings );
		}
	}
	settings.endGroup();

}

void XS::MainWindow::LoadLayout()
{
	QSettings settings( QApplication::applicationDirPath() + "/layout.ini", QSettings::IniFormat );

	settings.beginGroup( objectName() );
	{
		if ( settings.value( "maximized" ).toBool() )
		{
			showMaximized();
		}
		else
		{
			show();
		}

		QApplication::processEvents();

		auto types = settings.value( "docks" ).toStringList();
		for ( int i = 0; i < types.count(); i++ )
		{
			XS::DockWidget * dock = XS::Registry::ConstructT<XS::DockWidget>( types[i], this );
			dock->LoadLayout( settings );
			dock->show();
		}

		QApplication::processEvents();

		restoreGeometry( settings.value( "geometry" ).toByteArray() );
		restoreState( settings.value( "state" ).toByteArray() );
	}
	settings.endGroup();

}

void XS::MainWindow::SaveShortcuts()
{
	auto path = QApplication::applicationDirPath() + "/shortcuts.json";

	QJsonObject obj;
	{
		for ( auto it = _KeySequences.begin(); it != _KeySequences.end(); ++it )
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

void XS::MainWindow::LoadShortcuts()
{
	auto path = QApplication::applicationDirPath() + "/shortcuts.json";

	QFile file( path );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );

		auto obj = doc.object();

		for ( auto it = obj.begin(); it != obj.end(); ++it )
		{
			_KeySequences.insert( it.key(), QKeySequence::fromString( it.value().toString() ) );
		}
	}
}

QShortcut * XS::MainWindow::AddShortcuts( const QString & name, const QKeySequence & key, QWidget * widget )
{
	auto it = _KeySequences.find( name );
	if ( it != _KeySequences.end() )
	{
		auto it2 = _Shortcuts.find( name );
		if ( it2 != _Shortcuts.end() )
		{
			return it2.value();
		}
		else
		{
			auto shortcut = new QShortcut( it.value(), widget, nullptr, nullptr, Qt::ShortcutContext::WidgetWithChildrenShortcut );
			_Shortcuts.insert( name, shortcut );
			return shortcut;
		}
	}
	else
	{
		auto shortcut = new QShortcut( key, widget, nullptr, nullptr, Qt::ShortcutContext::WidgetWithChildrenShortcut );

		_KeySequences.insert( name, key );
		_Shortcuts.insert( name, shortcut );

		return shortcut;
	}
}
