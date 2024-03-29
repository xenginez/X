/*!
 * \file	NodeGraphicsItem.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC
#define NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC

#include <QMenu>
#include <QTimer>
#include <QTextCursor>
#include <QTextDocument>
#include <QGraphicsItem>
#include <QGraphicsView>

#include "Global.h"

BEG_XS_NAMESPACE

class NodeGraphicsItem : public QGraphicsObject
{
	Q_OBJECT

public:
	NodeGraphicsItem( QGraphicsScene * scene, QGraphicsItem * parent = nullptr );
	~NodeGraphicsItem();

public:
	bool isHovered() const;
	QGraphicsView * view() const;

public:
	QRectF titleBarRect() const;

public:
	QRectF boundingRect() const override;
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr ) override;

protected:
	void keyPressEvent( QKeyEvent * event ) override;
	void keyReleaseEvent( QKeyEvent * event ) override;
	void focusOutEvent( QFocusEvent * event ) override;
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event ) override;
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event ) override;
	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event ) override;
	void contextMenuEvent( QGraphicsSceneContextMenuEvent * event ) override;

	void inputMethodEvent( QInputMethodEvent * event ) override;
	QVariant inputMethodQuery( Qt::InputMethodQuery query ) const override;

protected:
	virtual QMenu * contextMenu();
	virtual QRectF contextRect() const;
	virtual QBrush titleBarBrush() const;

protected:
	virtual void drawBorder( QPainter * painter, const QRectF & rect );
	virtual void drawTitleBar( QPainter * painter, const QRectF & rect );
	virtual void drawContextBody( QPainter * painter, const QRectF & rect );

private:
	QTimer _timer;
	QPointF _movePos;
	QTextCursor _cursor;
	QTextDocument _document;
	bool _cursorShow = false;
	bool _titleEdit = false;
	bool _isMoveed = false;
	bool _isHovered = false;
};

END_XS_NAMESPACE

#endif//NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC
