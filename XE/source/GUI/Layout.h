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

private:
	friend class Canvas;
	friend class Widget;

private:
	struct Private;
	struct FlexItem;
	struct FlexLayout;

public:
	Layout();

	~Layout();

private:
	void Rebuild( const XE::Canvas * canvas );

	XE::Recti GetFrameRect( const XE::Widget * val ) const;

private:
	static void layout_init( FlexItem * item, float width, float height, FlexLayout * layout );
	static bool layout_align( XE::FlexAlign align, float flex_dim, unsigned int children_count, float * pos_p, float * spacing_p, bool stretch_allowed );
	static XE::FlexAlign child_align( FlexItem * child, FlexItem * parent );
	static void layout_items( FlexItem * item, unsigned int child_begin, unsigned int child_end, unsigned int children_count, FlexLayout * layout );
	static void layout_item( FlexItem * item, float width, float height );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//LAYOUT_H__A51BC185_E2F8_494C_A5BC_391195BFF793
