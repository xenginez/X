#include "MouseInput.h"

#include "Core/EventService.h"
#include "Core/CoreFramework.h"

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
#include <Windows.h>
#include <windowsx.h>
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	XE::MouseCode code;
	bool move = false;
	bool pressed = false;
	XE::int32 ax = -1;
	XE::int32 ay = -1;
	XE::int32 wheel = 0;

	switch ( info.win_message )
	{
	case WM_LBUTTONDOWN:
	{
		code = XE::MouseCode::MouseLeft;
		pressed = true;
	}
	break;
	case WM_LBUTTONUP:
	{
		code = XE::MouseCode::MouseLeft;
		pressed = false;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		code = XE::MouseCode::MouseRight;
		pressed = true;
	}
	break;
	case WM_RBUTTONUP:
	{
		code = XE::MouseCode::MouseRight;
		pressed = false;
	}
	break;
	case WM_MBUTTONDOWN:
	{
		code = XE::MouseCode::MouseMiddle;
		pressed = true;
	}
	break;
	case WM_MBUTTONUP:
	{
		code = XE::MouseCode::MouseMiddle;
		pressed = false;
	}
	break;
	case WM_XBUTTONDOWN:
	{
		code = (XE::MouseCode)( (XE::uint32)XE::MouseCode::MouseButton4 + GET_XBUTTON_WPARAM( info.win_wparam ) );
		pressed = true;
	}
	break;
	case WM_XBUTTONUP:
	{
		code = (XE::MouseCode)( (XE::uint32)XE::MouseCode::MouseButton4 + GET_XBUTTON_WPARAM( info.win_wparam ) );
		pressed = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		move = true;
		ax = GET_X_LPARAM( info.win_lparam );
		ay = GET_Y_LPARAM( info.win_lparam );
		code = XE::MouseCode::MousePos;
	}
	break;
	case WM_MOUSEWHEEL:
	{
		wheel = GET_WHEEL_DELTA_WPARAM( info.win_wparam );
		code = XE::MouseCode::MouseWheel;
	}
	break;
	case WM_INPUT:
	{
		XE::uint8 data[40];
		XE::uint32 size = 40;

		GetRawInputData( (HRAWINPUT)info.win_lparam, RID_INPUT, data, &size, sizeof( RAWINPUTHEADER ) );

		RAWINPUT * raw = (RAWINPUT *)data;

		if ( raw->header.dwType == RIM_TYPEMOUSE )
		{
			if ( raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE )
			{
				move = true;
				ax = raw->data.mouse.lLastX;
				ay = raw->data.mouse.lLastY;
				code = XE::MouseCode::MouseAxisX;
			}
			else if ( raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE )
			{
				move = true;
				ax = raw->data.mouse.lLastX;
				ay = raw->data.mouse.lLastY;
				code = XE::MouseCode::MousePos;
			}

			if ( raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL )
			{
				wheel = raw->data.mouse.usButtonData;
			}
			else
			{
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN )
				{
					code = XE::MouseCode::MouseLeft;
					pressed = true;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP )
				{
					code = XE::MouseCode::MouseLeft;
					pressed = false;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN )
				{
					code = XE::MouseCode::MouseRight;
					pressed = true;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP )
				{
					code = XE::MouseCode::MouseRight;
					pressed = false;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN )
				{
					code = XE::MouseCode::MouseMiddle;
					pressed = true;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP )
				{
					code = XE::MouseCode::MouseMiddle;
					pressed = false;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN )
				{
					code = XE::MouseCode::MouseButton4;
					pressed = true;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP )
				{
					code = XE::MouseCode::MouseButton4;
					pressed = false;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN )
				{
					code = XE::MouseCode::MouseButton5;
					pressed = true;
				}
				if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP )
				{
					code = XE::MouseCode::MouseButton5;
					pressed = false;
				}
			}
		}
	}
	default:
		return;
	}

	// TODO: 
}
#elif PLATFORM_OS & OS_LINUX
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_MAC
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_IOS
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_WASM
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_ANDROID
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_NINTENDO
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_PLAY_STATION
void MouseInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#endif

IMPLEMENT_META( XE::MouseInput );

struct XE::MouseInput::Private
{
	XE::Disposable _Event;

	XE::InputState * _State;
	XE::InputState _State1, _State2;
	XE::Array< XE::InputEventInfo > _Infos;
};

XE::MouseInput::MouseInput()
	:_p( XE::New< Private >() )
{

}

XE::MouseInput::~MouseInput()
{
	XE::Delete( _p );
}

void XE::MouseInput::Startup()
{
	_p->_State = &_p->_State1;

	_p->_Event = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->RegisterListener( XE::EVENT_MOUSE, [this]( const XE::EventPtr & event )
		{
			_p->_Infos.push_back( event->parameter.Value< XE::InputEventInfo >() );
		} );
}

void XE::MouseInput::Update()
{
	auto prev = _p->_State; _p->_State = _p->_State == &_p->_State1 ? &_p->_State2 : &_p->_State1;

	for ( const auto & info : _p->_Infos )
	{
		MouseInputHandleMessage( _p->_State, prev, info );
	}

	prev->Clear();
	_p->_Infos.clear();
}

void XE::MouseInput::Clearup()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
	_p->_Event.Dispose();
}

void XE::MouseInput::ClearStatus()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
}

const XE::InputState & XE::MouseInput::GetState() const
{
	return *_p->_State;
}
