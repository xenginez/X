#include "NodeGraphicsScene.h"

#include "NodeGraphicsItem.h"

XS::NodeGraphicsScene::NodeGraphicsScene( QObject * parent )
	: QGraphicsScene( parent )
{
	setSceneRect( QRectF( 0, 0, INT_MAX, INT_MAX ) );

	addRect( 0, 0, 100, 100, QPen( Qt::white ), QBrush( Qt::white ) )->setPos( INT_MAX / 2, INT_MAX / 2 );
}

XS::NodeGraphicsScene::~NodeGraphicsScene()
{
}
