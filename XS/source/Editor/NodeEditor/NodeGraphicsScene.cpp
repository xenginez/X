#include "NodeGraphicsScene.h"

#include <QVariant>

NodeGraphicsScene::NodeGraphicsScene( QObject * parent )
	: QGraphicsScene( parent )
{
	setSceneRect( QRectF( -SHRT_MAX, -SHRT_MAX, SHRT_MAX, SHRT_MAX ) );
}

NodeGraphicsScene::~NodeGraphicsScene()
{
}
