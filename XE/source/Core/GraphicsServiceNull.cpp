#include "GraphicsService.h"

#if GRAPHICS_API == GRAPHICS_NULL

IMPLEMENT_META( XE::GraphicsService );

struct XE::GraphicsService::Private
{

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
	return true;
}

void XE::GraphicsService::Update()
{

}

void XE::GraphicsService::Clearup()
{

}

void XE::GraphicsService::AdapterEnumerateFeatures( XE::GraphicsAdapterHandle adapter, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsService::AdapterGetLimits( XE::GraphicsAdapterHandle adapter, XE::GraphicsSupportedLimits & limits )
{

}

void XE::GraphicsService::AdapterGetProperties( XE::GraphicsAdapterHandle adapter, XE::GraphicsAdapterProperties & properties )
{

}

bool XE::GraphicsService::AdapterHasFeature( XE::GraphicsAdapterHandle adapter, XE::GraphicsFeatureName feature )
{

}

void XE::GraphicsService::AdapterRequestDevice( XE::GraphicsAdapterHandle adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback )
{

}

void XE::GraphicsService::BufferDestroy( XE::GraphicsBufferHandle buffer )
{

}

XE::Span< XE::uint8 > XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback )
{

}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{

}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{

}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{

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

}

void XE::GraphicsService::ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle compute_pipeline, const XE::String & label )
{

}

XE::GraphicsBindGroupHandle XE::GraphicsService::DeviceCreateBindGroup( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupDescriptor & descriptor )
{

}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::DeviceCreateBindGroupLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{

}

XE::GraphicsBufferHandle XE::GraphicsService::DeviceCreateBuffer( XE::GraphicsDeviceHandle device, const XE::GraphicsBufferDescriptor & descriptor )
{

}

XE::GraphicsCommandEncoderHandle XE::GraphicsService::DeviceCreateCommandEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{

}

XE::GraphicsComputePipelineHandle XE::GraphicsService::DeviceCreateComputePipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{

}

void XE::GraphicsService::DeviceCreateComputePipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor, CreateComputePipelineAsyncCallback callback )
{

}

XE::GraphicsPipelineLayoutHandle XE::GraphicsService::DeviceCreatePipelineLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{

}

XE::GraphicsQuerySetHandle XE::GraphicsService::DeviceCreateQuerySet( XE::GraphicsDeviceHandle device, const XE::GraphicsQuerySetDescriptor & descriptor )
{

}

XE::GraphicsRenderBundleEncoderHandle XE::GraphicsService::DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{

}

XE::GraphicsRenderPipelineHandle XE::GraphicsService::DeviceCreateRenderPipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{

}

void XE::GraphicsService::DeviceCreateRenderPipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor, CreateRenderPipelineAsyncCallback callback )
{

}

XE::GraphicsSamplerHandle XE::GraphicsService::DeviceCreateSampler( XE::GraphicsDeviceHandle device, const XE::GraphicsSamplerDescriptor & descriptor )
{

}

XE::GraphicsShaderModuleHandle XE::GraphicsService::DeviceCreateShaderModule( XE::GraphicsDeviceHandle device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{

}

XE::GraphicsSwapChainHandle XE::GraphicsService::DeviceCreateSwapChain( XE::GraphicsDeviceHandle device, XE::GraphicsSurfaceHandle surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{

}

XE::GraphicsTextureHandle XE::GraphicsService::DeviceCreateTexture( XE::GraphicsDeviceHandle device, const XE::GraphicsTextureDescriptor & descriptor )
{

}

void XE::GraphicsService::DeviceDestroy( XE::GraphicsDeviceHandle device )
{

}

void XE::GraphicsService::DeviceEnumerateFeatures( XE::GraphicsDeviceHandle device, XE::Array< XE::GraphicsFeatureName > & features )
{

}

bool XE::GraphicsService::DeviceGetLimits( XE::GraphicsDeviceHandle device, XE::GraphicsSupportedLimits & limits )
{

}

XE::GraphicsQueueHandle XE::GraphicsService::DeviceGetQueue( XE::GraphicsDeviceHandle device )
{

}

bool XE::GraphicsService::DeviceHasFeature( XE::GraphicsDeviceHandle device, XE::GraphicsFeatureName feature )
{

}

bool XE::GraphicsService::DevicePopErrorScope( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{

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

}

XE::GraphicsTextureViewHandle XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swap_chain )
{

}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainHandle swap_chain )
{

}

XE::GraphicsTextureViewHandle XE::GraphicsService::TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{

}

void XE::GraphicsService::TextureDestroy( XE::GraphicsTextureHandle texture )
{

}

#endif // GRAPHICS_API == GRAPHICS_NULL
