#include "AmbientSphereLightComponent.h"

BEG_META( XE::AmbientSphereLightComponent )
END_META()

XE::AmbientSphereLightComponent::AmbientSphereLightComponent()
{
	_Data.Type = XE::LightType::AMBIENT_SPHERE;
}

XE::AmbientSphereLightComponent::~AmbientSphereLightComponent()
{

}

XE::float32 XE::AmbientSphereLightComponent::GetRadius() const
{
	return _Data.Constant;
}

void XE::AmbientSphereLightComponent::SetRadius( XE::float32 val )
{
	_Data.Constant = val;
}
