#include "GraphicsService.h"

#if defined( NULL_RENDER )

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

XE::MemoryView XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::GraphicsMapModeFlags mode, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback )
{

}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{

}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsComputePassDescriptor & descriptor )
{

}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{

}

void XE::GraphicsService::CommandEncoderClearBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsBufferHandle source, XE::uint64 sourceOffset, XE::GraphicsBufferHandle destination, XE::uint64 destinationOffset, XE::uint64 size )
{

}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::GraphicsExtent3D & copySize )
{

}

void XE::GraphicsService::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::GraphicsExtent3D & copySize )
{

}

void XE::GraphicsService::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::GraphicsExtent3D & copySize )
{

}

XE::GraphicsCommandBufferHandle XE::GraphicsService::CommandEncoderFinish( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{

}

void XE::GraphicsService::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::String & markerLabel )
{

}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle commandEncoder )
{

}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::String & groupLabel )
{

}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 firstQuery, XE::uint32 queryCount, XE::GraphicsBufferHandle destination, XE::uint64 destinationOffset )
{

}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex )
{

}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex )
{

}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::uint32 workgroupCountX, XE::uint32 workgroupCountY, XE::uint32 workgroupCountZ )
{

}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{

}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle computePassEncoder )
{

}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle computePassEncoder )
{

}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle computePassEncoder, const XE::String & markerLabel )
{

}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder )
{

}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder, const XE::String & groupLabel )
{

}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets )
{

}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsComputePipelineHandle pipeline )
{

}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle computePipeline, XE::uint32 groupIndex )
{

}

void XE::GraphicsService::ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle computePipeline, const XE::String & label )
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

void XE::GraphicsService::QuerySetDestroy( XE::GraphicsQuerySetHandle querySet )
{

}

void XE::GraphicsService::QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback )
{

}

void XE::GraphicsService::QueueSubmit( XE::GraphicsQueueHandle queue, XE::uint32 commandCount, const XE::GraphicsCommandBufferHandle & commands )
{

}

void XE::GraphicsService::QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 bufferOffset, XE::MemoryView data )
{

}

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & dataLayout, const XE::GraphicsExtent3D & writeSize )
{

}

void XE::GraphicsService::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 vertexCount, XE::uint32 instanceCount, XE::uint32 firstVertex, XE::uint32 firstInstance )
{

}

void XE::GraphicsService::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 indexCount, XE::uint32 instanceCount, XE::uint32 firstIndex, XE::int32 baseVertex, XE::uint32 firstInstance )
{

}

void XE::GraphicsService::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{

}

void XE::GraphicsService::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{

}

XE::GraphicsRenderBundleHandle XE::GraphicsService::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{

}

void XE::GraphicsService::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::String & markerLabel )
{

}

void XE::GraphicsService::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder )
{

}

void XE::GraphicsService::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::String & groupLabel )
{

}

void XE::GraphicsService::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets )
{

}

void XE::GraphicsService::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsRenderPipelineHandle pipeline )
{

}

void XE::GraphicsService::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 queryIndex )
{

}

void XE::GraphicsService::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex )
{

}

void XE::GraphicsService::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 vertexCount, XE::uint32 instanceCount, XE::uint32 firstVertex, XE::uint32 firstInstance )
{

}

void XE::GraphicsService::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 indexCount, XE::uint32 instanceCount, XE::uint32 firstIndex, XE::int32 baseVertex, XE::uint32 firstInstance )
{

}

void XE::GraphicsService::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{

}

void XE::GraphicsService::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{

}

void XE::GraphicsService::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{

}

void XE::GraphicsService::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{

}

void XE::GraphicsService::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{

}

void XE::GraphicsService::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 bundlesCount, XE::GraphicsRenderBundleHandle bundles )
{

}

void XE::GraphicsService::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::String & markerLabel )
{

}

void XE::GraphicsService::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{

}

void XE::GraphicsService::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::String & groupLabel )
{

}

void XE::GraphicsService::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets )
{

}

void XE::GraphicsService::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::Color & color )
{

}

void XE::GraphicsService::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsRenderPipelineHandle pipeline )
{

}

void XE::GraphicsService::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 x, XE::uint32 y, XE::uint32 width, XE::uint32 height )
{

}

void XE::GraphicsService::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 reference )
{

}

void XE::GraphicsService::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{

}

void XE::GraphicsService::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, float x, float y, float width, float height, float minDepth, float maxDepth )
{

}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle renderPipeline, XE::uint32 groupIndex )
{

}

void XE::GraphicsService::RenderPipelineSetLabel( XE::GraphicsRenderPipelineHandle renderPipeline, const XE::String & label )
{

}

void XE::GraphicsService::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shaderModule, CompilationInfoCallback callback )
{

}

void XE::GraphicsService::ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shaderModule, const XE::String & label )
{

}

XE::GraphicsTextureFormat XE::GraphicsService::SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter )
{

}

XE::GraphicsTextureViewHandle XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swapChain )
{

}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainHandle swapChain )
{

}

XE::GraphicsTextureViewHandle XE::GraphicsService::TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{

}

void XE::GraphicsService::TextureDestroy( XE::GraphicsTextureHandle texture )
{

}

#endif // defined( NULL_RENDER )
