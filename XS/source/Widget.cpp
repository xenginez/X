#include "Widget.h"

#include <QUndoCommand>

#include "DockWidget.h"

XS::Widget::Widget( QWidget * parent /*= nullptr */ )
	:QWidget( parent )
{
	QMetaObject::invokeMethod( this, [this]()
	{
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

void XS::Widget::OnCommandRedo()
{
	auto childs = children();
	for ( auto it : childs )
	{
		if ( it->metaObject()->inherits( &XS::Widget::staticMetaObject ) )
		{
			qobject_cast<XS::Widget *>( it )->OnCommandRedo();
		}
	}

	OnRedo();

	_CommandCount++;
}

void XS::Widget::OnCommandUndo()
{
	auto childs = children();
	for ( auto it : childs )
	{
		if ( it->metaObject()->inherits( &XS::Widget::staticMetaObject ) )
		{
			qobject_cast<XS::Widget *>( it )->OnCommandUndo();
		}
	}

	OnUndo();

	_CommandCount--;
}

void XS::Widget::OnCommandSave()
{
	auto childs = children();
	for ( auto it : childs )
	{
		if ( it->metaObject()->inherits( &XS::Widget::staticMetaObject ) )
		{
			qobject_cast<XS::Widget *>( it )->OnCommandSave();
		}
	}

	OnSave();

	_CommandCount = 0;
}

void XS::Widget::OnRedo()
{

}

void XS::Widget::OnUndo()
{

}

void XS::Widget::OnSave()
{

}

int XS::Widget::GetCommandCount() const
{
	return _CommandCount;
}
