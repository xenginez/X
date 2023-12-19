/*!
 * \file	NodeGraphicsItemGroup.h
 *
 * \author	ZhengYuanQing
 * \date	2023/12/18
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSITEMGROUP_H__D0941EC9_BF2D_42FA_AC23_0FF2E3FCAA22
#define NODEGRAPHICSITEMGROUP_H__D0941EC9_BF2D_42FA_AC23_0FF2E3FCAA22

#include "NodeGraphicsItem.h"

BEG_XS_NAMESPACE

class NodeGraphicsItemGroup : public XS::NodeGraphicsItem
{
	Q_OBJECT

public:
	NodeGraphicsItemGroup( const QList<QGraphicsItem * > & items, QGraphicsScene * scene, QGraphicsItem * parent = nullptr );
	~NodeGraphicsItemGroup() override;

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;

protected:
	QMenu * contextMenu() override;
	QRectF contextRect() const override;
	QBrush titleBarBrush() const override;
	
protected:
	void drawTitleBar( QPainter * painter, const QRectF & rect ) override;
	void drawContextBody( QPainter * painter, const QRectF & rect ) override;

private:
	QRectF colorButtonRect() const;

private slots:
	void updateBoundingRect();
	
private:
	QColor _color;
	bool _isUpdate = false;
};

END_XS_NAMESPACE

#endif//NODEGRAPHICSITEMGROUP_H__D0941EC9_BF2D_42FA_AC23_0FF2E3FCAA22