#include "GraphicsService.h"

#if !defined( NULL_RENDER ) && ( PLATFORM_OS & OS_WASM )

#include <webgpu/webgpu.h>

IMPLEMENT_META( XE::GraphicsService );

struct XE::GraphicsService::Private
{
	XE::RenderContextDesc _ContextDesc;
	XE::RenderQueryDesc _RenderQueryDesc;
	XE::RenderBufferDesc _RenderBufferDesc;
	XE::RenderTargetDesc _RenderTargetDesc;
	XE::RenderShaderDesc _RenderShaderDesc;
	XE::RenderTextureDesc _RenderTextureDesc;
	XE::RenderSamplerDesc _RenderSamplerDesc;
	XE::RenderDescriptorSetDesc _RenderDescriptorSetDesc;
	XE::RenderCommandBundleDesc _RenderCommandBundleDesc;
	XE::RenderShaderProgramDesc _RenderShaderProgramDesc;
	XE::RenderPipelineStateDesc _RenderPipelineStateDesc;
	XE::RenderVirtualBufferDesc _RenderVirtualBufferDesc;

	XE::ConcurrentHandleAllocator< XE::RenderPassHandle, XE::RENDER_MAX_PASS > _RenderPassHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderQueryHandle, XE::RENDER_MAX_QUERY > _RenderQueryHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderBufferHandle, XE::RENDER_MAX_BUFFER > _RenderBufferHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderTargetHandle, XE::RENDER_MAX_TARGET > _RenderTargetHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderShaderHandle, XE::RENDER_MAX_SHADER > _RenderShaderHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderTextureHandle, XE::RENDER_MAX_TEXTURE > _RenderTextureHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderSamplerHandle, XE::RENDER_MAX_SAMPLER > _RenderSamplerHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderDescriptorSetHandle, XE::RENDER_MAX_DESCRIPTOR_SET > _RenderDescriptorSetHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderCommandBufferHandle, XE::RENDER_MAX_COMMAND_BUFFER > _RenderCommandBufferHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderCommandBundleHandle, XE::RENDER_MAX_COMMAND_BUNDLE > _RenderCommandBundleHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderPipelineStateHandle, XE::RENDER_MAX_PIPELINE_STATE > _RenderPipelineStateHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderShaderProgramHandle, XE::RENDER_MAX_SHADER_PROGRAM > _RenderShaderProgramHandleAllocator;
	XE::ConcurrentHandleAllocator< XE::RenderVirtualBufferHandle, XE::RENDER_MAX_VIRTUAL_BUFFER > _RenderVirtualBufferHandleAllocator;
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

XE::RenderApiType XE::GraphicsService::GetApiType() const
{
	return RenderApiType::NIL;
}

XE::uint32 XE::GraphicsService::GetContextCount() const
{
	return 1;
}

XE::RenderContextHandle XE::GraphicsService::GetContextHandle( XE::uint32 val ) const
{
	return XE::HandleCast< XE::RenderContext >( 0 );
}

const XE::RenderContextDesc & XE::GraphicsService::GetContextDesc( XE::uint32 val ) const
{
	return _p->_ContextDesc;
}

XE::RenderContextHandle XE::GraphicsService::GetMainContextHandle() const
{
	return XE::HandleCast< XE::RenderContext >( 0 );
}

XE::RenderTargetHandle XE::GraphicsService::GetMainWindowRenderTarget() const
{
	return XE::HandleCast< XE::RenderTarget >( 0 );
}

XE::RenderTargetHandle XE::GraphicsService::GetMainWindowBackRenderTarget() const
{
	return XE::HandleCast< XE::RenderTarget >( 0 );
}

XE::RenderQueryHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderQueryDesc & desc )
{
	return _p->_RenderQueryHandleAllocator.Alloc();
}

XE::RenderBufferHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderBufferDesc & desc )
{
	return _p->_RenderBufferHandleAllocator.Alloc();
}

XE::RenderTargetHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderTargetDesc & desc )
{
	return _p->_RenderTargetHandleAllocator.Alloc();
}

XE::RenderShaderHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderShaderDesc & desc )
{
	return _p->_RenderShaderHandleAllocator.Alloc();
}

XE::RenderTextureHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderTextureDesc & desc )
{
	return _p->_RenderTextureHandleAllocator.Alloc();
}

XE::RenderSamplerHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderSamplerDesc & desc )
{
	return _p->_RenderSamplerHandleAllocator.Alloc();
}

XE::RenderDescriptorSetHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderDescriptorSetDesc & desc )
{
	return _p->_RenderDescriptorSetHandleAllocator.Alloc();
}

XE::RenderCommandBundleHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderCommandBundleDesc & desc )
{
	return _p->_RenderCommandBundleHandleAllocator.Alloc();
}

XE::RenderShaderProgramHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderShaderProgramDesc & desc )
{
	return _p->_RenderShaderProgramHandleAllocator.Alloc();
}

XE::RenderPipelineStateHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderPipelineStateDesc & desc )
{
	return _p->_RenderPipelineStateHandleAllocator.Alloc();
}

XE::RenderVirtualBufferHandle XE::GraphicsService::Create( XE::RenderContextHandle context, const XE::RenderVirtualBufferDesc & desc, XE::MemoryView data )
{
	return _p->_RenderVirtualBufferHandleAllocator.Alloc();
}

const XE::RenderQueryDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderQueryHandle handle )
{
	return _p->_RenderQueryDesc;
}

const XE::RenderBufferDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderBufferHandle handle )
{
	return _p->_RenderBufferDesc;
}

const XE::RenderTargetDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderTargetHandle handle )
{
	return _p->_RenderTargetDesc;
}

const XE::RenderShaderDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderShaderHandle handle )
{
	return _p->_RenderShaderDesc;
}

const XE::RenderTextureDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderTextureHandle handle )
{
	return _p->_RenderTextureDesc;
}

const XE::RenderSamplerDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderSamplerHandle handle )
{
	return _p->_RenderSamplerDesc;
}

const XE::RenderDescriptorSetDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderDescriptorSetHandle handle )
{
	return _p->_RenderDescriptorSetDesc;
}

const XE::RenderCommandBundleDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderCommandBundleHandle handle )
{
	return _p->_RenderCommandBundleDesc;
}

const XE::RenderShaderProgramDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderShaderProgramHandle handle )
{
	return _p->_RenderShaderProgramDesc;
}

const XE::RenderPipelineStateDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderPipelineStateHandle handle )
{
	return _p->_RenderPipelineStateDesc;
}

const XE::RenderVirtualBufferDesc & XE::GraphicsService::GetDesc( XE::RenderContextHandle context, XE::RenderVirtualBufferHandle handle )
{
	return _p->_RenderVirtualBufferDesc;
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderQueryHandle handle )
{
	_p->_RenderQueryHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderBufferHandle handle )
{
	_p->_RenderBufferHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderTargetHandle handle )
{
	_p->_RenderTargetHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderShaderHandle handle )
{
	_p->_RenderShaderHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderTextureHandle handle )
{
	_p->_RenderTextureHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderSamplerHandle handle )
{
	_p->_RenderSamplerHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderDescriptorSetHandle handle )
{
	_p->_RenderDescriptorSetHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderCommandBundleHandle handle )
{
	_p->_RenderCommandBundleHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderShaderProgramHandle handle )
{
	_p->_RenderShaderProgramHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderPipelineStateHandle handle )
{
	_p->_RenderPipelineStateHandleAllocator.Free( handle );
}

void XE::GraphicsService::Destroy( XE::RenderContextHandle context, XE::RenderVirtualBufferHandle handle )
{
	_p->_RenderVirtualBufferHandleAllocator.Free( handle );
}

XE::RenderCommandBufferHandle XE::GraphicsService::Begin( XE::RenderContextHandle context )
{
	return _p->_RenderCommandBufferHandleAllocator.Alloc();
}

void XE::GraphicsService::Read( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle handle, XE::uint64 offset, const XE::Delegate< void( void *, XE::uint64 ) > & callback )
{

}

void XE::GraphicsService::Read( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle handle, XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size, const XE::Delegate< void( void * data, XE::uint64 y, XE::uint64 z, XE::uint64 size ) > & callback )
{

}

void XE::GraphicsService::Write( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle handle, XE::uint64 offset, XE::MemoryView src_data )
{

}

void XE::GraphicsService::Write( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle handle, XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size, XE::MemoryView src_data )
{

}

void XE::GraphicsService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle dst_buf, XE::uint64 dst_offset, XE::RenderBufferHandle src_buf, XE::uint64 src_offset, XE::uint64 size )
{

}

void XE::GraphicsService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle dst_buf, XE::uint64 dst_offset, XE::RenderTextureHandle src_tex, XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size )
{

}

void XE::GraphicsService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle dst_tex, XE::uint32 dst_offset_x, XE::uint32 dst_offset_y, XE::uint32 dst_offset_z, XE::uint32 dst_mip, XE::RenderTextureHandle src_tex, XE::uint32 src_offset_x, XE::uint32 src_offset_y, XE::uint32 src_offset_z, XE::uint32 src_mip, XE::uint32 width, XE::uint32 height, XE::uint32 depth )
{

}

void XE::GraphicsService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle dst_tex, XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size, XE::RenderBufferHandle src_buf, XE::uint64 src_offset )
{

}

XE::RenderResourceStatesType XE::GraphicsService::TransitionBarrier( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle buffer, XE::RenderResourceStatesType after )
{
	return XE::RenderResourceStatesType::COMMON;
}

XE::RenderResourceStatesType XE::GraphicsService::TransitionBarrier( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle texture, XE::RenderResourceStatesType after )
{
	return XE::RenderResourceStatesType::COMMON;
}

XE::RenderPassHandle XE::GraphicsService::BeginPass( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, const XE::RenderPassDesc & desc )
{
	return _p->_RenderPassHandleAllocator.Alloc();
}

void XE::GraphicsService::SetViewport( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Rectf & view )
{

}

void XE::GraphicsService::SetViewport( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::Rectf > & views )
{

}

void XE::GraphicsService::SetScissor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Rectf & rect )
{

}

void XE::GraphicsService::SetScissor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::Rectf > & rects )
{

}

void XE::GraphicsService::SetIndexBuffer( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderVirtualBufferHandle buffer )
{

}

void XE::GraphicsService::SetVertexBuffer( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderVirtualBufferHandle buffer )
{

}

void XE::GraphicsService::SetDescriptorSet( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderDescriptorSetHandle heap, XE::uint32 first, XE::RenderPipelineStateType bind )
{

}

void XE::GraphicsService::SetPipelineState( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderPipelineStateHandle pso )
{

}

void XE::GraphicsService::SetUniform( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 location, const XE::Variant & value )
{

}

void XE::GraphicsService::SetUniform( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 location, const XE::VariantArray & value )
{

}

void XE::GraphicsService::SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{

}

void XE::GraphicsService::SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderTextureHandle texture, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{

}

void XE::GraphicsService::SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderSamplerHandle sampler, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{

}

void XE::GraphicsService::ResetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderResourceType type, const XE::Pair< XE::uint32, XE::uint32 > & slot_ranges, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{

}

void XE::GraphicsService::SetBlendFactor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::FColor & color )
{

}

void XE::GraphicsService::SetStencilReference( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 reference, XE::RenderStencilFaceType stencilFace /*= XE::RenderStencilFaceType::FRONT_BACK */ )
{

}

void XE::GraphicsService::BeginQuery( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 value )
{

}

void XE::GraphicsService::EndQuery( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 value )
{

}

void XE::GraphicsService::BeginCondition( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 offset, XE::RenderConditionType cond )
{

}

void XE::GraphicsService::EndCondition( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query )
{

}

void XE::GraphicsService::BeginStreamOutput( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::RenderBufferHandle > & buffers )
{

}

void XE::GraphicsService::EndStreamOutput( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass )
{

}

void XE::GraphicsService::Draw( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices )
{

}

void XE::GraphicsService::DrawIndexed( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & indices )
{

}

void XE::GraphicsService::DrawIndexed( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & indices, XE::uint32 vertex_offset )
{

}

void XE::GraphicsService::DrawInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices, XE::uint32 num_instances )
{

}

void XE::GraphicsService::DrawInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices, const XE::Pair< XE::uint32, XE::uint32 > & instances )
{

}

void XE::GraphicsService::DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index )
{

}

void XE::GraphicsService::DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index, XE::uint32 vertex_offset )
{

}

void XE::GraphicsService::DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index, XE::uint32 vertex_offset, XE::uint32 first_instance )
{

}

void XE::GraphicsService::DrawIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset )
{

}

void XE::GraphicsService::DrawIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset, XE::uint32 num_commands, XE::uint32 stride )
{

}

void XE::GraphicsService::DrawIndexedIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset )
{

}

void XE::GraphicsService::DrawIndexedIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset, XE::uint32 num_commands, XE::uint32 stride )
{

}

void XE::GraphicsService::ExecuteBundle( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderCommandBundleHandle bundle )
{

}

void XE::GraphicsService::EndPass( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass )
{
	_p->_RenderPassHandleAllocator.Free( pass );
}

void XE::GraphicsService::Dispatch( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::uint32 group_x, XE::uint32 group_y, XE::uint32 group_z )
{

}

void XE::GraphicsService::DispatchIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle buffer, XE::uint64 offset )
{

}

void XE::GraphicsService::QueryResolve( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderQueryHandle query, XE::uint32 offset, XE::uint32 size )
{

}

void XE::GraphicsService::QueryResult( XE::RenderContextHandle context, XE::RenderQueryHandle query, XE::uint32 offset, XE::uint32 size, std::ostream & stream )
{

}

void XE::GraphicsService::End( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf )
{
	_p->_RenderCommandBufferHandleAllocator.Free( cmdbuf );
}

void XE::GraphicsService::BeginBundle( XE::RenderContextHandle context, XE::RenderCommandBundleHandle bundle )
{

}

void XE::GraphicsService::EndBundle( XE::RenderContextHandle context, XE::RenderCommandBundleHandle bundle )
{

}

void XE::GraphicsService::Execute( XE::RenderContextHandle context )
{

}

void XE::GraphicsService::Wait( XE::RenderContextHandle context )
{

}

void XE::GraphicsService::OnMainWindowResize()
{

}

#endif // !defined( NULL_RENDER ) && ( PLATFORM_OS & OS_EMSCRIPTEN )

