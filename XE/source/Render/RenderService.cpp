#include "RenderService.h"

#include "LightComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"

IMPLEMENT_META( XE::RenderService );

XE::RenderService::RenderService()
{

}

XE::RenderService::~RenderService()
{

}

void XE::RenderService::Prepare()
{

}

void XE::RenderService::Startup()
{

}

void XE::RenderService::Update()
{

}

void XE::RenderService::Clearup()
{

}

const XE::CameraComponentPtr & XE::RenderService::GetMainCamera() const
{
	return _MainCamera;
}

void XE::RenderService::SetMainCamera( const XE::CameraComponentPtr & val )
{
	_MainCamera = val;
}

const XE::RenderGraphPtr & XE::RenderService::GetDefaultRenderGraph() const
{
	return _DefaultGraph;
}

void XE::RenderService::SetDefaultRenderGraph( const XE::RenderGraphPtr & val )
{
	_DefaultGraph = val;
}

XE::Disposable XE::RenderService::RegisterLight( const XE::LightComponentPtr & val )
{
	if ( val->GetDynamic() )
	{
		auto handle = _DynamicLightQueue.Alloc();

		if ( _DynamicLights.size() <= handle.GetValue() ) _DynamicLights.resize( handle.GetValue() + 1 );

		_DynamicLights[handle.GetValue()] = val;

		return { [this, handle]() { _DynamicLights[handle.GetValue()] = nullptr; _DynamicLightQueue.Free( handle ); } };
	}
	else
	{
		_StaticLights.Insert( val );

		return { [this, val]() { _StaticLights.Remove( val ); } };
	}
}

XE::Disposable XE::RenderService::RegisterRender( const XE::RenderComponentPtr & val )
{
	if ( val->GetDynamic() )
	{
		auto handle = _DynamicRenderQueue.Alloc();

		if ( _DynamicRenders.size() <= handle.GetValue() ) _DynamicRenders.resize( handle.GetValue() + 1 );

		_DynamicRenders[handle.GetValue()] = val;

		return { [this, handle]() { _DynamicRenders[handle.GetValue()] = nullptr; _DynamicRenderQueue.Free( handle ); } };
	}
	else
	{
		_StaticRenders.Insert( val );

		return { [this, val]() { _StaticRenders.Remove( val ); } };
	}
}

XE::Disposable XE::RenderService::RegisterCamera( const XE::CameraComponentPtr & val )
{
	auto handle = _CameraQueue.Alloc();

	if ( _Cameras.size() <= handle.GetValue() ) _Cameras.resize( handle.GetValue() + 1 );

	_Cameras[handle.GetValue()] = val;

	if ( val->GetRenderGraph() == nullptr )
	{
		val->SetRenderGraph( _DefaultGraph );
	}

	return { [this, handle]() { _Cameras[handle.GetValue()] = nullptr; _CameraQueue.Free( handle ); } };
}
