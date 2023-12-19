#include "NodeGraphicsItemGroup.h"

#include <QDebug>
#include <QColorDialog>
#include <QGraphicsSceneEvent>

XS::NodeGraphicsItemGroup::NodeGraphicsItemGroup( const QList<QGraphicsItem * > & items, QGraphicsScene * scene, QGraphicsItem * parent )
	: NodeGraphicsItem( scene, parent )
{
	QPointF pos = scene->sceneRect().bottomRight();
	for ( auto it : items )
	{
		if ( it->parentItem() == nullptr )
		{
			auto p = it->pos();

			if ( p.x() < pos.x() )
				pos.setX( p.x() );
			if ( p.y() < pos.y() )
				pos.setY( p.y() );
		}
	}
	
	pos.setX( pos.x() - 15 );
	pos.setY( pos.y() - 15 - titleBarRect().height() );
	setPos( pos );

	for ( auto it : items )
	{
		if ( it->parentItem() == nullptr )
		{
			it->setPos( mapFromScene( it->pos() ) );
			it->setParentItem( this );
			it->setSelected( false );

			if ( auto obj = dynamic_cast<QGraphicsObject *>( it ) )
			{
				connect( obj, &QGraphicsObject::xChanged, this, &NodeGraphicsItemGroup::updateBoundingRect );
				connect( obj, &QGraphicsObject::yChanged, this, &NodeGraphicsItemGroup::updateBoundingRect );
			}
		}
	}

	updateBoundingRect();
}

XS::NodeGraphicsItemGroup::~NodeGraphicsItemGroup()
{
}

void XS::NodeGraphicsItemGroup::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if ( colorButtonRect().contains( event->pos() ) && event->buttons() == Qt::LeftButton )
	{
		QColorDialog dialog;
		dialog.setCurrentColor( _color );
		if ( dialog.exec() == QDialog::Accepted )
		{
			_color = dialog.currentColor();

			update();
		}
	}
	else
	{
		NodeGraphicsItem::mousePressEvent( event );
	}
}

QMenu * XS::NodeGraphicsItemGroup::contextMenu()
{
	auto menu = NodeGraphicsItem::contextMenu();
	{
		menu->addAction( tr( "release" ), [this]()
		{
			auto items = childItems();
			for ( auto it : items )
			{
				if ( auto obj = dynamic_cast<QGraphicsObject *>( it ) )
				{
					disconnect( obj, &QGraphicsObject::xChanged, this, &NodeGraphicsItemGroup::updateBoundingRect );
					disconnect( obj, &QGraphicsObject::yChanged, this, &NodeGraphicsItemGroup::updateBoundingRect );
				}

				if ( parentItem() )
					it->setPos( mapToParent( it->pos() ) );
				else
					it->setPos( mapToScene( it->pos() ) );
				
				it->setParentItem( parentItem() );
			}
			this->deleteLater();
		} );
	}
	return menu;
}

QRectF XS::NodeGraphicsItemGroup::contextRect() const
{
	auto rect = childrenBoundingRect().marginsAdded( QMargins( 10, 10, 10, 10 ) );

	return QRectF( 0, 0, rect.width(), rect.height() );
}

QBrush XS::NodeGraphicsItemGroup::titleBarBrush() const
{
	return QBrush( _color );
}

void XS::NodeGraphicsItemGroup::drawTitleBar( QPainter * painter, const QRectF & rect )
{
	NodeGraphicsItem::drawTitleBar( painter, rect );

	painter->save();
	{
		QLinearGradient gradient( QPointF( colorButtonRect().left(), 0 ), QPointF( colorButtonRect().right(), 0 ) );
		gradient.setColorAt( 0.0, Qt::red );
		gradient.setColorAt( 0.5, Qt::green );
		gradient.setColorAt( 1.0, Qt::blue );
		painter->setBrush( QBrush( gradient ) );
		painter->setPen( QPen( QColor( 255 - _color.red(), 255 - _color.green(), 255 - _color.blue() ), 1 ) );
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

void XS::NodeGraphicsItemGroup::updateBoundingRect()
{
	if ( _isUpdate )
		return;

	_isUpdate = true;

	QMetaObject::invokeMethod( this, [this]()
	{
		auto items = childItems();
		QPointF new_pos = scene()->sceneRect().bottomRight();
		for ( auto it : items )
		{
			auto p = mapToScene( it->pos() );

			if ( p.x() < new_pos.x() )
				new_pos.setX( p.x() );
			if ( p.y() < new_pos.y() )
				new_pos.setY( p.y() );
		}

		new_pos.setX( new_pos.x() - 15 );
		new_pos.setY( new_pos.y() - 15 - titleBarRect().height() );

		auto dt = pos() - new_pos;
		setPos( new_pos );

		for ( auto it : items )
		{
			it->setPos( it->pos() + dt );
		}

		update();

		_isUpdate = false;
	}, Qt::QueuedConnection );
}
