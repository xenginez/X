#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

#include "WelcomeWindow.h"

int main( int argc, char * argv[] )
{
	qputenv( "QT_SCALE_FACTOR", QString::number( XE::Platform::GetDpiFactor() ).toUtf8() );
	QApplication::setAttribute( Qt::AA_EnableHighDpiScaling );

	QApplication a( argc, argv );

	QApplication::setStyle( new XS::Skin( QApplication::applicationDirPath() + "/resource/qss/darkstyle.qss" ) );

	XS::MainWindow MainWindow;

	MainWindow.RemoveCentralwidget();
	MainWindow.SetContent( new Launcher::WelcomeWindow );

	MainWindow.SetWindowTitle( "Welcome X Studio" );
	MainWindow.disableRestoreButton();
	MainWindow.disableMinimizeButton();
	MainWindow.disableMaximizeButton();
	MainWindow.show();

	auto desktop_rect = QApplication::desktop()->screenGeometry( 0 );

	MainWindow.setGeometry( QRect( desktop_rect.width() * 0.25f, desktop_rect.height() * 0.2f, desktop_rect.width() * 0.5f, desktop_rect.height() * 0.6f ) );

	return a.exec();
}
