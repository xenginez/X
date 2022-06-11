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

#include "Math/Rect.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Canvas : public XE::Object
{
	OBJECT( Canvas, XE::Object )

private:
	struct Private;

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

public:
	const XE::Array< XE::WidgetPtr > & GetWidgets() const;

	XE::WidgetPtr FindWidget( const XE::String & val ) const;

public:
	void Rebuild();

private:
	void SetWidgets( const XE::Array< XE::WidgetPtr > & val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//CANVAS_H__9ADE5F45_80AB_424C_AFFA_C5DDC5B28E2D
