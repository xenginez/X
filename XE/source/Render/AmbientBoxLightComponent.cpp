#include "AmbientBoxLightComponent.h"

BEG_META( XE::AmbientBoxLightComponent )
END_META()

XE::AmbientBoxLightComponent::AmbientBoxLightComponent()
{
	_Data.Type = XE::LightType::AMBIENT_BOX;
}

XE::AmbientBoxLightComponent::~AmbientBoxLightComponent()
{

}

XE::Vec3f XE::AmbientBoxLightComponent::GetSize() const
{
	return { _Data.Constant, _Data.Linear, _Data.Quadratic };
}

void XE::AmbientBoxLightComponent::SetSize( const XE::Vec3f & val )
{
	_Data.Constant = val.x;
	_Data.Linear = val.y;
	_Data.Quadratic = val.z;
}
