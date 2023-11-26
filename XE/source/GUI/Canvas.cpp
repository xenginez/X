#include "Canvas.h"

#include "Core/CoreFramework.h"

#include "Widget.h"
#include "Layout.h"
#include "GUIService.h"

BEG_META( XE::Canvas )
type->Property( "Enable", &XE::Canvas::_Enable );
type->Property( "Name", &XE::Canvas::_Name );
type->Property( "Rect", &XE::Canvas::_Rect );
type->Property( "Model", &XE::Canvas::_Controller );
type->Property( "Layout", &XE::Canvas::_Layout );
type->Property( "Children", &XE::Canvas::_Children )->Attribute( XE::NonInspectorAttribute() );
END_META()

XE::Canvas::Canvas()
{

}

XE::Canvas::~Canvas()
{

}

void XE::Canvas::Startup()
{
	if ( _Layout )
	{
		_Layout->Rebuild( _Rect.width, _Rect.height, _Children );
	}

	for ( const auto & it : _Children )
	{
		it->_Canvas = XE_THIS( XE::Canvas );

		it->Startup();
	}

	_Controller->Startup();
}

void XE::Canvas::Update()
{
	if ( GetEnable() && _Dirty )
	{
		if ( _Layout )
		{
			_Layout->Rebuild( _Rect.width, _Rect.height, _Children );
		}

		_Dirty = false;
	}
}

void XE::Canvas::Clearup()
{
	for ( const auto & it : _Children )
	{
		it->Clearup();
	}

	_Children.clear();

	_Controller->Clearup();

	_Layout = nullptr;
	_Controller = nullptr;
	_Dirty = false;
	_Enable = true;
}

bool XE::Canvas::GetEnable() const
{
	return _Enable;
}

void XE::Canvas::SetEnable( bool val )
{
	_Enable = val;

	Rebuild();
}

const XE::Recti & XE::Canvas::GetRect() const
{
	return _Rect;
}

void XE::Canvas::SetRect( const XE::Recti & val )
{
	_Rect = val;

	Rebuild();
}

const XE::String & XE::Canvas::GetName() const
{
	return _Name;
}

void XE::Canvas::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::ControllerPtr & XE::Canvas::GetController() const
{
	return _Controller;
}

void XE::Canvas::SetController( const XE::ControllerPtr & val )
{
	_Controller = val;
}

const XE::LayoutPtr & XE::Canvas::GetLayout() const
{
	return _Layout;
}

void XE::Canvas::SetLayout( const XE::LayoutPtr & val )
{
	_Layout = val;

	Rebuild();
}

const XE::Array< XE::WidgetPtr > & XE::Canvas::GetChildren() const
{
	return _Children;
}

XE::WidgetPtr XE::Canvas::FindChild( const XE::String & val ) const
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

void XE::Canvas::Rebuild()
{
	_Dirty = true;
}
