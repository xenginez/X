#include "NodeWidget.h"

#include <QStyle>
#include <QWheelEvent>
#include <QApplication>

namespace
{
	static constexpr XE::float32 GRID_STEP = 64;
	static constexpr XE::float32 MIN_SCALE = 0.2f;
	static constexpr XE::float32 MAX_SCALE = 5.0f;

	class NodeConnect : public QGraphicsItem
	{
	public:
		NodeConnect();

		~NodeConnect() override;

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
}

XS::NodeItem::NodeItem()
{

}

XS::NodeItem::~NodeItem()
{

}

void XS::NodeItem::paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget /*= 0 */ )
{

}

QVariant XS::NodeItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
	return QGraphicsItem::itemChange( change, value );
}

void XS::NodeItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mousePressEvent( event );
}

void XS::NodeItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseMoveEvent( event );
}

void XS::NodeItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseReleaseEvent( event );
}

void XS::NodeItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsItem::hoverEnterEvent( event );
}

void XS::NodeItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsItem::hoverLeaveEvent( event );
}

void XS::NodeItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsItem::hoverMoveEvent( event );
}

void XS::NodeItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseDoubleClickEvent( event );
}

void XS::NodeItem::contextMenuEvent( QGraphicsSceneContextMenuEvent * event )
{
	QGraphicsItem::contextMenuEvent( event );
}

struct XS::NodeWidget::Private
{
	QPoint _LastPos = {};
	bool _SceneMove = false;
	QVector< XS::NodeItem * > _Items;
	QVector< NodeConnect * > _Connects;
};

XS::NodeWidget::NodeWidget( QWidget * parent /*= nullptr */ )
	: QGraphicsView( new QGraphicsScene( parent ), parent ), _p( new Private )
{
	setMouseTracking( true );
	setCursor( Qt::PointingHandCursor );
	setDragMode( QGraphicsView::NoDrag );
	setRenderHint( QPainter::Antialiasing );
	setResizeAnchor( QGraphicsView::AnchorUnderMouse );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );

	setSceneRect( INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX );
}

XS::NodeWidget::~NodeWidget()
{
	delete _p;
}

void XS::NodeWidget::contextMenuEvent( QContextMenuEvent * event )
{
	QGraphicsView::contextMenuEvent( event );
}

void XS::NodeWidget::wheelEvent( QWheelEvent * event )
{
	auto dt = event->angleDelta();

	if ( dt.y() == 0 )
	{
		event->ignore();
		return;
	}

	XE::float32 step = 1.2f;
	XE::float32 factor = 1.0f;
	
	if ( dt.y() > 0.0f )
	{
		if ( transform().m11() > MAX_SCALE )
			return;

		factor = std::pow( step, 1.0f );
	}
	else
	{
		if ( transform().m11() < MIN_SCALE )
			return;

		factor = std::pow( step, -1.0f );
	}

	scale( factor, factor );
}

void XS::NodeWidget::keyPressEvent( QKeyEvent * event )
{
	QGraphicsView::keyPressEvent( event );
}

void XS::NodeWidget::keyReleaseEvent( QKeyEvent * event )
{
	QGraphicsView::keyReleaseEvent( event );
}

void XS::NodeWidget::mousePressEvent( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent( event );

	if ( itemAt( event->pos() ) == nullptr && event->buttons() == Qt::LeftButton )
	{
		_p->_SceneMove = true;
		_p->_LastPos = event->pos();

		setCursor( Qt::OpenHandCursor );
	}
}

void XS::NodeWidget::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent( event );

	if ( _p->_SceneMove )
	{
		setTransformationAnchor( QGraphicsView::AnchorViewCenter );
		{
			auto dt = ( mapToScene( event->pos() ) - mapToScene( _p->_LastPos ) ) * transform().m11();

			centerOn( mapToScene( QPoint( viewport()->rect().width() / 2 - dt.x(), viewport()->rect().height() / 2 - dt.y() ) ) );

			_p->_LastPos = event->pos();
		}
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	}
}

void XS::NodeWidget::mouseReleaseEvent( QMouseEvent * event )
{
	QGraphicsView::mouseReleaseEvent( event );

	if ( _p->_SceneMove )
	{
		_p->_SceneMove = false;
		setCursor( Qt::PointingHandCursor );
	}
}

void XS::NodeWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
	QGraphicsView::mouseDoubleClickEvent( event );

	if ( itemAt( event->pos() ) == nullptr )
	{
		scale( 1.0f / transform().m11(), 1.0f / transform().m22() );

		setTransformationAnchor( QGraphicsView::AnchorViewCenter );
		{
			centerOn( 0, 0 );
		}
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	}
}

void XS::NodeWidget::drawBackground( QPainter * painter, const QRectF & rect )
{
	QGraphicsView::drawBackground( painter, rect );

	painter->save();
	{
		auto drawGrid = [&]( qreal step )
		{
			QPointF tl = mapToScene( this->rect().topLeft() );
			QPointF br = mapToScene( this->rect().bottomRight() );

			qreal left = std::floor( tl.x() / step - 0.5 );
			qreal right = std::floor( br.x() / step + 1.0 );
			qreal bottom = std::floor( tl.y() / step - 0.5 );
			qreal top = std::floor( br.y() / step + 1.0 );

			for ( int i = int( left ); i <= int( right ); ++i )
			{
				painter->drawLine( i * step, bottom * step, i * step, top * step );
			}

			for ( int i = int( bottom ); i <= int( top ); ++i )
			{
				painter->drawLine( left * step, i * step, right * step, i * step );
			}
		};

		painter->setPen( QPen( qApp->palette().color( QPalette::PlaceholderText ), 1.0 ) );
		drawGrid( GRID_STEP );

		painter->setPen( QPen( qApp->palette().color( QPalette::Text ), 1.0 ) );
		drawGrid( GRID_STEP * 10 );
	}
	painter->restore();
}

void XS::NodeWidget::showEvent( QShowEvent * event )
{
	QGraphicsView::showEvent( event );
}
