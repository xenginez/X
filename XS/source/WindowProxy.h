/*!
 * \file	WindowProxy.h
 *
 * \author	ZhengYuanQing
 * \date	2022/04/02
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WINDOWPROXY_H__2558AC52_BE3F_46D7_B565_F48F4DCFBCD3
#define WINDOWPROXY_H__2558AC52_BE3F_46D7_B565_F48F4DCFBCD3

#include "Global.h"

BEG_XS_NAMESPACE

class MainWindow;

class XS_API WindowProxy : public XE::Window
{
public:
	WindowProxy( XS::MainWindow * window );

public:
	virtual void ShowWindow();

	virtual void HideWindow();

	virtual void MinimizeWindow();

	virtual void MaximizeWindow();

	virtual bool GetWindowFocus();

	virtual void FullscreenWindow();

	virtual void SetWindowTitle( const XE::String & title );

	virtual XE::Pair< XE::uint32, XE::uint32 > GetWindowSize();

	virtual XE::Pair< XE::uint32, XE::uint32 > GetScreenSize();

	virtual void SetWindowRect( XE::uint32 x, XE::uint32 y, XE::uint32 w, XE::uint32 h, bool topmost = false );

public:
	virtual bool IsShow() const;

	virtual bool IsHide() const;

	virtual bool IsMinimize() const;

	virtual bool IsMaximize() const;

	virtual bool IsFullscreen() const;

public:
	virtual void MessageLoop();

};

END_XS_NAMESPACE

#endif//WINDOWPROXY_H__2558AC52_BE3F_46D7_B565_F48F4DCFBCD3
