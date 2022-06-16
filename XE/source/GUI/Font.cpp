#include "Font.h"

BEG_META( XE::Font )
type->Property( "Name", &XE::Font::_Name );
type->Property( "FontSize", &XE::Font::_FontSize );
type->Property( "PixelSize", &XE::Font::_PixelSize );
type->Property( "FontBold", &XE::Font::_FontBold );
type->Property( "FontItalic", &XE::Font::_FontItalic );
type->Property( "Underline", &XE::Font::_Underline );
type->Property( "Strikeout", &XE::Font::_Strikeout );
END_META()

XE::Font::Font()
{

}

XE::Font::~Font()
{

}

const XE::Utf8String & XE::Font::GetName() const
{
	return _Name;
}

void XE::Font::SetName( const XE::Utf8String & val )
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

bool XE::Font::GetFontBold() const
{
	return _FontBold;
}

void XE::Font::SetFontBold( bool val )
{
	_FontBold = val;
}

bool XE::Font::GetFontItalic() const
{
	return _FontItalic;
}

void XE::Font::SetFontItalic( bool val )
{
	_FontItalic = val;
}

bool XE::Font::GetUnderline() const
{
	return _Underline;
}

void XE::Font::SetUnderline( bool val )
{
	_Underline = val;
}

bool XE::Font::GetStrikeout() const
{
	return _Strikeout;
}

void XE::Font::SetStrikeout( bool val )
{
	_Strikeout = val;
}
