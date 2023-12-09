#include "NodeGraphicsView.h"

#include <QPixmap>
#include <QScrollBar>
#include <QWheelEvent>

XS::NodeGraphicsView::NodeGraphicsView( QWidget * parent )
	: QGraphicsView( parent )
{
	setMouseTracking( true );
	setCacheMode( CacheNone );
	setViewportUpdateMode( FullViewportUpdate );
	setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
}

XS::NodeGraphicsView::~NodeGraphicsView()
{

}

void XS::NodeGraphicsView::wheelEvent( QWheelEvent * event )
{
	if ( event->modifiers() == Qt::KeyboardModifier::ControlModifier )
	{
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

	if ( itemAt( event->pos() ) == nullptr && event->buttons() == Qt::MouseButton::RightButton )
	{
		_isMove = true;
		_movePos = event->pos();
		_centerPos = mapToScene( width() / 2, height() / 2 );
	}
}

void XS::NodeGraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent( event );

	if ( _isMove )
	{
		centerOn( _centerPos - ( event->pos() - _movePos ) );
	}
}

void XS::NodeGraphicsView::mouseReleaseEvent( QMouseEvent * event )
{
	QGraphicsView::mouseReleaseEvent( event );

	_isMove = false;
}

void XS::NodeGraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
{
	QPixmap pixmap( "SkinIcons:/images/node/background.png" );
	painter->drawTiledPixmap( rect, pixmap );
}

void XS::NodeGraphicsView::drawForeground( QPainter * painter, const QRectF & rect )
{
}
