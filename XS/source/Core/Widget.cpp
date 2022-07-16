#include "Widget.h"

#include <QUndoCommand>

#include "DockWidget.h"

XS::Widget::Widget( QWidget * parent /*= nullptr */ )
	:QWidget( parent )
{

}

XS::Widget::~Widget()
{

}

QShortcut * XS::Widget::AddShortcuts( const QString & name, const QKeySequence & key )
{
	return GetParent< XS::DockWidget >()->AddShortcuts( name, key );
}

XS::CoreFramework * XS::Widget::GetFramework()
{
	return XS::CoreFramework::GetCurrentFramework();
}
