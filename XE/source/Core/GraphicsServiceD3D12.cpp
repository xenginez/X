#include "GraphicsService.h"

#if GRAPHICS_API == GRAPHICS_D3D12

#include <mutex>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <D3DX12/d3dx12.h>

#include "Utils/Logger.h"
#include "Utils/Window.h"
#include "Utils/RefCount.h"

#include "CoreFramework.h"
#include "EventService.h"
#include "CacheService.h"
#include "ThreadService.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#undef ERROR

IMPLEMENT_META( XE::GraphicsService );

namespace
{
	enum class PassKind
	{
		NONE,
		RENDER,
		BUNDLE,
		COMPUTE,
	};

	DXGI_FORMAT Cast( XE::GraphicsTextureFormat format )
	{
		switch ( format )
		{
		case XE::GraphicsTextureFormat::UNDEFINED:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::R8UNORM:
			return DXGI_FORMAT_R8_UNORM;
		case XE::GraphicsTextureFormat::R8SNORM:
			return DXGI_FORMAT_R8_SNORM;
		case XE::GraphicsTextureFormat::R8UINT:
			return DXGI_FORMAT_R8_UINT;
		case XE::GraphicsTextureFormat::R8SINT:
			return DXGI_FORMAT_R8_SINT;
		case XE::GraphicsTextureFormat::R16UINT:
			return DXGI_FORMAT_R16_UINT;
		case XE::GraphicsTextureFormat::R16SINT:
			return DXGI_FORMAT_R16_SINT;
		case XE::GraphicsTextureFormat::R16FLOAT:
			return DXGI_FORMAT_R16_FLOAT;
		case XE::GraphicsTextureFormat::RG8UNORM:
			return DXGI_FORMAT_R8G8_UNORM;
		case XE::GraphicsTextureFormat::RG8SNORM:
			return DXGI_FORMAT_R8G8_SNORM;
		case XE::GraphicsTextureFormat::RG8UINT:
			return DXGI_FORMAT_R8G8_UINT;
		case XE::GraphicsTextureFormat::RG8SINT:
			return DXGI_FORMAT_R8G8_SINT;
		case XE::GraphicsTextureFormat::R32FLOAT:
			return DXGI_FORMAT_R32_FLOAT;
		case XE::GraphicsTextureFormat::R32UINT:
			return DXGI_FORMAT_R32_UINT;
		case XE::GraphicsTextureFormat::R32SINT:
			return DXGI_FORMAT_R32_SINT;
		case XE::GraphicsTextureFormat::RG16UINT:
			return DXGI_FORMAT_R16G16_UINT;
		case XE::GraphicsTextureFormat::RG16SINT:
			return DXGI_FORMAT_R16G16_SINT;
		case XE::GraphicsTextureFormat::RG16FLOAT:
			return DXGI_FORMAT_R16G16_FLOAT;
		case XE::GraphicsTextureFormat::RGBA8UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case XE::GraphicsTextureFormat::RGBA8UNORMSRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case XE::GraphicsTextureFormat::RGBA8SNORM:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case XE::GraphicsTextureFormat::RGBA8UINT:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case XE::GraphicsTextureFormat::RGBA8SINT:
			return DXGI_FORMAT_R8G8B8A8_SINT;
		case XE::GraphicsTextureFormat::BGRA8UNORM:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case XE::GraphicsTextureFormat::BGRA8UNORMSRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case XE::GraphicsTextureFormat::RGB10A2UNORM:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case XE::GraphicsTextureFormat::RG11B10UFLOAT:
			return DXGI_FORMAT_R11G11B10_FLOAT;
		case XE::GraphicsTextureFormat::RGB9E5UFLOAT:
			return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
		case XE::GraphicsTextureFormat::RG32FLOAT:
			return DXGI_FORMAT_R32G32_FLOAT;
		case XE::GraphicsTextureFormat::RG32UINT:
			return DXGI_FORMAT_R32G32_UINT;
		case XE::GraphicsTextureFormat::RG32SINT:
			return DXGI_FORMAT_R32G32_SINT;
		case XE::GraphicsTextureFormat::RGBA16UINT:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case XE::GraphicsTextureFormat::RGBA16SINT:
			return DXGI_FORMAT_R16G16B16A16_SINT;
		case XE::GraphicsTextureFormat::RGBA16FLOAT:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case XE::GraphicsTextureFormat::RGBA32FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case XE::GraphicsTextureFormat::RGBA32UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case XE::GraphicsTextureFormat::RGBA32SINT:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case XE::GraphicsTextureFormat::STENCIL8:
			return DXGI_FORMAT_R8_UINT;
		case XE::GraphicsTextureFormat::DEPTH16UNORM:
			return DXGI_FORMAT_R16_UNORM;
		case XE::GraphicsTextureFormat::DEPTH24PLUS:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case XE::GraphicsTextureFormat::DEPTH24PLUSSTENCIL8:
			return DXGI_FORMAT_R24G8_TYPELESS;
		case XE::GraphicsTextureFormat::DEPTH24UNORMSTENCIL8:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case XE::GraphicsTextureFormat::DEPTH32FLOAT:
			return DXGI_FORMAT_R32_FLOAT;
		case XE::GraphicsTextureFormat::DEPTH32FLOATSTENCIL8:
			return DXGI_FORMAT_R32G8X24_TYPELESS;
		case XE::GraphicsTextureFormat::BC1RGBAUNORM:
			return DXGI_FORMAT_BC1_UNORM;
		case XE::GraphicsTextureFormat::BC1RGBAUNORMSRGB:
			return DXGI_FORMAT_BC1_UNORM_SRGB;
		case XE::GraphicsTextureFormat::BC2RGBAUNORM:
			return DXGI_FORMAT_BC2_UNORM;
		case XE::GraphicsTextureFormat::BC2RGBAUNORMSRGB:
			return DXGI_FORMAT_BC2_UNORM_SRGB;
		case XE::GraphicsTextureFormat::BC3RGBAUNORM:
			return DXGI_FORMAT_BC3_UNORM;
		case XE::GraphicsTextureFormat::BC3RGBAUNORMSRGB:
			return DXGI_FORMAT_BC3_UNORM_SRGB;
		case XE::GraphicsTextureFormat::BC4RUNORM:
			return DXGI_FORMAT_BC4_UNORM;
		case XE::GraphicsTextureFormat::BC4RSNORM:
			return DXGI_FORMAT_BC4_SNORM;
		case XE::GraphicsTextureFormat::BC5RGUNORM:
			return DXGI_FORMAT_BC5_UNORM;
		case XE::GraphicsTextureFormat::BC5RGSNORM:
			return DXGI_FORMAT_BC5_SNORM;
		case XE::GraphicsTextureFormat::BC6HRGBUFLOAT:
			return DXGI_FORMAT_BC6H_UF16;
		case XE::GraphicsTextureFormat::BC6HRGBFLOAT:
			return DXGI_FORMAT_BC6H_SF16;
		case XE::GraphicsTextureFormat::BC7RGBAUNORM:
			return DXGI_FORMAT_BC7_UNORM;
		case XE::GraphicsTextureFormat::BC7RGBAUNORMSRGB:
			return DXGI_FORMAT_BC7_UNORM_SRGB;
		case XE::GraphicsTextureFormat::ETC2RGB8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGB8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGB8A1UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGB8A1UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGBA8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGBA8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACR11UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACR11SNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACRG11UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACRG11SNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC4X4UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC4X4UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X4UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X4UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X6UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X6UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X6UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X6UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x6UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x6UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x10UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x10UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X10UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X10UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X12UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X12UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}

		return DXGI_FORMAT_UNKNOWN;
	}
	D3D12_QUERY_TYPE Cast( XE::GraphicsQueryType type )
	{
		switch ( type )
		{
		case XE::GraphicsQueryType::OCCLUSION:
			return D3D12_QUERY_TYPE_OCCLUSION;
		case XE::GraphicsQueryType::PIPELINE_STATISTICS:
			return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
		case XE::GraphicsQueryType::TIMESTAMP:
			return D3D12_QUERY_TYPE_TIMESTAMP;
		}

		return D3D12_QUERY_TYPE_OCCLUSION;
	}
	D3D12_RESOURCE_FLAGS Cast( XE::GraphicsBufferUsageFlags usage )
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

		if ( usage || XE::GraphicsBufferUsage::STORAGE )
			flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		return flags;
	}
	template< typename T > XE::Handle< T > Cast( T * ptr )
	{
		return XE::HandleCast< T >( reinterpret_cast<XE::uint64>( ptr ) );
	}
	template< typename T > T * Cast( XE::Handle< T > handle )
	{
		return reinterpret_cast<T *>( handle.GetValue() );
	}

	template< typename T > class RefHandle : public XE::RefCount
	{
	public:
		operator bool() const
		{
			return Count();
		}

		operator XE::Handle< T >() const
		{
			return Handle;
		}

	public:
		XE::Handle< T > Handle;
	};

	template< typename T, XE::uint64 S > class RefHandleArray
	{
	public:
		T & Alloc()
		{
			auto handle = _Handles.Alloc();
			_Datas[handle.GetValue()].Inc();
			_Datas[handle.GetValue()].Handle = handle;
			return _Datas[handle.GetValue()];
		}

		void Free( XE::Handle< T > handle )
		{
			_Handles.Free( handle );
		}

		T & operator []( XE::uint64 idx )
		{
			return _Datas[idx];
		}

	public:
		std::array< T, S > _Datas;
		XE::QueueHandleAllocator< XE::Handle< T >, S > _Handles;
	};
}

namespace D3D12
{
	template< typename T > using ComPtr = Microsoft::WRL::ComPtr< T >;
	using AdapterPtr = ComPtr < IDXGIAdapter4 >;
	using DevicePtr = ComPtr < ID3D12Device >;
	using ResourcePtr = ComPtr < ID3D12Resource >;
	using CommandQueuePtr = ComPtr < ID3D12CommandQueue >;
	using CommandAllocatorPtr = ComPtr < ID3D12CommandAllocator >;
	using CommandListPtr = ComPtr < ID3D12CommandList >;
	using GraphicsCommandListPtr = ComPtr < ID3D12GraphicsCommandList >;
	using PipelineStatePtr = ComPtr < ID3D12PipelineState >;
	using SwapChainPtr = ComPtr < IDXGISwapChain4 >;
	using CommandSignaturePtr = ComPtr< ID3D12CommandSignature >;
	using RootSignaturePtr = ComPtr< ID3D12RootSignature >;
	using QueryHeapPtr = ComPtr< ID3D12QueryHeap >;
	using FactoryPtr = ComPtr< IDXGIFactory >;
	using DebugPtr = ComPtr< ID3D12Debug >;
}

namespace XE
{
	class GraphicsSurface : public RefHandle< XE::GraphicsSurface >
	{
	public:
		XE::GraphicsSurfaceDescriptor Desc;
	};
	class GraphicsSwapChain : public RefHandle< XE::GraphicsSwapChain >
	{
	public:
		XE::GraphicsSwapChainDescriptor Desc;

		D3D12::SwapChainPtr SwapChain = nullptr;
	};
	class GraphicsAdapter : public RefHandle< XE::GraphicsAdapter >
	{
	public:
		D3D12::AdapterPtr Adapter = nullptr;

		XE::GraphicsDeviceHandle Device;
	};
	class GraphicsDevice : public RefHandle< XE::GraphicsDevice >
	{
	public:
		XE::GraphicsDeviceDescriptor Desc;

		D3D12::DevicePtr Device = nullptr;
		XE::GraphicsAdapterHandle Adapter;
		XE::GraphicsQueueHandle Queue;
	};
	class GraphicsQueue : public RefHandle< XE::GraphicsQueue >
	{
	public:
		XE::GraphicsQueueDescriptor Desc;

		XE::GraphicsDeviceHandle Device;
		D3D12::CommandQueuePtr CommandQueue = nullptr;
	};
	class GraphicsBindGroup : public RefHandle< XE::GraphicsBindGroup >
	{
	public:
		XE::GraphicsBindGroupDescriptor Desc;

		D3D12::RootSignaturePtr RootSignature = nullptr;
		D3D12::CommandSignaturePtr CommandSignature = nullptr;
	};
	class GraphicsBindGroupLayout : public RefHandle< XE::GraphicsBindGroupLayout >
	{
	public:
		XE::GraphicsBindGroupLayoutDescriptor Desc;
	};
	class GraphicsBuffer : public RefHandle< XE::GraphicsBuffer >
	{
	public:
		XE::GraphicsBufferDescriptor Desc;

		D3D12_RANGE MapRange = {};
		D3D12::ResourcePtr Resource = nullptr;
	};
	class GraphicsCommandBuffer : public RefHandle< XE::GraphicsCommandBuffer >
	{
	public:
		XE::GraphicsCommandBufferDescriptor Desc;

		D3D12::GraphicsCommandListPtr CommandList = nullptr;
		D3D12::CommandAllocatorPtr CommandAllocator = nullptr;
	};
	class GraphicsCommandEncoder : public RefHandle< XE::GraphicsCommandEncoder >
	{
	public:
		XE::GraphicsCommandEncoderDescriptor Desc;

		XE::GraphicsCommandBufferHandle CommandBuffer;
	};
	class GraphicsComputePassEncoder : public RefHandle< XE::GraphicsComputePassEncoder >
	{
	public:
		XE::GraphicsComputePassDescriptor Desc;

		XE::GraphicsBindGroupHandle BindGroup;
		XE::GraphicsCommandEncoderHandle Encoder;
		XE::GraphicsComputePipelineHandle Pipeline;

		XE::uint64 PiplineStatisticsQueryIndex = 0;
		XE::GraphicsQuerySetHandle PiplineStatisticsQuery;
	};
	class GraphicsComputePipeline : public RefHandle< XE::GraphicsComputePipeline >
	{
	public:
		XE::GraphicsComputePipelineDescriptor Desc;

		XE::GraphicsDeviceHandle Device;
		XE::GraphicsPipelineLayoutHandle Layout;
		std::array< XE::GraphicsBindGroupHandle, 4 > BindGroups;
		
		D3D12::PipelineStatePtr PipelineState = nullptr;
	};
	class GraphicsPipelineLayout : public RefHandle< XE::GraphicsPipelineLayout >
	{
	public:
		XE::GraphicsPipelineLayoutDescriptor Desc;
	};
	class GraphicsQuerySet : public RefHandle< XE::GraphicsQuerySet >
	{
	public:
		XE::GraphicsQuerySetDescriptor Desc;

		D3D12::QueryHeapPtr QueryHeap = nullptr;
	};
	class GraphicsRenderBundle : public RefHandle< XE::GraphicsRenderBundle >
	{
	public:
		XE::GraphicsRenderBundleDescriptor Desc;
	};
	class GraphicsRenderBundleEncoder : public RefHandle< XE::GraphicsRenderBundleEncoder >
	{
	public:
		XE::GraphicsRenderBundleEncoderDescriptor Desc;

		XE::GraphicsCommandEncoderHandle Encoder;
	};
	class GraphicsRenderPassEncoder : public RefHandle< XE::GraphicsRenderPassEncoder >
	{
	public:
		XE::GraphicsRenderPassDescriptor Desc;

		XE::GraphicsCommandEncoderHandle Encoder;
	};
	class GraphicsRenderPipeline : public RefHandle< XE::GraphicsRenderPipeline >
	{
	public:
		XE::GraphicsRenderPipelineDescriptor Desc;

		D3D12::PipelineStatePtr PipelineState = nullptr;
	};
	class GraphicsSampler : public RefHandle< XE::GraphicsSampler >
	{
	public:
		XE::GraphicsSamplerDescriptor Desc;
	};
	class GraphicsShaderModule : public RefHandle< XE::GraphicsShaderModule >
	{
	public:
		XE::GraphicsShaderModuleDescriptor Desc;

		D3D12_SHADER_BYTECODE ShaderCode = {};
	};
	class GraphicsTexture : public RefHandle< XE::GraphicsTexture >
	{
	public:
		XE::GraphicsTextureDescriptor Desc;

		XE::GraphicsBufferHandle Buffer;
	};
	class GraphicsTextureView : public RefHandle< XE::GraphicsTextureView >
	{
	public:
		XE::GraphicsTextureViewDescriptor Desc;
	};
}

struct XE::GraphicsService::Private
{
	D3D12::DebugPtr _Debug;
	D3D12::FactoryPtr _Factory;

	RefHandleArray< XE::GraphicsSurface, XE::GRAPHICS_MAX_SURFACE > _Surfaces;
	RefHandleArray< XE::GraphicsSwapChain, XE::GRAPHICS_MAX_SWAP_CHAIN > _SwapChains;
	RefHandleArray< XE::GraphicsAdapter, XE::GRAPHICS_MAX_ADAPTER > _Adapters;
	RefHandleArray< XE::GraphicsDevice, XE::GRAPHICS_MAX_DEVICE > _Devices;
	RefHandleArray< XE::GraphicsQueue, XE::GRAPHICS_MAX_QUEUE > _Queues;
	RefHandleArray< XE::GraphicsBindGroup, XE::GRAPHICS_MAX_BIND_GROUP > _BindGroups;
	RefHandleArray< XE::GraphicsBindGroupLayout, XE::GRAPHICS_MAX_BIND_GROUP_LAYOUT > _BindGroupLayouts;
	RefHandleArray< XE::GraphicsBuffer, XE::GRAPHICS_MAX_BUFFER > _Buffers;
	RefHandleArray< XE::GraphicsCommandBuffer, XE::GRAPHICS_MAX_COMMAND_BUFFER > _CommandBuffers;
	RefHandleArray< XE::GraphicsCommandEncoder, XE::GRAPHICS_MAX_COMMAND_ENCODER > _CommandEncoders;
	RefHandleArray< XE::GraphicsComputePassEncoder, XE::GRAPHICS_MAX_COMPUTE_PASS_ENCODER > _ComputePassEncoders;
	RefHandleArray< XE::GraphicsComputePipeline, XE::GRAPHICS_MAX_COMPUTE_PIPELINE > _ComputePipelines;
	RefHandleArray< XE::GraphicsPipelineLayout, XE::GRAPHICS_MAX_PIPELINE_LAYOUT > _PipelineLayouts;
	RefHandleArray< XE::GraphicsQuerySet, XE::GRAPHICS_MAX_QUERY_SET > _QuerySets;
	RefHandleArray< XE::GraphicsRenderBundle, XE::GRAPHICS_MAX_RENDER_BUNDLE > _RenderBundles;
	RefHandleArray< XE::GraphicsRenderBundleEncoder, XE::GRAPHICS_MAX_RENDER_BUNDLE_ENCODER > _RenderBundleEncoders;
	RefHandleArray< XE::GraphicsRenderPassEncoder, XE::GRAPHICS_MAX_RENDER_PASS_ENCODER > _RenderPassEncoders;
	RefHandleArray< XE::GraphicsRenderPipeline, XE::GRAPHICS_MAX_RENDER_PIPELINE > _RenderPipelines;
	RefHandleArray< XE::GraphicsShaderModule, XE::GRAPHICS_MAX_SHADER_MODULE > _ShaderModules;
	RefHandleArray< XE::GraphicsSampler, XE::GRAPHICS_MAX_SAMPLER > _Samplers;
	RefHandleArray< XE::GraphicsTexture, XE::GRAPHICS_MAX_TEXTURE > _Textures;
	RefHandleArray< XE::GraphicsTextureView, XE::GRAPHICS_MAX_TEXTURE_VIEW > _TextureViews;
};

XE::GraphicsService::GraphicsService()
	:_p( XE::New< Private >() )
{

}

XE::GraphicsService::~GraphicsService()
{
	XE::Delete( _p );
}

void XE::GraphicsService::Prepare()
{
	XE::uint32 flags = 0;

	if ( GetFramework()->GetBool( CONFIG_GRAPHICS_DEBUG ) )
	{
		if ( SUCCEEDED( ::D3D12GetDebugInterface( IID_PPV_ARGS( _p->_Debug.GetAddressOf() ) ) ) )
		{
			_p->_Debug->EnableDebugLayer();
			flags = DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	if ( FAILED( ::CreateDXGIFactory2( flags, IID_PPV_ARGS( _p->_Factory.GetAddressOf() ) ) ) )
	{
		XE_ERROR( "Create DXGIFactory failed" );
	}
}

void XE::GraphicsService::Startup()
{

}

void XE::GraphicsService::Update()
{

}

void XE::GraphicsService::Clearup()
{
	_p->_Factory = nullptr;
	_p->_Debug = nullptr;
}

XE::GraphicsSurfaceHandle XE::GraphicsService::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	auto & surface = _p->_Surfaces.Alloc();

	surface.Desc = descriptor;

	// TODO: 

	return surface;
}

void XE::GraphicsService::ProcessEvents()
{

}

void XE::GraphicsService::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback )
{

}

XE::GraphicsBindGroupHandle XE::GraphicsService::DeviceCreateBindGroup( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & group = _p->_BindGroups.Alloc();

		group.Desc = descriptor;

		// TODO: 

		return group;
	}
	
	return {};
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::DeviceCreateBindGroupLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & layout = _p->_BindGroupLayouts.Alloc();

		layout.Desc = descriptor;

		// TODO: 

		return layout;
	}
	return {};
}

XE::GraphicsBufferHandle XE::GraphicsService::DeviceCreateBuffer( XE::GraphicsDeviceHandle device, const XE::GraphicsBufferDescriptor & descriptor )
{
	if( auto & dev = _p->_Devices[device] )
	{
		auto & buf = _p->_Buffers.Alloc();

		buf.Desc = descriptor;

		auto size = descriptor.Size;
		XE::uint32 align_mask = 0;
		if ( descriptor.Usage || XE::GraphicsBufferUsage::UNIFORM )
		{
			align_mask = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
			size = ( ( size - 1 ) | align_mask ) + 1;
		}

		CD3DX12_RESOURCE_DESC raw_desc
		( D3D12_RESOURCE_DIMENSION_BUFFER, 0, size, 1, 1, 1, Cast( XE::GraphicsTextureFormat::UNDEFINED ), 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, Cast( descriptor.Usage ) );

		bool is_cpu_read = descriptor.Usage || XE::GraphicsBufferUsage::MAP_READ;
		bool is_cpu_write = descriptor.Usage || XE::GraphicsBufferUsage::MAP_WRITE;

		CD3DX12_HEAP_PROPERTIES heap_properties(
			is_cpu_read ? D3D12_CPU_PAGE_PROPERTY_WRITE_BACK : ( is_cpu_write ? D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE : D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE ),
			D3D12_MEMORY_POOL_L1,
			0, 0 );

		if ( SUCCEEDED( dev.Device->CreateCommittedResource( &heap_properties, D3D12_HEAP_FLAG_NONE, &raw_desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( buf.Resource.GetAddressOf() ) ) ) )
		{
			return buf;
		}
		else
		{
			_p->_Buffers.Free( buf );
		}
	}

	return {};
}

XE::GraphicsCommandEncoderHandle XE::GraphicsService::DeviceCreateCommandEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & encoder = _p->_CommandEncoders.Alloc();

		encoder.Desc = descriptor;

		// TODO: 

		return encoder;
	}

	return {};
}

XE::GraphicsComputePipelineHandle XE::GraphicsService::DeviceCreateComputePipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & pipe = _p->_ComputePipelines.Alloc();

		pipe.Desc = descriptor;

		// TODO: 

		return pipe;
	}

	return {};
}

XE::GraphicsPipelineLayoutHandle XE::GraphicsService::DeviceCreatePipelineLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & layout = _p->_PipelineLayouts.Alloc();

		layout.Desc = descriptor;

		// TODO: 

		return layout;
	}

	return {};
}

XE::GraphicsQuerySetHandle XE::GraphicsService::DeviceCreateQuerySet( XE::GraphicsDeviceHandle device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & query_set = _p->_QuerySets.Alloc();

		query_set.Desc = descriptor;

		// TODO: 

		return query_set;
	}

	return {};
}

XE::GraphicsRenderBundleEncoderHandle XE::GraphicsService::DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & bundle = _p->_RenderBundleEncoders.Alloc();

		bundle.Desc = descriptor;

		// TODO: 

		return bundle;
	}

	return {};
}

XE::GraphicsRenderPipelineHandle XE::GraphicsService::DeviceCreateRenderPipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & pipe = _p->_RenderPipelines.Alloc();

		pipe.Desc = descriptor;

		// TODO: 

		return pipe;
	}

	return {};
}

XE::GraphicsSamplerHandle XE::GraphicsService::DeviceCreateSampler( XE::GraphicsDeviceHandle device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & sampler = _p->_Samplers.Alloc();

		sampler.Desc = descriptor;

		// TODO: 

		return sampler;
	}

	return {};
}

XE::GraphicsShaderModuleHandle XE::GraphicsService::DeviceCreateShaderModule( XE::GraphicsDeviceHandle device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & shader = _p->_ShaderModules.Alloc();

		shader.Desc = descriptor;

		// TODO: 

		return shader;
	}

	return {};
}

XE::GraphicsSwapChainHandle XE::GraphicsService::DeviceCreateSwapChain( XE::GraphicsDeviceHandle device, XE::GraphicsSurfaceHandle surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & swap_chain = _p->_SwapChains.Alloc();

		swap_chain.Desc = descriptor;

		// TODO: 

		return swap_chain;
	}

	return {};
}

XE::GraphicsTextureHandle XE::GraphicsService::DeviceCreateTexture( XE::GraphicsDeviceHandle device, const XE::GraphicsTextureDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & texture = _p->_Textures.Alloc();

		texture.Desc = descriptor;

		// TODO: 

		return texture;
	}

	return {};
}

XE::GraphicsTextureViewHandle XE::GraphicsService::TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	if ( auto & tex = _p->_Textures[texture] )
	{
		auto & view = _p->_TextureViews.Alloc();

		view.Desc = descriptor;

		// TODO: 

		return view;
	}

	return {};
}

void XE::GraphicsService::AdapterEnumerateFeatures( XE::GraphicsAdapterHandle adapter, XE::Array< XE::GraphicsFeatureName > & features )
{
	auto & ada = _p->_Adapters[adapter.GetValue()];
	if ( ada.Count() != 0 )
	{
		features.push_back( XE::GraphicsFeatureName::DEPTH_CLIP_CONTROL );
		features.push_back( XE::GraphicsFeatureName::DEPTH24UNORM_STENCIL8 );
		features.push_back( XE::GraphicsFeatureName::DEPTH32FLOAT_STENCIL8 );
		features.push_back( XE::GraphicsFeatureName::INDIRECT_FIRST_INSTANCE );
		features.push_back( XE::GraphicsFeatureName::TIMESTAMP_QUERY );
		features.push_back( XE::GraphicsFeatureName::PIPELINE_STATISTICS_QUERY );
		features.push_back( XE::GraphicsFeatureName::TEXTURE_COMPRESSION_BC );
	}
}

bool XE::GraphicsService::AdapterGetLimits( XE::GraphicsAdapterHandle adapter, XE::GraphicsSupportedLimits & limits )
{
	auto & ada = _p->_Adapters[adapter.GetValue()];
	if ( ada.Count() != 0 )
	{
		limits.Limits.MaxTextureDimension1D = D3D12_REQ_TEXTURE1D_U_DIMENSION;
		limits.Limits.MaxTextureDimension2D = D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION;
		limits.Limits.MaxTextureDimension3D = D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
		limits.Limits.MaxTextureArrayLayers = D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
		limits.Limits.MaxBindGroups = 4;
		limits.Limits.MaxDynamicUniformBuffersPerPipelineLayout = 8;
		limits.Limits.MaxDynamicStorageBuffersPerPipelineLayout = 4;
		limits.Limits.MaxSampledTexturesPerShaderStage = 16;
		limits.Limits.MaxSamplersPerShaderStage = D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE;
		limits.Limits.MaxStorageBuffersPerShaderStage = 8;
		limits.Limits.MaxStorageTexturesPerShaderStage = 8;
		limits.Limits.MaxUniformBuffersPerShaderStage = 12;
		limits.Limits.MaxUniformBufferBindingSize = D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * 16;
		limits.Limits.MaxStorageBufferBindingSize = 128 << 20;
		limits.Limits.MinUniformBufferOffsetAlignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
		limits.Limits.MinStorageBufferOffsetAlignment = 4;
		limits.Limits.MaxVertexBuffers = D3D12_VS_INPUT_REGISTER_COUNT;
		limits.Limits.MaxVertexAttributes = D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
		limits.Limits.MaxVertexBufferArrayStride = D3D12_SO_BUFFER_MAX_STRIDE_IN_BYTES;
		limits.Limits.MaxInterStageShaderComponents = 60;
		limits.Limits.MaxComputeWorkgroupStorageSize = 16352;
		limits.Limits.MaxComputeInvocationsPerWorkgroup = D3D12_CS_4_X_THREAD_GROUP_MAX_THREADS_PER_GROUP;
		limits.Limits.MaxComputeWorkgroupSizeX = D3D12_CS_THREAD_GROUP_MAX_X;
		limits.Limits.MaxComputeWorkgroupSizeY = D3D12_CS_THREAD_GROUP_MAX_Y;
		limits.Limits.MaxComputeWorkgroupSizeZ = D3D12_CS_THREAD_GROUP_MAX_Z;
		limits.Limits.MaxComputeWorkgroupsPerDimension = D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;

		return true;
	}

	return false;
}

void XE::GraphicsService::AdapterGetProperties( XE::GraphicsAdapterHandle adapter, XE::GraphicsAdapterProperties & properties )
{
	auto & ada = _p->_Adapters[adapter.GetValue()];
	if ( ada.Count() != 0 )
	{
		DXGI_ADAPTER_DESC1 desc1;
		if ( SUCCEEDED( ada.Adapter->GetDesc1( &desc1 ) ) )
		{
			properties.Name = XE::WideString( desc1.Description );
			properties.DriverDescription = properties.Name;
			properties.DeviceID = desc1.DeviceId;
			properties.VendorID = desc1.VendorId;
			properties.BackendType = XE::GraphicsBackendType::D3D12;

			if ( desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE )
			{
				properties.AdapterType = GraphicsAdapterType::SOFTWARE_CPU;
			}
			else
			{
				D3D12_FEATURE_DATA_ARCHITECTURE arch = {};
				if ( ada.Device && SUCCEEDED( _p->_Devices[ada.Device.GetValue()].Device->CheckFeatureSupport( D3D12_FEATURE_ARCHITECTURE, &arch, sizeof( arch ) ) ) )
				{
					properties.AdapterType = arch.UMA ? XE::GraphicsAdapterType::INTEGRATED_GPU : XE::GraphicsAdapterType::DISCRETE_GPU;
				}
				else
				{
					properties.AdapterType = XE::GraphicsAdapterType::UNKNOWN;
				}
			}
		}
	}
}

bool XE::GraphicsService::AdapterHasFeature( XE::GraphicsAdapterHandle adapter, XE::GraphicsFeatureName feature )
{
	XE::Array< XE::GraphicsFeatureName > features;
	AdapterEnumerateFeatures( adapter, features );
	return std::find( features.begin(), features.end(), feature ) != features.end();
}

void XE::GraphicsService::AdapterRequestDevice( XE::GraphicsAdapterHandle adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback )
{
	auto & ada = _p->_Adapters[adapter.GetValue()];
	if ( ada.Count() != 0 )
	{
		std::array< D3D_FEATURE_LEVEL, 4 > levels =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		for ( size_t i = 0; i < levels.size(); i++ )
		{
			auto & device = _p->_Devices.Alloc();
			if ( SUCCEEDED( ::D3D12CreateDevice( ada.Adapter.Get(), levels[i], IID_PPV_ARGS( device.Device.GetAddressOf() ) ) ) )
			{
				ada.Device = device.Handle;
			}
		}

		if ( ada.Device )
		{
			callback( XE::GraphicsRequestDeviceStatus::SUCCESS, ada.Device, "" );
		}
		else
		{
			callback( XE::GraphicsRequestDeviceStatus::ERROR, ada.Device, "" );
		}
	}
}

XE::Span< XE::uint8 > XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	auto & buf = _p->_Buffers[buffer.GetValue()];
	if ( buf.Count() != 0 )
	{
		buf.MapRange.Begin = offset;
		buf.MapRange.End = offset + size;

		void * data = nullptr;
		if ( SUCCEEDED( buf.Resource->Map( 0, &buf.MapRange, &data ) ) )
		{
			return { reinterpret_cast<XE::uint8 *>( data ), size };
		}
	}

	return {};
}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{
	auto & buf = _p->_Buffers[buffer.GetValue()];
	if( buf.Count() != 0 )
	{
		buf.Resource->Unmap( 0, &buf.MapRange );
	}
}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	if ( cmd.Count() != 0 )
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		if ( cmd_buf.Count() != 0 )
		{
			auto & pass = _p->_ComputePassEncoders.Alloc();

			pass.Desc = descriptor;
			pass.Encoder = command_encoder;

			cmd_buf.CommandList->BeginEvent( 0, pass.Desc.Label.c_str(), pass.Desc.Label.size() );

			return pass.Handle;
		}
	}

	return {};
}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		{
			auto & pass = _p->_RenderPassEncoders.Alloc();

			pass.Desc = descriptor;
			pass.Encoder = command_encoder;

			cmd_buf.CommandList->BeginEvent( 0, pass.Desc.Label.c_str(), pass.Desc.Label.size() );

			return pass.Handle;
		}
	}

	return {};
}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle src, XE::uint64 src_offset, XE::GraphicsBufferHandle dst, XE::uint64 dst_offset, XE::uint64 size )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & src_buf = _p->_Buffers[src.GetValue()];
		{
			auto & dst_buf = _p->_Buffers[dst.GetValue()];
			{
				auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
				{
					D3D12::GraphicsCommandListPtr list;
					if ( SUCCEEDED( cmd_buf.CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
					{
						list->CopyBufferRegion( dst_buf.Resource.Get(), dst_offset, src_buf.Resource.Get(), src_offset, size );
					}
				}
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyBuffer & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & src_buf = _p->_Buffers[src.Buffer.GetValue()];
		{
			auto & dst_tex = _p->_Textures[dst.Texture.GetValue()];
			{
				auto & dst_buf = _p->_Buffers[dst_tex.Buffer.GetValue()];
				{
					auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
					{
						D3D12::GraphicsCommandListPtr list;
						if ( SUCCEEDED( cmd_buf.CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
						{
							D3D12_PLACED_SUBRESOURCE_FOOTPRINT src_footprint;
							src_footprint.Offset = 0;
							src_footprint.Footprint.Format = Cast( dst_tex.Desc.Format );
							src_footprint.Footprint.Width = dst_tex.Desc.Size.x;
							src_footprint.Footprint.Height = dst_tex.Desc.Size.y;
							src_footprint.Footprint.Depth = dst_tex.Desc.Size.z;
							//src_footprint.Footprint.RowPitch = dst_tex->Desc.

							CD3DX12_TEXTURE_COPY_LOCATION src_location( src_buf.Resource.Get(), {} );

							CD3DX12_TEXTURE_COPY_LOCATION dst_location;

							D3D12_BOX box;

							box.left = dst.Origin.x;
							box.top = dst.Origin.y;
							box.right = dst.Origin.x + copy_size.x;
							box.bottom = dst.Origin.y + copy_size.y;
							box.front = dst.Origin.z;
							box.back = dst.Origin.z + copy_size.z;

							list->CopyTextureRegion( &dst_location, dst.Origin.x, dst.Origin.y, dst.Origin.z, &src_location, &box );
						}
					}
				}
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyBuffer & dst, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsService::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & src_tex = _p->_Textures[src.Texture.GetValue()];
		{
			auto & src_buf = _p->_Buffers[src_tex.Buffer.GetValue()];
			{
				auto & dst_tex = _p->_Textures[dst.Texture.GetValue()];
				{
					auto & dst_buf = _p->_Buffers[dst_tex.Buffer.GetValue()];
					{
						auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
						{
							D3D12::GraphicsCommandListPtr list;
							if ( SUCCEEDED( cmd_buf.CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
							{
								CD3DX12_TEXTURE_COPY_LOCATION src_location( src_buf.Resource.Get() );

								CD3DX12_TEXTURE_COPY_LOCATION dst_location( dst_buf.Resource.Get() );

								D3D12_BOX box;

								box.left = src.Origin.x;
								box.top = src.Origin.y;
								box.right = src.Origin.x + copy_size.x;
								box.bottom = src.Origin.y + copy_size.y;
								box.front = src.Origin.z;
								box.back = src.Origin.z + copy_size.z;

								list->CopyTextureRegion( &dst_location, dst.Origin.x, dst.Origin.y, dst.Origin.z, &src_location, &box );
							}
						}
					}
				}
			}
		}
	}
}

XE::GraphicsCommandBufferHandle XE::GraphicsService::CommandEncoderFinish( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & marker_label )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		{
			cmd_buf.CommandList->SetMarker( 0, marker_label.c_str(), marker_label.size() );
		}
	}
}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		{
			cmd_buf.CommandList->EndEvent();
		}
	}
}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & group_label )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		{
			cmd_buf.CommandList->BeginEvent( 0, group_label.c_str(), group_label.size() );
		}
	}
}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferHandle dst, XE::uint64 dst_offset )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		{
			auto & query = _p->_QuerySets[query_set.GetValue()];
			{
				auto & buf = _p->_Buffers[dst.GetValue()];
				{
					cmd_buf.CommandList->ResolveQueryData( query.QueryHeap.Get(), Cast( query.Desc.Type ), first_query, query_count, buf.Resource.Get(), dst_offset );
				}
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	auto & cmd = _p->_CommandEncoders[command_encoder.GetValue()];
	{
		auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
		{
			auto & query = _p->_QuerySets[query_set.GetValue()];
			{
				cmd_buf.CommandList->EndQuery( query.QueryHeap.Get(), D3D12_QUERY_TYPE_TIMESTAMP, query_index );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				auto & query = _p->_QuerySets[query_set.GetValue()];
				{
					pass.PiplineStatisticsQuery = query_set;
					pass.PiplineStatisticsQueryIndex = query_index;
					cmd_buf.CommandList->BeginQuery( query.QueryHeap.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, query_index );
				}
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				cmd_buf.CommandList->Dispatch( workgroup_count_x, workgroup_count_y, workgroup_count_z );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				auto & bind_group = _p->_BindGroups[pass.BindGroup.GetValue()];
				{
					auto & buf = _p->_Buffers[indirect_buffer.GetValue()];
					{
						cmd_buf.CommandList->ExecuteIndirect( bind_group.CommandSignature.Get(), 1, buf.Resource.Get(), indirect_offset, nullptr, 0 );
					}
				}
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				cmd_buf.CommandList->EndEvent();
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				auto & query = _p->_QuerySets[pass.PiplineStatisticsQuery.GetValue()];
				{
					cmd_buf.CommandList->EndQuery( query.QueryHeap.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, pass.PiplineStatisticsQueryIndex );
				}
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & marker_label )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				cmd_buf.CommandList->SetMarker( 0, marker_label.c_str(), marker_label.size() );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				cmd_buf.CommandList->EndEvent();
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & group_label )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		auto & cmd = _p->_CommandEncoders[pass.Encoder.GetValue()];
		{
			auto & cmd_buf = _p->_CommandBuffers[cmd.CommandBuffer.GetValue()];
			{
				cmd_buf.CommandList->BeginEvent( 0, group_label.c_str(), group_label.size() );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		pass.BindGroup = group;

		// TODO: 
	}
}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsComputePipelineHandle pipeline )
{
	auto & pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()];
	{
		pass.Pipeline = pipeline;
	}
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle compute_pipeline, XE::uint32 group_index )
{
	auto & pipe = _p->_ComputePipelines[compute_pipeline.GetValue()];
	{
		auto & bind = _p->_BindGroups[pipe.BindGroups[group_index].GetValue()];
		{
			return bind.Desc.Layout;
		}
	}

	return {};
}

void XE::GraphicsService::ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle compute_pipeline, const XE::String & label )
{
	auto & pipe = _p->_ComputePipelines[compute_pipeline.GetValue()];
	{
		pipe.Desc.Label = label;


	}
}

void XE::GraphicsService::DeviceEnumerateFeatures( XE::GraphicsDeviceHandle device, XE::Array< XE::GraphicsFeatureName > & features )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}
}

bool XE::GraphicsService::DeviceGetLimits( XE::GraphicsDeviceHandle device, XE::GraphicsSupportedLimits & limits )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}

	return {};
}

XE::GraphicsQueueHandle XE::GraphicsService::DeviceGetQueue( XE::GraphicsDeviceHandle device )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{
		return dev.Queue;
	}

	return {};
}

bool XE::GraphicsService::DeviceHasFeature( XE::GraphicsDeviceHandle device, XE::GraphicsFeatureName feature )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}

	return {};
}

bool XE::GraphicsService::DevicePopErrorScope( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}

	return {};
}

void XE::GraphicsService::DevicePushErrorScope( XE::GraphicsDeviceHandle device, XE::GraphicsErrorFilter filter )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}
}

void XE::GraphicsService::DeviceSetDeviceLostCallback( XE::GraphicsDeviceHandle device, DeviceLostCallback callback )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}
}

void XE::GraphicsService::DeviceSetUncapturedErrorCallback( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	auto & dev = _p->_Devices[device.GetValue()];
	{

	}
}

void XE::GraphicsService::QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback )
{
	auto & que = _p->_Queues[queue.GetValue()];
	{

	}
}

void XE::GraphicsService::QueueSubmit( XE::GraphicsQueueHandle queue, const XE::Array< XE::GraphicsCommandBufferHandle > & commands )
{
	auto & que = _p->_Queues[queue.GetValue()];
	{

	}
}

void XE::GraphicsService::QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{
	auto & que = _p->_Queues[queue.GetValue()];
	{

	}
}

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & dst, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{
	auto & que = _p->_Queues[queue.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

XE::GraphicsRenderBundleHandle XE::GraphicsService::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}

	return {};
}

void XE::GraphicsService::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & marker_label )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & group_label )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsRenderPipelineHandle pipeline )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 query_index )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Array< XE::GraphicsRenderBundleHandle > & bundles )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & marker_label )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & group_label )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Color & color )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsRenderPipelineHandle pipeline )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Recti & rect )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 reference )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder.GetValue()];
	{

	}
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle render_pipeline, XE::uint32 group_index )
{
	auto & pipe = _p->_RenderPipelines[render_pipeline.GetValue()];
	{

	}

	return {};
}

void XE::GraphicsService::RenderPipelineSetLabel( XE::GraphicsRenderPipelineHandle render_pipeline, const XE::String & label )
{
	auto & pipe = _p->_RenderPipelines[render_pipeline.GetValue()];
	{

	}
}

void XE::GraphicsService::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shader_module, CompilationInfoCallback callback )
{
	auto & shader = _p->_ShaderModules[shader_module.GetValue()];
	{

	}
}

void XE::GraphicsService::ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shader_module, const XE::String & label )
{
	auto & shader = _p->_ShaderModules[shader_module.GetValue()];
	{

	}
}

XE::GraphicsTextureFormat XE::GraphicsService::SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter )
{
	auto & sur = _p->_Surfaces[surface.GetValue()];
	{

	}

	return {};
}

XE::GraphicsTextureViewHandle XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swap_chain )
{
	auto & swap = _p->_SwapChains[swap_chain.GetValue()];
	{

	}

	return {};
}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainHandle swap_chain )
{
	auto & swap = _p->_SwapChains[swap_chain.GetValue()];
	{

	}
}

void XE::GraphicsService::DeviceDestroy( XE::GraphicsDeviceHandle device )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		if ( dev.Dec() == 0 )
		{
			// TODO: 

			_p->_Devices.Free( dev );
		}
	}
}

void XE::GraphicsService::AdapterDestroy( XE::GraphicsAdapterHandle adapter )
{
	if ( auto & ada = _p->_Adapters[adapter] )
	{
		if ( ada.Dec() == 0 )
		{
			// TODO: 

			_p->_Adapters.Free( ada );
		}
	}
}

void XE::GraphicsService::BindGroupDestroy( XE::GraphicsBindGroupHandle bind_group )
{
	if ( auto & group = _p->_BindGroups[bind_group] )
	{
		if ( group.Dec() == 0 )
		{
			// TODO: 

			_p->_BindGroups.Free( group );
		}
	}
}

void XE::GraphicsService::BindGroupLayoutDestroy( XE::GraphicsBindGroupLayoutHandle bind_group_layout )
{
	if ( auto & layout = _p->_BindGroupLayouts[bind_group_layout] )
	{
		if ( layout.Dec() == 0 )
		{
			// TODO: 

			_p->_BindGroupLayouts.Free( layout );
		}
	}
}

void XE::GraphicsService::BufferDestroy( XE::GraphicsBufferHandle buffer )
{
	if ( auto & buf = _p->_Buffers[buffer] )
	{
		if ( buf.Dec() == 0 )
		{
			// TODO: 

			_p->_Buffers.Free( buf );
		}
	}
}

void XE::GraphicsService::CommandBufferDestroy( XE::GraphicsCommandBufferHandle cmd_buf )
{
	if ( auto & cmd = _p->_CommandBuffers[cmd_buf] )
	{
		if ( cmd.Dec() == 0 )
		{
			// TODO: 

			_p->_CommandBuffers.Free( cmd );
		}
	}
}

void XE::GraphicsService::CommandEncoderDestroy( XE::GraphicsCommandEncoderHandle cmd_encoder )
{
	if ( auto & encoder = _p->_CommandEncoders[cmd_encoder] )
	{
		if ( encoder.Dec() == 0 )
		{
			// TODO: 

			_p->_CommandEncoders.Free( encoder );
		}
	}
}

void XE::GraphicsService::ComputePassEncoderDestroy( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( pass.Dec() == 0 )
		{
			// TODO: 

			_p->_ComputePassEncoders.Free( pass );
		}
	}
}

void XE::GraphicsService::ComputePipelineDestroy( XE::GraphicsComputePipelineHandle compute_pipeline )
{
	if ( auto & pipe = _p->_ComputePipelines[compute_pipeline] )
	{
		if ( pipe.Dec() == 0 )
		{
			// TODO: 

			_p->_ComputePipelines.Free( pipe );
		}
	}
}

void XE::GraphicsService::PipelineLayoutDestroy( XE::GraphicsPipelineLayoutHandle pipeline_layout )
{
	if ( auto & layout = _p->_PipelineLayouts[pipeline_layout] )
	{
		if ( layout.Dec() == 0 )
		{
			// TODO: 

			_p->_PipelineLayouts.Free( layout );
		}
	}
}

void XE::GraphicsService::QuerySetDestroy( XE::GraphicsQuerySetHandle query_set )
{
	if ( auto & set = _p->_QuerySets[query_set] )
	{
		if ( set.Dec() == 0 )
		{
			// TODO: 

			_p->_QuerySets.Free( set );
		}
	}
}

void XE::GraphicsService::QueueDestroy( XE::GraphicsQueueHandle queue )
{
	if ( auto & q = _p->_Queues[queue] )
	{
		if ( q.Dec() == 0 )
		{
			// TODO: 

			_p->_Queues.Free( q );
		}
	}
}

void XE::GraphicsService::RenderBundleDestroy( XE::GraphicsRenderBundleHandle render_bundle )
{
	if ( auto & bundle = _p->_RenderBundles[render_bundle] )
	{
		if ( bundle.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderBundles.Free( bundle );
		}
	}
}

void XE::GraphicsService::RenderBundleEncoderDestroy( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder )
{
	if ( auto & encoder = _p->_RenderBundleEncoders[render_bundle_encoder] )
	{
		if ( encoder.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderBundleEncoders.Free( encoder );
		}
	}
}

void XE::GraphicsService::RenderPassEncoderDestroy( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	if ( auto & pass = _p->_RenderPassEncoders[render_pass_encoder] )
	{
		if ( pass.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderPassEncoders.Free( pass );
		}
	}
}

void XE::GraphicsService::RenderPipelineDestroy( XE::GraphicsRenderPipelineHandle render_pipeline )
{
	if ( auto & pipe = _p->_RenderPipelines[render_pipeline] )
	{
		if ( pipe.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderPipelines.Free( pipe );
		}
	}
}

void XE::GraphicsService::SamplerDestroy( XE::GraphicsSamplerHandle sampler )
{
	if ( auto & sam = _p->_Samplers[sampler] )
	{
		if ( sam.Dec() == 0 )
		{
			// TODO: 

			_p->_Samplers.Free( sam );
		}
	}
}

void XE::GraphicsService::ShaderModuleDestroy( XE::GraphicsShaderModuleHandle shader_module )
{
	if ( auto & shader = _p->_ShaderModules[shader_module] )
	{
		if ( shader.Dec() == 0 )
		{
			// TODO: 

			_p->_ShaderModules.Free( shader );
		}
	}
}

void XE::GraphicsService::SurfaceDestroy( XE::GraphicsSurfaceHandle surface )
{
	if ( auto & sur = _p->_Surfaces[surface] )
	{
		if ( sur.Dec() == 0 )
		{
			// TODO: 

			_p->_Surfaces.Free( sur );
		}
	}
}

void XE::GraphicsService::SwapChainDestroy( XE::GraphicsSwapChainHandle swap_chain )
{
	if ( auto & swap = _p->_SwapChains[swap_chain] )
	{
		if ( swap.Dec() == 0 )
		{
			// TODO: 

			_p->_SwapChains.Free( swap );
		}
	}
}

void XE::GraphicsService::TextureDestroy( XE::GraphicsTextureHandle texture )
{
	if ( auto & tex = _p->_Textures[texture] )
	{
		if ( tex.Dec() == 0 )
		{
			// TODO: 

			_p->_Textures.Free( tex );
		}
	}
}

void XE::GraphicsService::TextureViewDestroy( XE::GraphicsTextureViewHandle texture_view )
{
	if ( auto & view = _p->_TextureViews[texture_view] )
	{
		if ( view.Dec() == 0 )
		{
			// TODO: 

			_p->_TextureViews.Free( view );
		}
	}
}

#endif // GRAPHICS_API == GRAPHICS_D3D12