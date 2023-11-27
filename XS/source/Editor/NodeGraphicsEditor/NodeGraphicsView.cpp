#include "NodeGraphicsView.h"

#include <QScrollBar>
#include <QWheelEvent>

XS::NodeGraphicsView::NodeGraphicsView( QWidget * parent )
	: QGraphicsView( parent )
{
	setCacheMode( CacheNone );
	setViewportUpdateMode( FullViewportUpdate );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

XS::NodeGraphicsView::~NodeGraphicsView()
{

}

void XS::NodeGraphicsView::wheelEvent( QWheelEvent * event )
{
	if ( event->modifiers() == Qt::KeyboardModifier::ControlModifier )
	{
		qDebug() << event->angleDelta().y();
		qreal step = qBound( 0.1, transform().m11() + event->delta() / 1800.0, 2.0 );
		auto trans = transform();
		setTransform( QTransform().translate( trans.m31(), trans.m32() ).scale( step, step ) );
	}
	else
	{
		QGraphicsView::wheelEvent( event );
	}
}

void XS::NodeGraphicsView::mousePressEvent( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent( event );
}

void XS::NodeGraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent( event );
}

void XS::NodeGraphicsView::mouseReleaseEvent( QMouseEvent * event )
{
	QGraphicsView::mouseReleaseEvent( event );
}

void XS::NodeGraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
{
	int step = 50;
	qreal grid = step * transform().m11();
	
	qreal top = int( rect.top() ) - ( int( rect.top() ) % ( step ) );
	qreal left = int( rect.left() ) - ( int( rect.left() ) % ( step ) );

	painter->setPen( QPen( backgroundBrush().color().light(), 1, Qt::PenStyle::DashLine ) );
	for ( qreal x = left; x < rect.right(); x += grid )
		painter->drawLine( QPointF( x, rect.top() ), QPointF( x, rect.bottom() ) );
	for ( qreal y = top; y < rect.bottom(); y += grid )
		painter->drawLine( QPointF( rect.left(), y ), QPointF( rect.right(), y ) );

	painter->setPen( QPen( backgroundBrush().color().light(), 2, Qt::PenStyle::DashLine ) );
	for ( qreal x = left; x < rect.right(); x += grid * 3 )
		painter->drawLine( QPointF( x, rect.top() ), QPointF( x, rect.bottom() ) );
	for ( qreal y = top; y < rect.bottom(); y += grid * 3 )
		painter->drawLine( QPointF( rect.left(), y ), QPointF( rect.right(), y ) );
}

void XS::NodeGraphicsView::drawForeground( QPainter * painter, const QRectF & rect )
{
}
