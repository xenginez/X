#include "NodeGraphicsItem.h"

#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#define TITLE_HEIGHT 20

NodeGraphicsItem::NodeGraphicsItem( QGraphicsItem * parent )
	: QGraphicsObject( parent ), _timer( this ), _document( "aaaaaaaa", this )
{
	setAcceptHoverEvents( true );
	setFlag( ItemIsFocusable, true );
	setFlag( ItemIsSelectable, true );
	setFlag( ItemAcceptsInputMethod, true );
	setFlag( ItemContainsChildrenInShape, true );
	setCacheMode( ItemCoordinateCache );

	_cursor = QTextCursor( &_document );
	connect( &_timer, &QTimer::timeout, [this]()
	{
		_cursorShow = !_cursorShow;
		update();
	} );
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
	if ( _titleEdit && ( event->modifiers() & Qt::AltModifier ) == 0 )
	{
		if ( event->modifiers() == Qt::ControlModifier )
		{
			if ( event->key() == Qt::Key_A )
			{
				_cursor.movePosition( QTextCursor::Start );
				_cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor );

				QTextCharFormat fmt = _cursor.charFormat();
				fmt.setForeground( QColor( view()->palette().color( QPalette::ColorRole::WindowText ) ) );
				fmt.setBackground( QColor( view()->palette().color( QPalette::ColorRole::Highlight ) ) );
				_cursor.setCharFormat( fmt );
			}
			else if ( event->key() == Qt::Key_C )
			{
				if ( _cursor.hasSelection() )
				{
					qApp->clipboard()->setText( _cursor.selectedText() );
				}
			}
			else if ( event->key() == Qt::Key_V )
			{
				if ( _cursor.hasSelection() )
				{
					_cursor.removeSelectedText();
				}

				_cursor.insertText( qApp->clipboard()->text() );
			}
		}
		else
		{
			if ( event->key() >= Qt::Key_Space && event->key() <= Qt::Key_AsciiTilde )
			{
				if ( _cursor.hasSelection() )
				{
					_cursor.removeSelectedText();
				}

				_cursor.insertText( event->text() );
			}
			else if ( event->key() == Qt::Key_Left )
			{
				if ( _cursor.hasSelection() )
				{
					QTextCharFormat fmt = _cursor.charFormat();
					fmt.setForeground( QColor( view()->palette().color( QPalette::ColorRole::WindowText ) ) );
					fmt.setBackground( titleBarBrush() );
					_cursor.setCharFormat( fmt );

					_cursor.clearSelection();
				}
				_cursor.setPosition( _cursor.position() - 1 );
			}
			else if ( event->key() == Qt::Key_Right )
			{
				if ( _cursor.hasSelection() )
				{
					QTextCharFormat fmt = _cursor.charFormat();
					fmt.setForeground( QColor( view()->palette().color( QPalette::ColorRole::WindowText ) ) );
					fmt.setBackground( titleBarBrush() );
					_cursor.setCharFormat( fmt );

					_cursor.clearSelection();
				}
				_cursor.setPosition( _cursor.position() + 1 );
			}
			else if ( event->key() == Qt::Key_Home )
			{
				if ( _cursor.hasSelection() )
				{
					QTextCharFormat fmt = _cursor.charFormat();
					fmt.setForeground( QColor( view()->palette().color( QPalette::ColorRole::WindowText ) ) );
					fmt.setBackground( titleBarBrush() );
					_cursor.setCharFormat( fmt );

					_cursor.clearSelection();
				}
				_cursor.movePosition( QTextCursor::Start );
			}
			else if ( event->key() == Qt::Key_End )
			{
				if ( _cursor.hasSelection() )
				{
					QTextCharFormat fmt = _cursor.charFormat();
					fmt.setForeground( QColor( view()->palette().color( QPalette::ColorRole::WindowText ) ) );
					fmt.setBackground( titleBarBrush() );
					_cursor.setCharFormat( fmt );

					_cursor.clearSelection();
				}
				_cursor.movePosition( QTextCursor::End );
			}
			else
			{
				qDebug() << event->text() << ":" << event->key();
			}
		}

		update();
	}
	else
	{
		QGraphicsObject::keyPressEvent( event );
	}
}

void NodeGraphicsItem::keyReleaseEvent( QKeyEvent * event )
{
	if ( _titleEdit )
	{
		if ( event->key() == Qt::Key_Return )
		{
			_cursorShow = false;
			_titleEdit = false;
			_timer.stop();
		}

		update();
	}
	else
	{
		QGraphicsObject::keyReleaseEvent( event );
	}
}

void NodeGraphicsItem::focusOutEvent( QFocusEvent * event )
{
	QGraphicsObject::focusOutEvent( event );

	_cursorShow = false;
	_titleEdit = false;
	_timer.stop();
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

	if ( event->buttons() == Qt::LeftButton )
	{
		auto rect = boundingRect();
		QRectF title_rect( 5, 5, rect.width() - 5, TITLE_HEIGHT );
		if ( title_rect.contains( event->pos() ) )
		{
			_isMoveed = true;
			_pos = pos();
			_movePos = mapToScene( event->pos() );
		}
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
		_cursor.movePosition( QTextCursor::Start );
		_cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor );

		QTextCharFormat fmt = _cursor.charFormat();
		fmt.setForeground( QColor( view()->palette().color( QPalette::ColorRole::WindowText ) ) );
		fmt.setBackground( QColor( view()->palette().color( QPalette::ColorRole::Highlight ) ) );
		_cursor.setCharFormat( fmt );

		_titleEdit = true;
		_cursorShow = true;
		_timer.start( 500 );

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

QBrush NodeGraphicsItem::titleBarBrush() const
{
	return QBrush( Qt::black );
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

	painter->setBrush( titleBarBrush() );
	painter->drawRect( rect );

	painter->setPen( QPen( view()->palette().color( QPalette::ColorRole::WindowText ), 1 ) );
	QFontMetrics metrics( painter->font() );
	auto text_rect = rect.marginsRemoved( QMarginsF( 5, 0, 0, 0 ) );
	if ( _titleEdit )
	{
		_document.drawContents( painter, text_rect );
		if ( _cursorShow )
		{
			auto s = _document.toPlainText();
			auto i = _cursor.position();
			int x = metrics.width( _document.toPlainText(), _cursor.position() );
			painter->drawLine( QPoint( text_rect.left() + x, text_rect.top() + 1 ), QPoint( text_rect.left() + x, text_rect.bottom() - 1 ) );
		}
	}
	else
	{
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
