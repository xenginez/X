/*!
 * \file	Canvas.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CANVAS_H__9ADE5F45_80AB_424C_AFFA_C5DDC5B28E2D
#define CANVAS_H__9ADE5F45_80AB_424C_AFFA_C5DDC5B28E2D

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Canvas final : public XE::Object
{
	OBJECT( Canvas, XE::Object )

public:
	Canvas();

	~Canvas() override;

public:
	void Startup();

	void Update();

	void Clearup();

public:
	bool GetEnable() const;

	void SetEnable( bool val );

	const XE::Recti & GetRect() const;

	void SetRect( const XE::Recti & val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const ImGuiStyle & GetStyle() const;

	void SetStyle( const ImGuiStyle & val );

	const XE::ModelPtr & GetModel() const;

	void SetModel( const XE::ModelPtr & val );

	const XE::LayoutPtr & GetLayout() const;

	void SetLayout( const XE::LayoutPtr & val );

	const XE::ImGuiImplPtr & GetImpl() const;

	void SetImpl( const XE::ImGuiImplPtr & val );

public:
	const XE::Array< XE::WidgetPtr > & GetChildren() const;

	XE::WidgetPtr FindChild( const XE::String & val ) const;

public:
	void Rebuild();

private:
	bool _Dirty = false;

	bool _Enable = true;
	XE::Recti _Rect;
	XE::String _Name;
	XE::ModelPtr _Model;
	XE::LayoutPtr _Layout;
	XE::ImGuiImplPtr _Impl;
	XE::Array< XE::WidgetPtr > _Children;

	ImGuiStyle * _Style = nullptr;
	ImGuiContext * _Context = nullptr;

	XE::GraphicsRenderPassEncoderHandle _PassEncoder;
};

END_XE_NAMESPACE

#endif//CANVAS_H__9ADE5F45_80AB_424C_AFFA_C5DDC5B28E2D
