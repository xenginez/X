#include "CodeEditorWindow.h"
#include "ui_CodeEditorWindow.h"

REG_WIDGET( XS::CodeEditorWindow );

XS::CodeEditorWindow::CodeEditorWindow(QWidget *parent)
	: XS::AssetEditorWindow(parent), ui(new Ui::CodeEditorWindow())
{
	ui->setupUi(this);
}

XS::CodeEditorWindow::~CodeEditorWindow()
{
	delete ui;
}

void XS::CodeEditorWindow::assetOpen( const QUuid & uuid )
{

}
