#include "NodeGraphicsEditor.h"
#include "ui_NodeGraphicsEditor.h"

#include "NodeGraphicsWidget.h"

REG_WIDGET( XS::NodeGraphicsEditor );

XS::NodeGraphicsEditor::NodeGraphicsEditor( QWidget * parent )
	: XS::ToolEditorWindow( parent ), ui( new Ui::NodeGraphicsEditor() )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );

	ui->graphics_tab->addTab( new NodeGraphicsWidget( this ), "test" );
}

XS::NodeGraphicsEditor::~NodeGraphicsEditor()
{
	delete ui;
}

QIcon XS::NodeGraphicsEditor::icon()
{
	return QIcon( "SkinIcons:/images/icons/icon_node.png" );
}

QString XS::NodeGraphicsEditor::name()
{
	return tr( "NodeGraphics" );
}
