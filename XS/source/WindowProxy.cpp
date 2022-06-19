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

}

void XS::WindowProxy::ShowWindow()
{

}

void XS::WindowProxy::HideWindow()
{

}

void XS::WindowProxy::MinimizeWindow()
{

}

void XS::WindowProxy::MaximizeWindow()
{

}

bool XS::WindowProxy::GetWindowFocus()
{
	return true;
}

void XS::WindowProxy::FullscreenWindow()
{

}

void XS::WindowProxy::SetWindowTitle( const XE::String & title )
{

}

XE::Pair< XE::uint32, XE::uint32 > XS::WindowProxy::GetWindowSize()
{
	return {};
}

XE::Pair< XE::uint32, XE::uint32 > XS::WindowProxy::GetScreenSize()
{
	return {};
}

void XS::WindowProxy::SetWindowRect( XE::uint32 x, XE::uint32 y, XE::uint32 w, XE::uint32 h, bool topmost /*= false */ )
{

}

bool XS::WindowProxy::IsShow() const
{
	return true;
}

bool XS::WindowProxy::IsHide() const
{
	return false;
}

bool XS::WindowProxy::IsMinimize() const
{
	return false;
}

bool XS::WindowProxy::IsMaximize() const
{
	return false;
}

bool XS::WindowProxy::IsFullscreen() const
{
	return true;
}

void XS::WindowProxy::MessageLoop()
{

}
