#include "RenderService.h"

#if !defined( NULL_RENDER ) && ( PLATFORM_OS & ( OS_WINDOWS | OS_XBOX ) )

#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <D3DX12/d3dx12.h>

#include <tbb/concurrent_queue.h>
#include <D3D12MemAlloc/D3D12MemAlloc.h>

#include "Utils/Logger.h"
#include "Utils/Window.h"
#include "Utils/RefCount.h"
#include "Utils/TaskGuard.h"

#include "CoreFramework.h"
#include "EventService.h"
#include "CacheService.h"
#include "ThreadService.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#undef min
#undef max

namespace
{
static constexpr const char * shaders[] =
{
	"cs_5_0",
	"vs_5_0",
	"ps_5_0",
	"hs_5_0",
	"ds_5_0",
	"gs_5_0"
};
static constexpr D3D_SHADER_MACRO macros[] =
{
#if PLATFORM_OS & OS_WINDOWS
		"OS_WINDOWS", "1",
		"OS_XBOX", "0",
#else
		"OS_WINDOWS", "0",
		"OS_XBOX", "1",
#endif
		"OS_EMSCRIPTEN", "0",
		"OS_MAC", "0",
		"OS_IOS", "0",
		"OS_ANDROID", "0",
		"OS_NINTENDO", "0",
		"OS_PLAY_STATION", "0",
		"OS_LINUX", "0",
		nullptr, nullptr
};

XE::String VendorIDToStr( XE::uint32 vendorID )
{
	switch( vendorID )
	{
	case 0x10001: return "VIV";
	case 0x10002: return "VSI";
	case 0x10003: return "KAZAN";
	case 0x10004: return "CODEPLAY";
	case 0x10005: return "MESA";
	case 0x10006: return "POCL";
	case 0x1002: return "AMD";
	case 0x10DE: return "NVIDIA";
	case 0x8086: return "Intel";
	case 0x1010: return "ImgTec";
	case 0x13B5: return "ARM";
	case 0x5143: return "Qualcomm";
	}

	return XE::ToString( vendorID );
}

XE::RenderFormatType StrideToFormat( XE::uint64 val )
{
	switch( val )
	{
	case 1: return XE::RenderFormatType::R8_UINT;
	case 2: return XE::RenderFormatType::R16_UINT;
	case 4: return XE::RenderFormatType::R32_UINT;
	case 8: return XE::RenderFormatType::R32G32_UINT;
	case 16: return XE::RenderFormatType::R32G32B32A32_UINT;
	}

	return XE::RenderFormatType::R8_UINT;
}

UINT8 To( const XE::Color & color )
{
	UINT8 mask = 0;

	if( color.r ) { mask |= D3D12_COLOR_WRITE_ENABLE_RED; }
	if( color.g ) { mask |= D3D12_COLOR_WRITE_ENABLE_GREEN; }
	if( color.b ) { mask |= D3D12_COLOR_WRITE_ENABLE_BLUE; }
	if( color.a ) { mask |= D3D12_COLOR_WRITE_ENABLE_ALPHA; }

	return mask;
}

DXGI_FORMAT To( XE::RenderFormatType val )
{
#define CASE( FMT ) case XE::RenderFormatType::##FMT: return DXGI_FORMAT_##FMT;
	switch( val )
	{
	CASE( R8_UNORM );
	CASE( R8_SNORM );
	CASE( R8_UINT );
	CASE( R8_SINT );
	CASE( R16_UNORM );
	CASE( R16_SNORM );
	CASE( R16_UINT );
	CASE( R16_SINT );
	CASE( R32_UINT );
	CASE( R32_SINT );
	CASE( R8G8_UNORM );
	CASE( R8G8_SNORM );
	CASE( R8G8_UINT );
	CASE( R8G8_SINT );
	CASE( R16G16_UNORM );
	CASE( R16G16_SNORM );
	CASE( R16G16_UINT );
	CASE( R16G16_SINT );
	CASE( R32G32_UINT );
	CASE( R32G32_SINT );
	CASE( R32G32B32_UINT );
	CASE( R32G32B32_SINT );
	CASE( R8G8B8A8_UNORM );
	CASE( R8G8B8A8_SNORM );
	CASE( R8G8B8A8_UINT );
	CASE( R8G8B8A8_SINT );
	CASE( R16G16B16A16_UNORM );
	CASE( R16G16B16A16_SNORM );
	CASE( R16G16B16A16_UINT );
	CASE( R16G16B16A16_SINT );
	CASE( R32G32B32A32_UINT );
	CASE( R32G32B32A32_SINT );
	CASE( B8G8R8A8_UNORM );
	CASE( D16_UNORM );
	CASE( D24_UNORM_S8_UINT );
	case XE::RenderFormatType::R16_SFLOAT: return DXGI_FORMAT_R16_FLOAT;
	case XE::RenderFormatType::R32_SFLOAT: return DXGI_FORMAT_R32_FLOAT;
	case XE::RenderFormatType::R16G16_SFLOAT: return DXGI_FORMAT_R16G16_FLOAT;
	case XE::RenderFormatType::R32G32_SFLOAT: return DXGI_FORMAT_R32G32_FLOAT;
	case XE::RenderFormatType::R32G32B32_SFLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
	case XE::RenderFormatType::R8G8B8A8_SRGB: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case XE::RenderFormatType::R16G16B16A16_SFLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case XE::RenderFormatType::R32G32B32A32_SFLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case XE::RenderFormatType::B8G8R8A8_SRGB: return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	case XE::RenderFormatType::B5G6R5_UNORM_PACK16: return DXGI_FORMAT_B5G6R5_UNORM;
	case XE::RenderFormatType::B5G5R5A1_UNORM_PACK16: return DXGI_FORMAT_B5G5R5A1_UNORM;
	case XE::RenderFormatType::R10G10B10A2_UNORM_PACK32: return DXGI_FORMAT_R10G10B10A2_UNORM;
	case XE::RenderFormatType::R10G10B10A2_UINT_PACK32: return DXGI_FORMAT_R10G10B10A2_UINT;
	case XE::RenderFormatType::R9G9B9E5_UFLOAT_PACK32: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	case XE::RenderFormatType::R11G11B10_UFLOAT_PACK32: return DXGI_FORMAT_R11G11B10_FLOAT;
	case XE::RenderFormatType::D32_SFLOAT: return DXGI_FORMAT_D32_FLOAT;
	case XE::RenderFormatType::D32_SFLOAT_S8_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case XE::RenderFormatType::BC1_UNORM_BLOCK: return DXGI_FORMAT_BC1_UNORM;
	case XE::RenderFormatType::BC1_SRGB_BLOCK: return DXGI_FORMAT_BC1_UNORM_SRGB;
	case XE::RenderFormatType::BC2_UNORM_BLOCK: return DXGI_FORMAT_BC2_UNORM;
	case XE::RenderFormatType::BC2_SRGB_BLOCK: return DXGI_FORMAT_BC2_UNORM_SRGB;
	case XE::RenderFormatType::BC3_UNORM_BLOCK: return DXGI_FORMAT_BC3_UNORM;
	case XE::RenderFormatType::BC3_SRGB_BLOCK: return DXGI_FORMAT_BC3_UNORM_SRGB;
	case XE::RenderFormatType::BC4_UNORM_BLOCK: return DXGI_FORMAT_BC4_UNORM;
	case XE::RenderFormatType::BC4_SNORM_BLOCK: return DXGI_FORMAT_BC4_SNORM;
	case XE::RenderFormatType::BC5_UNORM_BLOCK: return DXGI_FORMAT_BC5_UNORM;
	case XE::RenderFormatType::BC5_SNORM_BLOCK: return DXGI_FORMAT_BC5_SNORM;
	case XE::RenderFormatType::BC6_UFLOAT_BLOCK: return DXGI_FORMAT_BC6H_UF16;
	case XE::RenderFormatType::BC6_SFLOAT_BLOCK: return DXGI_FORMAT_BC6H_SF16;
	case XE::RenderFormatType::BC7_UNORM_BLOCK: return DXGI_FORMAT_BC7_UNORM;
	case XE::RenderFormatType::BC7_SRGB_BLOCK: return DXGI_FORMAT_BC7_UNORM_SRGB;
	}
#undef CASE

	return {};
}

XE::RenderFormatType To( DXGI_FORMAT val )
{
#define CASE( FMT ) case DXGI_FORMAT_##FMT: return XE::RenderFormatType::##FMT;
	switch( val )
	{
		CASE( R8_UNORM );
		CASE( R8_SNORM );
		CASE( R8_UINT );
		CASE( R8_SINT );
		CASE( R16_UNORM );
		CASE( R16_SNORM );
		CASE( R16_UINT );
		CASE( R16_SINT );
		CASE( R32_UINT );
		CASE( R32_SINT );
		CASE( R8G8_UNORM );
		CASE( R8G8_SNORM );
		CASE( R8G8_UINT );
		CASE( R8G8_SINT );
		CASE( R16G16_UNORM );
		CASE( R16G16_SNORM );
		CASE( R16G16_UINT );
		CASE( R16G16_SINT );
		CASE( R32G32_UINT );
		CASE( R32G32_SINT );
		CASE( R32G32B32_UINT );
		CASE( R32G32B32_SINT );
		CASE( R8G8B8A8_UNORM );
		CASE( R8G8B8A8_SNORM );
		CASE( R8G8B8A8_UINT );
		CASE( R8G8B8A8_SINT );
		CASE( R16G16B16A16_UNORM );
		CASE( R16G16B16A16_SNORM );
		CASE( R16G16B16A16_UINT );
		CASE( R16G16B16A16_SINT );
		CASE( R32G32B32A32_UINT );
		CASE( R32G32B32A32_SINT );
		CASE( B8G8R8A8_UNORM );
		CASE( D16_UNORM );
		CASE( D24_UNORM_S8_UINT );
	case DXGI_FORMAT_R16_FLOAT:return XE::RenderFormatType::R16_SFLOAT;
	case DXGI_FORMAT_R32_FLOAT:return XE::RenderFormatType::R32_SFLOAT;
	case DXGI_FORMAT_R16G16_FLOAT:return XE::RenderFormatType::R16G16_SFLOAT;
	case DXGI_FORMAT_R32G32_FLOAT:return XE::RenderFormatType::R32G32_SFLOAT;
	case DXGI_FORMAT_R32G32B32_FLOAT:return XE::RenderFormatType::R32G32B32_SFLOAT;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:return XE::RenderFormatType::R8G8B8A8_SRGB;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:return XE::RenderFormatType::R16G16B16A16_SFLOAT;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:return XE::RenderFormatType::R32G32B32A32_SFLOAT;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:return XE::RenderFormatType::B8G8R8A8_SRGB;
	case DXGI_FORMAT_B5G6R5_UNORM:return XE::RenderFormatType::B5G6R5_UNORM_PACK16;
	case DXGI_FORMAT_B5G5R5A1_UNORM:return XE::RenderFormatType::B5G5R5A1_UNORM_PACK16;
	case DXGI_FORMAT_R10G10B10A2_UNORM:return XE::RenderFormatType::R10G10B10A2_UNORM_PACK32;
	case DXGI_FORMAT_R10G10B10A2_UINT:return XE::RenderFormatType::R10G10B10A2_UINT_PACK32;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:return XE::RenderFormatType::R9G9B9E5_UFLOAT_PACK32;
	case DXGI_FORMAT_R11G11B10_FLOAT:return XE::RenderFormatType::R11G11B10_UFLOAT_PACK32;
	case DXGI_FORMAT_D32_FLOAT:return XE::RenderFormatType::D32_SFLOAT;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:return XE::RenderFormatType::D32_SFLOAT_S8_UINT;
	case DXGI_FORMAT_BC1_UNORM:return XE::RenderFormatType::BC1_UNORM_BLOCK;
	case DXGI_FORMAT_BC1_UNORM_SRGB:return XE::RenderFormatType::BC1_SRGB_BLOCK;
	case DXGI_FORMAT_BC2_UNORM:return XE::RenderFormatType::BC2_UNORM_BLOCK;
	case DXGI_FORMAT_BC2_UNORM_SRGB:return XE::RenderFormatType::BC2_SRGB_BLOCK;
	case DXGI_FORMAT_BC3_UNORM:return XE::RenderFormatType::BC3_UNORM_BLOCK;
	case DXGI_FORMAT_BC3_UNORM_SRGB:return XE::RenderFormatType::BC3_SRGB_BLOCK;
	case DXGI_FORMAT_BC4_UNORM:return XE::RenderFormatType::BC4_UNORM_BLOCK;
	case DXGI_FORMAT_BC4_SNORM:return XE::RenderFormatType::BC4_SNORM_BLOCK;
	case DXGI_FORMAT_BC5_UNORM:return XE::RenderFormatType::BC5_UNORM_BLOCK;
	case DXGI_FORMAT_BC5_SNORM:return XE::RenderFormatType::BC5_SNORM_BLOCK;
	case DXGI_FORMAT_BC6H_UF16:return XE::RenderFormatType::BC6_UFLOAT_BLOCK;
	case DXGI_FORMAT_BC6H_SF16:return XE::RenderFormatType::BC6_SFLOAT_BLOCK;
	case DXGI_FORMAT_BC7_UNORM:return XE::RenderFormatType::BC7_UNORM_BLOCK;
	case DXGI_FORMAT_BC7_UNORM_SRGB:return XE::RenderFormatType::BC7_SRGB_BLOCK;
	}
#undef CASE

	return {};
}

D3D12_RESOURCE_STATES To( XE::RenderResourceStatesType val )
{
#undef GENERIC_READ
#define CASE( NAME ) case XE::RenderResourceStatesType::##NAME: return  D3D12_RESOURCE_STATE_##NAME
	switch( val )
	{
		CASE( COMMON );
		CASE( VERTEX_AND_CONSTANT_BUFFER );
		CASE( INDEX_BUFFER );
		CASE( RENDER_TARGET );
		CASE( UNORDERED_ACCESS );
		CASE( DEPTH_WRITE );
		CASE( DEPTH_READ );
		CASE( NON_PIXEL_SHADER_RESOURCE );
		CASE( PIXEL_SHADER_RESOURCE );
		CASE( STREAM_OUT );
		CASE( INDIRECT_ARGUMENT );
		CASE( COPY_DEST );
		CASE( COPY_SOURCE );
		CASE( RESOLVE_DEST );
		CASE( RESOLVE_SOURCE );
		CASE( RAYTRACING_ACCELERATION_STRUCTURE );
		CASE( SHADING_RATE_SOURCE );
		CASE( GENERIC_READ );
		CASE( PRESENT );
		CASE( PREDICATION );
	}
#undef CASE

	return {};
}

D3D12_HEAP_TYPE To( XE::RenderAccessType val )
{
	switch( val )
	{
	case XE::RenderAccessType::READ_ONLY:
		return D3D12_HEAP_TYPE_READBACK;
		break;
	case XE::RenderAccessType::WRITE_ONLY:
		return D3D12_HEAP_TYPE_DEFAULT;
		break;
	case XE::RenderAccessType::READ_WRITE:
		return D3D12_HEAP_TYPE_UPLOAD;
		break;
	default:
		break;
	}

	return {};
}

D3D12_RESOURCE_STATES To( XE::RenderBindFlag val )
{
	switch( val )
	{
	case XE::RenderBindFlag::VERTEX_BUFFER:return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	case XE::RenderBindFlag::INDEX_BUFFER:return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	case XE::RenderBindFlag::CONSTANT_BUFFER:return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	case XE::RenderBindFlag::STREAM_OUTPUT_BUFFER:return D3D12_RESOURCE_STATE_STREAM_OUT;
	case XE::RenderBindFlag::INDIRECT_BUFFER:return D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
	case XE::RenderBindFlag::SAMPLED:return D3D12_RESOURCE_STATE_COMMON;
	case XE::RenderBindFlag::STORAGE:return D3D12_RESOURCE_STATE_COMMON;
	case XE::RenderBindFlag::COLOR_ATTACHMENT:return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case XE::RenderBindFlag::DEPTH_STENCIL_ATTACHMENT:return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case XE::RenderBindFlag::COMBINED_SAMPLER:return D3D12_RESOURCE_STATE_COMMON;
	case XE::RenderBindFlag::COPY_SRC:return D3D12_RESOURCE_STATE_COPY_SOURCE;
	case XE::RenderBindFlag::COPY_DST:return D3D12_RESOURCE_STATE_COPY_DEST;
	}

	return {};
}

D3D12_RESOURCE_STATES To( XE::RenderBindFlags val )
{
	D3D12_RESOURCE_STATES states = D3D12_RESOURCE_STATE_COMMON;

	if( val && XE::RenderBindFlag::VERTEX_BUFFER ) states |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	if( val && XE::RenderBindFlag::INDEX_BUFFER ) states |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	if( val && XE::RenderBindFlag::CONSTANT_BUFFER ) states |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	if( val && XE::RenderBindFlag::STREAM_OUTPUT_BUFFER ) states |= D3D12_RESOURCE_STATE_STREAM_OUT;
	if( val && XE::RenderBindFlag::INDIRECT_BUFFER ) states |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
	if( val && XE::RenderBindFlag::SAMPLED ) states |= D3D12_RESOURCE_STATE_COMMON;
	if( val && XE::RenderBindFlag::STORAGE ) states |= D3D12_RESOURCE_STATE_COMMON;
	if( val && XE::RenderBindFlag::COLOR_ATTACHMENT ) states |= D3D12_RESOURCE_STATE_RENDER_TARGET;
	if( val && XE::RenderBindFlag::DEPTH_STENCIL_ATTACHMENT ) states |= D3D12_RESOURCE_STATE_RENDER_TARGET;
	if( val && XE::RenderBindFlag::COMBINED_SAMPLER ) states |= D3D12_RESOURCE_STATE_COMMON;
	if( val && XE::RenderBindFlag::COPY_SRC ) states |= D3D12_RESOURCE_STATE_COPY_SOURCE;
	if( val && XE::RenderBindFlag::COPY_DST ) states |= D3D12_RESOURCE_STATE_COPY_DEST;

	return states;
}

D3D12_TEXTURE_ADDRESS_MODE To( XE::RenderSamplerAddressModeType val )
{
	switch( val )
	{
	case XE::RenderSamplerAddressModeType::REPEAT:
		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	case XE::RenderSamplerAddressModeType::MIRROR:
		return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
	case XE::RenderSamplerAddressModeType::CLAMP:
		return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	case XE::RenderSamplerAddressModeType::BORDER:
		return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	case XE::RenderSamplerAddressModeType::MIRRORONCE:
		return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
	default:
		break;
	}

	return {};
}

D3D12_COMPARISON_FUNC To( XE::RenderCompareOpType val )
{
	switch( val )
	{
	case XE::RenderCompareOpType::NEVER_PASS:
		return D3D12_COMPARISON_FUNC_NEVER;
	case XE::RenderCompareOpType::LESS:
		return D3D12_COMPARISON_FUNC_LESS;
	case XE::RenderCompareOpType::EQUAL:
		return D3D12_COMPARISON_FUNC_EQUAL;
	case XE::RenderCompareOpType::LESS_EQUAL:
		return D3D12_COMPARISON_FUNC_LESS_EQUAL;
	case XE::RenderCompareOpType::GREATER:
		return D3D12_COMPARISON_FUNC_GREATER;
	case XE::RenderCompareOpType::NOT_EQUAL:
		return D3D12_COMPARISON_FUNC_NOT_EQUAL;
	case XE::RenderCompareOpType::GREATER_EQUAL:
		return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	case XE::RenderCompareOpType::ALWAYS_PASS:
		return D3D12_COMPARISON_FUNC_ALWAYS;
	default:
		break;
	}

	return {};
}

D3D12_STENCIL_OP To( XE::RenderStencilOpType val )
{
	switch( val )
	{
	case XE::RenderStencilOpType::KEEP: return D3D12_STENCIL_OP_KEEP;
	case XE::RenderStencilOpType::ZERO: return D3D12_STENCIL_OP_ZERO;
	case XE::RenderStencilOpType::REPLACE: return D3D12_STENCIL_OP_REPLACE;
	case XE::RenderStencilOpType::INC_CLAMP: return D3D12_STENCIL_OP_INCR_SAT;
	case XE::RenderStencilOpType::DEC_CLAMP: return D3D12_STENCIL_OP_DECR_SAT;
	case XE::RenderStencilOpType::INVERT: return D3D12_STENCIL_OP_INVERT;
	case XE::RenderStencilOpType::INC_WRAP: return D3D12_STENCIL_OP_INCR;
	case XE::RenderStencilOpType::DEC_WRAP: return D3D12_STENCIL_OP_DECR;
	default:
		break;
	}

	return {};
}

D3D12_BLEND To( XE::RenderBlendOpType  val )
{
	switch( val )
	{
	case XE::RenderBlendOpType::ZERO: return D3D12_BLEND_ZERO;
	case XE::RenderBlendOpType::ONE: return D3D12_BLEND_ONE;
	case XE::RenderBlendOpType::SRC_COLOR: return D3D12_BLEND_SRC_COLOR;
	case XE::RenderBlendOpType::INV_SRC_COLOR: return D3D12_BLEND_INV_SRC_COLOR;
	case XE::RenderBlendOpType::SRC_ALPHA: return D3D12_BLEND_SRC_ALPHA;
	case XE::RenderBlendOpType::INV_SRC_ALPHA: return D3D12_BLEND_INV_SRC_ALPHA;
	case XE::RenderBlendOpType::DST_COLOR: return D3D12_BLEND_DEST_COLOR;
	case XE::RenderBlendOpType::INV_DST_COLOR: return D3D12_BLEND_INV_DEST_COLOR;
	case XE::RenderBlendOpType::DST_ALPHA: return D3D12_BLEND_DEST_ALPHA;
	case XE::RenderBlendOpType::INV_DST_ALPHA: return D3D12_BLEND_INV_DEST_ALPHA;
	case XE::RenderBlendOpType::SRC_ALPHA_SATURATE: return D3D12_BLEND_SRC_ALPHA_SAT;
	case XE::RenderBlendOpType::BLEND_FACTOR: return D3D12_BLEND_BLEND_FACTOR;
	case XE::RenderBlendOpType::INV_BLEND_FACTOR: return D3D12_BLEND_INV_BLEND_FACTOR;
	case XE::RenderBlendOpType::SRC1_COLOR: return D3D12_BLEND_SRC1_COLOR;
	case XE::RenderBlendOpType::INV_SRC1_COLOR: return D3D12_BLEND_INV_SRC1_COLOR;
	case XE::RenderBlendOpType::SRC1_ALPHA: return D3D12_BLEND_SRC1_ALPHA;
	case XE::RenderBlendOpType::INV_SRC1_ALPHA: return D3D12_BLEND_INV_SRC1_ALPHA;
	default:
		break;
	}

	return {};
}

D3D12_LOGIC_OP To( XE::RenderLogicOpType val )
{
	switch( val )
	{
	case XE::RenderLogicOpType::DISABLED: return {};
	case XE::RenderLogicOpType::CLEAR: return D3D12_LOGIC_OP_CLEAR;
	case XE::RenderLogicOpType::SET: return D3D12_LOGIC_OP_SET;
	case XE::RenderLogicOpType::COPY: return D3D12_LOGIC_OP_COPY;
	case XE::RenderLogicOpType::COPY_INVERTED: return D3D12_LOGIC_OP_COPY_INVERTED;
	case XE::RenderLogicOpType::NOOP: return D3D12_LOGIC_OP_NOOP;
	case XE::RenderLogicOpType::INVERT: return D3D12_LOGIC_OP_INVERT;
	case XE::RenderLogicOpType::AND: return D3D12_LOGIC_OP_AND;
	case XE::RenderLogicOpType::AND_REVERSE: return D3D12_LOGIC_OP_AND_REVERSE;
	case XE::RenderLogicOpType::AND_INVERTED: return D3D12_LOGIC_OP_AND_INVERTED;
	case XE::RenderLogicOpType::NAND: return D3D12_LOGIC_OP_NAND;
	case XE::RenderLogicOpType::OR: return D3D12_LOGIC_OP_OR;
	case XE::RenderLogicOpType::OR_REVERSE: return D3D12_LOGIC_OP_OR_REVERSE;
	case XE::RenderLogicOpType::OR_INVERTED: return D3D12_LOGIC_OP_OR_INVERTED;
	case XE::RenderLogicOpType::NOR: return D3D12_LOGIC_OP_NOR;
	case XE::RenderLogicOpType::XOR: return D3D12_LOGIC_OP_XOR;
	case XE::RenderLogicOpType::EQUIV: return D3D12_LOGIC_OP_EQUIV;
	default:
		break;
	}

	return {};
}

D3D12_BLEND_OP To( XE::RenderBlendArithmeticType val )
{
	switch( val )
	{
	case XE::RenderBlendArithmeticType::ADD: return D3D12_BLEND_OP_ADD;
	case XE::RenderBlendArithmeticType::SUBTRACT: return D3D12_BLEND_OP_SUBTRACT;
	case XE::RenderBlendArithmeticType::REV_SUBTRACT: return D3D12_BLEND_OP_REV_SUBTRACT;
	case XE::RenderBlendArithmeticType::MIN: return D3D12_BLEND_OP_MIN;
	case XE::RenderBlendArithmeticType::MAX: return D3D12_BLEND_OP_MAX;
	default:
		break;
	}

	return {};
}

D3D12_PRIMITIVE_TOPOLOGY To( XE::RenderPrimitiveTopologyType val )
{
	switch( val )
	{
	case XE::RenderPrimitiveTopologyType::POINT_LIST: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	case XE::RenderPrimitiveTopologyType::LINE_LIST: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	case XE::RenderPrimitiveTopologyType::LINE_STRIP: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case XE::RenderPrimitiveTopologyType::LINE_LOOP:
	case XE::RenderPrimitiveTopologyType::LINE_LIST_ADJACENCY: return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
	case XE::RenderPrimitiveTopologyType::LINE_STRIP_ADJACENCY: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
	case XE::RenderPrimitiveTopologyType::TRIANGLE_LIST: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case XE::RenderPrimitiveTopologyType::TRIANGLE_STRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case XE::RenderPrimitiveTopologyType::TRIANGLE_FAN:
	case XE::RenderPrimitiveTopologyType::TRIANGLE_LIST_ADJACENCY: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
	case XE::RenderPrimitiveTopologyType::TRIANGLE_STRIP_ADJACENCY: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
	case XE::RenderPrimitiveTopologyType::PATCHES_1: return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_2: return D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_3: return D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_4: return D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_5: return D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_6: return D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_7: return D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_8: return D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_9: return D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_10: return D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_11: return D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_12: return D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_13: return D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_14: return D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_15: return D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_16: return D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_17: return D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_18: return D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_19: return D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_20: return D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_21: return D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_22: return D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_23: return D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_24: return D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_25: return D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_26: return D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_27: return D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_28: return D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_29: return D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_30: return D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_31: return D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
	case XE::RenderPrimitiveTopologyType::PATCHES_32: return D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
	default:
		break;
	}

	return {};
}

D3D12_FILL_MODE To( XE::RenderPolygonType val )
{
	switch( val )
	{
	case XE::RenderPolygonType::FILL: return D3D12_FILL_MODE_SOLID;
	case XE::RenderPolygonType::WIREFRAME: return D3D12_FILL_MODE_WIREFRAME;
	case XE::RenderPolygonType::POINTS:
	default:
		break;
	}

	return {};
}

D3D12_CULL_MODE To( XE::RenderCullType val )
{
	switch( val )
	{
	case XE::RenderCullType::DISABLED: return D3D12_CULL_MODE_NONE;
	case XE::RenderCullType::FRONT: return D3D12_CULL_MODE_FRONT;
	case XE::RenderCullType::BACK: return D3D12_CULL_MODE_BACK;
	default:
		break;
	}

	return {};
}

D3D12_QUERY_TYPE To( XE::RenderQueryType val )
{
	switch( val )
	{
	case XE::RenderQueryType::SAMPLES_PASSED: return D3D12_QUERY_TYPE_OCCLUSION;
	case XE::RenderQueryType::ANY_SAMPLES_PASSED:
	case XE::RenderQueryType::ANY_SAMPLES_PASSED_CONSERVATIVE: return D3D12_QUERY_TYPE_BINARY_OCCLUSION;
	case XE::RenderQueryType::TIME_ELAPSED: return D3D12_QUERY_TYPE_TIMESTAMP;
	case XE::RenderQueryType::STREAMOUT_PRIMITIVES_WRITTEN: return D3D12_QUERY_TYPE_SO_STATISTICS_STREAM0;
	case XE::RenderQueryType::STREAMOUT_OVERFLOW: return D3D12_QUERY_TYPE_SO_STATISTICS_STREAM1;
	case XE::RenderQueryType::PIPELINE_STATISTICS: return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
	default:
		break;
	}

	return {};
}

D3D12_PREDICATION_OP To( XE::RenderConditionType val )
{
	switch( val )
	{
	case XE::RenderConditionType::WAIT_INVERTED: return D3D12_PREDICATION_OP_NOT_EQUAL_ZERO;
	case XE::RenderConditionType::WAIT:
	case XE::RenderConditionType::NO_WAIT:
	case XE::RenderConditionType::BY_REGION_WAIT:
	case XE::RenderConditionType::BY_REGION_NO_WAIT:
	case XE::RenderConditionType::NO_WAIT_INVERTED:
	case XE::RenderConditionType::BY_REGION_WAIT_INVERTED:
	case XE::RenderConditionType::BY_REGION_NO_WAIT_INVERTED: return D3D12_PREDICATION_OP_EQUAL_ZERO;
	}

	return {};
}

D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE To( XE::RenderAttachmentLoadOpType val )
{
	switch( val )
	{
	case XE::RenderAttachmentLoadOpType::DISCARD: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
	case XE::RenderAttachmentLoadOpType::PRESERVE: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
	case XE::RenderAttachmentLoadOpType::CLEAR: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
	case XE::RenderAttachmentLoadOpType::NO_ACCESS: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
	default:
		break;
	}

	return {};
}

D3D12_RENDER_PASS_ENDING_ACCESS_TYPE To( XE::RenderAttachmentStoreOpType val )
{
	switch( val )
	{
	case XE::RenderAttachmentStoreOpType::DISCARD: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
	case XE::RenderAttachmentStoreOpType::PRESERVE: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
	case XE::RenderAttachmentStoreOpType::RESOLVE: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
	case XE::RenderAttachmentStoreOpType::NO_ACCESS: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
	default:
		break;
	}

	return {};
}

D3D12_QUERY_HEAP_TYPE To( XE::RenderQueryType val, D3D12_QUERY_HEAP_TYPE )
{
	switch( val )
	{
	case XE::RenderQueryType::SAMPLES_PASSED:
	case XE::RenderQueryType::ANY_SAMPLES_PASSED:
	case XE::RenderQueryType::ANY_SAMPLES_PASSED_CONSERVATIVE: return D3D12_QUERY_HEAP_TYPE_OCCLUSION;
	case XE::RenderQueryType::TIME_ELAPSED: return D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
	case XE::RenderQueryType::STREAMOUT_PRIMITIVES_WRITTEN:
	case XE::RenderQueryType::STREAMOUT_OVERFLOW: return D3D12_QUERY_HEAP_TYPE_SO_STATISTICS;
	case XE::RenderQueryType::PIPELINE_STATISTICS: return D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;
	default:
		break;
	}

	return {};
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE To( XE::RenderPrimitiveTopologyType val, D3D12_PRIMITIVE_TOPOLOGY_TYPE )
{
	switch( val )
	{
	case XE::RenderPrimitiveTopologyType::POINT_LIST:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	case XE::RenderPrimitiveTopologyType::LINE_LIST:
	case XE::RenderPrimitiveTopologyType::LINE_STRIP:
	case XE::RenderPrimitiveTopologyType::LINE_LOOP:
	case XE::RenderPrimitiveTopologyType::LINE_LIST_ADJACENCY:
	case XE::RenderPrimitiveTopologyType::LINE_STRIP_ADJACENCY:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case XE::RenderPrimitiveTopologyType::TRIANGLE_LIST:
	case XE::RenderPrimitiveTopologyType::TRIANGLE_STRIP:
	case XE::RenderPrimitiveTopologyType::TRIANGLE_FAN:
	case XE::RenderPrimitiveTopologyType::TRIANGLE_LIST_ADJACENCY:
	case XE::RenderPrimitiveTopologyType::TRIANGLE_STRIP_ADJACENCY:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	case XE::RenderPrimitiveTopologyType::PATCHES_1:
	case XE::RenderPrimitiveTopologyType::PATCHES_2:
	case XE::RenderPrimitiveTopologyType::PATCHES_3:
	case XE::RenderPrimitiveTopologyType::PATCHES_4:
	case XE::RenderPrimitiveTopologyType::PATCHES_5:
	case XE::RenderPrimitiveTopologyType::PATCHES_6:
	case XE::RenderPrimitiveTopologyType::PATCHES_7:
	case XE::RenderPrimitiveTopologyType::PATCHES_8:
	case XE::RenderPrimitiveTopologyType::PATCHES_9:
	case XE::RenderPrimitiveTopologyType::PATCHES_10:
	case XE::RenderPrimitiveTopologyType::PATCHES_11:
	case XE::RenderPrimitiveTopologyType::PATCHES_12:
	case XE::RenderPrimitiveTopologyType::PATCHES_13:
	case XE::RenderPrimitiveTopologyType::PATCHES_14:
	case XE::RenderPrimitiveTopologyType::PATCHES_15:
	case XE::RenderPrimitiveTopologyType::PATCHES_16:
	case XE::RenderPrimitiveTopologyType::PATCHES_17:
	case XE::RenderPrimitiveTopologyType::PATCHES_18:
	case XE::RenderPrimitiveTopologyType::PATCHES_19:
	case XE::RenderPrimitiveTopologyType::PATCHES_20:
	case XE::RenderPrimitiveTopologyType::PATCHES_21:
	case XE::RenderPrimitiveTopologyType::PATCHES_22:
	case XE::RenderPrimitiveTopologyType::PATCHES_23:
	case XE::RenderPrimitiveTopologyType::PATCHES_24:
	case XE::RenderPrimitiveTopologyType::PATCHES_25:
	case XE::RenderPrimitiveTopologyType::PATCHES_26:
	case XE::RenderPrimitiveTopologyType::PATCHES_27:
	case XE::RenderPrimitiveTopologyType::PATCHES_28:
	case XE::RenderPrimitiveTopologyType::PATCHES_29:
	case XE::RenderPrimitiveTopologyType::PATCHES_30:
	case XE::RenderPrimitiveTopologyType::PATCHES_31:
	case XE::RenderPrimitiveTopologyType::PATCHES_32:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	default:
		break;
	}

	return {};
}

D3D12_FILTER To( XE::RenderSamplerFilterType min, XE::RenderSamplerFilterType mag, XE::RenderSamplerFilterType mip )
{
	switch( min )
	{
	case XE::RenderSamplerFilterType::NEAREST:
		switch( mag )
		{
		case XE::RenderSamplerFilterType::NEAREST:
			switch( mip )
			{
			case XE::RenderSamplerFilterType::NEAREST:
				return D3D12_FILTER_MIN_MAG_MIP_POINT;
			case XE::RenderSamplerFilterType::LINEAR:
				return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				break;
			default:
				break;
			}
			break;
		case XE::RenderSamplerFilterType::LINEAR:
			switch( mip )
			{
			case XE::RenderSamplerFilterType::NEAREST:
				return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case XE::RenderSamplerFilterType::LINEAR:
				return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case XE::RenderSamplerFilterType::LINEAR:
		switch( mag )
		{
		case XE::RenderSamplerFilterType::NEAREST:
			switch( mip )
			{
			case XE::RenderSamplerFilterType::NEAREST:
				return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case XE::RenderSamplerFilterType::LINEAR:
				return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			default:
				break;
			}
			break;
		case XE::RenderSamplerFilterType::LINEAR:
			switch( mip )
			{
			case XE::RenderSamplerFilterType::NEAREST:
				return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case XE::RenderSamplerFilterType::LINEAR:
				return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return {};
}

class D3D12Resource
{
public:
	D3D12Resource() = default;

	D3D12Resource( std::nullptr_t )
	{
	}

	D3D12Resource( D3D12Resource && val ) noexcept
	{
		std::swap( _Resource, val._Resource );
		std::swap( _Allocation, val._Allocation );
	}

	D3D12Resource( const D3D12Resource & val ) = default;

	~D3D12Resource()
	{
		Release();
	}

public:
	operator ID3D12Resource * ( ) const
	{
		return _Resource;
	}

	ID3D12Resource * operator ->() const
	{
		return _Resource;
	}

	ID3D12Resource ** operator &() const
	{
		return &_Resource;
	}

	D3D12Resource & operator=( std::nullptr_t )
	{
		Release();

		return *this;
	}

	D3D12Resource & operator=( ID3D12Resource * val )
	{
		Release();

		_Resource = val;

		return *this;
	}

	D3D12Resource & operator=( D3D12Resource && val ) noexcept
	{
		std::swap( _Resource, val._Resource );
		std::swap( _Allocation, val._Allocation );

		return *this;
	}

public:
	void Release() const
	{
		if( _Resource )
		{
			_Resource->Release();
			_Resource = nullptr;
		}
		if( _Allocation )
		{
			_Allocation->Release();
			_Allocation = nullptr;
		}
	}

public:
	mutable ID3D12Resource * _Resource = nullptr;
	mutable D3D12MA::Allocation * _Allocation = nullptr;
};
template< XE::uint64 MAX_COUNT > class D3D12DescriptorPage
{
public:
	using value_type = CD3DX12_CPU_DESCRIPTOR_HANDLE;
	using allocate_type = CD3DX12_CPU_DESCRIPTOR_HANDLE;

public:
	D3D12DescriptorPage() = default;

	D3D12DescriptorPage( ID3D12Device4 * device, D3D12_DESCRIPTOR_HEAP_TYPE type )
		:_Device( device ), _Count( 0 )
	{
		_Device->AddRef();

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = MAX_COUNT;
		desc.Type = type;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		_Size = _Device->GetDescriptorHandleIncrementSize( type );

		_Device->CreateDescriptorHeap( &desc, IID_PPV_ARGS( &_Heap ) );

		_Start = _Heap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12DescriptorPage( D3D12DescriptorPage && val ) noexcept
	{
		std::swap( _Size, val._Size );
		std::swap( _Heap, val._Heap );
		std::swap( _Count, val._Count );
		std::swap( _Start, val._Start );
		std::swap( _Device, val._Device );
	}

	D3D12DescriptorPage( const D3D12DescriptorPage & ) = delete;

	~D3D12DescriptorPage()
	{
		if( _Heap )
		{
			_Heap->Release();
		}
		if( _Device )
		{
			_Device->Release();
		}
	}

public:
	D3D12DescriptorPage & operator =( D3D12DescriptorPage && val )
	{
		std::swap( _Size, val._Size );
		std::swap( _Heap, val._Heap );
		std::swap( _Count, val._Count );
		std::swap( _Start, val._Start );
		std::swap( _Device, val._Device );

		return *this;
	}

public:
	bool IsEmpty() const
	{
		return _Count == 0;
	}

	bool HasAllocate( XE::uint64 val ) const
	{
		return _Count < MAX_COUNT;
	}

	bool HasDeallocate( const allocate_type & val ) const
	{
		return val.ptr >= _Start.ptr && val.ptr < _Start.ptr + ( MAX_COUNT * _Size );
	}

public:
	allocate_type Allocate( XE::uint64 val )
	{
		++_Count;
		return { _Start, (INT)_Allocator.Alloc().GetValue(), (UINT)_Size };
	}

	void Deallocate( const allocate_type & val )
	{
		_Allocator.Free( XE::HandleCast< int >( ( val.ptr - _Start.ptr ) / _Size ) );
		--_Count;
	}

private:
	XE::uint64 _Size = 0;
	XE::uint64 _Count = 0;
	ID3D12Device4 * _Device = nullptr;
	ID3D12DescriptorHeap * _Heap = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _Start = {};

	XE::ConcurrentHandleAllocator< XE::Handle< int >, MAX_COUNT > _Allocator;
};

class D3D12RenderPass : public XE::RefCount
{
public:
	XE::RenderPassDesc _Desc;
	XE::RenderPassHandle _Handle;

	XE::RenderTargetHandle _DepthStencil;
	XE::Array< XE::RenderTargetHandle > _Colors;
	D3D12_CPU_DESCRIPTOR_HANDLE _DepthStencilCPUHandle;
	XE::Array< D3D12_CPU_DESCRIPTOR_HANDLE > _ColorCPUHandles;

// 	D3D12_RENDER_PASS_DEPTH_STENCIL_DESC _DepthStencil;
// 	XE::Array< D3D12_RENDER_PASS_RENDER_TARGET_DESC > _RenderTargets;
};
class D3D12RenderQuery : public XE::RefCount
{
public:
	XE::RenderQueryDesc _Desc;
	XE::RenderQueryHandle _Handle;

	XE::uint32 _QueryStride = 0;
	XE::uint32 _QueryPreType = 0;
	D3D12Resource _QueryResults = nullptr;
	ID3D12QueryHeap * _QueryHeap = nullptr;
};
class D3D12RenderBuffer : public XE::RefCount
{
public:
	XE::RenderBufferDesc _Desc;
	XE::RenderBufferHandle _Handle;

	D3D12Resource _Resource;
	XE::RenderResourceStatesType _CurrentState;
};
class D3D12RenderTarget : public XE::RefCount
{
public:
	XE::RenderTargetDesc _Desc;
	XE::RenderTargetHandle _Handle;

	CD3DX12_CPU_DESCRIPTOR_HANDLE _CPUHandle;
};
class D3D12RenderShader : public XE::RefCount
{
public:
	XE::RenderShaderDesc _Desc;
	XE::RenderShaderHandle _Handle;
	
	ID3DBlob * _Blob = nullptr;
};
class D3D12RenderTexture : public XE::RefCount
{
public:
	XE::RenderTextureDesc _Desc;
	XE::RenderTextureHandle _Handle;

	D3D12Resource _Resource;
	XE::RenderResourceStatesType _CurrentState;
};
class D3D12RenderSampler : public XE::RefCount
{
public:
	XE::RenderSamplerDesc _Desc;
	XE::RenderSamplerHandle _Handle;

	CD3DX12_CPU_DESCRIPTOR_HANDLE _CPUHandle;
};
class D3D12RenderDescriptorSet : public XE::RefCount
{
public:
	XE::RenderDescriptorSetDesc _Desc;
	XE::RenderDescriptorSetHandle _Handle;

	ID3DBlob * _RootSignatureBlob = nullptr;
	ID3DBlob * _RootSignatureErrBlob = nullptr;
	ID3D12RootSignature * _RootSignature = nullptr;

	ID3D12DescriptorHeap * _RTVHeap = nullptr;
	ID3D12DescriptorHeap * _DSVHeap = nullptr;
	ID3D12DescriptorHeap * _SamplerHeap = nullptr;
	ID3D12DescriptorHeap * _CBVSRVUAVHeap = nullptr;
};
class D3D12RenderCommandBuffer : public XE::RefCount
{
public:
	XE::RenderCommandBufferHandle _Handle;

	ID3D12GraphicsCommandList4 * _CommandList = nullptr;
	ID3D12CommandAllocator * _CommandAllocator = nullptr;
	XE::Deque< XE::Delegate< void() > > _WaitCallbacks;
};
class D3D12RenderCommandBundle : public XE::RefCount
{
public:
	XE::RenderCommandBundleDesc _Desc;
	XE::RenderCommandBundleHandle _Handle;

	ID3D12GraphicsCommandList4 * _CommandList = nullptr;
	ID3D12CommandAllocator * _CommandAllocator = nullptr;
};
class D3D12RenderShaderProgram : public XE::RefCount
{
public:
	XE::RenderShaderProgramDesc _Desc;
	XE::RenderShaderProgramHandle _Handle;
};
class D3D12RenderPipelineState : public XE::RefCount
{
public:
	XE::RenderPipelineStateHandle _Handle;
	XE::RenderGraphicsPipelineStateDesc _Desc;

	ID3DBlob * _Blob = nullptr;
	ID3D12PipelineState * _PipelineState = nullptr;
};
class D3D12RenderVirtualBuffer : public XE::RefCount
{
public:
	XE::RenderVirtualBufferDesc _Desc;
	XE::RenderVirtualBufferHandle _Handle;

	XE::uint64 _Address;
};

class D3D12RenderContext
{
public:
	XE::RenderContextDesc _Desc;

	XE::Disposable _WindowListener;

	D3D_FEATURE_LEVEL _FeatureLevel;
	D3D12_FEATURE_DATA_D3D12_OPTIONS _Options;
	D3D12_FEATURE_DATA_ARCHITECTURE _Architecture;
	D3D12_FEATURE_DATA_ROOT_SIGNATURE _RootSignature;
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS _QualityLevels;

	ID3D12Device4 * _Device = nullptr;
	IDXGIAdapter1 * _Adapter = nullptr;
	ID3D12CommandQueue * _CommandQueue = nullptr;
	tbb::concurrent_queue< XE::Delegate< void() > > _WaitCallbacks;

	D3D12MA::Allocator * _HeapAllocator = nullptr;
	D3D12MA::VirtualBlock * _VirtualAllocator = nullptr;

	XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_RTV_DESC_HEAP_CACHE > > _RTVDescriptorAllocator;
	XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_DSV_DESC_HEAP_CACHE > > _DSVDescriptorAllocator;
	XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_SAMPLER_DESC_HEAP_CACHE > > _SamplerDescriptorAllocator;
	XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_CBV_SRV_UAV_DESC_HEAP_CACHE > > _CBVSRVUAVDescriptorAllocator;

	HANDLE _FenceEvent = nullptr;
	ID3D12Fence * _Fence = nullptr;
	std::atomic<XE::uint64> _FenceValue = 1;
	tbb::concurrent_queue< XE::RenderCommandBufferHandle > _CloseCommandList;

	std::array< D3D12RenderPass, XE::RENDER_MAX_PASS > _RenderPasses;
	std::array< D3D12RenderQuery, XE::RENDER_MAX_QUERY > _RenderQuerys;
	std::array< D3D12RenderBuffer, XE::RENDER_MAX_BUFFER > _RenderBuffers;
	std::array< D3D12RenderTarget, XE::RENDER_MAX_TARGET > _RenderTargets;
	std::array< D3D12RenderShader, XE::RENDER_MAX_SHADER > _RenderShaders;
	std::array< D3D12RenderTexture, XE::RENDER_MAX_TEXTURE > _RenderTextures;
	std::array< D3D12RenderSampler, XE::RENDER_MAX_SAMPLER > _RenderSamplers;
	std::array< D3D12RenderDescriptorSet, XE::RENDER_MAX_DESCRIPTOR_SET > _RenderDescriptorSets;
	std::array< D3D12RenderCommandBuffer, XE::RENDER_MAX_COMMAND_BUFFER > _RenderCommandBuffers;
	std::array< D3D12RenderCommandBundle, XE::RENDER_MAX_COMMAND_BUNDLE > _RenderCommandBundles;
	std::array< D3D12RenderPipelineState, XE::RENDER_MAX_PIPELINE_STATE > _RenderPipelineStates;
	std::array< D3D12RenderShaderProgram, XE::RENDER_MAX_SHADER_PROGRAM > _RenderShaderPrograms;
	std::array< D3D12RenderVirtualBuffer, XE::RENDER_MAX_VIRTUAL_BUFFER > _RenderVirtualBuffers;

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

}

IMPLEMENT_META( XE::RenderService );

struct XE::RenderService::Private
{
	ID3D12Debug * _Debug = nullptr;
	IDXGIFactory7 * _Factory = nullptr;
	bool _IsPresentAllowTearing = false;

	std::mutex _SwapChainMutex;
	IDXGISwapChain3 * _SwapChain = nullptr;
	XE::uint64 _SwapChainBackBufferIndex = 0;

	XE::int32 _MainContext = -1;
	XE::Array< std::unique_ptr< D3D12RenderContext > > _Contexts;
};

XE::RenderService::RenderService()
	:_p( XE::New< Private >() )
{

}

XE::RenderService::~RenderService()
{
	XE::Delete( _p );
}

void XE::RenderService::Prepare()
{
	// factory
	{
		XE::uint32 flags = 0;

		if( GetFramework()->GetBool( XE::CONFIG_RENDER_DEBUG ) )
		{
			if( SUCCEEDED( ::D3D12GetDebugInterface( IID_PPV_ARGS( &_p->_Debug ) ) ) )
			{
				_p->_Debug->EnableDebugLayer();
				flags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}

		auto hr = ::CreateDXGIFactory2( flags, IID_PPV_ARGS( &_p->_Factory ) );
		XE_ASSERT( FAILED( hr ) && "create dxgi factory error" );

		BOOL allow_tearing = FALSE;
		if( FAILED( _p->_Factory->CheckFeatureSupport( DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof( allow_tearing ) ) ) )
		{
			allow_tearing = FALSE;
		}
		_p->_IsPresentAllowTearing = allow_tearing;
	}

	// context
	{
		IDXGIAdapter1 * adapter = nullptr;
		for( size_t i = 0; _p->_Factory->EnumAdapters1( i, &adapter ) != DXGI_ERROR_NOT_FOUND; i++ )
		{
			DXGI_ADAPTER_DESC1 adapter_desc;
			adapter->GetDesc1( &adapter_desc );

			IDXGIOutput * output = nullptr;
			for( size_t j = 0; adapter->EnumOutputs( j, &output ) != DXGI_ERROR_NOT_FOUND; j++ )
			{
				DXGI_OUTPUT_DESC output_desc;
				output->GetDesc( &output_desc );

				XE_MESSAGE( R"()");

				UINT numModes = 0;
				output->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr );
				XE::Array< DXGI_MODE_DESC > mode_descs( numModes, XE::MemoryResource::GetFrameMemoryResource() );
				auto hr = output->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, mode_descs.data() );
				XE_ASSERT( FAILED( hr ) && "failed to get display mode list with format DXGI_FORMAT_R8G8B8A8_UNORM" );

				XE::String modes;
				for( const auto & mode : mode_descs )
				{
					modes += XE::Format( "{%0}x{%1}-{%2}\r", mode.Width, mode.Height, ( mode.RefreshRate.Denominator > 0 ? mode.RefreshRate.Numerator / mode.RefreshRate.Denominator : 0 ) );
				}

				XE_MESSAGE( R"(
--------------------------------------------------------------------------------------
adapter name:			{%0}
adapter vendor id:		{%1}
adapter device id:		{%2}
adapter video memory:	{%3}
adapter system memory:	{%4}
adapter shared memory:	{%5}
	output name:		{%6}
	output width:		{%7}
	output height:		{%8}
		modes:			{%9}
--------------------------------------------------------------------------------------)",
adapter_desc.Description, VendorIDToStr( adapter_desc.VendorId ), adapter_desc.DeviceId, adapter_desc.DedicatedVideoMemory, adapter_desc.DedicatedSystemMemory, adapter_desc.SharedSystemMemory,
output_desc.DeviceName, output_desc.DesktopCoordinates.right - output_desc.DesktopCoordinates.left, output_desc.DesktopCoordinates.top - output_desc.DesktopCoordinates.bottom,
modes );

				output->Release();
				output = nullptr;
			}

			D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_12_1 , D3D_FEATURE_LEVEL_12_0 };
			for( auto level : levels )
			{
				ID3D12Device4 * device = nullptr;
				auto hr = D3D12CreateDevice( adapter, level, IID_PPV_ARGS( &device ) );
				if( SUCCEEDED( hr ) )
				{
					if( _p->_MainContext == -1 && ( adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE ) == 0 )
					{
						_p->_MainContext = _p->_Contexts.size();
					}
					
					XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_RTV_DESC_HEAP_CACHE > > rtv_allocator( [device]()
					{
						return D3D12DescriptorPage< XE::RENDER_MAX_RTV_DESC_HEAP_CACHE >( device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
					}, nullptr, XE::MemoryResource::GetDefaultMemoryResource() );
					XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_DSV_DESC_HEAP_CACHE > > dsv_allocator( [device]()
					{
						return D3D12DescriptorPage< XE::RENDER_MAX_DSV_DESC_HEAP_CACHE >( device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV );
					}, nullptr, XE::MemoryResource::GetDefaultMemoryResource() );
					XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_SAMPLER_DESC_HEAP_CACHE > > sampler_allocator( [device]()
					{
						return D3D12DescriptorPage< XE::RENDER_MAX_SAMPLER_DESC_HEAP_CACHE >( device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );
					}, nullptr, XE::MemoryResource::GetDefaultMemoryResource() );
					XE::PageAllocator < D3D12DescriptorPage< XE::RENDER_MAX_CBV_SRV_UAV_DESC_HEAP_CACHE > > cbvsrvuav_allocator( [device]()
					{
						return D3D12DescriptorPage< XE::RENDER_MAX_CBV_SRV_UAV_DESC_HEAP_CACHE >( device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
					}, nullptr, XE::MemoryResource::GetDefaultMemoryResource() );

					auto context = std::make_unique< D3D12RenderContext >();
					
					D3D12MA::ALLOCATOR_DESC allocator_desc;
					allocator_desc.pDevice = device;
					allocator_desc.pAdapter = adapter;
					allocator_desc.PreferredBlockSize = 0;
					allocator_desc.pAllocationCallbacks = nullptr;

					D3D12MA::VIRTUAL_BLOCK_DESC virtual_desc;
					virtual_desc.Size = MBYTE( 512 );
					virtual_desc.pAllocationCallbacks = nullptr;

					device->CheckFeatureSupport( D3D12_FEATURE_D3D12_OPTIONS, &context->_Options, sizeof( D3D12_FEATURE_DATA_D3D12_OPTIONS ) );
					device->CheckFeatureSupport( D3D12_FEATURE_ARCHITECTURE, &context->_Architecture, sizeof( D3D12_FEATURE_DATA_ARCHITECTURE ) );
					device->CheckFeatureSupport( D3D12_FEATURE_ROOT_SIGNATURE, &context->_RootSignature, sizeof( D3D12_FEATURE_DATA_ROOT_SIGNATURE ) );
					device->CheckFeatureSupport( D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &context->_QualityLevels, sizeof( D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ) );
					
					context->_Desc.Name = XE::StringConvert< XE::WideEncode, XE::AnsiEncode >::convert( adapter_desc.Description );
					context->_Desc.Vendor = adapter_desc.VendorId;
					context->_Desc.Device = adapter_desc.DeviceId;
					context->_Desc.VideoMemory = adapter_desc.DedicatedVideoMemory;
					context->_Desc.SystemMemory = adapter_desc.DedicatedSystemMemory;
					context->_Desc.SharedMemory = adapter_desc.SharedSystemMemory;
					context->_Device = device;
					context->_Adapter = adapter;
					context->_FeatureLevel = level;
					D3D12MA::CreateAllocator( &allocator_desc, &context->_HeapAllocator );
					D3D12MA::CreateVirtualBlock( &virtual_desc, &context->_VirtualAllocator );
					context->_RTVDescriptorAllocator = std::move( rtv_allocator );
					context->_DSVDescriptorAllocator = std::move( dsv_allocator );
					context->_SamplerDescriptorAllocator = std::move( sampler_allocator );
					context->_CBVSRVUAVDescriptorAllocator = std::move( cbvsrvuav_allocator );

					_p->_Contexts.emplace_back( std::move( context ) );

					break;
				}

				device = nullptr;
			}

			adapter = nullptr;
		}
	}

	// main context default 0
	if( _p->_MainContext == -1 )
	{
		_p->_MainContext = 0;
	}

	// command queue
	{
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		for( auto & it : _p->_Contexts )
		{
			auto hr = it->_Device->CreateCommandQueue( &desc, IID_PPV_ARGS( &it->_CommandQueue ) );
			XE_ASSERT( FAILED( hr ) && "create command queue error" );
		}
	}

	// fence
	{
		for( auto & it : _p->_Contexts )
		{
			auto hr = it->_Device->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &it->_Fence ) );
			XE_ASSERT( FAILED( hr ) && "create fence error" );
			it->_FenceEvent = ::CreateEvent( nullptr, false, false, nullptr );
		}
	}

	// swap chain
	{
		DXGI_SWAP_CHAIN_DESC1 desc;

		auto window = GetFramework()->GetMainWindow();
		auto size = window->GetScreenSize();

		desc.Width = size.first;
		desc.Height = size.second;
		desc.Format = To( XE::RenderFormatType::R8G8B8A8_UNORM );
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SampleDesc.Count = std::min( _p->_Contexts[_p->_MainContext]->_QualityLevels.SampleCount, (UINT)4 );
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = RENDER_MAX_SWAP_CHAIN_BACK_BUFFER;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen;

		fullscreen.RefreshRate.Denominator = 0;
		fullscreen.RefreshRate.Numerator = 1;
		fullscreen.Scaling = DXGI_MODE_SCALING_STRETCHED;
		fullscreen.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fullscreen.Windowed = !window->IsFullscreen();

		IDXGISwapChain1 * swapchain = nullptr;
		auto hr = _p->_Factory->CreateSwapChainForHwnd(
			_p->_Contexts[_p->_MainContext]->_CommandQueue,
			(HWND)window->GetHandle().GetValue(),
			&desc,
			&fullscreen,
			nullptr,
			&swapchain );
		XE_ASSERT( FAILED( hr ) && "create swap chain error" );

		swapchain->QueryInterface( &_p->_SwapChain );
		_p->_SwapChainBackBufferIndex = _p->_SwapChain->GetCurrentBackBufferIndex();

		swapchain->Release();
	}

	// swap chain back buffer rtv
	{
		for( UINT i = 0; i < XE::RENDER_MAX_SWAP_CHAIN_BACK_BUFFER; i++ )
		{
			ID3D12Resource * resource = nullptr;

			_p->_SwapChain->GetBuffer( i, IID_PPV_ARGS( &resource ) );

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_cpu_handle = _p->_Contexts[_p->_MainContext]->_RTVDescriptorAllocator.Allocate();

			_p->_Contexts[_p->_MainContext]->_Device->CreateRenderTargetView( resource, nullptr, rtv_cpu_handle );

			D3D12_RESOURCE_DESC rt_desc = resource->GetDesc();
			auto rt_handle = _p->_Contexts[_p->_MainContext]->_RenderTargetHandleAllocator.Alloc();
			auto tex_handle = _p->_Contexts[_p->_MainContext]->_RenderTextureHandleAllocator.Alloc();
			{
				auto & texture = _p->_Contexts[_p->_MainContext]->_RenderTextures[tex_handle.GetValue()];

				texture.Inc();
				texture._Handle = tex_handle;
				texture._Resource = resource;
				texture._CurrentState = XE::RenderResourceStatesType::PRESENT;

				texture._Desc.Name = "MainRenderTexture_" + std::to_string( i );
				texture._Desc.Type = XE::RenderTextureType::TEXTURE_2D;
				texture._Desc.BindFlags = XE::RenderBindFlag::COLOR_ATTACHMENT;
				texture._Desc.Format = To( rt_desc.Format );
				texture._Desc.Width = rt_desc.Width;
				texture._Desc.Height = rt_desc.Height;
				texture._Desc.Depth = rt_desc.DepthOrArraySize;
				texture._Desc.MipLevels = 0;
				texture._Desc.Clear.Color = XE::FColor::Black;
				texture._Desc.Clear.Flags = XE::RenderClearFlag::COLOR;

				auto & rendertarget = _p->_Contexts[_p->_MainContext]->_RenderTargets[rt_handle.GetValue()];

				rendertarget.Inc();
				rendertarget._Handle = rt_handle;
				rendertarget._CPUHandle = rtv_cpu_handle;

				rendertarget._Desc.Name = "MainRenderTarget_" + std::to_string( i );
				rendertarget._Desc.Resolution.x = rt_desc.Width;
				rendertarget._Desc.Resolution.y = rt_desc.Height;
				rendertarget._Desc.Type = RenderAttachmentType::COLOR;
				rendertarget._Desc.Texture = tex_handle; texture.Inc();
			}
		}
	}
}

bool XE::RenderService::Startup()
{
	_p->_Contexts[_p->_MainContext]->_WindowListener = GetFramework()->GetServiceT< XE::EventService >()->RegisterListener( XE::EVENT_WINDOW, [this]( const XE::EventPtr & evt )
	{
		XE::WindowEventType type = evt->parameter.Value< XE::WindowEventType >();
		if( type == XE::WindowEventType::SIZE )
		{
			_p->_Contexts[_p->_MainContext]->_WaitCallbacks.push( XE::Delegate< void() >( &XE::RenderService::OnMainWindowResize, this ) );
		}
	} );

	return true;
}

void XE::RenderService::Update()
{
	Execute( GetMainContextHandle() );

	_p->_SwapChainMutex.lock();

	GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( XE::ThreadType::RENDER, [this]()
	{
		XE::TaskGuard guard( [this]()
		{
			_p->_SwapChainMutex.unlock();
		} );

		bool vsync = GetFramework()->GetBool( XE::CONFIG_RENDER_VSYNC );
		XE::uint32 sync_interval = vsync ? 1 : 0;
		XE::uint32 flags = _p->_IsPresentAllowTearing && !vsync ? DXGI_PRESENT_ALLOW_TEARING : 0;

		auto hr = _p->_SwapChain->Present( sync_interval, flags );
		XE_ASSERT( FAILED( hr ) && "swap chain paresent() error" );

		Wait( GetMainContextHandle() );
	} );

	_p->_SwapChainBackBufferIndex = ( _p->_SwapChainBackBufferIndex + 1 ) % XE::RENDER_MAX_SWAP_CHAIN_BACK_BUFFER;
}

void XE::RenderService::Clearup()
{
	for( size_t i = 0; i < _p->_Contexts.size(); i++ )
	{
		Execute( XE::HandleCast< XE::RenderContext >( i ) );
		Wait( XE::HandleCast< XE::RenderContext >( i ) );

		// TODO: Reset Render Context
	}

	_p->_Contexts.clear(); _p->_Contexts.shrink_to_fit();

	_p->_SwapChain->Release(); _p->_SwapChain = nullptr;

	_p->_Factory->Release(); _p->_Factory = nullptr;

	_p->_Debug->Release(); _p->_Debug = nullptr;
}

XE::uint32 XE::RenderService::GetContextCount() const
{
	return _p->_Contexts.size();
}

XE::RenderContextHandle XE::RenderService::GetMainContextHandle() const
{
	return XE::HandleCast< XE::RenderContext >( _p->_MainContext );
}

XE::RenderContextHandle XE::RenderService::GetContextHandle( XE::uint32 val ) const
{
	return XE::HandleCast< XE::RenderContext >( val );
}

const XE::RenderContextDesc & XE::RenderService::GetContextDesc( XE::uint32 val ) const
{
	return _p->_Contexts[val]->_Desc;
}

XE::RenderApiType XE::RenderService::GetApiType() const
{
	return XE::RenderApiType::D3D12;
}

XE::RenderTargetHandle XE::RenderService::GetMainWindowRenderTarget() const
{
	return XE::HandleCast< XE::RenderTarget >( _p->_SwapChainBackBufferIndex );
}

XE::RenderTargetHandle XE::RenderService::GetMainWindowBackRenderTarget() const
{
	return XE::HandleCast< XE::RenderTarget >( ( _p->_SwapChainBackBufferIndex + 1 ) % XE::RENDER_MAX_SWAP_CHAIN_BACK_BUFFER );
}

#define FIND_FROM_NAME( NAME ) \
	auto it = std::find_if( ctx->_##NAME##s.begin(), ctx->_##NAME##s.end(), [&]( const D3D12##NAME & val ) { return val._Desc.Name == desc.Name; } ); \
	if( it != ctx->_##NAME##s.end() ) \
	{ \
		it->Inc(); \
		return it->_Handle; \
	}

XE::RenderQueryHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderQueryDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderQuery );

	auto handle = ctx->_RenderQueryHandleAllocator.Alloc();
	if( handle )
	{
		auto & query = ctx->_RenderQuerys[handle.GetValue()];

		query.Inc();
		query._Desc = desc;
		query._Handle = handle;

		query._QueryPreType = desc.Type == RenderQueryType::TIME_ELAPSED ? 2 : 1;

		D3D12_QUERY_HEAP_DESC heap_desc;
		heap_desc.Type = To( desc.Type, {} );
		heap_desc.Count = desc.NumQueries * query._QueryPreType;
		heap_desc.NodeMask = 0;

		ctx->_Device->CreateQueryHeap( &heap_desc, IID_PPV_ARGS( &query._QueryHeap ) );

		query._QueryStride = To( desc.Type ) == D3D12_QUERY_TYPE_PIPELINE_STATISTICS ? sizeof( D3D12_QUERY_DATA_PIPELINE_STATISTICS ) : sizeof( UINT64 );

		D3D12MA::ALLOCATION_DESC aoc_desc;
		aoc_desc.HeapType = desc.RenderCondition ? D3D12_HEAP_TYPE_DEFAULT : D3D12_HEAP_TYPE_READBACK;
		D3D12_RESOURCE_DESC res_desc = CD3DX12_RESOURCE_DESC::Buffer( (XE::uint64)heap_desc.Count * query._QueryStride );

		ctx->_HeapAllocator->CreateResource( 
			&aoc_desc, 
			&res_desc, 
			desc.RenderCondition ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			&query._QueryResults._Allocation,
			IID_PPV_ARGS( &query._QueryResults ) );
	}
	return handle;
}

XE::RenderBufferHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderBufferDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderBuffer );

	auto handle = ctx->_RenderBufferHandleAllocator.Alloc();
	if( handle )
	{
		auto & buffer = ctx->_RenderBuffers[handle.GetValue()];

		buffer.Inc();
		buffer._Desc = desc;
		buffer._Handle = handle;
		buffer._CurrentState = XE::RenderResourceStatesType::COMMON;

		D3D12MA::ALLOCATION_DESC aoc_desc;
		aoc_desc.HeapType = To( desc.CpuAccess );
		CD3DX12_RESOURCE_DESC res_desc = CD3DX12_RESOURCE_DESC::Buffer( desc.Size * desc.Stride );
		D3D12_CLEAR_VALUE clear;
		clear.Format = To( desc.Format );
		clear.Color[0] = 0; clear.Color[1] = 0; clear.Color[1] = 0; clear.Color[1] = 0;

		ctx->_HeapAllocator->CreateResource( &aoc_desc, &res_desc, To( desc.BindFlags ), &clear, &buffer._Resource._Allocation, IID_PPV_ARGS( &buffer._Resource ) );
	}
	return handle;
}

XE::RenderTargetHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderTargetDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderTarget );

	auto handle = ctx->_RenderTargetHandleAllocator.Alloc();
	if( handle )
	{
		auto & rendertarget = ctx->_RenderTargets[handle.GetValue()];

		rendertarget.Inc();
		rendertarget._Desc = desc;
		rendertarget._Handle = handle;

		auto & texture = ctx->_RenderTextures[desc.Texture.GetValue()];

		texture.Inc();

		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc;
		rtv_desc.Format = To( texture._Desc.Format );
		switch( texture._Desc.Type )
		{
		case XE::RenderTextureType::TEXTURE_1D:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			rtv_desc.Texture1D.MipSlice = texture._Desc.MipLevels;
			break;
		case XE::RenderTextureType::TEXTURE_1D_ARRAY:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
			rtv_desc.Texture1DArray.MipSlice = texture._Desc.MipLevels;
			rtv_desc.Texture1DArray.FirstArraySlice = 1;
			rtv_desc.Texture1DArray.ArraySize = texture._Desc.Depth;
			break;
		case XE::RenderTextureType::TEXTURE_2D:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtv_desc.Texture2D.MipSlice = texture._Desc.MipLevels;
			rtv_desc.Texture2D.PlaneSlice = 1;
			break;
		case XE::RenderTextureType::TEXTURE_2D_MS:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
			rtv_desc.Texture2DMS.UnusedField_NothingToDefine = 1;
			break;
		case XE::RenderTextureType::TEXTURE_2D_ARRAY:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			rtv_desc.Texture2DArray.MipSlice = texture._Desc.MipLevels;
			rtv_desc.Texture2DArray.FirstArraySlice = 1;
			rtv_desc.Texture2DArray.ArraySize = texture._Desc.Depth;
			rtv_desc.Texture2D.PlaneSlice = 1;
			break;
		case XE::RenderTextureType::TEXTURE_3D:
		case XE::RenderTextureType::TEXTURE_CUBE:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			rtv_desc.Texture3D.FirstWSlice = 1;
			rtv_desc.Texture3D.WSize = texture._Desc.Depth;
			rtv_desc.Texture3D.MipSlice = texture._Desc.MipLevels;
			break;
		case XE::RenderTextureType::TEXTURE_2D_MS_ARRAY:
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
			rtv_desc.Texture2DMSArray.ArraySize = texture._Desc.Depth;
			rtv_desc.Texture2DMSArray.FirstArraySlice = 1;
			break;
		default:
			break;
		}

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_cpu_handle = ctx->_RTVDescriptorAllocator.Allocate();
		ctx->_Device->CreateRenderTargetView( texture._Resource, &rtv_desc, rtv_cpu_handle );
		rendertarget._CPUHandle = rtv_cpu_handle;
	}
	return handle;
}

XE::RenderShaderHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderShaderDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderShader );

	ID3DBlob * blob = nullptr;
	XE::uint64 hash = XE::Hash( desc.Source.c_str() );

	// 缓存
	{
		auto view = GetFramework()->GetServiceT< XE::CacheService >()->FindCache( hash );
		if( view.data() != nullptr )
		{
			::D3DCreateBlob( view.size(), &blob );

			std::copy( view.data(), view.data() + view.size(), (XE::int8 *)blob->GetBufferPointer() );
		}
	}

	// 编译
	if( blob == nullptr )
	{
		XE::uint32 flags = GetFramework()->GetBool( XE::CONFIG_RENDER_DEBUG ) ? D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION : 0;
		auto hr = D3DCompile( desc.Source.data(), desc.Source.size(), desc.Name.c_str(), macros, nullptr, desc.Enter.c_str(), shaders[(XE::uint64)desc.Type], flags, 0, &blob, nullptr );
		if( FAILED( hr ) )
		{
			XE_ERROR( "shader compile error: {%0}", desc.Name );
			return {};
		}
		
		// 添加缓存
		GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( XE::ThreadType::IO, [this, hash, blob]()
		{
			GetFramework()->GetServiceT< XE::CacheService >()->InsertCache( hash, { (XE::int8 *)blob->GetBufferPointer(), blob->GetBufferSize() } );
		} );
	}

	auto handle = ctx->_RenderShaderHandleAllocator.Alloc();
	if( handle )
	{
		ctx->_RenderShaders[handle.GetValue()].Inc();
		ctx->_RenderShaders[handle.GetValue()]._Blob = blob;
		ctx->_RenderShaders[handle.GetValue()]._Desc = desc;
		ctx->_RenderShaders[handle.GetValue()]._Handle = handle;
	}
	return handle;
}

XE::RenderTextureHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderTextureDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderTexture );

	auto handle = ctx->_RenderTextureHandleAllocator.Alloc();
	if( handle )
	{
		auto & texture = ctx->_RenderTextures[handle.GetValue()];

		texture.Inc();
		texture._Desc = desc;
		texture._Handle = handle;
		texture._CurrentState = XE::RenderResourceStatesType::COMMON;

		D3D12MA::ALLOCATION_DESC aoc_desc = {};
		aoc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
		
		CD3DX12_RESOURCE_DESC res_desc = {};

		switch( desc.Type )
		{
		case XE::RenderTextureType::TEXTURE_1D:
		case XE::RenderTextureType::TEXTURE_1D_ARRAY:
			res_desc = CD3DX12_RESOURCE_DESC::Tex1D( To( desc.Format ), desc.Width, desc.Depth, desc.MipLevels );
			break;
		case XE::RenderTextureType::TEXTURE_2D:
		case XE::RenderTextureType::TEXTURE_2D_MS:
		case XE::RenderTextureType::TEXTURE_2D_ARRAY:
			res_desc = CD3DX12_RESOURCE_DESC::Tex2D( To( desc.Format ), desc.Width, desc.Height, desc.Depth, desc.MipLevels );
			break;
		case XE::RenderTextureType::TEXTURE_3D:
		case XE::RenderTextureType::TEXTURE_CUBE:
		case XE::RenderTextureType::TEXTURE_2D_MS_ARRAY:
			res_desc = CD3DX12_RESOURCE_DESC::Tex3D( To( desc.Format ), desc.Width, desc.Height, desc.Depth, desc.MipLevels );
			break;
		default:
			break;
		}

		CD3DX12_CLEAR_VALUE clear = {};
		clear.Format = To( desc.Format );
		if( desc.Clear.Flags && XE::RenderClearFlag::DEPTH ) { clear.DepthStencil.Depth = desc.Clear.Depth; }
		if( desc.Clear.Flags && XE::RenderClearFlag::STENCIL ) { clear.DepthStencil.Stencil = desc.Clear.Stencil; }
		if( desc.Clear.Flags && XE::RenderClearFlag::COLOR ) { clear.Color[0] = desc.Clear.Color.d[0]; clear.Color[1] = desc.Clear.Color.d[1]; clear.Color[2] = desc.Clear.Color.d[2]; clear.Color[3] = desc.Clear.Color.d[3]; }

		ctx->_HeapAllocator->CreateResource( &aoc_desc, &res_desc, To( desc.BindFlags ), &clear, &texture._Resource._Allocation, IID_PPV_ARGS( &texture._Resource ) );
	}
	return handle;
}

XE::RenderSamplerHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderSamplerDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderSampler );

	auto handle = ctx->_RenderSamplerHandleAllocator.Alloc();
	if( handle )
	{
		auto & sampler = ctx->_RenderSamplers[handle.GetValue()];

		sampler.Inc();
		sampler._Desc = desc;
		sampler._Handle = handle;
		sampler._CPUHandle = ctx->_SamplerDescriptorAllocator.Allocate();

		D3D12_SAMPLER_DESC sam_desc = {};

		sam_desc.Filter = To( desc.MinFilter, desc.MagFilter, desc.MipMapFilter );
		sam_desc.AddressU = To( desc.AddressModeU );
		sam_desc.AddressV = To( desc.AddressModeV );
		sam_desc.AddressW = To( desc.AddressModeW );
		sam_desc.MipLODBias = desc.MipMapLODBias;
		sam_desc.MaxAnisotropy = desc.MaxAnisotropy;
		sam_desc.ComparisonFunc = To( desc.CompareOp );
		std::memcpy( sam_desc.BorderColor, desc.BorderColor.d, sizeof( XE::FColor ) );
		sam_desc.MinLOD = desc.MinLOD;
		sam_desc.MaxLOD = desc.MaxLOD;

		ctx->_Device->CreateSampler( &sam_desc, sampler._CPUHandle );
	}
	return handle;
}

XE::RenderDescriptorSetHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderDescriptorSetDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderDescriptorSet );

	auto handle = ctx->_RenderDescriptorSetHandleAllocator.Alloc();
	if( handle )
	{
		auto & descs = ctx->_RenderDescriptorSets[handle.GetValue()];

		descs.Inc();
		descs._Desc = desc;
		descs._Handle = handle;

		D3D12_ROOT_SIGNATURE_FLAGS signature_flags = D3D12_ROOT_SIGNATURE_FLAG_NONE | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_desc;
		XE::Array< CD3DX12_ROOT_PARAMETER1 > parameters( XE::MemoryResource::GetFrameMemoryResource() );
		XE::Array< CD3DX12_DESCRIPTOR_RANGE1 > uav_ranges( XE::MemoryResource::GetFrameMemoryResource() );
		XE::Array< CD3DX12_DESCRIPTOR_RANGE1 > srv_ranges( XE::MemoryResource::GetFrameMemoryResource() );
		XE::Array< CD3DX12_DESCRIPTOR_RANGE1 > cbv_ranges( XE::MemoryResource::GetFrameMemoryResource() );
		XE::Array< CD3DX12_DESCRIPTOR_RANGE1 > sampler_ranges( XE::MemoryResource::GetFrameMemoryResource() );

		// ranges
		{
			for( const auto & it : desc.Bindings )
			{
				if( ( it.Type == XE::RenderResourceType::SAMPLER ) )
				{
					CD3DX12_DESCRIPTOR_RANGE1 range;
					range.Init( D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, it.ArraySize, it.Slot, 0 );
					sampler_ranges.push_back( range );
				}
				else if( ( it.Type == XE::RenderResourceType::BUFFER ) && ( it.Bind && XE::RenderBindFlag::SAMPLED ) )
				{
					CD3DX12_DESCRIPTOR_RANGE1 range;
					range.Init( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, it.ArraySize, it.Slot, 0 );
					srv_ranges.push_back( range );
				}
				else if( ( it.Type == XE::RenderResourceType::TEXTURE ) && ( it.Bind && XE::RenderBindFlag::SAMPLED ) )
				{
					CD3DX12_DESCRIPTOR_RANGE1 range;
					range.Init( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, it.ArraySize, it.Slot, 0 );
					srv_ranges.push_back( range );
				}
				else if( ( it.Type == XE::RenderResourceType::BUFFER ) && ( it.Bind && XE::RenderBindFlag::STORAGE ) )
				{
					CD3DX12_DESCRIPTOR_RANGE1 range;
					range.Init( D3D12_DESCRIPTOR_RANGE_TYPE_UAV, it.ArraySize, it.Slot, 0 );
					uav_ranges.push_back( range );
				}
				else if( ( it.Type == XE::RenderResourceType::TEXTURE ) && ( it.Bind && XE::RenderBindFlag::STORAGE ) )
				{
					CD3DX12_DESCRIPTOR_RANGE1 range;
					range.Init( D3D12_DESCRIPTOR_RANGE_TYPE_UAV, it.ArraySize, it.Slot, 0 );
					uav_ranges.push_back( range );
				}
				else if( ( it.Type == XE::RenderResourceType::BUFFER ) && ( it.Bind && XE::RenderBindFlag::CONSTANT_BUFFER ) )
				{
					CD3DX12_DESCRIPTOR_RANGE1 range;
					range.Init( D3D12_DESCRIPTOR_RANGE_TYPE_CBV, it.ArraySize, it.Slot, 0 );
					cbv_ranges.push_back( range );
				}

				if( ( it.Stage && XE::RenderStageFlag::VERTEX ) ) { signature_flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS; }
				if( ( it.Stage && XE::RenderStageFlag::TESS_CONTROL ) ) { signature_flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS; }
				if( ( it.Stage && XE::RenderStageFlag::TESS_EVALUATION ) ) { signature_flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS; }
				if( ( it.Stage && XE::RenderStageFlag::GEOMETRY ) ) { signature_flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS; }
				if( ( it.Stage && XE::RenderStageFlag::FRAGMENT ) ) { signature_flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS; }
			}
		}

		// parameters
		{
			CD3DX12_ROOT_PARAMETER1 parameter;

			if( !uav_ranges.empty() )
			{
				parameter.InitAsDescriptorTable( uav_ranges.size(), uav_ranges.data(), D3D12_SHADER_VISIBILITY_ALL );
				parameters.push_back( std::move( parameter ) );
			}
			if( !srv_ranges.empty() )
			{
				parameter.InitAsDescriptorTable( srv_ranges.size(), srv_ranges.data(), D3D12_SHADER_VISIBILITY_ALL );
				parameters.push_back( std::move( parameter ) );
			}
			if( !cbv_ranges.empty() )
			{
				parameter.InitAsDescriptorTable( cbv_ranges.size(), cbv_ranges.data(), D3D12_SHADER_VISIBILITY_ALL );
				parameters.push_back( std::move( parameter ) );
			}
			if( !sampler_ranges.empty() )
			{
				parameter.InitAsDescriptorTable( sampler_ranges.size(), sampler_ranges.data(), D3D12_SHADER_VISIBILITY_ALL );
				parameters.push_back( std::move( parameter ) );
			}
		}

		// root desc
		{
			root_desc.Init_1_1( root_desc, parameters.size(), parameters.data() );
			root_desc.Version = ctx->_RootSignature.HighestVersion;
			D3DX12SerializeVersionedRootSignature( &root_desc,
												   ctx->_RootSignature.HighestVersion,
												   &descs._RootSignatureBlob,
												   &descs._RootSignatureErrBlob );
		}

		ctx->_Device->CreateRootSignature( 0,
										   descs._RootSignatureBlob->GetBufferPointer(),
										   descs._RootSignatureBlob->GetBufferSize(),
										   IID_PPV_ARGS( &descs._RootSignature ) );
	}
	return handle;
}

XE::RenderCommandBundleHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderCommandBundleDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderCommandBundle );

	auto handle = ctx->_RenderCommandBundleHandleAllocator.Alloc();
	if( handle )
	{
		auto & bundle = ctx->_RenderCommandBundles[handle.GetValue()];

		bundle.Inc();
		bundle._Desc = desc;
		bundle._Handle = handle;

		ctx->_Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS( &bundle._CommandAllocator ) );
		ctx->_Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_BUNDLE, bundle._CommandAllocator, nullptr, IID_PPV_ARGS( &bundle._CommandList ) );
		bundle._CommandList->Close();
	}
	return handle;
}

XE::RenderShaderProgramHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderShaderProgramDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderShaderProgram );

	auto handle = ctx->_RenderShaderProgramHandleAllocator.Alloc();
	if( handle )
	{
		auto & program = ctx->_RenderShaderPrograms[handle.GetValue()];

		program.Inc();
		program._Desc = desc;
		program._Handle = handle;

		if( desc.CS ) ctx->_RenderShaders[desc.CS.GetValue()].Inc();
		if( desc.VS ) ctx->_RenderShaders[desc.VS.GetValue()].Inc();
		if( desc.PS ) ctx->_RenderShaders[desc.PS.GetValue()].Inc();
		if( desc.HS ) ctx->_RenderShaders[desc.HS.GetValue()].Inc();
		if( desc.DS ) ctx->_RenderShaders[desc.DS.GetValue()].Inc();
		if( desc.GS ) ctx->_RenderShaders[desc.GS.GetValue()].Inc();
	}
	return handle;
}

XE::RenderPipelineStateHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderPipelineStateDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderPipelineState );

	auto handle = ctx->_RenderPipelineStateHandleAllocator.Alloc();
	if( handle )
	{
		auto & state = ctx->_RenderPipelineStates[handle.GetValue()];
		auto & program = ctx->_RenderShaderPrograms[desc.ShaderProgram.GetValue()];

		state.Inc();

		state._Handle = handle;

		ID3DBlob * blob = nullptr;
		auto view = GetFramework()->GetServiceT< XE::CacheService >()->FindCache( XE::Hash( desc.Name ) );
		if( view.data() != nullptr )
		{
			::D3DCreateBlob( view.size(), &blob );

			std::copy( view.data(), view.data() + view.size(), (XE::int8 *)blob->GetBufferPointer() );
		}

		if( desc.Type == RenderPipelineStateType::GRAPHICS )
		{
			const XE::RenderGraphicsPipelineStateDesc & graphics = *( (const XE::RenderGraphicsPipelineStateDesc *)&desc );

			D3D12_GRAPHICS_PIPELINE_STATE_DESC pipe_desc = {};

			if( blob == nullptr )
			{
				XE::Array< D3D12_INPUT_ELEMENT_DESC > elements;
				for( size_t i = 0; i < graphics.InputLayout.Attributes.size(); i++ )
				{
					D3D12_INPUT_ELEMENT_DESC elem_desc;

					elem_desc.SemanticName = graphics.InputLayout.Attributes[i].Name.c_str();
					elem_desc.SemanticIndex = graphics.InputLayout.Attributes[i].SemanticIndex;
					elem_desc.Format = To( graphics.InputLayout.Attributes[i].Format );
					elem_desc.InputSlot = graphics.InputLayout.Attributes[i].Slot;
					elem_desc.AlignedByteOffset = graphics.InputLayout.Attributes[i].Offset;
					elem_desc.InputSlotClass = (D3D12_INPUT_CLASSIFICATION)graphics.InputLayout.Attributes[i].InstanceDivisor;
					elem_desc.InstanceDataStepRate = graphics.InputLayout.Attributes[i].InstanceDivisor;

					elements.emplace_back( std::move( elem_desc ) );
				}

				pipe_desc.NodeMask = 0;
				pipe_desc.Flags = GetFramework()->GetBool( XE::CONFIG_RENDER_DEBUG ) ? D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG : D3D12_PIPELINE_STATE_FLAG_NONE;
				pipe_desc.pRootSignature = ctx->_RenderDescriptorSets[graphics.DescriptorSet.GetValue()]._RootSignature;
				pipe_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
				pipe_desc.PrimitiveTopologyType = To( graphics.Topology, {} );
				if( program._Desc.VS ) { pipe_desc.VS = CD3DX12_SHADER_BYTECODE( ctx->_RenderShaders[program._Desc.VS.GetValue()]._Blob ); }
				if( program._Desc.PS ) { pipe_desc.PS = CD3DX12_SHADER_BYTECODE( ctx->_RenderShaders[program._Desc.PS.GetValue()]._Blob ); }
				if( program._Desc.GS ) { pipe_desc.GS = CD3DX12_SHADER_BYTECODE( ctx->_RenderShaders[program._Desc.GS.GetValue()]._Blob ); }
				if( program._Desc.HS ) { pipe_desc.HS = CD3DX12_SHADER_BYTECODE( ctx->_RenderShaders[program._Desc.HS.GetValue()]._Blob ); }
				if( program._Desc.DS ) { pipe_desc.DS = CD3DX12_SHADER_BYTECODE( ctx->_RenderShaders[program._Desc.DS.GetValue()]._Blob ); }
				pipe_desc.DSVFormat = To( graphics.DepthStencil );
				for( size_t i = 0; i < graphics.RenderTargets.size(); i++ ) { pipe_desc.RTVFormats[i] = To( graphics.RenderTargets[i] ); }
				pipe_desc.NumRenderTargets = graphics.RenderTargets.size();
				pipe_desc.SampleDesc.Count = graphics.SampleCount;
				pipe_desc.SampleDesc.Quality = 0;
				pipe_desc.SampleMask = graphics.BlendState.SampleMask;
				pipe_desc.BlendState.AlphaToCoverageEnable = graphics.BlendState.AlphaToCoverageEnabled;
				pipe_desc.BlendState.IndependentBlendEnable = graphics.BlendState.IndependentBlendEnabled;
				for( size_t i = 0; i < graphics.RenderTargets.size(); i++ )
				{
					pipe_desc.BlendState.RenderTarget[i].BlendEnable = graphics.BlendState.BlendTargets[i].BlendEnabled;
					pipe_desc.BlendState.RenderTarget[i].LogicOpEnable = graphics.BlendState.BlendTargets[i].LogicOpEnable;
					pipe_desc.BlendState.RenderTarget[i].LogicOp = To( graphics.BlendState.BlendTargets[i].LogicOp );
					pipe_desc.BlendState.RenderTarget[i].BlendOp = To( graphics.BlendState.BlendTargets[i].ColorArithmetic );
					pipe_desc.BlendState.RenderTarget[i].SrcBlend = To( graphics.BlendState.BlendTargets[i].SrcColor );
					pipe_desc.BlendState.RenderTarget[i].DestBlend = To( graphics.BlendState.BlendTargets[i].DstColor );
					pipe_desc.BlendState.RenderTarget[i].BlendOpAlpha = To( graphics.BlendState.BlendTargets[i].AlphaArithmetic );
					pipe_desc.BlendState.RenderTarget[i].SrcBlendAlpha = To( graphics.BlendState.BlendTargets[i].SrcAlpha );
					pipe_desc.BlendState.RenderTarget[i].DestBlendAlpha = To( graphics.BlendState.BlendTargets[i].DstAlpha );
					pipe_desc.BlendState.RenderTarget[i].RenderTargetWriteMask = To( graphics.BlendState.BlendTargets[i].ColorMask );
				}
				pipe_desc.InputLayout.NumElements = elements.size();
				pipe_desc.InputLayout.pInputElementDescs = elements.data();
				// pipe_desc.StreamOutput;
				pipe_desc.RasterizerState.FillMode = To( graphics.RasterizerState.PolygonMode );
				pipe_desc.RasterizerState.CullMode = To( graphics.RasterizerState.CullMode );
				pipe_desc.RasterizerState.FrontCounterClockwise = graphics.RasterizerState.FrontCCW;
				pipe_desc.RasterizerState.DepthBias = graphics.RasterizerState.DepthBiasConstantFactor;
				pipe_desc.RasterizerState.DepthBiasClamp = graphics.RasterizerState.DepthBiasClamp;
				pipe_desc.RasterizerState.SlopeScaledDepthBias = graphics.RasterizerState.DepthBiasSlopeFactor;
				pipe_desc.RasterizerState.DepthClipEnable = graphics.RasterizerState.DepthClampEnabled;
				pipe_desc.RasterizerState.MultisampleEnable = graphics.RasterizerState.MultiSampleEnabled;
				pipe_desc.RasterizerState.AntialiasedLineEnable = graphics.RasterizerState.AntiAliasedLineEnabled;
				pipe_desc.RasterizerState.ForcedSampleCount = 0;
				pipe_desc.RasterizerState.ConservativeRaster = graphics.RasterizerState.ConservativeRasterization ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
				pipe_desc.DepthStencilState.DepthEnable = graphics.DepthState.TestEnabled;
				pipe_desc.DepthStencilState.DepthWriteMask = graphics.DepthState.WriteEnabled ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
				pipe_desc.DepthStencilState.DepthFunc = To( graphics.DepthState.Compare );
				pipe_desc.DepthStencilState.StencilEnable = graphics.StencilState.TestEnabled;
				pipe_desc.DepthStencilState.StencilReadMask = graphics.StencilState.FrontReadMask;
				pipe_desc.DepthStencilState.StencilWriteMask = graphics.StencilState.FrontWriteMask;
				pipe_desc.DepthStencilState.FrontFace.StencilFailOp = To( graphics.StencilState.FrontStencilFail );
				pipe_desc.DepthStencilState.FrontFace.StencilDepthFailOp = To( graphics.StencilState.FrontDepthFail );
				pipe_desc.DepthStencilState.FrontFace.StencilPassOp = To( graphics.StencilState.FrontDepthPass );
				pipe_desc.DepthStencilState.FrontFace.StencilFunc = To( graphics.StencilState.FrontCompare );
				pipe_desc.DepthStencilState.BackFace.StencilFailOp = To( graphics.StencilState.BackStencilFail );
				pipe_desc.DepthStencilState.BackFace.StencilDepthFailOp = To( graphics.StencilState.BackDepthFail );
				pipe_desc.DepthStencilState.BackFace.StencilPassOp = To( graphics.StencilState.BackDepthPass );
				pipe_desc.DepthStencilState.BackFace.StencilFunc = To( graphics.StencilState.BackCompare );
			}
			else
			{
				state._Blob = blob;
				pipe_desc.CachedPSO.pCachedBlob = blob->GetBufferPointer();
				pipe_desc.CachedPSO.CachedBlobSizeInBytes = blob->GetBufferSize();
			}

			ctx->_Device->CreateGraphicsPipelineState( &pipe_desc, IID_PPV_ARGS( &state._PipelineState ) );
			state._Desc = graphics;
		}
		else
		{
			D3D12_COMPUTE_PIPELINE_STATE_DESC pipe_desc;
			if( blob == nullptr )
			{
				pipe_desc.NodeMask = 0;
				if( program._Desc.CS ) { pipe_desc.CS = CD3DX12_SHADER_BYTECODE( ctx->_RenderShaders[program._Desc.CS.GetValue()]._Blob ); }
				pipe_desc.Flags = GetFramework()->GetBool( XE::CONFIG_RENDER_DEBUG ) ? D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG : D3D12_PIPELINE_STATE_FLAG_NONE;
				pipe_desc.pRootSignature = ctx->_RenderDescriptorSets[desc.DescriptorSet.GetValue()]._RootSignature;
			}
			else
			{
				state._Blob = blob;
				pipe_desc.CachedPSO.pCachedBlob = blob->GetBufferPointer();
				pipe_desc.CachedPSO.CachedBlobSizeInBytes = blob->GetBufferSize();
			}

			ctx->_Device->CreateComputePipelineState( &pipe_desc, IID_PPV_ARGS( &state._PipelineState ) );

			state._Desc.Name = desc.Name;
			state._Desc.Type = desc.Type;
			state._Desc.ShaderProgram = desc.ShaderProgram;
			state._Desc.DescriptorSet = desc.DescriptorSet;
		}

		ctx->_RenderDescriptorSets[desc.DescriptorSet.GetValue()].Inc();
		ctx->_RenderShaderPrograms[desc.ShaderProgram.GetValue()].Inc();

		if( blob == nullptr )
		{
			auto hash = XE::Hash( desc.Name );
			state._PipelineState->GetCachedBlob( &blob );
			state._Blob = blob;

			GetFramework()->GetServiceT< XE::ThreadService >()->PostTask( XE::ThreadType::IO, [this, hash, blob]()
			{
				GetFramework()->GetServiceT< XE::CacheService >()->InsertCache( hash, { (XE::int8 *)blob->GetBufferPointer(), blob->GetBufferSize() } );
			} );
		}
	}
	return handle;
}

XE::RenderVirtualBufferHandle XE::RenderService::Create( XE::RenderContextHandle context, const XE::RenderVirtualBufferDesc & desc, XE::MemoryView data )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	FIND_FROM_NAME( RenderVirtualBuffer );

	auto handle = ctx->_RenderVirtualBufferHandleAllocator.Alloc();
	{
		auto & buffer = _p->_Contexts[context]->_RenderVirtualBuffers[handle.GetValue()];

		buffer.Inc();

		buffer._Desc = desc;
		buffer._Handle = handle;

		D3D12MA::VIRTUAL_ALLOCATION_DESC desc;
		desc.Alignment = 1;
		desc.Size = data.size();
		desc.pUserData = (void *)data.data();

		_p->_Contexts[context]->_VirtualAllocator->Allocate( &desc, &buffer._Address );
	}
	return handle;
}

#undef FIND_FROM_NAME

const XE::RenderQueryDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderQueryHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderQuerys[handle.GetValue()]._Desc;
}

const XE::RenderBufferDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderBufferHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderBuffers[handle.GetValue()]._Desc;
}

const XE::RenderTargetDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderTargetHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderTargets[handle.GetValue()]._Desc;
}

const XE::RenderShaderDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderShaderHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderShaders[handle.GetValue()]._Desc;
}

const XE::RenderTextureDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderTextureHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderTextures[handle.GetValue()]._Desc;
}

const XE::RenderSamplerDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderSamplerHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderSamplers[handle.GetValue()]._Desc;
}

const XE::RenderDescriptorSetDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderDescriptorSetHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderDescriptorSets[handle.GetValue()]._Desc;
}

const XE::RenderCommandBundleDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderCommandBundleHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderCommandBundles[handle.GetValue()]._Desc;
}

const XE::RenderShaderProgramDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderShaderProgramHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderShaderPrograms[handle.GetValue()]._Desc;
}

const XE::RenderPipelineStateDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderPipelineStateHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderPipelineStates[handle.GetValue()]._Desc;
}

const XE::RenderVirtualBufferDesc & XE::RenderService::GetDesc( XE::RenderContextHandle context, XE::RenderVirtualBufferHandle handle )
{
	return _p->_Contexts[context.GetValue()]->_RenderVirtualBuffers[handle.GetValue()]._Desc;
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderQueryHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & query = ctx->_RenderQuerys[handle.GetValue()];

	if( query.Dec() == 0 )
	{
		query._QueryStride = 0;
		query._QueryPreType = 0;

		auto heap = query._QueryHeap;

		query._QueryHeap = nullptr;
		query._Handle.Reset();
		query.Reset();

		ctx->_WaitCallbacks.push( [heap, result = std::move( query._QueryResults )]()
		{
			heap->Release();
		} );

		ctx->_RenderQueryHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderBufferHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & buffer = ctx->_RenderBuffers[handle.GetValue()];

	if( buffer.Dec() == 0 )
	{
		buffer._Handle.Reset();
		buffer.Reset();

		ctx->_WaitCallbacks.push( [res = std::move( buffer._Resource )]() {} );

		ctx->_RenderBufferHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderTargetHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & rendertarget = ctx->_RenderTargets[handle.GetValue()];

	if( rendertarget.Dec() == 0 )
	{
		ctx->_RenderTextures[rendertarget._Desc.Texture.GetValue()].Dec();

		rendertarget._Handle.Reset();
		rendertarget.Reset();

		ctx->_WaitCallbacks.push( [this, context, cpu_handle = rendertarget._CPUHandle]()
		{
			auto & rts = _p->_Contexts[context.GetValue()]->_RTVDescriptorAllocator;

			rts.Deallocate( cpu_handle );
		} );

		ctx->_RenderTargetHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderShaderHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & shader = ctx->_RenderShaders[handle.GetValue()];

	if( shader.Dec() == 0 )
	{
		auto blob = shader._Blob;

		shader._Blob = nullptr;
		shader._Handle.Reset();
		shader.Reset();

		ctx->_WaitCallbacks.push( [blob]() { blob->Release(); } );

		ctx->_RenderShaderHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderTextureHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & texture = ctx->_RenderTextures[handle.GetValue()];

	if( texture.Dec() == 0 )
	{
		texture._Handle.Reset();
		texture.Reset();

		ctx->_WaitCallbacks.push( [res = std::move( texture._Resource )]() {} );

		ctx->_RenderTextureHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderSamplerHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & sampler = ctx->_RenderSamplers[handle.GetValue()];

	if( sampler.Dec() == 0 )
	{
		auto cpu = sampler._CPUHandle;

		sampler._CPUHandle = {};
		sampler._Handle.Reset();
		sampler.Reset();

		ctx->_WaitCallbacks.push( [this, context, cpu]() { _p->_Contexts[context.GetValue()]->_SamplerDescriptorAllocator.Deallocate( cpu ); } );

		ctx->_RenderSamplerHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderDescriptorSetHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & desc_set = ctx->_RenderDescriptorSets[handle.GetValue()];

	if( desc_set.Dec() == 0 )
	{
		for( auto & it : desc_set._Desc.ResourceViews )
		{
			std::visit( XE::Overloaded{
				[&]( std::monostate ) {},
				[&]( XE::RenderBufferHandle handle ) { ctx->_RenderBuffers[handle.GetValue()].Dec(); },
				[&]( XE::RenderTextureHandle handle ) { ctx->_RenderTextures[handle.GetValue()].Dec(); }
						}, it.Resource );
		}

		desc_set._Handle.Reset();
		desc_set.Reset();
		auto dsv = desc_set._DSVHeap;
		auto rtv = desc_set._RTVHeap;
		auto sampler = desc_set._SamplerHeap;
		auto cbvsrvuav = desc_set._CBVSRVUAVHeap;
		auto root = desc_set._RootSignature;
		auto blob = desc_set._RootSignatureBlob;
		auto err = desc_set._RootSignatureErrBlob;

		desc_set._DSVHeap = nullptr;
		desc_set._RTVHeap = nullptr;
		desc_set._SamplerHeap = nullptr;
		desc_set._CBVSRVUAVHeap = nullptr;
		desc_set._RootSignature = nullptr;
		desc_set._RootSignatureBlob = nullptr;
		desc_set._RootSignatureErrBlob = nullptr;

		ctx->_WaitCallbacks.push( [dsv, rtv, sampler, cbvsrvuav, root, blob, err]()
		{
			dsv->Release(); rtv->Release(); sampler->Release(); cbvsrvuav->Release(); root->Release(); blob->Release(); err->Release();
		} );

		ctx->_RenderDescriptorSetHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderCommandBundleHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & bundle = ctx->_RenderCommandBundles[handle.GetValue()];

	if( bundle.Dec() == 0 )
	{
		bundle._Handle.Reset();
		bundle.Reset();

		auto list = bundle._CommandList;
		auto alloc = bundle._CommandAllocator;

		bundle._CommandList = nullptr;
		bundle._CommandAllocator = nullptr;

		ctx->_WaitCallbacks.push( [list, alloc]() { list->Release(); alloc->Release(); } );

		ctx->_RenderCommandBundleHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderShaderProgramHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & programs = ctx->_RenderShaderPrograms[handle.GetValue()];

	if( programs.Dec() == 0 )
	{
		if( programs._Desc.CS ) { ctx->_RenderShaders[programs._Desc.CS.GetValue()].Dec(); programs._Desc.CS.Reset(); }
		if( programs._Desc.VS ) { ctx->_RenderShaders[programs._Desc.VS.GetValue()].Dec(); programs._Desc.VS.Reset(); }
		if( programs._Desc.PS ) { ctx->_RenderShaders[programs._Desc.PS.GetValue()].Dec(); programs._Desc.PS.Reset(); }
		if( programs._Desc.HS ) { ctx->_RenderShaders[programs._Desc.HS.GetValue()].Dec(); programs._Desc.HS.Reset(); }
		if( programs._Desc.DS ) { ctx->_RenderShaders[programs._Desc.DS.GetValue()].Dec(); programs._Desc.DS.Reset(); }
		if( programs._Desc.GS ) { ctx->_RenderShaders[programs._Desc.GS.GetValue()].Dec(); programs._Desc.GS.Reset(); }

		programs._Handle.Reset();
		programs.Reset();

		ctx->_RenderShaderProgramHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderPipelineStateHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & pipe = ctx->_RenderPipelineStates[handle.GetValue()];

	if( pipe.Dec() == 0 )
	{
		ctx->_RenderShaderPrograms[pipe._Desc.ShaderProgram.GetValue()].Dec();
		ctx->_RenderDescriptorSets[pipe._Desc.DescriptorSet.GetValue()].Dec();
		pipe._Handle.Reset();
		pipe.Reset();

		auto blob = pipe._Blob;
		auto state = pipe._PipelineState;

		pipe._Blob = nullptr;
		pipe._PipelineState = nullptr;

		ctx->_WaitCallbacks.push( [blob, state]() { blob->Release(); state->Release(); } );

		ctx->_RenderPipelineStateHandleAllocator.Free( handle );
	}
}

void XE::RenderService::Destroy( XE::RenderContextHandle context, XE::RenderVirtualBufferHandle handle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & buf = ctx->_RenderVirtualBuffers[handle.GetValue()];

	if( buf.Dec() == 0 )
	{
		XE::uint64 addr = buf._Address;

		buf._Address = 0;
		buf._Handle.Reset();
		buf.Reset();

		ctx->_WaitCallbacks.push( [this, context, addr]()
		{
			_p->_Contexts[context.GetValue()]->_VirtualAllocator->FreeAllocation( addr );
		} );

		ctx->_RenderVirtualBufferHandleAllocator.Free( handle );
	}
}

XE::RenderCommandBufferHandle XE::RenderService::Begin( XE::RenderContextHandle context )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto handle = ctx->_RenderCommandBufferHandleAllocator.Alloc();

	auto & list = ctx->_RenderCommandBuffers[handle.GetValue()];

	if( list._CommandList == nullptr )
	{
		ctx->_Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &list._CommandAllocator ) );
		ctx->_Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, list._CommandAllocator, nullptr, IID_PPV_ARGS( &list._CommandList ) );
		list._CommandList->Close();
	}

	list._CommandAllocator->Reset();
	list._CommandList->Reset( list._CommandAllocator, nullptr );

	return handle;
}

void XE::RenderService::Read( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							  XE::RenderBufferHandle handle, XE::uint64 offset,
							  const XE::Delegate< void( void *, XE::uint64 ) > & callback )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & buf = ctx->_RenderBuffers[handle.GetValue()];

	XE::uint64 size = ( buf._Desc.Size * buf._Desc.Stride ) - ( offset * buf._Desc.Stride );

	D3D12Resource readback;
	D3D12MA::ALLOCATION_DESC desc;
	desc.HeapType = D3D12_HEAP_TYPE_READBACK;
	CD3DX12_RESOURCE_DESC res_desc = CD3DX12_RESOURCE_DESC::Buffer( size );

	ctx->_HeapAllocator->CreateResource( &desc, &res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, &readback._Allocation, IID_PPV_ARGS( &readback ) );

	auto dst_barrier = CD3DX12_RESOURCE_BARRIER::Transition( readback, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST );
	cmd._CommandList->ResourceBarrier( 1, &dst_barrier );
	auto state = TransitionBarrier( context, cmdbuf, handle, RenderResourceStatesType::COPY_SOURCE );
	{
		cmd._CommandList->CopyBufferRegion( readback, 0, buf._Resource, offset, size );
	}
	TransitionBarrier( context, cmdbuf, handle, state );
	dst_barrier = CD3DX12_RESOURCE_BARRIER::Transition( readback, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ );
	cmd._CommandList->ResourceBarrier( 1, &dst_barrier );

	cmd._WaitCallbacks.push_back( [this, read = std::move( readback ), callback, size]() mutable
	{
		void * data = nullptr;
		D3D12_RANGE beg_range{ 0, size };
		D3D12_RANGE end_range{ 0, 0 };
		read->Map( 0, &beg_range, &data );
		{
			callback( data, size );
		}
		read->Unmap( 0, &end_range );

		read.Release();
	} );
}

void XE::RenderService::Read( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							  XE::RenderTextureHandle handle,
							  XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth,
							  XE::uint32 mip, XE::uint32 mip_size,
							  const XE::Delegate< void( void * data, XE::uint64 y, XE::uint64 z, XE::uint64 size ) > & callback )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & tex = ctx->_RenderTextures[handle.GetValue()];

	D3D12Resource readback;
	D3D12MA::ALLOCATION_DESC desc;
	desc.HeapType = D3D12_HEAP_TYPE_READBACK;
	CD3DX12_RESOURCE_DESC res_desc;

	switch( tex._Desc.Type )
	{
	case XE::RenderTextureType::TEXTURE_1D:
		depth = 1;
	case XE::RenderTextureType::TEXTURE_1D_ARRAY:
		res_desc = CD3DX12_RESOURCE_DESC::Tex1D( To( tex._Desc.Format ), width, depth, mip_size );
		break;
	case XE::RenderTextureType::TEXTURE_2D:
	case XE::RenderTextureType::TEXTURE_2D_MS:
		depth = 1;
	case XE::RenderTextureType::TEXTURE_2D_ARRAY:
		res_desc = CD3DX12_RESOURCE_DESC::Tex2D( To( tex._Desc.Format ), width, height, depth, mip_size );
		break;
	case XE::RenderTextureType::TEXTURE_3D:
	case XE::RenderTextureType::TEXTURE_CUBE:
	case XE::RenderTextureType::TEXTURE_2D_MS_ARRAY:
		res_desc = CD3DX12_RESOURCE_DESC::Tex3D( To( tex._Desc.Format ), width, height, depth, mip_size );
		break;
	default:
		break;
	}

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	XE::uint32 num_row;
	XE::uint64 row_size, total;
	auto src_desc = tex._Resource->GetDesc();
	ctx->_Device->GetCopyableFootprints( &src_desc, offset_z, depth, 0, &footprint, &num_row, &row_size, &total );

	ctx->_HeapAllocator->CreateResource( &desc, &res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, &readback._Allocation, IID_PPV_ARGS( &readback ) );

	auto dst_barrier = CD3DX12_RESOURCE_BARRIER::Transition( readback, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST );
	cmd._CommandList->ResourceBarrier( 1, &dst_barrier );
	auto state = TransitionBarrier( context, cmdbuf, handle, RenderResourceStatesType::COPY_SOURCE );
	{
		CD3DX12_TEXTURE_COPY_LOCATION dst{ readback };
		CD3DX12_TEXTURE_COPY_LOCATION src{ tex._Resource };
		CD3DX12_BOX box{ (LONG)offset_x, (LONG)offset_y, (LONG)offset_z, (LONG)width, (LONG)height, (LONG)depth };

		cmd._CommandList->CopyTextureRegion( &dst, 0, 0, 0, &src, &box );
	}
	TransitionBarrier( context, cmdbuf, handle, state );
	dst_barrier = CD3DX12_RESOURCE_BARRIER::Transition( readback, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ );
	cmd._CommandList->ResourceBarrier( 1, &dst_barrier );

	cmd._WaitCallbacks.push_back( [this, read = std::move( readback ), callback, height, depth, num_row, row_size, total]()
	{
		void * data = nullptr;

		D3D12_RANGE beg_range{ 0, total };
		D3D12_RANGE end_range{ 0, 0 };
		read->Map( 0, &beg_range, &data );
		{
			for( size_t z = 1; z < (XE::uint64)depth + 1; z++ )
			{
				for( size_t y = 0; y < height; y++ )
				{
					callback( (XE::uint8 *)data + ( row_size * y ) * z,
							  y,
							  z,
							  row_size );
				}
			}
		}
		read->Unmap( 0, &end_range );

		read.Release();
	} );
}

void XE::RenderService::Write( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							   XE::RenderBufferHandle handle, XE::uint64 offset,
							   XE::MemoryView src_data )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & buf = ctx->_RenderBuffers[handle.GetValue()];

	XE::uint64 size = src_data.size();

	D3D12Resource upload;

	D3D12MA::ALLOCATION_DESC desc;
	desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC res_desc = CD3DX12_RESOURCE_DESC::Buffer( size );
	D3D12_CLEAR_VALUE clear;
	clear.Format = To( XE::RenderFormatType::R8_UNORM );
	clear.Color[0] = 0; clear.Color[1] = 0; clear.Color[1] = 0; clear.Color[1] = 0;

	ctx->_HeapAllocator->CreateResource( &desc, &res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, &clear, &upload._Allocation, IID_PPV_ARGS( &upload ) );

	void * data = nullptr;
	upload->Map( 0, nullptr, &data );
	{
		std::memcpy( data, src_data.data(), src_data.size() );
	}
	upload->Unmap( 0, nullptr );

	auto dst_barrier = CD3DX12_RESOURCE_BARRIER::Transition( upload, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_SOURCE );
	cmd._CommandList->ResourceBarrier( 1, &dst_barrier );
	auto state = TransitionBarrier( context, cmdbuf, handle, RenderResourceStatesType::COPY_DEST );
	{
		ctx->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->CopyBufferRegion( ctx->_RenderBuffers[handle.GetValue()]._Resource, offset, upload, 0, size );
	}
	TransitionBarrier( context, cmdbuf, handle, state );


	cmd._WaitCallbacks.push_back( [this, upload = std::move( upload )](){ upload.Release(); } );
}

void XE::RenderService::Write( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							   XE::RenderTextureHandle handle,
							   XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth,
							   XE::uint32 mip, XE::uint32 mip_size,
							   XE::MemoryView src_data )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & tex = ctx->_RenderTextures[handle.GetValue()];

	D3D12Resource upload;
	D3D12MA::ALLOCATION_DESC desc;
	desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
	CD3DX12_RESOURCE_DESC res_desc;

	switch( tex._Desc.Type )
	{
	case XE::RenderTextureType::TEXTURE_1D:
		depth = 1;
	case XE::RenderTextureType::TEXTURE_1D_ARRAY:
		res_desc = CD3DX12_RESOURCE_DESC::Tex1D( To( tex._Desc.Format ), width, depth, mip_size );
		break;
	case XE::RenderTextureType::TEXTURE_2D:
	case XE::RenderTextureType::TEXTURE_2D_MS:
		depth = 1;
	case XE::RenderTextureType::TEXTURE_2D_ARRAY:
		res_desc = CD3DX12_RESOURCE_DESC::Tex2D( To( tex._Desc.Format ), width, height, depth, mip_size );
		break;
	case XE::RenderTextureType::TEXTURE_3D:
	case XE::RenderTextureType::TEXTURE_CUBE:
	case XE::RenderTextureType::TEXTURE_2D_MS_ARRAY:
		res_desc = CD3DX12_RESOURCE_DESC::Tex3D( To( tex._Desc.Format ), width, height, depth, mip_size );
		break;
	default:
		break;
	}

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	XE::uint32 num_row;
	XE::uint64 row_size, total;
	auto src_desc = tex._Resource->GetDesc();
	ctx->_Device->GetCopyableFootprints( &src_desc, offset_z, depth, offset_x, &footprint, &num_row, &row_size, &total );

	ctx->_HeapAllocator->CreateResource( &desc, &res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, &upload._Allocation, IID_PPV_ARGS( &upload ) );

	void * data = nullptr;
	D3D12_RANGE beg_range{ 0, total };
	D3D12_RANGE end_range{ 0, 0 };
	upload->Map( 0, &beg_range, &data );
	for( size_t z = 1; z < (XE::uint64)depth + 1; z++ )
	{
		for( size_t y = 0; y < height; ++y )
		{
			std::memcpy( (XE::uint8 *)data + static_cast<SIZE_T>( footprint.Footprint.RowPitch ) * y * z
						 , src_data.data() + static_cast<SIZE_T>( width ) * y * z
						 , width );
		}
	}
	upload->Unmap( 0, &end_range );

	auto dst_barrier = CD3DX12_RESOURCE_BARRIER::Transition( upload, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_SOURCE );
	cmd._CommandList->ResourceBarrier( 1, &dst_barrier );
	auto state = TransitionBarrier( context, cmdbuf, handle, RenderResourceStatesType::COPY_DEST );
	{
		CD3DX12_TEXTURE_COPY_LOCATION src{ upload };
		CD3DX12_TEXTURE_COPY_LOCATION dst{ tex._Resource };
		CD3DX12_BOX box{ (LONG)offset_x, (LONG)offset_y, (LONG)offset_z, (LONG)width, (LONG)height, (LONG)depth };

		cmd._CommandList->CopyTextureRegion( &dst, 0, 0, 0, &src, &box );
	}
	TransitionBarrier( context, cmdbuf, handle, state );

	cmd._WaitCallbacks.push_back( [this, upload = std::move( upload )]()
	{
		upload.Release();
	} );
}

void XE::RenderService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							  XE::RenderBufferHandle dst_buf, XE::uint64 dst_offset,
							  XE::RenderBufferHandle src_buf, XE::uint64 src_offset,
							  XE::uint64 size )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & dst = ctx->_RenderBuffers[dst_buf.GetValue()];
	auto & src = ctx->_RenderBuffers[src_buf.GetValue()];

	auto dst_state = TransitionBarrier( context, cmdbuf, dst_buf, RenderResourceStatesType::COPY_DEST );
	auto src_state = TransitionBarrier( context, cmdbuf, src_buf, RenderResourceStatesType::COPY_SOURCE );
	{
		cmd._CommandList->CopyBufferRegion( dst._Resource, dst_offset, src._Resource, src_offset, size );
	}
	TransitionBarrier( context, cmdbuf, dst_buf, dst_state );
	TransitionBarrier( context, cmdbuf, src_buf, src_state );
}

void XE::RenderService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							  XE::RenderBufferHandle dst_buf, XE::uint64 dst_offset,
							  XE::RenderTextureHandle src_tex,
							  XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size )
{
	// TODO: 
}

void XE::RenderService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							  XE::RenderTextureHandle dst_tex,
							  XE::uint32 offset_x, XE::uint32 offset_y, XE::uint32 offset_z, XE::uint32 width, XE::uint32 height, XE::uint32 depth, XE::uint32 mip, XE::uint32 mip_size,
							  XE::RenderBufferHandle src_buf, XE::uint64 src_offset )
{
	// TODO: 
}

void XE::RenderService::Copy( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf,
							  XE::RenderTextureHandle dst_tex,
							  XE::uint32 dst_offset_x, XE::uint32 dst_offset_y, XE::uint32 dst_offset_z, XE::uint32 dst_mip,
							  XE::RenderTextureHandle src_tex,
							  XE::uint32 src_offset_x, XE::uint32 src_offset_y, XE::uint32 src_offset_z, XE::uint32 src_mip,
							  XE::uint32 width, XE::uint32 height, XE::uint32 depth )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & dst = ctx->_RenderTextures[dst_tex.GetValue()];
	auto & src = ctx->_RenderTextures[src_tex.GetValue()];

	auto dst_state = TransitionBarrier( context, cmdbuf, dst_tex, RenderResourceStatesType::COPY_DEST );
	auto src_state = TransitionBarrier( context, cmdbuf, src_tex, RenderResourceStatesType::COPY_SOURCE );
	{
		CD3DX12_TEXTURE_COPY_LOCATION dst_loc( dst._Resource );
		CD3DX12_TEXTURE_COPY_LOCATION src_loc( src._Resource );
		CD3DX12_BOX box( src_offset_x, src_offset_y, src_offset_z, src_offset_x + width, src_offset_y + height, src_offset_z + depth );

		cmd._CommandList->CopyTextureRegion( &dst_loc, dst_offset_x, dst_offset_y, dst_offset_z, &src_loc, &box );
	}
	TransitionBarrier( context, cmdbuf, dst_tex, dst_state );
	TransitionBarrier( context, cmdbuf, src_tex, src_state );
}

XE::RenderResourceStatesType XE::RenderService::TransitionBarrier( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle buffer, XE::RenderResourceStatesType after )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & buf = ctx->_RenderBuffers[buffer.GetValue()];

	auto before = buf._CurrentState;

	if( before != after )
	{
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition( buf._Resource, To( before ), To( after ) );

		ctx->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->ResourceBarrier( 1, &barrier );

		buf._CurrentState = after;
	}

	return before;
}

XE::RenderResourceStatesType XE::RenderService::TransitionBarrier( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderTextureHandle texture, XE::RenderResourceStatesType after )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & tex = ctx->_RenderTextures[texture.GetValue()];

	auto before = tex._CurrentState;

	if( before != after )
	{
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition( tex._Resource, To( before ), To( after ) );

		ctx->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->ResourceBarrier( 1, &barrier );

		tex._CurrentState = after;
	}

	return before;
}

XE::RenderPassHandle XE::RenderService::BeginPass( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, const XE::RenderPassDesc & desc )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	auto handle = ctx->_RenderPassHandleAllocator.Alloc();
	{
		auto & pass = ctx->_RenderPasses[handle.GetValue()];

		for( const auto & it : desc.Colors )
		{
			ctx->_RenderTargets[it.RenderTarget.GetValue()].Inc();
			pass._Colors.push_back( it.RenderTarget );
			pass._ColorCPUHandles.push_back( ctx->_RenderTargets[it.RenderTarget.GetValue()]._CPUHandle );
		}

		ctx->_RenderTargets[desc.DepthStencil.RenderTarget.GetValue()].Inc();
		pass._DepthStencil = desc.DepthStencil.RenderTarget;
		pass._DepthStencilCPUHandle = ctx->_RenderTargets[desc.DepthStencil.RenderTarget.GetValue()]._CPUHandle;

		cmd._CommandList->OMSetRenderTargets( pass._ColorCPUHandles.size(), pass._ColorCPUHandles.data(), false, &pass._DepthStencilCPUHandle );

		if( desc.Clear.Flags && XE::RenderClearFlag::COLOR )
		{
			for( auto & it : pass._ColorCPUHandles )
			{
				cmd._CommandList->ClearRenderTargetView( it, desc.Clear.Color.d, 0, nullptr );
			}
		}
		if( ( desc.Clear.Flags || XE::RenderClearFlag::DEPTH ) || ( desc.Clear.Flags || XE::RenderClearFlag::STENCIL ) )
		{
			D3D12_CLEAR_FLAGS flags = (D3D12_CLEAR_FLAGS)( desc.Clear.Flags & XE::RenderClearFlag::DEPTH & XE::RenderClearFlag::STENCIL ).GetValue();

			cmd._CommandList->ClearDepthStencilView( pass._DepthStencilCPUHandle, flags, desc.Clear.Depth, desc.Clear.Stencil, 0, nullptr );
		}

		// TODO: 
		/*
		pass._RenderTargets.resize( desc.Colors.size() );
		for( size_t i = 0; i < desc.Colors.size(); i++ )
		{
			auto & color = desc.Colors[i];
			auto & rendertarget = ctx->_RenderTargets[color.RenderTarget.GetValue()];
			auto & texture = ctx->_RenderTextures[rendertarget._Desc.Texture.GetValue()];

			pass._RenderTargets[i].cpuDescriptor = rendertarget._CPUHandle;

			pass._RenderTargets[i].BeginningAccess.Type = To( color.LoadOp );
			pass._RenderTargets[i].BeginningAccess.Clear.ClearValue.Format = To( color.Format );
			pass._RenderTargets[i].BeginningAccess.Clear.ClearValue.DepthStencil.Depth = texture._Desc.Clear.Depth;
			pass._RenderTargets[i].BeginningAccess.Clear.ClearValue.DepthStencil.Stencil = texture._Desc.Clear.Stencil;
			std::memcpy( pass._RenderTargets[i].BeginningAccess.Clear.ClearValue.Color, texture._Desc.Clear.Color.d, sizeof( XE::FColor ) );

			pass._RenderTargets[i].EndingAccess.Type = To( color.StoreOp );
			pass._RenderTargets[i].EndingAccess.Resolve.Format = To( color.Format );
			pass._RenderTargets[i].EndingAccess.Resolve;
			pass._RenderTargets[i].EndingAccess.Resolve;
			pass._RenderTargets[i].EndingAccess.Resolve;
			pass._RenderTargets[i].EndingAccess.Resolve;
		}

		cmd._CommandList->BeginRenderPass(,)
		*/
	}
	return handle;
}

void XE::RenderService::SetViewport( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Rectf & view )
{
	D3D12_VIEWPORT viewport{ view.x, view.y, view.width, view.height, 0.0f, 1.0f };

	_p->_Contexts[context.GetValue()]->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->RSSetViewports( 1, &viewport );
}

void XE::RenderService::SetViewport( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::Rectf > & views )
{
	XE::Array< D3D12_VIEWPORT > ports;
	for( const auto & it : views )
	{
		ports.push_back( D3D12_VIEWPORT{ it.x, it.y, it.width, it.height, 0.0f, 1.0f } );
	}
	_p->_Contexts[context.GetValue()]->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->RSSetViewports( ports.size(), ports.data() );
}

void XE::RenderService::SetScissor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Rectf & rect )
{
	D3D12_RECT _rect{ (LONG)rect.x, (LONG)rect.y, (LONG)rect.x + (LONG)rect.width, (LONG)rect.y + (LONG)rect.height };
	_p->_Contexts[context.GetValue()]->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->RSSetScissorRects( 1, &_rect );
}

void XE::RenderService::SetScissor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::Rectf > & rects )
{
	XE::Array< D3D12_RECT > _rects;
	for( const auto & it : rects )
	{
		_rects.push_back( D3D12_RECT{ (LONG)it.x, (LONG)it.y, (LONG)it.x + (LONG)it.width, (LONG)it.y + (LONG)it.height } );
	}
	_p->_Contexts[context.GetValue()]->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->RSSetScissorRects( _rects.size(), _rects.data() );
}

void XE::RenderService::SetIndexBuffer( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderVirtualBufferHandle buffer )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & buf = ctx->_RenderVirtualBuffers[buffer.GetValue()];

	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = buf._Address;
	view.SizeInBytes = buf._Desc.Size;
	view.Format = To( StrideToFormat( buf._Desc.Stride ) );

	_p->_Contexts[context.GetValue()]->_RenderCommandBuffers[cmdbuf.GetValue()]._CommandList->IASetIndexBuffer( &view );
}

void XE::RenderService::SetVertexBuffer( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderVirtualBufferHandle buffer )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & buf = ctx->_RenderVirtualBuffers[buffer.GetValue()];

	D3D12_VERTEX_BUFFER_VIEW view;
	view.BufferLocation = buf._Address;
	view.SizeInBytes = buf._Desc.Size;
	view.StrideInBytes = buf._Desc.Stride;

	cmd._CommandList->IASetVertexBuffers( 0, 1, &view );
}

void XE::RenderService::SetDescriptorSet( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderDescriptorSetHandle heap, XE::uint32 first, XE::RenderPipelineStateType bind )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & desc = ctx->_RenderDescriptorSets[heap.GetValue()];

	cmd._CommandList->SetGraphicsRootSignature( desc._RootSignature );
}

void XE::RenderService::SetPipelineState( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderPipelineStateHandle pso )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & pipe = ctx->_RenderPipelineStates[pso.GetValue()];

	cmd._CommandList->SetPipelineState( pipe._PipelineState );
}

void XE::RenderService::SetUniform( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 location, const XE::Variant & value )
{
	// TODO: 
}

void XE::RenderService::SetUniform( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 location, const XE::VariantArray & value )
{
	// TODO: 
}

void XE::RenderService::SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{
	// TODO: 
}

void XE::RenderService::SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderTextureHandle texture, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{
	// TODO: 
}

void XE::RenderService::SetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderSamplerHandle sampler, XE::uint32 slot, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{
	// TODO: 
}

void XE::RenderService::ResetResource( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderResourceType type, const XE::Pair< XE::uint32, XE::uint32 > & slot_ranges, XE::RenderBindFlags bind, XE::RenderStageFlags stage )
{
	// TODO: 
}

void XE::RenderService::SetBlendFactor( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::FColor & color )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->OMSetBlendFactor( color.d );
}

void XE::RenderService::SetStencilReference( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 reference, XE::RenderStencilFaceType stencilFace /*= XE::RenderStencilFaceType::FRONT_BACK */ )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->OMSetStencilRef( reference );
}

void XE::RenderService::BeginQuery( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 value )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & quer = ctx->_RenderQuerys[query.GetValue()];

	if( To( quer._Desc.Type ) == D3D12_QUERY_TYPE_TIMESTAMP )
	{
		cmd._CommandList->EndQuery( quer._QueryHeap, To( quer._Desc.Type ), value * quer._QueryPreType );
	}
	else
	{
		cmd._CommandList->BeginQuery( quer._QueryHeap, To( quer._Desc.Type ), value * quer._QueryPreType );
	}
}

void XE::RenderService::EndQuery( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 value )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & quer = ctx->_RenderQuerys[query.GetValue()];

	if( To( quer._Desc.Type ) == D3D12_QUERY_TYPE_TIMESTAMP )
	{
		cmd._CommandList->EndQuery( quer._QueryHeap, To( quer._Desc.Type ), value * quer._QueryPreType + 1 );
	}
	else
	{
		cmd._CommandList->EndQuery( quer._QueryHeap, To( quer._Desc.Type ), value * quer._QueryPreType );
	}
}

void XE::RenderService::BeginCondition( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query, XE::uint32 offset, XE::RenderConditionType cond )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & quer = ctx->_RenderQuerys[query.GetValue()];

	cmd._CommandList->SetPredication( quer._QueryResults, (XE::uint64)offset * quer._QueryStride, To( cond ) );
}

void XE::RenderService::EndCondition( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderQueryHandle query )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & quer = ctx->_RenderQuerys[query.GetValue()];

	cmd._CommandList->SetPredication( quer._QueryResults, 0, D3D12_PREDICATION_OP_EQUAL_ZERO );
}

void XE::RenderService::BeginStreamOutput( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Array< XE::RenderBufferHandle > & buffers )
{
	// TODO: 
}

void XE::RenderService::EndStreamOutput( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass )
{
	// TODO: 
}

void XE::RenderService::Draw( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawInstanced( vertices.second - vertices.first, 1, vertices.first, 0 );
}

void XE::RenderService::DrawIndexed( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & indices )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawIndexedInstanced( indices.second - indices.first, 1, indices.first, 0, 0 );
}

void XE::RenderService::DrawIndexed( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & indices, XE::uint32 vertex_offset )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawIndexedInstanced( indices.second - indices.first, 1, indices.first, vertex_offset, 0 );
}

void XE::RenderService::DrawInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices, XE::uint32 num_instances )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawInstanced( vertices.second - vertices.first, num_instances, vertices.first, 0 );
}

void XE::RenderService::DrawInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, const XE::Pair< XE::uint32, XE::uint32 > & vertices, const XE::Pair< XE::uint32, XE::uint32 > & instances )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawInstanced( vertices.second - vertices.first, instances.second - instances.first, vertices.first, instances.first );
}

void XE::RenderService::DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawIndexedInstanced( num_indices, num_instances, first_index, 0, 0 );
}

void XE::RenderService::DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index, XE::uint32 vertex_offset )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawIndexedInstanced( num_indices, num_instances, first_index, vertex_offset, 0 );
}

void XE::RenderService::DrawIndexedInstanced( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::uint32 num_indices, XE::uint32 num_instances, XE::uint32 first_index, XE::uint32 vertex_offset, XE::uint32 first_instance )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->DrawIndexedInstanced( num_indices, num_instances, first_index, vertex_offset, first_instance );
}

void XE::RenderService::DrawIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset )
{
	// TODO: 
}

void XE::RenderService::DrawIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset, XE::uint32 num_commands, XE::uint32 stride )
{
	// TODO: 
}

void XE::RenderService::DrawIndexedIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset )
{
	// TODO: 
}

void XE::RenderService::DrawIndexedIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderBufferHandle buffer, XE::uint64 offset, XE::uint32 num_commands, XE::uint32 stride )
{
	// TODO: 
}

void XE::RenderService::EndPass( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass )
{
	// TODO: 
}

void XE::RenderService::ExecuteBundle( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderPassHandle pass, XE::RenderCommandBundleHandle bundle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & bun = ctx->_RenderCommandBundles[bundle.GetValue()];

	cmd._CommandList->ExecuteBundle( bun._CommandList );
}

void XE::RenderService::Dispatch( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::uint32 group_x, XE::uint32 group_y, XE::uint32 group_z )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->Dispatch( group_x, group_y, group_z );
}

void XE::RenderService::DispatchIndirect( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderBufferHandle buffer, XE::uint64 offset )
{
	// TODO: 
}

void XE::RenderService::End( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];

	cmd._CommandList->Close();
	ctx->_CloseCommandList.push( cmdbuf );
	ctx->_WaitCallbacks.push( [this, context, cmdbuf, callbacks = std::move(cmd._WaitCallbacks)]()
	{
		_p->_Contexts[context.GetValue()]->_RenderCommandBufferHandleAllocator.Free( cmdbuf );

		for( const auto & it : callbacks )
		{
			it();
		}
	} );
}

void XE::RenderService::BeginBundle( XE::RenderContextHandle context, XE::RenderCommandBundleHandle bundle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	auto & bun = ctx->_RenderCommandBundles[bundle.GetValue()];

	if( bun._CommandList == nullptr )
	{
		ctx->_Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS( &bun._CommandAllocator ) );
		ctx->_Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_BUNDLE, bun._CommandAllocator, nullptr, IID_PPV_ARGS( &bun._CommandList ) );
		bun._CommandList->Close();
	}

	bun._CommandAllocator->Reset();
	bun._CommandList->Reset( bun._CommandAllocator, nullptr );
}

void XE::RenderService::EndBundle( XE::RenderContextHandle context, XE::RenderCommandBundleHandle bundle )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & bun = ctx->_RenderCommandBundles[bundle.GetValue()];

	bun._CommandList->Close();
}

void XE::RenderService::Execute( XE::RenderContextHandle context )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	XE::Array< ID3D12CommandList * > lists( XE::MemoryResource::GetFrameMemoryResource() );

	XE::RenderCommandBufferHandle handle;
	while( ctx->_CloseCommandList.try_pop( handle ) )
	{
		lists.push_back( ctx->_RenderCommandBuffers[handle.GetValue()]._CommandList );
	}

	if( !lists.empty() )
	{
		ctx->_CommandQueue->ExecuteCommandLists( lists.size(), lists.data() );
	}
}

void XE::RenderService::Wait( XE::RenderContextHandle context )
{
	auto & ctx = _p->_Contexts[context.GetValue()];

	XE::uint64 fence = ctx->_FenceValue++;
	auto hr = ctx->_CommandQueue->Signal( ctx->_Fence, fence );
	XE_ASSERT( FAILED( hr ) && "command queue signal() error" );

	while( ctx->_Fence->GetCompletedValue() < fence )
	{
		auto hr = ctx->_Fence->SetEventOnCompletion( fence, ctx->_FenceEvent );
		XE_ASSERT( FAILED( hr ) && "fence SetEventOnCompletion() error" );
		::WaitForSingleObject( ctx->_FenceEvent, INFINITE );
	}

	XE::Delegate<void()> callback;
	while( ctx->_WaitCallbacks.try_pop( callback ) )
	{
		callback();
	}
}

void XE::RenderService::QueryResolve( XE::RenderContextHandle context, XE::RenderCommandBufferHandle cmdbuf, XE::RenderQueryHandle query, XE::uint32 offset, XE::uint32 size )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & cmd = ctx->_RenderCommandBuffers[cmdbuf.GetValue()];
	auto & quer = ctx->_RenderQuerys[query.GetValue()];

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		quer._QueryResults,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_COPY_DEST );
	cmd._CommandList->ResourceBarrier( 1, &barrier );

	cmd._CommandList->ResolveQueryData(
		quer._QueryHeap,
		To( quer._Desc.Type ),
		offset * quer._QueryPreType,
		size * quer._QueryPreType,
		quer._QueryResults,
		(XE::uint64)quer._QueryStride * offset * quer._QueryPreType );

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		quer._QueryResults,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ );
	cmd._CommandList->ResourceBarrier( 1, &barrier );
}

void XE::RenderService::QueryResult( XE::RenderContextHandle context, XE::RenderQueryHandle query, XE::uint32 offset, XE::uint32 size, std::ostream & stream )
{
	auto & ctx = _p->_Contexts[context.GetValue()];
	auto & quer = ctx->_RenderQuerys[query.GetValue()];

	void * data = nullptr;

	D3D12_RANGE beg_range, end_range;
	beg_range.Begin = (XE::uint64)offset * quer._QueryPreType * quer._QueryStride;
	beg_range.End = (XE::uint64)offset * quer._QueryPreType * quer._QueryStride + size * quer._QueryPreType * quer._QueryStride;
	end_range.Begin = 0;
	end_range.End = 0;

	quer._QueryResults->Map( 0, &beg_range, &data );
	{
		stream.write( (const char *)data, beg_range.End - beg_range.Begin );
	}
	quer._QueryResults->Unmap( 0, &end_range );
}

void XE::RenderService::OnMainWindowResize()
{
	auto size = GetFramework()->GetMainWindow()->GetScreenSize();
	bool fullscreen = GetFramework()->GetMainWindow()->IsFullscreen();
	BOOL state = false; _p->_SwapChain->GetFullscreenState( &state, nullptr );

	_p->_SwapChain->ResizeBuffers(
		XE::RENDER_MAX_SWAP_CHAIN_BACK_BUFFER,
		size.first,
		size.second,
		To( XE::RenderFormatType::R8G8B8A8_UNORM ),
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );

	if( fullscreen != (bool)state )
	{
		_p->_SwapChain->SetFullscreenState( !state, nullptr );
	}

	for( UINT i = 0; i < XE::RENDER_MAX_SWAP_CHAIN_BACK_BUFFER; i++ )
	{
		auto rt_handle = XE::HandleCast< XE::RenderTarget >( i );
		auto tex_handle = XE::HandleCast< XE::RenderTexture >( i );
		auto & texture = _p->_Contexts[_p->_MainContext]->_RenderTextures[tex_handle.GetValue()];
		auto & rendertarget = _p->_Contexts[_p->_MainContext]->_RenderTargets[rt_handle.GetValue()];

		ID3D12Resource * resource = nullptr;

		_p->_SwapChain->GetBuffer( i, IID_PPV_ARGS( &resource ) );

		_p->_Contexts[_p->_MainContext]->_Device->CreateRenderTargetView( resource, nullptr, rendertarget._CPUHandle );

		D3D12_RESOURCE_DESC rt_desc = resource->GetDesc();

		texture._Handle = tex_handle;
		texture._Resource = resource;
		texture._CurrentState = XE::RenderResourceStatesType::PRESENT;

		texture._Desc.Width = rt_desc.Width;
		texture._Desc.Height = rt_desc.Height;
		texture._Desc.Depth = rt_desc.DepthOrArraySize;

		rendertarget._Handle = rt_handle;

		rendertarget._Desc.Texture = tex_handle;
		rendertarget._Desc.Resolution.x = rt_desc.Width;
		rendertarget._Desc.Resolution.y = rt_desc.Height;
	}

	_p->_SwapChainBackBufferIndex = _p->_SwapChain->GetCurrentBackBufferIndex();
}

#endif // !defined( NULL_RENDER ) && ( PLATFORM_OS & ( OS_WINDOWS | OS_XBOX ) )