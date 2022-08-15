#include "Widget.h"

#include <QUndoCommand>

#include "DockWidget.h"

XS::Widget::Widget( QWidget * parent /*= nullptr */ )
	:QWidget( parent )
{
	QMetaObject::invokeMethod( this, [this]()
	{
		connect( AddShortcuts( "Save", QKeySequence( "CTRL+S" ) ), &QShortcut::activated, this, &XS::Widget::OnSaveCommand );
	}, Qt::QueuedConnection );
}

XS::Widget::~Widget()
{

}

void XS::Widget::PushUndoCommand( QUndoCommand * command )
{
	QWidget * parent = parentWidget();
	while ( parent != nullptr )
	{
		if ( parent->metaObject()->inherits( &XS::DockWidget::staticMetaObject ) )
		{
			dynamic_cast<XS::DockWidget *>( parent )->PushUndoCommand( command );
			OnPushCommand();
			break;
		}
		else
		{
			parent = parent->parentWidget();
		}
	}
}

QShortcut * XS::Widget::AddShortcuts( const QString & name, const QKeySequence & key )
{
	return GetParent< XS::DockWidget >()->AddShortcuts( name, key );
}

XS::CoreFramework * XS::Widget::GetFramework()
{
	return XS::CoreFramework::GetCurrentFramework();
}

void XS::Widget::SaveLayout( QSettings & settings )
{
	settings.setValue( "geometry", saveGeometry() );
}

void XS::Widget::LoadLayout( QSettings & settings )
{
	restoreGeometry( settings.value( "geometry" ).toByteArray() );
}

void XS::Widget::OnPushCommand()
{

}

void XS::Widget::OnSaveCommand()
{

}
