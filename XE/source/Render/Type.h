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

BEG_XE_NAMESPACE

DECL_PTR( RenderPass );
DECL_PTR( RenderBuilder );
DECL_PTR( RenderResource );

DECL_PTR( RenderTexture );
DECL_PTR( RenderMaterial );

DECL_PTR( RenderService );
DECL_PTR( LightComponent );
DECL_PTR( CameraComponent );
DECL_PTR( RenderComponent );

using RenderGraph = XE::Graph< XE::RenderPassPtr >;
DECL_PTR( RenderGraph );

enum class ProjectionType
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};
DECL_XE_ENUM( ProjectionType );

enum class CameraClearFlag
{
	NONE = 0,
	COLOR = 1 << 0,
	DEPTH = 1 << 1,
	STENCIL = 1 << 2,
};
DECL_XE_ENUM( CameraClearFlag ); DECL_FLAGS( CameraClearFlag, CameraClearFlags );

struct XE_API PerspectiveInfo
{
	XE::float32 Fov = 60.0f;
	XE::float32 ZNear = 0.1f;
	XE::float32 ZFar = 1000.0f;
};
DECL_XE_CLASS( PerspectiveInfo );

struct XE_API OrthographicInfo
{
	XE::float32 Size = 5;
	XE::float32 ZNear = 0.1f;
	XE::float32 ZFar = 1000.0f;
};
DECL_XE_CLASS( OrthographicInfo );

END_XE_NAMESPACE

#endif//TYPE_H__F5415E7B_FE49_440C_86FC_57B459E6B43E
