#include <QApplication>

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

	XStudio::EditorWindow window( project );

	window.show();

	return a.exec();
}
