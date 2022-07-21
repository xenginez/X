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
	static XE::uint64 TEMPORARY_INDEX = 0;
	static constexpr XE::uint64 TEMPORARY_POOL_MAX_SIZE = 32;
	static constexpr XE::uint64 GRAPHICS_FRAME_SYNC_SIZE = 2;

	class RenderTextureTemporary : public XE::EnableSharedFromThis< RenderTextureTemporary >
	{
	public:
		RenderTextureTemporary( XE::int32 width, XE::int32 height, const XE::GraphicsServicePtr & service, const XE::GraphicsDevicePtr & device )
		{
			Desc.Label = "RenderTextureTemporary_" + XE::ToString( TEMPORARY_INDEX++ );
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
	XE::uint64 _FrameIndex = 0;
	XE::GraphicsQueuePtr _GraphicsQueue;
	XE::GraphicsDevicePtr _GraphicsDevice;
	XE::GraphicsSurfacePtr _GraphicsSurface;
	XE::GraphicsSwapChainPtr _GraphicsSwapChain;

	std::mutex _TexturePoolMutex;
	XE::List< RenderTextureTemporaryPtr > _TexturePool;
	XE::Map< XE::String, RenderTextureTemporaryPtr > _GlobalTexture;

	XE::Array< XE::CameraData * > _Cameras;
	XE::OCTree< XE::LightData * > _StaticLights;
	XE::Array< XE::LightData * > _DynamicLights;
	XE::OCTree< XE::RenderData * > _StaticRenders;
	XE::Array< XE::RenderData * > _DynamicRenders;
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
	if ( auto graphics = GetFramework()->GetServiceT< XE::GraphicsService >() )
	{
		XE::GraphicsSurfaceDescriptor surface_desc = {};
		{
			surface_desc.Label = "MainWindow";
			surface_desc.Window = GetFramework()->GetMainWindow();
		}
		_p->_GraphicsSurface = graphics->CreateSurface( surface_desc );

		XE::GraphicsRequestAdapterOptions adapter_options = {};
		{
			adapter_options.CompatibleSurface = _p->_GraphicsSurface;
			adapter_options.ForceFallbackAdapter = false;
			adapter_options.PowerPreference = GraphicsPowerPreference::HIGH_PERFORMANCE;
		}
		graphics->RequestAdapter( adapter_options, [&]( XE::GraphicsRequestAdapterStatus status, XE::GraphicsAdapterPtr adapter )
		{
			if ( status == XE::GraphicsRequestAdapterStatus::SUCCESS )
			{
				XE::GraphicsDeviceDescriptor desc = {};
				{
					desc.Label = "MainWindowDevice";
					desc.DefaultQueue.Label = "MainWindowCommandQueue";
					//desc.RequiredLimits.Limits.
				}
				graphics->AdapterRequestDevice( adapter, desc, [&]( XE::GraphicsRequestDeviceStatus status, XE::GraphicsDevicePtr device )
				{
					if ( status == XE::GraphicsRequestDeviceStatus::SUCCESS )
					{
						_p->_GraphicsDevice = device;
						_p->_GraphicsQueue = graphics->DeviceGetQueue( device );
					}
				} );
			}
		} );

		XE::GraphicsSwapChainDescriptor swap_desc = {};
		{
			auto size = GetFramework()->GetMainWindow()->GetWindowSize();

			swap_desc.Label = "MainWindowSwapChain";
			swap_desc.Width = size.first;
			swap_desc.Height = size.second;
			swap_desc.Format = GraphicsTextureFormat::RGBA8SNORM;
			swap_desc.PresentMode = GraphicsPresentMode::IMMEDIATE;
			swap_desc.Usage = XE::MakeFlags( XE::GraphicsTextureUsage::RENDER_ATTACHMENT );
		}
		_p->_GraphicsSwapChain = graphics->DeviceCreateSwapChain( _p->_GraphicsDevice, _p->_GraphicsSurface, swap_desc );
	}
}

void XE::RenderService::Update()
{
	if ( auto graphics = GetFramework()->GetServiceT< XE::GraphicsService >() )
	{
		for ( const auto & it : _p->_Cameras )
		{
			if ( it != nullptr )
			{
				XE::RenderExecutor executor;

				it->RenderCallback( executor );

				executor.Submit();
			}
		}

		graphics->SwapChainPresent( _p->_GraphicsSwapChain );

		if ( ++_p->_FrameIndex == GRAPHICS_FRAME_SYNC_SIZE )
		{
			graphics->QueueOnSubmittedWorkDone( _p->_GraphicsQueue, nullptr );
			_p->_FrameIndex = 0;
		}
	}
}

void XE::RenderService::Clearup()
{
	_p->_FrameIndex = 0;
	TEMPORARY_INDEX = 0;

	_p->_Cameras.clear();

	_p->_StaticLights.Clear();
	_p->_DynamicLights.clear();

	_p->_StaticRenders.Clear();
	_p->_DynamicRenders.clear();

	_p->_GraphicsQueue = nullptr;
	_p->_GraphicsDevice = nullptr;
	_p->_GraphicsSurface = nullptr;
	_p->_GraphicsSwapChain = nullptr;
}

XE::RenderTexturePtr XE::RenderService::GetCurrentMainTexture() const
{
	auto texture = XE::MakeShared< XE::RenderTexture >();

	auto size = GetFramework()->GetMainWindow()->GetWindowSize();

	texture->ResetTextureView( size.first, size.second, 1, XE::GraphicsTextureFormat::RGBA8SNORM, XE::GraphicsTextureDimension::D2, GetFramework()->GetServiceT< XE::GraphicsService >()->SwapChainGetCurrentTextureView( _p->_GraphicsSwapChain ) );

	return texture;
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
			temporary = XE::MakeShared< RenderTextureTemporary >( width, height, service, _p->_GraphicsDevice );
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

			tex->ResetTextureView( width, height, 1, format, XE::GraphicsTextureDimension::D2, view );

			return XE::RenderTexturePtr( tex, [this, temporary]( XE::RenderTexture * val )
			{
				std::unique_lock< std::mutex > lock( _p->_TexturePoolMutex );

				if ( _p->_TexturePool.size() < TEMPORARY_POOL_MAX_SIZE )
				{
					_p->_TexturePool.push_back( temporary );
				}

				auto alloc = XE::AllocatorProxy< XE::RenderTexture >::GetAllocator();
				alloc.destroy( val ); alloc.deallocate( val, 1 );
			} );
		}
	}

	return nullptr;
}

XE::Disposable XE::RenderService::RegisterLight( XE::LightData * val )
{
	if ( val->IsDynamic )
	{
		auto it = std::find( _p->_DynamicLights.begin(), _p->_DynamicLights.end(), nullptr );
		if (it != _p->_DynamicLights.end() )
		{
			*it = val;
		}
		else
		{
			_p->_DynamicLights.push_back( val );
		}

		return { [this, val]()
		{
			auto it = std::find( _p->_DynamicLights.begin(), _p->_DynamicLights.end(), val );
			if ( it != _p->_DynamicLights.end() )
			{
				*it = nullptr;
			}
		} };
	}
	else
	{
		_p->_StaticLights.Insert( val );

		return { [this, val]() { _p->_StaticLights.Remove( val ); } };
	}
}

XE::Disposable XE::RenderService::RegisterRender( XE::RenderData * val )
{
	if ( val->IsDynamic )
	{
		auto it = std::find( _p->_DynamicRenders.begin(), _p->_DynamicRenders.end(), nullptr );
		if ( it != _p->_DynamicRenders.end() )
		{
			*it = val;
		}
		else
		{
			_p->_DynamicRenders.push_back( val );
		}

		return { [this, val]()
		{
			auto it = std::find( _p->_DynamicRenders.begin(), _p->_DynamicRenders.end(), val );
			if ( it != _p->_DynamicRenders.end() )
			{
				*it = nullptr;
			}
		} };
	}
	else
	{
		_p->_StaticRenders.Insert( val );

		return { [this, val]() { _p->_StaticRenders.Remove( val ); } };
	}
}

XE::Disposable XE::RenderService::RegisterCamera( XE::CameraData * val )
{
	auto it = std::find( _p->_Cameras.begin(), _p->_Cameras.end(), nullptr );
	if ( it != _p->_Cameras.end() )
	{
		*it = val;
	}
	else
	{
		it = _p->_Cameras.insert( _p->_Cameras.end(), val );
	}

	std::sort( _p->_Cameras.begin(), _p->_Cameras.end(), []( const auto & left, const auto & right ) { return left->Depth < right->Depth; } );

	return { [this, val]()
	{
		auto it = std::find( _p->_Cameras.begin(), _p->_Cameras.end(), val );
		if ( it != _p->_Cameras.end() )
		{
			*it = nullptr;
		}
	} };
}
