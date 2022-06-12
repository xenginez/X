/*!
 * \file	Layout.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LAYOUT_H__A51BC185_E2F8_494C_A5BC_391195BFF793
#define LAYOUT_H__A51BC185_E2F8_494C_A5BC_391195BFF793

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Layout : public XE::Object
{
	OBJECT( Layout, XE::Object )

public:
	Layout();

	~Layout();

public:
	XE::int32 GetSpacing() const;

	void SetSpacing( XE::int32 val );

	XE::int32 GetLeftMargin() const;

	void SetLeftMargin( XE::int32 val );

	XE::int32 GetRightMargin() const;

	void SetRightMargin( XE::int32 val );

	XE::int32 GetTopMargin() const;

	void SetTopMargin( XE::int32 val );

	XE::int32 GetBottomMargin() const;

	void SetBottomMargin( XE::int32 val );

	const XE::Vec4i & GetMargins() const;

	void SetMargins( const XE::Vec4i & val );

	XE::AlignmentFlags GetAlignment() const;

	void SetAlignment( XE::AlignmentFlags val );

public:
	virtual XE::Array< XE::Recti > CalcContentRects( const XE::Recti & rect, const XE::Array< XE::WidgetPtr > & widgets ) = 0;

private:
	XE::int32 _Spacing = 0;
	XE::Vec4i _Margins;
	XE::AlignmentFlags _Alignment;
};

END_XE_NAMESPACE

#endif//LAYOUT_H__A51BC185_E2F8_494C_A5BC_391195BFF793
