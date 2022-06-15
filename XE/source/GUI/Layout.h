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
	virtual void Rebuild( XE::int32 width, XE::int32 height, const XE::Array< XE::WidgetPtr > & children ) const = 0;
};

class XE_API LayoutItem : public XE::Object
{
	OBJECT( LayoutItem, XE::Object )

public:
	LayoutItem();

	~LayoutItem();
};

END_XE_NAMESPACE

#endif//LAYOUT_H__A51BC185_E2F8_494C_A5BC_391195BFF793
