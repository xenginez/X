/*!
 * \file	CameraComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CAMERACOMPONENT_H__5F675737_2B16_4C7B_BD8F_5B84B24DD1F9
#define CAMERACOMPONENT_H__5F675737_2B16_4C7B_BD8F_5B84B24DD1F9

#include "Scene/GameObject.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API CameraComponent : public XE::GameObjectComponent
{
	OBJECT( CameraComponent, XE::GameObjectComponent )

private:
	friend class RenderService;

public:
	CameraComponent();

	~CameraComponent();

private:
	void Render( XE::RenderExecutor & val ) const;

protected:
	void OnStartup() override;

	void OnClearup() override;

public:
	XE::CameraType GetType() const;

	void SetType( XE::CameraType val );

	XE::float32 GetFOV() const;

	void SetFOV( XE::float32 val );

	XE::float32 GetNear() const;

	void SetNear( XE::float32 val );

	XE::float32 GetFar() const;

	void SetFar( XE::float32 val );

	XE::uint32 GetDepth() const;

	void SetDepth( XE::uint32 val );

	const XE::Rectf & GetViewport() const;

	void SetViewport( const XE::Rectf & val );

public:
	const XE::RenderGraphPtr & GetRenderGraph() const;

	void SetRenderGraph( const XE::RenderGraphPtr & val );

	const XE::RenderTexturePtr & GetRenderTexture() const;

	void SetRenderTexture( const XE::RenderTexturePtr & val );

public:
	XE::Mat4x4f GetView() const;

	XE::Mat4x4f GetProjection() const;

private:
	XE::CameraData _Data;
	XE::Disposable _Disposable;
	XE::RenderGraphPtr _RenderGraph;
	XE::RenderTexturePtr _RenderTexture;
	XE::RenderResourcePtr _RenderResource;
};

END_XE_NAMESPACE

#endif//CAMERACOMPONENT_H__5F675737_2B16_4C7B_BD8F_5B84B24DD1F9
