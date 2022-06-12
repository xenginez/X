/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__025662EC_638B_44E4_BFC3_C330938E4079
#define DECLARE_H__025662EC_638B_44E4_BFC3_C330938E4079

#include "Utils/Declare.h"

struct ImGuiContext;

BEG_XE_NAMESPACE

class Canvas;
class Widget;
class ImGuiImpl;
class GUIService;

static thread_local ImGuiContext * _ImGuiContext = nullptr;

END_XE_NAMESPACE

#endif//DECLARE_H__025662EC_638B_44E4_BFC3_C330938E4079
