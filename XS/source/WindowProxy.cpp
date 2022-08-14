#include "WindowProxy.h"

#include "MainWindow.h"

XS::WindowProxy::WindowProxy( XS::MainWindow * window )
#if PLATFORM_OS & OS_LINUX
	: XE::Window( XE::HandleCast< XE::Window >( reinterpret_cast<XE::uint64>( window ) ), nullptr )
#elif PLATFORM_OS & OS_MAC
	: XE::Window( XE::HandleCast< XE::Window >( reinterpret_cast<XE::uint64>( window ) ), nullptr, nullptr )
#elif PLATFORM_OS & OS_WINDOWS
	: XE::Window( XE::HandleCast< XE::Window >( window->winId() ), nullptr )
#endif
{
	_Window = window;
}

void XS::WindowProxy::ShowWindow()
{
	_Window->show();
}

void XS::WindowProxy::HideWindow()
{
	_Window->hide();
}

void XS::WindowProxy::MinimizeWindow()
{
	_Window->showMinimized();
}

void XS::WindowProxy::MaximizeWindow()
{
	_Window->showMaximized();
}

bool XS::WindowProxy::GetWindowFocus()
{
	return _Window->hasFocus();
}

void XS::WindowProxy::FullscreenWindow()
{
	_Window->showFullScreen();
}

void XS::WindowProxy::SetWindowTitle( const XE::String & title )
{
	_Window->setWindowTitle( QString::fromUtf8( title.c_str() ) );
}

XE::Pair< XE::int32, XE::int32 > XS::WindowProxy::GetWindowSize()
{
	return { _Window->width(), _Window->height() };
}

void XS::WindowProxy::SetWindowRect( XE::int32 x, XE::int32 y, XE::int32 w, XE::int32 h, bool topmost /*= false */ )
{
	_Window->setGeometry( x, y, w, h );
}

bool XS::WindowProxy::IsShow() const
{
	return !_Window->isHidden();
}

bool XS::WindowProxy::IsHide() const
{
	return _Window->isHidden();
}

bool XS::WindowProxy::IsMinimize() const
{
	return _Window->isMinimized();
}

bool XS::WindowProxy::IsMaximize() const
{
	return _Window->isMaximized();
}

bool XS::WindowProxy::IsFullscreen() const
{
	return _Window->isFullScreen();
}

void XS::WindowProxy::MessageLoop()
{
	// TODO: 
}

XS::MainWindow * XS::WindowProxy::GetMainWindow() const
{
	return _Window;
}
