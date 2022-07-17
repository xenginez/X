#include "RenderService.h"

#include "Core/GraphicsService.h"

#include "RenderGraph.h"
#include "RenderTexture.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"

IMPLEMENT_META( XE::RenderService );

namespace
{
	static XE::uint64 RenderTextureTemporary_Index = 0;

	class RenderTextureTemporary
	{
	public:
		RenderTextureTemporary( XE::int32 width, XE::int32 height, const XE::GraphicsServicePtr & service, const XE::GraphicsDevicePtr & device )
		{
			Desc.Label = "RenderTextureTemporary_" + XE::ToString( RenderTextureTemporary_Index++ );
			Desc.Size.x = ALIGNED( width, 512 );
			Desc.Size.y = ALIGNED( height, 512 );
			Desc.Size.z = 1;
			Desc.Dimension = XE::GraphicsTextureDimension::D2;
			Desc.Format = XE::GraphicsTextureFormat::RGBA8SNORM;
			Desc.SampleCount = 1;
			Desc.MipLevelCount = 1;
			Desc.Usage = XE::MakeFlags( XE::GraphicsTextureUsage::COPY_SRC, XE::GraphicsTextureUsage::COPY_DST, XE::GraphicsTextureUsage::TEXTURE_BINDING, XE::GraphicsTextureUsage::STORAGE_BINDING, XE::GraphicsTextureUsage::RENDER_ATTACHMENT );

			Texture = service->DeviceCreateTexture( device, Desc );
		}

	public:
		XE::GraphicsTexturePtr Texture;
		XE::GraphicsTextureDescriptor Desc;
	};
	using RenderTextureTemporaryPtr = XE::SharedPtr< RenderTextureTemporary >;
}

struct XE::RenderService::Private
{
	XE::RenderTexturePtr _MainTexture;
	XE::GraphicsQueuePtr _GraphicsQueue;
	XE::GraphicsDevicePtr _GraphicsDevice;
	XE::GraphicsSurfacePtr _GraphicsSurface;
	XE::GraphicsSwapChainPtr _GraphicsSwapChain;

	std::mutex _TexturePoolMutex;
	XE::List< RenderTextureTemporaryPtr > _TexturePool;

	XE::RenderGraphPtr _DefaultGraph;
	XE::CameraComponentPtr _MainCamera;
	XE::Array< XE::CameraComponentPtr > _Cameras;
	XE::QueueHandleAllocator< XE::Handle< XE::CameraComponent > > _CameraQueue;
	XE::OCTree< XE::LightComponentPtr > _StaticLights;
	XE::Array< XE::LightComponentPtr > _DynamicLights;
	XE::QueueHandleAllocator< XE::Handle< XE::LightComponent > > _DynamicLightQueue;
	XE::OCTree< XE::RenderComponentPtr > _StaticRenders;
	XE::Array< XE::RenderComponentPtr > _DynamicRenders;
	XE::QueueHandleAllocator< XE::Handle< XE::RenderComponent > > _DynamicRenderQueue;
};

XE::RenderService::RenderService()
	: _p( XE::New< Private >() )
{

}

XE::RenderService::~RenderService()
{
	XE::Delete( _p );
}

void XE::RenderService::Prepare()
{

}

void XE::RenderService::Startup()
{

}

void XE::RenderService::Update()
{
	XE::Array< XE::GraphicsCommandBufferPtr > command_buffers;
	if ( auto graphics = GetFramework()->GetServiceT< XE::GraphicsService >() )
	{
		_p->_MainTexture->ResetTextureView( graphics->SwapChainGetCurrentTextureView( _p->_GraphicsSwapChain ) );

		if ( _p->_MainCamera )
		{
			_p->_MainCamera->SetRenderTexture( _p->_MainTexture );

			XE::RenderExecutor executor;

			_p->_MainCamera->Render( executor );

			executor.Submit();
		}

		for ( const auto & it : _p->_Cameras )
		{
			if ( it->GetRenderTexture() != nullptr )
			{
				XE::RenderExecutor executor;

				it->Render( executor );

				executor.Submit();
			}
		}
	}
}

void XE::RenderService::Clearup()
{
	_p->_MainCamera = nullptr;
	_p->_DefaultGraph = nullptr;

	_p->_Cameras.clear();
	_p->_CameraQueue.Reset();

	_p->_StaticLights.Clear();
	_p->_DynamicLights.clear();
	_p->_DynamicLightQueue.Reset();

	_p->_StaticRenders.Clear();
	_p->_DynamicRenders.clear();
	_p->_DynamicRenderQueue.Reset();

	_p->_GraphicsQueue = nullptr;
	_p->_GraphicsDevice = nullptr;
	_p->_GraphicsSurface = nullptr;
	_p->_GraphicsSwapChain = nullptr;
}

const XE::CameraComponentPtr & XE::RenderService::GetMainCamera() const
{
	return _p->_MainCamera;
}

void XE::RenderService::SetMainCamera( const XE::CameraComponentPtr & val )
{
	_p->_MainCamera = val;
}

const XE::RenderGraphPtr & XE::RenderService::GetDefaultRenderGraph() const
{
	return _p->_DefaultGraph;
}

void XE::RenderService::SetDefaultRenderGraph( const XE::RenderGraphPtr & val )
{
	_p->_DefaultGraph = val;
}

XE::RenderResourcePtr XE::RenderService::CreateResource( const XE::RenderGraphPtr & val )
{
	XE::RenderResourcePtr resource = XE::MakeShared< XE::RenderResource >();
	{
		XE::RenderBuilder builder;

		auto vertices = val->GetGraphPass().vertices();
		for ( auto it = vertices.first; it != vertices.second; ++it )
		{
			resource->_PassDataMap[it->get()] = builder.Compile( *it );
		}
	}
	return resource;
}

XE::RenderTexturePtr XE::RenderService::GetRenderTextureFromPool( XE::int32 width, XE::int32 height, XE::GraphicsTextureFormat format )
{
	if ( auto service = GetFramework()->GetServiceT< XE::GraphicsService >() )
	{
		RenderTextureTemporaryPtr temporary;

		{
			std::unique_lock< std::mutex > lock( _p->_TexturePoolMutex );

			for ( auto it = _p->_TexturePool.begin(); it != _p->_TexturePool.end(); ++it )
			{
				auto tex = *it;

				if ( tex->Desc.Size.x >= width && tex->Desc.Size.y >= height && tex->Desc.Format == format )
				{
					temporary = tex;

					_p->_TexturePool.erase( it );

					break;
				}
			}
		}

		if ( temporary == nullptr )
		{
			temporary = XE::MakeShared< RenderTextureTemporary >( width, height, format, service, _p->_GraphicsDevice );
		}

		XE::GraphicsTextureViewDescriptor desc = {};
		{
			desc.Label = temporary->Desc.Label + "_View";
			desc.Format = format;
			desc.Aspect = GraphicsTextureAspect::ALL;
			desc.BaseMipLevel = 0;
			desc.BaseArrayLayer = 0;
			desc.MipLevelCount = 1;
			desc.ArrayLayerCount = 1;
			desc.Dimension = GraphicsTextureViewDimension::D2;
		}
		auto view = service->TextureCreateView( temporary->Texture, desc );

		if ( view != nullptr )
		{
			auto alloc = XE::AllocatorProxy< XE::RenderTexture >::GetAllocator();
			auto tex = alloc.allocate( 1 ); alloc.construct( tex );

			tex->ResetTextureView( view );

			return XE::RenderTexturePtr( tex, [this, temporary]( XE::RenderTexture * val )
			{
				auto alloc = XE::AllocatorProxy< XE::RenderTexture >::GetAllocator();
				alloc.destroy( val ); alloc.deallocate( val, 1 );

				std::unique_lock< std::mutex > lock( _p->_TexturePoolMutex );
				_p->_TexturePool.push_back( temporary );
			} );
		}
	}

	return nullptr;
}

XE::Disposable XE::RenderService::RegisterLight( const XE::LightComponentPtr & val )
{
	if ( val->GetDynamic() )
	{
		auto handle = _p->_DynamicLightQueue.Alloc();

		if ( _p->_DynamicLights.size() <= handle.GetValue() ) _p->_DynamicLights.resize( handle.GetValue() + 1 );

		_p->_DynamicLights[handle.GetValue()] = val;

		return { [this, handle]() { _p->_DynamicLights[handle.GetValue()] = nullptr; _p->_DynamicLightQueue.Free( handle ); } };
	}
	else
	{
		_p->_StaticLights.Insert( val );

		return { [this, val]() { _p->_StaticLights.Remove( val ); } };
	}
}

XE::Disposable XE::RenderService::RegisterRender( const XE::RenderComponentPtr & val )
{
	if ( val->GetDynamic() )
	{
		auto handle = _p->_DynamicRenderQueue.Alloc();

		if ( _p->_DynamicRenders.size() <= handle.GetValue() ) _p->_DynamicRenders.resize( handle.GetValue() + 1 );

		_p->_DynamicRenders[handle.GetValue()] = val;

		return { [this, handle]() { _p->_DynamicRenders[handle.GetValue()] = nullptr; _p->_DynamicRenderQueue.Free( handle ); } };
	}
	else
	{
		_p->_StaticRenders.Insert( val );

		return { [this, val]() { _p->_StaticRenders.Remove( val ); } };
	}
}

XE::Disposable XE::RenderService::RegisterCamera( const XE::CameraComponentPtr & val )
{
	auto handle = _p->_CameraQueue.Alloc();

	if ( _p->_Cameras.size() <= handle.GetValue() ) _p->_Cameras.resize( handle.GetValue() + 1 );

	_p->_Cameras[handle.GetValue()] = val;

	if ( val->GetRenderGraph() == nullptr )
	{
		val->SetRenderGraph( _p->_DefaultGraph );
	}

	return { [this, handle]() { _p->_Cameras[handle.GetValue()] = nullptr; _p->_CameraQueue.Free( handle ); } };
}
