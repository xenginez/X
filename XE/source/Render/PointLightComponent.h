/*!
 * \file	PointLightComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef POINTLIGHTCOMPONENT_H__ADA420C5_B75F_4265_A00F_6E1D6EBBAECC
#define POINTLIGHTCOMPONENT_H__ADA420C5_B75F_4265_A00F_6E1D6EBBAECC

#include "LightComponent.h"

BEG_XE_NAMESPACE

class XE_API PointLightComponent : public XE::LightComponent
{
	OBJECT( PointLightComponent, XE::LightComponent )

public:
	PointLightComponent();

	~PointLightComponent() override;

public:
	XE::float32 GetConstant() const;

	void SetConstant( XE::float32 val );

	XE::float32 GetLinear() const;

	void SetLinear( XE::float32 val );

	XE::float32 GetQuadratic() const;

	void SetQuadratic( XE::float32 val );
};

END_XE_NAMESPACE

#endif//POINTLIGHTCOMPONENT_H__ADA420C5_B75F_4265_A00F_6E1D6EBBAECC
