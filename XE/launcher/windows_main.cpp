#include <XE/XE.h>

#if PLATFORM_OS & OS_WINDOWS

#include <Windows.h>

#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup" ) 

LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:
	{
		XE::CoreFramework::GetCurrentFramework()->Exit();
	}
	break;
	default:
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return 0;
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_CROSS );
	wndclass.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "XE";

	RegisterClass( &wndclass );



	HWND hwnd = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, "XE", "XE", WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, NULL, NULL, hInstance, NULL );



	XE::CoreFramework framework;

	int result = framework.Exec( XE::MakeShared< XE::Window >( XE::HandleCast< XE::Window >( reinterpret_cast< XE::uint64 >( hwnd ) ), hInstance ) );

	DestroyWindow( hwnd );

	return result;
}

#endif