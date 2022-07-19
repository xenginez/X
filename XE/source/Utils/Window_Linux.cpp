#include "Window.h"

#if PLATFORM_OS & OS_LINUX

#include "Inputs/Type.h"
#include "Core/EventService.h"
#include "Core/InputService.h"
#include "Core/CoreFramework.h"

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

XE::Window::Window( XE::WindowHandle window, void * display )
	:_Window( window ), _Display( display )
{

}

void XE::Window::ShowWindow()
{

}

void XE::Window::HideWindow()
{

}

void XE::Window::MinimizeWindow()
{

}

void XE::Window::MaximizeWindow()
{

}

bool XE::Window::GetWindowFocus()
{

}

void XE::Window::FullscreenWindow()
{

}

void XE::Window::SetWindowTitle( const XE::String & title )
{

}

XE::Pair< XE::int32, XE::int32 > XE::Window::GetWindowSize()
{

}

XE::Pair< XE::uint32, XE::uint32 > XE::Window::GetScreenSize()
{

}

void XE::Window::SetWindowRect( XE::int32 x, XE::int32 y, XE::int32 w, XE::int32 h, bool topmost )
{

}

void XE::Window::MessageLoop()
{
	XEvent event;
	while( XPending( _Display ) )
	{
		XNextEvent( _Display, &event );

		// TODO: 
	}
}

bool Window::IsShow() const
{

}

bool XE::Window::IsHide() const
{

}

bool XE::Window::IsMinimize() const
{

}

bool XE::Window::IsMaximize() const
{

}

bool XE::Window::IsFullscreen() const
{

}

#endif