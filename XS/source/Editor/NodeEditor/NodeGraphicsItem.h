/*!
 * \file	NodeGraphicsItem.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC
#define NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC

#include <QMenu>
#include <QGraphicsItem>

class NodeGraphicsItem : public QGraphicsObject
{
	Q_OBJECT

public:
	NodeGraphicsItem( QGraphicsItem * parent );
	~NodeGraphicsItem();

public:
	bool isHovered() const;

public:
	QRectF boundingRect() const override;
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr ) override;

protected:
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event ) override;
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event ) override;
	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
	void contextMenuEvent( QGraphicsSceneContextMenuEvent * event ) override;

protected:
	virtual QMenu * contextMenu() const;
	virtual QRectF contextRect() const = 0;
	virtual void drawContext( QPainter * painter, const QRectF & rect ) = 0;

private:
	void drawBorder( QPainter * painter, const QRectF & rect );
	void drawTitleBar( QPainter * painter, const QRectF & rect );
	void drawBackground( QPainter * painter, const QRectF & rect );
};

#endif//NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC
