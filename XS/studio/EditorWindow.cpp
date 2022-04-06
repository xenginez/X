#include "EditorWindow.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QFileInfo>
#include <QDataStream>
#include <QApplication>
#include <QDesktopWidget>

XStudio::EditorWindow::EditorWindow( const QString & project, QWidget * parent /*= nullptr */ )
	:XS::MainWindow( parent )
{
	setObjectName( "XStudio" );

	SetWindowTitle( project );

	QMenu * e_file = new QMenu( tr( "&File" ) );
	QAction * a_file = new QAction( "&11111" ); a_file->setShortcut( QKeySequence( "ALT + F" ) );
	e_file->addAction( a_file );
	QMenu * e_edit = new QMenu( tr( "&Edit" ) ); e_edit->addAction( new QAction( "11111" ) );
	QMenu * e_view = new QMenu( tr( "&View" ) ); e_view->addAction( new QAction( "11111" ) );
	QMenu * e_plug = new QMenu( tr( "&Plugin" ) ); e_plug->addAction( new QAction( "11111" ) );
	QMenu * e_help = new QMenu( tr( "&Help" ) ); e_help->addAction( new QAction( "11111" ) );

	menuBar()->addMenu( e_file );
	menuBar()->addMenu( e_edit );
	menuBar()->addMenu( e_view );
	menuBar()->addMenu( e_plug );
	menuBar()->addMenu( e_help );

	RemoveCentralwidget();
}

XStudio::EditorWindow::~EditorWindow()
{

}

void XStudio::EditorWindow::showEvent( QShowEvent * e )
{
	XS::MainWindow::showEvent( e );

	if ( QFileInfo::exists( "./layout.ini" ) )
	{
		Load();
	}
	else
	{
		auto objects = XS::Registry::ConstructT<XS::DockWidget>( "XS::ObjectsEditor", this ); objects->show();
		auto assets = XS::Registry::ConstructT<XS::DockWidget>( "XS::AssetsEditor", this ); assets->show();
		auto edit = XS::Registry::ConstructT<XS::DockWidget>( "XS::EditSceneEditor", this ); edit->show();
		auto game = XS::Registry::ConstructT<XS::DockWidget>( "XS::GameSceneEditor", this ); game->show();
		auto log = XS::Registry::ConstructT<XS::DockWidget>( "XS::LoggerEditor", this ); log->show();

		addDockWidget( Qt::LeftDockWidgetArea, edit );
		addDockWidget( Qt::RightDockWidgetArea, objects );
		QMainWindow::resizeDocks( { edit, objects }, { int( width() * 0.7f ), int( width() * 0.3f ) }, Qt::Horizontal );

		addDockWidget( Qt::LeftDockWidgetArea, assets );
		QMainWindow::splitDockWidget( edit, assets, Qt::Vertical );
		QMainWindow::resizeDocks( { edit, assets }, { int( height() * 0.7f ), int( height() * 0.3f ) }, Qt::Vertical );

		addDockWidget( Qt::LeftDockWidgetArea, game );
		QMainWindow::tabifyDockWidget( edit, game );
		QMainWindow::setTabPosition( Qt::LeftDockWidgetArea, QTabWidget::South );

		addDockWidget( Qt::LeftDockWidgetArea, log );
		QMainWindow::tabifyDockWidget( assets, log );
		QMainWindow::setTabPosition( Qt::LeftDockWidgetArea, QTabWidget::South );

		edit->raise();
		assets->raise();
	}
}

void XStudio::EditorWindow::closeEvent( QCloseEvent * e )
{
	Save();

	XS::MainWindow::closeEvent( e );
}
