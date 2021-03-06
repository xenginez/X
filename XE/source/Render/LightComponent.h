/*!
 * \file	LightComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LIGHTCOMPONENT_H__33D47F37_FE49_41CB_93C2_65DA590FD74D
#define LIGHTCOMPONENT_H__33D47F37_FE49_41CB_93C2_65DA590FD74D

#include "Scene/GameObject.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API LightComponent : public XE::GameObjectComponent
{
	OBJECT( LightComponent, XE::GameObjectComponent )

private:
	friend class RenderService;

public:
	LightComponent();

	~LightComponent() override;

protected:
	void OnStartup() override;

	void OnClearup() override;

public:
	bool GetDynamic() const;

	void SetDynamic( bool val );

	XE::uint64 GetMask() const;

	void SetMask( XE::uint64 val );

	XE::float32 GetIntensity() const;

	void SetIntensity( XE::float32 val );

	const XE::Color & GetColor() const;

	void SetColor( const XE::Color & val );

public:
	XE::AABB GetBoundingBox() const;

protected:
	XE::LightData _Data;

private:
	XE::Disposable _Disposable;
};

END_XE_NAMESPACE

#endif//LIGHTCOMPONENT_H__33D47F37_FE49_41CB_93C2_65DA590FD74D
