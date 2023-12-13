#include <QApplication>
#include <QDesktopWidget>

#include "MainWindow.h"

int main( int argc, char * argv[] )
{
	const char * project = nullptr;

	if( argc < 2 )
	{
		project = "D:\\TestProj";
	}
	else
	{
		project = argv[1];
	}

	qputenv( "QT_SCALE_FACTOR", QString::number( XE::Platform::GetDpiFactor() ).toUtf8() );
	QApplication::setAttribute( Qt::AA_EnableHighDpiScaling );


	QApplication a( argc, argv );
	XS::CoreFramework framework;

	auto skin = new XS::Skin( QApplication::applicationDirPath() + "/resource/qss/darkstyle.qss" );
	a.setStyle( skin );

	XStudio::MainWindow window( project );

	framework.Exec( XE::MakeShared< XS::WindowProxy >( &window ), project );

	window.showMaximized();

	int result = a.exec();

	framework.WaitExit();

	return result;
}
