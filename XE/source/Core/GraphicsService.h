/*!
 * \file	RenderService.h
 *
 * \author	ZhengYuanQing
 * \date	2021/07/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B
#define RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API GraphicsService : public XE::Service
{
	OBJECT( GraphicsService, XE::Service );

private:
	struct Private;

public:
	using ErrorCallback = XE::Delegate< void( XE::GraphicsErrorType type, XE::String message ) >;
	using BufferMapCallback = XE::Delegate< void( XE::GraphicsBufferMapAsyncStatus status ) >;
	using DeviceLostCallback = XE::Delegate< void( XE::GraphicsDeviceLostReason reason, XE::String message ) >;
	using QueueWorkDoneCallback = XE::Delegate< void( XE::GraphicsQueueWorkDoneStatus status ) >;
	using RequestDeviceCallback = XE::Delegate< void( XE::GraphicsRequestDeviceStatus status, XE::GraphicsDeviceHandle device, XE::String message ) >;
	using RequestAdapterCallback = XE::Delegate< void( XE::GraphicsRequestAdapterStatus status, XE::GraphicsAdapterHandle adapter, XE::String message ) >;
	using CompilationInfoCallback = XE::Delegate< void( XE::GraphicsCompilationInfoRequestStatus status, const XE::GraphicsCompilationInfo & compilationInfo ) >;
	using CreateRenderPipelineAsyncCallback = XE::Delegate< void( XE::GraphicsCreatePipelineAsyncStatus status, XE::GraphicsRenderPipelineHandle pipeline, XE::String message ) >;
	using CreateComputePipelineAsyncCallback = XE::Delegate< void( XE::GraphicsCreatePipelineAsyncStatus status, XE::GraphicsComputePipelineHandle pipeline, XE::String message ) >;

public:
	GraphicsService();

	~GraphicsService() override;

public:
	void Prepare() override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	void AdapterEnumerateFeatures( XE::GraphicsAdapterHandle adapter, XE::Array< XE::GraphicsFeatureName > & features );
	bool AdapterGetLimits( XE::GraphicsAdapterHandle adapter, XE::GraphicsSupportedLimits & limits );
	void AdapterGetProperties( XE::GraphicsAdapterHandle adapter, XE::GraphicsAdapterProperties & properties );
	bool AdapterHasFeature( XE::GraphicsAdapterHandle adapter, XE::GraphicsFeatureName feature );
	void AdapterRequestDevice( XE::GraphicsAdapterHandle adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback );

public:
	void BufferDestroy( XE::GraphicsBufferHandle buffer );
	XE::MemoryView BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );
	void BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::GraphicsMapModeFlags mode, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback );
	void BufferUnmap( XE::GraphicsBufferHandle buffer );

public:
	XE::GraphicsComputePassEncoderHandle CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsComputePassDescriptor & descriptor );
	XE::GraphicsRenderPassEncoderHandle CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsRenderPassDescriptor & descriptor );
	void CommandEncoderClearBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );
	void CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsBufferHandle source, XE::uint64 sourceOffset, XE::GraphicsBufferHandle destination, XE::uint64 destinationOffset, XE::uint64 size );
	void CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::GraphicsExtent3D & copySize );
	void CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::GraphicsExtent3D & copySize );
	void CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const  XE::GraphicsExtent3D & copySize );
	XE::GraphicsCommandBufferHandle CommandEncoderFinish( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::GraphicsCommandBufferDescriptor & descriptor );
	void CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::String & markerLabel );
	void CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle commandEncoder );
	void CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle commandEncoder, const XE::String & groupLabel );
	void CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 firstQuery, XE::uint32 queryCount, XE::GraphicsBufferHandle destination, XE::uint64 destinationOffset );
	void CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle commandEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex );

public:
	void ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex );
	void ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::uint32 workgroupCountX, XE::uint32 workgroupCountY, XE::uint32 workgroupCountZ );
	void ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset );
	void ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle computePassEncoder );
	void ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle computePassEncoder );
	void ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle computePassEncoder, const XE::String & markerLabel );
	void ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder );
	void ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder, const XE::String & groupLabel );
	void ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets );
	void ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle computePassEncoder, XE::GraphicsComputePipelineHandle pipeline );

public:
	XE::GraphicsBindGroupLayoutHandle ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle computePipeline, XE::uint32 groupIndex );
	void ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle computePipeline, const XE::String & label );

public:
	XE::GraphicsBindGroupHandle DeviceCreateBindGroup( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupDescriptor & descriptor );
	XE::GraphicsBindGroupLayoutHandle DeviceCreateBindGroupLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor );
	XE::GraphicsBufferHandle DeviceCreateBuffer( XE::GraphicsDeviceHandle device, const XE::GraphicsBufferDescriptor & descriptor );
	XE::GraphicsCommandEncoderHandle DeviceCreateCommandEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsCommandEncoderDescriptor & descriptor );
	XE::GraphicsComputePipelineHandle DeviceCreateComputePipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor );
	void DeviceCreateComputePipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor, CreateComputePipelineAsyncCallback callback );
	XE::GraphicsPipelineLayoutHandle DeviceCreatePipelineLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsPipelineLayoutDescriptor & descriptor );
	XE::GraphicsQuerySetHandle DeviceCreateQuerySet( XE::GraphicsDeviceHandle device, const XE::GraphicsQuerySetDescriptor & descriptor );
	XE::GraphicsRenderBundleEncoderHandle DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor );
	XE::GraphicsRenderPipelineHandle DeviceCreateRenderPipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor );
	void DeviceCreateRenderPipelineAsync( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor, CreateRenderPipelineAsyncCallback callback );
	XE::GraphicsSamplerHandle DeviceCreateSampler( XE::GraphicsDeviceHandle device, const XE::GraphicsSamplerDescriptor & descriptor );
	XE::GraphicsShaderModuleHandle DeviceCreateShaderModule( XE::GraphicsDeviceHandle device, const XE::GraphicsShaderModuleDescriptor & descriptor );
	XE::GraphicsSwapChainHandle DeviceCreateSwapChain( XE::GraphicsDeviceHandle device, XE::GraphicsSurfaceHandle surface, const XE::GraphicsSwapChainDescriptor & descriptor );
	XE::GraphicsTextureHandle DeviceCreateTexture( XE::GraphicsDeviceHandle device, const XE::GraphicsTextureDescriptor & descriptor );
	void DeviceDestroy( XE::GraphicsDeviceHandle device );
	void DeviceEnumerateFeatures( XE::GraphicsDeviceHandle device, XE::Array< XE::GraphicsFeatureName > & features );
	bool DeviceGetLimits( XE::GraphicsDeviceHandle device, XE::GraphicsSupportedLimits & limits );
	XE::GraphicsQueueHandle DeviceGetQueue( XE::GraphicsDeviceHandle device );
	bool DeviceHasFeature( XE::GraphicsDeviceHandle device, XE::GraphicsFeatureName feature );
	bool DevicePopErrorScope( XE::GraphicsDeviceHandle device, ErrorCallback callback );
	void DevicePushErrorScope( XE::GraphicsDeviceHandle device, XE::GraphicsErrorFilter filter );
	void DeviceSetDeviceLostCallback( XE::GraphicsDeviceHandle device, DeviceLostCallback callback );
	void DeviceSetUncapturedErrorCallback( XE::GraphicsDeviceHandle device, ErrorCallback callback );

public:
	XE::GraphicsSurfaceHandle InstanceCreateSurface( XE::GraphicsInstanceHandle instance, const XE::GraphicsSurfaceDescriptor & descriptor );
	void InstanceProcessEvents( XE::GraphicsInstanceHandle instance );
	void InstanceRequestAdapter( XE::GraphicsInstanceHandle instance, const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback );

public:
	void QuerySetDestroy( XE::GraphicsQuerySetHandle querySet );

public:
	void QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback );
	void QueueSubmit( XE::GraphicsQueueHandle queue, XE::uint32 commandCount, const XE::GraphicsCommandBufferHandle & commands );
	void QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 bufferOffset, XE::MemoryView data );
	void QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & dataLayout, const XE::GraphicsExtent3D & writeSize );

public:
	void RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 vertexCount, XE::uint32 instanceCount, XE::uint32 firstVertex, XE::uint32 firstInstance );
	void RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 indexCount, XE::uint32 instanceCount, XE::uint32 firstIndex, XE::int32 baseVertex, XE::uint32 firstInstance );
	void RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset );
	void RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset );
	XE::GraphicsRenderBundleHandle RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::GraphicsRenderBundleDescriptor & descriptor );
	void RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::String & markerLabel );
	void RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder );
	void RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, const XE::String & groupLabel );
	void RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets );
	void RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::GraphicsRenderPipelineHandle pipeline );
	void RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle renderBundleEncoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );

public:
	void RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 queryIndex );
	void RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsQuerySetHandle querySet, XE::uint32 queryIndex );
	void RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 vertexCount, XE::uint32 instanceCount, XE::uint32 firstVertex, XE::uint32 firstInstance );
	void RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 indexCount, XE::uint32 instanceCount, XE::uint32 firstIndex, XE::int32 baseVertex, XE::uint32 firstInstance );
	void RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset );
	void RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle indirectBuffer, XE::uint64 indirectOffset );
	void RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle renderPassEncoder );
	void RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder );
	void RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle renderPassEncoder );
	void RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 bundlesCount, XE::GraphicsRenderBundleHandle bundles );
	void RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::String & markerLabel );
	void RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder );
	void RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::String & groupLabel );
	void RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 groupIndex, XE::GraphicsBindGroupHandle group, XE::uint32 dynamicOffsetCount, XE::uint32 & dynamicOffsets );
	void RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, const XE::Color & color );
	void RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::GraphicsRenderPipelineHandle pipeline );
	void RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 x, XE::uint32 y, XE::uint32 width, XE::uint32 height );
	void RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 reference );
	void RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle renderPassEncoder, float x, float y, float width, float height, float minDepth, float maxDepth );

public:
	XE::GraphicsBindGroupLayoutHandle RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle renderPipeline, XE::uint32 groupIndex );
	void RenderPipelineSetLabel( XE::GraphicsRenderPipelineHandle renderPipeline, const XE::String & label );

public:
	void ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shaderModule, CompilationInfoCallback callback );
	void ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shaderModule, const XE::String & label );

public:
	XE::GraphicsTextureFormat SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter );

public:
	XE::GraphicsTextureViewHandle SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swapChain );
	void SwapChainPresent( XE::GraphicsSwapChainHandle swapChain );

public:
	XE::GraphicsTextureViewHandle TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor );
	void TextureDestroy( XE::GraphicsTextureHandle texture );

private:
	void OnMainWindowResize();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B
