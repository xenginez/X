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

private:
	void * _Instance;

public:
	Window( XE::WindowHandle window, void * instance );

	void * GetInstance() { return _Instance; }

#endif

public:
	virtual void ShowCursor();

	virtual void HideCursor();

	virtual void ShowWindow();

	virtual void HideWindow();

	virtual void MinimizeWindow();

	virtual void MaximizeWindow();

	virtual bool GetWindowFocus();

	virtual void FullscreenWindow();

	virtual void SetWindowTitle( const XE::String & title );

	virtual XE::Pair< XE::int32, XE::int32 > GetWindowSize();

	virtual void SetWindowRect( XE::int32 x, XE::int32 y, XE::int32 w, XE::int32 h, bool topmost = false );

public:
	virtual bool IsShow() const;

	virtual bool IsHide() const;

	virtual bool IsMinimize() const;

	virtual bool IsMaximize() const;

	virtual bool IsFullscreen() const;

public:
	virtual void MessageLoop();

public:
	XE::int32 GetWindowWidth()
	{
		return GetWindowSize().first;
	}

	XE::int32 GetWindowHeight()
	{
		return GetWindowSize().second;
	}

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
