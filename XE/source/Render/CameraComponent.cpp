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
type->Property( "RenderGraph", &XE::CameraComponent::GetRenderGraph, &XE::CameraComponent::SetRenderGraph );
type->Property( "RenderTexture", &XE::CameraComponent::GetRenderTexture, &XE::CameraComponent::SetRenderTexture );
END_META()

XE::CameraComponent::CameraComponent()
{

}

XE::CameraComponent::~CameraComponent()
{

}

void XE::CameraComponent::OnStartup()
{
	if ( auto service = GetService< XE::RenderService >() )
	{
		_Disposable = service->RegisterCamera( &_Data );

		if ( _Data._RenderGraph )
		{
			_Data._RenderResource = service->CreateResource( _Data._RenderGraph );
		}
	}
}

void XE::CameraComponent::OnUpdate( XE::float32 dt )
{
	_Data._View = XE::Mathf::View( GetGameObject()->GetTransform().GetWorldPosition(), GetGameObject()->GetTransform().GetWorldRotation() );

	XE::float32 w = 1, h = 1;
	if( _Data._RenderTexture )
	{
		w = _Data._RenderTexture->GetWidth() * _Data._Viewport.width;
		h = _Data._RenderTexture->GetHeight() * _Data._Viewport.height;
	}
	auto aspect = w / h;

	if( _Data._Type == XE::CameraType::PERSPECTIVE )
	{
		_Data._Projection = XE::Mathf::ProjectionPerspective( _Data._FOV, aspect, _Data._Near, _Data._Far );
	}
	else
	{
		auto width = _Data._FOV * 10 * aspect;
		auto height = _Data._FOV * 10;
		_Data._Projection = XE::Mathf::ProjectionOrthographic( -( width / 2 ), width / 2, height / 2, -( height / 2 ), _Data._Near, _Data._Far );
	}
}

void XE::CameraComponent::OnClearup()
{
	_Disposable.Dispose();
}

XE::uint64 XE::CameraComponent::GetMask() const
{
	return _Data._Mask;
}

void XE::CameraComponent::SetMask( XE::uint64 val )
{
	_Data._Mask = val;
}

XE::CameraType XE::CameraComponent::GetType() const
{
	return _Data._Type;
}

void XE::CameraComponent::SetType( XE::CameraType val )
{
	_Data._Type = val;
}

XE::float32 XE::CameraComponent::GetFOV() const
{
	return _Data._FOV;
}

void XE::CameraComponent::SetFOV( XE::float32 val )
{
	_Data._FOV = val;
}

XE::float32 XE::CameraComponent::GetNear() const
{
	return _Data._Near;
}

void XE::CameraComponent::SetNear( XE::float32 val )
{
	_Data._Near = val;
}

XE::float32 XE::CameraComponent::GetFar() const
{
	return _Data._Far;
}

void XE::CameraComponent::SetFar( XE::float32 val )
{
	_Data._Far = val;
}

XE::uint32 XE::CameraComponent::GetDepth() const
{
	return _Data._Depth;
}

void XE::CameraComponent::SetDepth( XE::uint32 val )
{
	_Data._Depth = val;
}

const XE::Rectf & XE::CameraComponent::GetViewport() const
{
	return _Data._Viewport;
}

void XE::CameraComponent::SetViewport( const XE::Rectf & val )
{
	_Data._Viewport = val;
}

const XE::RenderGraphPtr & XE::CameraComponent::GetRenderGraph() const
{
	return _Data._RenderGraph;
}

void XE::CameraComponent::SetRenderGraph( const XE::RenderGraphPtr & val )
{
	_Data._RenderGraph = val;
}

const XE::RenderTexturePtr & XE::CameraComponent::GetRenderTexture() const
{
	return _Data._RenderTexture;
}

void XE::CameraComponent::SetRenderTexture( const XE::RenderTexturePtr & val )
{
	_Data._RenderTexture = val;
}

XE::Frustum XE::CameraComponent::GetFrustum() const
{
	return { GetView() * GetProjection() };
}

XE::Mat4x4f XE::CameraComponent::GetView() const
{
	return _Data._View;
}

XE::Mat4x4f XE::CameraComponent::GetProjection() const
{
	return _Data._Projection;
}
