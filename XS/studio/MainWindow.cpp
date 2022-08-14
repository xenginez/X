#include "MainWindow.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QFileInfo>
#include <QMetaMethod>
#include <QDataStream>
#include <QApplication>
#include <QDesktopWidget>

XStudio::MainWindow::MainWindow( const QString & project, QWidget * parent /*= nullptr */ )
	:XS::MainWindow( parent )
{
	setObjectName( "XStudio" );

	SetWindowTitle( project );

	QMenu * e_file = new QMenu( tr( "&File" ) );
	{
		e_file->addAction( new QAction( "&11111" ) );
	}
	QMenu * e_edit = new QMenu( tr( "&Edit" ) );
	{
		e_edit->addAction( new QAction( "11111" ) );
	}
	QMenu * e_wind = new QMenu( tr( "&Window" ) );
	{
		auto metas = XS::Registry::GetDerivedClass( &XS::ToolEditorWindow::staticMetaObject );
		
		for ( auto meta : metas )
		{
			QString name = XS::QMetaStaticCall< QString >( meta, "name()" );
			if ( !name.isEmpty() )
			{
				QIcon icon = XS::QMetaStaticCall< QIcon >( meta, "icon()" );

				e_wind->addAction( icon, name, [this, meta]()
				{
					auto childs = this->children();
					auto it = std::find_if( childs.begin(), childs.end(), [&]( QObject * obj ) { return obj->metaObject() == meta; } );
					if ( it == childs.end() )
					{
						XS::Registry::ConstructT< XS::DockWidget >( meta->className(), this )->showMaximized();
					}
					else
					{
						static_cast<XS::DockWidget *>( *it )->raise();
					}
				} );
			}
		}
	}
	QMenu * e_plug = new QMenu( tr( "&Plugin" ) );
	{
		e_plug->addAction( new QAction( "11111" ) );
	}
	QMenu * e_help = new QMenu( tr( "&Help" ) );
	{
		e_help->addAction( new QAction( "11111" ) );
	}

	menuBar()->addMenu( e_file );
	menuBar()->addMenu( e_edit );
	menuBar()->addMenu( e_wind );
	menuBar()->addMenu( e_plug );
	menuBar()->addMenu( e_help );

	RemoveCentralwidget();
}

XStudio::MainWindow::~MainWindow()
{

}

void XStudio::MainWindow::showEvent( QShowEvent * e )
{
	XS::MainWindow::showEvent( e );

	QMetaObject::invokeMethod( this, [this]()
	{
		if ( QFileInfo::exists( "./layout.ini" ) )
		{
			Load();
		}
		else
		{
			auto asset = XS::Registry::ConstructT<XS::DockWidget>( "XS::AssetExplorerEditor", this ); asset->showMaximized();
			auto world = XS::Registry::ConstructT<XS::DockWidget>( "XS::WorldEditor", this ); world->showMaximized();
			auto edit = XS::Registry::ConstructT<XS::DockWidget>( "XS::EditSceneEditor", this ); edit->showMaximized();
			auto game = XS::Registry::ConstructT<XS::DockWidget>( "XS::GameSceneEditor", this ); game->showMaximized();
			auto log = XS::Registry::ConstructT<XS::DockWidget>( "XS::LoggerEditor", this ); log->showMaximized();

			QMetaObject::invokeMethod( this, [=]()
			{
				addDockWidget( Qt::LeftDockWidgetArea, edit );
				addDockWidget( Qt::RightDockWidgetArea, world );
				QMainWindow::resizeDocks( { edit, world }, { int( width() * 0.7f ), int( width() * 0.3f ) }, Qt::Horizontal );

				addDockWidget( Qt::LeftDockWidgetArea, asset );
				QMainWindow::splitDockWidget( edit, asset, Qt::Vertical );
				QMainWindow::resizeDocks( { edit, asset }, { int( height() * 0.7f ), int( height() * 0.3f ) }, Qt::Vertical );

				addDockWidget( Qt::LeftDockWidgetArea, game );
				QMainWindow::tabifyDockWidget( edit, game );
				QMainWindow::setTabPosition( Qt::LeftDockWidgetArea, QTabWidget::South );

				addDockWidget( Qt::LeftDockWidgetArea, log );
				QMainWindow::tabifyDockWidget( asset, log );
				QMainWindow::setTabPosition( Qt::LeftDockWidgetArea, QTabWidget::South );

				edit->raise();
				asset->raise();
			}, Qt::QueuedConnection );
		}
	}, Qt::QueuedConnection );
}

void XStudio::MainWindow::closeEvent( QCloseEvent * e )
{
	Save();

	XS::MainWindow::closeEvent( e );
}
