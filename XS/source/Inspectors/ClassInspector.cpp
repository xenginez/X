#include "ClassInspector.h"

#include <QHeaderView>

REG_WIDGET( XS::ClassInspector );

XS::ClassInspector::ClassInspector( QWidget * parent /* = nullptr */ )
	: Inspector( parent )
{
	_QTreeWidget = new QTreeWidget( this );
	_QTreeWidget->setHeaderLabels( { tr( "name" ), tr( "value" ) } );
	_QTreeWidget->setEditTriggers( QAbstractItemView::EditTrigger::NoEditTriggers );
	_QTreeWidget->setSelectionMode( QAbstractItemView::SelectionMode::NoSelection );
}

XS::ClassInspector::~ClassInspector()
{

}

void XS::ClassInspector::Refresh()
{

}
