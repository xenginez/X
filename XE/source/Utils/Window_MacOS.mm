#include "Window.h"

#if PLATFORM_OS & OS_MAC

#import < Cocoa/Cocoa.h>

XE::Window::Window( XE::WindowHandle window, void * view, void * layer )
	:_Window( window ), _View( view ), _Layer( layer )
{

}

void XE::Window::ShowWindow()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	[window makeKeyAndOrderFront : nil] ;
}

void XE::Window::HideWindow()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	[window orderOut : nil] ;
}

void XE::Window::MinimizeWindow()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	if( ![window isMiniaturized] )
	{
		[window miniaturize : nil] ;
	}
}

void XE::Window::MaximizeWindow()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	if( ![window isZoomed] )
	{
		[window zoom : nil] ;
	}
}

bool XE::Window::GetWindowFocus()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	return [window isKeyWindow];
}

void XE::Window::FullscreenWindow()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	[window setStyleMask : NSWindowStyleMaskBorderless] ;
	[window setFrame : [[window screen]frame] display : NO] ;
	[window setLevel : NSMainMenuWindowLevel + 1] ;
	[window makeKeyAndOrderFront : nil] ;
}

void XE::Window::SetWindowTitle( const XE::String & title )
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	NSString * s = [[[NSString alloc]initWithCString:title.c_str()] autorelease];

	[window setTitle : s] ;
}

XE::Pair< XE::int32, XE::int32 > XE::Window::GetWindowSize()
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	NSRect * rect = [window contentRect];

	XE::Pair< XE::int32, XE::int32 > result;

	result.first = [rect width];
	result.second = [rect height];

	return result;
}

void XE::Window::SetWindowRect( XE::int32 x, XE::int32 y, XE::int32 w, XE::int32 h, bool topmost )
{
	NSWindow * window = reinterpret_cast< NSWindow * >( _Window.GetValue() );

	NSRect rect;
	rect.origin.x = x;
	rect.origin.y = y;
	rect.size.width = w;
	rect.size.height = h;

	[window setStyleMask : NSBorderlessWindowMask] ;
	[window setFrame : [window frameRectForContentRect : rect] display : YES] ;
	[window setLevel : NSNormalWindowLevel] ;
}

void XE::Window::MessageLoop()
{

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