/*!
 * \file	Type.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__0C77630F_7738_4E3F_98F9_BF858383FB30
#define TYPE_H__0C77630F_7738_4E3F_98F9_BF858383FB30

#include "Declare.h"
#include "Utils/Event.h"

BEG_XE_NAMESPACE

DECL_PTR( Input );
DECL_PTR( InputStateMap );
DECL_PTR( InputStateItem );

enum class KeyCode : XE::uint32
{
	KeyBackspace,
	KeyDelete,
	KeyTab,
	KeyClear,
	KeyReturn,
	KeyPause,
	KeyEscape,
	KeySpace,
	Key0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeypadPeriod,
	KeypadDivide,
	KeypadMultiply,
	KeypadMinus,
	KeypadPlus,
	KeypadEnter,
	KeypadEquals,
	KeyUpArrow,
	KeyDownArrow,
	KeyRightArrow,
	KeyLeftArrow,
	KeyInsert,
	KeyHome,
	KeyEnd,
	KeyPageUp,
	KeyPageDown,
	KeyF1,
	KeyF2,
	KeyF3,
	KeyF4,
	KeyF5,
	KeyF6,
	KeyF7,
	KeyF8,
	KeyF9,
	KeyF10,
	KeyF11,
	KeyF12,
	KeyF13,
	KeyF14,
	KeyF15,
	KeyExclaim,
	KeyDoubleQuote,
	KeyHash,
	KeyDollar,
	KeyAmpersand,
	KeyQuote,
	KeyLeftParen,
	KeyRightParen,
	KeyAsterisk,
	KeyPlus,
	KeyComma,
	KeyMinus,
	KeyPeriod,
	KeySlash,
	KeyColon,
	KeySemicolon,
	KeyLess,
	KeyEquals,
	KeyGreater,
	KeyQuestion,
	KeyAt,
	KeyLeftBracket,
	KeyBackslash,
	KeyRightBracket,
	KeyCaret,
	KeyUnderscore,
	KeyBackQuote,
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,
	KeyNumlock,
	KeyCapsLock,
	KeyScrollLock,
	KeyRightShift,
	KeyLeftShift,
	KeyRightControl,
	KeyLeftControl,
	KeyRightAlt,
	KeyLeftAlt,
	KeyLeftCommand,
	KeyRightCommand,
	KeyAltGr,
	KeyHelp,
	KeyPrint,
	KeySysReq,
	KeyBreak,
	KeyMenu,
};
DECL_XE_ENUM( KeyCode );
enum class PadCode : XE::uint32
{
	Pad0ButtonLeftStickX,
	Pad0ButtonLeftStickY,
	Pad0ButtonRightStickX,
	Pad0ButtonRightStickY,
	Pad0ButtonAxis4,
	Pad0ButtonAxis5,
	Pad0ButtonAxis6,
	Pad0ButtonAxis7,
	Pad0ButtonAxis8,
	Pad0ButtonAxis9,
	Pad0ButtonAxis10,
	Pad0ButtonAxis11,
	Pad0ButtonAxis12,
	Pad0ButtonAxis13,
	Pad0ButtonAxis14,
	Pad0ButtonAxis15,
	Pad0ButtonAxis16,
	Pad0ButtonAxis17,
	Pad0ButtonAxis18,
	Pad0ButtonAxis19,
	Pad0ButtonAxis20,
	Pad0ButtonAxis21,
	Pad0ButtonAxis22,
	Pad0ButtonAxis23,
	Pad0ButtonAxis24,
	Pad0ButtonAxis25,
	Pad0ButtonAxis26,
	Pad0ButtonAxis27,
	Pad0ButtonAxis28,
	Pad0ButtonAxis29,
	Pad0ButtonAxis30,
	Pad0ButtonAxis31,
	Pad0ButtonAccelerationX,
	Pad0ButtonAccelerationY,
	Pad0ButtonAccelerationZ,
	Pad0ButtonGravityX,
	Pad0ButtonGravityY,
	Pad0ButtonGravityZ,
	Pad0ButtonGyroscopeX,
	Pad0ButtonGyroscopeY,
	Pad0ButtonGyroscopeZ,
	Pad0ButtonMagneticFieldX,
	Pad0ButtonMagneticFieldY,
	Pad0ButtonMagneticFieldZ,
	Pad0ButtonStart,
	Pad0ButtonSelect,
	Pad0ButtonLeft,
	Pad0ButtonRight,
	Pad0ButtonUp,
	Pad0ButtonDown,
	Pad0ButtonA,
	Pad0ButtonB,
	Pad0ButtonX,
	Pad0ButtonY,
	Pad0ButtonL1,
	Pad0ButtonR1,
	Pad0ButtonL2,
	Pad0ButtonR2,
	Pad0ButtonL3,
	Pad0ButtonR3,
	Pad0ButtonHome,
	Pad0Button17,
	Pad0Button18,
	Pad0Button19,
	Pad0Button20,
	Pad0Button21,
	Pad0Button22,
	Pad0Button23,
	Pad0Button24,
	Pad0Button25,
	Pad0Button26,
	Pad0Button27,
	Pad0Button28,
	Pad0Button29,
	Pad0Button30,
	Pad0Button31,


	Pad1ButtonLeftStickX,
	Pad1ButtonLeftStickY,
	Pad1ButtonRightStickX,
	Pad1ButtonRightStickY,
	Pad1ButtonAxis4,
	Pad1ButtonAxis5,
	Pad1ButtonAxis6,
	Pad1ButtonAxis7,
	Pad1ButtonAxis8,
	Pad1ButtonAxis9,
	Pad1ButtonAxis10,
	Pad1ButtonAxis11,
	Pad1ButtonAxis12,
	Pad1ButtonAxis13,
	Pad1ButtonAxis14,
	Pad1ButtonAxis15,
	Pad1ButtonAxis16,
	Pad1ButtonAxis17,
	Pad1ButtonAxis18,
	Pad1ButtonAxis19,
	Pad1ButtonAxis20,
	Pad1ButtonAxis21,
	Pad1ButtonAxis22,
	Pad1ButtonAxis23,
	Pad1ButtonAxis24,
	Pad1ButtonAxis25,
	Pad1ButtonAxis26,
	Pad1ButtonAxis27,
	Pad1ButtonAxis28,
	Pad1ButtonAxis29,
	Pad1ButtonAxis30,
	Pad1ButtonAxis31,
	Pad1ButtonAccelerationX,
	Pad1ButtonAccelerationY,
	Pad1ButtonAccelerationZ,
	Pad1ButtonGravityX,
	Pad1ButtonGravityY,
	Pad1ButtonGravityZ,
	Pad1ButtonGyroscopeX,
	Pad1ButtonGyroscopeY,
	Pad1ButtonGyroscopeZ,
	Pad1ButtonMagneticFieldX,
	Pad1ButtonMagneticFieldY,
	Pad1ButtonMagneticFieldZ,
	Pad1ButtonStart,
	Pad1ButtonSelect,
	Pad1ButtonLeft,
	Pad1ButtonRight,
	Pad1ButtonUp,
	Pad1ButtonDown,
	Pad1ButtonA,
	Pad1ButtonB,
	Pad1ButtonX,
	Pad1ButtonY,
	Pad1ButtonL1,
	Pad1ButtonR1,
	Pad1ButtonL2,
	Pad1ButtonR2,
	Pad1ButtonL3,
	Pad1ButtonR3,
	Pad1ButtonHome,
	Pad1Button17,
	Pad1Button18,
	Pad1Button19,
	Pad1Button20,
	Pad1Button21,
	Pad1Button22,
	Pad1Button23,
	Pad1Button24,
	Pad1Button25,
	Pad1Button26,
	Pad1Button27,
	Pad1Button28,
	Pad1Button29,
	Pad1Button30,
	Pad1Button31,


	Pad2ButtonLeftStickX,
	Pad2ButtonLeftStickY,
	Pad2ButtonRightStickX,
	Pad2ButtonRightStickY,
	Pad2ButtonAxis4,
	Pad2ButtonAxis5,
	Pad2ButtonAxis6,
	Pad2ButtonAxis7,
	Pad2ButtonAxis8,
	Pad2ButtonAxis9,
	Pad2ButtonAxis10,
	Pad2ButtonAxis11,
	Pad2ButtonAxis12,
	Pad2ButtonAxis13,
	Pad2ButtonAxis14,
	Pad2ButtonAxis15,
	Pad2ButtonAxis16,
	Pad2ButtonAxis17,
	Pad2ButtonAxis18,
	Pad2ButtonAxis19,
	Pad2ButtonAxis20,
	Pad2ButtonAxis21,
	Pad2ButtonAxis22,
	Pad2ButtonAxis23,
	Pad2ButtonAxis24,
	Pad2ButtonAxis25,
	Pad2ButtonAxis26,
	Pad2ButtonAxis27,
	Pad2ButtonAxis28,
	Pad2ButtonAxis29,
	Pad2ButtonAxis30,
	Pad2ButtonAxis31,
	Pad2ButtonAccelerationX,
	Pad2ButtonAccelerationY,
	Pad2ButtonAccelerationZ,
	Pad2ButtonGravityX,
	Pad2ButtonGravityY,
	Pad2ButtonGravityZ,
	Pad2ButtonGyroscopeX,
	Pad2ButtonGyroscopeY,
	Pad2ButtonGyroscopeZ,
	Pad2ButtonMagneticFieldX,
	Pad2ButtonMagneticFieldY,
	Pad2ButtonMagneticFieldZ,
	Pad2ButtonStart,
	Pad2ButtonSelect,
	Pad2ButtonLeft,
	Pad2ButtonRight,
	Pad2ButtonUp,
	Pad2ButtonDown,
	Pad2ButtonA,
	Pad2ButtonB,
	Pad2ButtonX,
	Pad2ButtonY,
	Pad2ButtonL1,
	Pad2ButtonR1,
	Pad2ButtonL2,
	Pad2ButtonR2,
	Pad2ButtonL3,
	Pad2ButtonR3,
	Pad2ButtonHome,
	Pad2Button17,
	Pad2Button18,
	Pad2Button19,
	Pad2Button20,
	Pad2Button21,
	Pad2Button22,
	Pad2Button23,
	Pad2Button24,
	Pad2Button25,
	Pad2Button26,
	Pad2Button27,
	Pad2Button28,
	Pad2Button29,
	Pad2Button30,
	Pad2Button31,


	Pad3ButtonLeftStickX,
	Pad3ButtonLeftStickY,
	Pad3ButtonRightStickX,
	Pad3ButtonRightStickY,
	Pad3ButtonAxis4,
	Pad3ButtonAxis5,
	Pad3ButtonAxis6,
	Pad3ButtonAxis7,
	Pad3ButtonAxis8,
	Pad3ButtonAxis9,
	Pad3ButtonAxis10,
	Pad3ButtonAxis11,
	Pad3ButtonAxis12,
	Pad3ButtonAxis13,
	Pad3ButtonAxis14,
	Pad3ButtonAxis15,
	Pad3ButtonAxis16,
	Pad3ButtonAxis17,
	Pad3ButtonAxis18,
	Pad3ButtonAxis19,
	Pad3ButtonAxis20,
	Pad3ButtonAxis21,
	Pad3ButtonAxis22,
	Pad3ButtonAxis23,
	Pad3ButtonAxis24,
	Pad3ButtonAxis25,
	Pad3ButtonAxis26,
	Pad3ButtonAxis27,
	Pad3ButtonAxis28,
	Pad3ButtonAxis29,
	Pad3ButtonAxis30,
	Pad3ButtonAxis31,
	Pad3ButtonAccelerationX,
	Pad3ButtonAccelerationY,
	Pad3ButtonAccelerationZ,
	Pad3ButtonGravityX,
	Pad3ButtonGravityY,
	Pad3ButtonGravityZ,
	Pad3ButtonGyroscopeX,
	Pad3ButtonGyroscopeY,
	Pad3ButtonGyroscopeZ,
	Pad3ButtonMagneticFieldX,
	Pad3ButtonMagneticFieldY,
	Pad3ButtonMagneticFieldZ,
	Pad3ButtonStart,
	Pad3ButtonSelect,
	Pad3ButtonLeft,
	Pad3ButtonRight,
	Pad3ButtonUp,
	Pad3ButtonDown,
	Pad3ButtonA,
	Pad3ButtonB,
	Pad3ButtonX,
	Pad3ButtonY,
	Pad3ButtonL1,
	Pad3ButtonR1,
	Pad3ButtonL2,
	Pad3ButtonR2,
	Pad3ButtonL3,
	Pad3ButtonR3,
	Pad3ButtonHome,
	Pad3Button17,
	Pad3Button18,
	Pad3Button19,
	Pad3Button20,
	Pad3Button21,
	Pad3Button22,
	Pad3Button23,
	Pad3Button24,
	Pad3Button25,
	Pad3Button26,
	Pad3Button27,
	Pad3Button28,
	Pad3Button29,
	Pad3Button30,
	Pad3Button31,
};
DECL_XE_ENUM( PadCode );
enum class MouseCode : XE::uint32
{
	MousePos,
	MouseAxisX,
	MouseAxisY,
	MouseWheel,
	MouseHWheel,
	MouseLeft,
	MouseMiddle,
	MouseRight,
	MouseButton4,
	MouseButton5,
	MouseButton6,
	MouseButton7,
	MouseButton8,
	MouseButton9,
};
DECL_XE_ENUM( MouseCode );
enum class TouchCode : XE::uint32
{
	Touch0Down,
	Touch0X,
	Touch0Y,
	Touch0Z,
	Touch1Down,
	Touch1X,
	Touch1Y,
	Touch1Z,
	Touch2Down,
	Touch2X,
	Touch2Y,
	Touch2Z,
	Touch3Down,
	Touch3X,
	Touch3Y,
	Touch3Z,
	Touch4Down,
	Touch4X,
	Touch4Y,
	Touch4Z,
	Touch5Down,
	Touch5X,
	Touch5Y,
	Touch5Z,
	Touch6Down,
	Touch6X,
	Touch6Y,
	Touch6Z,
	Touch7Down,
	Touch7X,
	Touch7Y,
	Touch7Z,
};
DECL_XE_ENUM( TouchCode );
enum class InputStateItemTimeoutType
{
	TIME,
	FRAME,
	UNSCALE_TIME,
};
DECL_XE_ENUM( InputStateItemTimeoutType );

struct XE_API InputEventInfo
{
	union
	{
		struct
		{
			XE::uint64 win_lparam;
			XE::uint64 win_wparam;
			XE::uint32 win_message;
		};

		struct
		{
			XE::uint32 linux_x;
			XE::uint32 linux_y;
			XE::uint32 linux_dx;
			XE::uint32 linux_dy;
			XE::uint32 linux_type;
			XE::uint32 linux_button;
		};
	};

	XE::Array< XE::uint8 > raw_data;
};
DECL_XE_CLASS( InputEventInfo );

DECL_EVENT( HID, "hid input", XE::InputEventInfo );
DECL_EVENT( MOUSE, "mouse input", XE::InputEventInfo );
DECL_EVENT( KEYBOARD, "keyboard input", XE::InputEventInfo );

END_XE_NAMESPACE

#endif//TYPE_H__0C77630F_7738_4E3F_98F9_BF858383FB30
