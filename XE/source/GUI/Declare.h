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

struct ImFont;
struct ImGuiStyle;
struct ImFontAtlas;
struct ImGuiContext;

BEG_XE_NAMESPACE

class Font;
class Model;
class Canvas;
class Widget;
class Layout;
class ImGuiImpl;
class LayoutItem;
class GUIService;
template< typename ... T > class Action;

END_XE_NAMESPACE

#endif//DECLARE_H__025662EC_638B_44E4_BFC3_C330938E4079
