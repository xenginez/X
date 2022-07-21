#include "GraphicsServiceOpenGL.h"

#if GRAPHICS_API & GRAPHICS_OPENGL

#define GLBIND_IMPLEMENTATION
#include <glbind/glbind.h>

namespace
{
	DECL_PTR( GLGraphicsSurface );
	DECL_PTR( GLGraphicsSwapChain );
	DECL_PTR( GLGraphicsAdapter );
	DECL_PTR( GLGraphicsDevice );
	DECL_PTR( GLGraphicsQueue );
	DECL_PTR( GLGraphicsBindGroup );
	DECL_PTR( GLGraphicsBindGroupLayout );
	DECL_PTR( GLGraphicsBuffer );
	DECL_PTR( GLGraphicsCommandBuffer );
	DECL_PTR( GLGraphicsCommandEncoder );
	DECL_PTR( GLGraphicsComputePassEncoder );
	DECL_PTR( GLGraphicsComputePipeline );
	DECL_PTR( GLGraphicsPipelineLayout );
	DECL_PTR( GLGraphicsQuerySet );
	DECL_PTR( GLGraphicsRenderBundle );
	DECL_PTR( GLGraphicsRenderBundleEncoder );
	DECL_PTR( GLGraphicsRenderPassEncoder );
	DECL_PTR( GLGraphicsRenderPipeline );
	DECL_PTR( GLGraphicsSampler );
	DECL_PTR( GLGraphicsShaderModule );
	DECL_PTR( GLGraphicsTexture );
	DECL_PTR( GLGraphicsTextureView );

	class GLGraphicsSurface : public XE::GraphicsSurface {};
	class GLGraphicsSwapChain : public XE::GraphicsSwapChain {};
	class GLGraphicsAdapter : public XE::GraphicsAdapter {};
	class GLGraphicsDevice : public XE::GraphicsDevice {};
	class GLGraphicsQueue : public XE::GraphicsQueue {};
	class GLGraphicsBindGroup : public XE::GraphicsBindGroup {};
	class GLGraphicsBindGroupLayout : public XE::GraphicsBindGroupLayout {};
	class GLGraphicsBuffer : public XE::GraphicsBuffer {};
	class GLGraphicsCommandBuffer : public XE::GraphicsCommandBuffer {};
	class GLGraphicsCommandEncoder : public XE::GraphicsCommandEncoder {};
	class GLGraphicsComputePassEncoder : public XE::GraphicsComputePassEncoder {};
	class GLGraphicsComputePipeline : public XE::GraphicsComputePipeline {};
	class GLGraphicsPipelineLayout : public XE::GraphicsPipelineLayout {};
	class GLGraphicsQuerySet : public XE::GraphicsQuerySet {};
	class GLGraphicsRenderBundle : public XE::GraphicsRenderBundle {};
	class GLGraphicsRenderBundleEncoder : public XE::GraphicsRenderBundleEncoder {};
	class GLGraphicsRenderPassEncoder : public XE::GraphicsRenderPassEncoder {};
	class GLGraphicsRenderPipeline : public XE::GraphicsRenderPipeline {};
	class GLGraphicsSampler : public XE::GraphicsSampler {};
	class GLGraphicsShaderModule : public XE::GraphicsShaderModule {};
	class GLGraphicsTexture : public XE::GraphicsTexture {};
	class GLGraphicsTextureView : public XE::GraphicsTextureView {};
}

XE::GraphicsServiceOpenGL::GraphicsServiceOpenGL( bool debug /*= false */ )
{

}

XE::GraphicsServiceOpenGL::~GraphicsServiceOpenGL()
{

}

void XE::GraphicsServiceOpenGL::Prepare()
{

}

void XE::GraphicsServiceOpenGL::Startup()
{

}

void XE::GraphicsServiceOpenGL::Update()
{

}

void XE::GraphicsServiceOpenGL::Clearup()
{

}

XE::GraphicsSurfacePtr XE::GraphicsServiceOpenGL::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGL::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, XE::GraphicsService::RequestAdapterCallback callback )
{

}

void XE::GraphicsServiceOpenGL::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceOpenGL::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

void XE::GraphicsServiceOpenGL::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{

}

bool XE::GraphicsServiceOpenGL::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	return {};
}

void XE::GraphicsServiceOpenGL::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, XE::GraphicsService::RequestDeviceCallback callback )
{

}

XE::GraphicsBindGroupPtr XE::GraphicsServiceOpenGL::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceOpenGL::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBufferPtr XE::GraphicsServiceOpenGL::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	return {};
}

XE::GraphicsCommandEncoderPtr XE::GraphicsServiceOpenGL::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsComputePipelinePtr XE::GraphicsServiceOpenGL::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsServiceOpenGL::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsQuerySetPtr XE::GraphicsServiceOpenGL::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsServiceOpenGL::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPipelinePtr XE::GraphicsServiceOpenGL::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSamplerPtr XE::GraphicsServiceOpenGL::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	return {};
}

XE::GraphicsShaderModulePtr XE::GraphicsServiceOpenGL::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSwapChainPtr XE::GraphicsServiceOpenGL::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTexturePtr XE::GraphicsServiceOpenGL::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceOpenGL::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGL::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceOpenGL::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

XE::GraphicsQueuePtr XE::GraphicsServiceOpenGL::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	return {};
}

bool XE::GraphicsServiceOpenGL::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	return {};
}

bool XE::GraphicsServiceOpenGL::DevicePopErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	return {};
}

void XE::GraphicsServiceOpenGL::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{

}

void XE::GraphicsServiceOpenGL::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::DeviceLostCallback callback )
{

}

void XE::GraphicsServiceOpenGL::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{

}

void XE::GraphicsServiceOpenGL::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, XE::GraphicsService::QueueWorkDoneCallback callback )
{

}

void XE::GraphicsServiceOpenGL::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{

}

void XE::GraphicsServiceOpenGL::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{

}

void XE::GraphicsServiceOpenGL::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{

}

void XE::GraphicsServiceOpenGL::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, XE::GraphicsService::BufferMapCallback callback )
{

}

XE::Span< const XE::uint8 > XE::GraphicsServiceOpenGL::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

XE::Span< XE::uint8 > XE::GraphicsServiceOpenGL::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

void XE::GraphicsServiceOpenGL::BufferUnmap( XE::GraphicsBufferPtr buffer )
{

}

XE::GraphicsComputePassEncoderPtr XE::GraphicsServiceOpenGL::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsServiceOpenGL::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGL::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr source, XE::uint64 source_offset, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

XE::GraphicsCommandBufferPtr XE::GraphicsServiceOpenGL::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGL::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset )
{

}

void XE::GraphicsServiceOpenGL::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceOpenGL::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGL::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

XE::GraphicsRenderBundlePtr XE::GraphicsServiceOpenGL::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceOpenGL::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceOpenGL::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceOpenGL::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	return {};
}

void XE::GraphicsServiceOpenGL::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, XE::GraphicsService::CompilationInfoCallback callback )
{

}

void XE::GraphicsServiceOpenGL::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{

}

XE::GraphicsTextureFormat XE::GraphicsServiceOpenGL::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceOpenGL::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	return {};
}

void XE::GraphicsServiceOpenGL::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{

}

#endif