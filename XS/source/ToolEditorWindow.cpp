#include "ToolEditorWindow.h"

XS::ToolEditorWindow::ToolEditorWindow( QWidget * parent /*= nullptr */ )
	: XS::DockWidget( parent )
{

}

XS::ToolEditorWindow::~ToolEditorWindow()
{

}

QIcon XS::ToolEditorWindow::icon()
{
	return {};
}

QString XS::ToolEditorWindow::name()
{
	return {};
}
