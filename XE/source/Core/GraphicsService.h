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
	GraphicsService();

	~GraphicsService();

public:
	void Prepare() override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::RenderApiType GetApiType() const;

public:
	XE::uint32 GetContextCount() const;

	XE::RenderContextHandle GetContextHandle( XE::uint32 val ) const;

	const XE::RenderContextDesc & GetContextDesc( XE::uint32 val ) const;

public:
	XE::RenderContextHandle GetMainContextHandle() const;

	XE::RenderTargetHandle GetMainWindowRenderTarget() const;

	XE::RenderTargetHandle GetMainWindowBackRenderTarget() const;

public:
	XE::RenderQueryHandle Create( XE::RenderContextHandle context, const XE::RenderQueryDesc & desc );

	XE::RenderBufferHandle Create( XE::RenderContextHandle context, const XE::RenderBufferDesc & desc );

	XE::RenderTargetHandle Create( XE::RenderContextHandle context, const XE::RenderTargetDesc & desc );

	XE::RenderShaderHandle Create( XE::RenderContextHandle context, const XE::RenderShaderDesc & desc );

	XE::RenderTextureHandle Create( XE::RenderContextHandle context, const XE::RenderTextureDesc & desc );

	XE::RenderSamplerHandle Create( XE::RenderContextHandle context, const XE::RenderSamplerDesc & desc );

	XE::RenderDescriptorSetHandle Create( XE::RenderContextHandle context, const XE::RenderDescriptorSetDesc & desc );

	XE::RenderCommandBundleHandle Create( XE::RenderContextHandle context, const XE::RenderCommandBundleDesc & desc );

	XE::RenderShaderProgramHandle Create( XE::RenderContextHandle context, const XE::RenderShaderProgramDesc & desc );

	XE::RenderPipelineStateHandle Create( XE::RenderContextHandle context, const XE::RenderPipelineStateDesc & desc );

	XE::RenderVirtualBufferHandle Create( XE::RenderContextHandle context, const XE::RenderVirtualBufferDesc & desc, XE::MemoryView data );

public:
	const XE::RenderQueryDesc & GetDesc( XE::RenderContextHandle context, XE::RenderQueryHandle handle );

	const XE::RenderBufferDesc & GetDesc( XE::RenderContextHandle context, XE::RenderBufferHandle handle );

	const XE::RenderTargetDesc & GetDesc( XE::RenderContextHandle context, XE::RenderTargetHandle handle );

	const XE::RenderShaderDesc & GetDesc( XE::RenderContextHandle context, XE::RenderShaderHandle handle );

	const XE::RenderTextureDesc & GetDesc( XE::RenderContextHandle context, XE::RenderTextureHandle handle );

	const XE::RenderSamplerDesc & GetDesc( XE::RenderContextHandle context, XE::RenderSamplerHandle handle );

	const XE::RenderDescriptorSetDesc & GetDesc( XE::RenderContextHandle context, XE::RenderDescriptorSetHandle handle );

	const XE::RenderCommandBundleDesc & GetDesc( XE::RenderContextHandle context, XE::RenderCommandBundleHandle handle );

	const XE::RenderShaderProgramDesc & GetDesc( XE::RenderContextHandle context, XE::RenderShaderProgramHandle handle );

	const XE::RenderPipelineStateDesc & GetDesc( XE::RenderContextHandle context, XE::RenderPipelineStateHandle handle );

	const XE::RenderVirtualBufferDesc & GetDesc( XE::RenderContextHandle context, XE::RenderVirtualBufferHandle handle );

public:
	void Destroy( XE::RenderContextHandle context, XE::RenderQueryHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderBufferHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderTargetHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderShaderHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderTextureHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderSamplerHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderDescriptorSetHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderCommandBundleHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderShaderProgramHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderPipelineStateHandle handle );

	void Destroy( XE::RenderContextHandle context, XE::RenderVirtualBufferHandle handle );

public:
	XE::RenderCommandBufferHandle Begin( XE::RenderContextHandle context );

public:
	void Read( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
			   XE::RenderBufferHandle handle, XE::uint64 offset,
			   const XE::Delegate< void( void *, XE::uint64 ) > & callback );

	void Read( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
			   XE::RenderTextureHandle handle,
			   XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth,
			   XE::uint32 mip, XE::uint32 mip_size,
			   const XE::Delegate< void( void * data, XE::uint64 y, XE::uint64 z, XE::uint64 size ) > & callback );
	
	void Write( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
				XE::RenderBufferHandle handle, XE::uint64 offset,
				XE::MemoryView src_data );

	void Write( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
				XE::RenderTextureHandle handle,
				XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth,
				XE::uint32 mip, XE::uint32 mip_size,
				XE::MemoryView src_data );

	void Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
			   XE::RenderBufferHandle dst_buf, XE::uint64 dst_offset,
			   XE::RenderBufferHandle src_buf, XE::uint64 src_offset, XE::uint64 size );

	void Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
			   XE::RenderBufferHandle dst_buf, XE::uint64 dst_offset,
			   XE::RenderTextureHandle src_tex,
			   XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size );

	void Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
			   XE::RenderTextureHandle dst_tex, 
			   XE::uint32 dst_offset_x, XE::uint32 dst_offset_y, XE::uint32 dst_offset_z, XE::uint32 dst_mip,
			   XE::RenderTextureHandle src_tex,
			   XE::uint32 src_offset_x, XE::uint32 src_offset_y, XE::uint32 src_offset_z, XE::uint32 src_mip,
			   XE::uint32 width, XE::uint32 height, XE::uint32 depth );

	void Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
			   XE::RenderTextureHandle dst_tex,
			   XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size,
			   XE::RenderBufferHandle src_buf, XE::uint64 src_offset );

public:
	XE::RenderResourceStatesType TransitionBarrier( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle buffer, XE::RenderResourceStatesType after );

	XE::RenderResourceStatesType TransitionBarrier( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle texture, XE::RenderResourceStatesType after );

public:
	XE::RenderPassHandle BeginPass( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, const XE::RenderPassDesc & desc );

	void SetViewport( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Rectf & view );

	void SetViewport( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::Rectf > & views );

	void SetScissor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const  XE::Rectf & rect );

	void SetScissor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::Rectf > & rects );

	void SetIndexBuffer( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderVirtualBufferHandle buffer );

	void SetVertexBuffer( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderVirtualBufferHandle buffer );

	void SetDescriptorSet( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderDescriptorSetHandle heap, XE::uint32 first, XE::RenderPipelineStateType bind );

	void SetPipelineState( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderPipelineStateHandle pso );

	void SetUniform( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 location, const XE::Variant & value );

	void SetUniform( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 location, const XE::VariantArray & value );

	void SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage );

	void SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderTextureHandle texture, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage );

	void SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderSamplerHandle sampler, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage );

	void ResetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderResourceType type, const XE::Pair< XE::uint32, XE::uint32 > & slot_ranges, XE::RenderBindFlags bind, XE::RenderStageFlags stage );

	void SetBlendFactor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::FColor & color );

	void SetStencilReference( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 reference, XE::RenderStencilFaceType stencilFace = XE::RenderStencilFaceType::FRONT_BACK );

	void BeginQuery( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 value );

	void EndQuery( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 value );

	void BeginCondition( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 offset, XE::RenderConditionType cond );

	void EndCondition( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query );

	void BeginStreamOutput( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::RenderBufferHandle > & buffers );

	void EndStreamOutput( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass );

	void Draw( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices );

	void DrawIndexed( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & indices );

	void DrawIndexed( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & indices, XE::uint32 vertex_offset );

	void DrawInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices, XE::uint32 num_instances );

	void DrawInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices, const XE::Pair< XE::uint32, XE::uint32 > & instances );

	void DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index );

	void DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index, XE::uint32 vertex_offset );

	void DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index, XE::uint32 vertex_offset, XE::uint32 first_instance );

	void DrawIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset );

	void DrawIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset, XE::uint32 num_commands, XE::uint32 stride );

	void DrawIndexedIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset );

	void DrawIndexedIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset, XE::uint32 num_commands, XE::uint32 stride );

	void ExecuteBundle( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderCommandBundleHandle bundle );

public:
	void EndPass( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass );

public:
	void Dispatch( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::uint32 group_x, XE::uint32 group_y, XE::uint32 group_z );

	void DispatchIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle buffer, XE::uint64 offset );

public:
	void QueryResolve( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderQueryHandle query, XE::uint32 offset, XE::uint32 size );

	void QueryResult( XE::RenderContextHandle context, XE::RenderQueryHandle query, XE::uint32 offset, XE::uint32 size, std::ostream & stream );

public:
	void End( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf );

public:
	void BeginBundle( XE::RenderContextHandle context, XE::RenderCommandBundleHandle bundle );

	void EndBundle( XE::RenderContextHandle context, XE::RenderCommandBundleHandle bundle );

public:
	void Execute( XE::RenderContextHandle context );

	void Wait( XE::RenderContextHandle context );

private:
	void OnMainWindowResize();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // RENDERSERVICE_H__E61B2CB0_82AF_443B_B39D_810F6A43CF6B
