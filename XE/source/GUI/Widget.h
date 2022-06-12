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

	const XE::Vec2i & GetMinSize() const;

	void SetMinSize( const XE::Vec2i & val );

	const XE::Vec2i & GetMaxSize() const;

	void SetMaxSize( const XE::Vec2i & val );

	const XE::LayoutPtr & GetLayout() const;

	void SetLayout( const XE::LayoutPtr & val );

	XE::SizePolicy GetVerticalSizePolicy() const;

	void SetVerticalSizePolicy( XE::SizePolicy val );

	XE::SizePolicy GetHorizontalSizePolicy() const;

	void SetHorizontalSizePolicy( XE::SizePolicy val );

public:
	XE::CanvasPtr GetCanvas() const;

	XE::WidgetPtr GetParent() const;

	XE::WidgetPtr FindChild( const XE::String & val ) const;

	const XE::Array< XE::WidgetPtr > & GetChildren( XE::uint64 val ) const;

protected:
	virtual XE::Vec2i GetSizeHint() const;

private:
	bool _Enable = true;
	XE::String _Name;

	XE::Recti _Rect;
	XE::Vec2i _MinSize = { 0, 0 };
	XE::Vec2i _MaxSize = { std::numeric_limits<XE::int32>::max(), std::numeric_limits<XE::int32>::max() };
	XE::LayoutPtr _Layout;
	XE::SizePolicy _VerticalSizePolicy = SizePolicy::PREFERRED;
	XE::SizePolicy _HorizontalSizePolicy = SizePolicy::PREFERRED;

	XE::CanvasWPtr _Canvas;
	XE::WidgetWPtr _Parent;
	XE::Array< XE::WidgetPtr > _Children;
};

END_XE_NAMESPACE

#endif//WIDGET_H__EE35767C_971B_43A8_8B3F_9278900E2FAE
