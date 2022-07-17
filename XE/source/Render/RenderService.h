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
	const XE::CameraComponentPtr & GetMainCamera() const;

	void SetMainCamera( const XE::CameraComponentPtr & val );

	const XE::RenderGraphPtr & GetDefaultRenderGraph() const;

	void SetDefaultRenderGraph( const XE::RenderGraphPtr & val );

public:
	XE::RenderResourcePtr CreateResource( const XE::RenderGraphPtr & val );

private:
	XE::Disposable RegisterLight( const XE::LightComponentPtr & val );

	XE::Disposable RegisterRender( const XE::RenderComponentPtr & val );

	XE::Disposable RegisterCamera( const XE::CameraComponentPtr & val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//RENDERSERVICE_H__753D8DD6_A8F5_4DB9_A414_F0464B975761
