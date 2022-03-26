#include "InputStateItem.h"

BEG_META( XE::InputStateItem )
{
	type->Property( "Input", &XE::InputStateItem::_Input );
	type->Property( "Type", &XE::InputStateItem::_Type );
	type->Property( "Timeout", &XE::InputStateItem::_Timeout );
}
END_META()

BEG_META( XE::InputStateRangeItem )
{
	type->Property( "Min", &XE::InputStateRangeItem::_Min );
	type->Property( "Max", &XE::InputStateRangeItem::_Max );
}
END_META()

BEG_META( XE::InputStateButtonItem )
{
	type->Property( "Pressed", &XE::InputStateButtonItem::_Pressed );
}
END_META()

XE::InputStateItem::InputStateItem()
{

}

XE::InputStateItem::~InputStateItem()
{

}

XE::float32 XE::InputStateItem::GetTimeout() const
{
	return _Timeout;
}

const XE::String & XE::InputStateItem::GetInputName() const
{
	return _Input;
}

XE::InputStateItemTimeoutType XE::InputStateItem::GetTimeoutType() const
{
	return _Type;
}

XE::InputStateRangeItem::InputStateRangeItem()
{

}

XE::InputStateRangeItem::~InputStateRangeItem()
{

}

XE::float32 XE::InputStateRangeItem::GetMin() const
{
	return _Min;
}

XE::float32 XE::InputStateRangeItem::GetMax() const
{
	return _Max;
}

bool XE::InputStateRangeItem::Condition( const XE::Variant & val ) const
{
	XE::float32 value = val.Value< XE::float32 >();

	return value >= _Min && value < _Max;
}

XE::InputStateButtonItem::InputStateButtonItem()
{

}

XE::InputStateButtonItem::~InputStateButtonItem()
{

}

bool XE::InputStateButtonItem::GetPressed() const
{
	return _Pressed;
}

bool XE::InputStateButtonItem::Condition( const XE::Variant & val ) const
{
	return _Pressed == val.Value< bool >();
}
