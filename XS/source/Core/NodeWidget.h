/*!
 * \file	NodeView.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5
#define NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5

#include <QMap>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "Global.h"

BEG_XS_NAMESPACE

class NodeWidget;

class XS_API NodeItem : public QGraphicsProxyWidget
{
	Q_OBJECT

	friend class XS::NodeWidget;

public:
	enum class PortType
	{
		IN,
		OUT,
	};

public:
	NodeItem( XS::NodeItem * parent = nullptr );

	~NodeItem() override;

public:
	const QString & name() const;

	void setName( const QString & val );

	XS::NodeWidget * nodeWidget() const;

public:
	virtual void setupUi( QWidget * widget );

	virtual QPointF portPos( PortType type, int index );

	virtual QString portName( PortType type, int index );

protected:
	void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget /* = 0 */ ) override;

	bool eventFilter( QObject * object, QEvent * event ) override;

private:
	QString _Name;
	XS::NodeWidget * _NodeWidget = nullptr;
};

class XS_API NodeWidget : public QGraphicsView
{
	Q_OBJECT

	friend class NodeItem;

public:
	enum class ConnectType
	{
		LINE,
		BROKEN,
		BEZIER,
	};

public:
	NodeWidget( QWidget * parent = nullptr );

	~NodeWidget() override;

signals:
	void selectionChanged();

public:
	QList< XS::NodeItem * > selectedItems() const;

	XS::NodeItem * addItem( XS::NodeItem * item );

	QGraphicsItem * addConnect( ConnectType type, XS::NodeItem * in_item, int in_port, XS::NodeItem * out_item, int out_port );

public:
	void drawConnect( const QPointF & start, const QPointF & end );

protected:
	void contextMenuEvent( QContextMenuEvent * event ) override;

	void wheelEvent( QWheelEvent * event ) override;

	void mousePressEvent( QMouseEvent * event ) override;

	void mouseMoveEvent( QMouseEvent * event ) override;

	void mouseReleaseEvent( QMouseEvent * event ) override;

	void mouseDoubleClickEvent( QMouseEvent * event ) override;

	void drawBackground( QPainter * painter, const QRectF & rect ) override;

	void showEvent( QShowEvent * event ) override;

private:
	int _SceneFlag = 0;
	QPoint _LastPos = {};
	QGraphicsRectItem * _Group = nullptr;
	QGraphicsPathItem * _Connect = nullptr;
};

END_XS_NAMESPACE

#endif//NODEVIEW_H__0A61A4A1_A166_4BD3_99B0_378458A20FC5
