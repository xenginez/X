/*!
 * \file	imconfig.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef IMCONFIG_H__C69A0346_AA03_492B_B19D_55EF0C654605
#define IMCONFIG_H__C69A0346_AA03_492B_B19D_55EF0C654605

#include "Math/Vec2.h"
#include "Math/Vec4.h"
#include "Math/Color.h"

#include "Declare.h"

#define IMGUI_API XE_API
#define IMGUI_DISABLE_DEMO_WINDOWS

#define ImDrawIdx XE::uint32
#define ImTextureID XE::uint64


#define IM_VEC2_CLASS_EXTRA \
template< typename T > constexpr ImVec2( const XE::Vec2<T> & f ) : x( f.x ), y( f.y ) {} \
template< typename T > operator XE::Vec2<T>() const { return XE::Vec2<T>( x, y ); }
#define IM_VEC4_CLASS_EXTRA \
template< typename T > constexpr ImVec4( const XE::Vec4<T> & f ) : x( f.x ), y( f.y ), z( f.z ), w( f.w ) {} \
template< typename T > operator XE::Vec4<T>() const { return XE::Vec4<T>( x, y, z, w ); }

static thread_local ImGuiContext * G_XE_ImGuiContext = nullptr;

#define GImGui G_XE_ImGuiContext

#endif//IMCONFIG_H__C69A0346_AA03_492B_B19D_55EF0C654605
