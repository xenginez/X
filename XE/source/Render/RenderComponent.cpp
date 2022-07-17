#include "RenderComponent.h"

#include "RenderMaterial.h"

BEG_META( XE::RenderComponent )
END_META()

XE::RenderComponent::RenderComponent()
{

}

XE::RenderComponent::~RenderComponent()
{

}

void XE::RenderComponent::OnStartup()
{

}

void XE::RenderComponent::OnClearup()
{

}

void XE::RenderComponent::Render( const XE::GraphicsCommandEncoderPtr & encoder ) const
{

}

bool XE::RenderComponent::GetDynamic() const
{
	return _Dynamic;
}

void XE::RenderComponent::SetDynamic( bool val )
{
	_Dynamic = val;
}

const XE::RenderMaterialPtr & XE::RenderComponent::GetMaterial() const
{
	return _Material;
}

void XE::RenderComponent::SetMaterial( const XE::RenderMaterialPtr & val )
{
	_Material = val;
}

XE::AABB XE::RenderComponent::GetBoundingBox() const
{
	return {};
}
