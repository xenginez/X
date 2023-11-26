#include "NodeGraphicsItem.h"

#include <QGraphicsSceneEvent>

NodeGraphicsItem::NodeGraphicsItem( QGraphicsItem * parent )
	: QGraphicsObject( parent )
{
	setFlag( ItemIsMovable, true );
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
	return property( "isHovered" ).toBool();
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

	drawBackground( painter, rect );
	drawTitleBar( painter, QRectF( rect.left() + 5, rect.top() + 5, rect.width() - 5, 20 ) );
	drawContext(painter, QRectF( rect.left() + 5, rect.top() + 25, rect.width() - 5, rect.height() - 5 ) );
}

void NodeGraphicsItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsObject::hoverEnterEvent( event );

	setProperty( "isHovered", true );
}

void NodeGraphicsItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	QGraphicsObject::hoverLeaveEvent( event );

	setProperty( "isHovered", false );
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

	}
	return menu;
}

void NodeGraphicsItem::drawBorder( QPainter * painter, const QRectF & rect )
{
}

void NodeGraphicsItem::drawTitleBar( QPainter * painter, const QRectF & rect )
{
}

void NodeGraphicsItem::drawBackground( QPainter * painter, const QRectF & rect )
{
}
