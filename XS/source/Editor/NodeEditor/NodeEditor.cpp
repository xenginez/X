#include "NodeEditor.h"
#include "ui_NodeEditor.h"

REG_WIDGET( XS::NodeEditor );

XS::NodeEditor::NodeEditor( QWidget * parent )
	: XS::ToolEditorWindow( parent ), ui( new Ui::NodeEditor() )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );
}

XS::NodeEditor::~NodeEditor()
{
	delete ui;
}

QIcon XS::NodeEditor::icon()
{
	return QIcon( "SkinIcons:/images/icons/icon_node.png" );
}

QString XS::NodeEditor::name()
{
	return QString( "Node" );
}
