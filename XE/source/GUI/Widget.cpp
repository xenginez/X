#include "Widget.h"

#include "Canvas.h"
#include "Layout.h"

BEG_META( XE::Widget )
type->Property( "Name", &XE::Widget::_Name );
type->Property( "Enable", &XE::Widget::_Enable );
type->Property( "Rect", &XE::Widget::_Rect );
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

const XE::Recti & XE::Widget::GetPadding() const
{
	return _Padding;
}

void XE::Widget::SetPadding( const XE::Recti & val )
{
	_Padding = val;
}

const XE::Recti & XE::Widget::GetMargins() const
{
	return _Margins;
}

void XE::Widget::SetMargins( const XE::Recti & val )
{
	_Margins = val;
}

XE::int32 XE::Widget::GetWidth() const
{
	return _Rect.width;
}

void XE::Widget::SetWidth( XE::int32 val )
{
	_Rect.width = val;
}

XE::int32 XE::Widget::GetHeight() const
{
	return _Rect.height;
}

void XE::Widget::SetHeight( XE::int32 val )
{
	_Rect.height = val;
}

XE::int32 XE::Widget::GetLeft() const
{
	return _Rect.GetLeft();
}

void XE::Widget::SetLeft( XE::int32 val )
{
	_Rect.SetLeft( val );
}

XE::int32 XE::Widget::GetRight() const
{
	return _Rect.GetRight();
}

void XE::Widget::SetRight( XE::int32 val )
{
	_Rect.SetRight( val );
}

XE::int32 XE::Widget::GetTop() const
{
	return _Rect.GetTop();
}

void XE::Widget::SetTop( XE::int32 val )
{
	_Rect.SetTop( val );
}

XE::int32 XE::Widget::GetBottom() const
{
	return _Rect.GetBottom();
}

void XE::Widget::SetBottom( XE::int32 val )
{
	_Rect.SetBottom( val );
}

XE::int32 XE::Widget::GetPaddingLeft() const
{
	return _Padding.GetLeft();
}

void XE::Widget::SetPaddingLeft( XE::int32 val )
{
	_Padding.SetLeft( val );
}

XE::int32 XE::Widget::GetPaddingRight() const
{
	return _Padding.GetRight();
}

void XE::Widget::SetPaddingRight( XE::int32 val )
{
	_Padding.SetRight( val );
}

XE::int32 XE::Widget::GetPaddingTop() const
{
	return _Padding.GetTop();
}

void XE::Widget::SetPaddingTop( XE::int32 val )
{
	_Padding.SetTop( val );
}

XE::int32 XE::Widget::GetPaddingBottom() const
{
	return _Padding.GetBottom();
}

void XE::Widget::SetPaddingBottom( XE::int32 val )
{
	_Padding.SetBottom( val );
}

XE::int32 XE::Widget::GetMarginLeft() const
{
	return _Margins.GetLeft();
}

void XE::Widget::SetMarginLeft( XE::int32 val )
{
	_Margins.SetLeft( val );
}

XE::int32 XE::Widget::GetMarginRight() const
{
	return _Margins.GetRight();
}

void XE::Widget::SetMarginRight( XE::int32 val )
{
	_Margins.SetRight( val );
}

XE::int32 XE::Widget::GetMarginTop() const
{
	return _Margins.GetTop();
}

void XE::Widget::SetMarginTop( XE::int32 val )
{
	_Margins.SetTop( val );
}

XE::int32 XE::Widget::GetMarginBottom() const
{
	return _Margins.GetBottom();
}

void XE::Widget::SetMarginBottom( XE::int32 val )
{
	_Margins.SetBottom( val );
}

XE::FlexAlign XE::Widget::GetJustifyContent() const
{
	return _JustifyContent;
}

void XE::Widget::SetJustifyContent( XE::FlexAlign val )
{
	_JustifyContent = val;
}

XE::FlexAlign XE::Widget::GetAlignContent() const
{
	return _AlignContent;
}

void XE::Widget::SetAlignContent( XE::FlexAlign val )
{
	_AlignContent = val;
}

XE::FlexAlign XE::Widget::GetAlignItems() const
{
	return _AlignItems;
}

void XE::Widget::SetAlignItems( XE::FlexAlign val )
{
	_AlignItems = val;
}

XE::FlexAlign XE::Widget::GetAlignSelf() const
{
	return _AlignSelf;
}

void XE::Widget::SetAlignSelf( XE::FlexAlign val )
{
	_AlignSelf = val;
}

XE::FlexPosition XE::Widget::GetFlexPosition() const
{
	return _Position;
}

void XE::Widget::SetFlexPosition( XE::FlexPosition val )
{
	_Position = val;
}

XE::FlexDirection XE::Widget::GetFlexDirection() const
{
	return _Direction;
}

void XE::Widget::SetFlexDirection( XE::FlexDirection val )
{
	_Direction = val;
}

XE::FlexWrap XE::Widget::GetFlexWrap() const
{
	return _Wrap;
}

void XE::Widget::SetFlexWrap( XE::FlexWrap val )
{
	_Wrap = val;
}

XE::float32 XE::Widget::GetGrow() const
{
	return _Grow;
}

void XE::Widget::SetGrow( XE::float32 val )
{
	_Grow = val;
}

XE::float32 XE::Widget::GetShrink() const
{
	return _Shrink;
}

void XE::Widget::SetShrink( XE::float32 val )
{
	_Shrink = val;
}

XE::int32 XE::Widget::GetOrder() const
{
	return _Order;
}

void XE::Widget::SetOrder( XE::int32 val )
{
	_Order = val;
}

XE::float32 XE::Widget::GetBasis() const
{
	return _Basis;
}

void XE::Widget::SetBasis( XE::float32 val )
{
	_Basis = val;
}

XE::Recti XE::Widget::GetFrameRect() const
{
	if ( auto canvas = GetCanvas() )
	{
		if ( auto layout = canvas->GetLayout() )
		{
			return layout->GetFrameRect( this );
		}
	}

	return _Rect;
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
