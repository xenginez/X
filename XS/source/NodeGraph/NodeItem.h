/*!
 * \file	NodeItem.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/28
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef NODEITEM_H__196BE37A_728B_4E71_A6EE_3596AB05D2EA
#define NODEITEM_H__196BE37A_728B_4E71_A6EE_3596AB05D2EA

#include <QGraphicsItem>

#include "Core/Global.h"

BEG_XS_NAMESPACE

class NodeItem;
class NodeView;
class NodeScene;
class NodeConnect;

class XS_API NodeItem : public QGraphicsItem
{
	Q_OBJECT

public:
	NodeItem( XS::NodeScene * scene, const XE::Variant & data = {} );

	~NodeItem() override;

protected:
	void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget = 0 ) override;

	QVariant itemChange( GraphicsItemChange change, const QVariant & value ) override;

	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;

	void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;

	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

	void hoverEnterEvent( QGraphicsSceneHoverEvent * event ) override;

	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event ) override;

	void hoverMoveEvent( QGraphicsSceneHoverEvent * ) override;

	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event ) override;

	void contextMenuEvent( QGraphicsSceneContextMenuEvent * event ) override;

private:
	XE::Variant _Data;
	XS::NodeScene * _Scene = nullptr;
};

END_XS_NAMESPACE

#endif//NODEITEM_H__196BE37A_728B_4E71_A6EE_3596AB05D2EA
