#include "RenderComponent.h"

#include "RenderService.h"
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
	_Disposable = GetService< XE::RenderService >()->RegisterRender( &_Data );
}

void XE::RenderComponent::OnClearup()
{
	_Disposable.Dispose();
}

void XE::RenderComponent::Render( const XE::GraphicsCommandEncoderPtr & encoder ) const
{

}

bool XE::RenderComponent::GetDynamic() const
{
	return _Data.IsDynamic;
}

void XE::RenderComponent::SetDynamic( bool val )
{
	_Data.IsDynamic = val;
}

XE::uint64 XE::RenderComponent::GetMask() const
{
	return _Data.Mask;
}

void XE::RenderComponent::SetMask( XE::uint64 val )
{
	_Data.Mask = val;
}

const XE::RenderMaterialPtr & XE::RenderComponent::GetMaterial() const
{
	return _Data.Material;
}

void XE::RenderComponent::SetMaterial( const XE::RenderMaterialPtr & val )
{
	_Data.Material = val;
}

XE::AABB XE::RenderComponent::GetBoundingBox() const
{
	return {};
}
