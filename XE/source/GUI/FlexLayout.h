/*!
 * \file	FlexLayout.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/15
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FLEXLAYOUT_H__3ACDAAEF_8AB6_4AD0_BCC9_8292A2276452
#define FLEXLAYOUT_H__3ACDAAEF_8AB6_4AD0_BCC9_8292A2276452

#include "Layout.h"

BEG_XE_NAMESPACE

class XE_API FlexLayout : public XE::Layout
{
	OBJECT( FlexLayout, XE::Layout )

public:
	FlexLayout();

	~FlexLayout() override;

public:
	void Rebuild( XE::int32 width, XE::int32 height, const XE::Array< XE::WidgetPtr > & children ) const override;

public:
	XE::FlexWrap GetFlexWrap() const;

	void SetFlexWrap( XE::FlexWrap val );

	XE::FlexAlign GetJustifyContent() const;

	void SetJustifyContent( XE::FlexAlign val );

	XE::FlexAlign GetAlignContent() const;

	void SetAlignContent( XE::FlexAlign val );

	XE::FlexAlign GetAlignItems() const;

	void SetAlignItems( XE::FlexAlign val );

	XE::FlexPosition GetFlexPosition() const;

	void SetFlexPosition( XE::FlexPosition val );

	XE::FlexDirection GetFlexDirection() const;

	void SetFlexDirection( XE::FlexDirection val );

public:
	const XE::Recti & GetPadding() const;

	void SetPadding( const XE::Recti & val );

	const XE::Recti & GetMargins() const;

	void SetMargins( const XE::Recti & val );

private:
	XE::Recti _Padding;
	XE::Recti _Margins;
	XE::FlexWrap _FlexWrap = XE::FlexWrap::NO_WRAP;
	XE::FlexAlign _AlignItems = XE::FlexAlign::STRETCH;
	XE::FlexAlign _AlignContent = XE::FlexAlign::STRETCH;
	XE::FlexAlign _JustifyContent = XE::FlexAlign::START;
	XE::FlexPosition _FlexPosition = XE::FlexPosition::RELATIVE;
	XE::FlexDirection _FlexDirection = XE::FlexDirection::COLUMN;
};

class XE_API FlexLayoutItem : public XE::LayoutItem
{
	OBJECT( FlexLayoutItem, XE::LayoutItem )

public:
	FlexLayoutItem();

	~FlexLayoutItem() override;

public:
	XE::float32 GetGrow() const;

	void SetGrow( XE::float32 val );

	XE::float32 GetShrink() const;

	void SetShrink( XE::float32 val );

	XE::float32 GetBasis() const;

	void SetBasis( XE::float32 val );

	XE::FlexAlign GetAlignSelf() const;

	void SetAlignSelf( XE::FlexAlign val );

public:
	XE::float32 _Grow = 0.0f;
	XE::float32 _Shrink = 1.0f;
	XE::float32 _Basis = std::numeric_limits<XE::float32>::quiet_NaN();
	XE::FlexAlign _AlignSelf = XE::FlexAlign::AUTO;
};

END_XE_NAMESPACE

#endif//FLEXLAYOUT_H__3ACDAAEF_8AB6_4AD0_BCC9_8292A2276452
