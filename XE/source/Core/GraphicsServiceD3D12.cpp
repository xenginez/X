#include "GraphicsServiceD3D12.h"

#if GRAPHICS_API & GRAPHICS_D3D12

#include <mutex>

#include <d3d12.h>
#include <windows.h>
#include <dxgi1_6.h>
#include <sdkddkver.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#include "Utils/Logger.h"
#include "Utils/Window.h"
#include "Utils/RefCount.h"

#include "CoreFramework.h"
#include "EventService.h"
#include "CacheService.h"
#include "ThreadService.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#undef ERROR

namespace
{
	static constexpr XE::uint64 GRAPHICS_MAX_COMMAND_ALLOCATOR = 32;
	static constexpr XE::uint64 GRAPHICS_MAX_RTV_DESCRIPTOR_HEAP = 64;
	static constexpr XE::uint64 GRAPHICS_MAX_DSV_DESCRIPTOR_HEAP = 64;
	static constexpr XE::uint64 GRAPHICS_MAX_CSU_DESCRIPTOR_HEAP = 4096;
	static constexpr XE::uint64 GRAPHICS_MAX_SAMPLER_DESCRIPTOR_HEAP = 1024;

	enum class PassKind
	{
		NONE,
		RENDER,
		BUNDLE,
		COMPUTE,
	};

	DXGI_FORMAT Cast( XE::GraphicsIndexFormat format )
	{
		switch ( format )
		{
		case XE::GraphicsIndexFormat::UINT16:
			return DXGI_FORMAT_R16_UINT;
		case XE::GraphicsIndexFormat::UINT32:
			return DXGI_FORMAT_R32_UINT;
		default:
			break;
		}

		return DXGI_FORMAT_UNKNOWN;
	}
	DXGI_FORMAT Cast( XE::GraphicsVertexFormat format )
	{
		switch ( format )
		{
		case XE::GraphicsVertexFormat::UINT8X2:
			return DXGI_FORMAT_R8G8_UINT;
		case XE::GraphicsVertexFormat::UINT8X4:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case XE::GraphicsVertexFormat::SINT8X2:
			return DXGI_FORMAT_R8G8_SINT;
		case XE::GraphicsVertexFormat::SINT8X4:
			return DXGI_FORMAT_R8G8B8A8_SINT;
		case XE::GraphicsVertexFormat::UNORM8X2:
			return DXGI_FORMAT_R8G8_UNORM;
		case XE::GraphicsVertexFormat::UNORM8X4:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case XE::GraphicsVertexFormat::SNORM8X2:
			return DXGI_FORMAT_R8G8_SNORM;
		case XE::GraphicsVertexFormat::SNORM8X4:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case XE::GraphicsVertexFormat::UINT16X2:
			return DXGI_FORMAT_R16G16_UINT;
		case XE::GraphicsVertexFormat::UINT16X4:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case XE::GraphicsVertexFormat::SINT16X2:
			return DXGI_FORMAT_R16G16_SINT;
		case XE::GraphicsVertexFormat::SINT16X4:
			return DXGI_FORMAT_R16G16B16A16_SINT;
		case XE::GraphicsVertexFormat::UNORM16X2:
			return DXGI_FORMAT_R16G16_UNORM;
		case XE::GraphicsVertexFormat::UNORM16X4:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case XE::GraphicsVertexFormat::SNORM16X2:
			return DXGI_FORMAT_R16G16_SNORM;
		case XE::GraphicsVertexFormat::SNORM16X4:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		case XE::GraphicsVertexFormat::FLOAT16X2:
			return DXGI_FORMAT_R16G16_FLOAT;
		case XE::GraphicsVertexFormat::FLOAT16X4:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case XE::GraphicsVertexFormat::FLOAT32:
			return DXGI_FORMAT_R32_FLOAT;
		case XE::GraphicsVertexFormat::FLOAT32X2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case XE::GraphicsVertexFormat::FLOAT32X3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case XE::GraphicsVertexFormat::FLOAT32X4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case XE::GraphicsVertexFormat::UINT32:
			return DXGI_FORMAT_R32_UINT;
		case XE::GraphicsVertexFormat::UINT32X2:
			return DXGI_FORMAT_R32G32_UINT;
		case XE::GraphicsVertexFormat::UINT32X3:
			return DXGI_FORMAT_R32G32B32_UINT;
		case XE::GraphicsVertexFormat::UINT32X4:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case XE::GraphicsVertexFormat::SINT32:
			return DXGI_FORMAT_R32_SINT;
		case XE::GraphicsVertexFormat::SINT32X2:
			return DXGI_FORMAT_R32G32_SINT;
		case XE::GraphicsVertexFormat::SINT32X3:
			return DXGI_FORMAT_R32G32B32_SINT;
		case XE::GraphicsVertexFormat::SINT32X4:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		}

		return DXGI_FORMAT_UNKNOWN;
	}
	DXGI_FORMAT Cast( XE::GraphicsTextureFormat format )
	{
		switch ( format )
		{
		case XE::GraphicsTextureFormat::UNDEFINED:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::R8UNORM:
			return DXGI_FORMAT_R8_UNORM;
		case XE::GraphicsTextureFormat::R8SNORM:
			return DXGI_FORMAT_R8_SNORM;
		case XE::GraphicsTextureFormat::R8UINT:
			return DXGI_FORMAT_R8_UINT;
		case XE::GraphicsTextureFormat::R8SINT:
			return DXGI_FORMAT_R8_SINT;
		case XE::GraphicsTextureFormat::R16UINT:
			return DXGI_FORMAT_R16_UINT;
		case XE::GraphicsTextureFormat::R16SINT:
			return DXGI_FORMAT_R16_SINT;
		case XE::GraphicsTextureFormat::R16FLOAT:
			return DXGI_FORMAT_R16_FLOAT;
		case XE::GraphicsTextureFormat::RG8UNORM:
			return DXGI_FORMAT_R8G8_UNORM;
		case XE::GraphicsTextureFormat::RG8SNORM:
			return DXGI_FORMAT_R8G8_SNORM;
		case XE::GraphicsTextureFormat::RG8UINT:
			return DXGI_FORMAT_R8G8_UINT;
		case XE::GraphicsTextureFormat::RG8SINT:
			return DXGI_FORMAT_R8G8_SINT;
		case XE::GraphicsTextureFormat::R32FLOAT:
			return DXGI_FORMAT_R32_FLOAT;
		case XE::GraphicsTextureFormat::R32UINT:
			return DXGI_FORMAT_R32_UINT;
		case XE::GraphicsTextureFormat::R32SINT:
			return DXGI_FORMAT_R32_SINT;
		case XE::GraphicsTextureFormat::RG16UINT:
			return DXGI_FORMAT_R16G16_UINT;
		case XE::GraphicsTextureFormat::RG16SINT:
			return DXGI_FORMAT_R16G16_SINT;
		case XE::GraphicsTextureFormat::RG16FLOAT:
			return DXGI_FORMAT_R16G16_FLOAT;
		case XE::GraphicsTextureFormat::RGBA8UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case XE::GraphicsTextureFormat::RGBA8UNORMSRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case XE::GraphicsTextureFormat::RGBA8SNORM:
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case XE::GraphicsTextureFormat::RGBA8UINT:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case XE::GraphicsTextureFormat::RGBA8SINT:
			return DXGI_FORMAT_R8G8B8A8_SINT;
		case XE::GraphicsTextureFormat::BGRA8UNORM:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case XE::GraphicsTextureFormat::BGRA8UNORMSRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case XE::GraphicsTextureFormat::RGB10A2UNORM:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case XE::GraphicsTextureFormat::RG11B10UFLOAT:
			return DXGI_FORMAT_R11G11B10_FLOAT;
		case XE::GraphicsTextureFormat::RGB9E5UFLOAT:
			return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
		case XE::GraphicsTextureFormat::RG32FLOAT:
			return DXGI_FORMAT_R32G32_FLOAT;
		case XE::GraphicsTextureFormat::RG32UINT:
			return DXGI_FORMAT_R32G32_UINT;
		case XE::GraphicsTextureFormat::RG32SINT:
			return DXGI_FORMAT_R32G32_SINT;
		case XE::GraphicsTextureFormat::RGBA16UINT:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		case XE::GraphicsTextureFormat::RGBA16SINT:
			return DXGI_FORMAT_R16G16B16A16_SINT;
		case XE::GraphicsTextureFormat::RGBA16FLOAT:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case XE::GraphicsTextureFormat::RGBA32FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case XE::GraphicsTextureFormat::RGBA32UINT:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case XE::GraphicsTextureFormat::RGBA32SINT:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case XE::GraphicsTextureFormat::STENCIL8:
			return DXGI_FORMAT_R8_UINT;
		case XE::GraphicsTextureFormat::DEPTH16UNORM:
			return DXGI_FORMAT_R16_UNORM;
		case XE::GraphicsTextureFormat::DEPTH24PLUS:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case XE::GraphicsTextureFormat::DEPTH24PLUSSTENCIL8:
			return DXGI_FORMAT_R24G8_TYPELESS;
		case XE::GraphicsTextureFormat::DEPTH24UNORMSTENCIL8:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case XE::GraphicsTextureFormat::DEPTH32FLOAT:
			return DXGI_FORMAT_R32_FLOAT;
		case XE::GraphicsTextureFormat::DEPTH32FLOATSTENCIL8:
			return DXGI_FORMAT_R32G8X24_TYPELESS;
		case XE::GraphicsTextureFormat::BC1RGBAUNORM:
			return DXGI_FORMAT_BC1_UNORM;
		case XE::GraphicsTextureFormat::BC1RGBAUNORMSRGB:
			return DXGI_FORMAT_BC1_UNORM_SRGB;
		case XE::GraphicsTextureFormat::BC2RGBAUNORM:
			return DXGI_FORMAT_BC2_UNORM;
		case XE::GraphicsTextureFormat::BC2RGBAUNORMSRGB:
			return DXGI_FORMAT_BC2_UNORM_SRGB;
		case XE::GraphicsTextureFormat::BC3RGBAUNORM:
			return DXGI_FORMAT_BC3_UNORM;
		case XE::GraphicsTextureFormat::BC3RGBAUNORMSRGB:
			return DXGI_FORMAT_BC3_UNORM_SRGB;
		case XE::GraphicsTextureFormat::BC4RUNORM:
			return DXGI_FORMAT_BC4_UNORM;
		case XE::GraphicsTextureFormat::BC4RSNORM:
			return DXGI_FORMAT_BC4_SNORM;
		case XE::GraphicsTextureFormat::BC5RGUNORM:
			return DXGI_FORMAT_BC5_UNORM;
		case XE::GraphicsTextureFormat::BC5RGSNORM:
			return DXGI_FORMAT_BC5_SNORM;
		case XE::GraphicsTextureFormat::BC6HRGBUFLOAT:
			return DXGI_FORMAT_BC6H_UF16;
		case XE::GraphicsTextureFormat::BC6HRGBFLOAT:
			return DXGI_FORMAT_BC6H_SF16;
		case XE::GraphicsTextureFormat::BC7RGBAUNORM:
			return DXGI_FORMAT_BC7_UNORM;
		case XE::GraphicsTextureFormat::BC7RGBAUNORMSRGB:
			return DXGI_FORMAT_BC7_UNORM_SRGB;
		case XE::GraphicsTextureFormat::ETC2RGB8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGB8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGB8A1UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGB8A1UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGBA8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ETC2RGBA8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACR11UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACR11SNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACRG11UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::EACRG11SNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC4X4UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC4X4UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X4UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X4UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC5X5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X6UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC6X6UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X6UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X6UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC8X8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x5UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x5UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x6UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x6UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x8UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x8UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x10UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC10x10UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X10UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X10UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X12UNORM:
			return DXGI_FORMAT_UNKNOWN;
		case XE::GraphicsTextureFormat::ASTC12X12UNORMSRGB:
			return DXGI_FORMAT_UNKNOWN;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}

		return DXGI_FORMAT_UNKNOWN;
	}
	D3D12_QUERY_TYPE Cast( XE::GraphicsQueryType type )
	{
		switch ( type )
		{
		case XE::GraphicsQueryType::OCCLUSION:
			return D3D12_QUERY_TYPE_OCCLUSION;
		case XE::GraphicsQueryType::PIPELINE_STATISTICS:
			return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
		case XE::GraphicsQueryType::TIMESTAMP:
			return D3D12_QUERY_TYPE_TIMESTAMP;
		}

		return D3D12_QUERY_TYPE_OCCLUSION;
	}
	D3D12_RESOURCE_FLAGS Cast( XE::GraphicsBufferUsageFlags usage )
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

		if ( usage || XE::GraphicsBufferUsage::STORAGE )
			flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		return flags;
	}
	D3D12_RESOURCE_DIMENSION Cast( XE::GraphicsTextureDimension dim )
	{
		switch ( dim )
		{
		case XE::GraphicsTextureDimension::D1:
			return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		case XE::GraphicsTextureDimension::D2:
			return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		case XE::GraphicsTextureDimension::D3:
			return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		default:
			break;
		}

		return D3D12_RESOURCE_DIMENSION_UNKNOWN;
	}
	D3D12_BLEND_OP Cast( XE::GraphicsBlendOperation op )
	{
		switch ( op )
		{
		case XE::GraphicsBlendOperation::ADD:
			return D3D12_BLEND_OP_ADD;
		case XE::GraphicsBlendOperation::SUBTRACT:
			return D3D12_BLEND_OP_SUBTRACT;
		case XE::GraphicsBlendOperation::REVERSE_SUBTRACT:
			return D3D12_BLEND_OP_REV_SUBTRACT;
		case XE::GraphicsBlendOperation::MIN:
			return D3D12_BLEND_OP_MIN;
		case XE::GraphicsBlendOperation::MAX:
			return D3D12_BLEND_OP_MAX;
		}

		return D3D12_BLEND_OP_ADD;
	}
	D3D12_BLEND Cast( XE::GraphicsBlendFactor factor, bool is_alpha )
	{
		switch ( factor )
		{
		case XE::GraphicsBlendFactor::ZERO:
			return D3D12_BLEND_ZERO;
		case XE::GraphicsBlendFactor::ONE:
			return D3D12_BLEND_ONE;
		case XE::GraphicsBlendFactor::SRC:
			return is_alpha ? D3D12_BLEND_SRC_ALPHA : D3D12_BLEND_SRC_COLOR;
		case XE::GraphicsBlendFactor::ONE_MINUS_SRC:
			return is_alpha ? D3D12_BLEND_INV_SRC_ALPHA : D3D12_BLEND_INV_SRC_COLOR;
		case XE::GraphicsBlendFactor::SRC_ALPHA:
			return D3D12_BLEND_SRC_ALPHA;
		case XE::GraphicsBlendFactor::ONE_MINUS_SRC_ALPHA:
			return D3D12_BLEND_INV_SRC_ALPHA;
		case XE::GraphicsBlendFactor::DST:
			return is_alpha ? D3D12_BLEND_DEST_ALPHA : D3D12_BLEND_DEST_COLOR;
		case XE::GraphicsBlendFactor::ONE_MINUS_DST:
			return is_alpha ? D3D12_BLEND_INV_DEST_ALPHA : D3D12_BLEND_INV_DEST_COLOR;
		case XE::GraphicsBlendFactor::DST_ALPHA:
			return D3D12_BLEND_DEST_ALPHA;
		case XE::GraphicsBlendFactor::ONE_MINUS_DST_ALPHA:
			return D3D12_BLEND_INV_DEST_ALPHA;
		case XE::GraphicsBlendFactor::SRC_ALPHA_SATURATED:
			return D3D12_BLEND_SRC_ALPHA_SAT;
		case XE::GraphicsBlendFactor::CONSTANT:
			return D3D12_BLEND_BLEND_FACTOR;
		case XE::GraphicsBlendFactor::ONE_MINUS_CONSTANT:
			return D3D12_BLEND_INV_BLEND_FACTOR;
		}

		return D3D12_BLEND_ZERO;
	}
	D3D12_CULL_MODE Cast( XE::GraphicsCullMode mode )
	{
		switch ( mode )
		{
		case XE::GraphicsCullMode::FRONT:
			return D3D12_CULL_MODE_FRONT;
		case XE::GraphicsCullMode::BACK:
			return D3D12_CULL_MODE_BACK;
		}

		return D3D12_CULL_MODE_NONE;
	}
	bool Cast( XE::GraphicsFrontFace face )
	{
		switch ( face )
		{
		case XE::GraphicsFrontFace::CCW:
			return true;
		case XE::GraphicsFrontFace::CW:
			return false;
		}

		return false;
	}
	D3D12_COMPARISON_FUNC Cast( XE::GraphicsCompareFunction func )
	{
		switch ( func )
		{
		case XE::GraphicsCompareFunction::NEVER:
			return D3D12_COMPARISON_FUNC_NEVER;
		case XE::GraphicsCompareFunction::LESS:
			return D3D12_COMPARISON_FUNC_LESS;
		case XE::GraphicsCompareFunction::LESS_EQUAL:
			return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case XE::GraphicsCompareFunction::GREATER:
			return D3D12_COMPARISON_FUNC_GREATER;
		case XE::GraphicsCompareFunction::GREATER_EQUAL:
			return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case XE::GraphicsCompareFunction::EQUAL:
			return D3D12_COMPARISON_FUNC_EQUAL;
		case XE::GraphicsCompareFunction::NOT_EQUAL:
			return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case XE::GraphicsCompareFunction::ALWAYS:
			return D3D12_COMPARISON_FUNC_ALWAYS;
		}

		return D3D12_COMPARISON_FUNC( 0 );
	}
	D3D12_STENCIL_OP Cast( XE::GraphicsStencilOperation op )
	{
		switch ( op )
		{
		case XE::GraphicsStencilOperation::KEEP:
			return D3D12_STENCIL_OP_KEEP;
		case XE::GraphicsStencilOperation::ZERO:
			return D3D12_STENCIL_OP_ZERO;
		case XE::GraphicsStencilOperation::REPLACE:
			return D3D12_STENCIL_OP_REPLACE;
		case XE::GraphicsStencilOperation::INVERT:
			return D3D12_STENCIL_OP_INVERT;
		case XE::GraphicsStencilOperation::INCREMENT_CLAMP:
			return D3D12_STENCIL_OP_INCR_SAT;
		case XE::GraphicsStencilOperation::DECREMENT_CLAMP:
			return D3D12_STENCIL_OP_DECR_SAT;
		case XE::GraphicsStencilOperation::INCREMENT_WRAP:
			return D3D12_STENCIL_OP_INCR;
		case XE::GraphicsStencilOperation::DECREMENT_WRAP:
			return D3D12_STENCIL_OP_DECR;
		}

		return D3D12_STENCIL_OP_KEEP;
	}
	D3D12_PRIMITIVE_TOPOLOGY_TYPE Cast( XE::GraphicsPrimitiveTopology topology )
	{
		switch ( topology )
		{
		case XE::GraphicsPrimitiveTopology::POINT_LIST:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		case XE::GraphicsPrimitiveTopology::LINE_LIST:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case XE::GraphicsPrimitiveTopology::LINE_STRIP:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case XE::GraphicsPrimitiveTopology::TRIANGLE_LIST:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		case XE::GraphicsPrimitiveTopology::TRIANGLE_STRIP:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}

		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	}
	D3D12_TEXTURE_ADDRESS_MODE Cast( XE::GraphicsAddressMode mode )
	{
		switch ( mode )
		{
		case XE::GraphicsAddressMode::REPEAT:
			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		case XE::GraphicsAddressMode::MIRROR_REPEAT:
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
		case XE::GraphicsAddressMode::CLAMP_TO_EDGE:
			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}

		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	}
	D3D12_FILTER_TYPE Cast( XE::GraphicsFilterMode mode )
	{
		switch ( mode )
		{
		case XE::GraphicsFilterMode::NEAREST:
			return D3D12_FILTER_TYPE_POINT;
		case XE::GraphicsFilterMode::LINEAR:
			return D3D12_FILTER_TYPE_LINEAR;
		}

		return D3D12_FILTER_TYPE_POINT;
	}
	D3D12_FILTER_TYPE Cast( XE::GraphicsMipmapFilterMode mode )
	{
		switch ( mode )
		{
		case XE::GraphicsMipmapFilterMode::NEAREST:
			return D3D12_FILTER_TYPE_POINT;
		case XE::GraphicsMipmapFilterMode::LINEAR:
			return D3D12_FILTER_TYPE_LINEAR;
		}

		return D3D12_FILTER_TYPE_POINT;
	}

	Microsoft::WRL::ComPtr< ID3DBlob > LoadShader( const XE::GraphicsShaderModuleDescriptor & shader_desc, const XE::String & entry_point, XE::GraphicsShaderStage stage )
	{
		for ( const auto & ep : shader_desc.Hints )
		{
			if ( ep.EntryPoint == entry_point )
			{
				XE::uint32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
				if ( shader_desc.Debug )
				{
					flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
				}

				const char * target = nullptr;
				switch ( stage )
				{
				case XE::GraphicsShaderStage::VERTEX:
					target = "vs_5_1";
					break;
				case XE::GraphicsShaderStage::FRAGMENT:
					target = "ps_5_1";
					break;
				case XE::GraphicsShaderStage::COMPUTE:
					target = "cs_5_1";
					break;
				default:
					return nullptr;
				}

				Microsoft::WRL::ComPtr< ID3DBlob > code = nullptr;
				Microsoft::WRL::ComPtr< ID3DBlob > errmsg = nullptr;

				if ( SUCCEEDED( ::D3DCompile( shader_desc.Code.data(), shader_desc.Code.size(), shader_desc.Label.c_str(), nullptr, nullptr, ep.EntryPoint.c_str(), target, flags, 0, code.GetAddressOf(), errmsg.GetAddressOf() ) ) )
				{
					return code;
				}
			}
		}

		return nullptr;
	}

	template< typename T > using ComPtr = Microsoft::WRL::ComPtr< T >;
	using D3D12BlobPtr = ComPtr< ID3DBlob >;
	using D3D12AdapterPtr = ComPtr < IDXGIAdapter4 >;
	using D3D12DevicePtr = ComPtr < ID3D12Device6 >;
	using D3D12FencePtr = ComPtr< ID3D12Fence >;
	using D3D12ResourcePtr = ComPtr < ID3D12Resource >;
	using D3D12DescriptorHeapPtr = ComPtr< ID3D12DescriptorHeap >;
	using D3D12CommandQueuePtr = ComPtr < ID3D12CommandQueue >;
	using D3D12CommandAllocatorPtr = ComPtr < ID3D12CommandAllocator >;
	using D3D12CommandListPtr = ComPtr < ID3D12GraphicsCommandList >;
	using D3D12GraphicsCommandListPtr = ComPtr < ID3D12GraphicsCommandList >;
	using D3D12PipelineStatePtr = ComPtr < ID3D12PipelineState >;
	using D3D12SwapChainPtr = ComPtr < IDXGISwapChain4 >;
	using D3D12CommandSignaturePtr = ComPtr< ID3D12CommandSignature >;
	using D3D12RootSignaturePtr = ComPtr< ID3D12RootSignature >;
	using D3D12QueryHeapPtr = ComPtr< ID3D12QueryHeap >;
	using D3D12FactoryPtr = ComPtr< IDXGIFactory6 >;
	using D3D12DebugPtr = ComPtr< ID3D12Debug >;
}

namespace XE
{
	class GraphicsSurface : public XE::EnableSharedFromThis< GraphicsSurface >
	{
	public:
		XE::GraphicsSurfaceDescriptor Desc;

		XE::GraphicsSwapChainPtr SwapChain;
	};
	class GraphicsSwapChain : public XE::EnableSharedFromThis< GraphicsSwapChain >
	{
	public:
		XE::GraphicsSwapChainDescriptor Desc;

		D3D12SwapChainPtr Raw = nullptr;

		std::array< XE::GraphicsTexturePtr, 2 > Textures;
		std::array< XE::GraphicsTextureViewPtr, 2 > TextureViews;

		XE::GraphicsSurfacePtr Parent;
	};
	class GraphicsAdapter : public XE::EnableSharedFromThis< GraphicsAdapter >
	{
	public:
		XE::uint64 Desc = 0;

		D3D12AdapterPtr Raw = nullptr;

		XE::GraphicsDevicePtr Device;
	};
	class GraphicsDevice : public XE::EnableSharedFromThis< GraphicsDevice >
	{
	public:
		XE::GraphicsDeviceDescriptor Desc;

		D3D12DevicePtr Raw = nullptr;

		XE::uint32 CSUItemSize = 0;
		XE::uint32 RTVItemSize = 0;
		XE::uint32 DSVItemSize = 0;
		XE::uint32 SmaplerItemSize = 0;

		D3D12DescriptorHeapPtr CSUHeap;
		D3D12DescriptorHeapPtr RTVHeap;
		D3D12DescriptorHeapPtr DSVHeap;
		D3D12DescriptorHeapPtr SmaplerHeap;

		XE::QueueHandleAllocator< XE::Handle< int >, GRAPHICS_MAX_CSU_DESCRIPTOR_HEAP > CSUAllocator;
		XE::QueueHandleAllocator< XE::Handle< int >, GRAPHICS_MAX_RTV_DESCRIPTOR_HEAP > RTVAllocator;
		XE::QueueHandleAllocator< XE::Handle< int >, GRAPHICS_MAX_DSV_DESCRIPTOR_HEAP > DSVAllocator;
		XE::QueueHandleAllocator< XE::Handle< int >, GRAPHICS_MAX_SAMPLER_DESCRIPTOR_HEAP > SamplerAllocator;

		D3D12CommandSignaturePtr DrawIndirectSignature;
		D3D12CommandSignaturePtr DispatchIndirectSignature;
		D3D12CommandSignaturePtr DrawIndexedIndirectSignature;

		XE::ConcurrentQueue<D3D12CommandListPtr> FreeBufferLists;
		XE::ConcurrentQueue<D3D12CommandAllocatorPtr> FreeAllocators;

		XE::GraphicsService::ErrorCallback ErrorCallback;
		XE::GraphicsService::DeviceLostCallback LostCallback;

		XE::GraphicsQueue * Queue = nullptr;
		XE::GraphicsAdapterPtr Parent = nullptr;
	};
	class GraphicsQueue : public XE::EnableSharedFromThis< GraphicsQueue >
	{
	public:
		XE::GraphicsQueueDescriptor Desc;

		D3D12CommandQueuePtr Raw = nullptr;

		HANDLE Event = NULL;
		D3D12FencePtr Fence = nullptr;
		XE::ConcurrentQueue<D3D12CommandListPtr> TakeBufferLists;
		XE::ConcurrentQueue<D3D12CommandAllocatorPtr> TakeAllocators;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsBindGroup : public XE::EnableSharedFromThis< GraphicsBindGroup >
	{
	public:
		XE::GraphicsBindGroupDescriptor Desc;

		D3D12DescriptorHeapPtr ViewHeap = nullptr;
		D3D12DescriptorHeapPtr SamplerHeap = nullptr;
		D3D12RootSignaturePtr RootSignature = nullptr;
		D3D12CommandSignaturePtr CommandSignature = nullptr;

		XE::ConcurrentQueue<D3D12_GPU_VIRTUAL_ADDRESS> DynamicBuffers;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsBindGroupLayout : public XE::EnableSharedFromThis< GraphicsBindGroupLayout >
	{
	public:
		XE::GraphicsBindGroupLayoutDescriptor Desc;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsBuffer : public XE::EnableSharedFromThis< GraphicsBuffer >
	{
	public:
		XE::GraphicsBufferDescriptor Desc;

		D3D12ResourcePtr Raw = nullptr;

		D3D12_RANGE MapRange = {};
		XE::Array<XE::uint8> _MapData;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsCommandBuffer : public XE::EnableSharedFromThis< GraphicsCommandBuffer >
	{
	public:
		XE::GraphicsCommandBufferDescriptor Desc;

		D3D12GraphicsCommandListPtr Raw = nullptr;

		XE::GraphicsCommandEncoderPtr Parent;
	};
	class GraphicsCommandEncoder : public XE::EnableSharedFromThis< GraphicsCommandEncoder >
	{
	public:
		XE::GraphicsCommandEncoderDescriptor Desc;
		
		D3D12CommandListPtr Raw;

		D3D12CommandAllocatorPtr Allocator;

		XE::GraphicsQueuePtr Parent;
	};
	class GraphicsComputePassEncoder : public XE::EnableSharedFromThis< GraphicsComputePassEncoder >
	{
	public:
		struct BeginPipelineStatisticsQuery { XE::uint32 query_index = 0; XE::GraphicsQuerySetPtr query_set; };
		struct Dispatch { XE::uint32 workgroup_count_x = 0; XE::uint32 workgroup_count_y = 0; XE::uint32 workgroup_count_z = 0; };
		struct DispatchIndirect { XE::GraphicsBufferPtr indirect_buffer; XE::uint64 indirect_offset = 0; };
		struct EndPipelineStatisticsQuery { };
		struct InsertDebugMarker { XE::String marker_label; };
		struct PopDebugGroup { };
		struct PushDebugGroup { XE::String group_label; };
		struct SetBindGroup { XE::uint32 group_index = 0; XE::GraphicsBindGroupPtr group; XE::Array< XE::uint32 > dynamic_offsets; };
		struct SetPipeline { XE::GraphicsComputePipelinePtr pipeline; };
		using Info = std::variant< std::monostate, BeginPipelineStatisticsQuery, Dispatch, DispatchIndirect, EndPipelineStatisticsQuery, InsertDebugMarker, PopDebugGroup, PushDebugGroup, SetBindGroup, SetPipeline >;

	public:
		XE::GraphicsComputePassDescriptor Desc;

		XE::Queue< Info > CommandInfos;

		XE::GraphicsCommandEncoderPtr Parent;
	};
	class GraphicsComputePipeline : public XE::EnableSharedFromThis< GraphicsComputePipeline >
	{
	public:
		XE::GraphicsComputePipelineDescriptor Desc;

		D3D12PipelineStatePtr Raw = nullptr;

		D3D12BlobPtr ShaderCode = nullptr;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsPipelineLayout : public XE::EnableSharedFromThis< GraphicsPipelineLayout >
	{
	public:
		XE::GraphicsPipelineLayoutDescriptor Desc;

		D3D12RootSignaturePtr Raw = nullptr;
		D3D12BlobPtr BlobRootSignature;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsQuerySet : public XE::EnableSharedFromThis< GraphicsQuerySet >
	{
	public:
		XE::GraphicsQuerySetDescriptor Desc;

		D3D12QueryHeapPtr Raw = nullptr;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsRenderBundle : public XE::EnableSharedFromThis< GraphicsRenderBundle >
	{
	public:
		XE::GraphicsRenderBundleDescriptor Desc;

		D3D12CommandAllocatorPtr CommandAllocator;
		D3D12CommandListPtr Raw;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsRenderBundleEncoder : public XE::EnableSharedFromThis< GraphicsRenderBundleEncoder >
	{
	public:
		struct BeginOcclusionQuery { XE::GraphicsQuerySetPtr query_set; XE::uint32 query_index = 0; };
		struct BeginPipelineStatisticsQuery { XE::GraphicsQuerySetPtr query_set; XE::uint32 query_index = 0; };
		struct Draw { XE::uint32 vertex_count; XE::uint32 instance_count; XE::uint32 first_vertex; XE::uint32 first_instance; };
		struct DrawIndexed { XE::uint32 index_count; XE::uint32 instance_count; XE::uint32 first_index; XE::int32 base_vertex; XE::uint32 first_instance; };
		struct DrawIndexedIndirect { XE::GraphicsBufferPtr indirect_buffer; XE::uint64 indirect_offset = 0; };
		struct DrawIndirect { XE::GraphicsBufferPtr indirect_buffer; XE::uint64 indirect_offset = 0; };
		struct InsertDebugMarker { XE::String marker_label; };
		struct PopDebugGroup {};
		struct PushDebugGroup { XE::String group_label; };
		struct SetBindGroup { XE::uint32 group_index = 0; XE::GraphicsBindGroupPtr group; XE::Array< XE::uint32 > dynamic_offsets; };
		struct SetIndexBuffer { XE::GraphicsBufferPtr buffer; XE::GraphicsIndexFormat format = GraphicsIndexFormat::UINT16; XE::uint64 offset = 0; XE::uint64 size = 0; };
		struct SetPipeline { XE::GraphicsRenderPipelinePtr pipeline; };
		struct SetVertexBuffer { XE::uint32 slot = 0; XE::GraphicsBufferPtr buffer; XE::uint64 offset = 0; XE::uint64 size = 0; };
		using Info = std::variant< std::monostate, BeginOcclusionQuery, BeginPipelineStatisticsQuery, Draw, DrawIndexed, DrawIndexedIndirect, DrawIndirect, InsertDebugMarker, PopDebugGroup, PushDebugGroup, SetBindGroup, SetIndexBuffer, SetPipeline, SetVertexBuffer >;

	public:
		XE::GraphicsRenderBundleEncoderDescriptor Desc;

		XE::Queue< Info > CommandInfos;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsRenderPassEncoder : public XE::EnableSharedFromThis< GraphicsRenderPassEncoder >
	{
	public:
		struct BeginOcclusionQuery { XE::GraphicsQuerySetPtr query_set; XE::uint32 query_index = 0; };
		struct BeginPipelineStatisticsQuery { XE::GraphicsQuerySetPtr query_set; XE::uint32 query_index = 0; };
		struct Draw { XE::uint32 vertex_count; XE::uint32 instance_count; XE::uint32 first_vertex; XE::uint32 first_instance; };
		struct DrawIndexed { XE::uint32 index_count; XE::uint32 instance_count; XE::uint32 first_index; XE::int32 base_vertex; XE::uint32 first_instance; };
		struct DrawIndexedIndirect { XE::GraphicsBufferPtr indirect_buffer; XE::uint64 indirect_offset = 0; };
		struct DrawIndirect { XE::GraphicsBufferPtr indirect_buffer; XE::uint64 indirect_offset = 0; };
		struct EndOcclusionQuery {};
		struct EndPipelineStatisticsQuery {};
		struct ExecuteBundles { XE::Array< XE::GraphicsRenderBundlePtr > bundles; };
		struct InsertDebugMarker { XE::String marker_label; };
		struct PopDebugGroup {};
		struct PushDebugGroup { XE::String group_label; };
		struct SetBindGroup { XE::uint32 group_index = 0; XE::GraphicsBindGroupPtr group; XE::Array< XE::uint32 > dynamic_offsets; };
		struct SetBlendConstant { XE::Color color; };
		struct SetIndexBuffer { XE::GraphicsBufferPtr buffer; XE::GraphicsIndexFormat format = GraphicsIndexFormat::UINT16; XE::uint64 offset = 0; XE::uint64 size = 0; };
		struct SetPipeline { XE::GraphicsRenderPipelinePtr pipeline; };
		struct SetScissorRect { D3D12_RECT rect; };
		struct SetStencilReference { XE::uint32 reference = 0; };
		struct SetVertexBuffer { XE::uint32 slot = 0; XE::GraphicsBufferPtr buffer; XE::uint64 offset = 0; XE::uint64 size = 0; };
		struct SetViewport { D3D12_VIEWPORT viewport; };
		using Info = std::variant< std::monostate, BeginOcclusionQuery, BeginPipelineStatisticsQuery, Draw, DrawIndexed, DrawIndexedIndirect, DrawIndirect, EndOcclusionQuery, EndPipelineStatisticsQuery, ExecuteBundles, InsertDebugMarker, PopDebugGroup, PushDebugGroup, SetBindGroup, SetBlendConstant, SetIndexBuffer, SetPipeline, SetScissorRect, SetStencilReference, SetVertexBuffer, SetViewport >;

	public:
		XE::GraphicsRenderPassDescriptor Desc;

		XE::Queue< Info > CommandInfos;

		XE::GraphicsCommandEncoderPtr Parent;
	};
	class GraphicsRenderPipeline : public XE::EnableSharedFromThis< GraphicsRenderPipeline >
	{
	public:
		XE::GraphicsRenderPipelineDescriptor Desc;

		D3D12PipelineStatePtr Raw = nullptr;

		D3D12BlobPtr VSCode = nullptr;
		D3D12BlobPtr FSCode = nullptr;
		XE::Array<D3D12_INPUT_ELEMENT_DESC> Elements;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsSampler : public XE::EnableSharedFromThis< GraphicsSampler >
	{
	public:
		XE::GraphicsSamplerDescriptor Desc;

		XE::Handle< int > SamplerHeapHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle = { 0 };

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsShaderModule : public XE::EnableSharedFromThis< GraphicsShaderModule >
	{
	public:
		XE::GraphicsShaderModuleDescriptor Desc;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsTexture : public XE::EnableSharedFromThis< GraphicsTexture >
	{
	public:
		XE::GraphicsTextureDescriptor Desc;

		D3D12ResourcePtr Raw;

		XE::GraphicsDevicePtr Parent;
	};
	class GraphicsTextureView : public XE::EnableSharedFromThis< GraphicsTextureView >
	{
	public:
		XE::GraphicsTextureViewDescriptor Desc;

		XE::Handle< int > SRVHandle;
		XE::Handle< int > UAVHandle;
		XE::Handle< int > RTVHandle;
		XE::Handle< int > DSVHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE SRVCPUHandle = { 0 };
		D3D12_CPU_DESCRIPTOR_HANDLE UAVCPUHandle = { 0 };
		D3D12_CPU_DESCRIPTOR_HANDLE RTVCPUHandle = { 0 };
		D3D12_CPU_DESCRIPTOR_HANDLE DSVCPUHandle = { 0 };

		XE::GraphicsTexturePtr Parent;
	};
}

struct XE::GraphicsServiceD3D12::Private
{
	bool _IsDebug = false;
	D3D12DebugPtr _Debug;
	D3D12FactoryPtr _Factory;

	template< typename T, XE::uint64 S > class QueueArray
	{
	public:
		using iterator = std::array< T, S >::iterator;

	public:
		XE::SharedPtr< T > Alloc()
		{
			auto handle = _HandleQueue.Alloc();

			return XE::SharedPtr< T >( &_Datas[handle.GetValue()], [this]( T * val ) { Free( val ); } );
		}

		void Free( T * val )
		{
			val->~T();

			auto handle = XE::HandleCast< T >( ( reinterpret_cast<XE::uint8 *>( val ) - reinterpret_cast<XE::uint8 *>( _Datas.data() ) ) / sizeof( T ) );

			auto p = new ( val ) T();

			_HandleQueue.Free( handle );
		}

	public:
		iterator begin()
		{
			return _Datas.begin();
		}

		iterator end()
		{
			return _Datas.end();
		}

	public:
		std::array< T, S > _Datas;
		XE::QueueHandleAllocator< XE::Handle< T >, S > _HandleQueue;
	};

	QueueArray< XE::GraphicsSurface, XE::GRAPHICS_MAX_SURFACE > _Surfaces;
	QueueArray< XE::GraphicsSwapChain, XE::GRAPHICS_MAX_SWAP_CHAIN > _SwapChains;
	QueueArray< XE::GraphicsAdapter, XE::GRAPHICS_MAX_ADAPTER > _Adapters;
	QueueArray< XE::GraphicsDevice, XE::GRAPHICS_MAX_DEVICE > _Devices;
	QueueArray< XE::GraphicsQueue, XE::GRAPHICS_MAX_QUEUE > _Queues;
	QueueArray< XE::GraphicsBindGroup, XE::GRAPHICS_MAX_BIND_GROUP > _BindGroups;
	QueueArray< XE::GraphicsBindGroupLayout, XE::GRAPHICS_MAX_BIND_GROUP_LAYOUT > _BindGroupLayouts;
	QueueArray< XE::GraphicsBuffer, XE::GRAPHICS_MAX_BUFFER > _Buffers;
	QueueArray< XE::GraphicsCommandBuffer, XE::GRAPHICS_MAX_COMMAND_BUFFER > _CommandBuffers;
	QueueArray< XE::GraphicsCommandEncoder, XE::GRAPHICS_MAX_COMMAND_ENCODER > _CommandEncoders;
	QueueArray< XE::GraphicsComputePassEncoder, XE::GRAPHICS_MAX_COMPUTE_PASS_ENCODER > _ComputePassEncoders;
	QueueArray< XE::GraphicsComputePipeline, XE::GRAPHICS_MAX_COMPUTE_PIPELINE > _ComputePipelines;
	QueueArray< XE::GraphicsPipelineLayout, XE::GRAPHICS_MAX_PIPELINE_LAYOUT > _PipelineLayouts;
	QueueArray< XE::GraphicsQuerySet, XE::GRAPHICS_MAX_QUERY_SET > _QuerySets;
	QueueArray< XE::GraphicsRenderBundle, XE::GRAPHICS_MAX_RENDER_BUNDLE > _RenderBundles;
	QueueArray< XE::GraphicsRenderBundleEncoder, XE::GRAPHICS_MAX_RENDER_BUNDLE_ENCODER > _RenderBundleEncoders;
	QueueArray< XE::GraphicsRenderPassEncoder, XE::GRAPHICS_MAX_RENDER_PASS_ENCODER > _RenderPassEncoders;
	QueueArray< XE::GraphicsRenderPipeline, XE::GRAPHICS_MAX_RENDER_PIPELINE > _RenderPipelines;
	QueueArray< XE::GraphicsShaderModule, XE::GRAPHICS_MAX_SHADER_MODULE > _ShaderModules;
	QueueArray< XE::GraphicsSampler, XE::GRAPHICS_MAX_SAMPLER > _Samplers;
	QueueArray< XE::GraphicsTexture, XE::GRAPHICS_MAX_TEXTURE > _Textures;
	QueueArray< XE::GraphicsTextureView, XE::GRAPHICS_MAX_TEXTURE_VIEW > _TextureViews;
};

XE::GraphicsServiceD3D12::GraphicsServiceD3D12( bool debug /*= false*/ )
	:_p( XE::New< Private >() )
{
	_p->_IsDebug = debug;
}

XE::GraphicsServiceD3D12::~GraphicsServiceD3D12()
{
	XE::Delete( _p );
}

void XE::GraphicsServiceD3D12::Prepare()
{
	XE::uint32 flags = 0;

	if ( _p->_IsDebug )
	{
		if ( SUCCEEDED( ::D3D12GetDebugInterface( IID_PPV_ARGS( _p->_Debug.GetAddressOf() ) ) ) )
		{
			_p->_Debug->EnableDebugLayer();
			flags = DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	if ( FAILED( ::CreateDXGIFactory2( flags, IID_PPV_ARGS( _p->_Factory.GetAddressOf() ) ) ) )
	{
		XE_ERROR( "Create DXGIFactory failed" );
	}
}

void XE::GraphicsServiceD3D12::Startup()
{

}

void XE::GraphicsServiceD3D12::Update()
{

}

void XE::GraphicsServiceD3D12::Clearup()
{
	_p->_Factory = nullptr;
	_p->_Debug = nullptr;
}

XE::GraphicsSurfacePtr XE::GraphicsServiceD3D12::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	auto surface = _p->_Surfaces.Alloc();

	surface->Desc = descriptor;

	return surface;
}

void XE::GraphicsServiceD3D12::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, XE::GraphicsService::RequestAdapterCallback callback )
{
	D3D12AdapterPtr adapter;

	DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE_UNSPECIFIED;
	switch ( options.PowerPreference )
	{
	case XE::GraphicsPowerPreference::LOW_POWER:
		preference = DXGI_GPU_PREFERENCE_MINIMUM_POWER;
		break;
	case XE::GraphicsPowerPreference::HIGH_PERFORMANCE:
		preference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
		break;
	default:
		preference = DXGI_GPU_PREFERENCE_UNSPECIFIED;
		break;
	}

	for ( size_t i = 0; _p->_Factory->EnumAdapterByGpuPreference( i, preference, IID_PPV_ARGS( adapter.GetAddressOf() ) ) != DXGI_ERROR_NOT_FOUND; i++ )
	{
		auto ada = _p->_Adapters.Alloc();

		ada->Raw = adapter;
		ada->Desc = i;

		callback( XE::GraphicsRequestAdapterStatus::SUCCESS, ada );
	}
}

void XE::GraphicsServiceD3D12::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{
	features.push_back( XE::GraphicsFeatureName::DEPTH_CLIP_CONTROL );
	features.push_back( XE::GraphicsFeatureName::DEPTH24UNORM_STENCIL8 );
	features.push_back( XE::GraphicsFeatureName::DEPTH32FLOAT_STENCIL8 );
	features.push_back( XE::GraphicsFeatureName::INDIRECT_FIRST_INSTANCE );
	features.push_back( XE::GraphicsFeatureName::TIMESTAMP_QUERY );
	features.push_back( XE::GraphicsFeatureName::PIPELINE_STATISTICS_QUERY );
	features.push_back( XE::GraphicsFeatureName::TEXTURE_COMPRESSION_BC );
}

bool XE::GraphicsServiceD3D12::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	limits.Limits.MaxTextureDimension1D = D3D12_REQ_TEXTURE1D_U_DIMENSION;
	limits.Limits.MaxTextureDimension2D = D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	limits.Limits.MaxTextureDimension3D = D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
	limits.Limits.MaxTextureArrayLayers = D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
	limits.Limits.MaxBindGroups = 4;
	limits.Limits.MaxDynamicUniformBuffersPerPipelineLayout = 8;
	limits.Limits.MaxDynamicStorageBuffersPerPipelineLayout = 4;
	limits.Limits.MaxSampledTexturesPerShaderStage = 16;
	limits.Limits.MaxSamplersPerShaderStage = D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE;
	limits.Limits.MaxStorageBuffersPerShaderStage = 8;
	limits.Limits.MaxStorageTexturesPerShaderStage = 8;
	limits.Limits.MaxUniformBuffersPerShaderStage = 12;
	limits.Limits.MaxUniformBufferBindingSize = D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * 16;
	limits.Limits.MaxStorageBufferBindingSize = 128 << 20;
	limits.Limits.MinUniformBufferOffsetAlignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	limits.Limits.MinStorageBufferOffsetAlignment = 4;
	limits.Limits.MaxVertexBuffers = D3D12_VS_INPUT_REGISTER_COUNT;
	limits.Limits.MaxVertexAttributes = D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
	limits.Limits.MaxVertexBufferArrayStride = D3D12_SO_BUFFER_MAX_STRIDE_IN_BYTES;
	limits.Limits.MaxInterStageShaderComponents = 60;
	limits.Limits.MaxComputeWorkgroupStorageSize = 16352;
	limits.Limits.MaxComputeInvocationsPerWorkgroup = D3D12_CS_4_X_THREAD_GROUP_MAX_THREADS_PER_GROUP;
	limits.Limits.MaxComputeWorkgroupSizeX = D3D12_CS_THREAD_GROUP_MAX_X;
	limits.Limits.MaxComputeWorkgroupSizeY = D3D12_CS_THREAD_GROUP_MAX_Y;
	limits.Limits.MaxComputeWorkgroupSizeZ = D3D12_CS_THREAD_GROUP_MAX_Z;
	limits.Limits.MaxComputeWorkgroupsPerDimension = D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION;

	return true;
}

void XE::GraphicsServiceD3D12::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{
	DXGI_ADAPTER_DESC1 desc1;
	if ( SUCCEEDED( adapter->Raw->GetDesc1( &desc1 ) ) )
	{
		properties.Name = XE::WideString( desc1.Description );
		properties.DriverDescription = properties.Name;
		properties.DeviceID = desc1.DeviceId;
		properties.VendorID = desc1.VendorId;
		properties.BackendType = XE::GraphicsBackendType::D3D12;

		if ( desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE )
		{
			properties.AdapterType = GraphicsAdapterType::SOFTWARE_CPU;
		}
		else
		{
			D3D12_FEATURE_DATA_ARCHITECTURE arch = {};
			if ( adapter->Device && SUCCEEDED( adapter->Device->Raw->CheckFeatureSupport( D3D12_FEATURE_ARCHITECTURE, &arch, sizeof( arch ) ) ) )
			{
				properties.AdapterType = arch.UMA ? XE::GraphicsAdapterType::INTEGRATED_GPU : XE::GraphicsAdapterType::DISCRETE_GPU;
			}
			else
			{
				properties.AdapterType = XE::GraphicsAdapterType::UNKNOWN;
			}
		}
	}
}

bool XE::GraphicsServiceD3D12::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	XE::Array< XE::GraphicsFeatureName > features;
	AdapterEnumerateFeatures( adapter, features );
	return std::find( features.begin(), features.end(), feature ) != features.end();
}

void XE::GraphicsServiceD3D12::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, XE::GraphicsService::RequestDeviceCallback callback )
{
	std::array< D3D_FEATURE_LEVEL, 4 > levels =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	for ( size_t i = 0; i < levels.size(); i++ )
	{
		D3D12DevicePtr device;
		if ( SUCCEEDED( ::D3D12CreateDevice( adapter->Raw.Get(), levels[i], IID_PPV_ARGS( device.GetAddressOf() ) ) ) )
		{
			auto dev = _p->_Devices.Alloc();

			dev->Raw = device;
			dev->Parent = adapter;
			adapter->Device = dev;

			D3D12_COMMAND_QUEUE_DESC queue_desc = {};
			{
				queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				D3D12CommandQueuePtr command_queue;
				if ( SUCCEEDED( device->CreateCommandQueue( &queue_desc, IID_PPV_ARGS( command_queue.GetAddressOf() ) ) ) )
				{
					auto que = _p->_Queues.Alloc();
					que->Raw = command_queue;
					que->Parent = dev;
					que->Desc = descriptor.DefaultQueue;
				}
			}

			D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
			{
				desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc_heap_desc.NumDescriptors = GRAPHICS_MAX_CSU_DESCRIPTOR_HEAP;
				desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc_heap_desc.NodeMask = 0;
				device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev->CSUHeap.GetAddressOf() ) );
				dev->CSUItemSize = device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

				desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				desc_heap_desc.NumDescriptors = GRAPHICS_MAX_RTV_DESCRIPTOR_HEAP;
				desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc_heap_desc.NodeMask = 0;
				device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev->RTVHeap.GetAddressOf() ) );
				dev->RTVItemSize = device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );

				desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				desc_heap_desc.NumDescriptors = GRAPHICS_MAX_DSV_DESCRIPTOR_HEAP;
				desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc_heap_desc.NodeMask = 0;
				device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev->DSVHeap.GetAddressOf() ) );
				dev->DSVItemSize = device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_DSV );

				desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
				desc_heap_desc.NumDescriptors = GRAPHICS_MAX_SAMPLER_DESCRIPTOR_HEAP;
				desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc_heap_desc.NodeMask = 0;
				device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev->SmaplerHeap.GetAddressOf() ) );
				dev->SmaplerItemSize = device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );
			}

			D3D12_COMMAND_SIGNATURE_DESC signature_desc = {};
			{
				D3D12_INDIRECT_ARGUMENT_DESC argument_desc = {};
				signature_desc.NumArgumentDescs = 1;
				signature_desc.pArgumentDescs = &argument_desc;

				argument_desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
				signature_desc.ByteStride = 4 * sizeof( uint32_t );
				device->CreateCommandSignature( &signature_desc, nullptr, IID_PPV_ARGS( dev->DrawIndirectSignature.GetAddressOf() ) );

				argument_desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
				signature_desc.ByteStride = 3 * sizeof( uint32_t );
				device->CreateCommandSignature( &signature_desc, nullptr, IID_PPV_ARGS( dev->DispatchIndirectSignature.GetAddressOf() ) );

				argument_desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
				signature_desc.ByteStride = 5 * sizeof( uint32_t );
				device->CreateCommandSignature( &signature_desc, nullptr, IID_PPV_ARGS( dev->DrawIndexedIndirectSignature.GetAddressOf() ) );
			}

			for ( size_t i = 0; i < GRAPHICS_MAX_COMMAND_ALLOCATOR; i++ )
			{
				D3D12CommandAllocatorPtr allocator;
				if ( SUCCEEDED( device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( allocator.GetAddressOf() ) ) ) )
				{
					dev->FreeAllocators.push( allocator );
				}
			}

			break;
		}
	}

	if ( adapter->Device )
	{
		callback( XE::GraphicsRequestDeviceStatus::SUCCESS, adapter->Device );
	}
	else
	{
		callback( XE::GraphicsRequestDeviceStatus::ERROR, adapter->Device );
	}
}

XE::GraphicsBindGroupPtr XE::GraphicsServiceD3D12::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	auto group = _p->_BindGroups.Alloc();

	group->Desc = descriptor;
	{
		XE::uint32 sampler_count = 0;
		XE::uint32 buffer_view_count = 0;
		XE::uint32 texture_view_count = 0;
		for ( const auto & entry : descriptor.Entries )
		{
			if ( entry.Buffer )
			{
				buffer_view_count += entry.Size;
			}
			else if ( entry.Sampler )
			{
				sampler_count += entry.Size;
			}
			else if ( entry.TextureView )
			{
				texture_view_count += entry.Size;
			}
		}
		if ( buffer_view_count + texture_view_count != 0 )
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			{
				desc.NumDescriptors = buffer_view_count + texture_view_count;
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc.NodeMask = 0;
			}

			device->Raw->CreateDescriptorHeap( &desc, IID_PPV_ARGS( group->ViewHeap.GetAddressOf() ) );
		}
		if ( sampler_count != 0 )
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			{
				desc.NumDescriptors = buffer_view_count + texture_view_count;
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc.NodeMask = 0;
			}

			device->Raw->CreateDescriptorHeap( &desc, IID_PPV_ARGS( group->SamplerHeap.GetAddressOf() ) );
		}
	}

	XE::uint32 view_count = 0;
	XE::uint32 sampler_count = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE view_handle = group->ViewHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE sampler_handle = group->SamplerHeap->GetCPUDescriptorHandleForHeapStart();
	XE::uint64 view_item_size = device->Raw->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
	XE::uint64 sampler_item_size = device->Raw->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );

	for ( size_t i = 0; i < descriptor.Entries.size(); i++ )
	{
		const auto & group_entry = descriptor.Entries[i];
		const auto & layout_entry = descriptor.Layout->Desc.Entries[i];

		if ( group_entry.Buffer )
		{
			if ( group_entry.Buffer )
			{
				D3D12_GPU_VIRTUAL_ADDRESS gpu_address = group_entry.Buffer->Raw->GetGPUVirtualAddress();
				D3D12_CPU_DESCRIPTOR_HANDLE cpu_address = { view_handle.ptr + ( view_count++ * view_item_size ) };

				if ( layout_entry.Buffer.HasDynamicOffset )
				{
					group->DynamicBuffers.push( gpu_address + group_entry.Offset );
				}
				else
				{
					switch ( layout_entry.Buffer.Type )
					{
					case XE::GraphicsBufferBindingType::UNIFORM:
					{
						D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
						{
							desc.SizeInBytes = ( ( group_entry.Size - 1 ) | ( D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1 ) ) - 1;
							desc.BufferLocation = gpu_address;
						}

						device->Raw->CreateConstantBufferView( &desc, cpu_address );
					}
					break;
					case XE::GraphicsBufferBindingType::STORAGE:
					{
						D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
						{
							desc.Format = DXGI_FORMAT_R32_TYPELESS;
							desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
							desc.Buffer.FirstElement = group_entry.Offset / 4;
							desc.Buffer.NumElements = group_entry.Size / 4;
							desc.Buffer.StructureByteStride = 0;
							desc.Buffer.CounterOffsetInBytes = 0;
							desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
						}

						device->Raw->CreateUnorderedAccessView( group_entry.Buffer->Raw.Get(), nullptr, &desc, cpu_address );
					}
					break;
					case XE::GraphicsBufferBindingType::READ_ONLY_STORAGE:
					{
						D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
						{
							desc.Format = DXGI_FORMAT_R32_TYPELESS;
							desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
							desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
							desc.Buffer.FirstElement = group_entry.Offset / 4;
							desc.Buffer.NumElements = group_entry.Size / 4;
							desc.Buffer.StructureByteStride = 0;
							desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;

							device->Raw->CreateShaderResourceView( group_entry.Buffer->Raw.Get(), &desc, cpu_address );
						}
					}
					break;
					default:
						break;
					}
				}
			}
			else if ( group_entry.Sampler )
			{
				D3D12_CPU_DESCRIPTOR_HANDLE cpu_address = { sampler_handle.ptr + ( sampler_count++ * sampler_item_size ) };

				device->Raw->CopyDescriptorsSimple( 1, cpu_address, group_entry.Sampler->CPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );
			}
			else if ( group_entry.TextureView )
			{
				D3D12_CPU_DESCRIPTOR_HANDLE cpu_address = { view_handle.ptr + ( view_count++ * view_item_size ) };

				if ( layout_entry.StorageTexture.Access == XE::GraphicsStorageTextureAccess::UNDEFINED )
				{
					device->Raw->CopyDescriptorsSimple( 1, cpu_address, group_entry.TextureView->SRVCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
				}
				else
				{
					device->Raw->CopyDescriptorsSimple( 1, cpu_address, group_entry.TextureView->UAVCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
				}
			}
		}
	}

	return group;
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceD3D12::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	auto layout = _p->_BindGroupLayouts.Alloc();

	layout->Desc = descriptor;
	layout->Parent = device;

	return layout;
}

XE::GraphicsBufferPtr XE::GraphicsServiceD3D12::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	auto buf = _p->_Buffers.Alloc();

	buf->Desc = descriptor;
	buf->Parent = device;

	auto size = descriptor.Size;
	XE::uint32 align_mask = 0;
	if ( descriptor.Usage || XE::GraphicsBufferUsage::UNIFORM )
	{
		align_mask = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
		size = ( ( size - 1 ) | align_mask ) + 1;
	}

	D3D12_RESOURCE_DESC desc = {};
	{
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = size;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = Cast( XE::GraphicsTextureFormat::UNDEFINED );
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = Cast( descriptor.Usage );
	}


	bool is_cpu_read = descriptor.Usage || XE::GraphicsBufferUsage::MAP_READ;
	bool is_cpu_write = descriptor.Usage || XE::GraphicsBufferUsage::MAP_WRITE;

	D3D12_HEAP_PROPERTIES heap_properties = {};
	{
		heap_properties.CPUPageProperty = is_cpu_read ? D3D12_CPU_PAGE_PROPERTY_WRITE_BACK : ( is_cpu_write ? D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE : D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE );
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_L1;
		heap_properties.CreationNodeMask = 0;
		heap_properties.VisibleNodeMask = 0;
		heap_properties.Type = D3D12_HEAP_TYPE_CUSTOM;
	}

	if ( SUCCEEDED( device->Raw->CreateCommittedResource( &heap_properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( buf->Raw.GetAddressOf() ) ) ) )
	{
		return buf;
	}

return {};
}

XE::GraphicsCommandEncoderPtr XE::GraphicsServiceD3D12::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	auto encoder = _p->_CommandEncoders.Alloc();

	encoder->Desc = descriptor;
	encoder->Parent = device->Queue->shared_from_this();

	if ( device->FreeAllocators.try_pop( encoder->Allocator ) )
	{
		encoder->Allocator->Reset();
	}
	else if ( !SUCCEEDED( device->Raw->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( encoder->Allocator.GetAddressOf() ) ) ) )
	{
		return {};
	}

	if ( device->FreeBufferLists.try_pop( encoder->Raw ) )
	{
		encoder->Raw->Reset( encoder->Allocator.Get(), nullptr );
	}
	else if ( !SUCCEEDED( device->Raw->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, encoder->Allocator.Get(), nullptr, IID_PPV_ARGS( encoder->Raw.GetAddressOf() ) ) ) )
	{
		return {};
	}

	return encoder;
}

XE::GraphicsComputePipelinePtr XE::GraphicsServiceD3D12::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	auto pipe = _p->_ComputePipelines.Alloc();

	pipe->Desc = descriptor;
	pipe->Parent = device;
	pipe->ShaderCode = LoadShader( descriptor.Compute.Shader->Desc, descriptor.Compute.EntryPoint, XE::GraphicsShaderStage::COMPUTE );

	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	{
		desc.pRootSignature = descriptor.Layout->Raw.Get();
		desc.CS = { pipe->ShaderCode->GetBufferPointer(), pipe->ShaderCode->GetBufferSize() };
		desc.NodeMask = 0;
		desc.CachedPSO = { nullptr, 0 };
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		if ( SUCCEEDED( device->Raw->CreateComputePipelineState( &desc, IID_PPV_ARGS( pipe->Raw.GetAddressOf() ) ) ) )
		{
			return pipe;
		}
	}

	return {};
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsServiceD3D12::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	auto layout = _p->_PipelineLayouts.Alloc();

	layout->Desc = descriptor;
	layout->Parent = device;

	XE::Array< D3D12_DESCRIPTOR_RANGE > view_ranges( 100, XE::MemoryResource::GetFrameMemoryResource() );
	XE::Array< D3D12_DESCRIPTOR_RANGE > sampler_ranges( 100, XE::MemoryResource::GetFrameMemoryResource() );
	XE::Array< D3D12_DESCRIPTOR_RANGE > dynamic_ranges( 100, XE::MemoryResource::GetFrameMemoryResource() );
	XE::Array< D3D12_ROOT_PARAMETER > root_parameters( 102, XE::MemoryResource::GetFrameMemoryResource() );

	D3D12_ROOT_SIGNATURE_DESC desc = {};
	{
		for ( UINT i = 0; i < descriptor.BindGroupLayouts.size(); i++ )
		{
			auto view_start = view_ranges.size();
			auto sampler_start = sampler_ranges.size();
			auto dynamic_start = dynamic_ranges.size();

			for ( const auto & entry : descriptor.BindGroupLayouts[i]->Desc.Entries )
			{
				if ( entry.Sampler.Type != XE::GraphicsSamplerBindingType::UNDEFINED )
				{
					D3D12_DESCRIPTOR_RANGE range = {};
					{
						range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
						range.NumDescriptors = 1;
						range.RegisterSpace = i;
						range.BaseShaderRegister = entry.Binding;
						range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					}
					sampler_ranges.push_back( range );
				}
				else if ( entry.Buffer.Type != XE::GraphicsBufferBindingType::UNDEFINED && entry.Buffer.HasDynamicOffset )
				{
					D3D12_DESCRIPTOR_RANGE range = {};
					{
						switch ( entry.Buffer.Type )
						{
						case XE::GraphicsBufferBindingType::UNIFORM:
							range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
							break;
						case XE::GraphicsBufferBindingType::STORAGE:
							range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
							break;
						case XE::GraphicsBufferBindingType::READ_ONLY_STORAGE:
							range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
							break;
						}
						range.NumDescriptors = 1;
						range.RegisterSpace = i;
						range.BaseShaderRegister = entry.Binding;
						range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					}
					dynamic_ranges.push_back( range );
				}
				else
				{
					D3D12_DESCRIPTOR_RANGE range = {};
					{
						if ( entry.Buffer.Type == XE::GraphicsBufferBindingType::UNIFORM )
						{
							range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
						}
						else if ( entry.Buffer.Type == XE::GraphicsBufferBindingType::STORAGE || entry.StorageTexture.Access != XE::GraphicsStorageTextureAccess::UNDEFINED )
						{
							range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
						}
						else if ( entry.Buffer.Type == XE::GraphicsBufferBindingType::READ_ONLY_STORAGE || entry.Texture.SampleType != XE::GraphicsTextureSampleType::UNDEFINED )
						{
							range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
						}

						range.NumDescriptors = 1;
						range.RegisterSpace = i;
						range.BaseShaderRegister = entry.Binding;
						range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					}
					view_ranges.push_back( range );
				}
			}

			if ( view_ranges.size() > view_start )
			{
				D3D12_ROOT_PARAMETER view_root = {};

				view_root.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				view_root.DescriptorTable.pDescriptorRanges = view_ranges.data() + view_start;
				view_root.DescriptorTable.NumDescriptorRanges = view_ranges.size() - view_start;

				root_parameters.push_back( view_root );
			}

			if ( sampler_ranges.size() > sampler_start )
			{
				D3D12_ROOT_PARAMETER sampler_root = {};

				sampler_root.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				sampler_root.DescriptorTable.pDescriptorRanges = sampler_ranges.data() + sampler_start;
				sampler_root.DescriptorTable.NumDescriptorRanges = sampler_ranges.size() - sampler_start;

				root_parameters.push_back( sampler_root );
			}

			for ( size_t i = dynamic_start; i < dynamic_ranges.size(); i++ )
			{
				D3D12_ROOT_PARAMETER dynamic_root = {};

				switch ( dynamic_ranges[i].RangeType )
				{
				case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
					dynamic_root.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
					break;
				case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
					dynamic_root.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
					break;
				case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
					dynamic_root.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
					break;
				}
				dynamic_root.Descriptor.RegisterSpace = dynamic_ranges[i].RegisterSpace;
				dynamic_root.Descriptor.ShaderRegister = dynamic_ranges[i].BaseShaderRegister;

				root_parameters.push_back( dynamic_root );
			}
		}

		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pStaticSamplers = nullptr;
		desc.NumStaticSamplers = 0;
		desc.pParameters = root_parameters.data();
		desc.NumParameters = root_parameters.size();
	}
	D3D12BlobPtr errmsg;
	if ( SUCCEEDED( D3D12SerializeRootSignature( &desc, D3D_ROOT_SIGNATURE_VERSION_1_0, layout->BlobRootSignature.GetAddressOf(), errmsg.GetAddressOf() ) ) )
	{
		if ( SUCCEEDED( device->Raw->CreateRootSignature( 0, layout->BlobRootSignature->GetBufferPointer(), layout->BlobRootSignature->GetBufferSize(), IID_PPV_ARGS( layout->Raw.GetAddressOf() ) ) ) )
		{
			return layout;
		}
	}

	XE_ERROR( "D3D12SerializeRootSignature Error: {%0}", (const char *)errmsg->GetBufferPointer() );

	return {};
}

XE::GraphicsQuerySetPtr XE::GraphicsServiceD3D12::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	auto query_set = _p->_QuerySets.Alloc();

	query_set->Desc = descriptor;
	query_set->Parent = device;

	D3D12_QUERY_HEAP_DESC query_heap_desc = {};
	{
		switch ( descriptor.Type )
		{
		case XE::GraphicsQueryType::OCCLUSION:
			query_heap_desc.Type = D3D12_QUERY_HEAP_TYPE_OCCLUSION;
			break;
		case XE::GraphicsQueryType::PIPELINE_STATISTICS:
			query_heap_desc.Type = D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;
			break;
		case XE::GraphicsQueryType::TIMESTAMP:
			query_heap_desc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
			break;
		}

		query_heap_desc.Count = std::max<UINT>( descriptor.Count, 1 );

		if ( SUCCEEDED( device->Raw->CreateQueryHeap( &query_heap_desc, IID_PPV_ARGS( query_set->Raw.GetAddressOf() ) ) ) )
		{
			return query_set;
		}
	}

	return {};
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsServiceD3D12::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	auto bundle = _p->_RenderBundleEncoders.Alloc();

	bundle->Desc = descriptor;
	bundle->Parent = device;

	return bundle;
}

XE::GraphicsRenderPipelinePtr XE::GraphicsServiceD3D12::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	auto pipe = _p->_RenderPipelines.Alloc();

	pipe->Desc = descriptor;
	pipe->Parent = device;
	pipe->VSCode = LoadShader( descriptor.Vertex.Module->Desc, descriptor.Vertex.EntryPoint, XE::GraphicsShaderStage::VERTEX );
	pipe->FSCode = LoadShader( descriptor.Fragment.Module->Desc, descriptor.Fragment.EntryPoint, XE::GraphicsShaderStage::FRAGMENT );

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	{
		desc.pRootSignature = descriptor.Layout->Raw.Get();
		desc.VS = { pipe->VSCode->GetBufferPointer(), pipe->VSCode->GetBufferSize() };
		desc.PS = { pipe->FSCode->GetBufferPointer(), pipe->FSCode->GetBufferSize() };
		desc.GS = {};
		desc.DS = {};
		desc.HS = {};
		desc.SampleMask = descriptor.Multisample.Mask;
		switch ( descriptor.Primitive.StripIndexFormat )
		{
		case XE::GraphicsIndexFormat::UINT16:
			desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF;
			break;
		case XE::GraphicsIndexFormat::UINT32:
			desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;
			break;
		default:
			desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			break;
		}
		desc.NumRenderTargets = descriptor.Fragment.Targets.size();
		for ( size_t i = 0; i < descriptor.Fragment.Targets.size(); i++ )
		{
			desc.RTVFormats[i] = Cast( descriptor.Fragment.Targets[i].Format );
		}
		desc.DSVFormat = Cast( descriptor.DepthStencil.Format );
		desc.NodeMask = 0;
		desc.CachedPSO = {};
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.PrimitiveTopologyType = Cast( descriptor.Primitive.Topology );

		desc.SampleDesc = {};
		{
			desc.SampleDesc.Count = descriptor.Multisample.Count;
			desc.SampleDesc.Quality = 0;
		}
		desc.BlendState = {};
		{
			desc.BlendState.AlphaToCoverageEnable = descriptor.Multisample.AlphaToCoverageEnabled;
			desc.BlendState.IndependentBlendEnable = true;
			for ( size_t i = 0; i < descriptor.Fragment.Targets.size(); i++ )
			{
				auto & target = descriptor.Fragment.Targets[i];
				D3D12_RENDER_TARGET_BLEND_DESC rt_blend_desc = {};
				{
					rt_blend_desc.RenderTargetWriteMask = target.WriteMask.GetValue();

					rt_blend_desc.LogicOp = D3D12_LOGIC_OP_CLEAR;
					rt_blend_desc.LogicOpEnable = false;
					rt_blend_desc.BlendEnable = target.Blend.Enable;
					rt_blend_desc.BlendOp = Cast( target.Blend.Color.Operation );
					rt_blend_desc.SrcBlend = Cast( target.Blend.Color.SrcFactor, false );
					rt_blend_desc.DestBlend = Cast( target.Blend.Color.DstFactor, false );
					rt_blend_desc.BlendOpAlpha = Cast( target.Blend.Alpha.Operation );
					rt_blend_desc.SrcBlendAlpha = Cast( target.Blend.Alpha.SrcFactor, true );
					rt_blend_desc.DestBlendAlpha = Cast( target.Blend.Alpha.DstFactor, true );
				}
				desc.BlendState.RenderTarget[i] = rt_blend_desc;
			}
		}
		desc.InputLayout = {};
		{
			for ( size_t i = 0; i < descriptor.Vertex.Buffers.size(); i++ )
			{
				auto & vbuf = descriptor.Vertex.Buffers[i];
				for ( const auto & attrubute : vbuf.Attributes )
				{
					D3D12_INPUT_ELEMENT_DESC element_desc = {};
					{
						element_desc.SemanticIndex = attrubute.ShaderLocation;
						element_desc.Format = Cast( attrubute.Format );
						element_desc.InputSlot = i;
						element_desc.AlignedByteOffset = attrubute.Offset;
						element_desc.InputSlotClass = ( vbuf.StepMode == XE::GraphicsVertexStepMode::VERTEX ) ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
						element_desc.InstanceDataStepRate = ( vbuf.StepMode == XE::GraphicsVertexStepMode::VERTEX ) ? 0 : 1;
					}
					pipe->Elements.push_back( element_desc );
				}
			}
			desc.InputLayout.NumElements = pipe->Elements.size();
			desc.InputLayout.pInputElementDescs = pipe->Elements.data();
		}
		desc.StreamOutput = {};
		{
			desc.StreamOutput.pSODeclaration = nullptr;
			desc.StreamOutput.NumEntries = 0;
			desc.StreamOutput.pBufferStrides = nullptr;
			desc.StreamOutput.NumStrides = 0;
			desc.StreamOutput.RasterizedStream = 0;
		}
		desc.RasterizerState = {};
		{
			desc.RasterizerState.FillMode = descriptor.Primitive.Topology < GraphicsPrimitiveTopology::TRIANGLE_LIST ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
			desc.RasterizerState.CullMode = Cast( descriptor.Primitive.CullMode );
			desc.RasterizerState.FrontCounterClockwise = Cast( descriptor.Primitive.FrontFace );
			desc.RasterizerState.DepthBias = descriptor.DepthStencil.DepthBias;
			desc.RasterizerState.DepthBiasClamp = descriptor.DepthStencil.DepthBiasClamp;
			desc.RasterizerState.SlopeScaledDepthBias = descriptor.DepthStencil.DepthBiasSlopeScale;
			desc.RasterizerState.DepthClipEnable = !descriptor.Primitive.UnclippedDepth;
			desc.RasterizerState.MultisampleEnable = descriptor.Multisample.Count > 1;
			desc.RasterizerState.ForcedSampleCount = 0;
			desc.RasterizerState.AntialiasedLineEnable = false;
			desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		}
		desc.DepthStencilState = {};
		{
			desc.DepthStencilState.DepthEnable = descriptor.DepthStencil.DepthCompare != GraphicsCompareFunction::ALWAYS && descriptor.DepthStencil.DepthWriteEnabled;
			desc.DepthStencilState.DepthWriteMask = descriptor.DepthStencil.DepthWriteEnabled ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
			desc.DepthStencilState.DepthFunc = Cast( descriptor.DepthStencil.DepthCompare );
			desc.DepthStencilState.StencilEnable = descriptor.DepthStencil.StencilReadMask != 0 || descriptor.DepthStencil.StencilWriteMask != 0;
			desc.DepthStencilState.StencilReadMask = descriptor.DepthStencil.StencilReadMask;
			desc.DepthStencilState.StencilWriteMask = descriptor.DepthStencil.StencilWriteMask;
			desc.DepthStencilState.FrontFace.StencilFunc = Cast( descriptor.DepthStencil.StencilFront.Compare );
			desc.DepthStencilState.FrontFace.StencilFailOp = Cast( descriptor.DepthStencil.StencilFront.FailOp );
			desc.DepthStencilState.FrontFace.StencilPassOp = Cast( descriptor.DepthStencil.StencilFront.PassOp );
			desc.DepthStencilState.FrontFace.StencilDepthFailOp = Cast( descriptor.DepthStencil.StencilFront.DepthFailOp );
			desc.DepthStencilState.BackFace.StencilFunc = Cast( descriptor.DepthStencil.StencilBack.Compare );
			desc.DepthStencilState.BackFace.StencilFailOp = Cast( descriptor.DepthStencil.StencilBack.FailOp );
			desc.DepthStencilState.BackFace.StencilPassOp = Cast( descriptor.DepthStencil.StencilBack.PassOp );
			desc.DepthStencilState.BackFace.StencilDepthFailOp = Cast( descriptor.DepthStencil.StencilBack.DepthFailOp );
		}
	}
	if ( SUCCEEDED( device->Raw->CreateGraphicsPipelineState( &desc, IID_PPV_ARGS( pipe->Raw.GetAddressOf() ) ) ) )
	{
		return pipe;
	}

	return {};
}

XE::GraphicsSamplerPtr XE::GraphicsServiceD3D12::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	auto sampler = _p->_Samplers.Alloc();

	sampler->Desc = descriptor;
	sampler->Parent = device;

	D3D12_SAMPLER_DESC desc = {};
	{
		desc.Filter = (D3D12_FILTER)(
			( Cast( descriptor.MinFilter ) << D3D12_MIN_FILTER_SHIFT ) |
			( Cast( descriptor.MagFilter ) << D3D12_MAG_FILTER_SHIFT ) |
			( Cast( descriptor.MipmapFilter ) << D3D12_MIP_FILTER_SHIFT ) |
			( descriptor.Compare == GraphicsCompareFunction::UNDEFINED ? D3D12_FILTER_REDUCTION_TYPE_STANDARD : D3D12_FILTER_REDUCTION_TYPE_COMPARISON ) |
			( descriptor.MaxAnisotropy == 0 ? D3D12_FILTER_ANISOTROPIC : descriptor.MaxAnisotropy ) );
		desc.AddressU = Cast( descriptor.AddressModeU );
		desc.AddressV = Cast( descriptor.AddressModeV );
		desc.AddressW = Cast( descriptor.AddressModeW );
		desc.MipLODBias = 0.0f;
		desc.ComparisonFunc = Cast( descriptor.Compare );
		desc.MaxAnisotropy = descriptor.MaxAnisotropy;
		desc.MinLOD = descriptor.LodMinClamp;
		desc.MaxLOD = descriptor.LodMaxClamp;
	}

	sampler->SamplerHeapHandle = device->SamplerAllocator.Alloc();
	sampler->CPUHandle.ptr = device->SmaplerHeap->GetCPUDescriptorHandleForHeapStart().ptr + sampler->SamplerHeapHandle.GetValue() * device->SmaplerItemSize;

	device->Raw->CreateSampler( &desc, sampler->CPUHandle );

	return sampler;
}

XE::GraphicsShaderModulePtr XE::GraphicsServiceD3D12::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	auto shader = _p->_ShaderModules.Alloc();

	shader->Desc = descriptor;
	shader->Parent = device;

	return shader;
}

XE::GraphicsSwapChainPtr XE::GraphicsServiceD3D12::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	auto swap_chain = _p->_SwapChains.Alloc();

	swap_chain->Desc = descriptor;
	swap_chain->Parent = surface;

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	{
		desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		desc.Width = descriptor.Width;
		desc.Height = descriptor.Height;
		desc.Format = Cast( descriptor.Format );
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT | ( descriptor.PresentMode == GraphicsPresentMode::IMMEDIATE ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0 );
	}

	ComPtr< IDXGISwapChain1 > swap;
	if ( surface->Desc.Window != nullptr )
	{
		if ( SUCCEEDED( _p->_Factory->CreateSwapChainForHwnd( device->Queue->Raw.Get(), (HWND)surface->Desc.Window->GetHandle().GetValue(), &desc, nullptr, nullptr, swap.GetAddressOf() ) ) )
		{
			swap->QueryInterface( swap_chain->Raw.GetAddressOf() );
		}
	}
	else
	{
		if ( SUCCEEDED( _p->_Factory->CreateSwapChainForComposition( device->Queue->Raw.Get(), &desc, nullptr, swap.GetAddressOf() ) ) )
		{
			swap->QueryInterface( swap_chain->Raw.GetAddressOf() );
		}
	}

	if( swap_chain->Raw != nullptr )
	{
		for ( size_t i = 0; i < swap_chain->Textures.size(); i++ )
		{
			swap_chain->Textures[i] = _p->_Textures.Alloc();
			{
				swap_chain->Textures[i]->Parent = device;

				if ( SUCCEEDED( swap->GetBuffer( i, IID_PPV_ARGS( swap_chain->Textures[i]->Raw.GetAddressOf() ) ) ) )
				{
					swap_chain->Textures[i]->Desc.Label = descriptor.Label + "_Texture_" + XE::ToString( i );
					swap_chain->Textures[i]->Desc.Format = descriptor.Format;
					swap_chain->Textures[i]->Desc.Size = { (XE::int32)descriptor.Width, (XE::int32)descriptor.Height, 1 };
					swap_chain->Textures[i]->Desc.Dimension = XE::GraphicsTextureDimension::D2;
					swap_chain->Textures[i]->Desc.SampleCount = 0;
					swap_chain->Textures[i]->Desc.MipLevelCount = 1;
					swap_chain->Textures[i]->Desc.Usage = XE::GraphicsTextureUsage::RENDER_ATTACHMENT;
					swap_chain->Textures[i]->Desc.ViewFormats.push_back( descriptor.Format );

					XE::GraphicsTextureViewDescriptor desc = {};
					{
						desc.Label = descriptor.Label + "_TextureView_" + XE::ToString( i );
						desc.MipLevelCount = 1;
						desc.ArrayLayerCount = 1;
						desc.BaseMipLevel = 0;
						desc.BaseArrayLayer = 0;
						desc.Aspect = XE::GraphicsTextureAspect::ALL;
						desc.Dimension = XE::GraphicsTextureViewDimension::D2;
						desc.Format = descriptor.Format;
					}
					swap_chain->TextureViews[i] = TextureCreateView( swap_chain->Textures[i], desc );
				}
			}
		}

		return swap_chain;
	}

	return {};
}

XE::GraphicsTexturePtr XE::GraphicsServiceD3D12::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	auto texture = _p->_Textures.Alloc();

	texture->Desc = descriptor;
	texture->Parent = device;

	D3D12_RESOURCE_DESC desc = {};
	{
		desc.Dimension = Cast( descriptor.Dimension );
		desc.Alignment = 0;
		desc.Width = descriptor.Size.x;
		desc.Height = descriptor.Size.y;
		desc.DepthOrArraySize = descriptor.Size.z;
		desc.MipLevels = descriptor.MipLevelCount;
		if ( descriptor.Usage || XE::MakeFlags( XE::GraphicsTextureUsage::TEXTURE_BINDING, XE::GraphicsTextureUsage::STORAGE_BINDING ) )
		{
			switch ( descriptor.Format )
			{
			case XE::GraphicsTextureFormat::DEPTH24PLUS:
			case XE::GraphicsTextureFormat::DEPTH24PLUSSTENCIL8:
			case XE::GraphicsTextureFormat::DEPTH24UNORMSTENCIL8:
				desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
				break;
			case XE::GraphicsTextureFormat::DEPTH32FLOAT:
				desc.Format = DXGI_FORMAT_R32_TYPELESS;
				break;
			case XE::GraphicsTextureFormat::DEPTH32FLOATSTENCIL8:
				desc.Format = DXGI_FORMAT_R32G8X24_TYPELESS;
				break;
			default:
				desc.Format = DXGI_FORMAT_UNKNOWN;
				break;
			}
		}
		else
		{
			desc.Format = Cast( descriptor.Format );
		}
		desc.SampleDesc = { descriptor.SampleCount, 0 };
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if ( descriptor.Usage && XE::GraphicsTextureUsage::RENDER_ATTACHMENT )
		{
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
		if ( descriptor.Usage && XE::GraphicsTextureUsage::STORAGE_BINDING )
		{
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			if ( descriptor.Usage && XE::GraphicsTextureUsage::TEXTURE_BINDING )
			{
				desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
			}
		}
	}
	D3D12_HEAP_PROPERTIES heap_properties = {};
	{
		heap_properties.Type = D3D12_HEAP_TYPE_CUSTOM;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		heap_properties.CreationNodeMask = 0;
		heap_properties.VisibleNodeMask = 0;
	}

	D3D12ResourcePtr resource;
	if ( SUCCEEDED( device->Raw->CreateCommittedResource( &heap_properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( resource.GetAddressOf() ) ) ) )
	{
		texture->Raw = resource;
	}

	return texture;
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceD3D12::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	auto view = _p->_TextureViews.Alloc();

	view->Desc = descriptor;
	view->Parent = texture;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
	{
		uav_desc.Format = Cast( descriptor.Format );

		switch ( descriptor.Dimension )
		{
		case XE::GraphicsTextureViewDimension::D1:
		{
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;

			uav_desc.Texture1D.MipSlice = descriptor.BaseMipLevel;
		}
		break;
		case XE::GraphicsTextureViewDimension::D2:
		{
			if ( descriptor.BaseArrayLayer == 0 )
			{
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				uav_desc.Texture2D.MipSlice = descriptor.BaseMipLevel;
				uav_desc.Texture2D.PlaneSlice = 0;
			}
			else
			{
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				uav_desc.Texture2DArray.MipSlice = descriptor.BaseMipLevel;
				uav_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
				uav_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
				uav_desc.Texture2DArray.PlaneSlice = 0;
			}
		}
		break;
		case XE::GraphicsTextureViewDimension::D2ARRAY:
		{
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			uav_desc.Texture2DArray.MipSlice = descriptor.BaseMipLevel;
			uav_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
			uav_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
			uav_desc.Texture2DArray.PlaneSlice = 0;
		}
		break;
		case XE::GraphicsTextureViewDimension::D3:
		{
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			uav_desc.Texture3D.MipSlice = descriptor.BaseMipLevel;
			uav_desc.Texture3D.FirstWSlice = descriptor.BaseArrayLayer;
			uav_desc.Texture3D.WSize = descriptor.ArrayLayerCount;
		}
		break;
		default:
			break;
		}

		view->UAVHandle = texture->Parent->CSUAllocator.Alloc();
		view->UAVCPUHandle.ptr = texture->Parent->CSUHeap->GetCPUDescriptorHandleForHeapStart().ptr + view->UAVHandle.GetValue() * texture->Parent->CSUItemSize;

		texture->Parent->Raw->CreateUnorderedAccessView( texture->Raw.Get(), nullptr, &uav_desc, view->UAVCPUHandle );
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	{
		srv_desc.Format = Cast( descriptor.Format );
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		switch ( descriptor.Dimension )
		{
		case XE::GraphicsTextureViewDimension::UNDEFINED:
			break;
		case XE::GraphicsTextureViewDimension::D1:
		{
			srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			srv_desc.Texture1D.MostDetailedMip = descriptor.BaseMipLevel;
			srv_desc.Texture1D.MipLevels = descriptor.MipLevelCount;
			srv_desc.Texture1D.ResourceMinLODClamp = 0.0f;
		}
		break;
		case XE::GraphicsTextureViewDimension::D2:
		{
			if ( texture->Desc.SampleCount != 0 && descriptor.BaseArrayLayer == 0 )
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
				srv_desc.Texture2DMS.UnusedField_NothingToDefine = 0;
			}
			else if ( descriptor.BaseArrayLayer == 0 )
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srv_desc.Texture2D.MostDetailedMip = descriptor.BaseMipLevel;
				srv_desc.Texture2D.MipLevels = descriptor.MipLevelCount;
				srv_desc.Texture2D.PlaneSlice = 0;
				srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
			}
			else if ( texture->Desc.SampleCount != 0 )
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				srv_desc.Texture2DMSArray.FirstArraySlice = descriptor.BaseArrayLayer;
				srv_desc.Texture2DMSArray.ArraySize = descriptor.ArrayLayerCount;
			}
			else
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				srv_desc.Texture2DArray.MostDetailedMip = descriptor.BaseMipLevel;
				srv_desc.Texture2DArray.MipLevels = descriptor.MipLevelCount;
				srv_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
				srv_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
				srv_desc.Texture2DArray.PlaneSlice = 0;
				srv_desc.Texture2DArray.ResourceMinLODClamp = 0.0f;
			}
		}
		break;
		case XE::GraphicsTextureViewDimension::D2ARRAY:
		{
			if ( texture->Desc.SampleCount != 0 )
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				srv_desc.Texture2DMSArray.FirstArraySlice = descriptor.BaseArrayLayer;
				srv_desc.Texture2DMSArray.ArraySize = descriptor.ArrayLayerCount;
			}
			else
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				srv_desc.Texture2DArray.MostDetailedMip = descriptor.BaseMipLevel;
				srv_desc.Texture2DArray.MipLevels = descriptor.MipLevelCount;
				srv_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
				srv_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
				srv_desc.Texture2DArray.PlaneSlice = 0;
				srv_desc.Texture2DArray.ResourceMinLODClamp = 0.0f;
			}
		}
		break;
		case XE::GraphicsTextureViewDimension::D3:
		{
			srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			srv_desc.Texture3D.MostDetailedMip = descriptor.BaseMipLevel;
			srv_desc.Texture3D.MipLevels = descriptor.MipLevelCount;
			srv_desc.Texture3D.ResourceMinLODClamp = 0.0f;
		}
		break;
		case XE::GraphicsTextureViewDimension::CUBE:
		{
			if ( descriptor.BaseArrayLayer == 0 )
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				srv_desc.TextureCube.MostDetailedMip = descriptor.BaseMipLevel;
				srv_desc.TextureCube.MipLevels = descriptor.MipLevelCount;
				srv_desc.TextureCube.ResourceMinLODClamp = 0.0f;
			}
			else
			{
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				srv_desc.TextureCubeArray.MostDetailedMip = descriptor.BaseMipLevel;
				srv_desc.TextureCubeArray.MipLevels = descriptor.MipLevelCount;
				srv_desc.TextureCubeArray.First2DArrayFace = descriptor.BaseArrayLayer;
				srv_desc.TextureCubeArray.NumCubes = descriptor.ArrayLayerCount == 0 ? 0 : descriptor.ArrayLayerCount / 6;
				srv_desc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
			}
		}
		break;
		case XE::GraphicsTextureViewDimension::CUBEARRAY:
		{
			srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
			srv_desc.TextureCubeArray.MostDetailedMip = descriptor.BaseMipLevel;
			srv_desc.TextureCubeArray.MipLevels = descriptor.MipLevelCount;
			srv_desc.TextureCubeArray.First2DArrayFace = descriptor.BaseArrayLayer;
			srv_desc.TextureCubeArray.NumCubes = descriptor.ArrayLayerCount == 0 ? 0 : descriptor.ArrayLayerCount / 6;
			srv_desc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
		}
		break;
		default:
			break;
		}

		view->SRVHandle = texture->Parent->CSUAllocator.Alloc();
		view->SRVCPUHandle.ptr = texture->Parent->CSUHeap->GetCPUDescriptorHandleForHeapStart().ptr + view->SRVHandle.GetValue() * texture->Parent->CSUItemSize;

		texture->Parent->Raw->CreateShaderResourceView( texture->Raw.Get(), &srv_desc, view->SRVCPUHandle );
	}

	D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
	{
		rtv_desc.Format = Cast( descriptor.Format );

		switch ( descriptor.Dimension )
		{
		case XE::GraphicsTextureViewDimension::D1:
		{
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			rtv_desc.Texture1D.MipSlice = descriptor.BaseMipLevel;
		}
		break;
		case XE::GraphicsTextureViewDimension::D2:
		{
			if ( texture->Desc.SampleCount != 0 && descriptor.BaseArrayLayer == 0 )
			{
				rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
				rtv_desc.Texture2DMS.UnusedField_NothingToDefine = 0;
			}
			else if ( descriptor.BaseArrayLayer == 0 )
			{
				rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				rtv_desc.Texture2D.MipSlice = descriptor.BaseMipLevel;
				rtv_desc.Texture2D.PlaneSlice = 0;
			}
			else if ( texture->Desc.SampleCount != 0 )
			{
				rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
				rtv_desc.Texture2DMSArray.FirstArraySlice = descriptor.BaseArrayLayer;
				rtv_desc.Texture2DMSArray.ArraySize = descriptor.ArrayLayerCount;
			}
			else
			{
				rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				rtv_desc.Texture2DArray.MipSlice = descriptor.BaseMipLevel;
				rtv_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
				rtv_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
				rtv_desc.Texture2DArray.PlaneSlice = 0;
			}
		}
		break;
		case XE::GraphicsTextureViewDimension::D2ARRAY:
		{
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			rtv_desc.Texture2DArray.MipSlice = descriptor.BaseMipLevel;
			rtv_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
			rtv_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
			rtv_desc.Texture2DArray.PlaneSlice = 0;
		}
		break;
		case XE::GraphicsTextureViewDimension::D3:
		{
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			rtv_desc.Texture3D.MipSlice = descriptor.BaseMipLevel;
			rtv_desc.Texture3D.FirstWSlice = descriptor.BaseArrayLayer;
			rtv_desc.Texture3D.WSize = descriptor.ArrayLayerCount;
		}
		break;
		default:
			break;
		}

		view->RTVHandle = texture->Parent->RTVAllocator.Alloc();
		view->RTVCPUHandle.ptr = texture->Parent->RTVHeap->GetCPUDescriptorHandleForHeapStart().ptr + view->RTVHandle.GetValue() * texture->Parent->RTVItemSize;

		texture->Parent->Raw->CreateRenderTargetView( texture->Raw.Get(), &rtv_desc, view->RTVCPUHandle );
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	{
		dsv_desc.Format = Cast( descriptor.Format );
		switch ( descriptor.Aspect )
		{
		case XE::GraphicsTextureAspect::ALL:
			dsv_desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_STENCIL | D3D12_DSV_FLAG_READ_ONLY_DEPTH;
			break;
		case XE::GraphicsTextureAspect::STENCIL_ONLY:
			dsv_desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_STENCIL;
			break;
		case XE::GraphicsTextureAspect::DEPTH_ONLY:
			dsv_desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_DEPTH;
			break;
		default:
			break;
		}

		switch ( descriptor.Dimension )
		{
		case XE::GraphicsTextureViewDimension::D1:
		{
			dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
			dsv_desc.Texture1D.MipSlice = descriptor.BaseMipLevel;
		}
		break;
		case XE::GraphicsTextureViewDimension::D2:
		{
			if ( texture->Desc.SampleCount != 0 && descriptor.BaseArrayLayer == 0 )
			{
				dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
				dsv_desc.Texture2DMS.UnusedField_NothingToDefine = 0;
			}
			else if ( descriptor.BaseArrayLayer == 0 )
			{
				dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsv_desc.Texture2D.MipSlice = descriptor.BaseMipLevel;
			}
			else if ( texture->Desc.SampleCount != 0 )
			{
				dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				dsv_desc.Texture2DMSArray.FirstArraySlice = descriptor.BaseArrayLayer;
				dsv_desc.Texture2DMSArray.ArraySize = descriptor.ArrayLayerCount;
			}
			else
			{
				dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				dsv_desc.Texture2DArray.MipSlice = descriptor.BaseMipLevel;
				dsv_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
				dsv_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
			}
		}
		break;
		case XE::GraphicsTextureViewDimension::D2ARRAY:
		{
			dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			dsv_desc.Texture2DArray.MipSlice = descriptor.BaseMipLevel;
			dsv_desc.Texture2DArray.FirstArraySlice = descriptor.BaseArrayLayer;
			dsv_desc.Texture2DArray.ArraySize = descriptor.ArrayLayerCount;
		}
		break;
		default:
			break;
		}

		view->DSVHandle = texture->Parent->DSVAllocator.Alloc();
		view->DSVCPUHandle.ptr = texture->Parent->DSVHeap->GetCPUDescriptorHandleForHeapStart().ptr + view->DSVHandle.GetValue() * texture->Parent->DSVItemSize;

		texture->Parent->Raw->CreateDepthStencilView( texture->Raw.Get(), &dsv_desc, view->DSVCPUHandle );
	}

	return view;
}

void XE::GraphicsServiceD3D12::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{
	// TODO: 
}

bool XE::GraphicsServiceD3D12::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	// TODO: 
	return {};
}

XE::GraphicsQueuePtr XE::GraphicsServiceD3D12::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	if ( device->Queue )
	{
		return XE::GraphicsQueuePtr( device->Queue );
	}

	auto queue = _p->_Queues.Alloc();

	device->Queue = queue.get();
	queue->Parent = device;

	queue->Event = ::CreateEvent( nullptr, false, false, nullptr );

	if ( SUCCEEDED( device->Raw->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( queue->Fence.GetAddressOf() ) ) ) )
	{
		return XE::GraphicsQueuePtr( device->Queue );
	}

	return {};
}

bool XE::GraphicsServiceD3D12::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	// TODO: 
	return {};
}

bool XE::GraphicsServiceD3D12::DevicePopErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	// TODO: 
	return {};
}

void XE::GraphicsServiceD3D12::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{
	// TODO: 
}

void XE::GraphicsServiceD3D12::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::DeviceLostCallback callback )
{
	device->LostCallback = callback;
}

void XE::GraphicsServiceD3D12::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	device->ErrorCallback = callback;
}

void XE::GraphicsServiceD3D12::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, XE::GraphicsService::QueueWorkDoneCallback callback )
{
	UINT64 value = queue->Fence->GetCompletedValue();
	if ( value == 0 )
	{
		callback( XE::GraphicsQueueWorkDoneStatus::DEVICE_LOST );

		return;
	}

	value++;

	if ( SUCCEEDED( queue->Raw->Signal( queue->Fence.Get(), value ) ) )
	{
		if ( SUCCEEDED( queue->Fence->SetEventOnCompletion( value, queue->Event ) ) )
		{
			switch ( ::WaitForSingleObject( queue->Event, INFINITE ) )
			{
			case WAIT_OBJECT_0:
				callback( XE::GraphicsQueueWorkDoneStatus::SUCCESS );
				break;
			default:
				callback( XE::GraphicsQueueWorkDoneStatus::ERROR );
				break;
			}

			D3D12CommandListPtr list;
			while ( queue->TakeBufferLists.try_pop( list ) )
			{
				queue->Parent->FreeBufferLists.push( list );
			}

			D3D12CommandAllocatorPtr allocator;
			while ( queue->TakeAllocators.try_pop( allocator ) )
			{
				allocator->Reset();
				queue->Parent->FreeAllocators.push( allocator );
			}

			return;
		}
	}

	callback( XE::GraphicsQueueWorkDoneStatus::ERROR );
}

void XE::GraphicsServiceD3D12::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{
	XE::Array< ID3D12CommandList * > lists( commands.size(), XE::MemoryResource::GetFrameMemoryResource() );

	std::for_each( commands.begin(), commands.end(), [&]( const auto & cmd ) { lists.push_back( cmd->Raw.Get() ); } );

	queue->Raw->ExecuteCommandLists( lists.size(), lists.data() );
}

void XE::GraphicsServiceD3D12::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{
	// TODO: 
}

void XE::GraphicsServiceD3D12::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & dst, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{
	// TODO: 
}

void XE::GraphicsServiceD3D12::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, XE::GraphicsService::BufferMapCallback callback )
{
	// TODO: 
}

XE::Span< const XE::uint8 > XE::GraphicsServiceD3D12::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	auto data = BufferGetMappedRange( buffer, offset, size );
	return { data.data(), data.size() };
}

XE::Span< XE::uint8 > XE::GraphicsServiceD3D12::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	buffer->MapRange.Begin = offset;
	buffer->MapRange.End = offset + size;

	void * data = nullptr;
	if ( SUCCEEDED( buffer->Raw->Map( 0, &buffer->MapRange, &data ) ) )
	{
		return { reinterpret_cast<XE::uint8 *>( data ), size };
	}

	return {};
}

void XE::GraphicsServiceD3D12::BufferUnmap( XE::GraphicsBufferPtr buffer )
{
	buffer->Raw->Unmap( 0, &buffer->MapRange );
}

XE::GraphicsComputePassEncoderPtr XE::GraphicsServiceD3D12::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	auto pass = _p->_ComputePassEncoders.Alloc();

	pass->Desc = descriptor;
	pass->Parent = command_encoder;

	return pass;
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsServiceD3D12::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	auto pass = _p->_RenderPassEncoders.Alloc();

	pass->Desc = descriptor;
	pass->Parent = command_encoder;

	return pass;
}

void XE::GraphicsServiceD3D12::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr src, XE::uint64 src_offset, XE::GraphicsBufferPtr dst, XE::uint64 dst_offset, XE::uint64 size )
{
	command_encoder->Raw->CopyBufferRegion( dst->Raw.Get(), dst_offset, src->Raw.Get(), src_offset, size );
}

void XE::GraphicsServiceD3D12::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	D3D12_BOX box = {};
	{
		box.left = 0;
		box.top = 0;
		box.right = 0 + copy_size.x;
		box.bottom = 0 + copy_size.y;
		box.front = 0;
		box.back = 0 + copy_size.z;
	}

	D3D12_TEXTURE_COPY_LOCATION src_location = {};
	{
		src_location.pResource = src.Buffer->Raw.Get();
		src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src_location.PlacedFootprint.Offset = src.Layout.Offset;
		src_location.PlacedFootprint.Footprint.Format = Cast( dst.Texture->Desc.Format );
		src_location.PlacedFootprint.Footprint.Width = copy_size.x;
		src_location.PlacedFootprint.Footprint.Height = src.Layout.RowsPerImage;
		src_location.PlacedFootprint.Footprint.Depth = copy_size.z;
		src_location.PlacedFootprint.Footprint.RowPitch = src.Layout.BytesPerRow;
	}

	D3D12_TEXTURE_COPY_LOCATION dst_location = {};
	{
		dst_location.pResource = dst.Texture->Raw.Get();
		dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		int array_layer_count = dst.Texture->Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : dst.Texture->Desc.Size.z;
		dst_location.SubresourceIndex = dst.MipLevel + ( dst.Origin.z * array_layer_count ) * dst.Texture->Desc.MipLevelCount;
	}

	command_encoder->Raw->CopyTextureRegion( &dst_location, dst.Origin.x, dst.Origin.y, dst.Origin.z, &src_location, &box );
}

void XE::GraphicsServiceD3D12::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyBuffer & dst, const XE::Vec3f & copy_size )
{
	D3D12_BOX box = {};
	{
		box.left = src.Origin.x;
		box.top = src.Origin.y;
		box.right = src.Origin.x + copy_size.x;
		box.bottom = src.Origin.y + copy_size.y;
		box.front = src.Origin.z;
		box.back = src.Origin.z + copy_size.z;
	}

	D3D12_TEXTURE_COPY_LOCATION src_location = {};
	{
		src_location.pResource = src.Texture->Raw.Get();
		src_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		int array_layer_count = src.Texture->Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : src.Texture->Desc.Size.z;
		src_location.SubresourceIndex = src.MipLevel + ( src.Origin.z * array_layer_count ) * src.Texture->Desc.MipLevelCount;
	}

	D3D12_TEXTURE_COPY_LOCATION dst_location = {};
	{
		dst_location.pResource = dst.Buffer->Raw.Get();
		dst_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		dst_location.PlacedFootprint.Offset = dst.Layout.Offset;
		dst_location.PlacedFootprint.Footprint.Format = Cast( src.Texture->Desc.Format );
		dst_location.PlacedFootprint.Footprint.Width = copy_size.x;
		dst_location.PlacedFootprint.Footprint.Height = dst.Layout.RowsPerImage;
		dst_location.PlacedFootprint.Footprint.Depth = copy_size.z;
		dst_location.PlacedFootprint.Footprint.RowPitch = dst.Layout.BytesPerRow;
	}

	command_encoder->Raw->CopyTextureRegion( &dst_location, 0, 0, 0, &src_location, &box );
}

void XE::GraphicsServiceD3D12::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	D3D12_BOX box = {};
	{
		box.left = src.Origin.x;
		box.top = src.Origin.y;
		box.right = src.Origin.x + copy_size.x;
		box.bottom = src.Origin.y + copy_size.y;
		box.front = src.Origin.z;
		box.back = src.Origin.z + copy_size.z;
	}

	D3D12_TEXTURE_COPY_LOCATION src_location = {};
	{
		src_location.pResource = src.Texture->Raw.Get();
		src_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		int array_layer_count = src.Texture->Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : src.Texture->Desc.Size.z;
		src_location.SubresourceIndex = src.MipLevel + ( src.Origin.z * array_layer_count ) * src.Texture->Desc.MipLevelCount;
	}

	D3D12_TEXTURE_COPY_LOCATION dst_location = {};
	{
		dst_location.pResource = dst.Texture->Raw.Get();
		dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		int array_layer_count = dst.Texture->Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : dst.Texture->Desc.Size.z;
		dst_location.SubresourceIndex = dst.MipLevel + ( dst.Origin.z * array_layer_count ) * dst.Texture->Desc.MipLevelCount;
	}

	command_encoder->Raw->CopyTextureRegion( &dst_location, dst.Origin.x, dst.Origin.y, dst.Origin.z, &src_location, &box );
}

XE::GraphicsCommandBufferPtr XE::GraphicsServiceD3D12::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	auto buf = _p->_CommandBuffers.Alloc();
	{
		command_encoder->Raw->Close();

		buf->Desc = descriptor;
		buf->Raw = command_encoder->Raw;
		buf->Parent = command_encoder;

		command_encoder->Parent->TakeBufferLists.push( command_encoder->Raw );
		command_encoder->Parent->TakeAllocators.push( command_encoder->Allocator );
		command_encoder->Allocator = nullptr;
		command_encoder->Raw = nullptr;
	}
	return buf;
}

void XE::GraphicsServiceD3D12::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{
	command_encoder->Raw->SetMarker( 0, marker_label.c_str(), marker_label.size() );
}

void XE::GraphicsServiceD3D12::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{
	command_encoder->Raw->EndEvent();
}

void XE::GraphicsServiceD3D12::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{
	command_encoder->Raw->BeginEvent( 0, group_label.c_str(), group_label.size() );
}

void XE::GraphicsServiceD3D12::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr dst, XE::uint64 dst_offset )
{
	command_encoder->Raw->ResolveQueryData( query_set->Raw.Get(), Cast( query_set->Desc.Type ), first_query, query_count, dst->Raw.Get(), dst_offset );
}

void XE::GraphicsServiceD3D12::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	command_encoder->Raw->EndQuery( query_set->Raw.Get(), D3D12_QUERY_TYPE_TIMESTAMP, query_index );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	XE::GraphicsComputePassEncoder::BeginPipelineStatisticsQuery info;
	{
		info.query_set = query_set;
		info.query_index = query_index;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{
	XE::GraphicsComputePassEncoder::Dispatch info;
	{
		info.workgroup_count_x = workgroup_count_x;
		info.workgroup_count_y = workgroup_count_y;
		info.workgroup_count_z = workgroup_count_z;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	XE::GraphicsComputePassEncoder::DispatchIndirect info;
	{
		info.indirect_buffer = indirect_buffer;
		info.indirect_offset = indirect_offset;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	XE::Stack< XE::GraphicsComputePassEncoder::BeginPipelineStatisticsQuery > PipelineStatisticsQueryStack( XE::MemoryResource::GetFrameMemoryResource() );

	D3D12CommandListPtr list = compute_pass_encoder->Parent->Raw;
	ID3D12DescriptorHeap * heaps[2] = { compute_pass_encoder->Parent->Parent->Parent->CSUHeap.Get(), compute_pass_encoder->Parent->Parent->Parent->SmaplerHeap.Get() };

	list->BeginEvent( 0, compute_pass_encoder->Desc.Label.c_str(), compute_pass_encoder->Desc.Label.size() );
	list->SetDescriptorHeaps( 2, heaps );

	while ( !compute_pass_encoder->CommandInfos.empty() )
	{
		std::visit( XE::Overloaded{
			[&]( const std::monostate & info ) {},
			[&]( const XE::GraphicsComputePassEncoder::BeginPipelineStatisticsQuery & info ) { PipelineStatisticsQueryStack.push( info );  list->BeginQuery( info.query_set->Raw.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, info.query_index ); },
			[&]( const XE::GraphicsComputePassEncoder::Dispatch & info ) { list->Dispatch( info.workgroup_count_x, info.workgroup_count_y, info.workgroup_count_z ); },
			[&]( const XE::GraphicsComputePassEncoder::DispatchIndirect & info ) { list->ExecuteIndirect( compute_pass_encoder->Parent->Parent->Parent->DispatchIndirectSignature.Get(), 1, info.indirect_buffer->Raw.Get(), info.indirect_offset, nullptr, 0 ); },
			[&]( const XE::GraphicsComputePassEncoder::EndPipelineStatisticsQuery & info ) { list->EndQuery( PipelineStatisticsQueryStack.top().query_set->Raw.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS,  PipelineStatisticsQueryStack.top().query_index ); PipelineStatisticsQueryStack.pop(); },
			[&]( const XE::GraphicsComputePassEncoder::InsertDebugMarker & info ) { list->SetMarker( 0, info.marker_label.c_str(), info.marker_label.size() ); },
			[&]( const XE::GraphicsComputePassEncoder::PopDebugGroup & info ) { list->EndEvent(); },
			[&]( const XE::GraphicsComputePassEncoder::PushDebugGroup & info ) { list->BeginEvent( 0, info.group_label.c_str(), info.group_label.size() ); },
			[&]( const XE::GraphicsComputePassEncoder::SetBindGroup & info ) { /* TODO */ },
			[&]( const XE::GraphicsComputePassEncoder::SetPipeline & info ) { list->SetComputeRootSignature( info.pipeline->Desc.Layout->Raw.Get() ); list->SetPipelineState( info.pipeline->Raw.Get() ); } },
			compute_pass_encoder->CommandInfos.front() );

		compute_pass_encoder->CommandInfos.pop();
	}

	list->SetDescriptorHeaps( 0, nullptr );
	list->EndEvent();
}

void XE::GraphicsServiceD3D12::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	compute_pass_encoder->CommandInfos.push( XE::GraphicsComputePassEncoder::EndPipelineStatisticsQuery() );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{
	XE::GraphicsComputePassEncoder::InsertDebugMarker info;
	{
		info.marker_label = marker_label;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	compute_pass_encoder->CommandInfos.push( XE::GraphicsComputePassEncoder::PopDebugGroup() );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{
	XE::GraphicsComputePassEncoder::PushDebugGroup info;
	{
		info.group_label = group_label;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	XE::GraphicsComputePassEncoder::SetBindGroup info;
	{
		info.group_index = group_index;
		info.group = group;
		info.dynamic_offsets = dynamic_offsets;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{
	XE::GraphicsComputePassEncoder::SetPipeline info;
	{
		info.pipeline = pipeline;
	}
	compute_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	XE::GraphicsRenderPassEncoder::BeginOcclusionQuery info;
	{
		info.query_set = query_set;
		info.query_index = query_index;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	XE::GraphicsRenderPassEncoder::BeginPipelineStatisticsQuery info;
	{
		info.query_set = query_set;
		info.query_index = query_index;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	XE::GraphicsRenderPassEncoder::Draw info;
	{
		info.vertex_count = vertex_count;
		info.instance_count = instance_count;
		info.first_vertex = first_vertex;
		info.first_instance = first_instance;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	XE::GraphicsRenderPassEncoder::DrawIndexed info;
	{
		info.index_count = index_count;
		info.instance_count = instance_count;
		info.first_index = first_index;
		info.base_vertex = base_vertex;
		info.first_instance = first_instance;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	XE::GraphicsRenderPassEncoder::DrawIndexedIndirect info;
	{
		info.indirect_buffer = indirect_buffer;
		info.indirect_offset = indirect_offset;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	XE::GraphicsRenderPassEncoder::DrawIndirect info;
	{
		info.indirect_buffer = indirect_buffer;
		info.indirect_offset = indirect_offset;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	XE::Array< D3D12_VERTEX_BUFFER_VIEW > VertexBuffers( XE::MemoryResource::GetFrameMemoryResource() );
	XE::Stack< XE::GraphicsRenderPassEncoder::BeginOcclusionQuery > OcclusionQueryStack( XE::MemoryResource::GetFrameMemoryResource() );
	XE::Stack< XE::GraphicsRenderPassEncoder::BeginPipelineStatisticsQuery > PipelineStatisticsQueryStack( XE::MemoryResource::GetFrameMemoryResource() );

	D3D12CommandListPtr list = render_pass_encoder->Parent->Raw;
	ID3D12DescriptorHeap * heaps[2] = { render_pass_encoder->Parent->Parent->Parent->CSUHeap.Get(), render_pass_encoder->Parent->Parent->Parent->SmaplerHeap.Get() };

	list->BeginEvent( 0, render_pass_encoder->Parent->Desc.Label.c_str(), render_pass_encoder->Parent->Desc.Label.size() );
	list->SetDescriptorHeaps( 2, heaps );

	while ( !render_pass_encoder->CommandInfos.empty() )
	{
		std::visit( XE::Overloaded{
			[&]( const std::monostate & info ) {},
			[&]( const XE::GraphicsRenderPassEncoder::BeginOcclusionQuery & info ) { OcclusionQueryStack.push( info ); list->BeginQuery( info.query_set->Raw.Get(), D3D12_QUERY_TYPE_OCCLUSION, info.query_index ); },
			[&]( const XE::GraphicsRenderPassEncoder::BeginPipelineStatisticsQuery & info ) { PipelineStatisticsQueryStack.push( info ); list->BeginQuery( info.query_set->Raw.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, info.query_index ); },
			[&]( const XE::GraphicsRenderPassEncoder::Draw & info ) { list->IASetVertexBuffers(0, VertexBuffers.size(), VertexBuffers.data() ); list->DrawInstanced( info.vertex_count, info.instance_count, info.first_vertex, info.first_instance ); },
			[&]( const XE::GraphicsRenderPassEncoder::DrawIndexed & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() );  list->DrawIndexedInstanced( info.index_count, info.instance_count, info.first_index, info.base_vertex, info.first_instance ); },
			[&]( const XE::GraphicsRenderPassEncoder::DrawIndexedIndirect & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() );  list->ExecuteIndirect( render_pass_encoder->Parent->Parent->Parent->DrawIndexedIndirectSignature.Get(), 1, info.indirect_buffer->Raw.Get(), info.indirect_offset, nullptr, 0 ); },
			[&]( const XE::GraphicsRenderPassEncoder::DrawIndirect & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() );  list->ExecuteIndirect( render_pass_encoder->Parent->Parent->Parent->DrawIndirectSignature.Get(), 1, info.indirect_buffer->Raw.Get(), info.indirect_offset, nullptr, 0 ); },
			[&]( const XE::GraphicsRenderPassEncoder::EndOcclusionQuery & info ) { list->EndQuery( OcclusionQueryStack.top().query_set->Raw.Get(), D3D12_QUERY_TYPE_OCCLUSION, OcclusionQueryStack.top().query_index ); OcclusionQueryStack.pop(); },
			[&]( const XE::GraphicsRenderPassEncoder::EndPipelineStatisticsQuery & info ) { list->EndQuery( PipelineStatisticsQueryStack.top().query_set->Raw.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS,  PipelineStatisticsQueryStack.top().query_index ); PipelineStatisticsQueryStack.pop(); },
			[&]( const XE::GraphicsRenderPassEncoder::ExecuteBundles & info ) { for ( const auto & it : info.bundles ) list->ExecuteBundle( it->Raw.Get() ); },
			[&]( const XE::GraphicsRenderPassEncoder::InsertDebugMarker & info ) { list->SetMarker( 0, info.marker_label.c_str(), info.marker_label.size() ); },
			[&]( const XE::GraphicsRenderPassEncoder::PopDebugGroup & info ) { list->EndEvent(); },
			[&]( const XE::GraphicsRenderPassEncoder::PushDebugGroup & info ) { list->BeginEvent( 0, info.group_label.c_str(), info.group_label.size() ); },
			[&]( const XE::GraphicsRenderPassEncoder::SetBindGroup & info ) { /* TODO */ },
			[&]( const XE::GraphicsRenderPassEncoder::SetBlendConstant & info ) { list->OMSetBlendFactor( XE::FColor( info.color ).d ); },
			[&]( const XE::GraphicsRenderPassEncoder::SetIndexBuffer & info ) { D3D12_INDEX_BUFFER_VIEW view{ info.buffer->Raw->GetGPUVirtualAddress() + info.offset, (UINT)info.size, Cast( info.format ) }; list->IASetIndexBuffer( &view ); },
			[&]( const XE::GraphicsRenderPassEncoder::SetPipeline & info ) { list->SetGraphicsRootSignature( info.pipeline->Desc.Layout->Raw.Get() ); list->SetPipelineState( info.pipeline->Raw.Get() ); for ( size_t i = 0; i < VertexBuffers.size(); i++ ) { VertexBuffers[i].StrideInBytes = info.pipeline->Desc.Vertex.Buffers[i].ArrayStride; } },
			[&]( const XE::GraphicsRenderPassEncoder::SetScissorRect & info ) { list->RSSetScissorRects( 1, &info.rect ); },
			[&]( const XE::GraphicsRenderPassEncoder::SetStencilReference & info ) { list->OMSetStencilRef( info.reference ); },
			[&]( const XE::GraphicsRenderPassEncoder::SetVertexBuffer & info ) { if ( VertexBuffers.size() <= info.slot ) VertexBuffers.resize( (XE::uint64)info.slot + 1 ); VertexBuffers[info.slot].BufferLocation = info.buffer->Raw->GetGPUVirtualAddress() + info.offset; VertexBuffers[info.slot].SizeInBytes = info.size; VertexBuffers[info.slot].StrideInBytes; },
			[&]( const XE::GraphicsRenderPassEncoder::SetViewport & info ) { list->RSSetViewports( 1, &info.viewport ); } }
		, render_pass_encoder->CommandInfos.front() );

		render_pass_encoder->CommandInfos.pop();
	}

	list->SetDescriptorHeaps( 0, nullptr );
	list->EndEvent();
}

void XE::GraphicsServiceD3D12::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	render_pass_encoder->CommandInfos.push( XE::GraphicsRenderPassEncoder::EndOcclusionQuery() );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	render_pass_encoder->CommandInfos.push( XE::GraphicsRenderPassEncoder::EndPipelineStatisticsQuery() );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{
	XE::GraphicsRenderPassEncoder::ExecuteBundles info;
	{
		info.bundles = bundles;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{
	XE::GraphicsRenderPassEncoder::InsertDebugMarker info;
	{
		info.marker_label = marker_label;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	render_pass_encoder->CommandInfos.push( XE::GraphicsRenderPassEncoder::PopDebugGroup() );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{
	XE::GraphicsRenderPassEncoder::PushDebugGroup info;
	{
		info.group_label = group_label;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	XE::GraphicsRenderPassEncoder::SetBindGroup info;
	{
		info.group_index = group_index;
		info.group = group;
		info.dynamic_offsets = dynamic_offsets;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{
	XE::GraphicsRenderPassEncoder::SetBlendConstant info;
	{
		info.color = color;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	XE::GraphicsRenderPassEncoder::SetIndexBuffer info;
	{
		info.buffer = buffer;
		info.format = format;
		info.offset = offset;
		info.size = size;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{
	XE::GraphicsRenderPassEncoder::SetPipeline info;
	{
		info.pipeline = pipeline;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{
	XE::GraphicsRenderPassEncoder::SetScissorRect info;
	{
		info.rect.left = rect.GetLeft();
		info.rect.right = rect.GetRight();
		info.rect.top = rect.GetTop();
		info.rect.bottom = rect.GetBottom();
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{
	XE::GraphicsRenderPassEncoder::SetStencilReference info;
	{
		info.reference = reference;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	XE::GraphicsRenderPassEncoder::SetVertexBuffer info;
	{
		info.slot = slot;
		info.size = size;
		info.buffer = buffer;
		info.offset = offset;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{
	XE::GraphicsRenderPassEncoder::SetViewport info;
	{
		info.viewport.TopLeftX = x;
		info.viewport.TopLeftY = y;
		info.viewport.Width = width;
		info.viewport.Height = height;
		info.viewport.MinDepth = min_depth;
		info.viewport.MaxDepth = max_depth;
	}
	render_pass_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	XE::GraphicsRenderBundleEncoder::Draw info;
	{
		info.vertex_count = vertex_count;
		info.instance_count = instance_count;
		info.first_vertex = first_vertex;
		info.first_instance = first_instance;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	XE::GraphicsRenderBundleEncoder::DrawIndexed info;
	{
		info.index_count = index_count;
		info.instance_count = instance_count;
		info.first_index = first_index;
		info.base_vertex = base_vertex;
		info.first_instance = first_instance;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	XE::GraphicsRenderBundleEncoder::DrawIndexedIndirect info;
	{
		info.indirect_buffer = indirect_buffer;
		info.indirect_offset = indirect_offset;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	XE::GraphicsRenderBundleEncoder::DrawIndirect info;
	{
		info.indirect_buffer = indirect_buffer;
		info.indirect_offset = indirect_offset;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

XE::GraphicsRenderBundlePtr XE::GraphicsServiceD3D12::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	auto bundle = _p->_RenderBundles.Alloc();

	bundle->Desc = descriptor;
	bundle->Parent = render_bundle_encoder->Parent;

	if ( SUCCEEDED( bundle->Parent->Raw->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS( bundle->CommandAllocator.GetAddressOf() ) ) ) )
	{
		if ( SUCCEEDED( bundle->Parent->Raw->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_BUNDLE, bundle->CommandAllocator.Get(), nullptr, IID_PPV_ARGS( bundle->Raw.GetAddressOf() ) ) ) )
		{
			XE::Array< D3D12_VERTEX_BUFFER_VIEW > VertexBuffers( XE::MemoryResource::GetFrameMemoryResource() );
			XE::Stack< XE::GraphicsRenderBundleEncoder::BeginOcclusionQuery > OcclusionQueryStack( XE::MemoryResource::GetFrameMemoryResource() );
			XE::Stack< XE::GraphicsRenderBundleEncoder::BeginPipelineStatisticsQuery > PipelineStatisticsQueryStack( XE::MemoryResource::GetFrameMemoryResource() );

			D3D12CommandListPtr list = bundle->Raw;
			
			while ( !render_bundle_encoder->CommandInfos.empty() )
			{
				std::visit( XE::Overloaded{
					[&]( const std::monostate & info ) {},
					[&]( const XE::GraphicsRenderBundleEncoder::BeginOcclusionQuery & info ) { OcclusionQueryStack.push( info ); list->BeginQuery( info.query_set->Raw.Get(), D3D12_QUERY_TYPE_OCCLUSION, info.query_index ); },
					[&]( const XE::GraphicsRenderBundleEncoder::BeginPipelineStatisticsQuery & info ) { PipelineStatisticsQueryStack.push( info ); list->BeginQuery( info.query_set->Raw.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, info.query_index ); },
					[&]( const XE::GraphicsRenderBundleEncoder::Draw & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() ); list->DrawInstanced( info.vertex_count, info.instance_count, info.first_vertex, info.first_instance ); },
					[&]( const XE::GraphicsRenderBundleEncoder::DrawIndexed & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() );  list->DrawIndexedInstanced( info.index_count, info.instance_count, info.first_index, info.base_vertex, info.first_instance ); },
					[&]( const XE::GraphicsRenderBundleEncoder::DrawIndexedIndirect & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() );  list->ExecuteIndirect( bundle->Parent->DrawIndexedIndirectSignature.Get(), 1, info.indirect_buffer->Raw.Get(), info.indirect_offset, nullptr, 0 ); },
					[&]( const XE::GraphicsRenderBundleEncoder::DrawIndirect & info ) { list->IASetVertexBuffers( 0, VertexBuffers.size(), VertexBuffers.data() );  list->ExecuteIndirect( bundle->Parent->DrawIndirectSignature.Get(), 1, info.indirect_buffer->Raw.Get(), info.indirect_offset, nullptr, 0 ); },
					[&]( const XE::GraphicsRenderBundleEncoder::InsertDebugMarker & info ) { list->SetMarker( 0, info.marker_label.c_str(), info.marker_label.size() ); },
					[&]( const XE::GraphicsRenderBundleEncoder::PopDebugGroup & info ) { list->EndEvent(); },
					[&]( const XE::GraphicsRenderBundleEncoder::PushDebugGroup & info ) { list->BeginEvent( 0, info.group_label.c_str(), info.group_label.size() ); },
					[&]( const XE::GraphicsRenderBundleEncoder::SetBindGroup & info ) { /* TODO */ },
					[&]( const XE::GraphicsRenderBundleEncoder::SetIndexBuffer & info ) { D3D12_INDEX_BUFFER_VIEW view{ info.buffer->Raw->GetGPUVirtualAddress() + info.offset, (UINT)info.size, Cast( info.format ) }; list->IASetIndexBuffer( &view ); },
					[&]( const XE::GraphicsRenderBundleEncoder::SetPipeline & info ) { list->SetGraphicsRootSignature( info.pipeline->Desc.Layout->Raw.Get() ); list->SetPipelineState( info.pipeline->Raw.Get() ); for ( size_t i = 0; i < VertexBuffers.size(); i++ ) { VertexBuffers[i].StrideInBytes = info.pipeline->Desc.Vertex.Buffers[i].ArrayStride; } },
					[&]( const XE::GraphicsRenderBundleEncoder::SetVertexBuffer & info ) { if ( VertexBuffers.size() <= info.slot ) VertexBuffers.resize( (XE::uint64)info.slot + 1 ); VertexBuffers[info.slot].BufferLocation = info.buffer->Raw->GetGPUVirtualAddress() + info.offset; VertexBuffers[info.slot].SizeInBytes = info.size; VertexBuffers[info.slot].StrideInBytes; } }
				, render_bundle_encoder->CommandInfos.front() );

				render_bundle_encoder->CommandInfos.pop();
			}

			return bundle;
		}
	}

	return {};
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{
	XE::GraphicsRenderBundleEncoder::InsertDebugMarker info;
	{
		info.marker_label = marker_label;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{
	render_bundle_encoder->CommandInfos.push( XE::GraphicsRenderBundleEncoder::PopDebugGroup() );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{
	XE::GraphicsRenderBundleEncoder::PushDebugGroup info;
	{
		info.group_label = group_label;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	XE::GraphicsRenderBundleEncoder::SetBindGroup info;
	{
		info.group_index = group_index;
		info.group = group;
		info.dynamic_offsets = dynamic_offsets;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	XE::GraphicsRenderBundleEncoder::SetIndexBuffer info;
	{
		info.buffer = buffer;
		info.format = format;
		info.offset = offset;
		info.size = size;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{
	XE::GraphicsRenderBundleEncoder::SetPipeline info;
	{
		info.pipeline = pipeline;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

void XE::GraphicsServiceD3D12::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	XE::GraphicsRenderBundleEncoder::SetVertexBuffer info;
	{
		info.slot = slot;
		info.size = size;
		info.buffer = buffer;
		info.offset = offset;
	}
	render_bundle_encoder->CommandInfos.push( info );
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceD3D12::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	return compute_pipeline->Desc.Layout->Desc.BindGroupLayouts[group_index];
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceD3D12::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	return render_pipeline->Desc.Layout->Desc.BindGroupLayouts[group_index];
}

void XE::GraphicsServiceD3D12::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, XE::GraphicsService::CompilationInfoCallback callback )
{
	callback( XE::GraphicsCompilationInfoRequestStatus::SUCCESS, {} );
}

void XE::GraphicsServiceD3D12::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{
	shader_module->Desc.Label = label;
}

XE::GraphicsTextureFormat XE::GraphicsServiceD3D12::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	return GraphicsTextureFormat::RGBA8SNORM;
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceD3D12::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	return swap_chain->TextureViews[swap_chain->Raw->GetCurrentBackBufferIndex()];
}

void XE::GraphicsServiceD3D12::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{
	UINT interval = 0;
	UINT flags = DXGI_PRESENT_ALLOW_TEARING;

	switch ( swap_chain->Desc.PresentMode )
	{
	case XE::GraphicsPresentMode::IMMEDIATE:
		interval = 0;
		flags = DXGI_PRESENT_ALLOW_TEARING;
		break;
	case XE::GraphicsPresentMode::MAILBOX:
	case XE::GraphicsPresentMode::FIFO:
		interval = 1;
		flags = 0;
		break;
	}

	swap_chain->Raw->Present( interval, flags );
}

#endif // GRAPHICS_API == GRAPHICS_D3D12