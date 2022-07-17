#include "CameraComponent.h"

#include "Core/GraphicsService.h"

#include "RenderGraph.h"
#include "RenderTexture.h"
#include "RenderService.h"

BEG_META( XE::CameraComponent )
type->Property( "ClearFlags", &XE::CameraComponent::_ClearFlags );
type->Property( "ClearColor", &XE::CameraComponent::_ClearColor )->Attribute( XE::NonInspectorAttribute() )->Attribute( XE::LinkAttribute( "ClearFlags", []( const XE::Variant & link ) { return link.Value< XE::CameraClearFlags >() && XE::CameraClearFlag::COLOR; } ) );
type->Property( "ClearDepth", &XE::CameraComponent::_ClearDepth )->Attribute( XE::NonInspectorAttribute() )->Attribute( XE::LinkAttribute( "ClearFlags", []( const XE::Variant & link ) { return link.Value< XE::CameraClearFlags >() && XE::CameraClearFlag::DEPTH; } ) );
type->Property( "ClearStencil", &XE::CameraComponent::_ClearStencil )->Attribute( XE::NonInspectorAttribute() )->Attribute( XE::LinkAttribute( "ClearFlags", []( const XE::Variant & link ) { return link.Value< XE::CameraClearFlags >() && XE::CameraClearFlag::STENCIL; } ) );
type->Property( "Projection", &XE::CameraComponent::_Projection );
type->Property( "Perspective", &XE::CameraComponent::_Perspective )->Attribute( XE::NonInspectorAttribute() )->Attribute( XE::LinkAttribute( "Projection", []( const XE::Variant & link ) { return link.Value< XE::ProjectionType >() == XE::ProjectionType::PERSPECTIVE; } ) );
type->Property( "Orthographic", &XE::CameraComponent::_Orthographic )->Attribute( XE::NonInspectorAttribute() )->Attribute( XE::LinkAttribute( "Projection", []( const XE::Variant & link ) { return link.Value< XE::ProjectionType >() == XE::ProjectionType::ORTHOGRAPHIC; } ) );
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

XE::CameraClearFlags XE::CameraComponent::GetClearFlags() const
{
	return _ClearFlags;
}

void XE::CameraComponent::SetClearFlags( XE::CameraClearFlags val )
{
	_ClearFlags = val;
}

const XE::Color & XE::CameraComponent::GetClearColor() const
{
	return _ClearColor;
}

void XE::CameraComponent::SetClearColor( const XE::Color & val )
{
	_ClearColor = val;
}

XE::float32 XE::CameraComponent::GetClearDepth() const
{
	return _ClearDepth;
}

void XE::CameraComponent::SetClearDepth( XE::float32 val )
{
	_ClearDepth = val;
}

XE::uint32 XE::CameraComponent::GetClearStencil() const
{
	return _ClearStencil;
}

void XE::CameraComponent::SetClearStencil( XE::uint32 val )
{
	_ClearStencil = val;
}

XE::ProjectionType XE::CameraComponent::GetProjectionType() const
{
	return _Projection;
}

void XE::CameraComponent::SetProjectionType( XE::ProjectionType val )
{
	_Projection = val;
}

const XE::PerspectiveInfo & XE::CameraComponent::GetPerspective() const
{
	return _Perspective;
}

void XE::CameraComponent::SetPerspective( const XE::PerspectiveInfo & val )
{
	_Perspective = val;
}

const XE::OrthographicInfo & XE::CameraComponent::GetOrthographic() const
{
	return _Orthographic;
}

void XE::CameraComponent::SetOrthographic( const XE::OrthographicInfo & val )
{
	_Orthographic = val;
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
