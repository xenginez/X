#include "GraphicsServiceD3D11.h"

#if GRAPHICS_API & GRAPHICS_D3D11

namespace
{
	DECL_PTR( D3D11GraphicsSurface );
	DECL_PTR( D3D11GraphicsSwapChain );
	DECL_PTR( D3D11GraphicsAdapter );
	DECL_PTR( D3D11GraphicsDevice );
	DECL_PTR( D3D11GraphicsQueue );
	DECL_PTR( D3D11GraphicsBindGroup );
	DECL_PTR( D3D11GraphicsBindGroupLayout );
	DECL_PTR( D3D11GraphicsBuffer );
	DECL_PTR( D3D11GraphicsCommandBuffer );
	DECL_PTR( D3D11GraphicsCommandEncoder );
	DECL_PTR( D3D11GraphicsComputePassEncoder );
	DECL_PTR( D3D11GraphicsComputePipeline );
	DECL_PTR( D3D11GraphicsPipelineLayout );
	DECL_PTR( D3D11GraphicsQuerySet );
	DECL_PTR( D3D11GraphicsRenderBundle );
	DECL_PTR( D3D11GraphicsRenderBundleEncoder );
	DECL_PTR( D3D11GraphicsRenderPassEncoder );
	DECL_PTR( D3D11GraphicsRenderPipeline );
	DECL_PTR( D3D11GraphicsSampler );
	DECL_PTR( D3D11GraphicsShaderModule );
	DECL_PTR( D3D11GraphicsTexture );
	DECL_PTR( D3D11GraphicsTextureView );

	class D3D11GraphicsSurface : public XE::GraphicsSurface {};
	class D3D11GraphicsSwapChain : public XE::GraphicsSwapChain {};
	class D3D11GraphicsAdapter : public XE::GraphicsAdapter {};
	class D3D11GraphicsDevice : public XE::GraphicsDevice {};
	class D3D11GraphicsQueue : public XE::GraphicsQueue {};
	class D3D11GraphicsBindGroup : public XE::GraphicsBindGroup {};
	class D3D11GraphicsBindGroupLayout : public XE::GraphicsBindGroupLayout {};
	class D3D11GraphicsBuffer : public XE::GraphicsBuffer {};
	class D3D11GraphicsCommandBuffer : public XE::GraphicsCommandBuffer {};
	class D3D11GraphicsCommandEncoder : public XE::GraphicsCommandEncoder {};
	class D3D11GraphicsComputePassEncoder : public XE::GraphicsComputePassEncoder {};
	class D3D11GraphicsComputePipeline : public XE::GraphicsComputePipeline {};
	class D3D11GraphicsPipelineLayout : public XE::GraphicsPipelineLayout {};
	class D3D11GraphicsQuerySet : public XE::GraphicsQuerySet {};
	class D3D11GraphicsRenderBundle : public XE::GraphicsRenderBundle {};
	class D3D11GraphicsRenderBundleEncoder : public XE::GraphicsRenderBundleEncoder {};
	class D3D11GraphicsRenderPassEncoder : public XE::GraphicsRenderPassEncoder {};
	class D3D11GraphicsRenderPipeline : public XE::GraphicsRenderPipeline {};
	class D3D11GraphicsSampler : public XE::GraphicsSampler {};
	class D3D11GraphicsShaderModule : public XE::GraphicsShaderModule {};
	class D3D11GraphicsTexture : public XE::GraphicsTexture {};
	class D3D11GraphicsTextureView : public XE::GraphicsTextureView {};
}

XE::GraphicsServiceD3D11::GraphicsServiceD3D11( bool debug /*= false */ )
{

}

XE::GraphicsServiceD3D11::~GraphicsServiceD3D11()
{

}

void XE::GraphicsServiceD3D11::Prepare()
{

}

void XE::GraphicsServiceD3D11::Startup()
{

}

void XE::GraphicsServiceD3D11::Update()
{

}

void XE::GraphicsServiceD3D11::Clearup()
{

}

XE::GraphicsSurfacePtr XE::GraphicsServiceD3D11::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceD3D11::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, XE::GraphicsService::RequestAdapterCallback callback )
{

}

void XE::GraphicsServiceD3D11::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceD3D11::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

void XE::GraphicsServiceD3D11::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{

}

bool XE::GraphicsServiceD3D11::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	return {};
}

void XE::GraphicsServiceD3D11::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, XE::GraphicsService::RequestDeviceCallback callback )
{

}

XE::GraphicsBindGroupPtr XE::GraphicsServiceD3D11::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceD3D11::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsBufferPtr XE::GraphicsServiceD3D11::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	return {};
}

XE::GraphicsCommandEncoderPtr XE::GraphicsServiceD3D11::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsComputePipelinePtr XE::GraphicsServiceD3D11::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsServiceD3D11::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	return {};
}

XE::GraphicsQuerySetPtr XE::GraphicsServiceD3D11::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsServiceD3D11::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPipelinePtr XE::GraphicsServiceD3D11::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSamplerPtr XE::GraphicsServiceD3D11::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	return {};
}

XE::GraphicsShaderModulePtr XE::GraphicsServiceD3D11::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	return {};
}

XE::GraphicsSwapChainPtr XE::GraphicsServiceD3D11::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTexturePtr XE::GraphicsServiceD3D11::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceD3D11::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceD3D11::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsServiceD3D11::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	return {};
}

XE::GraphicsQueuePtr XE::GraphicsServiceD3D11::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	return {};
}

bool XE::GraphicsServiceD3D11::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	return {};
}

bool XE::GraphicsServiceD3D11::DevicePopErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	return {};
}

void XE::GraphicsServiceD3D11::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{

}

void XE::GraphicsServiceD3D11::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::DeviceLostCallback callback )
{

}

void XE::GraphicsServiceD3D11::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{

}

void XE::GraphicsServiceD3D11::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, XE::GraphicsService::QueueWorkDoneCallback callback )
{

}

void XE::GraphicsServiceD3D11::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{

}

void XE::GraphicsServiceD3D11::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{

}

void XE::GraphicsServiceD3D11::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{

}

void XE::GraphicsServiceD3D11::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, XE::GraphicsService::BufferMapCallback callback )
{

}

XE::Span< const XE::uint8 > XE::GraphicsServiceD3D11::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

XE::Span< XE::uint8 > XE::GraphicsServiceD3D11::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return {};
}

void XE::GraphicsServiceD3D11::BufferUnmap( XE::GraphicsBufferPtr buffer )
{

}

XE::GraphicsComputePassEncoderPtr XE::GraphicsServiceD3D11::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	return {};
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsServiceD3D11::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceD3D11::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr source, XE::uint64 source_offset, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset, XE::uint64 size )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size )
{

}

XE::GraphicsCommandBufferPtr XE::GraphicsServiceD3D11::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceD3D11::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset )
{

}

void XE::GraphicsServiceD3D11::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceD3D11::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceD3D11::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{

}

XE::GraphicsRenderBundlePtr XE::GraphicsServiceD3D11::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	return {};
}

void XE::GraphicsServiceD3D11::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{

}

void XE::GraphicsServiceD3D11::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{

}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceD3D11::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	return {};
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceD3D11::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	return {};
}

void XE::GraphicsServiceD3D11::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, XE::GraphicsService::CompilationInfoCallback callback )
{

}

void XE::GraphicsServiceD3D11::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{

}

XE::GraphicsTextureFormat XE::GraphicsServiceD3D11::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	return {};
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceD3D11::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	return {};
}

void XE::GraphicsServiceD3D11::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{

}

#endif
