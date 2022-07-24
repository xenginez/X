#include "DirectionalLightComponent.h"

IMPLEMENT_META( XE::DirectionalLightComponent );

XE::DirectionalLightComponent::DirectionalLightComponent()
{
	_Data.Type = XE::LightType::DIRECTIONAL;
}

XE::DirectionalLightComponent::~DirectionalLightComponent()
{

}
