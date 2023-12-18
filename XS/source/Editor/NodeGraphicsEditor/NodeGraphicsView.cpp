#include "NodeGraphicsView.h"

#include <QPixmap>
#include <QScrollBar>
#include <QWheelEvent>

#include "NodeGraphicsItemGroup.h"

XS::NodeGraphicsView::NodeGraphicsView( QWidget * parent )
	: QGraphicsView( parent )
{
	setMouseTracking( true );
	setCacheMode( CacheNone );
	setDragMode( QGraphicsView::NoDrag );
	setViewportUpdateMode( FullViewportUpdate );
	setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
}

XS::NodeGraphicsView::~NodeGraphicsView()
{

}

void XS::NodeGraphicsView::wheelEvent( QWheelEvent * event )
{
	if ( event->modifiers() == Qt::KeyboardModifier::ControlModifier && event->buttons() == 0 )
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
	if ( itemAt( event->pos() ) == nullptr && event->buttons() == Qt::RightButton && event->modifiers() == 0 )
	{
		_isMove = true;
		_movePos = event->pos();
		_centerPos = mapToScene( width() / 2, height() / 2 );
	}
	else if( itemAt( event->pos() ) == nullptr && event->buttons() == Qt::LeftButton && event->modifiers() == 0 )
	{
		setDragMode( QGraphicsView::RubberBandDrag );
	}

	QGraphicsView::mousePressEvent( event );
}

void XS::NodeGraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	if ( _isMove )
	{
		centerOn( _centerPos - ( event->pos() - _movePos ) );
	}

	QGraphicsView::mouseMoveEvent( event );
}

void XS::NodeGraphicsView::mouseReleaseEvent( QMouseEvent * event )
{
	if ( dragMode() == QGraphicsView::RubberBandDrag )
	{
		auto list = items( rubberBandRect(), Qt::IntersectsItemShape);
		if ( !list.isEmpty() )
		{
			auto group = new NodeGraphicsItemGroup( nullptr );
			scene()->addItem( group );
			for ( auto & it : list )
			{
				group->addToGroup( dynamic_cast<XS::NodeGraphicsItem *>( it ) );
			}
		}
	}

	QGraphicsView::mouseReleaseEvent( event );

	_isMove = false;
	setDragMode( QGraphicsView::NoDrag );
}

void XS::NodeGraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
{
	QPixmap pixmap( "SkinIcons:/images/node/background.png" );
	painter->drawTiledPixmap( rect, pixmap );
}

void XS::NodeGraphicsView::drawForeground( QPainter * painter, const QRectF & rect )
{
}
