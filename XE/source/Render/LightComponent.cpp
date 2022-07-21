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

XE::AABB XE::LightComponent::GetBoundingBox() const
{
	return {};
}
