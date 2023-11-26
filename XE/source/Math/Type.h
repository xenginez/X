/*!
 * \file   Type.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __TYPE_H__98A93208_FE74_468C_A0B7_059AC2D5374C
#define __TYPE_H__98A93208_FE74_468C_A0B7_059AC2D5374C

#include "Reflect/Reflection.h"

#include "Declare.h"

BEG_XE_NAMESPACE

enum class SideType
{
	NOOP,
	BOTH,
	POSITIVE,
	NEGATIVE,
};
DECL_XE_ENUM( SideType );

enum class AxisType : XE::uint8
{
	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2,
	XY = X | Y,
	XZ = X | Z,
	YZ = Y | Z,
	XYZ = X | Y | Z,
};
DECL_XE_ENUM( AxisType );

enum class RotationType
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};
DECL_XE_ENUM( RotationType );

END_XE_NAMESPACE

#endif // __TYPE_H__98A93208_FE74_468C_A0B7_059AC2D5374C
