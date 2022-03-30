#include <QApplication>
#include <QDesktopWidget>

#include "EditorWindow.h"

int main( int argc, char * argv[] )
{
	QString project;

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

	XStudio::EditorWindow EditorWindow( project );

	EditorWindow.setGeometry( QApplication::desktop()->screenGeometry( 0 ) );

	EditorWindow.show();

	return a.exec();
}
