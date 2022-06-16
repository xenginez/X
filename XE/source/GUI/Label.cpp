#include "Label.h"

#include "Core/LocalizationService.h"

#include "imconfig.h"
#include "imgui.h"
#include "Canvas.h"
#include "GUIService.h"

BEG_META( XE::Label )
END_META()

XE::Label::Label()
{

}

XE::Label::~Label()
{

}

void XE::Label::OnStartup()
{

}

void XE::Label::OnUpdate()
{

}

void XE::Label::OnRender()
{
	if ( auto local = GetFramework()->GetServiceT< XE::LocalizationService >() )
	{
		ImGui::PushFont( GetFramework()->GetServiceT< XE::GUIService >()->FindFont( local->LocalizedString( _Font ) ) );
		{
			ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( _Color.r, _Color.g, _Color.b, _Color.a ) );
			{
				const auto & str = local->LocalizedString( _Text );

				auto w_size = ImGui::GetWindowSize();
				auto t_size = ImGui::CalcTextSize( str.char_str() );

				XE::float32 x, y;
				switch ( _HAlignment )
				{
				case XE::HAlignment::LEFT:
					x = 0;
					break;
				case XE::HAlignment::RIGHT:
					x = w_size.x;
					break;
				case XE::HAlignment::CENTER:
					x = std::max( 20.0f, ( w_size.x - t_size.x ) * 0.5f );
					break;
				default:
					break;
				}
				switch ( _VAlignment )
				{
				case XE::VAlignment::TOP:
					y = 0;
					break;
				case XE::VAlignment::BOTTOM:
					y = w_size.y - t_size.y;
					break;
				case XE::VAlignment::CENTER:
					y = w_size.y * 0.5f;
					break;
				default:
					break;
				}

				if ( !_WordWarp )
				{
					ImGui::SetCursorPos( { x, y } );
					ImGui::Text( str.char_str() );
				}
				else
				{
					ImGui::SameLine( x );
					ImGui::PushTextWrapPos( w_size.x - x );
					ImGui::TextWrapped( str.char_str() );
					ImGui::PopTextWrapPos();
				}
			}
			ImGui::PopStyleColor();
		}
		ImGui::PopFont();
	}
}

void XE::Label::OnClearup()
{

}

const XE::Utf8String & XE::Label::GetFont() const
{
	return _Font;
}

void XE::Label::SetFont( const XE::Utf8String & val )
{
	_Font = val;

	GetCanvas()->Rebuild();
}

const XE::Color & XE::Label::GetColor() const
{
	return _Color;
}

void XE::Label::SetColor( const XE::Color & val )
{
	_Color = val;

	GetCanvas()->Rebuild();
}

const XE::Utf8String & XE::Label::GetText() const
{
	return _Text;
}

void XE::Label::SetText( const XE::Utf8String & val )
{
	_Text = val;

	GetCanvas()->Rebuild();
}

XE::VAlignment XE::Label::GetVerticalAlignment() const
{
	return _VAlignment;
}

void XE::Label::SetVerticalAlignment( XE::VAlignment val )
{
	_VAlignment = val;

	GetCanvas()->Rebuild();
}

XE::HAlignment XE::Label::GetHorizontalAlignment() const
{
	return _HAlignment;
}

void XE::Label::SetHorizontalAlignment( XE::HAlignment val )
{
	_HAlignment = val;

	GetCanvas()->Rebuild();
}

bool XE::Label::GetWordWrap() const
{
	return _WordWarp;
}

void XE::Label::SetWordWrap( bool val )
{
	_WordWarp = val;

	GetCanvas()->Rebuild();
}
