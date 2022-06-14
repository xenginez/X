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

	const XE::Recti & GetPadding() const;

	void SetPadding( const XE::Recti & val );

	const XE::Recti & GetMargins() const;

	void SetMargins( const XE::Recti & val );

	XE::int32 GetWidth() const;

	void SetWidth( XE::int32 val );

	XE::int32 GetHeight() const;

	void SetHeight( XE::int32 val );

	XE::int32 GetLeft() const;

	void SetLeft( XE::int32 val );

	XE::int32 GetRight() const;

	void SetRight( XE::int32 val );

	XE::int32 GetTop() const;

	void SetTop( XE::int32 val );

	XE::int32 GetBottom() const;

	void SetBottom( XE::int32 val );

	XE::int32 GetPaddingLeft() const;

	void SetPaddingLeft( XE::int32 val );

	XE::int32 GetPaddingRight() const;

	void SetPaddingRight( XE::int32 val );

	XE::int32 GetPaddingTop() const;

	void SetPaddingTop( XE::int32 val );

	XE::int32 GetPaddingBottom() const;

	void SetPaddingBottom( XE::int32 val );

	XE::int32 GetMarginLeft() const;

	void SetMarginLeft( XE::int32 val );

	XE::int32 GetMarginRight() const;

	void SetMarginRight( XE::int32 val );

	XE::int32 GetMarginTop() const;

	void SetMarginTop( XE::int32 val );

	XE::int32 GetMarginBottom() const;

	void SetMarginBottom( XE::int32 val );

	XE::FlexAlign GetJustifyContent() const;

	void SetJustifyContent( XE::FlexAlign val );

	XE::FlexAlign GetAlignContent() const;

	void SetAlignContent( XE::FlexAlign val );

	XE::FlexAlign GetAlignItems() const;

	void SetAlignItems( XE::FlexAlign val );

	XE::FlexAlign GetAlignSelf() const;

	void SetAlignSelf( XE::FlexAlign val );

	XE::FlexPosition GetFlexPosition() const;

	void SetFlexPosition( XE::FlexPosition val );

	XE::FlexDirection GetFlexDirection() const;

	void SetFlexDirection( XE::FlexDirection val );

	XE::FlexWrap GetFlexWrap() const;

	void SetFlexWrap( XE::FlexWrap val );

	XE::float32 GetGrow() const;

	void SetGrow( XE::float32 val );

	XE::float32 GetShrink() const;

	void SetShrink( XE::float32 val );

	XE::int32 GetOrder() const;

	void SetOrder( XE::int32 val );

	XE::float32 GetBasis() const;

	void SetBasis( XE::float32 val );

public:
	XE::Recti GetFrameRect() const;

	XE::CanvasPtr GetCanvas() const;

	XE::WidgetPtr GetParent() const;

	const XE::Array< XE::WidgetPtr > & GetChildren() const;

	XE::WidgetPtr FindChild( const XE::String & val ) const;

private:
	bool _Enable = true;
	XE::String _Name;

	XE::Recti _Rect;
	XE::CanvasWPtr _Canvas;
	XE::WidgetWPtr _Parent;
	XE::Array< XE::WidgetPtr > _Children;

	XE::Recti _Padding;
	XE::Recti _Margins;
	XE::FlexAlign _JustifyContent = XE::FlexAlign::START;
	XE::FlexAlign _AlignContent = XE::FlexAlign::STRETCH;
	XE::FlexAlign _AlignItems = XE::FlexAlign::STRETCH;
	XE::FlexAlign _AlignSelf = XE::FlexAlign::AUTO;
	XE::FlexPosition _Position = XE::FlexPosition::RELATIVE;
	XE::FlexDirection _Direction = XE::FlexDirection::COLUMN;
	XE::FlexWrap _Wrap = XE::FlexWrap::NO_WRAP;
	XE::float32 _Grow = 0.0f;
	XE::float32 _Shrink = 1.0f;
	XE::int32 _Order = 0;
	XE::float32 _Basis = std::numeric_limits<XE::float32>::quiet_NaN();
};

END_XE_NAMESPACE

#endif//WIDGET_H__EE35767C_971B_43A8_8B3F_9278900E2FAE
