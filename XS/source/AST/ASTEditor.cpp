#include "ASTEditor.h"

#include "ui_ASTEditor.h"

REG_WIDGET( XS::ASTEditor );

XS::ASTEditor::ASTEditor( QWidget * parent /*= nullptr */ )
	: XS::EditorWindow( parent ), ui( new Ui::ASTEditor )
{
	setupUi( ui );
	setTitleBar( ui->title_bar );
}

XS::ASTEditor::~ASTEditor()
{
	delete ui;
}
