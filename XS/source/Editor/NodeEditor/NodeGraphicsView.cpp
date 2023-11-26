#include "NodeGraphicsView.h"

NodeGraphicsView::NodeGraphicsView( QWidget * parent )
	: QGraphicsView( parent )
{

}

NodeGraphicsView::~NodeGraphicsView()
{

}

void NodeGraphicsView::wheelEvent( QWheelEvent * event )
{
	QGraphicsView::wheelEvent( event );
}

void NodeGraphicsView::mousePressEvent( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent( event );
}

void NodeGraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent( event );
}

void NodeGraphicsView::mouseReleaseEvent( QMouseEvent * event )
{
	QGraphicsView::mouseReleaseEvent( event );
}

void NodeGraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
{
}

void NodeGraphicsView::drawForeground( QPainter * painter, const QRectF & rect )
{
}
