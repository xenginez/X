/*!
 * \file	NodeConnect.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef NODECONNECT_H__E7D2645D_F038_4437_81A0_E133E3226C8C
#define NODECONNECT_H__E7D2645D_F038_4437_81A0_E133E3226C8C

#include <QGraphicsItem>

#include "Core/Global.h"

BEG_XS_NAMESPACE

class NodeItem;
class NodeView;
class NodeScene;
class NodeConnect;

class XS_API NodeConnect : public QGraphicsItem
{
	Q_OBJECT

public:
public:
	NodeConnect( XS::NodeScene * scene );

	~NodeConnect() override;

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
	XS::NodeScene * _Scene = nullptr;
};

END_XS_NAMESPACE

#endif//NODECONNECT_H__E7D2645D_F038_4437_81A0_E133E3226C8C
