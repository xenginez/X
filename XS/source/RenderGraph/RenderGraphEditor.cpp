#include "RenderGraphEditor.h"

#include "ui_RenderGraphEditor.h"

REG_WIDGET( XS::RenderGraphEditor );

XS::RenderGraphEditor::RenderGraphEditor( QWidget * parent /*= nullptr */ )
	: XS::DockWidget( parent ), ui( new Ui::RenderGraphEditor )
{
	setupUi( ui );

	setTitleBar( ui->title_bar );
}

XS::RenderGraphEditor::~RenderGraphEditor()
{
	delete ui;
}
