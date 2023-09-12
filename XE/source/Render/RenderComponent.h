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

	void OnClearup() override;

public:
	bool GetDynamic() const;

	void SetDynamic( bool val );

	XE::uint64 GetMask() const;

	void SetMask( XE::uint64 val );

	const XE::RenderMaterialPtr & GetMaterial() const;

	void SetMaterial( const XE::RenderMaterialPtr & val );

public:
	XE::AABB GetBoundingBox() const;

private:
	XE::RenderData _Data;
	XE::Disposable _Disposable;
};

END_XE_NAMESPACE

#endif//RENDERCOMPONENT_H__6844F58D_270A_4B19_AF31_04312B6B1F87
