#include "NodeGraphicsScene.h"

#include "NodeGraphicsItem.h"

XS::NodeGraphicsScene::NodeGraphicsScene( QObject * parent )
	: QGraphicsScene( parent )
{
	setSceneRect( QRectF( 0, 0, INT_MAX, INT_MAX ) );

	auto item = new NodeGraphicsItem( nullptr );
	addItem( item );
	item->setPos( INT_MAX / 2, INT_MAX / 2 );

	auto text = addText( "aaaaa" );
	text->setPos( INT_MAX / 2 + 150, INT_MAX / 2 + 150 );
	text->setTextInteractionFlags( Qt::TextEditorInteraction );
}

XS::NodeGraphicsScene::~NodeGraphicsScene()
{
}
