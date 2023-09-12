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
#include "Math/Color.h"
#include "Math/Graph.hpp"
#include "Utils/Flags.hpp"
#include "Utils/Handle.hpp"
#include "Utils/RefCount.h"

BEG_XE_NAMESPACE

DECL_HANDLE( RenderResource );

enum class LightType : XE::uint32
{
	SPOT,
	POINT,
	DIRECTIONAL,
	AMBIENT_BOX,
	AMBIENT_SPHERE,
};
DECL_XE_ENUM( LightType );

enum class CameraType : XE::uint32
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};
DECL_XE_ENUM( CameraType );

enum class RenderQueueType : XE::uint32
{
	BACKGROUND = 0,			// [0, 999]
	GEOMETRY = 1000,		// [1000, 1999]
	ALPHA_TEST = 2000,		// [2000, 2999]
	GEOMETRY_LAST = 3000,	// [3000, 3999]
	SKY_BOX = 4000,			// [4000, 4999]
	TRANSPARENT = 5000,		// [5000, 5999]
	POST_PROCESS = 6000,	// [6000, 6999]
	USER_INTERFACE = 7000,	// [7000, 7999]
};
DECL_XE_ENUM( RenderQueueType );


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
};

struct XE_API RenderData
{
	bool IsDynamic = false;
	XE::uint64 Mask = 0;
	XE::AABB BoundingBox;
	XE::Mat4x4f Transform;
	XE::RenderMaterialPtr Material;
	XE::GraphicsBufferPtr IndexBuffer;
	XE::GraphicsBufferPtr VertexBuffer;
	XE::GraphicsBufferPtr IndirectBuffer;
};

struct XE_API CameraData
{
	XE::float32 _FOV = 60.0f;
	XE::float32 _Near = 0.1f;
	XE::float32 _Far = 1000.0f;
	XE::uint32 _Depth = 1;
	XE::uint64 _Mask = 0;
	XE::Rectf _Viewport = { 0, 0, 1, 1 };
	XE::Mat4x4f _View;
	XE::Mat4x4f _Projection;
	XE::CameraType _Type = XE::CameraType::ORTHOGRAPHIC;
	XE::RenderGraphPtr _RenderGraph;
	XE::RenderTexturePtr _RenderTexture;
	XE::RenderResourcePtr _RenderResource;
};

struct XE_API CullingData
{
	XE::Array< XE::LightData * > Lights;

	XE::Array< XE::RenderData * > Background;
	XE::Array< XE::RenderData * > Geometry;
	XE::Array< XE::RenderData * > AlphaTest;
	XE::Array< XE::RenderData * > GeometryLast;
	XE::Array< XE::RenderData * > SkyBox;
	XE::Array< XE::RenderData * > Transparent;
	XE::Array< XE::RenderData * > PostProcess;
	XE::Array< XE::RenderData * > UserInterface;
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
		return val->BoundingBox;
	}
};

END_XE_NAMESPACE

#endif//TYPE_H__F5415E7B_FE49_440C_86FC_57B459E6B43E
