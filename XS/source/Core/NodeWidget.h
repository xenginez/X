/*!
 * \file	NodeView.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5
#define NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5

#include <QGraphicsItem>
#include <QGraphicsView>

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API NodeItem : public QGraphicsItem
{
public:
	NodeItem();

	~NodeItem() override;

protected:
	void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget = 0 ) override;

	QVariant itemChange( GraphicsItemChange change, const QVariant & value ) override;

	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;

	void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;

	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

	void hoverEnterEvent( QGraphicsSceneHoverEvent * event ) override;

	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event ) override;

	void hoverMoveEvent( QGraphicsSceneHoverEvent * event ) override;

	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event ) override;

	void contextMenuEvent( QGraphicsSceneContextMenuEvent * event ) override;
};

class XS_API NodeWidget : public QGraphicsView
{
	Q_OBJECT

private:
	struct Private;

public:
	NodeWidget( QWidget * parent = nullptr );

	~NodeWidget() override;

protected:
	void contextMenuEvent( QContextMenuEvent * event ) override;

	void wheelEvent( QWheelEvent * event ) override;

	void keyPressEvent( QKeyEvent * event ) override;

	void keyReleaseEvent( QKeyEvent * event ) override;

	void mousePressEvent( QMouseEvent * event ) override;

	void mouseMoveEvent( QMouseEvent * event ) override;

	void mouseReleaseEvent( QMouseEvent * event ) override;

	void mouseDoubleClickEvent( QMouseEvent * event ) override;

	void drawBackground( QPainter * painter, const QRectF & rect ) override;

	void showEvent( QShowEvent * event ) override;

private:
	Private * _p;
};

END_XS_NAMESPACE

#endif//NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5
