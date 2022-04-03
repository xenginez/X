#include <QApplication>
#include <QDesktopWidget>

#include "EditorWindow.h"

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

	QApplication a( argc, argv );

	a.setStyle( new XS::Skin( QApplication::applicationDirPath() + "/resource/qss/darkstyle.qss" ) );

	XS::CoreFramework framework;
	XStudio::EditorWindow window( project );
	
	window.show();
	framework.Exec( XE::MakeShared< XS::WindowProxy >( &window ), project );

	return a.exec();
}
