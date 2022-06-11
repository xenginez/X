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

#include "Math/Rect.h"

#include "Type.h"

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

	const XE::Recti & GetRect() const;

	void SetRect( const XE::Recti & val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

public:
	XE::CanvasPtr GetCanvas() const;

	XE::WidgetPtr GetParent() const;

	XE::uint64 GetChildCount() const;

	XE::WidgetPtr GetChild( XE::uint64 val ) const;

	XE::WidgetPtr FindChild( const XE::String & val ) const;

protected:
	void Rebuild();

private:
	bool _Enable = true;
	XE::Recti _Rect;
	XE::String _Name;
	XE::CanvasWPtr _Canvas;
	XE::WidgetWPtr _Parent;
	XE::Array< XE::WidgetPtr > _Children;
};

END_XE_NAMESPACE

#endif//WIDGET_H__EE35767C_971B_43A8_8B3F_9278900E2FAE
