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
	void ShowWindow() override;

	void HideWindow() override;

	void MinimizeWindow() override;

	void MaximizeWindow() override;

	bool GetWindowFocus() override;

	void FullscreenWindow() override;

	void SetWindowTitle( const XE::String & title ) override;

	XE::Pair< XE::int32, XE::int32 > GetWindowSize() override;

	void SetWindowRect( XE::int32 x, XE::int32 y, XE::int32 w, XE::int32 h, bool topmost = false ) override;

public:
	bool IsShow() const override;

	bool IsHide() const override;

	bool IsMinimize() const override;

	bool IsMaximize() const override;

	bool IsFullscreen() const override;

public:
	void MessageLoop() override;

public:
	XS::MainWindow * GetMainWindow() const;

private:
	XS::MainWindow * _Window = nullptr;
};
DECL_PTR( WindowProxy );

END_XS_NAMESPACE

#endif//WINDOWPROXY_H__2558AC52_BE3F_46D7_B565_F48F4DCFBCD3
