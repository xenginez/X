/*!
 * \file	Widget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WIDGET_H__EE35767C_971B_43A8_8B3F_9278900E2FAE
#define WIDGET_H__EE35767C_971B_43A8_8B3F_9278900E2FAE

#include "Utils/Picture.h"
#include "Core/Asset.h"
#include "Core/CoreFramework.h"

#include "Action.h"

BEG_XE_NAMESPACE

class XE_API Widget : public XE::Object
{
	OBJECT( Widget, XE::Object )

private:
	friend class Canvas;

public:
	Widget();

	~Widget() override;

protected:
	void Startup();

	void Update();

	void Render();

	void Clearup();

protected:
	virtual void OnStartup();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClearup();

public:
	bool GetEnable() const;

	void SetEnable( bool val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const XE::LayoutPtr & GetLayout() const;

	void SetLayout( const XE::LayoutPtr & val );

	const XE::LayoutItemPtr & GetLayoutItem() const;

	void SetLayoutItem( const XE::LayoutItemPtr & val );

public:
	const XE::Recti & GetRect() const;

	void SetRect( const XE::Recti & val );

	const XE::Recti & GetFrame() const;

	void SetFrame( const XE::Recti & val );

public:
	XE::CanvasPtr GetCanvas() const;

	XE::WidgetPtr GetParent() const;

	XE::ControllerPtr GetController() const;

	XE::CoreFrameworkPtr GetFramework() const;

	const XE::Array< XE::WidgetPtr > & GetChildren() const;

	XE::WidgetPtr FindChild( const XE::String & val ) const;

public:
	void Show();

	void Hide();

	bool IsHidden() const;

private:
	bool _Enable = true;
	bool _Hidden = false;

	XE::String _Name;

	XE::Recti _Rect;
	XE::Recti _Frame;

	XE::LayoutPtr _Layout;
	XE::LayoutItemPtr _LayoutItem;

	XE::CanvasWPtr _Canvas;
	XE::WidgetWPtr _Parent;
	XE::Array< XE::WidgetPtr > _Children;
};

END_XE_NAMESPACE

#endif//WIDGET_H__EE35767C_971B_43A8_8B3F_9278900E2FAE
