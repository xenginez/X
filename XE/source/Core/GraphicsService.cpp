#include "GraphicsService.h"

#include "CoreFramework.h"
#include "GraphicsServiceNull.h"
#include "GraphicsServiceD3D11.h"
#include "GraphicsServiceD3D12.h"
#include "GraphicsServiceMetal.h"
#include "GraphicsServiceVulkan.h"
#include "GraphicsServiceWebGpu.h"
#include "GraphicsServiceOpenGL.h"
#include "GraphicsServiceOpenGLES.h"

namespace XE
{
#if !( GRAPHICS_API & GRAPHICS_D3D11 )
	class GraphicsServiceD3D11 : public GraphicsServiceNull { public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif
#if !( GRAPHICS_API & GRAPHICS_D3D12 )
	class GraphicsServiceD3D12 : public GraphicsServiceNull{ public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif
#if !( GRAPHICS_API & GRAPHICS_METAL )
	class GraphicsServiceMetal : public GraphicsServiceNull{ public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif
#if !( GRAPHICS_API & GRAPHICS_VULKAN )
	class GraphicsServiceVulkan : public GraphicsServiceNull{ public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif
#if !( GRAPHICS_API & GRAPHICS_WEBGPU )
	class GraphicsServiceWebGpu : public GraphicsServiceNull{ public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif
#if !( GRAPHICS_API & GRAPHICS_OPENGL )
	class GraphicsServiceOpenGL : public GraphicsServiceNull{ public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif
#if !( GRAPHICS_API & GRAPHICS_OPENGLES )
	class GraphicsServiceOpenGLES : public GraphicsServiceNull{ public: using GraphicsServiceNull::GraphicsServiceNull; };
#endif

#define SKIP_CALL( NAME, ... ) \
	if constexpr ( !std::is_void_v< typename XE::FunctionTraits< decltype( &XE::GraphicsService::##NAME ) >::result_type > ) \
	{ \
		return std::visit( XE::Overloaded{ \
			[&]( XE::GraphicsServiceNull * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceD3D11 * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceD3D12 * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceMetal * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceVulkan * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceWebGpu * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceOpenGL * instance ) { return instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceOpenGLES * instance ) { return instance->NAME( __VA_ARGS__ ); } \
						   }, _p->_Instance ); \
	} \
	else \
	{ \
		std::visit( XE::Overloaded{ \
			[&]( XE::GraphicsServiceNull * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceD3D11 * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceD3D12 * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceMetal * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceVulkan * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceWebGpu * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceOpenGL * instance ) { instance->NAME( __VA_ARGS__ ); }, \
			[&]( XE::GraphicsServiceOpenGLES * instance ) { instance->NAME( __VA_ARGS__ ); } \
					}, _p->_Instance ); \
	}
}

IMPLEMENT_META( XE::GraphicsService );

struct XE::GraphicsService::Private
{
	XE::GraphicsBackendType _Type;

	std::variant<
		XE::GraphicsServiceNull *,
		XE::GraphicsServiceD3D11 *,
		XE::GraphicsServiceD3D12 *,
		XE::GraphicsServiceMetal *,
		XE::GraphicsServiceVulkan *,
		XE::GraphicsServiceWebGpu *,
		XE::GraphicsServiceOpenGL *,
		XE::GraphicsServiceOpenGLES *
	> _Instance;
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
	bool debug = GetFramework()->GetBool( CONFIG_GRAPHICS_DEBUG );
	auto backend = GetFramework()->GetString( CONFIG_GRAPHICS_BACKEND );

	if ( backend == "XE::GraphicsBackendType::NIL" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceNull >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::WEBGPU" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceWebGpu >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::D3D11" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceD3D11 >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::D3D12" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceD3D12 >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::METAL" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceMetal >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::VULKAN" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceVulkan >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::OPENGL" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceOpenGL >( debug );
	}
	else if ( backend == "XE::GraphicsBackendType::OPENGLES" )
	{
		_p->_Instance = XE::New < XE::GraphicsServiceOpenGLES >( debug );
	}
	else
	{
#if GRAPHICS_API & GRAPHICS_D3D12
		_p->_Instance = XE::New < XE::GraphicsServiceD3D12 >( debug );
#elif GRAPHICS_API & GRAPHICS_METAL
		_p->_Instance = XE::New < XE::GraphicsServiceMetal >( debug );
#elif GRAPHICS_API & GRAPHICS_VULKAN
		_p->_Instance = XE::New < XE::GraphicsServiceVulkan >( debug );
#elif GRAPHICS_API & GRAPHICS_WEBGPU
		_p->_Instance = XE::New < XE::GraphicsServiceWebGpu >( debug );
#elif GRAPHICS_API & GRAPHICS_OPENGL
		_p->_Instance = XE::New < XE::GraphicsServiceOpenGL >( debug );
#elif GRAPHICS_API & GRAPHICS_OPENGLES
		_p->_Instance = XE::New < XE::GraphicsServiceOpenGLES >( debug );
#else
		_p->_Instance = XE::New < XE::GraphicsServiceNull >( debug );
#endif
	}

	SKIP_CALL( Prepare );
}

void XE::GraphicsService::Startup()
{
	SKIP_CALL( Startup );
}

void XE::GraphicsService::Update()
{
	SKIP_CALL( Update );
}

void XE::GraphicsService::Clearup()
{
	SKIP_CALL( Clearup );

	std::visit( XE::Overloaded{ 
			[&]( XE::GraphicsServiceNull * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceD3D11 * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceD3D12 * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceMetal * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceVulkan * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceWebGpu * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceOpenGL * instance ) { XE::Delete( instance ); },
			[&]( XE::GraphicsServiceOpenGLES * instance ) { XE::Delete( instance ); }
				}, _p->_Instance );

	_p->_Instance = (XE::GraphicsServiceNull *)( nullptr );
}

XE::GraphicsBackendType XE::GraphicsService::GetBackendType() const
{
	SKIP_CALL( GetBackendType );
}

XE::GraphicsSurfacePtr XE::GraphicsService::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	SKIP_CALL( CreateSurface, descriptor );
}

void XE::GraphicsService::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback )
{
	SKIP_CALL( RequestAdapter, options, callback );
}

void XE::GraphicsService::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{
	SKIP_CALL( AdapterEnumerateFeatures, adapter, features );
}

bool XE::GraphicsService::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	SKIP_CALL( AdapterGetLimits, adapter, limits );
}

void XE::GraphicsService::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{
	SKIP_CALL( AdapterGetProperties, adapter, properties );
}

bool XE::GraphicsService::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	SKIP_CALL( AdapterHasFeature, adapter, feature );
}

void XE::GraphicsService::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback )
{
	SKIP_CALL( AdapterRequestDevice, adapter, descriptor, callback );
}

XE::GraphicsBindGroupPtr XE::GraphicsService::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateBindGroup, device, descriptor );
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsService::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateBindGroupLayout, device, descriptor );
}

XE::GraphicsBufferPtr XE::GraphicsService::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateBuffer, device, descriptor );
}

XE::GraphicsCommandEncoderPtr XE::GraphicsService::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateCommandEncoder, device, descriptor );
}

XE::GraphicsComputePipelinePtr XE::GraphicsService::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateComputePipeline, device, descriptor );
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsService::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreatePipelineLayout, device, descriptor );
}

XE::GraphicsQuerySetPtr XE::GraphicsService::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateQuerySet, device, descriptor );
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsService::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateRenderBundleEncoder, device, descriptor );
}

XE::GraphicsRenderPipelinePtr XE::GraphicsService::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateRenderPipeline, device, descriptor );
}

XE::GraphicsSamplerPtr XE::GraphicsService::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateSampler, device, descriptor );
}

XE::GraphicsShaderModulePtr XE::GraphicsService::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateShaderModule, device, descriptor );
}

XE::GraphicsSwapChainPtr XE::GraphicsService::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateSwapChain, device, surface, descriptor );
}

XE::GraphicsTexturePtr XE::GraphicsService::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	SKIP_CALL( DeviceCreateTexture, device, descriptor );
}

XE::GraphicsTextureViewPtr XE::GraphicsService::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	SKIP_CALL( TextureCreateView, texture, descriptor );
}

void XE::GraphicsService::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{
	SKIP_CALL( DeviceEnumerateFeatures, device, features );
}

bool XE::GraphicsService::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	SKIP_CALL( DeviceGetLimits, device, limits );
}

XE::GraphicsQueuePtr XE::GraphicsService::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	SKIP_CALL( DeviceGetQueue, device );
}

bool XE::GraphicsService::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	SKIP_CALL( DeviceHasFeature, device, feature );
}

bool XE::GraphicsService::DevicePopErrorScope( XE::GraphicsDevicePtr device, ErrorCallback callback )
{
	SKIP_CALL( DevicePopErrorScope, device, callback );
}

void XE::GraphicsService::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{
	SKIP_CALL( DevicePushErrorScope, device, filter );
}

void XE::GraphicsService::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, DeviceLostCallback callback )
{
	SKIP_CALL( DeviceSetDeviceLostCallback, device, callback );
}

void XE::GraphicsService::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, ErrorCallback callback )
{
	SKIP_CALL( DeviceSetUncapturedErrorCallback, device, callback );
}

void XE::GraphicsService::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, QueueWorkDoneCallback callback )
{
	SKIP_CALL( QueueOnSubmittedWorkDone, queue, callback );
}

void XE::GraphicsService::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{
	SKIP_CALL( QueueSubmit, queue, commands );
}

void XE::GraphicsService::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{
	SKIP_CALL( QueueWriteBuffer, queue, buffer, buffer_offset, data );
}

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{
	SKIP_CALL( QueueWriteTexture, queue, destination, data, data_layout, write_size );
}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, BufferMapCallback callback )
{
	SKIP_CALL( BufferMapAsync, buffer, mode, offset, size, callback );
}

XE::Span< const XE::uint8 > XE::GraphicsService::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	SKIP_CALL( BufferGetConstMappedRange, buffer, offset, size );
}

XE::Span< XE::uint8 > XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	SKIP_CALL( BufferGetMappedRange, buffer, offset, size );
}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferPtr buffer )
{
	SKIP_CALL( BufferUnmap, buffer );
}

XE::GraphicsComputePassEncoderPtr XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	SKIP_CALL( CommandEncoderBeginComputePass, command_encoder, descriptor );
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	SKIP_CALL( CommandEncoderBeginRenderPass, command_encoder, descriptor );
}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr source, XE::uint64 source_offset, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset, XE::uint64 size )
{
	SKIP_CALL( CommandEncoderCopyBufferToBuffer, command_encoder, source, source_offset, destination, destination_offset, size );
}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{
	SKIP_CALL( CommandEncoderCopyBufferToTexture, command_encoder, source, destination, copy_size );
}

void XE::GraphicsService::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size )
{
	SKIP_CALL( CommandEncoderCopyTextureToBuffer, command_encoder, source, destination, copy_size );
}

void XE::GraphicsService::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{
	SKIP_CALL( CommandEncoderCopyTextureToTexture, command_encoder, source, destination, copy_size );
}

XE::GraphicsCommandBufferPtr XE::GraphicsService::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	SKIP_CALL( CommandEncoderFinish, command_encoder, descriptor );
}

void XE::GraphicsService::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{
	SKIP_CALL( CommandEncoderInsertDebugMarker, command_encoder, marker_label );
}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{
	SKIP_CALL( CommandEncoderPopDebugGroup, command_encoder );
}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{
	SKIP_CALL( CommandEncoderPushDebugGroup, command_encoder, group_label );
}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset )
{
	SKIP_CALL( CommandEncoderResolveQuerySet, command_encoder, query_set, first_query, query_count, destination, destination_offset );
}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	SKIP_CALL( CommandEncoderWriteTimestamp, command_encoder, query_set, query_index );
}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	SKIP_CALL( ComputePassEncoderBeginPipelineStatisticsQuery, compute_pass_encoder, query_set, query_index );
}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{
	SKIP_CALL( ComputePassEncoderDispatch, compute_pass_encoder, workgroup_count_x, workgroup_count_y, workgroup_count_z );
}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	SKIP_CALL( ComputePassEncoderDispatchIndirect, compute_pass_encoder, indirect_buffer, indirect_offset );
}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	SKIP_CALL( ComputePassEncoderEnd, compute_pass_encoder );
}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	SKIP_CALL( ComputePassEncoderEndPipelineStatisticsQuery, compute_pass_encoder );
}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{
	SKIP_CALL( ComputePassEncoderInsertDebugMarker, compute_pass_encoder, marker_label );
}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	SKIP_CALL( ComputePassEncoderPopDebugGroup, compute_pass_encoder );
}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{
	SKIP_CALL( ComputePassEncoderPushDebugGroup, compute_pass_encoder, group_label );
}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	SKIP_CALL( ComputePassEncoderSetBindGroup, compute_pass_encoder, group_index, group, dynamic_offsets );
}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{
	SKIP_CALL( ComputePassEncoderSetPipeline, compute_pass_encoder, pipeline );
}

void XE::GraphicsService::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	SKIP_CALL( RenderPassEncoderBeginOcclusionQuery, render_pass_encoder, query_set, query_index );
}

void XE::GraphicsService::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	SKIP_CALL( RenderPassEncoderBeginPipelineStatisticsQuery, render_pass_encoder, query_set, query_index );
}

void XE::GraphicsService::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	SKIP_CALL( RenderPassEncoderDraw, render_pass_encoder, vertex_count, instance_count, first_vertex, first_instance );
}

void XE::GraphicsService::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	SKIP_CALL( RenderPassEncoderDrawIndexed, render_pass_encoder, index_count, instance_count, first_index, base_vertex, first_instance );
}

void XE::GraphicsService::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	SKIP_CALL( RenderPassEncoderDrawIndexedIndirect, render_pass_encoder, indirect_buffer, indirect_offset );
}

void XE::GraphicsService::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	SKIP_CALL( RenderPassEncoderDrawIndirect, render_pass_encoder, indirect_buffer, indirect_offset );
}

void XE::GraphicsService::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	SKIP_CALL( RenderPassEncoderEnd, render_pass_encoder );
}

void XE::GraphicsService::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	SKIP_CALL( RenderPassEncoderEndOcclusionQuery, render_pass_encoder );
}

void XE::GraphicsService::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	SKIP_CALL( RenderPassEncoderEndPipelineStatisticsQuery, render_pass_encoder );
}

void XE::GraphicsService::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{
	SKIP_CALL( RenderPassEncoderExecuteBundles, render_pass_encoder, bundles );
}

void XE::GraphicsService::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{
	SKIP_CALL( RenderPassEncoderInsertDebugMarker, render_pass_encoder, marker_label );
}

void XE::GraphicsService::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	SKIP_CALL( RenderPassEncoderPopDebugGroup, render_pass_encoder );
}

void XE::GraphicsService::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{
	SKIP_CALL( RenderPassEncoderPushDebugGroup, render_pass_encoder, group_label );
}

void XE::GraphicsService::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	SKIP_CALL( RenderPassEncoderSetBindGroup, render_pass_encoder, group_index, group, dynamic_offsets );
}

void XE::GraphicsService::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{
	SKIP_CALL( RenderPassEncoderSetBlendConstant, render_pass_encoder, color );
}

void XE::GraphicsService::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	SKIP_CALL( RenderPassEncoderSetIndexBuffer, render_pass_encoder, buffer, format, offset, size );
}

void XE::GraphicsService::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{
	SKIP_CALL( RenderPassEncoderSetPipeline, render_pass_encoder, pipeline );
}

void XE::GraphicsService::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{
	SKIP_CALL( RenderPassEncoderSetScissorRect, render_pass_encoder, rect );
}

void XE::GraphicsService::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{
	SKIP_CALL( RenderPassEncoderSetStencilReference, render_pass_encoder, reference );
}

void XE::GraphicsService::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	SKIP_CALL( RenderPassEncoderSetVertexBuffer, render_pass_encoder, slot, buffer, offset, size );
}

void XE::GraphicsService::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{
	SKIP_CALL( RenderPassEncoderSetViewport, render_pass_encoder, x, y, width, height, min_depth, max_depth );
}

void XE::GraphicsService::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	SKIP_CALL( RenderBundleEncoderDraw, render_bundle_encoder, vertex_count, instance_count, first_vertex, first_instance );
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	SKIP_CALL( RenderBundleEncoderDrawIndexed, render_bundle_encoder, index_count, instance_count, first_index, base_vertex, first_instance );
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	SKIP_CALL( RenderBundleEncoderDrawIndexedIndirect, render_bundle_encoder, indirect_buffer, indirect_offset );
}

void XE::GraphicsService::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	SKIP_CALL( RenderBundleEncoderDrawIndirect, render_bundle_encoder, indirect_buffer, indirect_offset );
}

XE::GraphicsRenderBundlePtr XE::GraphicsService::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	SKIP_CALL( RenderBundleEncoderFinish, render_bundle_encoder, descriptor );
}

void XE::GraphicsService::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{
	SKIP_CALL( RenderBundleEncoderInsertDebugMarker, render_bundle_encoder, marker_label );
}

void XE::GraphicsService::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{
	SKIP_CALL( RenderBundleEncoderPopDebugGroup, render_bundle_encoder );
}

void XE::GraphicsService::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{
	SKIP_CALL( RenderBundleEncoderPushDebugGroup, render_bundle_encoder, group_label );
}

void XE::GraphicsService::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	SKIP_CALL( RenderBundleEncoderSetBindGroup, render_bundle_encoder, group_index, group, dynamic_offsets );
}

void XE::GraphicsService::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	SKIP_CALL( RenderBundleEncoderSetIndexBuffer, render_bundle_encoder, buffer, format, offset, size );
}

void XE::GraphicsService::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{
	SKIP_CALL( RenderBundleEncoderSetPipeline, render_bundle_encoder, pipeline );
}

void XE::GraphicsService::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	SKIP_CALL( RenderBundleEncoderSetVertexBuffer, render_bundle_encoder, slot, buffer, offset, size );
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	SKIP_CALL( ComputePipelineGetBindGroupLayout, compute_pipeline, group_index );
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsService::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	SKIP_CALL( RenderPipelineGetBindGroupLayout, render_pipeline, group_index );
}

void XE::GraphicsService::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, CompilationInfoCallback callback )
{
	SKIP_CALL( ShaderModuleGetCompilationInfo, shader_module, callback );
}

void XE::GraphicsService::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{
	SKIP_CALL( ShaderModuleSetLabel, shader_module, label );
}

XE::GraphicsTextureFormat XE::GraphicsService::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	SKIP_CALL( SurfaceGetPreferredFormat, surface, adapter );
}

XE::GraphicsTextureViewPtr XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	SKIP_CALL( SwapChainGetCurrentTextureView, swap_chain );
}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{
	SKIP_CALL( SwapChainPresent, swap_chain );
}
