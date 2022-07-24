/*!
 * \file	DirectionalLightComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DIRECTIONALLIGHTCOMPONENT_H__4618B3A1_3149_4562_9639_B3D5598DAD79
#define DIRECTIONALLIGHTCOMPONENT_H__4618B3A1_3149_4562_9639_B3D5598DAD79

#include "LightComponent.h"

BEG_XE_NAMESPACE

class XE_API DirectionalLightComponent : public XE::LightComponent
{
	OBJECT( DirectionalLightComponent, XE::LightComponent )

public:
	DirectionalLightComponent();

	~DirectionalLightComponent() override;
};

END_XE_NAMESPACE

#endif//DIRECTIONALLIGHTCOMPONENT_H__4618B3A1_3149_4562_9639_B3D5598DAD79
