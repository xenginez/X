#include "NodeGraphicsItemGroup.h"

#include <QDebug>

XS::NodeGraphicsItemGroup::NodeGraphicsItemGroup( QGraphicsItem * parent )
	: NodeGraphicsItem( parent )
{
}

XS::NodeGraphicsItemGroup::~NodeGraphicsItemGroup()
{
}

QBrush XS::NodeGraphicsItemGroup::titleBarBrush() const
{
	return QBrush( _color );
}

void XS::NodeGraphicsItemGroup::addToGroup( XS::NodeGraphicsItem * item )
{
	if ( item != nullptr && item != this && !childItems().contains( item ) )
	{
		item->setParentItem( this );
		updatePos();
	}
}

void XS::NodeGraphicsItemGroup::removeFromGroup( XS::NodeGraphicsItem * item )
{
	if ( item != nullptr && item != this && childItems().contains( item ) )
	{
		item->setParentItem( parentItem() );
		updatePos();
	}
}

QRectF XS::NodeGraphicsItemGroup::contextRect() const
{
	auto rect = childrenBoundingRect().marginsAdded( QMargins( 10, 10, 10, 10 ) );
	return QRectF( 0, 0, rect.width(), rect.height() );
}

void XS::NodeGraphicsItemGroup::drawTitleBar( QPainter * painter, const QRectF & rect )
{
	NodeGraphicsItem::drawTitleBar( painter, rect );

	painter->save();
	{
		painter->setPen( QPen( QColor( 255 - _color.red(), 255 - _color.green(), 255 - _color.blue() ), 1 ) );
		painter->setBrush( QBrush( _color ) );
		painter->drawRect( colorButtonRect() );
	}
	painter->restore();
}

void XS::NodeGraphicsItemGroup::drawContextBody( QPainter * painter, const QRectF & rect )
{
	painter->save();
	{
		auto c = _color;
		c.setAlpha( 100 );
		painter->setBrush( QBrush( c ) );
		painter->drawRect( rect );
	}
	painter->restore();
}

void XS::NodeGraphicsItemGroup::updatePos()
{
	setPos( childrenBoundingRect().topLeft() );
	update();
}

QRectF XS::NodeGraphicsItemGroup::colorButtonRect() const
{
	auto r = titleBarRect();
	r.setRight( r.right() - 5 );
	r.setLeft( r.right() - 10 );
	auto s = ( r.height() - 10 ) / 2;
	r.setTop( r.top() + s );
	r.setBottom( r.bottom() - s );
	return r;
}
