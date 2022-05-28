#include "GraphicsService.h"

#if GRAPHICS_API == GRAPHICS_D3D12

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

IMPLEMENT_META( XE::GraphicsService );

namespace
{
	template< typename T > T * HandleCast( XE::Handle< T > handle )
	{
		return reinterpret_cast<T *>( handle.GetValue() );
	}
	template< typename T > XE::Handle< T > HandleCast( T * ptr )
	{
		return XE::HandleCast< T >( reinterpret_cast<XE::uint64>( ptr ) );
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

	class GraphicsInstance
	{
	public:

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
		XE::GraphicsQueuePtr CommandQueue = nullptr;
	};
	class GraphicsBindGroup
	{
	public:
		D3D12::RootSignaturePtr RootSignature = nullptr;
	};
	class GraphicsBindGroupLayout
	{
	public:

	};
	class GraphicsBuffer
	{
	public:
		D3D12::ResourcePtr Resource = nullptr;
	};
	class GraphicsCommandBuffer
	{
	public:
		D3D12::CommandQueuePtr CommandQueue = nullptr;
		D3D12::CommandAllocatorPtr CommandAllocator = nullptr;
	};
	class GraphicsCommandEncoder
	{
	public:
		D3D12::CommandListPtr CommandList = nullptr;
	};
	class GraphicsComputePassEncoder
	{
	public:
		D3D12::GraphicsCommandListPtr CommandList = nullptr;
	};
	class GraphicsComputePipeline
	{
	public:
		D3D12::PipelineStatePtr PipelineState = nullptr;
	};
	class GraphicsPipelineLayout
	{
	public:

	};
	class GraphicsQuerySet
	{
	public:
		D3D12::QueryHeapPtr QueryHeap = nullptr;
	};
	class GraphicsQueue
	{
	public:
		D3D12::CommandQueuePtr CommandQueue = nullptr;
	};
	class GraphicsRenderBundle
	{
	public:

	};
	class GraphicsRenderBundleEncoder
	{
	public:

	};
	class GraphicsRenderPassEncoder
	{
	public:
		D3D12::GraphicsCommandListPtr CommandList = nullptr;
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
	class GraphicsTexture
	{
	public:

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

	XE::ConcurrentArray< XE::GraphicsAdapterPtr > _Adapters;
	XE::ConcurrentArray< XE::GraphicsDevicePtr > _Devices;
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
		
	}
}

void XE::GraphicsService::BufferDestroy( XE::GraphicsBufferHandle buffer )
{

}

XE::Span< XE::uint8 > XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::GraphicsMapModeFlags mode, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback )
{

}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{

}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::CommandEncoderClearBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle source, XE::uint64 source_offset, XE::GraphicsBufferHandle destination, XE::uint64 destination_offset, XE::uint64 size )
{

}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsService::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsService::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

XE::GraphicsCommandBufferHandle XE::GraphicsService::CommandEncoderFinish( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsService::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder )
{

}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & group_label )
{

}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferHandle destination, XE::uint64 destination_offset )
{

}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{

}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{

}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{

}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{

}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{

}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{

}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{

}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsComputePipelineHandle pipeline )
{

}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle compute_pipeline, XE::uint32 group_index )
{
	return {};
}

void XE::GraphicsService::ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle compute_pipeline, const XE::String & label )
{

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

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
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