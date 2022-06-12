#include "Widget.h"

#include "Canvas.h"

BEG_META( XE::Widget )
type->Property( "Name", &XE::Widget::_Name );
type->Property( "Enable", &XE::Widget::_Enable );
type->Property( "Children", &XE::Widget::_Children )->Attribute( XE::NonEditorAttribute() );
END_META()

XE::Widget::Widget()
{

}

XE::Widget::~Widget()
{

}

void XE::Widget::Startup()
{
	OnStartup();

	for ( const auto & it : _Children )
	{
		it->_Canvas = _Canvas;

		it->Startup();
	}
}

void XE::Widget::Update()
{
	if ( GetEnable() )
	{
		OnUpdate();

		for ( const auto & it : _Children )
		{
			it->Update();
		}
	}
}

void XE::Widget::Render()
{
	OnRender();
}

void XE::Widget::Clearup()
{
	for ( const auto & it : _Children )
	{
		it->Clearup();
	}

	OnClearup();

	_Canvas.reset();
	_Parent.reset();
	_Children.clear();
}

void XE::Widget::OnStartup()
{

}

void XE::Widget::OnUpdate()
{

}

void XE::Widget::OnRender()
{

}

void XE::Widget::OnClearup()
{

}

bool XE::Widget::GetEnable() const
{
	return _Enable;
}

void XE::Widget::SetEnable( bool val )
{
	_Enable = val;

	Rebuild();
}

const XE::Recti & XE::Widget::GetRect() const
{
	return _Rect;
}

void XE::Widget::SetRect( const XE::Recti & val )
{
	_Rect = val;

	Rebuild();
}

const XE::String & XE::Widget::GetName() const
{
	return _Name;
}

void XE::Widget::SetName( const XE::String & val )
{
	_Name = val;
}

XE::CanvasPtr XE::Widget::GetCanvas() const
{
	return _Canvas.lock();
}

XE::WidgetPtr XE::Widget::GetParent() const
{
	return _Parent.lock();
}

XE::WidgetPtr XE::Widget::FindChild( const XE::String & val ) const
{
	auto end = val.find( '/' );
	auto name = val.substr( 0, end );

	auto it = std::find_if( _Children.begin(), _Children.end(), [&]( const auto & it ) { return it->GetName() == name; } );
	if ( it != _Children.end() )
	{
		return ( end == XE::String::npos ) ? *it : ( *it )->FindChild( val.substr( end + 1 ) );
	}

	return nullptr;
}

const XE::Array< XE::WidgetPtr > & XE::Widget::GetChildren( XE::uint64 val ) const
{
	return _Children;
}

void XE::Widget::Rebuild()
{
	GetCanvas()->Rebuild();
}
