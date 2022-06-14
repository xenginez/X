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

#include "imconfig.h"
#include "imgui.h"
#include "imgui_internal.h"

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

DECL_META_CLASS( ImGuiStyle );
template<> struct XE::Serializable< ImGuiStyle >
{
	static constexpr const char * COLOR_NAME[] =
	{
		"ColorText",
		"ColorTextDisabled",
		"ColorWindowBg",
		"ColorChildBg",
		"ColorPopupBg",
		"ColorBorder",
		"ColorBorderShadow",
		"ColorFrameBg",
		"ColorFrameBgHovered",
		"ColorFrameBgActive",
		"ColorTitleBg",
		"ColorTitleBgActive",
		"ColorTitleBgCollapsed",
		"ColorMenuBarBg",
		"ColorScrollbarBg",
		"ColorScrollbarGrab",
		"ColorScrollbarGrabHovered",
		"ColorScrollbarGrabActive",
		"ColorCheckMark",
		"ColorSliderGrab",
		"ColorSliderGrabActive",
		"ColorButton",
		"ColorButtonHovered",
		"ColorButtonActive",
		"ColorHeader",
		"ColorHeaderHovered",
		"ColorHeaderActive",
		"ColorSeparator",
		"ColorSeparatorHovered",
		"ColorSeparatorActive",
		"ColorResizeGrip",
		"ColorResizeGripHovered",
		"ColorResizeGripActive",
		"ColorTab",
		"ColorTabHovered",
		"ColorTabActive",
		"ColorTabUnfocused",
		"ColorTabUnfocusedActive",
		"ColorPlotLines",
		"ColorPlotLinesHovered",
		"ColorPlotHistogram",
		"ColorPlotHistogramHovered",
		"ColorTableHeaderBg",
		"ColorTableBorderStrong",
		"ColorTableBorderLight",
		"ColorTableRowBg",
		"ColorTableRowBgAlt",
		"ColorTextSelectedBg",
		"ColorDragDropTarget",
		"ColorNavHighlight",
		"ColorNavWindowingHighlight",
		"ColorNavWindowingDimBg",
		"ColorModalWindowDimBg",
	};

#define NVP( NAME ) arc & ARCHIVE_NVP( #NAME, val.NAME )
#define V2_NVP( NAME ) { XE::Vec2f NAME = val.NAME; arc & ARCHIVE_NVP( #NAME, NAME ); }
#define V3_NVP( NAME ) { XE::Vec2f NAME; arc & ARCHIVE_NVP( #NAME, NAME ); val.NAME.x = NAME.x; val.NAME.y = NAME.y; }

	static void Serialize( OArchive & arc, const ImGuiStyle & val )
	{
		NVP( Alpha );
		NVP( DisabledAlpha );
		NVP( WindowRounding );
		NVP( WindowBorderSize );
		NVP( ChildRounding );
		NVP( ChildBorderSize );
		NVP( PopupRounding );
		NVP( PopupBorderSize );
		NVP( FrameRounding );
		NVP( FrameBorderSize );
		NVP( IndentSpacing );
		NVP( ColumnsMinSpacing );
		NVP( ScrollbarSize );
		NVP( ScrollbarRounding );
		NVP( GrabMinSize );
		NVP( GrabRounding );
		NVP( LogSliderDeadzone );
		NVP( TabRounding );
		NVP( TabBorderSize );
		NVP( TabMinWidthForCloseButton );
		NVP( MouseCursorScale );
		NVP( AntiAliasedLines );
		NVP( AntiAliasedLinesUseTex );
		NVP( AntiAliasedFill );
		NVP( CurveTessellationTol );
		NVP( CircleTessellationMaxError );
		NVP( WindowMenuButtonPosition );
		NVP( ColorButtonPosition );

		V2_NVP( WindowPadding );
		V2_NVP( WindowMinSize );
		V2_NVP( WindowTitleAlign );
		V2_NVP( FramePadding );
		V2_NVP( ItemSpacing );
		V2_NVP( ItemInnerSpacing );
		V2_NVP( CellPadding );
		V2_NVP( TouchExtraPadding );
		V2_NVP( ButtonTextAlign );
		V2_NVP( SelectableTextAlign );
		V2_NVP( DisplayWindowPadding );
		V2_NVP( DisplaySafeAreaPadding );

		for ( size_t i = 0; i < ImGuiCol_COUNT; i++ )
		{
			XE::Color col( val.Colors[i].x, val.Colors[i].y, val.Colors[i].z, val.Colors[i].w );
			arc & ARCHIVE_NVP( COLOR_NAME[i], col );
		}
	}

	static void Deserialize( IArchive & arc, ImGuiStyle & val )
	{
		NVP( Alpha );
		NVP( DisabledAlpha );
		NVP( WindowRounding );
		NVP( WindowBorderSize );
		NVP( ChildRounding );
		NVP( ChildBorderSize );
		NVP( PopupRounding );
		NVP( PopupBorderSize );
		NVP( FrameRounding );
		NVP( FrameBorderSize );
		NVP( IndentSpacing );
		NVP( ColumnsMinSpacing );
		NVP( ScrollbarSize );
		NVP( ScrollbarRounding );
		NVP( GrabMinSize );
		NVP( GrabRounding );
		NVP( LogSliderDeadzone );
		NVP( TabRounding );
		NVP( TabBorderSize );
		NVP( TabMinWidthForCloseButton );
		NVP( MouseCursorScale );
		NVP( AntiAliasedLines );
		NVP( AntiAliasedLinesUseTex );
		NVP( AntiAliasedFill );
		NVP( CurveTessellationTol );
		NVP( CircleTessellationMaxError );
		NVP( WindowMenuButtonPosition );
		NVP( ColorButtonPosition );

		V3_NVP( WindowPadding );
		V3_NVP( WindowMinSize );
		V3_NVP( WindowTitleAlign );
		V3_NVP( FramePadding );
		V3_NVP( ItemSpacing );
		V3_NVP( ItemInnerSpacing );
		V3_NVP( CellPadding );
		V3_NVP( TouchExtraPadding );
		V3_NVP( ButtonTextAlign );
		V3_NVP( SelectableTextAlign );
		V3_NVP( DisplayWindowPadding );
		V3_NVP( DisplaySafeAreaPadding );

		for ( size_t i = 0; i < ImGuiCol_COUNT; i++ )
		{
			XE::Color col( val.Colors[i].x, val.Colors[i].y, val.Colors[i].z, val.Colors[i].w );
			arc & ARCHIVE_NVP( COLOR_NAME[i], col );
			val.Colors[i].x = col.r; val.Colors[i].y = col.g; val.Colors[i].z = col.b; val.Colors[i].w = col.a;
		}
	}

#undef NVP
#undef V2_NVP
#undef V3_NVP
};

#endif//IMGUI_IMPL_H__CD8160B4_FC65_4B9A_9A82_F9948618AFA2
