#include "Canvas.h"

#include "Core/CoreFramework.h"

#include "Widget.h"
#include "Layout.h"
#include "GUIService.h"
#include "imgui_impl.h"
#include "Controller.h"

BEG_META( XE::Canvas )
type->Property( "Enable", &XE::Canvas::_Enable );
type->Property( "Name", &XE::Canvas::_Name );
type->Property( "Rect", &XE::Canvas::_Rect );
type->Property( "Model", &XE::Canvas::_Controller );
type->Property( "Style", &XE::Canvas::GetStyle, &XE::Canvas::SetStyle );
type->Property( "Layout", &XE::Canvas::_Layout );
type->Property( "Children", &XE::Canvas::_Children )->Attribute( XE::NonEditorAttribute() );
END_META()

XE::Canvas::Canvas()
	: _Style( XE::New< ImGuiStyle >() )
{

}

XE::Canvas::~Canvas()
{
	XE::Delete( _Style );
}

void XE::Canvas::Startup()
{
	_Context = ImGui::CreateContext( XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::GUIService >()->GetFontAtlas() );

	_Impl->StartupContext( _Context );

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

		ImGui::SetCurrentContext( _Context );
		{
			ImGui::GetStyle() = GetStyle();

			auto & io = ImGui::GetIO();
			{

			}

			ImGui::NewFrame();
			{
				ImGui::SetNextWindowPos( _Rect.GetMin() );
				ImGui::SetNextWindowSize( _Rect.GetSize() );

				ImGui::Begin( _Name.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground );
				{
					for ( const auto & it : _Children )
					{
						it->Update();
						it->Render();
					}
				}
				ImGui::End();
			}
			ImGui::Render();
		}
		ImGui::SetCurrentContext( nullptr );

		_Dirty = false;

		_Impl->RenderContext( _Context, _PassEncoder );
	}
}

void XE::Canvas::Clearup()
{
	for ( const auto & it : _Children )
	{
		it->Clearup();
	}

	_Impl->ClearupContext( _Context );
	ImGui::DestroyContext( _Context );

	_Children.clear();

	_Controller->Clearup();

	_Context = nullptr;
	_Layout = nullptr;
	_Controller = nullptr;
	_Impl = nullptr;
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

const ImGuiStyle & XE::Canvas::GetStyle() const
{
	return *_Style;
}

void XE::Canvas::SetStyle( const ImGuiStyle & val )
{
	*_Style = val;

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

const XE::ImGuiImplPtr & XE::Canvas::GetImpl() const
{
	return _Impl;
}

void XE::Canvas::SetImpl( const XE::ImGuiImplPtr & val )
{
	_Impl = val;
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
