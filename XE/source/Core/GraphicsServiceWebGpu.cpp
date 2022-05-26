#include "GraphicsService.h"

#if !defined( NULL_RENDER ) && ( PLATFORM_OS & OS_WASM )

#include <webgpu/webgpu.h>

IMPLEMENT_META( XE::GraphicsService );

// BEG_XE_NAMESPACE
// #define RENAME_TYPE( NAME ) using Graphics##NAME = WGPU##NAME;
// RENAME_TYPE( Instance );
// RENAME_TYPE( Adapter );
// RENAME_TYPE( Device );
// RENAME_TYPE( BindGroup );
// RENAME_TYPE( BindGroupLayout );
// RENAME_TYPE( Buffer );
// RENAME_TYPE( CommandBuffer );
// RENAME_TYPE( CommandEncoder );
// RENAME_TYPE( ComputePassEncoder );
// RENAME_TYPE( ComputePipeline );
// RENAME_TYPE( PipelineLayout );
// RENAME_TYPE( QuerySet );
// RENAME_TYPE( Queue );
// RENAME_TYPE( RenderBundle );
// RENAME_TYPE( RenderBundleEncoder );
// RENAME_TYPE( RenderPassEncoder );
// RENAME_TYPE( RenderPipeline );
// RENAME_TYPE( Sampler );
// RENAME_TYPE( ShaderModule );
// RENAME_TYPE( Surface );
// RENAME_TYPE( SwapChain );
// RENAME_TYPE( Texture );
// RENAME_TYPE( TextureView );
// #undef RENAME_TYPE
// END_XE_NAMESPACE

XE::GraphicsService::GraphicsService()
	:_p( nullptr )
{

}

XE::GraphicsService::~GraphicsService()
{

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
	features.resize( 10 );
	features.resize( wgpuAdapterEnumerateFeatures( reinterpret_cast<WGPUAdapter>( adapter.GetValue() ), reinterpret_cast<WGPUFeatureName *>( features.data() ) ) );
}

bool XE::GraphicsService::AdapterGetLimits( XE::GraphicsAdapterHandle adapter, XE::GraphicsSupportedLimits & limits )
{
	return wgpuAdapterGetLimits( reinterpret_cast<WGPUAdapter>( adapter.GetValue() ), reinterpret_cast<WGPUSupportedLimits *>( &limits ) );
}

void XE::GraphicsService::AdapterGetProperties( XE::GraphicsAdapterHandle adapter, XE::GraphicsAdapterProperties & properties )
{
	wgpuAdapterGetProperties( reinterpret_cast<WGPUAdapter>( adapter.GetValue() ), reinterpret_cast<WGPUAdapterProperties *>( &properties ) );
}

bool XE::GraphicsService::AdapterHasFeature( XE::GraphicsAdapterHandle adapter, XE::GraphicsFeatureName feature )
{
	return wgpuAdapterHasFeature( reinterpret_cast<WGPUAdapter>( adapter.GetValue() ), static_cast<WGPUFeatureName>( feature ) );
}

void XE::GraphicsService::AdapterRequestDevice( XE::GraphicsAdapterHandle adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback )
{
	wgpuAdapterRequestDevice( reinterpret_cast<WGPUAdapter>( adapter.GetValue() ), reinterpret_cast<const WGPUDeviceDescriptor *>( &descriptor ),
							  []( WGPURequestDeviceStatus status, WGPUDevice device, char const * message, void * userdata )
	{
		( *reinterpret_cast<RequestDeviceCallback *>( userdata ) )
			( static_cast<XE::GraphicsRequestDeviceStatus>( status ), XE::HandleCast< XE::GraphicsDevice >( reinterpret_cast<XE::uint64>( device ) ), message );
	}, &callback );
}

void XE::GraphicsService::BufferDestroy( XE::GraphicsBufferHandle buffer )
{
	wgpuBufferDestroy( reinterpret_cast<WGPUBuffer>( buffer.GetValue() ) );
}

XE::MemoryView XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	void * p = wgpuBufferGetMappedRange( reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), offset, size );
	return { reinterpret_cast<XE::int8 *>( p ), size };
}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::GraphicsMapModeFlags mode, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback )
{
	wgpuBufferMapAsync( reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), static_cast<WGPUMapModeFlags>( mode ), offset, size,
						[]( WGPUBufferMapAsyncStatus status, void * userdata )
	{
		( *reinterpret_cast<BufferMapCallback *>( userdata ) )( static_cast<XE::GraphicsBufferMapAsyncStatus>( status ) );
	}, &callback );
}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{
	wgpuBufferUnmap( reinterpret_cast<WGPUBuffer>( buffer.GetValue() ) );
}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsComputePassEncoder>( reinterpret_cast<XE::uint64>( wgpuCommandEncoderBeginComputePass( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<const WGPUComputePassDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsRenderPassEncoder>( reinterpret_cast<XE::uint64>( wgpuCommandEncoderBeginRenderPass( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<const WGPURenderPassDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::CommandEncoderClearBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	wgpuCommandEncoderClearBuffer( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), offset, size );
}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsBufferHandle source, XE::uint64 sourceOffset, XE::GraphicsBufferHandle destination, XE::uint64 destinationOffset, XE::uint64 size )
{
	wgpuCommandEncoderCopyBufferToBuffer( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( source.GetValue() ), sourceOffset, reinterpret_cast<WGPUBuffer>( destination.GetValue() ), destinationOffset, size );
}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::GraphicsExtent3D & copySize )
{
	wgpuCommandEncoderCopyBufferToTexture( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<const WGPUImageCopyBuffer *>( &source ), reinterpret_cast<const WGPUImageCopyTexture *>( &destination ), reinterpret_cast<const WGPUExtent3D *>( &copySize ) );
}

void XE::GraphicsService::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::GraphicsExtent3D & copySize )
{
	wgpuCommandEncoderCopyTextureToBuffer( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<const WGPUImageCopyTexture *>( &source ), reinterpret_cast<const WGPUImageCopyBuffer *>( &destination ), reinterpret_cast<const WGPUExtent3D *>( &copySize ) );
}

void XE::GraphicsService::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const XE::GraphicsExtent3D & copySize )
{
	wgpuCommandEncoderCopyTextureToTexture( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<const WGPUImageCopyTexture *>( &source ), reinterpret_cast<const WGPUImageCopyTexture *>( &destination ), reinterpret_cast<const WGPUExtent3D *>( &copySize ) );
}

XE::GraphicsCommandBufferHandle XE::GraphicsService::CommandEncoderFinish( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsCommandBuffer>( reinterpret_cast<XE::uint64>( wgpuCommandEncoderFinish( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<const WGPUCommandBufferDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::String & markerLabel )
{
	wgpuCommandEncoderInsertDebugMarker( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), markerLabel.c_str() );
}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle commandEncoder )
{
	wgpuCommandEncoderPopDebugGroup( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ) );
}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::String & groupLabel )
{
	wgpuCommandEncoderPushDebugGroup( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), groupLabel.c_str() );
}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 firstQuery, XE::uint32 queryCount, XE::GraphicsBufferHandle destination, XE::uint64 destinationOffset )
{
	wgpuCommandEncoderResolveQuerySet( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<WGPUQuerySet>( querySet.GetValue() ), firstQuery, queryCount, reinterpret_cast<WGPUBuffer>( destination.GetValue() ), destinationOffset );
}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex )
{
	wgpuCommandEncoderWriteTimestamp( reinterpret_cast<WGPUCommandEncoder>( commandEncoder.GetValue() ), reinterpret_cast<WGPUQuerySet>( querySet.GetValue() ), queryIndex );
}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex )
{
	wgpuComputePassEncoderBeginPipelineStatisticsQuery( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), reinterpret_cast<WGPUQuerySet>( querySet.GetValue() ), queryIndex );
}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::uint32 workgroupCountX, XE::uint32 workgroupCountY, XE::uint32 workgroupCountZ )
{
	wgpuComputePassEncoderDispatch( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), workgroupCountX, workgroupCountY, workgroupCountZ );
}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{
	wgpuComputePassEncoderDispatchIndirect( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( indirectBuffer.GetValue() ), indirectOffset );
}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle computePassEncoder )
{
	wgpuComputePassEncoderEnd( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ) );
}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle computePassEncoder )
{
	wgpuComputePassEncoderEndPipelineStatisticsQuery( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ) );
}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle computePassEncoder, const XE::String & markerLabel )
{
	wgpuComputePassEncoderInsertDebugMarker( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), markerLabel.c_str() );
}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder )
{
	wgpuComputePassEncoderPopDebugGroup( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ) );
}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder, const XE::String & groupLabel )
{
	wgpuComputePassEncoderPushDebugGroup( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), groupLabel.c_str() );
}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets )
{
	wgpuComputePassEncoderSetBindGroup( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), groupIndex, reinterpret_cast<WGPUBindGroup>( group.GetValue() ), dynamicOffsetCount, &dynamicOffsets );
}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsComputePipelineHandle pipeline )
{
	wgpuComputePassEncoderSetPipeline( reinterpret_cast<WGPUComputePassEncoder>( computePassEncoder.GetValue() ), reinterpret_cast<WGPUComputePipeline>( pipeline.GetValue() ) );
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle computePipeline, XE::uint32 groupIndex )
{
	return XE::HandleCast<XE::GraphicsBindGroupLayout>( reinterpret_cast<XE::uint64>( wgpuComputePipelineGetBindGroupLayout( reinterpret_cast<WGPUComputePipeline>( computePipeline.GetValue() ), groupIndex ) ) );
}

void XE::GraphicsService::ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle computePipeline, const XE::String & label )
{
	wgpuComputePipelineSetLabel( reinterpret_cast<WGPUComputePipeline>( computePipeline.GetValue() ), label.c_str() );
}

XE::GraphicsBindGroupHandle XE::GraphicsService::DeviceCreateBindGroup( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsBindGroup>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateBindGroup( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUBindGroupDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::DeviceCreateBindGroupLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsBindGroupLayout>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateBindGroupLayout( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUBindGroupLayoutDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsBufferHandle XE::GraphicsService::DeviceCreateBuffer( XE::GraphicsDeviceHandle device, const XE::GraphicsBufferDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsBuffer>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateBuffer( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUBufferDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsCommandEncoderHandle XE::GraphicsService::DeviceCreateCommandEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsCommandEncoder>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateCommandEncoder( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUCommandEncoderDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsComputePipelineHandle XE::GraphicsService::DeviceCreateComputePipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsComputePipeline>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateComputePipeline( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUComputePipelineDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::DeviceCreateComputePipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor, CreateComputePipelineAsyncCallback callback )
{
	wgpuDeviceCreateComputePipelineAsync( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUComputePipelineDescriptor *>( &descriptor ), 
										  []( WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, char const * message, void * userdata )
	{
		( *reinterpret_cast<CreateComputePipelineAsyncCallback *>( userdata ) )
			( static_cast<XE::GraphicsCreatePipelineAsyncStatus>( status ), XE::HandleCast<XE::GraphicsComputePipeline>( reinterpret_cast<XE::uint64>( pipeline ) ), message );
	}, &callback );
}

XE::GraphicsPipelineLayoutHandle XE::GraphicsService::DeviceCreatePipelineLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsPipelineLayout>( reinterpret_cast<XE::uint64>( wgpuDeviceCreatePipelineLayout( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUPipelineLayoutDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsQuerySetHandle XE::GraphicsService::DeviceCreateQuerySet( XE::GraphicsDeviceHandle device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsQuerySet>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateQuerySet( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUQuerySetDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsRenderBundleEncoderHandle XE::GraphicsService::DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsRenderBundleEncoder>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateRenderBundleEncoder( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPURenderBundleEncoderDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsRenderPipelineHandle XE::GraphicsService::DeviceCreateRenderPipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsRenderPipeline>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateRenderPipeline( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPURenderPipelineDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::DeviceCreateRenderPipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor, CreateRenderPipelineAsyncCallback callback )
{
	wgpuDeviceCreateRenderPipelineAsync( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPURenderPipelineDescriptor *>( &descriptor ),
										  []( WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, char const * message, void * userdata )
	{
		( *reinterpret_cast<CreateRenderPipelineAsyncCallback *>( userdata ) )
			( static_cast<XE::GraphicsCreatePipelineAsyncStatus>( status ), XE::HandleCast<XE::GraphicsRenderPipeline>( reinterpret_cast<XE::uint64>( pipeline ) ), message );
	}, &callback );
}

XE::GraphicsSamplerHandle XE::GraphicsService::DeviceCreateSampler( XE::GraphicsDeviceHandle device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsSampler>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateSampler( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUSamplerDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsShaderModuleHandle XE::GraphicsService::DeviceCreateShaderModule( XE::GraphicsDeviceHandle device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsShaderModule>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateShaderModule( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUShaderModuleDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsSwapChainHandle XE::GraphicsService::DeviceCreateSwapChain( XE::GraphicsDeviceHandle device, XE::GraphicsSurfaceHandle surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsSwapChain>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateSwapChain( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<WGPUSurface>( surface.GetValue() ), reinterpret_cast<const WGPUSwapChainDescriptor *>( &descriptor ) ) ) );
}

XE::GraphicsTextureHandle XE::GraphicsService::DeviceCreateTexture( XE::GraphicsDeviceHandle device, const XE::GraphicsTextureDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsTexture>( reinterpret_cast<XE::uint64>( wgpuDeviceCreateTexture( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<const WGPUTextureDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::DeviceDestroy( XE::GraphicsDeviceHandle device )
{
	wgpuDeviceDestroy( reinterpret_cast<WGPUDevice>( device.GetValue() ) );
}

void XE::GraphicsService::DeviceEnumerateFeatures( XE::GraphicsDeviceHandle device, XE::Array< XE::GraphicsFeatureName > & features )
{
	features.resize( 10 );
	features.resize( wgpuDeviceEnumerateFeatures( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<WGPUFeatureName *>( features.data() ) ) );
}

bool XE::GraphicsService::DeviceGetLimits( XE::GraphicsDeviceHandle device, XE::GraphicsSupportedLimits & limits )
{
	return wgpuDeviceGetLimits( reinterpret_cast<WGPUDevice>( device.GetValue() ), reinterpret_cast<WGPUSupportedLimits *>( &limits ) );
}

XE::GraphicsQueueHandle XE::GraphicsService::DeviceGetQueue( XE::GraphicsDeviceHandle device )
{
	return XE::HandleCast<XE::GraphicsQueue>( reinterpret_cast<XE::uint64>( wgpuDeviceGetQueue( reinterpret_cast<WGPUDevice>( device.GetValue() ) ) ) );
}

bool XE::GraphicsService::DeviceHasFeature( XE::GraphicsDeviceHandle device, XE::GraphicsFeatureName feature )
{
	return wgpuDeviceHasFeature( reinterpret_cast<WGPUDevice>( device.GetValue() ), static_cast<WGPUFeatureName>( feature ) );
}

bool XE::GraphicsService::DevicePopErrorScope( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	return wgpuDevicePopErrorScope( reinterpret_cast<WGPUDevice>( device.GetValue() ), []( WGPUErrorType type, char const * message, void * userdata )
	{
		( *reinterpret_cast<ErrorCallback *>( userdata ) )( static_cast<XE::GraphicsErrorType>( type ), message );
	}, &callback );
}

void XE::GraphicsService::DevicePushErrorScope( XE::GraphicsDeviceHandle device, XE::GraphicsErrorFilter filter )
{
	wgpuDevicePushErrorScope( reinterpret_cast<WGPUDevice>( device.GetValue() ), static_cast<WGPUErrorFilter>( filter ) );
}

void XE::GraphicsService::DeviceSetDeviceLostCallback( XE::GraphicsDeviceHandle device, DeviceLostCallback callback )
{
	wgpuDeviceSetDeviceLostCallback( reinterpret_cast<WGPUDevice>( device.GetValue() ), []( WGPUDeviceLostReason type, char const * message, void * userdata )
	{
		( *reinterpret_cast<DeviceLostCallback *>( userdata ) )( static_cast<XE::GraphicsDeviceLostReason>( type ), message );
	}, &callback );
}

void XE::GraphicsService::DeviceSetUncapturedErrorCallback( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	wgpuDeviceSetUncapturedErrorCallback( reinterpret_cast<WGPUDevice>( device.GetValue() ), []( WGPUErrorType type, char const * message, void * userdata )
	{
		( *reinterpret_cast<ErrorCallback *>( userdata ) )( static_cast<XE::GraphicsErrorType>( type ), message );
	}, &callback );
}

XE::GraphicsSurfaceHandle XE::GraphicsService::InstanceCreateSurface( XE::GraphicsInstanceHandle instance, const XE::GraphicsSurfaceDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsSurface>( reinterpret_cast<XE::uint64>( wgpuInstanceCreateSurface( reinterpret_cast<WGPUInstance>( instance.GetValue() ), reinterpret_cast<const WGPUSurfaceDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::InstanceProcessEvents( XE::GraphicsInstanceHandle instance )
{
	wgpuInstanceProcessEvents( reinterpret_cast<WGPUInstance>( instance.GetValue() ) );
}

void XE::GraphicsService::InstanceRequestAdapter( XE::GraphicsInstanceHandle instance, const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback )
{
	wgpuInstanceRequestAdapter( reinterpret_cast<WGPUInstance>( instance.GetValue() ), reinterpret_cast<const WGPURequestAdapterOptions *>( &options ),
								[]( WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * userdata )
	{
		( *reinterpret_cast<RequestAdapterCallback *>( userdata ) )( static_cast<XE::GraphicsRequestAdapterStatus>( status ), XE::HandleCast<XE::GraphicsAdapter>( reinterpret_cast<XE::uint64>( adapter ) ), message );
	}, &callback );
}

void XE::GraphicsService::QuerySetDestroy( XE::GraphicsQuerySetHandle querySet )
{
	wgpuQuerySetDestroy( reinterpret_cast<WGPUQuerySet>( querySet.GetValue() ) );
}

void XE::GraphicsService::QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback )
{
	wgpuQueueOnSubmittedWorkDone( reinterpret_cast<WGPUQueue>( queue.GetValue() ), []( WGPUQueueWorkDoneStatus status, void * userdata )
	{
		( *reinterpret_cast<QueueWorkDoneCallback *>( userdata ) )( static_cast<XE::GraphicsQueueWorkDoneStatus>( status ) );
	}, &callback );
}

void XE::GraphicsService::QueueSubmit( XE::GraphicsQueueHandle queue, XE::uint32 commandCount, const XE::GraphicsCommandBufferHandle & commands )
{
	wgpuQueueSubmit( reinterpret_cast<WGPUQueue>( queue.GetValue() ), commandCount, reinterpret_cast<const WGPUCommandBuffer *>( commands.GetValue() ) );
}

void XE::GraphicsService::QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 bufferOffset, XE::MemoryView data )
{
	wgpuQueueWriteBuffer( reinterpret_cast<WGPUQueue>( queue.GetValue() ), reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), bufferOffset, data.data(), data.size() );
}

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & dataLayout, const XE::GraphicsExtent3D & writeSize )
{
	wgpuQueueWriteTexture( reinterpret_cast<WGPUQueue>( queue.GetValue() ), reinterpret_cast<const WGPUImageCopyTexture *>( &destination ), data.data(), data.size(), reinterpret_cast<const WGPUTextureDataLayout *>( &dataLayout ), reinterpret_cast<const WGPUExtent3D *>( &writeSize ) );
}

void XE::GraphicsService::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 vertexCount, XE::uint32 instanceCount, XE::uint32 firstVertex, XE::uint32 firstInstance )
{
	wgpuRenderBundleEncoderDraw( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), vertexCount, instanceCount, firstVertex, firstInstance );
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 indexCount, XE::uint32 instanceCount, XE::uint32 firstIndex, XE::int32 baseVertex, XE::uint32 firstInstance )
{
	wgpuRenderBundleEncoderDrawIndexed( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), indexCount, instanceCount, firstIndex, baseVertex, firstInstance );
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{
	wgpuRenderBundleEncoderDrawIndexedIndirect( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( indirectBuffer.GetValue() ), indirectOffset );
}

void XE::GraphicsService::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{
	wgpuRenderBundleEncoderDrawIndirect( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( indirectBuffer.GetValue() ), indirectOffset );
}

XE::GraphicsRenderBundleHandle XE::GraphicsService::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	return XE::HandleCast<XE::GraphicsRenderBundle>( reinterpret_cast<XE::uint64>( wgpuRenderBundleEncoderFinish( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), reinterpret_cast<const WGPURenderBundleDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::String & markerLabel )
{
	wgpuRenderBundleEncoderInsertDebugMarker( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), markerLabel.c_str() );
}

void XE::GraphicsService::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder )
{
	wgpuRenderBundleEncoderPopDebugGroup( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ) );
}

void XE::GraphicsService::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::String & groupLabel )
{
	wgpuRenderBundleEncoderPushDebugGroup( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), groupLabel.c_str() );
}

void XE::GraphicsService::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets )
{
	wgpuRenderBundleEncoderSetBindGroup( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), groupIndex, reinterpret_cast<WGPUBindGroup>( group.GetValue() ), dynamicOffsetCount, &dynamicOffsets );
}

void XE::GraphicsService::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderBundleEncoderSetIndexBuffer( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), static_cast<WGPUIndexFormat>( format ), offset, size );
}

void XE::GraphicsService::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsRenderPipelineHandle pipeline )
{
	wgpuRenderBundleEncoderSetPipeline( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), reinterpret_cast<WGPURenderPipeline>( pipeline.GetValue() ) );
}

void XE::GraphicsService::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderBundleEncoderSetVertexBuffer( reinterpret_cast<WGPURenderBundleEncoder>( renderBundleEncoder.GetValue() ), slot, reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), offset, size );
}

void XE::GraphicsService::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 queryIndex )
{
	wgpuRenderPassEncoderBeginOcclusionQuery( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), queryIndex );
}

void XE::GraphicsService::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex )
{
	wgpuRenderPassEncoderBeginPipelineStatisticsQuery( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), reinterpret_cast<WGPUQuerySet>( querySet.GetValue() ), queryIndex );
}

void XE::GraphicsService::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 vertexCount, XE::uint32 instanceCount, XE::uint32 firstVertex, XE::uint32 firstInstance )
{
	wgpuRenderPassEncoderDraw( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), vertexCount, instanceCount, firstVertex, firstInstance );
}

void XE::GraphicsService::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 indexCount, XE::uint32 instanceCount, XE::uint32 firstIndex, XE::int32 baseVertex, XE::uint32 firstInstance )
{
	wgpuRenderPassEncoderDrawIndexed( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), indexCount, instanceCount, firstIndex, baseVertex, firstInstance );
}

void XE::GraphicsService::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{
	wgpuRenderPassEncoderDrawIndexedIndirect( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( indirectBuffer.GetValue() ), indirectOffset );
}

void XE::GraphicsService::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset )
{
	wgpuRenderPassEncoderDrawIndirect( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( indirectBuffer.GetValue() ), indirectOffset );
}

void XE::GraphicsService::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{
	wgpuRenderPassEncoderEnd( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ) );
}

void XE::GraphicsService::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{
	wgpuRenderPassEncoderEndOcclusionQuery( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ) );
}

void XE::GraphicsService::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{
	wgpuRenderPassEncoderEndPipelineStatisticsQuery( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ) );
}

void XE::GraphicsService::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 bundlesCount, XE::GraphicsRenderBundleHandle bundles )
{
	wgpuRenderPassEncoderExecuteBundles( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), bundlesCount, reinterpret_cast<const WGPURenderBundle *>( bundles.GetValue() ) );
}

void XE::GraphicsService::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::String & markerLabel )
{
	wgpuRenderPassEncoderInsertDebugMarker( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), markerLabel.c_str() );
}

void XE::GraphicsService::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder )
{
	wgpuRenderPassEncoderPopDebugGroup( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ) );
}

void XE::GraphicsService::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::String & groupLabel )
{
	wgpuRenderPassEncoderPushDebugGroup( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), groupLabel.c_str() );
}

void XE::GraphicsService::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets )
{
	wgpuRenderPassEncoderSetBindGroup( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), groupIndex, reinterpret_cast<WGPUBindGroup>( group.GetValue() ), dynamicOffsetCount, &dynamicOffsets );
}

void XE::GraphicsService::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::Color & color )
{
	WGPUColor c;

	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;

	wgpuRenderPassEncoderSetBlendConstant( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), &c );
}

void XE::GraphicsService::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderPassEncoderSetIndexBuffer( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), static_cast<WGPUIndexFormat>( format ), offset, size );
}

void XE::GraphicsService::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsRenderPipelineHandle pipeline )
{
	wgpuRenderPassEncoderSetPipeline( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), reinterpret_cast<WGPURenderPipeline>( pipeline.GetValue() ) );
}

void XE::GraphicsService::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 x, XE::uint32 y, XE::uint32 width, XE::uint32 height )
{
	wgpuRenderPassEncoderSetScissorRect( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), x, y, width, height );
}

void XE::GraphicsService::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 reference )
{
	wgpuRenderPassEncoderSetStencilReference( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), reference );
}

void XE::GraphicsService::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderPassEncoderSetVertexBuffer( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), slot, reinterpret_cast<WGPUBuffer>( buffer.GetValue() ), offset, size );
}

void XE::GraphicsService::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, float x, float y, float width, float height, float minDepth, float maxDepth )
{
	wgpuRenderPassEncoderSetViewport( reinterpret_cast<WGPURenderPassEncoder>( renderPassEncoder.GetValue() ), x, y, width, height, minDepth, maxDepth );
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle renderPipeline, XE::uint32 groupIndex )
{
	wgpuRenderPipelineGetBindGroupLayout( reinterpret_cast<WGPURenderPipeline>( renderPipeline.GetValue() ), groupIndex );
}

void XE::GraphicsService::RenderPipelineSetLabel( XE::GraphicsRenderPipelineHandle renderPipeline, const XE::String & label )
{
	wgpuRenderPipelineSetLabel( reinterpret_cast<WGPURenderPipeline>( renderPipeline.GetValue() ), label.c_str() );
}

void XE::GraphicsService::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shaderModule, CompilationInfoCallback callback )
{
	wgpuShaderModuleGetCompilationInfo( reinterpret_cast<WGPUShaderModule>( shaderModule.GetValue() ),
										[]( WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const * compilationInfo, void * userdata )
	{
		( *reinterpret_cast<CompilationInfoCallback *>( userdata ) )( static_cast<XE::GraphicsCompilationInfoRequestStatus>( status ), *reinterpret_cast<const XE::GraphicsCompilationInfo *>( compilationInfo ) );
	}, &callback );
}

void XE::GraphicsService::ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shaderModule, const XE::String & label )
{
	wgpuShaderModuleSetLabel( reinterpret_cast<WGPUShaderModule>( shaderModule.GetValue() ), label.c_str() );
}

XE::GraphicsTextureFormat XE::GraphicsService::SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter )
{
	wgpuSurfaceGetPreferredFormat( reinterpret_cast<WGPUSurface>( surface.GetValue() ), reinterpret_cast<WGPUAdapter>( adapter.GetValue() ) );
}

XE::GraphicsTextureViewHandle XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swapChain )
{
	wgpuSwapChainGetCurrentTextureView( reinterpret_cast<WGPUSwapChain>( swapChain.GetValue() ) );
}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainHandle swapChain )
{
	wgpuSwapChainPresent( reinterpret_cast<WGPUSwapChain>( swapChain.GetValue() ) );
}

XE::GraphicsTextureViewHandle XE::GraphicsService::TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	return XE::HandleCast< XE::GraphicsTextureView>( reinterpret_cast<XE::uint64>( wgpuTextureCreateView( reinterpret_cast<WGPUTexture>( texture.GetValue() ), reinterpret_cast<const WGPUTextureViewDescriptor *>( &descriptor ) ) ) );
}

void XE::GraphicsService::TextureDestroy( XE::GraphicsTextureHandle texture )
{
	wgpuTextureDestroy( reinterpret_cast<WGPUTexture>( texture.GetValue() ) );
}

#endif // !defined( NULL_RENDER ) && ( PLATFORM_OS & OS_EMSCRIPTEN )

