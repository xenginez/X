#include "Widget.h"

#include "Canvas.h"
#include "Layout.h"

BEG_META( XE::Widget )
type->Property( "Name", &XE::Widget::_Name );
type->Property( "Enable", &XE::Widget::_Enable );
type->Property( "Rect", &XE::Widget::_Rect );
type->Property( "Frame", &XE::Widget::_Frame )->Attribute( XE::NonEditorAttribute() );
type->Property( "Padding", &XE::Widget::_Padding );
type->Property( "Margins", &XE::Widget::_Margins );
type->Property( "Layout", &XE::Widget::_Layout );
type->Property( "LayoutItem", &XE::Widget::_LayoutItem );
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

	if ( _Layout )
	{
		_Layout->Rebuild( _Rect.width, _Rect.height, _Children );
	}

	for ( const auto & it : _Children )
	{
		it->_Canvas = _Canvas;
		it->_Parent = XE_THIS( XE::Widget );

		it->Startup();
	}
}

void XE::Widget::Update()
{
	if ( GetEnable() )
	{
		OnUpdate();

		if ( _Layout )
		{
			_Layout->Rebuild( _Rect.width, _Rect.height, _Children );
		}

		for ( const auto & it : _Children )
		{
			it->Update();
		}
	}
}

void XE::Widget::Render()
{
	if ( GetEnable() )
	{
		OnRender();

		for ( const auto & it : _Children )
		{
			it->Render();
		}
	}
}

void XE::Widget::Clearup()
{
	for ( const auto & it : _Children )
	{
		it->Clearup();
	}

	OnClearup();

	_Layout = nullptr;
	_LayoutItem = nullptr;

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

	GetCanvas()->Rebuild();
}

const XE::String & XE::Widget::GetName() const
{
	return _Name;
}

void XE::Widget::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::LayoutPtr & XE::Widget::GetLayout() const
{
	return _Layout;
}

void XE::Widget::SetLayout( const XE::LayoutPtr & val )
{
	_Layout = val;
}

const XE::LayoutItemPtr & XE::Widget::GetLayoutItem() const
{
	return _LayoutItem;
}

void XE::Widget::SetLayoutItem( const XE::LayoutItemPtr & val )
{
	_LayoutItem = val;
}

const XE::Recti & XE::Widget::GetRect() const
{
	return _Rect;
}

void XE::Widget::SetRect( const XE::Recti & val )
{
	_Rect = val;

	GetCanvas()->Rebuild();
}

const XE::Recti & XE::Widget::GetFrame() const
{
	return _Frame;
}

void XE::Widget::SetFrame( const XE::Recti & val )
{
	_Frame = val;
}

const XE::Recti & XE::Widget::GetPadding() const
{
	return _Padding;
}

void XE::Widget::SetPadding( const XE::Recti & val )
{
	_Padding = val;

	GetCanvas()->Rebuild();
}

const XE::Recti & XE::Widget::GetMargins() const
{
	return _Margins;
}

void XE::Widget::SetMargins( const XE::Recti & val )
{
	_Margins = val;

	GetCanvas()->Rebuild();
}

XE::CanvasPtr XE::Widget::GetCanvas() const
{
	return _Canvas.lock();
}

XE::WidgetPtr XE::Widget::GetParent() const
{
	return _Parent.lock();
}

const XE::Array< XE::WidgetPtr > & XE::Widget::GetChildren() const
{
	return _Children;
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
