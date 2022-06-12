/*!
 * \file	imgui_impl.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef IMGUI_IMPL_H__CD8160B4_FC65_4B9A_9A82_F9948618AFA2
#define IMGUI_IMPL_H__CD8160B4_FC65_4B9A_9A82_F9948618AFA2

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ImGuiImpl : public XE::EnableSharedFromThis< XE::ImGuiImpl >
{
private:
	struct Private;

public:
	ImGuiImpl();

	~ImGuiImpl();

public:
	void Startup( XE::GraphicsDeviceHandle device, XE::int32 num_frames_in_flight, XE::GraphicsTextureFormat rt_format );

	void Clearup();

public:
	bool StartupContext( ImGuiContext * ctx );

	void RenderContext( ImGuiContext * ctx, XE::GraphicsRenderPassEncoderHandle pass_encoder );

	void ClearupContext( ImGuiContext * ctx );

private:
	void InvalidateDeviceObjects( ImGuiContext * ctx );

	bool CreateDeviceObjects( ImGuiContext * ctx );

	void SetupRenderState( ImGuiContext * ctx, ImDrawData * draw_data, XE::GraphicsRenderPassEncoderHandle pass_encoder, void * frame_resources );

private:
	void CreateFontsTexture( ImGuiContext * ctx );

	void CreateUniformBuffer( ImGuiContext * ctx );

	XE::GraphicsProgrammableStageDescriptor CreateShaderModule( const XE::uint32 * binary_data, uint32_t binary_data_size );

	XE::GraphicsBindGroupHandle CreateImageBindGroup( XE::GraphicsBindGroupLayoutHandle layout, XE::GraphicsTextureViewHandle texture );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//IMGUI_IMPL_H__CD8160B4_FC65_4B9A_9A82_F9948618AFA2
