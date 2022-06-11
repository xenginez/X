#include "Canvas.h"

#include "Widget.h"

BEG_META( XE::Canvas )
type->Property( "Enable", &XE::Canvas::GetEnable, &XE::Canvas::SetEnable );
type->Property( "Name", &XE::Canvas::GetName, &XE::Canvas::SetName );
type->Property( "Rect", &XE::Canvas::GetRect, &XE::Canvas::SetRect );
type->Property( "Style", &XE::Canvas::GetStyle, &XE::Canvas::SetStyle );
type->Property( "Widgets", &XE::Canvas::GetWidgets, &XE::Canvas::SetWidgets )->Attribute( XE::NonEditorAttribute() );
END_META()

struct XE::Canvas::Private
{
	bool _Dirty = false;

	bool _Enable = true;
	XE::Recti _Rect;
	XE::String _Name;
	ImGuiStyle _Style;
	ImGuiContext * _Context = nullptr;
	XE::Array< XE::WidgetPtr > _Widgets;
};

XE::Canvas::Canvas()
	:_p( XE::New< Private >() )
{

}

XE::Canvas::~Canvas()
{
	XE::Delete( _p );
}

void XE::Canvas::Startup()
{
	_p->_Context = ImGui::CreateContext();

	for ( const auto & it : _p->_Widgets )
	{
		it->_Canvas = XE_THIS( XE::Canvas );

		it->Startup();
	}
}

void XE::Canvas::Update()
{
	if ( GetEnable() && _p->_Dirty )
	{
		ImGui::SetCurrentContext( _p->_Context );

		ImGui::GetStyle() = _p->_Style;

		ImGui::NewFrame();
		{
			ImGui::SetNextWindowPos( _p->_Rect.GetMin() );
			ImGui::SetNextWindowSize( _p->_Rect.GetSize() );
			ImGui::SetNextWindowBgAlpha( 0.0f );

			ImGui::Begin( _p->_Name.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground );
			{
				for ( const auto & it : _p->_Widgets )
				{
					it->Update();
					it->Render();
				}
			}
			ImGui::End();
		}
		ImGui::Render();

		_p->_Dirty = false;
	}
}

void XE::Canvas::Clearup()
{
	for ( const auto & it : _p->_Widgets )
	{
		it->Clearup();
	}

	ImGui::DestroyContext( _p->_Context );
	_p->_Context = nullptr;
	_p->_Widgets.clear();

	_p->_Dirty = false;
}

bool XE::Canvas::GetEnable() const
{
	return _p->_Enable;
}

void XE::Canvas::SetEnable( bool val )
{
	_p->_Enable = val;
}

const ImGuiStyle & XE::Canvas::GetStyle() const
{
	return _p->_Style;
}

void XE::Canvas::SetStyle( const ImGuiStyle & val )
{
	_p->_Style = val;
}

const XE::Recti & XE::Canvas::GetRect() const
{
	return _p->_Rect;
}

void XE::Canvas::SetRect( const XE::Recti & val )
{
	_p->_Rect = val;
}

const XE::String & XE::Canvas::GetName() const
{
	return _p->_Name;
}

void XE::Canvas::SetName( const XE::String & val )
{
	_p->_Name = val;
}

const XE::Array< XE::WidgetPtr > & XE::Canvas::GetWidgets() const
{
	return _p->_Widgets;
}

void XE::Canvas::SetWidgets( const XE::Array< XE::WidgetPtr > & val )
{
	_p->_Widgets = val;
}

XE::WidgetPtr XE::Canvas::FindWidget( const XE::String & val ) const
{
	auto end = val.find( '/' );
	auto name = val.substr( 0, end );

	auto it = std::find_if( _p->_Widgets.begin(), _p->_Widgets.end(), [&]( const auto & it ) { return it->GetName() == name; } );
	if ( it != _p->_Widgets.end() )
	{
		return ( end == XE::String::npos ) ? *it : ( *it )->FindChild( val.substr( end + 1 ) );
	}

	return nullptr;
}

void XE::Canvas::Rebuild()
{
	_p->_Dirty = true;
}
