#include "Layout.h"

BEG_META( XE::Layout )
type->Property( "Spacing", &XE::Layout::_Spacing );
type->Property( "Margins", &XE::Layout::_Margins );
type->Property( "Alignment", &XE::Layout::_Alignment );
END_META()

XE::Layout::Layout()
{

}

XE::Layout::~Layout()
{

}

XE::int32 XE::Layout::GetSpacing() const
{
	return _Spacing;
}

void XE::Layout::SetSpacing( XE::int32 val )
{
	_Spacing = val;
}

XE::int32 XE::Layout::GetLeftMargin() const
{
	return _Margins.x;
}

void XE::Layout::SetLeftMargin( XE::int32 val )
{
	_Margins.x = val;
}

XE::int32 XE::Layout::GetRightMargin() const
{
	return _Margins.y;
}

void XE::Layout::SetRightMargin( XE::int32 val )
{
	_Margins.y = val;
}

XE::int32 XE::Layout::GetTopMargin() const
{
	return _Margins.z;
}

void XE::Layout::SetTopMargin( XE::int32 val )
{
	_Margins.z = val;
}

XE::int32 XE::Layout::GetBottomMargin() const
{
	return _Margins.w;
}

void XE::Layout::SetBottomMargin( XE::int32 val )
{
	_Margins.w = val;
}

const XE::Vec4i & XE::Layout::GetMargins() const
{
	return _Margins;
}

void XE::Layout::SetMargins( const XE::Vec4i & val )
{
	_Margins = val;
}

XE::AlignmentFlags XE::Layout::GetAlignment() const
{
	return _Alignment;
}

void XE::Layout::SetAlignment( XE::AlignmentFlags val )
{
	_Alignment = val;
}

