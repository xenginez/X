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

class XE_API Canvas : public XE::Object
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

	const XE::ControllerPtr & GetController() const;

	void SetController( const XE::ControllerPtr & val );

	const XE::LayoutPtr & GetLayout() const;

	void SetLayout( const XE::LayoutPtr & val );

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
	XE::LayoutPtr _Layout;
	XE::ControllerPtr _Controller;
	XE::Array< XE::WidgetPtr > _Children;
	XE::GraphicsRenderPassEncoderPtr _PassEncoder;
};

END_XE_NAMESPACE

#endif//CANVAS_H__9ADE5F45_80AB_424C_AFFA_C5DDC5B28E2D
