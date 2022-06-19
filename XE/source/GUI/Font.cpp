#include "Font.h"

BEG_META( XE::Font )
type->Property( "Name", &XE::Font::_Name );
type->Property( "FontSize", &XE::Font::_FontSize );
type->Property( "PixelSize", &XE::Font::_PixelSize );
END_META()

XE::Font::Font()
{

}

XE::Font::~Font()
{

}

const XE::String & XE::Font::GetName() const
{
	return _Name;
}

void XE::Font::SetName( const XE::String & val )
{
	_Name = val;
}

XE::uint32 XE::Font::GetFontSize() const
{
	return _FontSize;
}

void XE::Font::SetFontSize( XE::uint32 val )
{
	_FontSize = val;
}

XE::uint32 XE::Font::GetPixelSize() const
{
	return _PixelSize;
}

void XE::Font::SetPixelSize( XE::uint32 val )
{
	_PixelSize = val;
}
