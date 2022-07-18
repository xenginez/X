#include "KeyboardInput.h"

#include "Core/EventService.h"
#include "Core/CoreFramework.h"

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
#include <Windows.h>
#include <windowsx.h>
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{
	static XE::Map< XE::uint32, XE::KeyCode > map =
	{
		{ VK_ESCAPE, XE::KeyCode::KeyEscape },
		{ VK_F1, XE::KeyCode::KeyF1 },
		{ VK_F2, XE::KeyCode::KeyF2 },
		{ VK_F3, XE::KeyCode::KeyF3 },
		{ VK_F4, XE::KeyCode::KeyF4 },
		{ VK_F5, XE::KeyCode::KeyF5 },
		{ VK_F6, XE::KeyCode::KeyF6 },
		{ VK_F7, XE::KeyCode::KeyF7 },
		{ VK_F8, XE::KeyCode::KeyF8 },
		{ VK_F9, XE::KeyCode::KeyF9 },
		{ VK_F10, XE::KeyCode::KeyF10 },
		{ VK_F11, XE::KeyCode::KeyF11 },
		{ VK_F12, XE::KeyCode::KeyF12 },
		{ VK_PRINT, XE::KeyCode::KeyPrint },
		{ VK_SCROLL, XE::KeyCode::KeyScrollLock },
		{ VK_PAUSE, XE::KeyCode::KeyBreak },
		{ VK_SPACE, XE::KeyCode::KeySpace },
		{ VK_OEM_COMMA, XE::KeyCode::KeyComma },
		{ '0', XE::KeyCode::Key0 },
		{ '1', XE::KeyCode::Key1 },
		{ '2', XE::KeyCode::Key2 },
		{ '3', XE::KeyCode::Key3 },
		{ '4', XE::KeyCode::Key4 },
		{ '5', XE::KeyCode::Key5 },
		{ '6', XE::KeyCode::Key6 },
		{ '7', XE::KeyCode::Key7 },
		{ '8', XE::KeyCode::Key8 },
		{ '9', XE::KeyCode::Key9 },
		{ 'A', XE::KeyCode::KeyA },
		{ 'B', XE::KeyCode::KeyB },
		{ 'C', XE::KeyCode::KeyC },
		{ 'D', XE::KeyCode::KeyD },
		{ 'E', XE::KeyCode::KeyE },
		{ 'F', XE::KeyCode::KeyF },
		{ 'G', XE::KeyCode::KeyG },
		{ 'H', XE::KeyCode::KeyH },
		{ 'I', XE::KeyCode::KeyI },
		{ 'J', XE::KeyCode::KeyJ },
		{ 'K', XE::KeyCode::KeyK },
		{ 'L', XE::KeyCode::KeyL },
		{ 'M', XE::KeyCode::KeyM },
		{ 'N', XE::KeyCode::KeyN },
		{ 'O', XE::KeyCode::KeyO },
		{ 'P', XE::KeyCode::KeyP },
		{ 'Q', XE::KeyCode::KeyQ },
		{ 'R', XE::KeyCode::KeyR },
		{ 'S', XE::KeyCode::KeyS },
		{ 'T', XE::KeyCode::KeyT },
		{ 'U', XE::KeyCode::KeyU },
		{ 'V', XE::KeyCode::KeyV },
		{ 'W', XE::KeyCode::KeyW },
		{ 'X', XE::KeyCode::KeyX },
		{ 'Y', XE::KeyCode::KeyY },
		{ 'Z', XE::KeyCode::KeyZ },
		{ VK_LEFT, XE::KeyCode::KeyLeftArrow },
		{ VK_RIGHT, XE::KeyCode::KeyRightArrow },
		{ VK_UP, XE::KeyCode::KeyUpArrow },
		{ VK_DOWN, XE::KeyCode::KeyDownArrow },
		{ VK_INSERT, XE::KeyCode::KeyInsert },
		{ VK_HOME, XE::KeyCode::KeyHome },
		{ VK_DELETE, XE::KeyCode::KeyDelete },
		{ VK_END, XE::KeyCode::KeyEnd },
		{ VK_PRIOR, XE::KeyCode::KeyPageUp },
		{ VK_NEXT, XE::KeyCode::KeyPageDown },
		{ VK_BACK, XE::KeyCode::KeyBackspace },
		{ VK_TAB, XE::KeyCode::KeyTab },
		{ VK_RETURN, XE::KeyCode::KeyReturn },
		{ VK_CAPITAL, XE::KeyCode::KeyCapsLock },
		{ VK_LSHIFT, XE::KeyCode::KeyLeftShift },
		{ VK_LCONTROL, XE::KeyCode::KeyLeftControl },
		{ VK_LWIN, XE::KeyCode::KeyLeftCommand },
		{ VK_LMENU, XE::KeyCode::KeyLeftAlt },
		{ VK_RMENU, XE::KeyCode::KeyRightAlt },
		{ VK_RWIN, XE::KeyCode::KeyRightCommand },
		{ VK_APPS, XE::KeyCode::KeyMenu },
		{ VK_RCONTROL, XE::KeyCode::KeyRightControl },
		{ VK_RSHIFT, XE::KeyCode::KeyRightShift },
		{ VK_SNAPSHOT, XE::KeyCode::KeyPrint },
		{ VK_OEM_MINUS, XE::KeyCode::KeyMinus },
		{ VK_OEM_PERIOD, XE::KeyCode::KeyPeriod },
		{ VK_OEM_PLUS, XE::KeyCode::KeyPlus },
// 		{ VK_OEM_1, XE::KeyCode::KeyExtra6 },
// 		{ VK_OEM_2, XE::KeyCode::KeyExtra3 },
// 		{ VK_OEM_3, XE::KeyCode::KeyExtra5 },
// 		{ VK_OEM_4, XE::KeyCode::KeyExtra1 },
// 		{ VK_OEM_5, XE::KeyCode::KeyApostrophe },
// 		{ VK_OEM_6, XE::KeyCode::KeyExtra2 },
// 		{ VK_OEM_7, XE::KeyCode::KeyExtra4 },
// 		{ VK_BROWSER_BACK, XE::KeyCode::KeyBack },
// 		{ VK_BROWSER_FORWARD, XE::KeyCode::KeyForward },
	};

	if ( info.win_message == WM_CHAR )
	{
		int key = info.win_wparam;
		if ( key == VK_BACK || key == VK_ESCAPE || key == VK_TAB || key == VK_RETURN )
		{
			return;
		}

		cur_state->PutUnicode( static_cast<char32_t>( key ) );
	}
	else if ( info.win_message == WM_INPUT )
	{
		const RAWINPUT * raw = reinterpret_cast<const RAWINPUT *>( info.raw_data.data() );

		if ( raw->header.dwType == RIM_TYPEKEYBOARD )
		{
			XE::uint32 key = raw->data.keyboard.VKey;

			if ( key == VK_CONTROL )
			{
				if ( raw->data.keyboard.Flags & RI_KEY_E0 )
					key = VK_RCONTROL;
				else
					key = VK_LCONTROL;
			}
			else if ( key == VK_SHIFT )
			{
				if ( raw->data.keyboard.MakeCode == 0x36 )
					key = VK_RSHIFT;
				else
					key = VK_LSHIFT;
			}
			else if ( key == VK_MENU )
			{
				if ( raw->data.keyboard.Flags & RI_KEY_E0 )
					key = VK_RMENU;
				else
					key = VK_LMENU;
			}

			auto it = map.find( key );
			if ( it != map.end() )
			{
				XE::KeyCode code = it->second;
				bool pressed = raw->data.keyboard.Message == WM_KEYDOWN || raw->data.keyboard.Message == WM_SYSKEYDOWN;
				// TODO: 
			}
		}
	}
	else if ( info.win_message == WM_KEYUP || info.win_message == WM_SYSKEYUP || info.win_message == WM_KEYDOWN || info.win_message == WM_SYSKEYDOWN )
	{
		XE::uint32 lparam_mask = 1 << 24;

		bool pressed = false;
		XE::uint32 key = info.win_wparam;
		switch ( info.win_message )
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			pressed = true;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			pressed = false;
			break;
		}

		if ( key == VK_CONTROL )
		{
			key = ( info.win_lparam & lparam_mask ) ? VK_RCONTROL : VK_LCONTROL;
		}
		else if ( key == VK_MENU )
		{
			key = ( info.win_lparam & lparam_mask ) ? VK_RMENU : VK_LMENU;
		}
		else if ( key == VK_SHIFT )
		{
			if ( pressed )
			{
				if ( GetKeyState( VK_LSHIFT ) & 0x8000 )
				{
					key = VK_LSHIFT;
				}
				else if ( GetKeyState( VK_RSHIFT ) & 0x8000 )
				{
					key = VK_RSHIFT;
				}
			}
			else
			{
				if ( pre_state->GetBool( XE::KeyCode::KeyLeftShift ) && !( GetKeyState( VK_LSHIFT ) & 0x8000 ) )
				{
					key = VK_LSHIFT;
				}
				else if ( pre_state->GetBool( XE::KeyCode::KeyRightShift ) && !( GetKeyState( VK_RSHIFT ) & 0x8000 ) )
				{
					key = VK_RSHIFT;
				}
			}
		}

		auto it = map.find( key );
		if ( it != map.end() )
		{
			XE::KeyCode code = it->second;
			// TODO: 
		}
	}
}
#elif PLATFORM_OS & OS_LINUX
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_MAC
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_IOS
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_HTML5
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_ANDROID
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_NINTENDO
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#elif PLATFORM_OS & OS_PLAY_STATION
void KeyboardInputHandleMessage( XE::InputState * cur_state, XE::InputState * pre_state, const XE::InputEventInfo & info )
{

}
#endif

IMPLEMENT_META( XE::KeyboardInput );

struct XE::KeyboardInput::Private
{
	XE::Disposable _Event;

	XE::InputState * _State;
	XE::InputState _State1, _State2;
	XE::Array< XE::InputEventInfo > _Infos;
};

XE::KeyboardInput::KeyboardInput()
	:_p( XE::New< Private >() )
{

}

XE::KeyboardInput::~KeyboardInput()
{
	XE::Delete( _p );
}

void XE::KeyboardInput::Startup()
{
	_p->_State = &_p->_State1;

	_p->_Event = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::EventService >()->RegisterListener( XE::EVENT_MOUSE, [this]( const XE::EventPtr & event )
		{
			_p->_Infos.push_back( event->parameter.Value< XE::InputEventInfo >() );
		} );
}

void XE::KeyboardInput::Update()
{
	auto prev = _p->_State; _p->_State = _p->_State == &_p->_State1 ? &_p->_State2 : &_p->_State1;

	for ( const auto & info : _p->_Infos )
	{
		KeyboardInputHandleMessage( _p->_State, prev, info );
	}

	prev->Clear();
	_p->_Infos.clear();
}

void XE::KeyboardInput::Clearup()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
	_p->_Event.Dispose();
}

void XE::KeyboardInput::ClearStatus()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
}

const XE::InputState & XE::KeyboardInput::GetState() const
{
	return *_p->_State;
}
