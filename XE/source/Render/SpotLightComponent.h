/*!
 * \file	SpotLightComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SPOTLIGHTCOMPONENT_H__2A774124_5A70_484E_B9E0_00DF2DCE4CAA
#define SPOTLIGHTCOMPONENT_H__2A774124_5A70_484E_B9E0_00DF2DCE4CAA

#include "LightComponent.h"

BEG_XE_NAMESPACE

class XE_API SpotLightComponent : public XE::LightComponent
{
	OBJECT( SpotLightComponent, XE::LightComponent )

public:
	SpotLightComponent();

	~SpotLightComponent() override;

public:
	XE::float32 GetConstant() const;

	void SetConstant( XE::float32 val );

	XE::float32 GetLinear() const;

	void SetLinear( XE::float32 val );

	XE::float32 GetQuadratic() const;

	void SetQuadratic( XE::float32 val );

	XE::float32 GetCutoff() const;

	void SetCutoff( XE::float32 val );

	XE::float32 GetOuterCutoff() const;

	void SetOuterCutoff( XE::float32 val );
};

END_XE_NAMESPACE

#endif//SPOTLIGHTCOMPONENT_H__2A774124_5A70_484E_B9E0_00DF2DCE4CAA
