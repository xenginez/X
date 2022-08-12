#include "NodeWidget.h"

#include <QPair>
#include <QStyle>
#include <QWheelEvent>
#include <QApplication>
#include <QStyleOption>
#include <QColorDialog>
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

	struct Ports
	{
		QVector< NodeConnect * > InPorts;
		QVector< NodeConnect * > OutPorts;
	};

	class NodeGroup : public QGraphicsItemGroup
	{
	public:
		NodeGroup( QGraphicsItem * parent = nullptr )
			: QGraphicsItemGroup( parent )
		{
			setFlag( QGraphicsItem::ItemIsMovable );
			setFlag( QGraphicsItem::ItemIsFocusable );
			setFlag( QGraphicsItem::ItemIsSelectable );
			setFlag( QGraphicsItem::ItemSendsScenePositionChanges );

			setZValue( GROUP_Z );
		}

	public:
		QRectF titleRect() const
		{
			auto rect = boundingRect();

			return QRectF( rect.left(), rect.top(), rect.width(), 20 );
		}

		QRectF groupRect() const
		{
			auto rect = boundingRect();

			return QRectF( rect.left(), rect.top() + 20, rect.width(), rect.height() - 20 );
		}

		QRectF textRect() const
		{
			auto rect = titleRect();

			rect.setLeft( rect.left() + 5 );
			rect.setRight( rect.right() - 25 );
			rect.setTop( rect.top() + 2 );
			rect.setBottom( rect.bottom() - 2 );

			return rect;
		}

		QRectF colorButtonRect() const
		{
			auto rect = titleRect();

			rect.setLeft( rect.right() - 20 );
			rect.setTop( rect.top() + 2 );
			rect.setRight( rect.right() - 5 );
			rect.setBottom( rect.bottom() - 2 );

			return rect;
		}

	protected:
		QRectF boundingRect() const override
		{
			return childrenBoundingRect().marginsAdded( QMargins( 5, 25, 5, 5 ) );
		}

		void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = nullptr */ ) override
		{
			painter->setRenderHint( QPainter::Antialiasing );

			auto title_rect = titleRect();
			painter->save();
			{
				int corner_size = 10;
				int arc_r = corner_size / 2;

				QPainterPath path;

				path.moveTo( title_rect.left() + arc_r, title_rect.top() );
				path.arcTo( title_rect.left(), title_rect.top(), corner_size, corner_size, 90.0f, 90.0f );
				path.lineTo( title_rect.left(), title_rect.bottom() );
				path.lineTo( title_rect.right(), title_rect.bottom() );
				path.lineTo( title_rect.right(), title_rect.top() + arc_r );
				path.arcTo( title_rect.right() - corner_size, title_rect.top(), corner_size, corner_size, 0.0f, 90.0f );
				path.lineTo( title_rect.left() + arc_r, title_rect.top() );

				QLinearGradient linear( QPointF( 0, 0 ), QPointF( 200, 200 ) );
				linear.setColorAt( 0, _Color );
				linear.setColorAt( 1, Qt::black );

				painter->fillPath( path, QBrush( linear ) );

				painter->drawPath( path );
			}
			painter->restore();

			auto text_rect = textRect();
			painter->save();
			{
				QFont font = qApp->font();
				{
					font.setBold( true );
				}
				painter->setFont( font );

				QColor color = _Color;
				{
					color.setRed( 255 - color.red() );
					color.setGreen( 255 - color.green() );
					color.setBlue( 255 - color.blue() );
				}
				painter->setPen( QPen( color ) );

				QFontMetrics fontMetrics( font );
				int fontSize = fontMetrics.width( _Name );
				if ( fontSize > title_rect.width() - 25 )
				{
					painter->drawText( text_rect, fontMetrics.elidedText( _Name, Qt::ElideRight, title_rect.width() - 25 ) );
				}
				else
				{
					painter->drawText( text_rect, _Name );
				}
			}
			painter->restore();

			QRect pix_rect = {};
			painter->save();
			{
				pix_rect.setLeft( title_rect.right() - 20 );
				pix_rect.setTop( title_rect.top() + 2 );
				pix_rect.setRight( title_rect.right() - 5 );
				pix_rect.setBottom( title_rect.bottom() - 2 );

				painter->drawPixmap( pix_rect, QPixmap( "SkinIcons:/images/common/icon_colors.png" ) );
			}
			painter->restore();

			auto group_rect = groupRect();
			painter->save();
			{
				QColor color = _Color;
				color.setAlphaF( 0.7f );
				painter->setBrush( QBrush( color ) );
				painter->drawRect( groupRect() );
			}
			painter->restore();
		}

		void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override
		{
			if ( mapRectToScene( colorButtonRect() ).contains( mapToScene( event->pos() ) ) )
			{
				QColorDialog color;

				color.setCurrentColor( _Color );

				if ( color.exec() == QDialog::Accepted )
				{
					_Color = color.currentColor();
				}
			}
			else
			{
				QGraphicsItemGroup::mouseReleaseEvent( event );
			}
		}

		void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event ) override
		{
			QGraphicsItemGroup::mouseDoubleClickEvent( event );

			if ( textRect().contains( mapToScene( event->pos() ) ) )
			{
				// TODO: name edit
			}
		}

	public:
		QColor _Color = Qt::green;
		QString _Name = "Node Group";
	};

	class NodeConnect : public QGraphicsItem
	{
		static constexpr int LINE_WIDTH = 10;
		static constexpr int SELECT_LINE_WIDTH = 20;

	public:
		NodeConnect( XS::NodeWidget::ConnectType type, XS::NodeItem * in_item, int in_port, XS::NodeItem * out_item, int out_port )
			: InItem( in_item ), InPort( in_port ), OutItem( out_item ), OutPort( out_port )
		{
			setFlag( QGraphicsItem::ItemIsSelectable );

			setZValue( CONNECT_Z );

			Ports * in_ports = (Ports *)InItem->property( PORT_CONNECT_NAME ).value< std::uintptr_t >();
			if ( in_ports->OutPorts.size() <= InPort )
			{
				in_ports->OutPorts.resize( InPort + 1 );
			}
			in_ports->OutPorts[InPort] = this;

			Ports * out_ports = (Ports *)OutItem->property( PORT_CONNECT_NAME ).value< std::uintptr_t >();
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
				Ports * in_ports = (Ports *)InItem->property( PORT_CONNECT_NAME ).value<void *>();
				in_ports->OutPorts[InPort] = nullptr;
			}
			if ( OutItem != nullptr )
			{
				Ports * out_ports = (Ports *)OutItem->property( PORT_CONNECT_NAME ).value<void *>();
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

			switch ( Type )
			{
			case XS::NodeWidget::ConnectType::LINE:
				p.moveTo( p1 ); p.lineTo( p4 );
				break;
			case XS::NodeWidget::ConnectType::BROKEN:
				p2.setX( p1.x() ); p2.setY( ( p1.y() + p4.y() ) / 2 );
				p3.setX( p4.x() ); p3.setY( ( p1.y() + p4.y() ) / 2 );
				p.moveTo( p1 ); p.lineTo( p2 ); p.lineTo( p3 ); p.lineTo( p4 );
				break;
			case XS::NodeWidget::ConnectType::BEZIER:
				p2.setX( ( p1.x() + p4.x() ) / 2 ); p2.setY( p1.y() );
				p3.setX( ( p1.x() + p4.x() ) / 2 ); p3.setY( p4.y() );
				p.moveTo( p1 ); p.cubicTo( p2, p3, p4 );
				break;
			default:
				break;
			}

			return p;
		}

		QRectF boundingRect() const override
		{
			return shape().boundingRect();
		}

		bool contains( const QPointF & point ) const override
		{
			QPainterPathStroker stroker;
			stroker.setWidth( SELECT_LINE_WIDTH );
			return stroker.createStroke( shape() ).contains( point );
		}

		void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget = 0 ) override
		{
			painter->save();
			{
				if ( isSelected() )
				{
					painter->setPen( QPen( qApp->palette().color( QPalette::Highlight ), SELECT_LINE_WIDTH ) );

					painter->drawPath( shape() );
				}

				painter->setPen( QPen( qApp->palette().color( QPalette::Foreground ), LINE_WIDTH ) );

				painter->drawPath( shape() );
			}
			painter->restore();

			QMetaObject::invokeMethod( nullptr, [this]()
			{
				setToolTip( QString( "%1.%2 > %3.%4" )
							.arg( InItem->name() )
							.arg( InItem->portName( XS::NodeItem::PortType::OUT, InPort ) )
							.arg( OutItem->name() )
							.arg( OutItem->portName( XS::NodeItem::PortType::IN, OutPort ) ) );
			}, Qt::QueuedConnection);
		}

		QVariant itemChange( GraphicsItemChange change, const QVariant & value ) override
		{
			switch ( change )
			{
			case QGraphicsItem::ItemSelectedHasChanged:
				QMetaObject::invokeMethod( nullptr, [this, value]() { setZValue( value.toBool() ? DRAW_CONNECT_Z : CONNECT_Z ); }, Qt::QueuedConnection );
				break;
			default:
				break;
			}

			return  QGraphicsItem::itemChange( change, value );
		}

	public:
		int InPort = 0;
		int OutPort = 0;
		XS::NodeItem * InItem = nullptr;
		XS::NodeItem * OutItem = nullptr;
		XS::NodeWidget::ConnectType Type = XS::NodeWidget::ConnectType::BEZIER;
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

	auto ports = new Ports;

	setProperty( PORT_CONNECT_NAME, (std::uintptr_t)ports );
}

XS::NodeItem::~NodeItem()
{
	auto scene = _NodeWidget->scene();
	Ports * ports = (Ports *)property( PORT_CONNECT_NAME ).value<std::uintptr_t>();

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

QString XS::NodeItem::portName( PortType type, int index )
{
	return QString( "port %1" ).arg( index );
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

QList< XS::NodeItem * > XS::NodeWidget::selectedItems() const
{
	QList< XS::NodeItem * > result;

	auto items = scene()->selectedItems();

	for ( auto it : items )
	{
		auto node = dynamic_cast<XS::NodeItem *>( it );
		if ( node != nullptr )
		{
			result.push_back( node );
		}
	}

	return result;
}

XS::NodeItem * XS::NodeWidget::addItem( XS::NodeItem * item )
{
	item->_NodeWidget = this;

	scene()->addItem( item );

	return item;
}

QGraphicsItem * XS::NodeWidget::addConnect( ConnectType type, XS::NodeItem * in_item, int in_port, XS::NodeItem * out_item, int out_port )
{
	if ( _Connect != nullptr )
	{
		scene()->removeItem( _Connect );

		_Connect = nullptr;
	}

	auto con = new NodeConnect( type, in_item, in_port, out_item, out_port );

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
			auto color = palette().color( QPalette::Foreground );

			QPen pen( color, 1.0 );
			color.setAlphaF( 0.3f );
			QBrush brush( color );

			_Group = scene()->addRect( pos.x(), pos.y(), 0, 0, pen, brush );
			_Group->setZValue( DRAW_CONNECT_Z );
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
			auto arc = mapToScene( _LastPos );
			auto pos = mapToScene( event->pos() );

			qreal left = std::min( pos.x(), arc.x() );
			qreal right = std::max( pos.x(), arc.x() );
			qreal top = std::min( pos.y(), arc.y() );
			qreal bottom = std::max( pos.y(), arc.y() );

			_Group->setRect( left, top, std::abs( right - left ), std::abs( bottom - top ) );
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
		auto rect = _Group->rect();

		auto items = scene()->items( rect.x(), rect.y(), rect.width(), rect.height(), Qt::ContainsItemBoundingRect, Qt::AscendingOrder );
		if ( !items.empty() )
		{
			NodeGroup * group = new NodeGroup();

			for ( auto i : items )
			{
				auto node = dynamic_cast<XS::NodeItem *>( i );
				if ( node != nullptr )
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
