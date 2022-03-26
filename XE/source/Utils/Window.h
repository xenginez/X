/*!
 * \file	Window.h
 *
 * \author	ZhengYuanQing
 * \date	2021/12/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WINDOW_H__0AAFC42D_A0A3_4AC7_B881_903F0994120B
#define WINDOW_H__0AAFC42D_A0A3_4AC7_B881_903F0994120B

#include "Event.h"
#include "Disposable.h"

BEG_XE_NAMESPACE

DECL_HANDLE( Window );

class XE_API Window
{
#if PLATFORM_OS & OS_LINUX

private:
	void * _Display;

public:
	Window( XE::WindowHandle window, void * display );

	void * GetDisplay() { return _Display; }

#elif PLATFORM_OS & OS_ANDROID

private:
	void * _App;

public:
	Window( XE::WindowHandle window, void * app );

	void * GetApp() { return _App; }

#elif PLATFORM_OS & ( OS_IOS | OS_MAC )

private:
	void * _View;
	void * _Layer;

public:
	Window( XE::WindowHandle window, void * view, void * layer );

	void * GetView() { return _View; }

	void * GetLayer() { return _Layer; }

#elif PLATFORM_OS & OS_WINDOWS

public:
	Window( XE::WindowHandle window );

#endif

public:
	void ShowWindow();

	void HideWindow();

	void MinimizeWindow();

	void MaximizeWindow();

	bool GetWindowFocus();

	void FullscreenWindow();

	void SetWindowTitle( const XE::String & title );

	XE::Pair< XE::uint32, XE::uint32 > GetWindowSize();

	XE::Pair< XE::uint32, XE::uint32 > GetScreenSize();

	void SetWindowRect( XE::uint32 x, XE::uint32 y, XE::uint32 w, XE::uint32 h, bool topmost = false );

public:
	bool IsShow() const;

	bool IsHide() const;

	bool IsMinimize() const;

	bool IsMaximize() const;

	bool IsFullscreen() const;

public:
	void MessageLoop();

public:
	XE::WindowHandle GetHandle()
	{
		return _Window;
	}

private:
	XE::WindowHandle _Window;
};

DECL_EVENT( WINDOW, "window", XE::WindowEventType );

END_XE_NAMESPACE

#endif//WINDOW_H__0AAFC42D_A0A3_4AC7_B881_903F0994120B
