/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__A10FF3B4_0CA7_4A92_A70E_F058E9A0F04A
#define DECLARE_H__A10FF3B4_0CA7_4A92_A70E_F058E9A0F04A

#include "Math/Declare.h"
#include "Utils/Declare.h"
#include "Network/Declare.h"

BEG_XE_NAMESPACE

class Asset;
class CoreFramework;  using CoreFrameworkPtr = CoreFramework *;

DECL_PTR( Service );
DECL_PTR( EventService );
DECL_PTR( TimerService );
DECL_PTR( AudioService );
DECL_PTR( CacheService );
DECL_PTR( AssetsService );
DECL_PTR( ThreadService );
DECL_PTR( PhysicsService );
DECL_PTR( GraphicsService );
DECL_PTR( LocalizationService );

DECL_PTR( GraphicsAdapter );
DECL_PTR( GraphicsDevice );
DECL_PTR( GraphicsBindGroup );
DECL_PTR( GraphicsBindGroupLayout );
DECL_PTR( GraphicsBuffer );
DECL_PTR( GraphicsCommandBuffer );
DECL_PTR( GraphicsCommandEncoder );
DECL_PTR( GraphicsComputePassEncoder );
DECL_PTR( GraphicsComputePipeline );
DECL_PTR( GraphicsPipelineLayout );
DECL_PTR( GraphicsQuerySet );
DECL_PTR( GraphicsQueue );
DECL_PTR( GraphicsRenderBundle );
DECL_PTR( GraphicsRenderBundleEncoder );
DECL_PTR( GraphicsRenderPassEncoder );
DECL_PTR( GraphicsRenderPipeline );
DECL_PTR( GraphicsSampler );
DECL_PTR( GraphicsShaderModule );
DECL_PTR( GraphicsSurface );
DECL_PTR( GraphicsSwapChain );
DECL_PTR( GraphicsTexture );
DECL_PTR( GraphicsTextureView );

END_XE_NAMESPACE

#endif // DECLARE_H__A10FF3B4_0CA7_4A92_A70E_F058E9A0F04A
