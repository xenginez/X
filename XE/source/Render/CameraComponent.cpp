#include "CameraComponent.h"

#include "Math/Mathf.h"
#include "Core/GraphicsService.h"
#include "Scene/GameObject.h"

#include "RenderGraph.h"
#include "RenderTexture.h"
#include "RenderService.h"

BEG_META( XE::CameraComponent )
type->Property( "Mask", &XE::CameraComponent::GetMask, &XE::CameraComponent::SetMask );
type->Property( "Type", &XE::CameraComponent::GetType, &XE::CameraComponent::SetType );
type->Property( "FOV", &XE::CameraComponent::GetFOV, &XE::CameraComponent::SetFOV );
type->Property( "Near", &XE::CameraComponent::GetNear, &XE::CameraComponent::SetNear );
type->Property( "Far", &XE::CameraComponent::GetFar, &XE::CameraComponent::SetFar );
type->Property( "Depth", &XE::CameraComponent::GetDepth, &XE::CameraComponent::SetDepth );
type->Property( "Viewport", &XE::CameraComponent::GetViewport, &XE::CameraComponent::SetViewport );

type->Property( "RenderGraph", &XE::CameraComponent::_RenderGraph );
type->Property( "RenderTexture", &XE::CameraComponent::_RenderTexture );
END_META()

XE::CameraComponent::CameraComponent()
{

}

XE::CameraComponent::~CameraComponent()
{

}

void XE::CameraComponent::Render( XE::RenderExecutor & val ) const
{
	if ( _RenderResource && _RenderGraph && _RenderTexture )
	{
		val.Execute( _RenderResource, _RenderGraph, _RenderTexture );
	}
}

void XE::CameraComponent::OnStartup()
{
	if ( auto service = GetService< XE::RenderService >() )
	{
		_Disposable = service->RegisterCamera( &_Data );

		if ( _RenderGraph )
		{
			_RenderResource = service->CreateResource( _RenderGraph );
		}
	}
}

void XE::CameraComponent::OnClearup()
{
	_Disposable.Dispose();
}

XE::uint64 XE::CameraComponent::GetMask() const
{
	return _Data.Mask;
}

void XE::CameraComponent::SetMask( XE::uint64 val )
{
	_Data.Mask = val;
}

XE::CameraType XE::CameraComponent::GetType() const
{
	return _Data.Type;
}

void XE::CameraComponent::SetType( XE::CameraType val )
{
	_Data.Type = val;
}

XE::float32 XE::CameraComponent::GetFOV() const
{
	return _Data.FOV;
}

void XE::CameraComponent::SetFOV( XE::float32 val )
{
	_Data.FOV = val;
}

XE::float32 XE::CameraComponent::GetNear() const
{
	return _Data.Near;
}

void XE::CameraComponent::SetNear( XE::float32 val )
{
	_Data.Near = val;
}

XE::float32 XE::CameraComponent::GetFar() const
{
	return _Data.Far;
}

void XE::CameraComponent::SetFar( XE::float32 val )
{
	_Data.Far = val;
}

XE::uint32 XE::CameraComponent::GetDepth() const
{
	return _Data.Depth;
}

void XE::CameraComponent::SetDepth( XE::uint32 val )
{
	_Data.Depth = val;
}

const XE::Rectf & XE::CameraComponent::GetViewport() const
{
	return _Data.Viewport;
}

void XE::CameraComponent::SetViewport( const XE::Rectf & val )
{
	_Data.Viewport = val;
}

const XE::RenderGraphPtr & XE::CameraComponent::GetRenderGraph() const
{
	return _RenderGraph;
}

void XE::CameraComponent::SetRenderGraph( const XE::RenderGraphPtr & val )
{
	_RenderGraph = val;
}

const XE::RenderTexturePtr & XE::CameraComponent::GetRenderTexture() const
{
	return _RenderTexture;
}

void XE::CameraComponent::SetRenderTexture( const XE::RenderTexturePtr & val )
{
	_RenderTexture = val;
}

XE::Frustum XE::CameraComponent::GetFrustum() const
{
	return { GetView() * GetProjection() };
}

XE::Mat4x4f XE::CameraComponent::GetView() const
{
	return XE::Mathf::View( GetGameObject()->GetTransform().GetWorldPosition(), GetGameObject()->GetTransform().GetWorldRotation() );
}

XE::Mat4x4f XE::CameraComponent::GetProjection() const
{
	XE::float32 w = 1, h = 1;
	if ( _RenderTexture )
	{
		w = _RenderTexture->GetWidth() * _Data.Viewport.width;
		h = _RenderTexture->GetHeight() * _Data.Viewport.height;
	}
	auto aspect = w / h;

	if ( _Data.Type == XE::CameraType::PERSPECTIVE )
	{
		return XE::Mathf::ProjectionPerspective( _Data.FOV, aspect, _Data.Near, _Data.Far );
	}
	else
	{
		auto width = _Data.FOV * 10 * aspect;
		auto height = _Data.FOV * 10;
		return XE::Mathf::ProjectionOrthographic( -( width / 2 ), width / 2, height / 2, -( height / 2 ), _Data.Near, _Data.Far );
	}
}
