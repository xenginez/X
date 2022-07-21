#include "GraphicsServiceOpenGLES.h"

#if GRAPHICS_API & GRAPHICS_OPENGLES

namespace
{
	DECL_PTR( ESGraphicsSurface );
	DECL_PTR( ESGraphicsSwapChain );
	DECL_PTR( ESGraphicsAdapter );
	DECL_PTR( ESGraphicsDevice );
	DECL_PTR( ESGraphicsQueue );
	DECL_PTR( ESGraphicsBindGroup );
	DECL_PTR( ESGraphicsBindGroupLayout );
	DECL_PTR( ESGraphicsBuffer );
	DECL_PTR( ESGraphicsCommandBuffer );
	DECL_PTR( ESGraphicsCommandEncoder );
	DECL_PTR( ESGraphicsComputePassEncoder );
	DECL_PTR( ESGraphicsComputePipeline );
	DECL_PTR( ESGraphicsPipelineLayout );
	DECL_PTR( ESGraphicsQuerySet );
	DECL_PTR( ESGraphicsRenderBundle );
	DECL_PTR( ESGraphicsRenderBundleEncoder );
	DECL_PTR( ESGraphicsRenderPassEncoder );
	DECL_PTR( ESGraphicsRenderPipeline );
	DECL_PTR( ESGraphicsSampler );
	DECL_PTR( ESGraphicsShaderModule );
	DECL_PTR( ESGraphicsTexture );
	DECL_PTR( ESGraphicsTextureView );

	class ESGraphicsSurface : public XE::GraphicsSurface {};
	class ESGraphicsSwapChain : public XE::GraphicsSwapChain {};
	class ESGraphicsAdapter : public XE::GraphicsAdapter {};
	class ESGraphicsDevice : public XE::GraphicsDevice {};
	class ESGraphicsQueue : public XE::GraphicsQueue {};
	class ESGraphicsBindGroup : public XE::GraphicsBindGroup {};
	class ESGraphicsBindGroupLayout : public XE::GraphicsBindGroupLayout {};
	class ESGraphicsBuffer : public XE::GraphicsBuffer {};
	class ESGraphicsCommandBuffer : public XE::GraphicsCommandBuffer {};
	class ESGraphicsCommandEncoder : public XE::GraphicsCommandEncoder {};
	class ESGraphicsComputePassEncoder : public XE::GraphicsComputePassEncoder {};
	class ESGraphicsComputePipeline : public XE::GraphicsComputePipeline {};
	class ESGraphicsPipelineLayout : public XE::GraphicsPipelineLayout {};
	class ESGraphicsQuerySet : public XE::GraphicsQuerySet {};
	class ESGraphicsRenderBundle : public XE::GraphicsRenderBundle {};
	class ESGraphicsRenderBundleEncoder : public XE::GraphicsRenderBundleEncoder {};
	class ESGraphicsRenderPassEncoder : public XE::GraphicsRenderPassEncoder {};
	class ESGraphicsRenderPipeline : public XE::GraphicsRenderPipeline {};
	class ESGraphicsSampler : public XE::GraphicsSampler {};
	class ESGraphicsShaderModule : public XE::GraphicsShaderModule {};
	class ESGraphicsTexture : public XE::GraphicsTexture {};
	class ESGraphicsTextureView : public XE::GraphicsTextureView {};
}

XE::GraphicsServiceOpenGLES::GraphicsServiceOpenGLES( bool debug /*= false */ )
{

}

XE::GraphicsServiceOpenGLES::~GraphicsServiceOpenGLES()
{

}

void XE::GraphicsServiceOpenGLES::Prepare()
{

}

void XE::GraphicsServiceOpenGLES::Startup()
{

}

void XE::GraphicsServiceOpenGLES::Update()
{

}

void XE::GraphicsServiceOpenGLES::Clearup()
{

}

XE::GraphicsSurfacePtr XE::GraphicsServiceOpenGLES::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, XE::GraphicsService::RequestAdapterCallback callback )
{

}

void XE::GraphicsServiceOpenGLES::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceOpenGLES::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{

}

bool XE::GraphicsServiceOpenGLES::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, XE::GraphicsService::RequestDeviceCallback callback )
{

}

XE::GraphicsBindGroupPtr XE::GraphicsServiceOpenGLES::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceOpenGLES::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBufferPtr XE::GraphicsServiceOpenGLES::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	return {};
}

XE::GraphicsCommandEncoderPtr XE::GraphicsServiceOpenGLES::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsComputePipelinePtr XE::GraphicsServiceOpenGLES::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsServiceOpenGLES::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsQuerySetPtr XE::GraphicsServiceOpenGLES::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsServiceOpenGLES::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPipelinePtr XE::GraphicsServiceOpenGLES::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSamplerPtr XE::GraphicsServiceOpenGLES::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	return {};
}

XE::GraphicsShaderModulePtr XE::GraphicsServiceOpenGLES::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSwapChainPtr XE::GraphicsServiceOpenGLES::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTexturePtr XE::GraphicsServiceOpenGLES::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceOpenGLES::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceOpenGLES::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

XE::GraphicsQueuePtr XE::GraphicsServiceOpenGLES::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	return {};
}

bool XE::GraphicsServiceOpenGLES::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	return {};
}

bool XE::GraphicsServiceOpenGLES::DevicePopErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{

}

void XE::GraphicsServiceOpenGLES::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::DeviceLostCallback callback )
{

}

void XE::GraphicsServiceOpenGLES::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{

}

void XE::GraphicsServiceOpenGLES::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, XE::GraphicsService::QueueWorkDoneCallback callback )
{

}

void XE::GraphicsServiceOpenGLES::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{

}

void XE::GraphicsServiceOpenGLES::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{

}

void XE::GraphicsServiceOpenGLES::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{

}

void XE::GraphicsServiceOpenGLES::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, XE::GraphicsService::BufferMapCallback callback )
{

}

XE::Span< const XE::uint8 > XE::GraphicsServiceOpenGLES::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

XE::Span< XE::uint8 > XE::GraphicsServiceOpenGLES::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::BufferUnmap( XE::GraphicsBufferPtr buffer )
{

}

XE::GraphicsComputePassEncoderPtr XE::GraphicsServiceOpenGLES::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsServiceOpenGLES::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr source, XE::uint64 source_offset, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

XE::GraphicsCommandBufferPtr XE::GraphicsServiceOpenGLES::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset )
{

}

void XE::GraphicsServiceOpenGLES::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceOpenGLES::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGLES::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

XE::GraphicsRenderBundlePtr XE::GraphicsServiceOpenGLES::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceOpenGLES::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceOpenGLES::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceOpenGLES::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, XE::GraphicsService::CompilationInfoCallback callback )
{

}

void XE::GraphicsServiceOpenGLES::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{

}

XE::GraphicsTextureFormat XE::GraphicsServiceOpenGLES::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceOpenGLES::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	return {};
}

void XE::GraphicsServiceOpenGLES::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{

}

#endif