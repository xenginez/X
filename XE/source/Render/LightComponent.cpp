#include "LightComponent.h"

#include "RenderService.h"

BEG_META( XE::LightComponent )
END_META()

XE::LightComponent::LightComponent()
{

}

XE::LightComponent::~LightComponent()
{

}

void XE::LightComponent::OnStartup()
{
	_Disposable = GetService< XE::RenderService >()->RegisterLight( &_Data );
}

void XE::LightComponent::OnClearup()
{
	_Disposable.Dispose();
}

bool XE::LightComponent::GetDynamic() const
{
	return _Data.IsDynamic;
}

void XE::LightComponent::SetDynamic( bool val )
{
	_Data.IsDynamic = val;
}

XE::uint64 XE::LightComponent::GetMask() const
{
	return _Data.Mask;
}

void XE::LightComponent::SetMask( XE::uint64 val )
{
	_Data.Mask = val;
}

XE::float32 XE::LightComponent::GetIntensity() const
{
	return _Data.Intensity;
}

void XE::LightComponent::SetIntensity( XE::float32 val )
{
	_Data.Intensity = val;
}

const XE::Color & XE::LightComponent::GetColor() const
{
	return _Data.Color;
}

void XE::LightComponent::SetColor( const XE::Color & val )
{
	_Data.Color = val;
}

XE::AABB XE::LightComponent::GetBoundingBox() const
{
	switch ( _Data.Type )
	{
	case XE::LightType::SPOT:
		break;
	case XE::LightType::POINT:
		break;
	case XE::LightType::DIRECTIONAL:
		break;
	case XE::LightType::AMBIENT_BOX:
		break;
	case XE::LightType::AMBIENT_SPHERE:
		break;
	default:
		break;
	}

	return {};
}
