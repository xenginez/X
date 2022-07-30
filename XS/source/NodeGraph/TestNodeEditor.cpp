#include "TestNodeEditor.h"

#include "ui_TestNodeEditor.h"

REG_WIDGET( XS::TestNodeEditor );

XS::TestNodeEditor::TestNodeEditor( QWidget * parent /*= nullptr */ )
	:XS::DockWidget( parent ), ui( new Ui::TestNodeEditor )
{
	setupUi( ui );

	setTitleBar( nullptr );
}

XS::TestNodeEditor::~TestNodeEditor()
{

}
