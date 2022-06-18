#include "Window.h"

#if PLATFORM_OS == OS_WINDOWS

#include "Core/EventService.h"
#include "Core/CoreFramework.h"
#include "Inputs/InputService.h"

#include <Windows.h>

XE::Window::Window( XE::WindowHandle window, void * instance )
	:_Window( window ), _Instance( instance )
{

}

void XE::Window::ShowCursor()
{
	::ShowCursor( true );
}

void XE::Window::HideCursor()
{
	::ShowCursor( false );
}

void XE::Window::ShowWindow()
{
	::ShowWindow( reinterpret_cast< HWND >( _Window.GetValue() ), SW_SHOW );
	::UpdateWindow( reinterpret_cast< HWND >( _Window.GetValue() ) );
}

void XE::Window::HideWindow()
{
	::ShowWindow( reinterpret_cast< HWND >( _Window.GetValue() ), SW_HIDE );
	::UpdateWindow( reinterpret_cast< HWND >( _Window.GetValue() ) );
}

void XE::Window::MinimizeWindow()
{
	::ShowWindow( reinterpret_cast< HWND >( _Window.GetValue() ), SW_MINIMIZE );
	::UpdateWindow( reinterpret_cast< HWND >( _Window.GetValue() ) );
}

void XE::Window::MaximizeWindow()
{
	::ShowWindow( reinterpret_cast< HWND >( _Window.GetValue() ), SW_MAXIMIZE );
	::UpdateWindow( reinterpret_cast< HWND >( _Window.GetValue() ) );
}

bool XE::Window::GetWindowFocus()
{
	return ::GetFocus() == reinterpret_cast< HWND >( _Window.GetValue() );
}

void XE::Window::FullscreenWindow()
{
	int w = ::GetSystemMetrics( SM_CXSCREEN );
	int h = ::GetSystemMetrics( SM_CYSCREEN );

	::SetForegroundWindow( reinterpret_cast<HWND>( _Window.GetValue() ) );

	::SetWindowLong( reinterpret_cast<HWND>( _Window.GetValue() ), GWL_STYLE, WS_VISIBLE | WS_POPUP );
	::SetWindowPos( reinterpret_cast< HWND >( _Window.GetValue() ), HWND_TOPMOST, 0, 0, w, h, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );
	::UpdateWindow( reinterpret_cast< HWND >( _Window.GetValue() ) );
}

void XE::Window::SetWindowTitle( const XE::String & title )
{
	::SetWindowTextW( reinterpret_cast< HWND >( _Window.GetValue() ), XE::StringConvert< XE::AnsiEncode, XE::WideEncode >::convert( title.c_str() ).c_str() );
}

XE::Pair< XE::uint32, XE::uint32 > XE::Window::GetWindowSize()
{
	XE::Pair< XE::uint32, XE::uint32 > result;

	::RECT rect;

	::GetWindowRect( reinterpret_cast< HWND >( _Window.GetValue() ), &rect );

	result.first = rect.right - rect.left;
	result.second = rect.bottom - rect.top;

	return result;
}

XE::Pair< XE::uint32, XE::uint32 > XE::Window::GetScreenSize()
{
	XE::Pair< XE::uint32, XE::uint32 > result;

	::RECT rect;

	::GetClientRect( ::GetDesktopWindow(), &rect );

	result.first = rect.right - rect.left;
	result.second = rect.bottom - rect.top;

	return result;
}

void XE::Window::SetWindowRect( XE::uint32 x, XE::uint32 y, XE::uint32 w, XE::uint32 h, bool topmost )
{
	::SetWindowLong( reinterpret_cast<HWND>( _Window.GetValue() ), GWL_STYLE, WS_OVERLAPPEDWINDOW );
	::SetWindowPos( reinterpret_cast< HWND >( _Window.GetValue() ), topmost ? HWND_TOPMOST : HWND_NOTOPMOST, x, y, w, h, SWP_SHOWWINDOW );
	::UpdateWindow( reinterpret_cast< HWND >( _Window.GetValue() ) );
}

void XE::Window::MessageLoop()
{
	MSG msg;
	while( ::PeekMessage( &msg, reinterpret_cast< HWND >( _Window.GetValue() ), 0, 0, PM_REMOVE ) )
	{
		::TranslateMessage( &msg );
		::DispatchMessage( &msg );

		XE::InputEventInfo info;
		info.win_message = msg.message;
		info.win_lparam = msg.lParam;
		info.win_wparam = msg.wParam;

		switch ( msg.message )
		{
		case WM_CHAR:
		{
			XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->PostEvent( XE::EVENT_KEYBOARD, info );
		}
		break;
		case WM_INPUT:
		{
			XE::uint32 size = 0;
			::GetRawInputData( (HRAWINPUT)info.win_lparam, RID_INPUT, nullptr, &size, sizeof( RAWINPUTHEADER ) );
			info.raw_data.resize( size );
			::GetRawInputData( (HRAWINPUT)info.win_lparam, RID_INPUT, info.raw_data.data(), &size, sizeof( RAWINPUTHEADER ) );

			RAWINPUT * raw = reinterpret_cast<RAWINPUT *>( info.raw_data.data() );

			switch ( raw->header.dwType )
			{
			case RIM_TYPEHID:
				XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->PostEvent( XE::EVENT_HID, info );
				break;
			case RIM_TYPEMOUSE:
				XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->PostEvent( XE::EVENT_MOUSE, info );
				break;
			case RIM_TYPEKEYBOARD:
				XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->PostEvent( XE::EVENT_KEYBOARD, info );
				break;
			}
		}
		break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		{
			XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::InputService >()->ClearStatus();
		}
		break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		{
			XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->PostEvent( XE::EVENT_MOUSE, info );
		}
		break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->PostEvent( XE::EVENT_KEYBOARD, info );
		}
		break;
		}
	}
}

bool XE::Window::IsShow() const
{
	return ::IsWindowVisible( reinterpret_cast<HWND>( _Window.GetValue() ) );
}

bool XE::Window::IsHide() const
{
	return !( ::IsWindowVisible( reinterpret_cast<HWND>( _Window.GetValue() ) ) );
}

bool XE::Window::IsMinimize() const
{
	return ::IsIconic( reinterpret_cast<HWND>( _Window.GetValue() ) );
}

bool XE::Window::IsMaximize() const
{
	return ::IsZoomed( reinterpret_cast<HWND>( _Window.GetValue() ) );
}

bool XE::Window::IsFullscreen() const
{
	return ::GetWindowLong( reinterpret_cast<HWND>( _Window.GetValue() ), GWL_STYLE ) != WS_OVERLAPPEDWINDOW;
}

#endif