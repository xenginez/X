#include "NodeGraphicsItem.h"

#include <QPainter>
#include <QGraphicsSceneEvent>

NodeGraphicsItem::NodeGraphicsItem( QGraphicsItem * parent )
	: QGraphicsObject( parent )
{
	setAcceptHoverEvents( true );
	setFlag( ItemIsMovable, true );
	setFlag( ItemIsFocusable, true );
	setFlag( ItemIsSelectable, true );
	setFlag( ItemAcceptsInputMethod, true );
	setFlag( ItemContainsChildrenInShape, true );
	setCacheMode( ItemCoordinateCache );
}

NodeGraphicsItem::~NodeGraphicsItem()
{
}

bool NodeGraphicsItem::isHovered() const
{
	return _isHovered;
}

QRectF NodeGraphicsItem::boundingRect() const
{
	auto sz = contextRect().size();
	sz.setHeight( sz.height() + 20 );
	return QRectF( QPointF(), sz ).marginsAdded( QMargins( 5, 5, 5, 5 ) );
}

void NodeGraphicsItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	auto rect = boundingRect();

	if ( isHovered() || isSelected() )
	{
		drawBorder( painter, boundingRect() );
	}

	rect = rect.marginsRemoved( QMargins( 5, 5, 5, 5 ) );

	drawTitleBar( painter, QRectF( rect.left(), rect.top(), rect.width(), 20 ) );
	drawContextBody(painter, QRectF( rect.left(), rect.top() + 20, rect.width(), rect.height() - 20 ) );
}

void NodeGraphicsItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsObject::hoverEnterEvent( event );

	_isHovered = true;
}

void NodeGraphicsItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsObject::hoverLeaveEvent( event );

	_isHovered = false;
}

void NodeGraphicsItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsObject::mousePressEvent( event );
}

void NodeGraphicsItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsObject::mouseMoveEvent( event );
}

void NodeGraphicsItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsObject::mouseReleaseEvent( event );
}

void NodeGraphicsItem::contextMenuEvent( QGraphicsSceneContextMenuEvent * event )
{
	QGraphicsObject::contextMenuEvent( event );
	
	auto menu = contextMenu();
	menu->exec( QCursor::pos() );
	delete menu;
}

QMenu * NodeGraphicsItem::contextMenu() const
{
	QMenu * menu = new QMenu;
	{
		menu->addAction( tr( "delete" ), []()
		{

		} );
		menu->addSeparator();
	}
	return menu;
}

QRectF NodeGraphicsItem::contextRect() const
{
	return QRectF( 0, 0, 100, 100 );
}

void NodeGraphicsItem::drawBorder( QPainter * painter, const QRectF & rect )
{
	painter->save();

	if( isSelected() )
		painter->setPen( QPen( Qt::yellow, 3 ) );
	else
		painter->setPen( QPen( Qt::yellow, 3, Qt::PenStyle::DashDotDotLine ) );

	QPainterPath path;
	path.moveTo( rect.topLeft() );
	path.lineTo( rect.topRight() );
	path.lineTo( rect.bottomRight() );
	path.lineTo( rect.bottomLeft() );
	path.lineTo( rect.topLeft() );
	painter->drawPath( path );

	painter->restore();
}

void NodeGraphicsItem::drawTitleBar( QPainter * painter, const QRectF & rect )
{

}

void NodeGraphicsItem::drawContextBody( QPainter * painter, const QRectF & rect )
{
	painter->save();

	painter->setBrush( QBrush( Qt::white ) );

	painter->drawRect( rect );

	painter->restore();
}
