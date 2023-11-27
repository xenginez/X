#include "NodeGraphicsWidget.h"
#include "ui_NodeGraphicsWidget.h"

#include "NodeGraphicsView.h"
#include "NodeGraphicsScene.h"

XS::NodeGraphicsWidget::NodeGraphicsWidget( QWidget * parent )
	: QWidget( parent ), ui( new Ui::NodeGraphicsWidget )
{
	ui->setupUi( this );

	ui->splitter->setSizes( { 30000, 70000 } );

	auto scene = new NodeGraphicsScene( this );
	ui->view->setScene( scene );
	ui->view->centerOn( scene->sceneRect().center().x(), scene->sceneRect().center().y() );
}

XS::NodeGraphicsWidget::~NodeGraphicsWidget()
{
}
