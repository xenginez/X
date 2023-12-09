/*!
 * \file	NodeGraphicsView.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSVIEW_H__0BDEDAEB_3D81_4485_87E7_AC940E007D1B
#define NODEGRAPHICSVIEW_H__0BDEDAEB_3D81_4485_87E7_AC940E007D1B

#include <QGraphicsView>

#include "Global.h"

BEG_XS_NAMESPACE

class NodeGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	NodeGraphicsView( QWidget * parent = nullptr );
	~NodeGraphicsView();

protected:
	void wheelEvent( QWheelEvent * event ) override;
	void mousePressEvent( QMouseEvent * event ) override;
	void mouseMoveEvent( QMouseEvent * event ) override;
	void mouseReleaseEvent( QMouseEvent * event ) override;

protected:
	virtual void drawBackground( QPainter * painter, const QRectF & rect );
	virtual void drawForeground( QPainter * painter, const QRectF & rect );

private:
	bool _isMove = false;
	QPointF _centerPos, _movePos;
};

END_XS_NAMESPACE

#endif//NODEGRAPHICSVIEW_H__0BDEDAEB_3D81_4485_87E7_AC940E007D1B
