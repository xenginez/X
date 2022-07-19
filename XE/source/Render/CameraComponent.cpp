#include "CameraComponent.h"

#include "Math/Mathf.h"
#include "Core/GraphicsService.h"
#include "Scene/GameObject.h"

#include "RenderGraph.h"
#include "RenderTexture.h"
#include "RenderService.h"

BEG_META( XE::CameraComponent )
type->Property( "Type", &XE::CameraComponent::_Type );
type->Property( "FOV", &XE::CameraComponent::_FOV );
type->Property( "Near", &XE::CameraComponent::_Near );
type->Property( "Far", &XE::CameraComponent::_Far );
type->Property( "Viewport", &XE::CameraComponent::_Viewport );
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
		_Disposable = service->RegisterCamera( XE_THIS( XE::CameraComponent ) );

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

XE::float32 XE::CameraComponent::GetFOV() const
{
	return _FOV;
}

void XE::CameraComponent::SetFOV( XE::float32 val )
{
	_FOV = val;
}

XE::float32 XE::CameraComponent::GetNear() const
{
	return _Near;
}

void XE::CameraComponent::SetNear( XE::float32 val )
{
	_Near = val;
}

XE::float32 XE::CameraComponent::GetFar() const
{
	return _Far;
}

void XE::CameraComponent::SetFar( XE::float32 val )
{
	_Far = val;
}

XE::CameraType XE::CameraComponent::GetCameraType() const
{
	return _Type;
}

void XE::CameraComponent::SetCameraType( XE::CameraType val )
{
	_Type = val;
}

const XE::Rectf & XE::CameraComponent::GetViewport() const
{
	return _Viewport;
}

void XE::CameraComponent::SetViewport( const XE::Rectf & val )
{
	_Viewport = val;
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

XE::Mat4x4f XE::CameraComponent::GetView() const
{
	return XE::Mathf::View( GetGameObject()->GetTransform().GetWorldPosition(), GetGameObject()->GetTransform().GetWorldRotation() );
}

XE::Mat4x4f XE::CameraComponent::GetProjection() const
{
	XE::float32 w = 1, h = 1;
	if ( _RenderTexture )
	{
		w = _RenderTexture->GetWidth() * _Viewport.width;
		h = _RenderTexture->GetHeight() * _Viewport.height;
	}
	auto aspect = w / h;

	if ( _Type == XE::CameraType::PERSPECTIVE )
	{
		return XE::Mathf::ProjectionPerspective( _FOV, aspect, _Near, _Far );
	}
	else
	{
		auto width = _FOV * 10 * aspect;
		auto height = _FOV * 10;
		return XE::Mathf::ProjectionOrthographic( -( width / 2 ), width / 2, height / 2, -( height / 2 ), _Near, _Far );
	}
}
