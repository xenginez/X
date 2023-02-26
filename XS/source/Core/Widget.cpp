#include "Widget.h"

#include <QUndoCommand>

#include "DockWidget.h"

XS::Widget::Widget( QWidget * parent /*= nullptr */ )
	:QWidget( parent )
{
	_Parent = GetParent< XS::Widget >();
	if ( _Parent )
	{
		_Parent->_Children.push_back( this );
	}
	else
	{
		GetParent< XS::DockWidget >()->_Children.push_back( this );
	}
}

XS::Widget::~Widget()
{
	if ( _Parent )
	{
		_Parent->_Children.removeOne( this );
	}
	else if( GetParent< XS::DockWidget >() )
	{
		GetParent< XS::DockWidget >()->_Children.removeOne( this );
	}
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
	for ( auto it : _Children )
	{
		it->OnCommandRedo();
	}

	OnRedo();
}

void XS::Widget::OnCommandUndo()
{
	for ( auto it : _Children )
	{
		it->OnCommandUndo();
	}

	OnUndo();
}

void XS::Widget::OnCommandSave()
{
	for ( auto it : _Children )
	{
		it->OnCommandSave();
	}

	OnSave();
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
