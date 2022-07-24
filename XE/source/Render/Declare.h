/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__2C7FF4A4_D367_4A7D_AAEC_4B8217B3D7A7
#define DECLARE_H__2C7FF4A4_D367_4A7D_AAEC_4B8217B3D7A7

#include "Core/Declare.h"
#include "Scene/Declare.h"

BEG_XE_NAMESPACE

class RenderBuilder;
class RenderPassData;
class RenderExecutor;

class RenderService;
class LightComponent;
class CameraComponent;
class RenderComponent;
class SpriteComponent;
class SkinnedComponent;
class SpotLightComponent;
class PointLightComponent;
class StaticMeshComponent;
class AmbientBoxLightComponent;
class DirectionalLightComponent;
class AmbientSphereLightComponent;

DECL_PTR( RenderPass );
DECL_PTR( RenderGraph );
DECL_PTR( RenderResource );

DECL_PTR( RenderMesh );
DECL_PTR( RenderShader );
DECL_PTR( RenderTexture );
DECL_PTR( RenderMaterial );
// DECL_PTR( RenderGBuffer );
// DECL_PTR( RenderVirtualTexture );

DECL_PTR( RenderService );
DECL_PTR( LightComponent );
DECL_PTR( CameraComponent );
DECL_PTR( RenderComponent );

END_XE_NAMESPACE

#endif//DECLARE_H__2C7FF4A4_D367_4A7D_AAEC_4B8217B3D7A7
