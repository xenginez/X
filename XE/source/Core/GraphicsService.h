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
	using RequestDeviceCallback = XE::Delegate< void( XE::GraphicsRequestDeviceStatus status, XE::GraphicsDeviceRPtr device ) >;
	using RequestAdapterCallback = XE::Delegate< void( XE::GraphicsRequestAdapterStatus status, XE::GraphicsAdapterRPtr adapter ) >;
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
	XE::GraphicsSurfaceRPtr CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor );

public:
	void RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback );

public:
	void AdapterEnumerateFeatures( XE::GraphicsAdapterRPtr adapter, XE::Array< XE::GraphicsFeatureName > & features );
	bool AdapterGetLimits( XE::GraphicsAdapterRPtr adapter, XE::GraphicsSupportedLimits & limits );
	void AdapterGetProperties( XE::GraphicsAdapterRPtr adapter, XE::GraphicsAdapterProperties & properties );
	bool AdapterHasFeature( XE::GraphicsAdapterRPtr adapter, XE::GraphicsFeatureName feature );
	void AdapterRequestDevice( XE::GraphicsAdapterRPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback );

public:
	XE::GraphicsBindGroupRPtr DeviceCreateBindGroup( XE::GraphicsDeviceRPtr device, const XE::GraphicsBindGroupDescriptor & descriptor );
	XE::GraphicsBindGroupLayoutRPtr DeviceCreateBindGroupLayout( XE::GraphicsDeviceRPtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor );
	XE::GraphicsBufferRPtr DeviceCreateBuffer( XE::GraphicsDeviceRPtr device, const XE::GraphicsBufferDescriptor & descriptor );
	XE::GraphicsCommandEncoderRPtr DeviceCreateCommandEncoder( XE::GraphicsDeviceRPtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor );
	XE::GraphicsComputePipelineRPtr DeviceCreateComputePipeline( XE::GraphicsDeviceRPtr device, const XE::GraphicsComputePipelineDescriptor & descriptor );
	XE::GraphicsPipelineLayoutRPtr DeviceCreatePipelineLayout( XE::GraphicsDeviceRPtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor );
	XE::GraphicsQuerySetRPtr DeviceCreateQuerySet( XE::GraphicsDeviceRPtr device, const XE::GraphicsQuerySetDescriptor & descriptor );
	XE::GraphicsRenderBundleEncoderRPtr DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceRPtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor );
	XE::GraphicsRenderPipelineRPtr DeviceCreateRenderPipeline( XE::GraphicsDeviceRPtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor );
	XE::GraphicsSamplerRPtr DeviceCreateSampler( XE::GraphicsDeviceRPtr device, const XE::GraphicsSamplerDescriptor & descriptor );
	XE::GraphicsShaderModuleRPtr DeviceCreateShaderModule( XE::GraphicsDeviceRPtr device, const XE::GraphicsShaderModuleDescriptor & descriptor );
	XE::GraphicsSwapChainRPtr DeviceCreateSwapChain( XE::GraphicsDeviceRPtr device, XE::GraphicsSurfaceRPtr surface, const XE::GraphicsSwapChainDescriptor & descriptor );
	XE::GraphicsTextureRPtr DeviceCreateTexture( XE::GraphicsDeviceRPtr device, const XE::GraphicsTextureDescriptor & descriptor );
	XE::GraphicsTextureViewRPtr TextureCreateView( XE::GraphicsTextureRPtr texture, const XE::GraphicsTextureViewDescriptor & descriptor );

public:
	void DeviceEnumerateFeatures( XE::GraphicsDeviceRPtr device, XE::Array< XE::GraphicsFeatureName > & features );
	bool DeviceGetLimits( XE::GraphicsDeviceRPtr device, XE::GraphicsSupportedLimits & limits );
	XE::GraphicsQueueRPtr DeviceGetQueue( XE::GraphicsDeviceRPtr device );
	bool DeviceHasFeature( XE::GraphicsDeviceRPtr device, XE::GraphicsFeatureName feature );
	bool DevicePopErrorScope( XE::GraphicsDeviceRPtr device, ErrorCallback callback );
	void DevicePushErrorScope( XE::GraphicsDeviceRPtr device, XE::GraphicsErrorFilter filter );
	void DeviceSetDeviceLostCallback( XE::GraphicsDeviceRPtr device, DeviceLostCallback callback );
	void DeviceSetUncapturedErrorCallback( XE::GraphicsDeviceRPtr device, ErrorCallback callback );

public:
	void QueueOnSubmittedWorkDone( XE::GraphicsQueueRPtr queue, QueueWorkDoneCallback callback );
	void QueueSubmit( XE::GraphicsQueueRPtr queue, const XE::Array< XE::GraphicsCommandBufferRPtr > & commands );
	void QueueWriteBuffer( XE::GraphicsQueueRPtr queue, XE::GraphicsBufferRPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data );
	void QueueWriteTexture( XE::GraphicsQueueRPtr queue, const XE::GraphicsImageCopyTexture & destination, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size );

public:
	void BufferMapAsync( XE::GraphicsBufferRPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, BufferMapCallback callback, void * userdata );
	XE::Span< const XE::uint8 > BufferGetConstMappedRange( XE::GraphicsBufferRPtr buffer, XE::uint64 offset, XE::uint64 size );
	XE::Span< XE::uint8 > BufferGetMappedRange( XE::GraphicsBufferRPtr buffer, XE::uint64 offset, XE::uint64 size );
	void BufferUnmap( XE::GraphicsBufferRPtr buffer );

public:
	XE::GraphicsComputePassEncoderRPtr CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor );
	XE::GraphicsRenderPassEncoderRPtr CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor );
	void CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderRPtr command_encoder, XE::GraphicsBufferRPtr source, XE::uint64 source_offset, XE::GraphicsBufferRPtr destination, XE::uint64 destination_offset, XE::uint64 size );
	void CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::GraphicsImageCopyBuffer & source, const XE::GraphicsImageCopyTexture & destination, const XE::Vec3f & copy_size );
	void CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyBuffer & destination, const XE::Vec3f & copy_size );
	void CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::GraphicsImageCopyTexture & source, const XE::GraphicsImageCopyTexture & destination, const  XE::Vec3f & copy_size );
	XE::GraphicsCommandBufferRPtr CommandEncoderFinish( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor );
	void CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::String & marker_label );
	void CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderRPtr command_encoder );
	void CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderRPtr command_encoder, const XE::String & group_label );
	void CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderRPtr command_encoder, XE::GraphicsQuerySetRPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferRPtr destination, XE::uint64 destination_offset );
	void CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderRPtr command_encoder, XE::GraphicsQuerySetRPtr query_set, XE::uint32 query_index );

public:
	void ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, XE::GraphicsQuerySetRPtr query_set, XE::uint32 query_index );
	void ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z );
	void ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, XE::GraphicsBufferRPtr indirect_buffer, XE::uint64 indirect_offset );
	void ComputePassEncoderEnd( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder );
	void ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder );
	void ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, const XE::String & marker_label );
	void ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder );
	void ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, const XE::String & group_label );
	void ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupRPtr group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets );
	void ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderRPtr compute_pass_encoder, XE::GraphicsComputePipelineRPtr pipeline );

public:
	void RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::uint32 query_index );
	void RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::GraphicsQuerySetRPtr query_set, XE::uint32 query_index );
	void RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance );
	void RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance );
	void RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::GraphicsBufferRPtr indirect_buffer, XE::uint64 indirect_offset );
	void RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::GraphicsBufferRPtr indirect_buffer, XE::uint64 indirect_offset );
	void RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder );
	void RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder );
	void RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder );
	void RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundleRPtr > & bundles );
	void RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, const XE::String & marker_label );
	void RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder );
	void RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, const XE::String & group_label );
	void RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupRPtr group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets );
	void RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, const XE::Color & color );
	void RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::GraphicsBufferRPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::GraphicsRenderPipelineRPtr pipeline );
	void RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, const XE::Recti & rect );
	void RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::uint32 reference );
	void RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferRPtr buffer, XE::uint64 offset, XE::uint64 size );
	void RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderRPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth );

public:
	void RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance );
	void RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance );
	void RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::GraphicsBufferRPtr indirect_buffer, XE::uint64 indirect_offset );
	void RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::GraphicsBufferRPtr indirect_buffer, XE::uint64 indirect_offset );
	XE::GraphicsRenderBundleRPtr RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor );
	void RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, const XE::String & marker_label );
	void RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder );
	void RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, const XE::String & group_label );
	void RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupRPtr group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets );
	void RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::GraphicsBufferRPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size );
	void RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::GraphicsRenderPipelineRPtr pipeline );
	void RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderRPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferRPtr buffer, XE::uint64 offset, XE::uint64 size );

public:
	XE::GraphicsBindGroupLayoutRPtr ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineRPtr compute_pipeline, XE::uint32 group_index );
	XE::GraphicsBindGroupLayoutRPtr RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineRPtr render_pipeline, XE::uint32 group_index );

public:
	void ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleRPtr shader_module, CompilationInfoCallback callback );
	void ShaderModuleSetLabel( XE::GraphicsShaderModuleRPtr shader_module, const XE::String & label );

public:
	XE::GraphicsTextureFormat SurfaceGetPreferredFormat( XE::GraphicsSurfaceRPtr surface, XE::GraphicsAdapterRPtr adapter );

public:
	XE::GraphicsTextureViewRPtr SwapChainGetCurrentTextureView( XE::GraphicsSwapChainRPtr swap_chain );
	void SwapChainPresent( XE::GraphicsSwapChainRPtr swap_chain );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B
