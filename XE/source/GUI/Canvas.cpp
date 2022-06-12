#include "Canvas.h"

#include "Model.h"
#include "Widget.h"
#include "imgui_impl.h"

BEG_META( XE::Canvas )
type->Property( "Enable", &XE::Canvas::_Enable );
type->Property( "Name", &XE::Canvas::_Name );
type->Property( "Rect", &XE::Canvas::_Rect );
type->Property( "Model", &XE::Canvas::_Model );
type->Property( "Style", &XE::Canvas::_Style );
type->Property( "Widgets", &XE::Canvas::_Widgets )->Attribute( XE::NonEditorAttribute() );
END_META()

XE::Canvas::Canvas()
{

}

XE::Canvas::~Canvas()
{

}

void XE::Canvas::Startup()
{
	_Context = ImGui::CreateContext();

	_Impl->StartupContext( _Context );

	for ( const auto & it : _Widgets )
	{
		it->_Canvas = XE_THIS( XE::Canvas );

		it->Startup();
	}

	_Model->Startup();
}

void XE::Canvas::Update()
{
	if ( GetEnable() && _Dirty )
	{
		ImGui::SetCurrentContext( _Context );
		{
			ImGui::GetStyle() = _Style;

			ImGui::NewFrame();
			{
				ImGui::SetNextWindowPos( _Rect.GetMin() );
				ImGui::SetNextWindowSize( _Rect.GetSize() );
				ImGui::SetNextWindowBgAlpha( 0.0f );

				ImGui::Begin( _Name.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground );
				{
					for ( const auto & it : _Widgets )
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
	for ( const auto & it : _Widgets )
	{
		it->Clearup();
	}

	_Impl->ClearupContext( _Context );
	ImGui::DestroyContext( _Context );

	_Widgets.clear();

	_Model->Clearup();

	_Context = nullptr;
	_Model = nullptr;
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
	return _Style;
}

void XE::Canvas::SetStyle( const ImGuiStyle & val )
{
	_Style = val;
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

const XE::ModelPtr & XE::Canvas::GetModel() const
{
	return _Model;
}

void XE::Canvas::SetModel( const XE::ModelPtr & val )
{
	_Model = val;
}

const XE::ImGuiImplPtr & XE::Canvas::GetImpl() const
{
	return _Impl;
}

void XE::Canvas::SetImpl( const XE::ImGuiImplPtr & val )
{
	_Impl = val;
}

const XE::Array< XE::WidgetPtr > & XE::Canvas::GetWidgets() const
{
	return _Widgets;
}

XE::WidgetPtr XE::Canvas::FindWidget( const XE::String & val ) const
{
	auto end = val.find( '/' );
	auto name = val.substr( 0, end );

	auto it = std::find_if( _Widgets.begin(), _Widgets.end(), [&]( const auto & it ) { return it->GetName() == name; } );
	if ( it != _Widgets.end() )
	{
		return ( end == XE::String::npos ) ? *it : ( *it )->FindChild( val.substr( end + 1 ) );
	}

	return nullptr;
}

void XE::Canvas::Rebuild()
{
	_Dirty = true;
}
