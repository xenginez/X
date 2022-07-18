#include "PadInput.h"

#include "Core/TimerService.h"
#include "Core/EventService.h"
#include "Core/CoreFramework.h"

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
#include <Windows.h>
#include <XInput.h>
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	XE::uint64 i = start;
	for ( ; i < 10; i++ )
	{
		XINPUT_CAPABILITIES caps = {};
		::XInputGetCapabilities( i, XINPUT_FLAG_GAMEPAD, &caps );
		if ( caps.Type != XINPUT_DEVTYPE_GAMEPAD )
		{
			break;
		}
	}
	return i;
}
bool PadInputVibrate( XE::uint32 index, XE::float32 left, XE::float32 right )
{
	XINPUT_VIBRATION xvibration;
	xvibration.wLeftMotorSpeed = static_cast<WORD>( left * 65535 );
	xvibration.wRightMotorSpeed = static_cast<WORD>( right * 65535 );
	return ::XInputSetState( index, &xvibration ) == ERROR_SUCCESS;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	XE::uint32 index = info.win_lparam;
	XE::uint64 packets = info.win_wparam;

	XINPUT_STATE xstate;
	DWORD result = ::XInputGetState( index, &xstate );

	if ( result != ERROR_SUCCESS )
	{
		return packets;
	}

	if ( xstate.dwPacketNumber == packets )
	{
		return packets;
	}

	auto handle_button = [&]( XE::PadCode code, bool flag )
	{
		code = static_cast<XE::PadCode>( static_cast<XE::uint32>( code ) + ( static_cast<XE::uint32>( XE::PadCode::Pad0Button31 ) * index ) );


		// TODO: 
	};
	auto handle_axis = [&]( XE::PadCode code, XE::float32 val )
	{
		code = static_cast<XE::PadCode>( static_cast<XE::uint32>( code ) + ( static_cast<XE::uint32>( XE::PadCode::Pad0Button31 ) * index ) );

		// TODO: 
	};
	auto get_axis_value = []( XE::uint16 value )
	{
		if ( value < 0 )
		{
			return float( value ) / 32768;
		}
		else
		{
			return float( value ) / 32767;
		}
	};

	handle_button( XE::PadCode::Pad0ButtonUp, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonDown, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonLeft, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonRight, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonA, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_A ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonB, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_B ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonX, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_X ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonY, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_Y ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonStart, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_START ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonSelect, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonL3, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonR3, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonL1, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonR1, ( xstate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) != 0 );
	handle_button( XE::PadCode::Pad0ButtonL2, xstate.Gamepad.bLeftTrigger != 0 );
	handle_button( XE::PadCode::Pad0ButtonR2, xstate.Gamepad.bRightTrigger != 0 );

	handle_axis( XE::PadCode::Pad0ButtonAxis4, float( xstate.Gamepad.bLeftTrigger ) / 255 );
	handle_axis( XE::PadCode::Pad0ButtonAxis5, float( xstate.Gamepad.bRightTrigger ) / 255 );

	handle_axis( XE::PadCode::Pad0ButtonLeftStickX, get_axis_value( xstate.Gamepad.sThumbLX ) );
	handle_axis( XE::PadCode::Pad0ButtonLeftStickY, get_axis_value( xstate.Gamepad.sThumbLY ) );
	handle_axis( XE::PadCode::Pad0ButtonRightStickX, get_axis_value( xstate.Gamepad.sThumbRX ) );
	handle_axis( XE::PadCode::Pad0ButtonRightStickY, get_axis_value( xstate.Gamepad.sThumbRY ) );

	return xstate.dwPacketNumber;
}
#elif PLATFORM_OS & OS_LINUX
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#elif PLATFORM_OS & OS_MAC
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#elif PLATFORM_OS & OS_IOS
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#elif PLATFORM_OS & OS_HTML5
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#elif PLATFORM_OS & OS_ANDROID
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#elif PLATFORM_OS & OS_NINTENDO
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#elif PLATFORM_OS & OS_PLAY_STATION
XE::uint64 PadInputCapabilities( XE::uint64 start )
{
	return 0;
}
XE::uint64 PadInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	return 0;
}
#endif

IMPLEMENT_META( XE::PadInput );

struct XE::PadInput::Private
{
	XE::InputState * _State;
	XE::InputState _State1, _State2;
	XE::Array< XE::uint64 > _Packets;
};

XE::PadInput::PadInput()
	:_p( XE::New< Private >() )
{

}

XE::PadInput::~PadInput()
{
	XE::Delete( _p );
}

void XE::PadInput::Startup()
{
	_p->_State = &_p->_State1;

	_p->_Packets.resize( PadInputCapabilities( 0 ) );
}

void XE::PadInput::Update()
{
	auto prev = _p->_State;
	_p->_State = _p->_State == &_p->_State1 ? &_p->_State2 : &_p->_State1;

	for ( size_t i = 0; i < _p->_Packets.size(); i++ )
	{
		XE::InputEventInfo info;
		info.win_lparam = i;
		info.win_wparam = _p->_Packets[i];
		_p->_Packets[i] = PadInputHandleMessage( _p->_State, prev, info );
	}

	prev->Clear();
}

void XE::PadInput::Clearup()
{
	_p->_State1.Clear();
	_p->_State2.Clear();

	_p->_Packets.clear();
}

void XE::PadInput::ClearStatus()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
}

const XE::InputState & XE::PadInput::GetState() const
{
	return *_p->_State;
}

bool XE::PadInput::Vibrate( XE::uint32 index, XE::float32 left, XE::float32 right )
{
	return PadInputVibrate( index, left, right );
}
