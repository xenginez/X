#include "NodeWidget.h"

#include <QPair>
#include <QStyle>
#include <QWheelEvent>
#include <QApplication>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>

namespace
{
	class NodeConnect;

	static constexpr qreal NODE_Z = 2;
	static constexpr qreal GROUP_Z = 0;
	static constexpr qreal CONNECT_Z = 1;
	static constexpr qreal DRAW_CONNECT_Z = 3;
	static constexpr XE::uint32 NONE_FLAG = 0;
	static constexpr XE::uint32 MOVE_FLAG = 1;
	static constexpr XE::uint32 GROUP_FLAG = 2;
	static constexpr XE::float32 GRID_STEP = 64;
	static constexpr XE::float32 MIN_SCALE = 0.2f;
	static constexpr XE::float32 MAX_SCALE = 5.0f;
	static constexpr const char * ENTER_FLAG_NAME = "_EnterFlagName";
	static constexpr const char * PORT_CONNECT_NAME = "_PortConnectName";

	struct PortConnect
	{
		QVector< NodeConnect * > InPorts;
		QVector< NodeConnect * > OutPorts;
	};

	class NodeConnect : public QGraphicsItem
	{
		static constexpr std::array<Qt::GlobalColor, 6> COLORS =
		{
			Qt::GlobalColor::red,
			Qt::GlobalColor::green,
			Qt::GlobalColor::blue,
			Qt::GlobalColor::cyan,
			Qt::GlobalColor::magenta,
			Qt::GlobalColor::yellow,
		};

	public:
		NodeConnect( XS::NodeItem * in_item, int in_port, XS::NodeItem * out_item, int out_port )
			: InItem( in_item ), InPort( in_port ), OutItem( out_item ), OutPort( out_port )
		{
			setFlag( QGraphicsItem::ItemIsSelectable );

			setZValue( CONNECT_Z );

			setToolTip( in_item->name() + " > " + out_item->name() );

			PortConnect * in_ports = (PortConnect *)InItem->property( PORT_CONNECT_NAME ).value< std::uintptr_t >();
			if ( in_ports->OutPorts.size() <= InPort )
			{
				in_ports->OutPorts.resize( InPort + 1 );
			}
			in_ports->OutPorts[InPort] = this;

			PortConnect * out_ports = (PortConnect *)OutItem->property( PORT_CONNECT_NAME ).value< std::uintptr_t >();
			if ( out_ports->InPorts.size() <= OutPort )
			{
				out_ports->InPorts.resize( OutPort + 1 );
			}
			out_ports->InPorts[OutPort] = this;
		}

		~NodeConnect() override
		{
			if ( InItem != nullptr )
			{
				PortConnect * in_ports = (PortConnect *)InItem->property( PORT_CONNECT_NAME ).value<void *>();
				in_ports->OutPorts[InPort] = nullptr;
			}
			if ( OutItem != nullptr )
			{
				PortConnect * out_ports = (PortConnect *)OutItem->property( PORT_CONNECT_NAME ).value<void *>();
				out_ports->InPorts[OutPort] = nullptr;
			}
		}

	protected:
		QPainterPath shape() const override
		{
			QPainterPath p;

			QPointF p1, p2, p3, p4;
			p1 = InItem->portPos( XS::NodeItem::PortType::OUT, InPort );
			p4 = OutItem->portPos( XS::NodeItem::PortType::IN, OutPort );

			p2.setX( ( p1.x() + p4.x() ) / 2 ); p2.setY( p1.y() );
			p3.setX( ( p1.x() + p4.x() ) / 2 ); p3.setY( p4.y() );

			p.moveTo( p1 );
			p.cubicTo( p2, p3, p4 );

			return p;
		}

		QRectF boundingRect() const override
		{
			return shape().boundingRect();
		}

		bool contains( const QPointF & point ) const override
		{
			QPainterPathStroker stroker;
			stroker.setWidth( 15 );
			return stroker.createStroke( shape() ).contains( point );
		}

		void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget = 0 ) override
		{
			painter->save();
			{
				if ( isSelected() )
				{
					painter->setPen( QPen( qApp->palette().color( QPalette::Highlight ), 5.0f ) );

					painter->drawPath( shape() );
				}

				painter->setPen( QPen( COLORS[InPort % COLORS.size()], 2.0f ) );

				painter->drawPath( shape() );
			}
			painter->restore();
		}

	public:
		int InPort = 0;
		int OutPort = 0;
		XS::NodeItem * InItem = nullptr;
		XS::NodeItem * OutItem = nullptr;
		XS::NodeWidget * NodeWidget = nullptr;
	};
}

XS::NodeItem::NodeItem( XS::NodeItem * parent /*= nullptr */ )
	: QGraphicsProxyWidget( parent ), _Name( "NodeItem" )
{
	setFlag( QGraphicsItem::ItemIsMovable );
	setFlag( QGraphicsItem::ItemIsFocusable );
	setFlag( QGraphicsItem::ItemIsSelectable );
	setFlag( QGraphicsItem::ItemSendsScenePositionChanges );
	setFlag( QGraphicsItem::ItemDoesntPropagateOpacityToChildren );

	setZValue( NODE_Z );

	QWidget * widget = new QWidget( nullptr );
	
	widget->setMouseTracking( true );

	widget->installEventFilter( this );

	setupUi( widget );

	setWidget( widget );

	setToolTip( _Name );

	auto ports = new PortConnect;

	setProperty( PORT_CONNECT_NAME, (std::uintptr_t)ports );
}

XS::NodeItem::~NodeItem()
{
	auto scene = _NodeWidget->scene();
	PortConnect * ports = (PortConnect *)property( PORT_CONNECT_NAME ).value<std::uintptr_t>();

	for ( auto i : ports->InPorts )
	{
		if ( i != nullptr )
		{
			i->OutItem = nullptr;
			QMetaObject::invokeMethod( nullptr, [i, scene]() { scene->removeItem( i ); } );
		}
	}

	for ( auto i : ports->OutPorts )
	{
		if ( i != nullptr )
		{
			i->InItem = nullptr;
			QMetaObject::invokeMethod( nullptr, [i, scene]() { scene->removeItem( i ); } );
		}
	}

	delete ports;
}

const QString & XS::NodeItem::name() const
{
	return _Name;
}

void XS::NodeItem::setName( const QString & val )
{
	_Name = val;

	setToolTip( _Name );
}

XS::NodeWidget * XS::NodeItem::nodeWidget() const
{
	return _NodeWidget;
}

void XS::NodeItem::setupUi( QWidget * widget )
{
	widget->resize( 100, 100 );
}

QPointF XS::NodeItem::portPos( PortType type, int index )
{
	return mapToScene( this->rect().center() );
}

void XS::NodeItem::paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget /* = 0 */ )
{
	if ( isSelected() )
	{
		painter->save();
		{
			painter->setBrush( QBrush( option->palette.color( QPalette::Highlight ) ) );

			painter->drawRect( rect().marginsAdded( QMargins( 1, 1, 1, 1 ) ) );
		}
		painter->restore();
	}

	QGraphicsProxyWidget::paint( painter, option, widget );
}

bool XS::NodeItem::eventFilter( QObject * object, QEvent * event )
{
	if ( object == widget() )
	{
		switch ( event->type() )
		{
		case QEvent::Enter:
		{
			this->setProperty( ENTER_FLAG_NAME, true );

			QMetaObject::invokeMethod( this, [this]() { update(); }, Qt::QueuedConnection );
		}
		break;
		case QEvent::Leave:
		{
			this->setProperty( ENTER_FLAG_NAME, false );

			QMetaObject::invokeMethod( this, [this]() { update(); }, Qt::QueuedConnection );
		}
		break;
		case QEvent::Paint:
		{
			bool res = QGraphicsProxyWidget::eventFilter( object, event );

			if ( this->property( ENTER_FLAG_NAME ).toBool() )
			{
				QPainter painter( widget() );

				painter.save();
				{
					painter.setPen( QPen( widget()->palette().color( QPalette::Highlight ), 1.0f ) );

					auto rect = widget()->rect();

					painter.drawLine( rect.left() + 1, rect.top() + 1, rect.right(), rect.top() + 1 );
					painter.drawLine( rect.right(), rect.top() + 1, rect.right(), rect.bottom() );
					painter.drawLine( rect.right(), rect.bottom(), rect.left() + 1, rect.bottom() );
					painter.drawLine( rect.left() + 1, rect.bottom(), rect.left() + 1, rect.top() + 1 );
				}
				painter.restore();
			}

			return res;
		}
		break;
		case QEvent::MouseButtonPress:
		{
			QMouseEvent * mouse = (QMouseEvent *)event;
			if ( ( mouse->buttons() == Qt::LeftButton || widget()->hasFocus() ) && !isSelected() )
			{
				QMetaObject::invokeMethod( this, [this]() { setSelected( true ); }, Qt::QueuedConnection );
			}
		}
		break;
		default:
			break;
		}
	}

	return QGraphicsProxyWidget::eventFilter( object, event );
}



XS::NodeWidget::NodeWidget( QWidget * parent /*= nullptr */ )
	: QGraphicsView( new QGraphicsScene( parent ), parent )
{
	setMouseTracking( true );
	setCursor( Qt::PointingHandCursor );
	setDragMode( QGraphicsView::NoDrag );
	setRenderHint( QPainter::Antialiasing );
	setResizeAnchor( QGraphicsView::AnchorUnderMouse );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	setContextMenuPolicy( Qt::ContextMenuPolicy::CustomContextMenu );
	setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );

	setSceneRect( INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX );

	connect( scene(), &QGraphicsScene::selectionChanged, this, &XS::NodeWidget::selectionChanged );
}

XS::NodeWidget::~NodeWidget()
{

}

XS::NodeItem * XS::NodeWidget::addItem( XS::NodeItem * item )
{
	item->_NodeWidget = this;

	scene()->addItem( item );

	return item;
}

QGraphicsItem * XS::NodeWidget::addConnect( XS::NodeItem * in_item, int in_port, XS::NodeItem * out_item, int out_port )
{
	if ( _Connect != nullptr )
	{
		scene()->removeItem( _Connect );

		_Connect = nullptr;
	}

	auto con = new NodeConnect( in_item, in_port, out_item, out_port );

	con->NodeWidget = this;

	scene()->addItem( con );

	return con;
}

void XS::NodeWidget::drawConnect( const QPointF & start, const QPointF & end )
{
	QPainterPath p;

	QPointF p1, p2, p3, p4;
	p1 = start;
	p4 = end;
	p2.setX( ( p1.x() + p4.x() ) / 2 ); p2.setY( p1.y() );
	p3.setX( ( p1.x() + p4.x() ) / 2 ); p3.setY( p4.y() );

	p.moveTo( p1 );
	p.cubicTo( p2, p3, p4 );

	if ( _Connect == nullptr )
	{
		_Connect = scene()->addPath( p, QPen( palette().color( QPalette::Highlight ), 5.0f ) );
		_Connect->setZValue( DRAW_CONNECT_Z );
	}

	_Connect->setPath( p );
}

void XS::NodeWidget::contextMenuEvent( QContextMenuEvent * event )
{
	QGraphicsView::contextMenuEvent( event );
}

void XS::NodeWidget::wheelEvent( QWheelEvent * event )
{
	if ( _SceneFlag == NONE_FLAG && itemAt( event->pos() ) == nullptr )
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
}

void XS::NodeWidget::mousePressEvent( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent( event );

	if ( scene()->selectedItems().empty() && event->buttons() == Qt::LeftButton )
	{
		if ( ( event->modifiers() & Qt::ShiftModifier ) != 0 )
		{
			_SceneFlag = GROUP_FLAG;

			_LastPos = event->pos();

			auto pos = mapToScene( event->pos() );
			QPen pen( palette().color( QPalette::Text ), 1.0 );
			QBrush brush( palette().color( QPalette::PlaceholderText ), Qt::Dense1Pattern );
			_Group = scene()->addRect( pos.x(), pos.y(), 0, 0, pen, brush );
			_Group->setZValue( NODE_Z + 1 );
		}
		else
		{
			_SceneFlag = MOVE_FLAG;

			_LastPos = event->pos();
			setCursor( Qt::OpenHandCursor );
		}
	}
}

void XS::NodeWidget::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent( event );

	if ( event->buttons() == Qt::LeftButton )
	{
		switch ( _SceneFlag )
		{
		case MOVE_FLAG:
		{
			setTransformationAnchor( QGraphicsView::AnchorViewCenter );
			{
				auto dt = ( mapToScene( event->pos() ) - mapToScene( _LastPos ) ) * transform().m11();

				centerOn( mapToScene( QPoint( viewport()->rect().width() / 2 - dt.x(), viewport()->rect().height() / 2 - dt.y() ) ) );

				_LastPos = event->pos();
			}
			setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
		}
		break;
		case GROUP_FLAG:
		{
			auto item = static_cast<QGraphicsRectItem *>( _Group );

			auto arc = mapToScene( _LastPos );
			auto pos = mapToScene( event->pos() );

			qreal left = std::min( pos.x(), arc.x() );
			qreal right = std::max( pos.x(), arc.x() );
			qreal top = std::min( pos.y(), arc.y() );
			qreal bottom = std::max( pos.y(), arc.y() );

			item->setRect( left, top, std::abs( right - left ), std::abs( bottom - top ) );
		}
		break;
		default:
			break;
		}
	}
}

void XS::NodeWidget::mouseReleaseEvent( QMouseEvent * event )
{
	QGraphicsView::mouseReleaseEvent( event );

	switch ( _SceneFlag )
	{
	case MOVE_FLAG:
	{
		setCursor( Qt::PointingHandCursor );
	}
	break;
	case GROUP_FLAG:
	{
		auto rect = static_cast<QGraphicsRectItem *>( _Group )->rect();

		auto items = scene()->items( rect.x(), rect.y(), rect.width(), rect.height(), Qt::ContainsItemBoundingRect, Qt::AscendingOrder );
		if ( !items.empty() )
		{
			QGraphicsItemGroup * group = new QGraphicsItemGroup();
			group->setZValue( GROUP_Z );

			for ( auto i : items )
			{
				if ( std::abs( i->zValue() - NODE_Z ) < std::numeric_limits< qreal >::epsilon() )
				{
					group->addToGroup( i );
				}
			}
			scene()->addItem( group );
		}

		scene()->removeItem( _Group );
		_Group = nullptr;
	}
	break;
	default:
		break;
	}

	_SceneFlag = NONE_FLAG;
}

void XS::NodeWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
	QGraphicsView::mouseDoubleClickEvent( event );

	if ( scene()->selectedItems().empty() && event->buttons() == Qt::LeftButton )
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

		painter->setPen( QPen( palette().color( QPalette::PlaceholderText ), 1.0 ) );
		drawGrid( GRID_STEP );

		painter->setPen( QPen( palette().color( QPalette::Text ), 1.0 ) );
		drawGrid( GRID_STEP * 10 );
	}
	painter->restore();
}
