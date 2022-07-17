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
	XE::CameraClearFlags GetClearFlags() const;

	void SetClearFlags( XE::CameraClearFlags val );

	const XE::Color & GetClearColor() const;

	void SetClearColor( const XE::Color & val );

	XE::float32 GetClearDepth() const;

	void SetClearDepth( XE::float32 val );

	XE::uint32 GetClearStencil() const;

	void SetClearStencil( XE::uint32 val );

	XE::ProjectionType GetProjectionType() const;

	void SetProjectionType( XE::ProjectionType val );

	const XE::PerspectiveInfo & GetPerspective() const;

	void SetPerspective( const XE::PerspectiveInfo & val );

	const XE::OrthographicInfo & GetOrthographic() const;

	void SetOrthographic( const XE::OrthographicInfo & val );

	const XE::Rectf & GetViewport() const;

	void SetViewport( const XE::Rectf & val );

	const XE::RenderGraphPtr & GetRenderGraph() const;

	void SetRenderGraph( const XE::RenderGraphPtr & val );

	const XE::RenderTexturePtr & GetRenderTexture() const;

	void SetRenderTexture( const XE::RenderTexturePtr & val );

private:
	XE::Disposable _Disposable;

	XE::CameraClearFlags _ClearFlags = XE::MakeFlags( XE::CameraClearFlag::COLOR, XE::CameraClearFlag::DEPTH, XE::CameraClearFlag::STENCIL );
	XE::Color _ClearColor = XE::Color::Black;
	XE::float32 _ClearDepth = 0.0f;
	XE::uint32 _ClearStencil = 1;

	XE::Rectf _Viewport = { 0.0f, 0.0f, 1.0f, 1.0f };
	XE::ProjectionType _Projection = ProjectionType::ORTHOGRAPHIC;
	XE::PerspectiveInfo _Perspective;
	XE::OrthographicInfo _Orthographic;

	XE::RenderGraphPtr _RenderGraph;
	XE::RenderTexturePtr _RenderTexture;
	XE::RenderResourcePtr _RenderResource;
};

END_XE_NAMESPACE

#endif//CAMERACOMPONENT_H__5F675737_2B16_4C7B_BD8F_5B84B24DD1F9
