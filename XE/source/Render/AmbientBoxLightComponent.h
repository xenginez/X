/*!
 * \file	AmbientBoxLightComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AMBIENTBOXLIGHTCOMPONENT_H__13CD6274_6EC5_4D2D_AD28_34765C121D57
#define AMBIENTBOXLIGHTCOMPONENT_H__13CD6274_6EC5_4D2D_AD28_34765C121D57

#include "LightComponent.h"

BEG_XE_NAMESPACE

class XE_API AmbientBoxLightComponent : public XE::LightComponent
{
	OBJECT( AmbientBoxLightComponent, XE::LightComponent )

public:
	AmbientBoxLightComponent();

	~AmbientBoxLightComponent() override;

public:
	XE::Vec3f GetSize() const;

	void SetSize( const XE::Vec3f & val );
};

END_XE_NAMESPACE

#endif//AMBIENTBOXLIGHTCOMPONENT_H__13CD6274_6EC5_4D2D_AD28_34765C121D57
