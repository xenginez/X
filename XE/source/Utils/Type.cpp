#include"Type.h"

#include "Flags.hpp"
#include "Handle.hpp"

BEG_META( XE::Language )
type->Value( "ENGLISH", XE::Language::ENGLISH );
type->Value( "CHINESESIMPLIFIED", XE::Language::CHINESESIMPLIFIED );
type->Value( "CHINESETRADITIONAL", XE::Language::CHINESETRADITIONAL );
type->Value( "AFRIKAANS", XE::Language::AFRIKAANS );
type->Value( "ARABIC", XE::Language::ARABIC );
type->Value( "BASQUE", XE::Language::BASQUE );
type->Value( "BELARUSIAN", XE::Language::BELARUSIAN );
type->Value( "BULGARIAN", XE::Language::BULGARIAN );
type->Value( "CATALAN", XE::Language::CATALAN );
type->Value( "CZECH", XE::Language::CZECH );
type->Value( "DANISH", XE::Language::DANISH );
type->Value( "DUTCH", XE::Language::DUTCH );
type->Value( "ESTONIAN", XE::Language::ESTONIAN );
type->Value( "FAROESE", XE::Language::FAROESE );
type->Value( "FINNISH", XE::Language::FINNISH );
type->Value( "FRENCH", XE::Language::FRENCH );
type->Value( "GERMAN", XE::Language::GERMAN );
type->Value( "GREEK", XE::Language::GREEK );
type->Value( "HEBREW", XE::Language::HEBREW );
type->Value( "ICELANDIC", XE::Language::ICELANDIC );
type->Value( "INDONESIAN", XE::Language::INDONESIAN );
type->Value( "ITALIAN", XE::Language::ITALIAN );
type->Value( "JAPANESE", XE::Language::JAPANESE );
type->Value( "KOREAN", XE::Language::KOREAN );
type->Value( "LATVIAN", XE::Language::LATVIAN );
type->Value( "LITHUANIAN", XE::Language::LITHUANIAN );
type->Value( "NORWEGIAN", XE::Language::NORWEGIAN );
type->Value( "POLISH", XE::Language::POLISH );
type->Value( "PORTUGUESE", XE::Language::PORTUGUESE );
type->Value( "ROMANIAN", XE::Language::ROMANIAN );
type->Value( "RUSSIAN", XE::Language::RUSSIAN );
type->Value( "SERBOCROATIAN", XE::Language::SERBOCROATIAN );
type->Value( "SLOVAK", XE::Language::SLOVAK );
type->Value( "SLOVENIAN", XE::Language::SLOVENIAN );
type->Value( "SPANISH", XE::Language::SPANISH );
type->Value( "SWEDISH", XE::Language::SWEDISH );
type->Value( "THAI", XE::Language::THAI );
type->Value( "TURKISH", XE::Language::TURKISH );
type->Value( "UKRAINIAN", XE::Language::UKRAINIAN );
type->Value( "VIETNAMESE", XE::Language::VIETNAMESE );
type->Value( "HUNGARIAN", XE::Language::HUNGARIAN );
type->Value( "UNKNOWN", XE::Language::UNKNOWN );
END_META()

BEG_META( XE::WindowEventType )
type->Value( "CREATE", XE::WindowEventType::CREATE );
type->Value( "DESTROY", XE::WindowEventType::DESTROY );
type->Value( "MOVE", XE::WindowEventType::MOVE );
type->Value( "SIZE", XE::WindowEventType::SIZE );
type->Value( "ACTIVATE", XE::WindowEventType::ACTIVATE );
type->Value( "SETFOCUS", XE::WindowEventType::SETFOCUS );
type->Value( "KILLFOCUS", XE::WindowEventType::KILLFOCUS );
type->Value( "ENABLE", XE::WindowEventType::ENABLE );
type->Value( "PAINT", XE::WindowEventType::PAINT );
type->Value( "CLOSE", XE::WindowEventType::CLOSE );
type->Value( "QUERYENDSESSION", XE::WindowEventType::QUERYENDSESSION );
type->Value( "QUIT", XE::WindowEventType::QUIT );
type->Value( "SHOWWINDOW", XE::WindowEventType::SHOWWINDOW );
type->Value( "ACTIVATEAPP", XE::WindowEventType::ACTIVATEAPP );
type->Value( "SETCURSOR", XE::WindowEventType::SETCURSOR );
type->Value( "MOUSEACTIVATE", XE::WindowEventType::MOUSEACTIVATE );
type->Value( "GETMINMAXINFO", XE::WindowEventType::GETMINMAXINFO );
type->Value( "NEXTDLGCTL", XE::WindowEventType::NEXTDLGCTL );
type->Value( "COMPACTING", XE::WindowEventType::COMPACTING );
type->Value( "WINDOWPOSCHANGING", XE::WindowEventType::WINDOWPOSCHANGING );
type->Value( "WINDOWPOSCHANGED", XE::WindowEventType::WINDOWPOSCHANGED );
type->Value( "DISPLAYCHANGE", XE::WindowEventType::DISPLAYCHANGE );
type->Value( "GETICON", XE::WindowEventType::GETICON );
type->Value( "SETICON", XE::WindowEventType::SETICON );
type->Value( "KEYDOWN", XE::WindowEventType::KEYDOWN );
type->Value( "KEYUP", XE::WindowEventType::KEYUP );
type->Value( "CHAR", XE::WindowEventType::CHAR );
type->Value( "DEADCHAR", XE::WindowEventType::DEADCHAR );
type->Value( "SYSKEYDOWN", XE::WindowEventType::SYSKEYDOWN );
type->Value( "SYSKEYUP", XE::WindowEventType::SYSKEYUP );
type->Value( "SYSCHAR", XE::WindowEventType::SYSCHAR );
type->Value( "SYSDEADCHAR", XE::WindowEventType::SYSDEADCHAR );
type->Value( "MOUSEMOVE", XE::WindowEventType::MOUSEMOVE );
type->Value( "LBUTTONDOWN", XE::WindowEventType::LBUTTONDOWN );
type->Value( "LBUTTONUP", XE::WindowEventType::LBUTTONUP );
type->Value( "LBUTTONDBLCLK", XE::WindowEventType::LBUTTONDBLCLK );
type->Value( "RBUTTONDOWN", XE::WindowEventType::RBUTTONDOWN );
type->Value( "RBUTTONUP", XE::WindowEventType::RBUTTONUP );
type->Value( "RBUTTONDBLCLK", XE::WindowEventType::RBUTTONDBLCLK );
type->Value( "MBUTTONDOWN", XE::WindowEventType::MBUTTONDOWN );
type->Value( "MBUTTONUP", XE::WindowEventType::MBUTTONUP );
type->Value( "MBUTTONDBLCLK", XE::WindowEventType::MBUTTONDBLCLK );
type->Value( "MOUSEWHEEL", XE::WindowEventType::MOUSEWHEEL );
type->Value( "SIZING", XE::WindowEventType::SIZING );
type->Value( "CAPTURECHANGED", XE::WindowEventType::CAPTURECHANGED );
type->Value( "MOVING", XE::WindowEventType::MOVING );
type->Value( "POWERBROADCAST", XE::WindowEventType::POWERBROADCAST );
type->Value( "DEVICECHANGE", XE::WindowEventType::DEVICECHANGE );
END_META()
