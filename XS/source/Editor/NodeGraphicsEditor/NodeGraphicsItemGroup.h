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
	NodeGraphicsItemGroup( QGraphicsItem * parent = nullptr );
	~NodeGraphicsItemGroup() override;

public:
	void addToGroup( XS::NodeGraphicsItem * item );
	void removeFromGroup( XS::NodeGraphicsItem * item );

protected:
	QBrush titleBarBrush() const override;

protected:
	QRectF contextRect() const override;
	
protected:
	void drawTitleBar( QPainter * painter, const QRectF & rect ) override;
	void drawContextBody( QPainter * painter, const QRectF & rect ) override;

private:
	void updatePos();
	QRectF colorButtonRect() const;

private:
	QColor _color;
};

END_XS_NAMESPACE

#endif//NODEGRAPHICSITEMGROUP_H__D0941EC9_BF2D_42FA_AC23_0FF2E3FCAA22