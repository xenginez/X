/*!
 * \file	Type.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__90AFD9B0_70C7_4A6D_8903_53584B412E43
#define TYPE_H__90AFD9B0_70C7_4A6D_8903_53584B412E43

#include "Declare.h"

#include "Math/Rect.h"
#include "Utils/Flags.hpp"
#include "Reflect/Object.h"

BEG_XE_NAMESPACE

DECL_PTR( Canvas );
DECL_PTR( Widget );
DECL_PTR( Layout );
DECL_PTR( Controller );
DECL_PTR( LayoutItem );

enum class Alignment
{
	LEFT = 1 << 0,
	RIGHT = 1 << 1,
	HCENTER = 1 << 2,
	TOP = 1 << 3,
	BOTTOM = 1 << 4,
	VCENTER = 1 << 5,
	CENTER = HCENTER | VCENTER,
};

enum class FlexWrap
{
	NO_WRAP,
	WRAP,
	WRAP_REVERSE,
} ;

enum class FlexAlign
{
	AUTO,
	STRETCH,
	CENTER,
	START,
	END,
	SPACE_BETWEEN,
	SPACE_AROUND,
	SPACE_EVENLY,
};

enum class FlexPosition
{
	RELATIVE,
	ABSOLUTE,
} ;

enum class FlexDirection
{
	ROW,
	ROW_REVERSE,
	COLUMN,
	COLUMN_REVERSE,
};

enum class TextEditFlag
{
	NONE = 0,
	CHARS_DECIMAL = 1 << 0,   // Allow 0123456789.+-*/
	CHARS_HEXADECIMAL = 1 << 1,   // Allow 0123456789ABCDEFabcdef
	CHARS_UPPERCASE = 1 << 2,   // Turn a..z into A..Z
	CHARS_NO_BLANK = 1 << 3,   // Filter out spaces, tabs
	AUTO_SELECT_ALL = 1 << 4,   // Select entire text when first taking mouse focus
	ALLOW_TAB_INPUT = 1 << 10,  // Pressing TAB input a '\t' character into the text field
	CTRL_ENTER_FOR_NEW_LINE = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
	NO_HORIZONTAL_SCROLL = 1 << 12,  // Disable following the cursor horizontally
	ALWAYS_OVERWRITE = 1 << 13,  // Overwrite mode
	READ_ONLY = 1 << 14,  // Read-only mode
	PASSWORD = 1 << 15,  // Password mode, display all characters as '*'
	NO_UNDO_REDO = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
	CHARS_SCIENTIFIC = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
};
DECL_FLAGS( TextEditFlag, TextEditFlags );

END_XE_NAMESPACE

#endif//TYPE_H__90AFD9B0_70C7_4A6D_8903_53584B412E43
