/*!
 * \file	UIController.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef UICONTROLLER_H__9537A7ED_8E8A_4AB9_BCB1_29714AFA9C61
#define UICONTROLLER_H__9537A7ED_8E8A_4AB9_BCB1_29714AFA9C61

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Controller : public XE::Object
{
	OBJECT( Controller, XE::Object )

private:
	friend class ActionBase;

public:
	Controller();

	~Controller() override;

public:
	virtual void Startup() = 0;

	virtual void Clearup() = 0;

public:
	XE::CanvasPtr GetCanvas() const;

	XE::WidgetPtr GetWidget() const;

private:
	void SetWidget( const XE::WidgetPtr & val );

private:
	XE::CanvasWPtr _Canvas;
	XE::WidgetWPtr _Widget;
};

END_XE_NAMESPACE

#endif//UICONTROLLER_H__9537A7ED_8E8A_4AB9_BCB1_29714AFA9C61
