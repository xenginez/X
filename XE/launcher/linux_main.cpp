#include <XE/XE.h>

#if PLATFORM_OS & OS_LINUX

#include <X11/Xlib.h>
#include <X11/XKBlib.h>


int main( int argc, char ** argv )
{
	XInitThreads();

	Display * display = XOpenDisplay( NULL );
	int screen = DefaultScreen( display );
	Visual * visual = DefaultVisual( display, screen );
	int depth = DefaultDepth( display, screen );

	Colormap colormap = XCreateColormap( display, RootWindow( display, screen ), visual, AllocNone );

	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = colormap;
	windowAttributes.background_pixel = 0xFFFFFFFF;
	windowAttributes.border_pixel = 0;
	windowAttributes.event_mask = KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask;

	Window window = XCreateWindow( display, RootWindow( display, screen ), 0, 0, desc.width, desc.height, 0, depth,
								   InputOutput, visual,
								   CWBackPixel | CWBorderPixel | CWEventMask | CWColormap,
								   &windowAttributes );

	XSelectInput( display, window, ExposureMask | KeyPressMask );
	XMapWindow( display, window );
	XFlush( display );



	XE::CoreFramework framework;

	int result = framework.Exec( XE::MakeShared< XE::Window >( XE::HandleCast< XE::Window >( reinterpret_cast< XE::uint64 >( window ) ), display ) );



	XDestroyWindow( display, window );
	XCloseDisplay( display );

	return result;
}

#endif