/*!
 * \file	RenderService.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERSERVICE_H__753D8DD6_A8F5_4DB9_A414_F0464B975761
#define RENDERSERVICE_H__753D8DD6_A8F5_4DB9_A414_F0464B975761

#include "Core/Service.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderService : public XE::Service
{
	OBJECT( RenderService, XE::Service )

private:
	friend class LightComponent;
	friend class CameraComponent;
	friend class RenderComponent;

private:
	struct Private;

public:
	RenderService();

	~RenderService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::RenderResourcePtr CreateResource( const XE::RenderGraphPtr & val );

public:
	XE::RenderTexturePtr GetCurrentMainTexture() const;

	XE::RenderTexturePtr GetRenderTextureFromPool( XE::int32 width, XE::int32 height, XE::GraphicsTextureFormat format );

	XE::RenderTexturePtr GetRenderTextureFromGlobal( const XE::String & name );

	XE::RenderTexturePtr CreateRenderTextureFromGlobal( const XE::String & name, XE::int32 width, XE::int32 height, XE::GraphicsTextureFormat format );

public:
	XE::RenderQueueData Culling( const XE::CameraData * val );

private:
	XE::Disposable RegisterLight( XE::LightData * val );

	XE::Disposable RegisterRender( XE::RenderData * val );

	XE::Disposable RegisterCamera( XE::CameraData * val );

private:
	void RenderCamera( XE::CameraData * val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//RENDERSERVICE_H__753D8DD6_A8F5_4DB9_A414_F0464B975761
