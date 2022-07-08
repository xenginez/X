#include "imgui_impl.h"

#include "Core/CoreFramework.h"

IMPLEMENT_META( ImGuiStyle );

namespace
{
	struct RenderResources
	{
		XE::GraphicsTexturePtr FontTexture;
		XE::GraphicsTextureViewPtr FontTextureView;
		XE::GraphicsSamplerPtr Sampler;
		XE::GraphicsBufferPtr Uniforms;
		XE::GraphicsBindGroupPtr CommonBindGroup;
		XE::GraphicsBindGroupPtr ImageBindGroup;
		XE::GraphicsBindGroupLayoutPtr ImageBindGroupLayout;
		ImGuiStorage ImageBindGroups;
	};

	struct FrameResources
	{
		XE::int32 IndexBufferSize = 0;
		XE::int32 VertexBufferSize = 0;
		ImDrawIdx * IndexBufferHost = nullptr;
		ImDrawVert * VertexBufferHost = nullptr;
		XE::GraphicsBufferPtr IndexBuffer;
		XE::GraphicsBufferPtr VertexBuffer;
	};

    static uint32_t shader_vert_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
        0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
        0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
        0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
        0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
        0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
        0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
        0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
        0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
        0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
        0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
        0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
        0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
        0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
        0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
        0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
        0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
        0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
        0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
        0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
        0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
        0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
        0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
        0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
        0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
        0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
        0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
        0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
        0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
        0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
        0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
        0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
        0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
        0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
        0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
        0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
        0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
        0x0000002d,0x0000002c,0x000100fd,0x00010038
    };

    static uint32_t shader_frag_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
        0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
        0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
        0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
        0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
        0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
        0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
        0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
        0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
        0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
        0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
        0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
        0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
        0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
        0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
        0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
        0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
        0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
        0x00010038
    };

	struct ImGuiContextPrivate
	{
		RenderResources _Resources;
		XE::GraphicsRenderPipelinePtr _PipelineState;
	};
}

struct XE::ImGuiImpl::Private
{
	FrameResources * _FrameResources = nullptr;
	XE::uint32 _NumFramesInFlight = 0;
	XE::uint32 _FrameIndex = std::numeric_limits<XE::uint32>::max();

	XE::GraphicsDevicePtr _Device;
	XE::GraphicsQueuePtr _DefaultQueue;
	XE::GraphicsTextureFormat _RenderTargetFormat = XE::GraphicsTextureFormat::UNDEFINED;

	XE::Array< XE::GraphicsBindGroupPtr > _BindGroups;
	XE::Array< XE::GraphicsTextureViewPtr > _TextureViews;
	XE::ConcurrentHashMap< ImGuiContext *, ImGuiContextPrivate * > _Impls;

	XE::GraphicsServicePtr _GraphicsService;
};

XE::ImGuiImpl::ImGuiImpl()
	:_p( XE::New< Private >() )
{

}

XE::ImGuiImpl::~ImGuiImpl()
{
	XE::Delete( _p );
}

void XE::ImGuiImpl::Startup( XE::GraphicsDevicePtr device, XE::int32 num_frames_in_flight, XE::GraphicsTextureFormat rt_format )
{
	_p->_GraphicsService = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::GraphicsService >();
	_p->_Device = device;
	_p->_DefaultQueue = _p->_GraphicsService->DeviceGetQueue( device );
	_p->_RenderTargetFormat = rt_format;
	_p->_FrameResources = new FrameResources[num_frames_in_flight];
	_p->_NumFramesInFlight = num_frames_in_flight;
	_p->_FrameIndex = std::numeric_limits<XE::uint32>::max();

	for ( int i = 0; i < num_frames_in_flight; i++ )
	{
		FrameResources * fr = &_p->_FrameResources[i];
		fr->IndexBuffer = nullptr;
		fr->VertexBuffer = nullptr;
		fr->IndexBufferHost = nullptr;
		fr->VertexBufferHost = nullptr;
		fr->IndexBufferSize = 10000;
		fr->VertexBufferSize = 5000;
	}
}

void XE::ImGuiImpl::Clearup()
{
	for ( unsigned int i = 0; i < _p->_NumFramesInFlight; i++ )
	{
		_p->_FrameResources[i].IndexBuffer = nullptr;
		_p->_FrameResources[i].VertexBuffer = nullptr;
		if ( _p->_FrameResources[i].IndexBufferHost != nullptr ) delete[] _p->_FrameResources[i].IndexBufferHost;
		if ( _p->_FrameResources[i].VertexBufferHost != nullptr ) delete[] _p->_FrameResources[i].VertexBufferHost;
	}

	delete[] _p->_FrameResources;
	_p->_FrameResources = nullptr;

	_p->_DefaultQueue = nullptr;

	_p->_Device = {};
	_p->_NumFramesInFlight = 0;
	_p->_FrameIndex = std::numeric_limits<XE::uint32>::max();

	_p->_GraphicsService = nullptr;
}

bool XE::ImGuiImpl::StartupContext( ImGuiContext * ctx )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		p = XE::New< ImGuiContextPrivate >();

		p->_Resources.FontTexture = nullptr;
		p->_Resources.FontTextureView = nullptr;
		p->_Resources.Sampler = nullptr;
		p->_Resources.Uniforms = nullptr;
		p->_Resources.CommonBindGroup = nullptr;
		p->_Resources.ImageBindGroups.Data.reserve( 100 );
		p->_Resources.ImageBindGroup = nullptr;
		p->_Resources.ImageBindGroupLayout = nullptr;

		_p->_Impls.insert( ctx, p );

		CreateDeviceObjects( ctx );

		return true;
	}

	return false;
}

void XE::ImGuiImpl::ClearupContext( ImGuiContext * ctx )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		delete p;
		_p->_Impls.erase( ctx );
	}
}

void XE::ImGuiImpl::RenderContext( ImGuiContext * ctx, XE::GraphicsRenderPassEncoderPtr pass_encoder )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		if ( !ctx->Viewports[0]->DrawDataP.Valid )
			return;

		ImDrawData * draw_data = &ctx->Viewports[0]->DrawDataP;

		// Avoid rendering when minimized
		if ( draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f )
			return;

		// FIXME: Assuming that this only gets called once per frame!
		// If not, we can't just re-allocate the IB or VB, we'll have to do a proper allocator.
		_p->_FrameIndex = _p->_FrameIndex + 1;
		FrameResources * fr = &_p->_FrameResources[_p->_FrameIndex % _p->_NumFramesInFlight];

		// Create and grow vertex/index buffers if needed
		if ( !fr->VertexBuffer || fr->VertexBufferSize < draw_data->TotalVtxCount )
		{
			fr->VertexBuffer = nullptr;
			delete fr->VertexBufferHost;

			fr->VertexBufferSize = draw_data->TotalVtxCount + 5000;

			XE::GraphicsBufferDescriptor vb_desc =
			{
				"Dear ImGui Vertex buffer",
				XE::MakeFlags( XE::GraphicsBufferUsage::COPY_DST, XE::GraphicsBufferUsage::VERTEX ),
				fr->VertexBufferSize * sizeof( ImDrawVert ),
				false
			};
			fr->VertexBuffer = _p->_GraphicsService->DeviceCreateBuffer( _p->_Device, vb_desc );
			if ( !fr->VertexBuffer )
				return;

			fr->VertexBufferHost = new ImDrawVert[fr->VertexBufferSize];
		}
		if ( !fr->IndexBuffer || fr->IndexBufferSize < draw_data->TotalIdxCount )
		{
			fr->IndexBuffer = nullptr;
			delete fr->IndexBufferHost;

			fr->IndexBufferSize = draw_data->TotalIdxCount + 10000;

			XE::GraphicsBufferDescriptor ib_desc =
			{
				"Dear ImGui Index buffer",
				XE::MakeFlags( XE::GraphicsBufferUsage::COPY_DST, XE::GraphicsBufferUsage::INDEX ),
				fr->IndexBufferSize * sizeof( ImDrawIdx ),
				false
			};
			fr->IndexBuffer = _p->_GraphicsService->DeviceCreateBuffer( _p->_Device, ib_desc );
			if ( !fr->IndexBuffer )
				return;

			fr->IndexBufferHost = new ImDrawIdx[fr->IndexBufferSize];
		}

		// Upload vertex/index data into a single contiguous GPU buffer
		ImDrawVert * vtx_dst = (ImDrawVert *)fr->VertexBufferHost;
		ImDrawIdx * idx_dst = (ImDrawIdx *)fr->IndexBufferHost;
		for ( XE::uint64 n = 0; n < draw_data->CmdListsCount; n++ )
		{
			const ImDrawList * cmd_list = draw_data->CmdLists[n];
			memcpy( vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof( ImDrawVert ) );
			memcpy( idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof( ImDrawIdx ) );
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		XE::uint64 vb_write_size = ( (char *)vtx_dst - (char *)fr->VertexBufferHost + 3 ) & ~3;
		XE::uint64 ib_write_size = ( (char *)idx_dst - (char *)fr->IndexBufferHost + 3 ) & ~3;
		_p->_GraphicsService->QueueWriteBuffer( _p->_DefaultQueue, fr->VertexBuffer, 0, { reinterpret_cast<const char *>( fr->VertexBufferHost ), vb_write_size } );
		_p->_GraphicsService->QueueWriteBuffer( _p->_DefaultQueue, fr->IndexBuffer, 0, { reinterpret_cast<const char *>( fr->IndexBufferHost ), ib_write_size } );

		// Setup desired render state
		SetupRenderState( ctx, draw_data, pass_encoder, fr );

		// Render command lists
		// (Because we merged all buffers into a single one, we maintain our own offset into them)
		int global_vtx_offset = 0;
		int global_idx_offset = 0;
		ImVec2 clip_scale = draw_data->FramebufferScale;
		ImVec2 clip_off = draw_data->DisplayPos;
		for ( int n = 0; n < draw_data->CmdListsCount; n++ )
		{
			const ImDrawList * cmd_list = draw_data->CmdLists[n];
			for ( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ )
			{
				const ImDrawCmd * pcmd = &cmd_list->CmdBuffer[cmd_i];
				if ( pcmd->UserCallback != NULL )
				{
					// User callback, registered via ImDrawList::AddCallback()
					// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
					if ( pcmd->UserCallback == ImDrawCallback_ResetRenderState )
						SetupRenderState( ctx, draw_data, pass_encoder, fr );
					else
						pcmd->UserCallback( cmd_list, pcmd );
				}
				else
				{
					// Bind custom texture
					ImTextureID tex_id = pcmd->GetTexID();
					ImGuiID tex_id_hash = ImHashData( &tex_id, sizeof( tex_id ) );
					auto bind_group = _p->_BindGroups[p->_Resources.ImageBindGroups.GetInt( tex_id_hash )];
					if ( !bind_group )
					{
						bind_group = CreateImageBindGroup( p->_Resources.ImageBindGroupLayout, _p->_TextureViews[tex_id] );
						_p->_BindGroups.push_back( bind_group );
						p->_Resources.ImageBindGroups.SetInt( tex_id_hash, _p->_BindGroups.size() - 1 );
					}
					_p->_GraphicsService->RenderPassEncoderSetBindGroup( pass_encoder, 1, bind_group, {} );

					// Project scissor/clipping rectangles into framebuffer space
					ImVec2 clip_min( ( pcmd->ClipRect.x - clip_off.x ) * clip_scale.x, ( pcmd->ClipRect.y - clip_off.y ) * clip_scale.y );
					ImVec2 clip_max( ( pcmd->ClipRect.z - clip_off.x ) * clip_scale.x, ( pcmd->ClipRect.w - clip_off.y ) * clip_scale.y );
					if ( clip_max.x <= clip_min.x || clip_max.y <= clip_min.y )
						continue;

					// Apply scissor/clipping rectangle, Draw
					_p->_GraphicsService->RenderPassEncoderSetScissorRect( pass_encoder, { (XE::int32)clip_min.x, (XE::int32)clip_min.y, (XE::int32)( clip_max.x - clip_min.x ), (XE::int32)( clip_max.y - clip_min.y ) } );
					_p->_GraphicsService->RenderPassEncoderDrawIndexed( pass_encoder, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0 );
				}
			}
			global_idx_offset += cmd_list->IdxBuffer.Size;
			global_vtx_offset += cmd_list->VtxBuffer.Size;
		}
	}
}

void XE::ImGuiImpl::InvalidateDeviceObjects( ImGuiContext * ctx )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		_p->_Device = nullptr;
		p->_PipelineState = nullptr;
		p->_Resources.FontTexture = nullptr;
		p->_Resources.FontTextureView = nullptr;
		p->_Resources.Sampler = nullptr;
		p->_Resources.Uniforms = nullptr;
		p->_Resources.CommonBindGroup = nullptr;
		p->_Resources.ImageBindGroup = nullptr;
		p->_Resources.ImageBindGroupLayout = nullptr;

		ctx->IO.Fonts->SetTexID( 0 );
	}
}

bool XE::ImGuiImpl::CreateDeviceObjects( ImGuiContext * ctx )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		if ( !_p->_Device )
			return false;

		if ( !p->_PipelineState )
		{
			InvalidateDeviceObjects( ctx );
		}

		// Create render pipeline
		XE::GraphicsRenderPipelineDescriptor graphics_pipeline_desc = {};
		graphics_pipeline_desc.Primitive.Topology = XE::GraphicsPrimitiveTopology::TRIANGLE_LIST;
		graphics_pipeline_desc.Primitive.StripIndexFormat = XE::GraphicsIndexFormat::UNDEFINED;
		graphics_pipeline_desc.Primitive.FrontFace = XE::GraphicsFrontFace::CW;
		graphics_pipeline_desc.Primitive.CullMode = XE::GraphicsCullMode::NONE;
		graphics_pipeline_desc.Multisample.Count = 1;
		graphics_pipeline_desc.Multisample.Mask = std::numeric_limits<XE::uint32>::max();
		graphics_pipeline_desc.Multisample.AlphaToCoverageEnabled = false;

		// Create the vertex shader
		XE::GraphicsProgrammableStageDescriptor vertex_shader_desc = CreateShaderModule( shader_vert_spv, sizeof( shader_vert_spv ) );
		graphics_pipeline_desc.Vertex.Module = vertex_shader_desc.Shader;
		graphics_pipeline_desc.Vertex.EntryPoint = vertex_shader_desc.EntryPoint;

		// Vertex input configuration
		XE::GraphicsVertexBufferLayout buffer_layouts;
		buffer_layouts.ArrayStride = sizeof( ImDrawVert );
		buffer_layouts.StepMode = XE::GraphicsVertexStepMode::VERTEX;
		buffer_layouts.Attributes =
		{
			{ XE::GraphicsVertexFormat::FLOAT32X2, (uint64_t)IM_OFFSETOF( ImDrawVert, pos ), 0 },
			{ XE::GraphicsVertexFormat::FLOAT32X2, (uint64_t)IM_OFFSETOF( ImDrawVert, uv ),  1 },
			{ XE::GraphicsVertexFormat::UNORM8X4,  (uint64_t)IM_OFFSETOF( ImDrawVert, col ), 2 },
		};

		graphics_pipeline_desc.Vertex.Buffers.emplace_back( std::move( buffer_layouts ) );

		// Create the pixel shader
		XE::GraphicsProgrammableStageDescriptor pixel_shader_desc = CreateShaderModule( shader_frag_spv, sizeof( shader_frag_spv ) );

		// Create the blending setup
		XE::GraphicsColorTargetState color_state = {};
		color_state.Format = _p->_RenderTargetFormat;
		color_state.Blend.Alpha.Operation = XE::GraphicsBlendOperation::ADD;
		color_state.Blend.Alpha.SrcFactor = XE::GraphicsBlendFactor::ONE;
		color_state.Blend.Alpha.DstFactor = XE::GraphicsBlendFactor::ONE_MINUS_SRC_ALPHA;
		color_state.Blend.Color.Operation = XE::GraphicsBlendOperation::ADD;
		color_state.Blend.Color.SrcFactor = XE::GraphicsBlendFactor::SRC_ALPHA;
		color_state.Blend.Color.DstFactor = XE::GraphicsBlendFactor::ONE_MINUS_SRC_ALPHA;
		color_state.WriteMask = XE::GraphicsColorWriteMask::ALL;

		graphics_pipeline_desc.Fragment.Module = pixel_shader_desc.Shader;
		graphics_pipeline_desc.Fragment.EntryPoint = pixel_shader_desc.EntryPoint;
		graphics_pipeline_desc.Fragment.Targets.emplace_back( std::move( color_state ) );

		// Create depth-stencil State
		XE::GraphicsDepthStencilState depth_stencil_state = {};
		depth_stencil_state.DepthBias = 0;
		depth_stencil_state.DepthBiasClamp = 0;
		depth_stencil_state.DepthBiasSlopeScale = 0;

		p->_PipelineState = _p->_GraphicsService->DeviceCreateRenderPipeline( _p->_Device, graphics_pipeline_desc );

		CreateFontsTexture( ctx );
		CreateUniformBuffer( ctx );

		// Create resource bind group
		XE::GraphicsBindGroupLayoutPtr bg_layouts[2] = {
		_p->_GraphicsService->RenderPipelineGetBindGroupLayout( p->_PipelineState, 0 ) ,
		_p->_GraphicsService->RenderPipelineGetBindGroupLayout( p->_PipelineState, 1 ) };

		XE::GraphicsBindGroupDescriptor common_bg_descriptor = {};
		common_bg_descriptor.Layout = bg_layouts[0];
		common_bg_descriptor.Entries =
		{
			{ 0, p->_Resources.Uniforms, 0, sizeof( XE::Mat4x4f ), {}, {} },
			{ 1, {},                      0, 0, p->_Resources.Sampler, {} },
		};
		p->_Resources.CommonBindGroup = _p->_GraphicsService->DeviceCreateBindGroup( _p->_Device, common_bg_descriptor );

		XE::GraphicsBindGroupPtr image_bind_group = CreateImageBindGroup( bg_layouts[1], p->_Resources.FontTextureView );
		p->_Resources.ImageBindGroup = image_bind_group;
		p->_Resources.ImageBindGroupLayout = bg_layouts[1];

		_p->_BindGroups.push_back( image_bind_group );
		p->_Resources.ImageBindGroups.SetInt( ImHashData( &p->_Resources.FontTextureView, sizeof( ImTextureID ) ), _p->_BindGroups.size() - 1 );

		vertex_shader_desc.Shader = nullptr;
		pixel_shader_desc.Shader = nullptr;
		bg_layouts[0] = nullptr;

		return true;
	}

	return false;
}

void XE::ImGuiImpl::SetupRenderState( ImGuiContext * ctx, ImDrawData * draw_data, XE::GraphicsRenderPassEncoderPtr pass_encoder, void * resources )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		FrameResources * frame_resources = reinterpret_cast<FrameResources *>( resources );

		// Setup orthographic projection matrix into our constant buffer
		// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right).
		{
			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
			XE::Mat4x4f mvp =
			{
				2.0f / ( R - L ),   0.0f,           0.0f,       0.0f,
				0.0f,         2.0f / ( T - B ),     0.0f,       0.0f,
				0.0f,         0.0f,           0.5f,       0.0f,
				( R + L ) / ( L - R ),  ( T + B ) / ( B - T ),    0.5f,       1.0f,
			};
			_p->_GraphicsService->QueueWriteBuffer( _p->_DefaultQueue, p->_Resources.Uniforms, 0, { reinterpret_cast<const char *>( &mvp ), sizeof( mvp ) } );
		}

		// Setup viewport
		_p->_GraphicsService->RenderPassEncoderSetViewport( pass_encoder, 0, 0, draw_data->FramebufferScale.x * draw_data->DisplaySize.x, draw_data->FramebufferScale.y * draw_data->DisplaySize.y, 0, 1 );

		// Bind shader and vertex buffers
		_p->_GraphicsService->RenderPassEncoderSetVertexBuffer( pass_encoder, 0, frame_resources->VertexBuffer, 0, frame_resources->VertexBufferSize * sizeof( ImDrawVert ) );
		_p->_GraphicsService->RenderPassEncoderSetIndexBuffer( pass_encoder, frame_resources->IndexBuffer, sizeof( ImDrawIdx ) == 2 ? XE::GraphicsIndexFormat::UINT16 : XE::GraphicsIndexFormat::UINT32, 0, frame_resources->IndexBufferSize * sizeof( ImDrawIdx ) );
		_p->_GraphicsService->RenderPassEncoderSetPipeline( pass_encoder, p->_PipelineState );
		_p->_GraphicsService->RenderPassEncoderSetBindGroup( pass_encoder, 0, p->_Resources.CommonBindGroup, {} );

		// Setup blend factor
		_p->_GraphicsService->RenderPassEncoderSetBlendConstant( pass_encoder, { 0, 0, 0, 0 } );
	}
}

void XE::ImGuiImpl::CreateFontsTexture( ImGuiContext * ctx )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
	// Build texture atlas
		ImGuiIO & io = ctx->IO;
		unsigned char * pixels;
		int width, height, size_pp;
		io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height, &size_pp );

		// Upload texture to graphics system
		{
			XE::GraphicsTextureDescriptor tex_desc = {};
			tex_desc.Label = "Dear ImGui Font Texture";
			tex_desc.Dimension = XE::GraphicsTextureDimension::D2;
			tex_desc.Size.x = width;
			tex_desc.Size.y = height;
			tex_desc.Size.z = 1;
			tex_desc.SampleCount = 1;
			tex_desc.Format = XE::GraphicsTextureFormat::RGBA8UNORM;
			tex_desc.MipLevelCount = 1;
			tex_desc.Usage = XE::MakeFlags( XE::GraphicsTextureUsage::COPY_DST, XE::GraphicsTextureUsage::TEXTURE_BINDING );
			p->_Resources.FontTexture = _p->_GraphicsService->DeviceCreateTexture( _p->_Device, tex_desc );

			XE::GraphicsTextureViewDescriptor tex_view_desc = {};
			tex_view_desc.Format = XE::GraphicsTextureFormat::RGBA8UNORM;
			tex_view_desc.Dimension = XE::GraphicsTextureViewDimension::D2;
			tex_view_desc.BaseMipLevel = 0;
			tex_view_desc.MipLevelCount = 1;
			tex_view_desc.BaseArrayLayer = 0;
			tex_view_desc.ArrayLayerCount = 1;
			tex_view_desc.Aspect = XE::GraphicsTextureAspect::ALL;
			p->_Resources.FontTextureView = _p->_GraphicsService->TextureCreateView( p->_Resources.FontTexture, tex_view_desc );
		}

		// Upload texture data
		{
			XE::GraphicsImageCopyTexture dst_view = {};
			dst_view.Texture = p->_Resources.FontTexture;
			dst_view.MipLevel = 0;
			dst_view.Origin = { 0, 0, 0 };
			dst_view.Aspect = XE::GraphicsTextureAspect::ALL;
			XE::GraphicsTextureDataLayout layout = {};
			layout.Offset = 0;
			layout.BytesPerRow = width * size_pp;
			layout.RowsPerImage = height;
			XE::Vec3i size = { (XE::int32)width, (XE::int32)height, 1 };
			_p->_GraphicsService->QueueWriteTexture( _p->_DefaultQueue, dst_view, { reinterpret_cast<const char *>( pixels ), ( (XE::uint64)width * (XE::uint64)size_pp * (XE::uint64)height ) }, layout, size );
		}

		// Create the associated sampler
		// (Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling)
		{
			XE::GraphicsSamplerDescriptor sampler_desc = {};
			sampler_desc.MinFilter = XE::GraphicsFilterMode::LINEAR;
			sampler_desc.MagFilter = XE::GraphicsFilterMode::LINEAR;
			sampler_desc.MipmapFilter = XE::GraphicsMipmapFilterMode::LINEAR;
			sampler_desc.AddressModeU = XE::GraphicsAddressMode::REPEAT;
			sampler_desc.AddressModeV = XE::GraphicsAddressMode::REPEAT;
			sampler_desc.AddressModeW = XE::GraphicsAddressMode::REPEAT;
			sampler_desc.MaxAnisotropy = 1;
			p->_Resources.Sampler = _p->_GraphicsService->DeviceCreateSampler( _p->_Device, sampler_desc );
		}

		// Store our identifier
		auto idx = _p->_TextureViews.size();
		_p->_TextureViews.push_back( p->_Resources.FontTextureView );
		io.Fonts->SetTexID( idx );
	}
}

void XE::ImGuiImpl::CreateUniformBuffer( ImGuiContext * ctx )
{
	ImGuiContextPrivate * p = nullptr;
	if ( !_p->_Impls.find( ctx, p ) )
	{
		XE::GraphicsBufferDescriptor ub_desc =
		{
			"Dear ImGui Uniform buffer",
			 XE::MakeFlags( XE::GraphicsBufferUsage::COPY_DST, XE::GraphicsBufferUsage::UNIFORM ),
			sizeof( XE::Mat4x4f ),
			false
		};
		p->_Resources.Uniforms = _p->_GraphicsService->DeviceCreateBuffer( _p->_Device, ub_desc );
	}
}

XE::GraphicsProgrammableStageDescriptor XE::ImGuiImpl::CreateShaderModule( const XE::uint32 * binary_data, uint32_t binary_data_size )
{
	XE::GraphicsProgrammableStageDescriptor stage_desc = {};

	XE::GraphicsShaderModuleDescriptor desc = {};
	desc.Code = { reinterpret_cast<const char *>( binary_data ), binary_data_size };

	stage_desc.Shader = _p->_GraphicsService->DeviceCreateShaderModule( _p->_Device, desc );
	stage_desc.EntryPoint = "main";

	return stage_desc;
}

XE::GraphicsBindGroupPtr XE::ImGuiImpl::CreateImageBindGroup( XE::GraphicsBindGroupLayoutPtr layout, XE::GraphicsTextureViewPtr texture )
{
	XE::GraphicsBindGroupDescriptor image_bg_descriptor = {};

	image_bg_descriptor.Layout = layout;
	image_bg_descriptor.Entries = { { 0, {}, 0, 0, {}, texture } };

	return _p->_GraphicsService->DeviceCreateBindGroup( _p->_Device, image_bg_descriptor );
}
