#include "Widget.h"

#include "Layout.h"
#include "Canvas.h"

BEG_META( XE::Widget )
type->Property( "Name", &XE::Widget::_Name );
type->Property( "Enable", &XE::Widget::_Enable );
type->Property( "Rect", &XE::Widget::_Rect );
type->Property( "MinSize", &XE::Widget::_MinSize );
type->Property( "MaxSize", &XE::Widget::_MaxSize );
type->Property( "VerticalSizePolicy", &XE::Widget::_VerticalSizePolicy );
type->Property( "HorizontalSizePolicy", &XE::Widget::_HorizontalSizePolicy );
type->Property( "Layout", &XE::Widget::_Layout );
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

		if ( _Layout )
		{
			auto rects = _Layout->CalcContentRects( _Rect, _Children );
			for ( size_t i = 0; i < _Children.size(); i++ )
			{
				_Children[i]->SetRect( rects[i] );
			}
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

const XE::Recti & XE::Widget::GetRect() const
{
	return _Rect;
}

void XE::Widget::SetRect( const XE::Recti & val )
{
	_Rect = val;

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

const XE::Vec2i & XE::Widget::GetMinSize() const
{
	return _MinSize;
}

void XE::Widget::SetMinSize( const XE::Vec2i & val )
{
	_MinSize = val;
}

const XE::Vec2i & XE::Widget::GetMaxSize() const
{
	return _MaxSize;
}

void XE::Widget::SetMaxSize( const XE::Vec2i & val )
{
	_MaxSize = val;
}

const XE::LayoutPtr & XE::Widget::GetLayout() const
{
	return _Layout;
}

void XE::Widget::SetLayout( const XE::LayoutPtr & val )
{
	_Layout = val;
}

XE::SizePolicy XE::Widget::GetVerticalSizePolicy() const
{
	return _VerticalSizePolicy;
}

void XE::Widget::SetVerticalSizePolicy( XE::SizePolicy val )
{
	_VerticalSizePolicy = val;
}

XE::SizePolicy XE::Widget::GetHorizontalSizePolicy() const
{
	return _HorizontalSizePolicy;
}

void XE::Widget::SetHorizontalSizePolicy( XE::SizePolicy val )
{
	_HorizontalSizePolicy = val;
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

XE::Vec2i XE::Widget::GetSizeHint() const
{
	XE::Vec2i size;

	for ( const auto & it : _Children )
	{
		auto sz = it->GetSizeHint();
		size.x = std::max( size.x, sz.x );
		size.y = std::max( size.y, sz.y );
	}

	switch ( _HorizontalSizePolicy )
	{
	case XE::SizePolicy::FIXED:
		size.x = _Rect.width;
		break;
	case XE::SizePolicy::MINIMUM:
		size.x = _MinSize.x;
		break;
	case XE::SizePolicy::MAXIMUM:
		size.x = _MaxSize.x;
		break;
	case XE::SizePolicy::PREFERRED:
	case XE::SizePolicy::EXPANDING:
	case XE::SizePolicy::IGNORED:
		break;
	}

	switch ( _VerticalSizePolicy )
	{
	case XE::SizePolicy::FIXED:
		size.y = _Rect.height;
		break;
	case XE::SizePolicy::MINIMUM:
		size.y = _MinSize.y;
		break;
	case XE::SizePolicy::MAXIMUM:
		size.y = _MaxSize.y;
		break;
	case XE::SizePolicy::PREFERRED:
	case XE::SizePolicy::EXPANDING:
	case XE::SizePolicy::IGNORED:
		break;
	}

	return size;
}
