#include "PointLightComponent.h"

BEG_META( XE::PointLightComponent )
END_META()

XE::PointLightComponent::PointLightComponent()
{
	_Data.Type = XE::LightType::POINT;
}

XE::PointLightComponent::~PointLightComponent()
{

}

XE::float32 XE::PointLightComponent::GetConstant() const
{
	return _Data.Constant;
}

void XE::PointLightComponent::SetConstant( XE::float32 val )
{
	_Data.Constant = val;
}

XE::float32 XE::PointLightComponent::GetLinear() const
{
	return _Data.Linear;
}

void XE::PointLightComponent::SetLinear( XE::float32 val )
{
	_Data.Linear = val;
}

XE::float32 XE::PointLightComponent::GetQuadratic() const
{
	return _Data.Quadratic;
}

void XE::PointLightComponent::SetQuadratic( XE::float32 val )
{
	_Data.Quadratic = val;
}
