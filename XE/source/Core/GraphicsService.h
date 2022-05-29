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
	using ErrorCallback = XE::Delegate< void( XE::GraphicsErrorType type, const XE::String & message ) >;
	using BufferMapCallback = XE::Delegate< void( XE::GraphicsBufferMapAsyncStatus status, XE::Span< XE::uint8 > ) >;
	using DeviceLostCallback = XE::Delegate< void( XE::GraphicsDeviceLostReason reason, const XE::String & message ) >;
	using QueueWorkDoneCallback = XE::Delegate< void( XE::GraphicsQueueWorkDoneStatus status ) >;
	using RequestDeviceCallback = XE::Delegate< void( XE::GraphicsRequestDeviceStatus status, XE::GraphicsDeviceHandle device, const XE::String & message ) >;
	using RequestAdapterCallback = XE::Delegate< void( XE::GraphicsRequestAdapterStatus status, XE::GraphicsAdapterHandle adapter, const XE::String & message ) >;
	using CompilationInfoCallback = XE::Delegate< void( XE::GraphicsCompilationInfoRequestStatus status, const XE::GraphicsCompilationInfo & compilation_info ) >;
	using CreateRenderPipelineAsyncCallback = XE::Delegate< void( XE::GraphicsCreatePipelineAsyncStatus status, XE::GraphicsRenderPipelineHandle pipeline, const XE::String & message ) >;
	using CreateComputePipelineAsyncCallback = XE::Delegate< void( XE::GraphicsCreatePipelineAsyncStatus status, XE::GraphicsComputePipelineHandle pipeline, const XE::String & message ) >;

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
	XE::Span< XE::uint8 > BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );
	void BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size, BufferMapCallback callback );
	void BufferUnmap( XE::GraphicsBufferHandle buffer );

public:
	XE::GraphicsComputePassEncoderHandle CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsComputePassDescriptor & descriptor );
	XE::GraphicsRenderPassEncoderHandle CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor );
	void CommandEncoderClearBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );
	void CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle source, XE::uint64 source_offset, XE::GraphicsBufferHandle destination, XE::uint64 destination_offset, XE::uint64 size );
	void CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size );
	void CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size );
	void CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const  XE::Vec3f & copy_size );
	XE::GraphicsCommandBufferHandle CommandEncoderFinish( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor );
	void CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & marker_label );
	void CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder );
	void CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & group_label );
	void CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferHandle destination, XE::uint64 destination_offset );
	void CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index );

public:
	void ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index );
	void ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z );
	void ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset );
	void ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle compute_pass_encoder );
	void ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder );
	void ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & marker_label );
	void ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder );
	void ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & group_label );
	void ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets );
	void ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsComputePipelineHandle pipeline );

public:
	XE::GraphicsBindGroupLayoutHandle ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle compute_pipeline, XE::uint32 group_index );
	void ComputePipelineSetLabel( XE::GraphicsComputePipelineHandle compute_pipeline, const XE::String & label );

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
	void QuerySetDestroy( XE::GraphicsQuerySetHandle query_set );

public:
	void QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback );
	void QueueSubmit( XE::GraphicsQueueHandle queue, const XE::Array< XE::GraphicsCommandBufferHandle > & commands );
	void QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 buffer_offset, XE::MemoryView data );
	void QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size );

public:
	void RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance );
	void RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance );
	void RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset );
	void RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset );
	XE::GraphicsRenderBundleHandle RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor );
	void RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & marker_label );
	void RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder );
	void RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & group_label );
	void RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets );
	void RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsRenderPipelineHandle pipeline );
	void RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );

public:
	void RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 query_index );
	void RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index );
	void RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance );
	void RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance );
	void RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset );
	void RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset );
	void RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle render_pass_encoder );
	void RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder );
	void RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder );
	void RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Array< XE::GraphicsRenderBundleHandle > & bundles );
	void RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & marker_label );
	void RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder );
	void RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & group_label );
	void RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets );
	void RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Color & color );
	void RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsRenderPipelineHandle pipeline );
	void RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Recti & rect );
	void RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 reference );
	void RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth );

public:
	XE::GraphicsBindGroupLayoutHandle RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle render_pipeline, XE::uint32 group_index );
	void RenderPipelineSetLabel( XE::GraphicsRenderPipelineHandle render_pipeline, const XE::String & label );

public:
	void ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shader_module, CompilationInfoCallback callback );
	void ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shader_module, const XE::String & label );

public:
	XE::GraphicsTextureFormat SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter );

public:
	XE::GraphicsTextureViewHandle SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swap_chain );
	void SwapChainPresent( XE::GraphicsSwapChainHandle swap_chain );

public:
	XE::GraphicsTextureViewHandle TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor );
	void TextureDestroy( XE::GraphicsTextureHandle texture );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B
