/*!
 * \file	AmbientSphereLightComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AMBIENTSPHERELIGHTCOMPONENT_H__47ECAB08_D175_4CB2_8AA2_84100B5C01FF
#define AMBIENTSPHERELIGHTCOMPONENT_H__47ECAB08_D175_4CB2_8AA2_84100B5C01FF

#include "LightComponent.h"

BEG_XE_NAMESPACE

class XE_API AmbientSphereLightComponent : public XE::LightComponent
{
	OBJECT( AmbientSphereLightComponent, XE::LightComponent )

public:
	AmbientSphereLightComponent();

	~AmbientSphereLightComponent() override;

public:
	XE::float32 GetRadius() const;

	void SetRadius( XE::float32 val );
};

END_XE_NAMESPACE

#endif//AMBIENTSPHERELIGHTCOMPONENT_H__47ECAB08_D175_4CB2_8AA2_84100B5C01FF
