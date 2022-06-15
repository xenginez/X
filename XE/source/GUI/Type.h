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
#include "Core/GraphicsService.h"

BEG_XE_NAMESPACE

DECL_PTR( Model );
DECL_PTR( Canvas );
DECL_PTR( Widget );
DECL_PTR( Layout );
DECL_PTR( ImGuiImpl );
DECL_PTR( LayoutItem );

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

END_XE_NAMESPACE

#endif//TYPE_H__90AFD9B0_70C7_4A6D_8903_53584B412E43
