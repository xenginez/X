/*!
 * \file	RenderComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERCOMPONENT_H__6844F58D_270A_4B19_AF31_04312B6B1F87
#define RENDERCOMPONENT_H__6844F58D_270A_4B19_AF31_04312B6B1F87

#include "Scene/GameObject.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderComponent : public XE::GameObjectComponent
{
	OBJECT( RenderComponent, XE::GameObjectComponent )

private:
	friend class RenderService;

public:
	RenderComponent();

	~RenderComponent() override;

protected:
	void OnStartup() override;

	void OnUpdate( XE::float32 dt ) override;

	virtual void OnRender( const XE::GraphicsCommandEncoderPtr & encoder ) = 0;

	void OnClearup() override;

private:
	void Render( const XE::GraphicsCommandEncoderPtr & encoder );

public:
	bool GetDynamic() const;

	void SetDynamic( bool val );

	const XE::RenderMaterialPtr & GetMaterial() const;

	void SetMaterial( const XE::RenderMaterialPtr & val );

private:
	bool _Dynamic = false;
	XE::Disposable _Disposable;
	XE::RenderMaterialPtr _Material;
};

END_XE_NAMESPACE

#endif//RENDERCOMPONENT_H__6844F58D_270A_4B19_AF31_04312B6B1F87
