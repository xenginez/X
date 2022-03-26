#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

#include "WelcomeWindow.h"

int main( int argc, char * argv[] )
{
	QApplication a( argc, argv );

	QApplication::setStyle( new XS::Skin( QApplication::applicationDirPath() + "/resource/qss/darkstyle.qss" ) );

	XS::FramelessWindow FramelessWindow;

	FramelessWindow.removeCentralwidget();
	FramelessWindow.setContent( new Launcher::WelcomeWindow );

	FramelessWindow.setWindowTitle( "Welcome X Studio" );
	FramelessWindow.disableWindowIcon();
	FramelessWindow.disableRestoreButton();
	FramelessWindow.disableMinimizeButton();
	FramelessWindow.disableMaximizeButton();
	FramelessWindow.show();

	auto desktop_rect = QApplication::desktop()->screenGeometry( 0 );

	FramelessWindow.setGeometry( QRect( desktop_rect.width() * 0.25f, desktop_rect.height() * 0.2f, desktop_rect.width() * 0.5f, desktop_rect.height() * 0.6f ) );

	return a.exec();
}
