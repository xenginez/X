#include "NodeGraphicsItem.h"

#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QTextCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#define TITLE_HEIGHT 20

NodeGraphicsItem::NodeGraphicsItem( QGraphicsItem * parent )
	: QGraphicsObject( parent ), _document( "aaaaaaaa", this)
{
	setAcceptHoverEvents( true );
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

QGraphicsView * NodeGraphicsItem::view() const
{
	return scene()->views().front();
}

QRectF NodeGraphicsItem::boundingRect() const
{
	auto cr = contextRect();

	return QRectF( QPointF(), QSizeF( cr.width(), TITLE_HEIGHT + cr.height() ) ).marginsAdded( QMargins( 5, 5, 5, 5 ) );
}

void NodeGraphicsItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	auto rect = boundingRect();

	if ( isHovered() || isSelected() )
	{
		drawBorder( painter, rect );
	}

	rect = rect.marginsRemoved( QMargins( 5, 5, 5, 5 ) );

	drawTitleBar( painter, QRectF( rect.left(), rect.top(), rect.width(), TITLE_HEIGHT ) );
	drawContextBody(painter, QRectF( rect.left(), rect.top() + TITLE_HEIGHT, rect.width(), rect.height() - TITLE_HEIGHT ) );
}

void NodeGraphicsItem::keyPressEvent( QKeyEvent * event )
{
	QGraphicsObject::keyPressEvent( event );
}

void NodeGraphicsItem::keyReleaseEvent( QKeyEvent * event )
{
	if ( _titleEdit && event->key() == Qt::Key_Enter )
	{
		_titleEdit = false;
	}

	QGraphicsObject::keyReleaseEvent( event );
}

void NodeGraphicsItem::focusOutEvent( QFocusEvent * event )
{
	QGraphicsObject::focusOutEvent( event );

	_titleEdit = false;
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

	auto rect = boundingRect();
	QRectF title_rect( 5, 5, rect.width() - 5, TITLE_HEIGHT );
	if ( title_rect.contains( event->pos() ) )
	{
		_isMoveed = true;
		_pos = pos();
		_movePos = mapToScene( event->pos() );
	}
}

void NodeGraphicsItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsObject::mouseMoveEvent( event );

	if ( _isMoveed )
	{
		auto dt = mapToScene( event->pos() ) - _movePos;
		setPos( _pos + dt );
	}
}

void NodeGraphicsItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsObject::mouseReleaseEvent( event );

	_isMoveed = false;
}

void NodeGraphicsItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	auto title_rect = boundingRect().marginsRemoved( QMargins( 5, 5, 5, 5 ) );
	title_rect.setHeight( TITLE_HEIGHT );

	if ( title_rect.contains( event->pos() ) )
	{
		QTextCursor cursor( &_document );
		cursor.movePosition( QTextCursor::Start );
		cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor );
		cursor.select( QTextCursor::BlockUnderCursor );
		QTextCharFormat fmt;
		fmt.setForeground( QColor( Qt::white ) );
		fmt.setBackground( QColor( Qt::blue ) );
		cursor.mergeCharFormat( fmt );

		_titleEdit = true;

		update();
	}
	else
	{
		QGraphicsObject::mouseDoubleClickEvent( event );
	}
}

void NodeGraphicsItem::contextMenuEvent( QGraphicsSceneContextMenuEvent * event )
{
	QGraphicsObject::contextMenuEvent( event );
	
	auto menu = contextMenu();
	menu->exec( QCursor::pos() );
	delete menu;
}

void NodeGraphicsItem::inputMethodEvent( QInputMethodEvent * event )
{
	QGraphicsObject::inputMethodEvent( event );
}

QVariant NodeGraphicsItem::inputMethodQuery( Qt::InputMethodQuery query ) const
{
	switch ( query )
	{
	case Qt::ImEnabled:
		return true;
	case Qt::ImCursorRectangle:
		return QRectF();// cursorRect();
	case Qt::ImAnchorRectangle:
		return QRectF();// d->rectForPosition( d->cursor.anchor() );
	case Qt::ImFont:
		return view()->font();
	case Qt::ImCursorPosition:
	{
		return QPointF();

		// const QPointF pt = argument.toPointF();
		// if ( !pt.isNull() )
		// 	return QVariant( cursorForPosition( pt ).position() - block.position() );
		// return QVariant( d->cursor.position() - block.position() );
	}
	case Qt::ImSurroundingText:
		return QString();// QVariant( block.text() );
	case Qt::ImCurrentSelection:
		return QString();// QVariant( d->cursor.selectedText() );
	case Qt::ImMaximumTextLength:
		return QVariant(); // No limit.
	case Qt::ImAnchorPosition:
		return 0;
		//return QVariant( d->cursor.anchor() - block.position() );
	case Qt::ImHints:
		return {};
	case Qt::ImAbsolutePosition:
	{
		return QPointF();

		// const QPointF pt = argument.toPointF();
		// if ( !pt.isNull() )
		// 	return QVariant( cursorForPosition( pt ).position() );
		// return QVariant( d->cursor.position() );
	}
	case Qt::ImTextAfterCursor:
	{
		return QString();

		// int maxLength = argument.isValid() ? argument.toInt() : 1024;
		// QTextCursor tmpCursor = d->cursor;
		// int localPos = d->cursor.position() - block.position();
		// QString result = block.text().mid( localPos );
		// while ( result.length() < maxLength )
		// {
		// 	int currentBlock = tmpCursor.blockNumber();
		// 	tmpCursor.movePosition( QTextCursor::NextBlock );
		// 	if ( tmpCursor.blockNumber() == currentBlock )
		// 		break;
		// 	result += QLatin1Char( '\n' ) + tmpCursor.block().text();
		// }
		// return QVariant( result );
	}
	case Qt::ImTextBeforeCursor:
	{
		return QString();

		// int maxLength = argument.isValid() ? argument.toInt() : 1024;
		// QTextCursor tmpCursor = d->cursor;
		// int localPos = d->cursor.position() - block.position();
		// int numBlocks = 0;
		// int resultLen = localPos;
		// while ( resultLen < maxLength )
		// {
		// 	int currentBlock = tmpCursor.blockNumber();
		// 	tmpCursor.movePosition( QTextCursor::PreviousBlock );
		// 	if ( tmpCursor.blockNumber() == currentBlock )
		// 		break;
		// 	numBlocks++;
		// 	resultLen += tmpCursor.block().length();
		// }
		// QString result;
		// while ( numBlocks )
		// {
		// 	result += tmpCursor.block().text() + QLatin1Char( '\n' );
		// 	tmpCursor.movePosition( QTextCursor::NextBlock );
		// 	--numBlocks;
		// }
		// result += block.text().midRef( 0, localPos );
		// return QVariant( result );
	}
	default:
		return QVariant();
	}

	return QGraphicsObject::inputMethodQuery( query );
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
	painter->save();

	painter->setBrush( QBrush( Qt::black ) );
	painter->drawRect( rect );

	painter->setPen( QPen( Qt::white, 1 ) );
	auto text_rect = rect.marginsRemoved( QMarginsF( 5, 0, 0, 0 ) );
	if ( _titleEdit )
	{
		_document.drawContents( painter, text_rect );
	}
	else
	{
		QFontMetrics metrics( painter->font() );
		QString elidedText = metrics.elidedText( _document.toPlainText(), Qt::ElideRight, text_rect.width() );
		painter->drawText( text_rect, elidedText, QTextOption( Qt::AlignLeft | Qt::AlignVCenter ) );
	}

	painter->restore();
}

void NodeGraphicsItem::drawContextBody( QPainter * painter, const QRectF & rect )
{
	painter->save();

	painter->setBrush( QBrush( Qt::white ) );

	painter->drawRect( rect );

	painter->restore();
}
