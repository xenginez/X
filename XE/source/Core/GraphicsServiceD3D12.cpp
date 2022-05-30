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
	DXGI_FORMAT Cast( XE::GraphicsTextureFormat format )
	{
		switch ( format )
		{
		case XE::GraphicsTextureFormat::UNDEFINED:
			break;
		case XE::GraphicsTextureFormat::R8UNORM:
			break;
		case XE::GraphicsTextureFormat::R8SNORM:
			break;
		case XE::GraphicsTextureFormat::R8UINT:
			break;
		case XE::GraphicsTextureFormat::R8SINT:
			break;
		case XE::GraphicsTextureFormat::R16UINT:
			break;
		case XE::GraphicsTextureFormat::R16SINT:
			break;
		case XE::GraphicsTextureFormat::R16FLOAT:
			break;
		case XE::GraphicsTextureFormat::RG8UNORM:
			break;
		case XE::GraphicsTextureFormat::RG8SNORM:
			break;
		case XE::GraphicsTextureFormat::RG8UINT:
			break;
		case XE::GraphicsTextureFormat::RG8SINT:
			break;
		case XE::GraphicsTextureFormat::R32FLOAT:
			break;
		case XE::GraphicsTextureFormat::R32UINT:
			break;
		case XE::GraphicsTextureFormat::R32SINT:
			break;
		case XE::GraphicsTextureFormat::RG16UINT:
			break;
		case XE::GraphicsTextureFormat::RG16SINT:
			break;
		case XE::GraphicsTextureFormat::RG16FLOAT:
			break;
		case XE::GraphicsTextureFormat::RGBA8UNORM:
			break;
		case XE::GraphicsTextureFormat::RGBA8UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::RGBA8SNORM:
			break;
		case XE::GraphicsTextureFormat::RGBA8UINT:
			break;
		case XE::GraphicsTextureFormat::RGBA8SINT:
			break;
		case XE::GraphicsTextureFormat::BGRA8UNORM:
			break;
		case XE::GraphicsTextureFormat::BGRA8UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::RGB10A2UNORM:
			break;
		case XE::GraphicsTextureFormat::RG11B10UFLOAT:
			break;
		case XE::GraphicsTextureFormat::RGB9E5UFLOAT:
			break;
		case XE::GraphicsTextureFormat::RG32FLOAT:
			break;
		case XE::GraphicsTextureFormat::RG32UINT:
			break;
		case XE::GraphicsTextureFormat::RG32SINT:
			break;
		case XE::GraphicsTextureFormat::RGBA16UINT:
			break;
		case XE::GraphicsTextureFormat::RGBA16SINT:
			break;
		case XE::GraphicsTextureFormat::RGBA16FLOAT:
			break;
		case XE::GraphicsTextureFormat::RGBA32FLOAT:
			break;
		case XE::GraphicsTextureFormat::RGBA32UINT:
			break;
		case XE::GraphicsTextureFormat::RGBA32SINT:
			break;
		case XE::GraphicsTextureFormat::STENCIL8:
			break;
		case XE::GraphicsTextureFormat::DEPTH16UNORM:
			break;
		case XE::GraphicsTextureFormat::DEPTH24PLUS:
			break;
		case XE::GraphicsTextureFormat::DEPTH24PLUSSTENCIL8:
			break;
		case XE::GraphicsTextureFormat::DEPTH24UNORMSTENCIL8:
			break;
		case XE::GraphicsTextureFormat::DEPTH32FLOAT:
			break;
		case XE::GraphicsTextureFormat::DEPTH32FLOATSTENCIL8:
			break;
		case XE::GraphicsTextureFormat::BC1RGBAUNORM:
			break;
		case XE::GraphicsTextureFormat::BC1RGBAUNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::BC2RGBAUNORM:
			break;
		case XE::GraphicsTextureFormat::BC2RGBAUNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::BC3RGBAUNORM:
			break;
		case XE::GraphicsTextureFormat::BC3RGBAUNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::BC4RUNORM:
			break;
		case XE::GraphicsTextureFormat::BC4RSNORM:
			break;
		case XE::GraphicsTextureFormat::BC5RGUNORM:
			break;
		case XE::GraphicsTextureFormat::BC5RGSNORM:
			break;
		case XE::GraphicsTextureFormat::BC6HRGBUFLOAT:
			break;
		case XE::GraphicsTextureFormat::BC6HRGBFLOAT:
			break;
		case XE::GraphicsTextureFormat::BC7RGBAUNORM:
			break;
		case XE::GraphicsTextureFormat::BC7RGBAUNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ETC2RGB8UNORM:
			break;
		case XE::GraphicsTextureFormat::ETC2RGB8UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ETC2RGB8A1UNORM:
			break;
		case XE::GraphicsTextureFormat::ETC2RGB8A1UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ETC2RGBA8UNORM:
			break;
		case XE::GraphicsTextureFormat::ETC2RGBA8UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::EACR11UNORM:
			break;
		case XE::GraphicsTextureFormat::EACR11SNORM:
			break;
		case XE::GraphicsTextureFormat::EACRG11UNORM:
			break;
		case XE::GraphicsTextureFormat::EACRG11SNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC4X4UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC4X4UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC5X4UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC5X4UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC5X5UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC5X5UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC6X5UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC6X5UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC6X6UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC6X6UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC8X5UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC8X5UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC8X6UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC8X6UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC8X8UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC8X8UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC10x5UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC10x5UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC10x6UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC10x6UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC10x8UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC10x8UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC10x10UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC10x10UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC12X10UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC12X10UNORMSRGB:
			break;
		case XE::GraphicsTextureFormat::ASTC12X12UNORM:
			break;
		case XE::GraphicsTextureFormat::ASTC12X12UNORMSRGB:
			break;
		default:
			break;
		}
		return DXGI_FORMAT_A8P8;
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
	DECL_PTR( GraphicsInstance );
	DECL_PTR( GraphicsAdapter );
	DECL_PTR( GraphicsDevice );
	DECL_PTR( GraphicsBindGroup );
	DECL_PTR( GraphicsBindGroupLayout );
	DECL_PTR( GraphicsBuffer );
	DECL_PTR( GraphicsCommandBuffer );
	DECL_PTR( GraphicsCommandEncoder );
	DECL_PTR( GraphicsComputePassEncoder );
	DECL_PTR( GraphicsComputePipeline );
	DECL_PTR( GraphicsPipelineLayout );
	DECL_PTR( GraphicsQuerySet );
	DECL_PTR( GraphicsQueue );
	DECL_PTR( GraphicsRenderBundle );
	DECL_PTR( GraphicsRenderBundleEncoder );
	DECL_PTR( GraphicsRenderPassEncoder );
	DECL_PTR( GraphicsRenderPipeline );
	DECL_PTR( GraphicsSampler );
	DECL_PTR( GraphicsShaderModule );
	DECL_PTR( GraphicsSurface );
	DECL_PTR( GraphicsSwapChain );
	DECL_PTR( GraphicsTexture );
	DECL_PTR( GraphicsTextureView );

	enum class PassKind
	{
		NONE,
		RENDER,
		BUNDLE,
		COMPUTE,
	};

	class GraphicsInstance
	{
	public:

	};
	class GraphicsSurface
	{
	public:
		XE::WindowPtr Window;
	};
	class GraphicsSwapChain
	{
	public:
		D3D12::SwapChainPtr SwapChain = nullptr;
	};
	class GraphicsAdapter
	{
	public:
		D3D12::AdapterPtr Adapter = nullptr;
		XE::GraphicsDeviceHandle Device;
	};
	class GraphicsDevice
	{
	public:
		D3D12::DevicePtr Device = nullptr;
		XE::GraphicsAdapterHandle Adapter;
		XE::GraphicsQueueHandle CommandQueue;
	};
	class GraphicsQueue
	{
	public:
		XE::GraphicsDeviceHandle Device;
		D3D12::CommandQueuePtr CommandQueue = nullptr;
	};
	class GraphicsBindGroup
	{
	public:
		XE::GraphicsBindGroupLayoutHandle Layout;

		D3D12::RootSignaturePtr RootSignature = nullptr;
		D3D12::CommandSignaturePtr CommandSignature = nullptr;
	};
	class GraphicsBindGroupLayout
	{
	public:

	};
	class GraphicsBuffer
	{
	public:
		D3D12_RANGE MapRange;
		std::atomic< bool > IsMap = false;
		D3D12::ResourcePtr Resource = nullptr;
		std::recursive_mutex Lock;
		XE::ConcurrentQueue< XE::Delegate< void() > > AsyncQueue;
	};
	class GraphicsCommandBuffer
	{
	public:
		XE::String MarkerLable;
		D3D12::GraphicsCommandListPtr CommandList = nullptr;
		D3D12::CommandAllocatorPtr CommandAllocator = nullptr;
	};
	class GraphicsCommandEncoder
	{
	public:
		XE::PassKind Kind = XE::PassKind::NONE;
		XE::uint64 PassIndex = 0;
		XE::GraphicsCommandBufferHandle CommandBuffer;
	};
	class GraphicsComputePassEncoder
	{
	public:
		XE::GraphicsComputePassDescriptor Desc;

		XE::GraphicsBindGroupHandle BindGroup;
		XE::GraphicsCommandEncoderHandle Encoder;
		XE::GraphicsComputePipelineHandle Pipeline;

		XE::uint64 PiplineStatisticsQueryIndex;
		XE::GraphicsQuerySetHandle PiplineStatisticsQuery;

		XE::ConcurrentQueue< XE::Delegate< void() > > TaskQueue;
	};
	class GraphicsComputePipeline
	{
	public:
		XE::GraphicsComputePipelineDescriptor Desc;

		XE::GraphicsDeviceHandle Device;
		XE::GraphicsPipelineLayoutHandle Layout;
		std::array< XE::GraphicsBindGroupHandle, 4 > BindGroups;
		
		D3D12::PipelineStatePtr PipelineState = nullptr;
	};
	class GraphicsPipelineLayout
	{
	public:

	};
	class GraphicsQuerySet
	{
	public:
		XE::GraphicsQuerySetDescriptor Desc;
		D3D12::QueryHeapPtr QueryHeap = nullptr;
	};
	class GraphicsRenderBundle
	{
	public:

	};
	class GraphicsRenderBundleEncoder
	{
	public:
		XE::GraphicsCommandEncoderHandle Encoder;
	};
	class GraphicsRenderPassEncoder
	{
	public:
		XE::GraphicsRenderPassDescriptor Desc;
		XE::GraphicsCommandEncoderHandle Encoder;
	};
	class GraphicsRenderPipeline
	{
	public:
		D3D12::PipelineStatePtr PipelineState = nullptr;
	};
	class GraphicsSampler
	{
	public:

	};
	class GraphicsShaderModule
	{
	public:
		D3D12_SHADER_BYTECODE ShaderCode;
	};
	class GraphicsTexture
	{
	public:
		XE::GraphicsTextureDescriptor Desc;
		XE::GraphicsBufferHandle Buffer;
	};
	class GraphicsTextureView
	{
	public:

	};
}

struct XE::GraphicsService::Private
{
	D3D12::DebugPtr _Debug;
	D3D12::FactoryPtr _Factory;

	XE::ConcurrentArray< XE::GraphicsInstancePtr > _Instances;
	XE::ConcurrentArray< XE::GraphicsSurfacePtr > _Surfaces;
	XE::ConcurrentArray< XE::GraphicsSwapChainPtr > _SwapChains;
	XE::ConcurrentArray< XE::GraphicsAdapterPtr > _Adapters;
	XE::ConcurrentArray< XE::GraphicsDevicePtr > _Devices;
	XE::ConcurrentArray< XE::GraphicsQueuePtr > _Queues;
	XE::ConcurrentArray< XE::GraphicsBindGroupPtr > _BindGroups;
	XE::ConcurrentArray< XE::GraphicsBindGroupLayoutPtr > _BindGroupLayouts;
	XE::ConcurrentArray< XE::GraphicsBufferPtr > _Buffers;
	XE::ConcurrentArray< XE::GraphicsCommandBufferPtr > _CommandBuffers;
	XE::ConcurrentArray< XE::GraphicsCommandEncoderPtr > _CommandEncoders;
	XE::ConcurrentArray< XE::GraphicsComputePassEncoderPtr > _ComputePassEncoders;
	XE::ConcurrentArray< XE::GraphicsComputePipelinePtr > _ComputePipelines;
	XE::ConcurrentArray< XE::GraphicsPipelineLayoutPtr > _PipelineLayouts;
	XE::ConcurrentArray< XE::GraphicsQuerySetPtr > _QuerySets;
	XE::ConcurrentArray< XE::GraphicsRenderBundlePtr > _RenderBundles;
	XE::ConcurrentArray< XE::GraphicsRenderBundleEncoderPtr > _RenderBundleEncoders;
	XE::ConcurrentArray< XE::GraphicsRenderPassEncoderPtr > _RenderPassEncoders;
	XE::ConcurrentArray< XE::GraphicsRenderPipelinePtr > _RenderPipelines;
	XE::ConcurrentArray< XE::GraphicsShaderModulePtr > _ShaderModules;
	XE::ConcurrentArray< XE::GraphicsTexturePtr > _Textures;
	XE::ConcurrentArray< XE::GraphicsTextureViewPtr > _TextureViews;

	XE::QueueHandleAllocator< XE::GraphicsInstanceHandle > _InstanceHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsSurfaceHandle > _SurfaceHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsSwapChainHandle > _SwapChainHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsAdapterHandle > _AdapterHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsDeviceHandle > _DeviceHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsQueueHandle > _QueueHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsBindGroupHandle > _BindGroupHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsBindGroupLayoutHandle > _BindGroupLayoutHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsBufferHandle > _BufferHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsCommandBufferHandle > _CommandBufferHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsCommandEncoderHandle > _CommandEncoderHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsComputePassEncoderHandle > _ComputePassEncoderHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsComputePipelineHandle > _ComputePipelineHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsPipelineLayoutHandle > _PipelineLayoutHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsQuerySetHandle > _QuerySetHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsRenderBundleHandle > _RenderBundleHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsRenderBundleEncoderHandle > _RenderBundleEncoderHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsRenderPassEncoderHandle > _RenderPassEncoderHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsRenderPipelineHandle > _RenderPipelineHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsShaderModuleHandle > _ShaderModuleHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsTextureHandle > _TextureHandleAllocator;
	XE::QueueHandleAllocator< XE::GraphicsTextureViewHandle > _TextureViewHandleAllocator;
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

}

bool XE::GraphicsService::Startup()
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
		return false;
	}

	return true;
}

void XE::GraphicsService::Update()
{

}

void XE::GraphicsService::Clearup()
{
	_p->_Factory = nullptr;
	_p->_Debug = nullptr;
}

void XE::GraphicsService::AdapterEnumerateFeatures( XE::GraphicsAdapterHandle adapter, XE::Array< XE::GraphicsFeatureName > & features )
{
	if ( auto ada = _p->_Adapters[adapter.GetValue()] )
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
	if ( auto ada = _p->_Adapters[adapter.GetValue()] )
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
	if ( auto ada = _p->_Adapters[adapter.GetValue()] )
	{
		DXGI_ADAPTER_DESC1 desc1;
		if ( SUCCEEDED( ada->Adapter->GetDesc1( &desc1 ) ) )
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
				if ( ada->Device && SUCCEEDED( _p->_Devices[ada->Device.GetValue()]->Device->CheckFeatureSupport( D3D12_FEATURE_ARCHITECTURE, &arch, sizeof( arch ) ) ) )
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
	if ( auto ada = _p->_Adapters[adapter.GetValue()] )
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
			XE::GraphicsDevicePtr device = XE::MakeShared< XE::GraphicsDevice >();
			if ( SUCCEEDED( ::D3D12CreateDevice( ada->Adapter.Get(), levels[i], IID_PPV_ARGS( device->Device.GetAddressOf() ) ) ) )
			{
				ada->Device = _p->_DeviceHandleAllocator.Alloc();
				if ( ada->Device.GetValue() >= _p->_Devices.size() )
				{
					_p->_Devices.resize( ada->Device.GetValue() + 1 );
				}
				_p->_Devices.set( ada->Device.GetValue(), device );
			}
		}

		if ( ada->Device )
		{
			callback( XE::GraphicsRequestDeviceStatus::SUCCESS, ada->Device, "" );
		}
		else
		{
			callback( XE::GraphicsRequestDeviceStatus::ERROR, ada->Device, "" );
		}
	}
}

void XE::GraphicsService::BufferDestroy( XE::GraphicsBufferHandle buffer )
{
	if ( auto buf = _p->_Buffers[buffer.GetValue()] )
	{
		_p->_Buffers.set( buffer.GetValue(), nullptr );
		_p->_BufferHandleAllocator.Free( buffer );
	}
}

XE::Span< XE::uint8 > XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	if ( auto buf = _p->_Buffers[buffer.GetValue()] )
	{
		std::unique_lock< std::recursive_mutex > lock( buf->Lock );

		buf->MapRange.Begin = offset;
		buf->MapRange.End = offset + size;

		void * data = nullptr;
		if ( SUCCEEDED( buf->Resource->Map( 0, &buf->MapRange, &data ) ) )
		{
			buf->IsMap = true;

			return { reinterpret_cast<XE::uint8 *>( data ), size };
		}
	}

	return {};
}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback )
{
	if ( auto buf = _p->_Buffers[buffer.GetValue()] )
	{
		auto func = [=]()
		{
			if ( auto buf = _p->_Buffers[buffer.GetValue()] )
			{
				std::unique_lock< std::recursive_mutex > lock( buf->Lock );

				callback( XE::GraphicsBufferMapAsyncStatus::SUCCESS, BufferGetMappedRange( buffer, offset, size ) );
			}
		};

		if ( buf->IsMap )
		{
			buf->AsyncQueue.push( func );
		}
		else
		{
			try
			{
				if ( buf->Lock.try_lock() )
				{
					func();

					buf->Lock.unlock();
				}
			}
			catch ( ... )
			{
				buf->Lock.unlock();
			}
		}
	}
}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{
	if ( auto buf = _p->_Buffers[buffer.GetValue()] )
	{
		if ( buf->IsMap )
		{
			buf->Resource->Unmap( 0, &buf->MapRange );

			buf->IsMap = false;

			XE::Delegate< void() > async;
			if ( buf->AsyncQueue.try_pop( async ) )
			{
				async();
			}
		}
	}
}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			auto handle = _p->_ComputePassEncoderHandleAllocator.Alloc();
			if ( handle.GetValue() >= _p->_ComputePassEncoders.size() )
			{
				_p->_ComputePassEncoders.push_back( nullptr );
			}

			auto pass = XE::MakeShared< XE::GraphicsComputePassEncoder >();

			pass->Desc = descriptor;
			pass->Encoder = command_encoder;

			_p->_ComputePassEncoders.set( handle.GetValue(), pass );

			cmd->Kind = XE::PassKind::COMPUTE;
			cmd->PassIndex = handle.GetValue();

			cmd_buf->CommandList->BeginEvent( 0, pass->Desc.Label.c_str(), pass->Desc.Label.size() );

			return handle;
		}
	}

	return {};
}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			auto handle = _p->_RenderPassEncoderHandleAllocator.Alloc();
			if ( handle.GetValue() >= _p->_RenderPassEncoders.size() )
			{
				_p->_RenderPassEncoders.push_back( nullptr );
			}

			auto pass = XE::MakeShared< XE::GraphicsRenderPassEncoder >();

			pass->Desc = descriptor;
			pass->Encoder = command_encoder;

			_p->_RenderPassEncoders.set( handle.GetValue(), pass );

			cmd->Kind = XE::PassKind::RENDER;
			cmd->PassIndex = handle.GetValue();

			cmd_buf->CommandList->BeginEvent( 0, pass->Desc.Label.c_str(), pass->Desc.Label.size() );

			return handle;
		}
	}

	return {};
}

void XE::GraphicsService::CommandEncoderClearBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto buf = _p->_Buffers[buffer.GetValue()] )
		{
			if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
			{
				D3D12::GraphicsCommandListPtr list;
				if ( SUCCEEDED( cmd_buf->CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
				{
					list->CopyBufferRegion( buf->Resource.Get(), offset, nullptr, 0, size );
				}
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle src, XE::uint64 src_offset, XE::GraphicsBufferHandle dst, XE::uint64 dst_offset, XE::uint64 size )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto src_buf = _p->_Buffers[src.GetValue()] )
		{
			if ( auto dst_buf = _p->_Buffers[dst.GetValue()] )
			{
				if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
				{
					D3D12::GraphicsCommandListPtr list;
					if ( SUCCEEDED( cmd_buf->CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
					{
						list->CopyBufferRegion( dst_buf->Resource.Get(), dst_offset, src_buf->Resource.Get(), src_offset, size );
					}
				}
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyBuffer & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto src_buf = _p->_Buffers[src.Buffer.GetValue()] )
		{
			if ( auto dst_tex = _p->_Textures[dst.Texture.GetValue()] )
			{
				if ( auto dst_buf = _p->_Buffers[dst_tex->Buffer.GetValue()] )
				{
					if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
					{
						D3D12::GraphicsCommandListPtr list;
						if ( SUCCEEDED( cmd_buf->CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
						{
							D3D12_PLACED_SUBRESOURCE_FOOTPRINT src_footprint;
							src_footprint.Offset = 0;
							src_footprint.Footprint.Format = Cast( dst_tex->Desc.Format );
							src_footprint.Footprint.Width = dst_tex->Desc.Size.x;
							src_footprint.Footprint.Height = dst_tex->Desc.Size.y;
							src_footprint.Footprint.Depth = dst_tex->Desc.Size.z;
							//src_footprint.Footprint.RowPitch = dst_tex->Desc.

							CD3DX12_TEXTURE_COPY_LOCATION src_location( src_buf->Resource.Get(), {} );

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
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto src_tex = _p->_Textures[src.Texture.GetValue()] )
		{
			if ( auto src_buf = _p->_Buffers[src_tex->Buffer.GetValue()] )
			{
				if ( auto dst_tex = _p->_Textures[dst.Texture.GetValue()] )
				{
					if ( auto dst_buf = _p->_Buffers[dst_tex->Buffer.GetValue()] )
					{
						if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
						{
							D3D12::GraphicsCommandListPtr list;
							if ( SUCCEEDED( cmd_buf->CommandList.As< ID3D12GraphicsCommandList >( &list ) ) )
							{
								CD3DX12_TEXTURE_COPY_LOCATION src_location( src_buf->Resource.Get() );

								CD3DX12_TEXTURE_COPY_LOCATION dst_location( dst_buf->Resource.Get() );

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
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			cmd_buf->CommandList->SetMarker( 0, marker_label.c_str(), marker_label.size() );
		}
	}
}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			cmd_buf->CommandList->EndEvent();
		}
	}
}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & group_label )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			cmd_buf->CommandList->BeginEvent( 0, group_label.c_str(), group_label.size() );
		}
	}
}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferHandle dst, XE::uint64 dst_offset )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			if ( auto query = _p->_QuerySets[query_set.GetValue()] )
			{
				if ( auto buf = _p->_Buffers[dst.GetValue()] )
				{
					cmd_buf->CommandList->ResolveQueryData( query->QueryHeap.Get(), Cast( query->Desc.Type ), first_query, query_count, buf->Resource.Get(), dst_offset );
				}
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	if ( auto cmd = _p->_CommandEncoders[command_encoder.GetValue()] )
	{
		if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
		{
			if ( auto query = _p->_QuerySets[query_set.GetValue()] )
			{
				cmd_buf->CommandList->EndQuery( query->QueryHeap.Get(), D3D12_QUERY_TYPE_TIMESTAMP, query_index );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		pass->TaskQueue.push( [=]()
		{
			if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
			{
				if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
				{
					if ( auto query = _p->_QuerySets[query_set.GetValue()] )
					{
						pass->PiplineStatisticsQuery = query_set;
						pass->PiplineStatisticsQueryIndex = query_index;
						cmd_buf->CommandList->BeginQuery( query->QueryHeap.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, query_index );
					}
				}
			}
		} );
	}
}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		pass->TaskQueue.push( [=]()
		{
			if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
			{
				if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
				{
					cmd_buf->CommandList->Dispatch( workgroup_count_x, workgroup_count_y, workgroup_count_z );
				}
			}
		} );
	}
}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		pass->TaskQueue.push( [=]()
		{
			if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
			{
				if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
				{
					if ( auto bind_group = _p->_BindGroups[pass->BindGroup.GetValue()] )
					{
						if ( auto buf = _p->_Buffers[indirect_buffer.GetValue()] )
						{
							cmd_buf->CommandList->ExecuteIndirect( bind_group->CommandSignature.Get(), 1, buf->Resource.Get(), indirect_offset, nullptr, 0 );
						}
					}
				}
			}
		} );
	}
}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
		{
			if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
			{
				cmd_buf->CommandList->EndEvent();
				cmd->Kind = XE::PassKind::NONE;
				cmd->PassIndex = 0;
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
		{
			if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
			{
				if ( auto query = _p->_QuerySets[pass->PiplineStatisticsQuery.GetValue()] )
				{
					cmd_buf->CommandList->EndQuery( query->QueryHeap.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, pass->PiplineStatisticsQueryIndex );
				}
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & marker_label )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
		{
			if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
			{
				cmd_buf->CommandList->SetMarker( 0, marker_label.c_str(), marker_label.size() );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
		{
			if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
			{
				cmd_buf->CommandList->EndEvent();
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & group_label )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		if ( auto cmd = _p->_CommandEncoders[pass->Encoder.GetValue()] )
		{
			if ( auto cmd_buf = _p->_CommandBuffers[cmd->CommandBuffer.GetValue()] )
			{
				cmd_buf->CommandList->BeginEvent( 0, group_label.c_str(), group_label.size() );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		pass->BindGroup = group;

		// TODO: 
	}
}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsComputePipelineHandle pipeline )
{
	if ( auto pass = _p->_ComputePassEncoders[compute_pass_encoder.GetValue()] )
	{
		pass->Pipeline = pipeline;
	}
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle compute_pipeline, XE::uint32 group_index )
{
	if ( auto pipe = _p->_ComputePipelines[compute_pipeline.GetValue()] )
	{
		if ( auto bind = _p->_BindGroups[pipe->BindGroups[group_index].GetValue()] )
		{
			return bind->Layout;
		}
	}

	return {};
}

void XE::GraphicsService::ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle compute_pipeline, const XE::String & label )
{
	if ( auto pipe = _p->_ComputePipelines[compute_pipeline.GetValue()] )
	{
		pipe->Desc.Label = label;


	}
}

XE::GraphicsBindGroupHandle XE::GraphicsService::DeviceCreateBindGroup( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::DeviceCreateBindGroupLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBufferHandle XE::GraphicsService::DeviceCreateBuffer( XE::GraphicsDeviceHandle device, const XE::GraphicsBufferDescriptor & descriptor )
{
	if ( auto dev = _p->_Devices[device.GetValue()] )
	{
		auto handle = _p->_BufferHandleAllocator.Alloc();
		if ( handle.GetValue() >= _p->_Buffers.size() ) _p->_Buffers.push_back( nullptr );

		auto buf = XE::MakeShared< XE::GraphicsBuffer >();
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

		if ( SUCCEEDED( dev->Device->CreateCommittedResource( &heap_properties, D3D12_HEAP_FLAG_NONE, &raw_desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( buf->Resource.GetAddressOf() ) ) ) )
		{
			_p->_Buffers.set( handle.GetValue(), buf );

			return handle;
		}
	}

	return {};
}

XE::GraphicsCommandEncoderHandle XE::GraphicsService::DeviceCreateCommandEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsComputePipelineHandle XE::GraphicsService::DeviceCreateComputePipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::DeviceCreateComputePipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor, CreateComputePipelineAsyncCallback callback )
{

}

XE::GraphicsPipelineLayoutHandle XE::GraphicsService::DeviceCreatePipelineLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsQuerySetHandle XE::GraphicsService::DeviceCreateQuerySet( XE::GraphicsDeviceHandle device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderBundleEncoderHandle XE::GraphicsService::DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPipelineHandle XE::GraphicsService::DeviceCreateRenderPipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::DeviceCreateRenderPipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor, CreateRenderPipelineAsyncCallback callback )
{

}

XE::GraphicsSamplerHandle XE::GraphicsService::DeviceCreateSampler( XE::GraphicsDeviceHandle device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	return {};
}

XE::GraphicsShaderModuleHandle XE::GraphicsService::DeviceCreateShaderModule( XE::GraphicsDeviceHandle device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSwapChainHandle XE::GraphicsService::DeviceCreateSwapChain( XE::GraphicsDeviceHandle device, XE::GraphicsSurfaceHandle surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTextureHandle XE::GraphicsService::DeviceCreateTexture( XE::GraphicsDeviceHandle device, const XE::GraphicsTextureDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::DeviceDestroy( XE::GraphicsDeviceHandle device )
{

}

void XE::GraphicsService::DeviceEnumerateFeatures( XE::GraphicsDeviceHandle device, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsService::DeviceGetLimits( XE::GraphicsDeviceHandle device, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

XE::GraphicsQueueHandle XE::GraphicsService::DeviceGetQueue( XE::GraphicsDeviceHandle device )
{
	return {};
}

bool XE::GraphicsService::DeviceHasFeature( XE::GraphicsDeviceHandle device, XE::GraphicsFeatureName feature )
{
	return {};
}

bool XE::GraphicsService::DevicePopErrorScope( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	return {};
}

void XE::GraphicsService::DevicePushErrorScope( XE::GraphicsDeviceHandle device, XE::GraphicsErrorFilter filter )
{

}

void XE::GraphicsService::DeviceSetDeviceLostCallback( XE::GraphicsDeviceHandle device, DeviceLostCallback callback )
{

}

void XE::GraphicsService::DeviceSetUncapturedErrorCallback( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{

}

XE::GraphicsSurfaceHandle XE::GraphicsService::InstanceCreateSurface( XE::GraphicsInstanceHandle instance, const XE::GraphicsSurfaceDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::InstanceProcessEvents( XE::GraphicsInstanceHandle instance )
{

}

void XE::GraphicsService::InstanceRequestAdapter( XE::GraphicsInstanceHandle instance, const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback )
{

}

void XE::GraphicsService::QuerySetDestroy( XE::GraphicsQuerySetHandle query_set )
{

}

void XE::GraphicsService::QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback )
{

}

void XE::GraphicsService::QueueSubmit( XE::GraphicsQueueHandle queue, const XE::Array< XE::GraphicsCommandBufferHandle > & commands )
{

}

void XE::GraphicsService::QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{

}

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & dst, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{

}

void XE::GraphicsService::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsService::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsService::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsService::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{

}

XE::GraphicsRenderBundleHandle XE::GraphicsService::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsService::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder )
{

}

void XE::GraphicsService::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & group_label )
{

}

void XE::GraphicsService::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{

}

void XE::GraphicsService::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsRenderPipelineHandle pipeline )
{

}

void XE::GraphicsService::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 query_index )
{

}

void XE::GraphicsService::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{

}

void XE::GraphicsService::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsService::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsService::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsService::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsService::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{

}

void XE::GraphicsService::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{

}

void XE::GraphicsService::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{

}

void XE::GraphicsService::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Array< XE::GraphicsRenderBundleHandle > & bundles )
{

}

void XE::GraphicsService::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsService::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{

}

void XE::GraphicsService::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsService::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{

}

void XE::GraphicsService::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Color & color )
{

}

void XE::GraphicsService::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsRenderPipelineHandle pipeline )
{

}

void XE::GraphicsService::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Recti & rect )
{

}

void XE::GraphicsService::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 reference )
{

}

void XE::GraphicsService::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{

}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle render_pipeline, XE::uint32 group_index )
{
	return {};
}

void XE::GraphicsService::RenderPipelineSetLabel( XE::GraphicsRenderPipelineHandle render_pipeline, const XE::String & label )
{

}

void XE::GraphicsService::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shader_module, CompilationInfoCallback callback )
{

}

void XE::GraphicsService::ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shader_module, const XE::String & label )
{

}

XE::GraphicsTextureFormat XE::GraphicsService::SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter )
{
	return {};
}

XE::GraphicsTextureViewHandle XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swap_chain )
{
	return {};
}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainHandle swap_chain )
{

}

XE::GraphicsTextureViewHandle XE::GraphicsService::TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::TextureDestroy( XE::GraphicsTextureHandle texture )
{

}

#endif // GRAPHICS_API == GRAPHICS_D3D12