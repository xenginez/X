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
	static XE::Map< XE::uint32, XE::KeyCode > map =
	{
		{ XK_Escape, XE::KeyCode::KeyEscape },
		{ XK_F1, XE::KeyCode::KeyF1 },
		{ XK_F2, XE::KeyCode::KeyF2 },
		{ XK_F3, XE::KeyCode::KeyF3 },
		{ XK_F4, XE::KeyCode::KeyF4 },
		{ XK_F5, XE::KeyCode::KeyF5 },
		{ XK_F6, XE::KeyCode::KeyF6 },
		{ XK_F7, XE::KeyCode::KeyF7 },
		{ XK_F8, XE::KeyCode::KeyF8 },
		{ XK_F9, XE::KeyCode::KeyF9 },
		{ XK_F10, XE::KeyCode::KeyF10 },
		{ XK_F11, XE::KeyCode::KeyF11 },
		{ XK_F12, XE::KeyCode::KeyF12 },
		{ XK_Print, XE::KeyCode::KeyPrint },
		{ XK_Scroll_Lock, XE::KeyCode::KeyScrollLock },
		{ XK_Pause, XE::KeyCode::KeyBreak },
		{ XK_space, XE::KeyCode::KeySpace },
//		{ XK_apostrophe, XE::KeyCode::KeyApostrophe },
		{ XK_comma, XE::KeyCode::KeyComma },
		{ XK_minus, XE::KeyCode::KeyMinus },
		{ XK_period, XE::KeyCode::KeyPeriod },
		{ XK_slash, XE::KeyCode::KeySlash },
		{ XK_0, XE::KeyCode::Key0 },
		{ XK_1, XE::KeyCode::Key1 },
		{ XK_2, XE::KeyCode::Key2 },
		{ XK_3, XE::KeyCode::Key3 },
		{ XK_4, XE::KeyCode::Key4 },
		{ XK_5, XE::KeyCode::Key5 },
		{ XK_6, XE::KeyCode::Key6 },
		{ XK_7, XE::KeyCode::Key7 },
		{ XK_8, XE::KeyCode::Key8 },
		{ XK_9, XE::KeyCode::Key9 },
		{ XK_semicolon, XE::KeyCode::KeySemicolon },
		{ XK_less, XE::KeyCode::KeyLess },
//		{ XK_equal, XE::KeyCode::KeyEqual },
		{ XK_a, XE::KeyCode::KeyA },
		{ XK_b, XE::KeyCode::KeyB },
		{ XK_c, XE::KeyCode::KeyC },
		{ XK_d, XE::KeyCode::KeyD },
		{ XK_e, XE::KeyCode::KeyE },
		{ XK_f, XE::KeyCode::KeyF },
		{ XK_g, XE::KeyCode::KeyG },
		{ XK_h, XE::KeyCode::KeyH },
		{ XK_i, XE::KeyCode::KeyI },
		{ XK_j, XE::KeyCode::KeyJ },
		{ XK_k, XE::KeyCode::KeyK },
		{ XK_l, XE::KeyCode::KeyL },
		{ XK_m, XE::KeyCode::KeyM },
		{ XK_n, XE::KeyCode::KeyN },
		{ XK_o, XE::KeyCode::KeyO },
		{ XK_p, XE::KeyCode::KeyP },
		{ XK_q, XE::KeyCode::KeyQ },
		{ XK_r, XE::KeyCode::KeyR },
		{ XK_s, XE::KeyCode::KeyS },
		{ XK_t, XE::KeyCode::KeyT },
		{ XK_u, XE::KeyCode::KeyU },
		{ XK_v, XE::KeyCode::KeyV },
		{ XK_w, XE::KeyCode::KeyW },
		{ XK_x, XE::KeyCode::KeyX },
		{ XK_y, XE::KeyCode::KeyY },
		{ XK_z, XE::KeyCode::KeyZ },
		{ XK_bracketleft, XE::KeyCode::KeyLeftBracket },
		{ XK_backslash, XE::KeyCode::KeyBackslash },
		{ XK_bracketright, XE::KeyCode::KeyRightBracket },
//		{ XK_grave, XE::KeyCode::KeyGrave },
		{ XK_Left, XE::KeyCode::KeyLeftArrow },
		{ XK_Right, XE::KeyCode::KeyRightArrow },
		{ XK_Up, XE::KeyCode::KeyUpArrow },
		{ XK_Down, XE::KeyCode::KeyDownArrow },
		{ XK_Insert, XE::KeyCode::KeyInsert },
		{ XK_Home, XE::KeyCode::KeyHome },
		{ XK_Delete, XE::KeyCode::KeyDelete },
		{ XK_End, XE::KeyCode::KeyEnd },
		{ XK_Page_Up, XE::KeyCode::KeyPageUp },
		{ XK_Page_Down, XE::KeyCode::KeyPageDown },
		{ XK_Num_Lock, XE::KeyCode::KeyNumlock },
		{ XK_KP_Divide, XE::KeyCode::KeypadDivide },
		{ XK_KP_Multiply, XE::KeyCode::KeypadMultiply },
//		{ XK_KP_Subtract, XE::KeyCode::KeyKpSubtract },
		{ XK_KP_Add, XE::KeyCode::KeypadPlus },
		{ XK_KP_Enter, XE::KeyCode::KeypadEnter },
		{ XK_KP_Insert, XE::KeyCode::KeyInsert },
		{ XK_KP_End, XE::KeyCode::KeyEnd },
		{ XK_KP_Down, XE::KeyCode::KeyDownArrow },
		{ XK_KP_Page_Down, XE::KeyCode::KeyPageDown },
		{ XK_KP_Left, XE::KeyCode::KeyLeftArrow },
//		{ XK_KP_Begin, XE::KeyCode::KeyKpBegin },
		{ XK_KP_Right, XE::KeyCode::KeyRightArrow },
		{ XK_KP_Home, XE::KeyCode::KeyHome },
		{ XK_KP_Up, XE::KeyCode::KeyUpArrow },
		{ XK_KP_Page_Up, XE::KeyCode::KeyPageUp },
		{ XK_KP_Delete, XE::KeyCode::KeyDelete },
		{ XK_BackSpace, XE::KeyCode::KeyBackspace },
		{ XK_Tab, XE::KeyCode::KeyTab },
		{ XK_Return, XE::KeyCode::KeyReturn },
		{ XK_Caps_Lock, XE::KeyCode::KeyCapsLock },
		{ XK_Shift_L, XE::KeyCode::KeyLeftShift },
		{ XK_Control_L, XE::KeyCode::KeyLeftControl },
//		{ XK_Super_L, XE::KeyCode::KeySuperL },
		{ XK_Alt_L, XE::KeyCode::KeyLeftAlt },
		{ XK_Alt_R, XE::KeyCode::KeyRightAlt },
//		{ XK_Super_R, XE::KeyCode::KeySuperR },
		{ XK_Menu, XE::KeyCode::KeyMenu },
		{ XK_Control_R, XE::KeyCode::KeyRightControl },
		{ XK_Shift_R, XE::KeyCode::KeyRightShift },
// 		{ XK_dead_circumflex, XE::KeyCode::KeyCircumflex },
// 		{ XK_ssharp, XE::KeyCode::KeySsharp },
// 		{ XK_dead_acute, XE::KeyCode::KeyAcute },
// 		{ XK_ISO_Level3_Shift, XE::KeyCode::KeyAltGr },
		{ XK_plus, XE::KeyCode::KeyPlus },
// 		{ XK_numbersign, XE::KeyCode::KeyNumbersign },
// 		{ XK_udiaeresis, XE::KeyCode::KeyUdiaeresis },
// 		{ XK_adiaeresis, XE::KeyCode::KeyAdiaeresis },
// 		{ XK_odiaeresis, XE::KeyCode::KeyOdiaeresis },
//		{ XK_section, XE::KeyCode::KeySection },
//		{ XK_aring, XE::KeyCode::KeyAring },
//		{ XK_dead_diaeresis, XE::KeyCode::KeyDiaeresis },
//		{ XK_twosuperior, XE::KeyCode::KeyTwosuperior },
//		{ XK_parenright, XE::KeyCode::KeyRightParenthesis },
		{ XK_dollar, XE::KeyCode::KeyDollar },
//		{ XK_ugrave, XE::KeyCode::KeyUgrave },
		{ XK_asterisk, XE::KeyCode::KeyAsterisk },
		{ XK_colon, XE::KeyCode::KeyColon },
//		{ XK_exclam, XE::KeyCode::KeyExclam },
	};

	/*
	if ( event.type == KeyPress || event.type == KeyRelease )
	{
		XKeyEvent & keyEvent = event.xkey;
		KeySym keySym = XkbKeycodeToKeysym( keyEvent.display, keyEvent.keycode, 0, 0 );
		const bool pressed = event.type == KeyPress;

		// Handle key repeat
		if ( event.type == KeyRelease && XPending( keyEvent.display ) )
		{
			XEvent nextEvent;
			XPeekEvent( keyEvent.display, &nextEvent );
			if ( nextEvent.type == KeyPress
				 && nextEvent.xkey.keycode == event.xkey.keycode
				 && nextEvent.xkey.time == event.xkey.time )
			{
				XNextEvent( keyEvent.display, &nextEvent );
				return;
			}
		}

		if ( dialect_.count( keySym ) )
		{
			const DeviceButtonId buttonId = dialect_[keySym];
			HandleButton( device_, nextState_, delta_, buttonId, pressed );
		}

		if ( textInputEnabled_ )
		{
			char buf[32];
			int len = XLookupString( &keyEvent, buf, 32, 0, 0 );
			if ( len == 1 )
			{
				textBuffer_.Put( buf[0] );
			}
		}
	}
	*/
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
