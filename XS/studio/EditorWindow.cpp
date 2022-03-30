#include "EditorWindow.h"

#include <QTimer>
#include <QFileInfo>
#include <QDataStream>

XStudio::EditorWindow::EditorWindow( const QString & project, QWidget * parent /*= nullptr */ )
	:XS::FramelessWindow( parent )
{
	setObjectName( "XStudio" );

	setWindowTitle( project );

	removeCentralwidget();
}

XStudio::EditorWindow::~EditorWindow()
{

}

void XStudio::EditorWindow::showEvent( QShowEvent * e )
{
	XS::FramelessWindow::showEvent( e );

	if ( QFileInfo::exists( "./layout.ini" ) )
	{
		QSettings settings( "./layout.ini", QSettings::IniFormat );

		Load( settings );
	}
	else
	{
		auto objects = new XS::ObjectsDockWidget( this ); objects->show();
		auto assets = new XS::AssetsDockWidget( this ); assets->show();
		auto edit = new XS::EditSceneDockWidget( this ); edit->show();
		auto game = new XS::GameSceneDockWidget( this ); game->show();
		auto log = new XS::LoggerDockWidget( this ); log->show();

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
	QSettings settings( "./layout.ini", QSettings::IniFormat );

	Save( settings );

	XS::FramelessWindow::closeEvent( e );
}
