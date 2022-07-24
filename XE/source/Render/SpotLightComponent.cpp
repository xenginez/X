#include "SpotLightComponent.h"

BEG_META( XE::SpotLightComponent )
END_META()

XE::SpotLightComponent::SpotLightComponent()
{
	_Data.Type = XE::LightType::SPOT;
}

XE::SpotLightComponent::~SpotLightComponent()
{

}

XE::float32 XE::SpotLightComponent::GetConstant() const
{
	return _Data.Constant;
}

void XE::SpotLightComponent::SetConstant( XE::float32 val )
{
	_Data.Constant = val;
}

XE::float32 XE::SpotLightComponent::GetLinear() const
{
	return _Data.Linear;
}

void XE::SpotLightComponent::SetLinear( XE::float32 val )
{
	_Data.Linear = val;
}

XE::float32 XE::SpotLightComponent::GetQuadratic() const
{
	return _Data.Quadratic;
}

void XE::SpotLightComponent::SetQuadratic( XE::float32 val )
{
	_Data.Quadratic = val;
}

XE::float32 XE::SpotLightComponent::GetCutoff() const
{
	return _Data.Cutoff;
}

void XE::SpotLightComponent::SetCutoff( XE::float32 val )
{
	_Data.Cutoff = val;
}

XE::float32 XE::SpotLightComponent::GetOuterCutoff() const
{
	return _Data.OuterCutoff;
}

void XE::SpotLightComponent::SetOuterCutoff( XE::float32 val )
{
	_Data.OuterCutoff = val;
}
