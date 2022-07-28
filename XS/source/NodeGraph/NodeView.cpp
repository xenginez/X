#include "NodeView.h"

#include <QStyle>
#include <QApplication>

XS::NodeView::NodeView( XS::NodeScene * scene, QWidget * parent /*= nullptr */ )
	: QGraphicsView( scene, parent ), _Scene( scene )
{

}

XS::NodeView::~NodeView()
{

}

void XS::NodeView::contextMenuEvent( QContextMenuEvent * event )
{
	QGraphicsView::contextMenuEvent( event );
}

void XS::NodeView::wheelEvent( QWheelEvent * event )
{
	QGraphicsView::wheelEvent( event );
}

void XS::NodeView::keyPressEvent( QKeyEvent * event )
{
	QGraphicsView::keyPressEvent( event );
}

void XS::NodeView::keyReleaseEvent( QKeyEvent * event )
{
	QGraphicsView::keyReleaseEvent( event );
}

void XS::NodeView::mousePressEvent( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent( event );
}

void XS::NodeView::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent( event );
}

void XS::NodeView::drawBackground( QPainter * painter, const QRectF & rect )
{
	QGraphicsView::drawBackground( painter, rect );

	qreal left = ( rect.left() / 128 ) * 128;
	qreal right = ( rect.right() / 128 ) * 128;
	qreal top = ( rect.top() / 128 ) * 128;
	qreal bottom = ( rect.bottom() / 128 ) * 128;

	QPen pen;
	pen.setColor( qApp->palette().color( QPalette::Link ) );
	pen.setWidth( 0 );
	pen.setStyle( Qt::DashLine );

	painter->setPen( pen );

	for ( int i = 0; i >= top; i -= 128 )
	{
		painter->drawLine( left, i, right, i );
	}

	for ( int i = 0; i <= bottom; i += 128 )
	{
		painter->drawLine( left, i, right, i );
	}

	for ( int i = 0; i <= right; i += 128 )
	{
		painter->drawLine( i, top, i, bottom );
	}

	for ( int i = 0; i >= left; i -= 128 )
	{
		painter->drawLine( i, top, i, bottom );
	}
}

void XS::NodeView::showEvent( QShowEvent * event )
{
	QGraphicsView::showEvent( event );
}
