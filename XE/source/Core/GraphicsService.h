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
	using ErrorCallback = XE::Delegate< void( XE::GraphicsErrorType type ) >;
	using BufferMapCallback = XE::Delegate<void( XE::GraphicsBufferMapAsyncStatus status )>;
	using DeviceLostCallback = XE::Delegate< void( XE::GraphicsDeviceLostReason reason ) >;
	using QueueWorkDoneCallback = XE::Delegate< void( XE::GraphicsQueueWorkDoneStatus status ) >;
	using RequestDeviceCallback = XE::Delegate< void( XE::GraphicsRequestDeviceStatus status, XE::GraphicsDevicePtr device ) >;
	using RequestAdapterCallback = XE::Delegate< void( XE::GraphicsRequestAdapterStatus status, XE::GraphicsAdapterPtr adapter ) >;
	using CompilationInfoCallback = XE::Delegate< void( XE::GraphicsCompilationInfoRequestStatus status, const XE::GraphicsCompilationInfo & compilation_info ) >;

public:
	GraphicsService();

	~GraphicsService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::GraphicsSurfacePtr CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor );

public:
	void RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback );

public:
	void AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features );
	bool AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits );
	void AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties );
	bool AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature );
	void AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback );

public:
	XE::GraphicsBindGroupPtr DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor );
	XE::GraphicsBindGroupLayoutPtr DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor );
	XE::GraphicsBufferPtr DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor );
	XE::GraphicsCommandEncoderPtr DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor );
	XE::GraphicsComputePipelinePtr DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor );
	XE::GraphicsPipelineLayoutPtr DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor );
	XE::GraphicsQuerySetPtr DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor );
	XE::GraphicsRenderBundleEncoderPtr DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor );
	XE::GraphicsRenderPipelinePtr DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor );
	XE::GraphicsSamplerPtr DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor );
	XE::GraphicsShaderModulePtr DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor );
	XE::GraphicsSwapChainPtr DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor );
	XE::GraphicsTexturePtr DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor );
	XE::GraphicsTextureViewPtr TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor );

public:
	void DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features );
	bool DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits );
	XE::GraphicsQueuePtr DeviceGetQueue( XE::GraphicsDevicePtr device );
	bool DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature );
	bool DevicePopErrorScope( XE::GraphicsDevicePtr device, ErrorCallback callback );
	void DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter );
	void DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, DeviceLostCallback callback );
	void DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, ErrorCallback callback );

public:
	void QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, QueueWorkDoneCallback callback );
	void QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands );
	void QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data );
	void QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size );

public:
	void BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, BufferMapCallback callback, void * userdata );
	XE::Span< const XE::uint8 > BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size );
	XE::Span< XE::uint8 > BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size );
	void BufferUnmap( XE::GraphicsBufferPtr buffer );

public:
	XE::GraphicsComputePassEncoderPtr CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor );
	XE::GraphicsRenderPassEncoderPtr CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor );
	void CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr source, XE::uint64 source_offset, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset, XE::uint64 size );
	void CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size );
	void CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size );
	void CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const  XE::Vec3f & copy_size );
	XE::GraphicsCommandBufferPtr CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor );
	void CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label );
	void CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder );
	void CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label );
	void CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr destination, XE::uint64 destination_offset );
	void CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index );

public:
	void ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index );
	void ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z );
	void ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset );
	void ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder );
	void ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder );
	void ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label );
	void ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder );
	void ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label );
	void ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets );
	void ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline );

public:
	void RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index );
	void RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index );
	void RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance );
	void RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance );
	void RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset );
	void RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset );
	void RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder );
	void RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder );
	void RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder );
	void RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles );
	void RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label );
	void RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder );
	void RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label );
	void RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets );
	void RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color );
	void RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline );
	void RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect );
	void RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference );
	void RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth );

public:
	void RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance );
	void RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance );
	void RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset );
	void RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset );
	XE::GraphicsRenderBundlePtr RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor );
	void RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label );
	void RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder );
	void RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label );
	void RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets );
	void RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline );
	void RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size );

public:
	XE::GraphicsBindGroupLayoutPtr ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index );
	XE::GraphicsBindGroupLayoutPtr RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index );

public:
	void ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, CompilationInfoCallback callback );
	void ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label );

public:
	XE::GraphicsTextureFormat SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter );

public:
	XE::GraphicsTextureViewPtr SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain );
	void SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B
