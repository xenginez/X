/*!
 * \file	Type.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__F5415E7B_FE49_440C_86FC_57B459E6B43E
#define TYPE_H__F5415E7B_FE49_440C_86FC_57B459E6B43E

#include "Declare.h"
#include "Math/Graph.hpp"
#include "Utils/Flags.hpp"
#include "Utils/Handle.hpp"
#include "Utils/RefCount.h"

BEG_XE_NAMESPACE

DECL_HANDLE( RenderResource );

enum class CameraType
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};
DECL_XE_ENUM( CameraType );

END_XE_NAMESPACE

#endif//TYPE_H__F5415E7B_FE49_440C_86FC_57B459E6B43E
