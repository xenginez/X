#include "Slider.h"

#include "imgui.h"

BEG_META( XE::Slider )
type->Property( "Min", &XE::Slider::_Min );
type->Property( "Max", &XE::Slider::_Max );
type->Property( "Value", &XE::Slider::_Value );
type->Property( "OnValueChanged", &XE::Slider::_OnValueChanged );
END_META()

XE::Slider::Slider()
{

}

XE::Slider::~Slider()
{

}

void XE::Slider::OnRender()
{
	if ( ImGui::SliderInt( "", &_Value, _Min, _Max ) )
	{
		_OnValueChanged( GetModel(), _Value );
	}
}

XE::int32 XE::Slider::GetMin() const
{
	return _Min;
}

void XE::Slider::SetMin( XE::int32 val )
{
	_Min = val;
}

XE::int32 XE::Slider::GetMax() const
{
	return _Max;
}

void XE::Slider::SetMax( XE::int32 val )
{
	_Max = val;
}

XE::int32 XE::Slider::GetValue() const
{
	return _Value;
}

void XE::Slider::SetValue( XE::int32 val )
{
	_Value = val;
}
