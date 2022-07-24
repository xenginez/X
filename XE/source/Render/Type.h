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
#include "Math/Rect.h"
#include "Math/AABB.h"
#include "Math/Graph.hpp"
#include "Utils/Flags.hpp"
#include "Utils/Handle.hpp"
#include "Utils/RefCount.h"

BEG_XE_NAMESPACE

DECL_HANDLE( RenderResource );

enum class CameraType : XE::uint32
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};
DECL_XE_ENUM( CameraType );

enum class LightType : XE::uint32
{
	SPOT,
	POINT,
	DIRECTIONAL,
	AMBIENT_BOX,
	AMBIENT_SPHERE,
};
DECL_XE_ENUM( LightType );

struct XE_API LightData
{
	bool IsDynamic = false;
	XE::uint64 Mask = 0;

	XE::Color Color = { 255, 255, 255, 255 };
	XE::float32 Intensity = 1.0f;
	XE::float32 Constant = 0.0f;
	XE::float32 Linear = 0.0f;
	XE::float32 Quadratic = 1.0f;
	XE::float32 Cutoff = 12.0f;
	XE::float32 OuterCutoff = 15.0f;
	XE::LightType Type = XE::LightType::POINT;

	XE::Delegate< void( XE::RenderExecutor & ) > RenderCallback;
};

struct XE_API RenderData
{
	bool IsDynamic = false;
	XE::uint64 Mask = 0;

	XE::Delegate< void( XE::RenderExecutor & ) > RenderCallback;
};

struct XE_API CameraData
{
	XE::float32 FOV = 60.0f;
	XE::float32 Near = 0.1f;
	XE::float32 Far = 1000.0f;
	XE::uint32 Depth = 1;
	XE::uint64 Mask = 0;
	XE::Rectf Viewport = { 0, 0, 1, 1 };
	XE::CameraType Type = XE::CameraType::ORTHOGRAPHIC;

	XE::Delegate< void( XE::RenderExecutor & ) > RenderCallback;
};

template<> class XE::MakeAABB< XE::LightData * >
{
public:
	XE::AABB operator()( XE::LightData * val )
	{
		return {};
	}
};

template<> class XE::MakeAABB< XE::RenderData * >
{
public:
	XE::AABB operator()( XE::RenderData * val )
	{
		return {};
	}
};

END_XE_NAMESPACE

#endif//TYPE_H__F5415E7B_FE49_440C_86FC_57B459E6B43E
