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

#include <QGraphicsView>

#include "Core/Global.h"

#include "NodeScene.h"

BEG_XS_NAMESPACE

class NodeItem;
class NodeView;
class NodeScene;
class NodeConnect;

class XS_API NodeView : public QGraphicsView
{
	Q_OBJECT

public:
	NodeView( XS::NodeScene * scene, QWidget * parent = nullptr );

	~NodeView() override;

protected:
	void contextMenuEvent( QContextMenuEvent * event ) override;

	void wheelEvent( QWheelEvent * event ) override;

	void keyPressEvent( QKeyEvent * event ) override;

	void keyReleaseEvent( QKeyEvent * event ) override;

	void mousePressEvent( QMouseEvent * event ) override;

	void mouseMoveEvent( QMouseEvent * event ) override;

	void drawBackground( QPainter * painter, const QRectF & rect ) override;

	void showEvent( QShowEvent * event ) override;

private:
	XS::NodeScene * _Scene = nullptr;
};

END_XS_NAMESPACE

#endif//NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5
