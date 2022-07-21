#include "GraphicsServiceVulkan.h"

#if GRAPHICS_API & GRAPHICS_VULKAN

#define VKBIND_IMPLEMENTATION
#include <vkbind/vkbind.h>

#include "Utils/Logger.h"

namespace
{
	DECL_PTR( VKGraphicsSurface );
	DECL_PTR( VKGraphicsSwapChain );
	DECL_PTR( VKGraphicsAdapter );
	DECL_PTR( VKGraphicsDevice );
	DECL_PTR( VKGraphicsQueue );
	DECL_PTR( VKGraphicsBindGroup );
	DECL_PTR( VKGraphicsBindGroupLayout );
	DECL_PTR( VKGraphicsBuffer );
	DECL_PTR( VKGraphicsCommandBuffer );
	DECL_PTR( VKGraphicsCommandEncoder );
	DECL_PTR( VKGraphicsComputePassEncoder );
	DECL_PTR( VKGraphicsComputePipeline );
	DECL_PTR( VKGraphicsPipelineLayout );
	DECL_PTR( VKGraphicsQuerySet );
	DECL_PTR( VKGraphicsRenderBundle );
	DECL_PTR( VKGraphicsRenderBundleEncoder );
	DECL_PTR( VKGraphicsRenderPassEncoder );
	DECL_PTR( VKGraphicsRenderPipeline );
	DECL_PTR( VKGraphicsSampler );
	DECL_PTR( VKGraphicsShaderModule );
	DECL_PTR( VKGraphicsTexture );
	DECL_PTR( VKGraphicsTextureView );

	class VKGraphicsSurface : public XE::GraphicsSurface { };
	class VKGraphicsSwapChain : public XE::GraphicsSwapChain {};
	class VKGraphicsAdapter : public XE::GraphicsAdapter {};
	class VKGraphicsDevice : public XE::GraphicsDevice {};
	class VKGraphicsQueue : public XE::GraphicsQueue {};
	class VKGraphicsBindGroup : public XE::GraphicsBindGroup {};
	class VKGraphicsBindGroupLayout : public XE::GraphicsBindGroupLayout {};
	class VKGraphicsBuffer : public XE::GraphicsBuffer {};
	class VKGraphicsCommandBuffer : public XE::GraphicsCommandBuffer {};
	class VKGraphicsCommandEncoder : public XE::GraphicsCommandEncoder {};
	class VKGraphicsComputePassEncoder : public XE::GraphicsComputePassEncoder {};
	class VKGraphicsComputePipeline : public XE::GraphicsComputePipeline {};
	class VKGraphicsPipelineLayout : public XE::GraphicsPipelineLayout {};
	class VKGraphicsQuerySet : public XE::GraphicsQuerySet {};
	class VKGraphicsRenderBundle : public XE::GraphicsRenderBundle {};
	class VKGraphicsRenderBundleEncoder : public XE::GraphicsRenderBundleEncoder {};
	class VKGraphicsRenderPassEncoder : public XE::GraphicsRenderPassEncoder {};
	class VKGraphicsRenderPipeline : public XE::GraphicsRenderPipeline {};
	class VKGraphicsSampler : public XE::GraphicsSampler {};
	class VKGraphicsShaderModule : public XE::GraphicsShaderModule {};
	class VKGraphicsTexture : public XE::GraphicsTexture {};
	class VKGraphicsTextureView : public XE::GraphicsTextureView {};
}

struct XE::GraphicsServiceVulkan::Private
{
	bool _IsDebug = false;

	VkbAPI _API;
	VkInstance _Instance;
	VkDebugReportCallbackEXT _Debug;
};

XE::GraphicsServiceVulkan::GraphicsServiceVulkan( bool debug /*= false*/ )
	:_p( XE::New< Private >() )
{
	_p->_IsDebug = debug;
}

XE::GraphicsServiceVulkan::~GraphicsServiceVulkan()
{
	XE::Delete( _p );
}

void XE::GraphicsServiceVulkan::Prepare()
{
	if ( vkbInit( &_p->_API ) != VK_SUCCESS )
	{
		XE_ERROR( "" );
		return;
	}

	VkInstanceCreateInfo inst_info = {};
	{
		inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		inst_info.pNext = nullptr;
		inst_info.flags = 0;
		inst_info.pApplicationInfo = nullptr;
		inst_info.enabledLayerCount;
		inst_info.ppEnabledLayerNames;
		inst_info.enabledExtensionCount;
		inst_info.ppEnabledExtensionNames;
	}
	VkAllocationCallbacks allocator = {};
	{
		allocator.pfnAllocation = []( void * pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope ) -> void *
		{
			void * p = XE::MemoryResource::GetDefaultMemoryResource()->allocate( size + 2 * sizeof( XE::uint64 ), alignment );

			XE::uint64 * h = reinterpret_cast<XE::uint64 *>( p );
			*( h ) = size;
			*( h + 1 ) = alignment;

			return h + 2;
		};
		allocator.pfnFree = []( void * pUserData, void * pMemory )
		{
			XE::uint64 * h = reinterpret_cast<XE::uint64 *>( pMemory ) - 2;

			XE::uint64 size = *( h );
			XE::uint64 alignment = *( h + 1 );

			XE::MemoryResource::GetDefaultMemoryResource()->deallocate( h, size, alignment );
		};
		allocator.pfnReallocation = []( void * pUserData, void * pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope ) -> void *
		{
			void * new_p = XE::MemoryResource::GetDefaultMemoryResource()->allocate( size + 2 * sizeof( XE::uint64 ), alignment );

			XE::uint64 * new_h = reinterpret_cast<XE::uint64 *>( new_p );
			*( new_h ) = size;
			*( new_h + 1 ) = alignment;

			void * old_p = pOriginal;

			XE::uint64 * old_h = reinterpret_cast<XE::uint64 *>( old_p ) - 2;

			XE::uint64 old_size = *( old_h );
			XE::uint64 old_alignment = *( old_h + 1 );

			std::copy( reinterpret_cast<XE::uint8 *>( old_p ), reinterpret_cast<XE::uint8 *>( old_p ) + size, reinterpret_cast<XE::uint8 *>( new_h + 2 ) );

			XE::MemoryResource::GetDefaultMemoryResource()->deallocate( old_h, old_size, old_alignment );

			return new_h + 2;
		};
		allocator.pUserData = nullptr;
	}
	if ( vkCreateInstance( &inst_info, &allocator, &_p->_Instance ) != VK_SUCCESS )
	{
		XE_ERROR( "" );
		return;
	}

	if ( vkbInitInstanceAPI( _p->_Instance, &_p->_API ) != VK_SUCCESS )
	{
		XE_ERROR( "" );
		return;
	}

	vkbBindAPI( &_p->_API );

	if ( _p->_IsDebug )
	{
		VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;
		debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugReportCallbackCreateInfo.pNext = NULL;
		debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		debugReportCallbackCreateInfo.pfnCallback = []( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char * pLayerPrefix, const char * pMessage, void * pUserData ) -> VkBool32
		{
			return 0;
		};
		debugReportCallbackCreateInfo.pUserData = NULL;

		if ( vkCreateDebugReportCallbackEXT( _p->_Instance, &debugReportCallbackCreateInfo, NULL, &_p->_Debug ) != VK_SUCCESS )
		{
			XE_ERROR( "" );
		}
	}


}

void XE::GraphicsServiceVulkan::Startup()
{

}

void XE::GraphicsServiceVulkan::Update()
{

}

void XE::GraphicsServiceVulkan::Clearup()
{

	vkDestroyInstance( _p->_Instance, nullptr );
	vkbUninit();
}

XE::GraphicsSurfacePtr XE::GraphicsServiceVulkan::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceVulkan::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, XE::GraphicsService::RequestAdapterCallback callback )
{

}

void XE::GraphicsServiceVulkan::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceVulkan::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

void XE::GraphicsServiceVulkan::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{

}

bool XE::GraphicsServiceVulkan::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	return {};
}

void XE::GraphicsServiceVulkan::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, XE::GraphicsService::RequestDeviceCallback callback )
{

}

XE::GraphicsBindGroupPtr XE::GraphicsServiceVulkan::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceVulkan::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBufferPtr XE::GraphicsServiceVulkan::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	return {};
}

XE::GraphicsCommandEncoderPtr XE::GraphicsServiceVulkan::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsComputePipelinePtr XE::GraphicsServiceVulkan::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsServiceVulkan::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsQuerySetPtr XE::GraphicsServiceVulkan::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsServiceVulkan::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPipelinePtr XE::GraphicsServiceVulkan::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSamplerPtr XE::GraphicsServiceVulkan::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	return {};
}

XE::GraphicsShaderModulePtr XE::GraphicsServiceVulkan::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSwapChainPtr XE::GraphicsServiceVulkan::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTexturePtr XE::GraphicsServiceVulkan::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceVulkan::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceVulkan::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceVulkan::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

XE::GraphicsQueuePtr XE::GraphicsServiceVulkan::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	return {};
}

bool XE::GraphicsServiceVulkan::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	return {};
}

bool XE::GraphicsServiceVulkan::DevicePopErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	return {};
}

void XE::GraphicsServiceVulkan::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{

}

void XE::GraphicsServiceVulkan::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::DeviceLostCallback callback )
{

}

void XE::GraphicsServiceVulkan::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{

}

void XE::GraphicsServiceVulkan::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, XE::GraphicsService::QueueWorkDoneCallback callback )
{

}

void XE::GraphicsServiceVulkan::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{

}

void XE::GraphicsServiceVulkan::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{

}

void XE::GraphicsServiceVulkan::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{

}

void XE::GraphicsServiceVulkan::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, XE::GraphicsService::BufferMapCallback callback )
{

}

XE::Span< const XE::uint8 > XE::GraphicsServiceVulkan::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

XE::Span< XE::uint8 > XE::GraphicsServiceVulkan::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

void XE::GraphicsServiceVulkan::BufferUnmap( XE::GraphicsBufferPtr buffer )
{

}

XE::GraphicsComputePassEncoderPtr XE::GraphicsServiceVulkan::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsServiceVulkan::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceVulkan::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr source, XE::uint64 source_offset, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset, XE::uint64 size )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

XE::GraphicsCommandBufferPtr XE::GraphicsServiceVulkan::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceVulkan::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset )
{

}

void XE::GraphicsServiceVulkan::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceVulkan::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceVulkan::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

XE::GraphicsRenderBundlePtr XE::GraphicsServiceVulkan::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceVulkan::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceVulkan::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceVulkan::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceVulkan::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	return {};
}

void XE::GraphicsServiceVulkan::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, XE::GraphicsService::CompilationInfoCallback callback )
{

}

void XE::GraphicsServiceVulkan::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{

}

XE::GraphicsTextureFormat XE::GraphicsServiceVulkan::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceVulkan::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	return {};
}

void XE::GraphicsServiceVulkan::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{

}

#endif