#include "RenderGraphEditor.h"

#include "ui_RenderGraphEditor.h"

REG_WIDGET( XS::RenderGraphEditor );

XS::RenderGraphEditor::RenderGraphEditor( QWidget * parent /*= nullptr */ )
	: XS::DockWidget( parent ), ui( new Ui::RenderGraphEditor )
{
	setupUi( ui );
	setTitleBar( ui->title_bar );

	auto n1 = new XS::NodeItem();
	auto n2 = new XS::NodeItem();
	auto n3 = new XS::NodeItem();
	auto n4 = new XS::NodeItem();

	n2->moveBy( 200, 0 );
	n3->moveBy( 0, 200 );
	n4->moveBy( 200, 200 );

	ui->node->addItem( n1 ); ui->node->addItem( n2 ); ui->node->addItem( n3 ); ui->node->addItem( n4 );
}

XS::RenderGraphEditor::~RenderGraphEditor()
{
	delete ui;
}
