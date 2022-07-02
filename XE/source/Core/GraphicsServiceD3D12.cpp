#include "GraphicsService.h"

#if GRAPHICS_API == GRAPHICS_D3D12

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

IMPLEMENT_META( XE::GraphicsService );

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

	template< typename T > XE::Handle< T > Cast( T * ptr )
	{
		return XE::HandleCast< T >( reinterpret_cast<XE::uint64>( ptr ) );
	}
	template< typename T > T * Cast( XE::Handle< T > handle )
	{
		return reinterpret_cast<T *>( handle.GetValue() );
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

				if ( FAILED( ::D3DCompile( shader_desc.Code.data(), shader_desc.Code.size(), shader_desc.Label.c_str(), nullptr, nullptr, ep.EntryPoint.c_str(), target, flags, 0, code.GetAddressOf(), errmsg.GetAddressOf() ) ) )
				{
					XE_ERROR( "compile shader {%0} error: {%1}", shader_desc.Label, (const char *)errmsg->GetBufferPointer() );
					return nullptr;
				}
				else
				{
					return code;
				}
			}
		}

		return nullptr;
	}

	template< typename T > class RefHandle : public XE::RefCount
	{
	public:
		operator bool() const
		{
			return Count() != 0;
		}

		operator XE::Handle< T >() const
		{
			return Handle;
		}

	public:
		XE::Handle< T > IncHandle()
		{
			Inc();

			return Handle;
		}

	public:
		XE::Handle< T > Handle;
	};
	template< typename T, XE::uint64 S > class RefHandleArray
	{
	public:
		using iterator = std::array< T, S >::iterator;

	public:
		T & Alloc()
		{
			auto handle = _Handles.Alloc();
			_Datas[handle.GetValue()].Inc();
			_Datas[handle.GetValue()].Handle = handle;
			return _Datas[handle.GetValue()];
		}

		void Free( XE::Handle< T > handle )
		{
			_Datas[handle.GetValue()].Handle = {};
			_Handles.Free( handle );
		}

		T & operator []( XE::Handle< T > handle )
		{
			return _Datas[handle.GetValue()];
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
		XE::QueueHandleAllocator< XE::Handle< T >, S > _Handles;
	};

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
	class GraphicsSurface : public RefHandle< XE::GraphicsSurface >
	{
	public:
		XE::GraphicsSurfaceDescriptor Desc;

		XE::GraphicsSwapChainHandle SwapChain;
	};
	class GraphicsSwapChain : public RefHandle< XE::GraphicsSwapChain >
	{
	public:
		XE::GraphicsSwapChainDescriptor Desc;

		D3D12SwapChainPtr SwapChain = nullptr;

		XE::GraphicsSurfaceHandle Parent;
	};
	class GraphicsAdapter : public RefHandle< XE::GraphicsAdapter >
	{
	public:
		XE::uint64 Desc;

		D3D12AdapterPtr Adapter = nullptr;

		XE::GraphicsDeviceHandle Device;
	};
	class GraphicsDevice : public RefHandle< XE::GraphicsDevice >
	{
	public:
		XE::GraphicsDeviceDescriptor Desc;

		D3D12FencePtr Fence = nullptr;
		D3D12DevicePtr Device = nullptr;

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

		XE::ConcurrentQueue<D3D12CommandAllocatorPtr> CommandAllocators;

		XE::GraphicsQueueHandle Queue;
		XE::GraphicsAdapterHandle Parent;
	};
	class GraphicsQueue : public RefHandle< XE::GraphicsQueue >
	{
	public:
		XE::GraphicsQueueDescriptor Desc;

		D3D12CommandQueuePtr CommandQueue = nullptr;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsBindGroup : public RefHandle< XE::GraphicsBindGroup >
	{
	public:
		XE::GraphicsBindGroupDescriptor Desc;

		D3D12DescriptorHeapPtr ViewHeap = nullptr;
		D3D12DescriptorHeapPtr SamplerHeap = nullptr;
		D3D12RootSignaturePtr RootSignature = nullptr;
		D3D12CommandSignaturePtr CommandSignature = nullptr;

		XE::ConcurrentQueue<D3D12_GPU_VIRTUAL_ADDRESS> DynamicBuffers;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsBindGroupLayout : public RefHandle< XE::GraphicsBindGroupLayout >
	{
	public:
		XE::GraphicsBindGroupLayoutDescriptor Desc;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsBuffer : public RefHandle< XE::GraphicsBuffer >
	{
	public:
		XE::GraphicsBufferDescriptor Desc;

		D3D12_RANGE MapRange = {};
		D3D12ResourcePtr Resource = nullptr;
		XE::Array<XE::uint8> _MapData;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsCommandBuffer : public RefHandle< XE::GraphicsCommandBuffer >
	{
	public:
		XE::GraphicsCommandBufferDescriptor Desc;

		D3D12GraphicsCommandListPtr CommandList = nullptr;

		XE::GraphicsCommandEncoderHandle Parent;
	};
	class GraphicsCommandEncoder : public RefHandle< XE::GraphicsCommandEncoder >
	{
	public:
		XE::GraphicsCommandEncoderDescriptor Desc;
		
		D3D12CommandAllocatorPtr CommandAllocator;

		D3D12CommandListPtr CurrentList;

		XE::ConcurrentQueue< D3D12CommandListPtr > FreeLists;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsComputePassEncoder : public RefHandle< XE::GraphicsComputePassEncoder >
	{
	public:
		XE::GraphicsComputePassDescriptor Desc;

		D3D12CommandListPtr List;

		XE::uint32 QueryIndex = 0;
		XE::GraphicsQuerySetHandle QuerySet;

		XE::GraphicsCommandEncoderHandle Parent;
	};
	class GraphicsComputePipeline : public RefHandle< XE::GraphicsComputePipeline >
	{
	public:
		XE::GraphicsComputePipelineDescriptor Desc;

		D3D12BlobPtr ShaderCode = nullptr;
		D3D12PipelineStatePtr PipelineState = nullptr;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsPipelineLayout : public RefHandle< XE::GraphicsPipelineLayout >
	{
	public:
		XE::GraphicsPipelineLayoutDescriptor Desc;

		D3D12RootSignaturePtr RootSignature;
	};
	class GraphicsQuerySet : public RefHandle< XE::GraphicsQuerySet >
	{
	public:
		XE::GraphicsQuerySetDescriptor Desc;

		D3D12QueryHeapPtr QueryHeap = nullptr;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsRenderBundle : public RefHandle< XE::GraphicsRenderBundle >
	{
	public:
		XE::GraphicsRenderBundleDescriptor Desc;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsRenderBundleEncoder : public RefHandle< XE::GraphicsRenderBundleEncoder >
	{
	public:
		XE::GraphicsRenderBundleEncoderDescriptor Desc;

		XE::GraphicsCommandEncoderHandle Encoder;
	};
	class GraphicsRenderPassEncoder : public RefHandle< XE::GraphicsRenderPassEncoder >
	{
	public:
		XE::GraphicsRenderPassDescriptor Desc;

		D3D12CommandListPtr List;

		XE::GraphicsCommandEncoderHandle Parent;
	};
	class GraphicsRenderPipeline : public RefHandle< XE::GraphicsRenderPipeline >
	{
	public:
		XE::GraphicsRenderPipelineDescriptor Desc;

		D3D12BlobPtr VSCode = nullptr;
		D3D12BlobPtr FSCode = nullptr;
		D3D12PipelineStatePtr PipelineState = nullptr;
		XE::Array<D3D12_INPUT_ELEMENT_DESC> Elements;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsSampler : public RefHandle< XE::GraphicsSampler >
	{
	public:
		XE::GraphicsSamplerDescriptor Desc;

		XE::Handle< int > SamplerHeapHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle = { 0 };

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsShaderModule : public RefHandle< XE::GraphicsShaderModule >
	{
	public:
		XE::GraphicsShaderModuleDescriptor Desc;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsTexture : public RefHandle< XE::GraphicsTexture >
	{
	public:
		XE::GraphicsTextureDescriptor Desc;

		D3D12ResourcePtr Resource;

		XE::GraphicsDeviceHandle Parent;
	};
	class GraphicsTextureView : public RefHandle< XE::GraphicsTextureView >
	{
	public:
		XE::GraphicsTextureViewDescriptor Desc;

		D3D12_CPU_DESCRIPTOR_HANDLE SRVCPUHandle = { 0 };
		D3D12_CPU_DESCRIPTOR_HANDLE UAVCPUHandle = { 0 };

		XE::GraphicsDeviceHandle Parent;
	};
}

struct XE::GraphicsService::Private
{
	D3D12DebugPtr _Debug;
	D3D12FactoryPtr _Factory;

	RefHandleArray< XE::GraphicsSurface, XE::GRAPHICS_MAX_SURFACE > _Surfaces;
	RefHandleArray< XE::GraphicsSwapChain, XE::GRAPHICS_MAX_SWAP_CHAIN > _SwapChains;
	RefHandleArray< XE::GraphicsAdapter, XE::GRAPHICS_MAX_ADAPTER > _Adapters;
	RefHandleArray< XE::GraphicsDevice, XE::GRAPHICS_MAX_DEVICE > _Devices;
	RefHandleArray< XE::GraphicsQueue, XE::GRAPHICS_MAX_QUEUE > _Queues;
	RefHandleArray< XE::GraphicsBindGroup, XE::GRAPHICS_MAX_BIND_GROUP > _BindGroups;
	RefHandleArray< XE::GraphicsBindGroupLayout, XE::GRAPHICS_MAX_BIND_GROUP_LAYOUT > _BindGroupLayouts;
	RefHandleArray< XE::GraphicsBuffer, XE::GRAPHICS_MAX_BUFFER > _Buffers;
	RefHandleArray< XE::GraphicsCommandBuffer, XE::GRAPHICS_MAX_COMMAND_BUFFER > _CommandBuffers;
	RefHandleArray< XE::GraphicsCommandEncoder, XE::GRAPHICS_MAX_COMMAND_ENCODER > _CommandEncoders;
	RefHandleArray< XE::GraphicsComputePassEncoder, XE::GRAPHICS_MAX_COMPUTE_PASS_ENCODER > _ComputePassEncoders;
	RefHandleArray< XE::GraphicsComputePipeline, XE::GRAPHICS_MAX_COMPUTE_PIPELINE > _ComputePipelines;
	RefHandleArray< XE::GraphicsPipelineLayout, XE::GRAPHICS_MAX_PIPELINE_LAYOUT > _PipelineLayouts;
	RefHandleArray< XE::GraphicsQuerySet, XE::GRAPHICS_MAX_QUERY_SET > _QuerySets;
	RefHandleArray< XE::GraphicsRenderBundle, XE::GRAPHICS_MAX_RENDER_BUNDLE > _RenderBundles;
	RefHandleArray< XE::GraphicsRenderBundleEncoder, XE::GRAPHICS_MAX_RENDER_BUNDLE_ENCODER > _RenderBundleEncoders;
	RefHandleArray< XE::GraphicsRenderPassEncoder, XE::GRAPHICS_MAX_RENDER_PASS_ENCODER > _RenderPassEncoders;
	RefHandleArray< XE::GraphicsRenderPipeline, XE::GRAPHICS_MAX_RENDER_PIPELINE > _RenderPipelines;
	RefHandleArray< XE::GraphicsShaderModule, XE::GRAPHICS_MAX_SHADER_MODULE > _ShaderModules;
	RefHandleArray< XE::GraphicsSampler, XE::GRAPHICS_MAX_SAMPLER > _Samplers;
	RefHandleArray< XE::GraphicsTexture, XE::GRAPHICS_MAX_TEXTURE > _Textures;
	RefHandleArray< XE::GraphicsTextureView, XE::GRAPHICS_MAX_TEXTURE_VIEW > _TextureViews;
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
	XE::uint32 flags = 0;

	if ( GetFramework()->GetBool( CONFIG_GRAPHICS_DEBUG ) )
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

void XE::GraphicsService::Startup()
{

}

void XE::GraphicsService::Update()
{

}

void XE::GraphicsService::Clearup()
{
	_p->_Factory = nullptr;
	_p->_Debug = nullptr;
}

XE::GraphicsSurfaceHandle XE::GraphicsService::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	auto & surface = _p->_Surfaces.Alloc();

	surface.Desc = descriptor;

	return surface;
}

void XE::GraphicsService::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, RequestAdapterCallback callback )
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
		auto & ada = _p->_Adapters.Alloc();

		ada.Adapter = adapter;

		ada.Desc = i;

		callback( XE::GraphicsRequestAdapterStatus::SUCCESS, ada );
	}
}

void XE::GraphicsService::AdapterEnumerateFeatures( XE::GraphicsAdapterHandle adapter, XE::Array< XE::GraphicsFeatureName > & features )
{
	auto & ada = _p->_Adapters[adapter];
	if ( ada.Count() != 0 )
	{
		features.push_back( XE::GraphicsFeatureName::DEPTH_CLIP_CONTROL );
		features.push_back( XE::GraphicsFeatureName::DEPTH24UNORM_STENCIL8 );
		features.push_back( XE::GraphicsFeatureName::DEPTH32FLOAT_STENCIL8 );
		features.push_back( XE::GraphicsFeatureName::INDIRECT_FIRST_INSTANCE );
		features.push_back( XE::GraphicsFeatureName::TIMESTAMP_QUERY );
		features.push_back( XE::GraphicsFeatureName::PIPELINE_STATISTICS_QUERY );
		features.push_back( XE::GraphicsFeatureName::TEXTURE_COMPRESSION_BC );
	}
}

bool XE::GraphicsService::AdapterGetLimits( XE::GraphicsAdapterHandle adapter, XE::GraphicsSupportedLimits & limits )
{
	auto & ada = _p->_Adapters[adapter];
	if ( ada.Count() != 0 )
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

	return false;
}

void XE::GraphicsService::AdapterGetProperties( XE::GraphicsAdapterHandle adapter, XE::GraphicsAdapterProperties & properties )
{
	auto & ada = _p->_Adapters[adapter];
	if ( ada.Count() != 0 )
	{
		DXGI_ADAPTER_DESC1 desc1;
		if ( SUCCEEDED( ada.Adapter->GetDesc1( &desc1 ) ) )
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
				if ( ada.Device && SUCCEEDED( _p->_Devices[ada.Device].Device->CheckFeatureSupport( D3D12_FEATURE_ARCHITECTURE, &arch, sizeof( arch ) ) ) )
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
}

bool XE::GraphicsService::AdapterHasFeature( XE::GraphicsAdapterHandle adapter, XE::GraphicsFeatureName feature )
{
	XE::Array< XE::GraphicsFeatureName > features;
	AdapterEnumerateFeatures( adapter, features );
	return std::find( features.begin(), features.end(), feature ) != features.end();
}

void XE::GraphicsService::AdapterRequestDevice( XE::GraphicsAdapterHandle adapter, const XE::GraphicsDeviceDescriptor & descriptor, RequestDeviceCallback callback )
{
	auto & ada = _p->_Adapters[adapter];
	if ( ada.Count() != 0 )
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
			if ( SUCCEEDED( ::D3D12CreateDevice( ada.Adapter.Get(), levels[i], IID_PPV_ARGS( device.GetAddressOf() ) ) ) )
			{
				auto & dev = _p->_Devices.Alloc();

				dev.Device = device;
				dev.Parent = ada.IncHandle();
				ada.Device = dev;

				D3D12FencePtr fence;
				if ( SUCCEEDED( device->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( fence.GetAddressOf() ) ) ) )
				{
					dev.Fence = fence;
				}

				D3D12_COMMAND_QUEUE_DESC queue_desc = {};
				{
					queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
					queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
					D3D12CommandQueuePtr command_queue;
					if ( SUCCEEDED( device->CreateCommandQueue( &queue_desc, IID_PPV_ARGS( command_queue.GetAddressOf() ) ) ) )
					{
						auto & que = _p->_Queues.Alloc();
						que.CommandQueue = command_queue;
						que.Parent = dev.IncHandle();
						que.Desc = descriptor.DefaultQueue;
					}
				}

				D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc = {};
				{
					desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
					desc_heap_desc.NumDescriptors = GRAPHICS_MAX_CSU_DESCRIPTOR_HEAP;
					desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
					desc_heap_desc.NodeMask = 0;
					device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev.CSUHeap.GetAddressOf() ) );

					desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
					desc_heap_desc.NumDescriptors = GRAPHICS_MAX_RTV_DESCRIPTOR_HEAP;
					desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
					desc_heap_desc.NodeMask = 0;
					device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev.RTVHeap.GetAddressOf() ) );

					desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
					desc_heap_desc.NumDescriptors = GRAPHICS_MAX_DSV_DESCRIPTOR_HEAP;
					desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
					desc_heap_desc.NodeMask = 0;
					device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev.DSVHeap.GetAddressOf() ) );

					desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
					desc_heap_desc.NumDescriptors = GRAPHICS_MAX_SAMPLER_DESCRIPTOR_HEAP;
					desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
					desc_heap_desc.NodeMask = 0;
					device->CreateDescriptorHeap( &desc_heap_desc, IID_PPV_ARGS( dev.SmaplerHeap.GetAddressOf() ) );
				}

				D3D12_COMMAND_SIGNATURE_DESC signature_desc = {};
				{
					D3D12_INDIRECT_ARGUMENT_DESC argument_desc = {};
					signature_desc.NumArgumentDescs = 1;
					signature_desc.pArgumentDescs = &argument_desc;

					argument_desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
					signature_desc.ByteStride = 4 * sizeof( uint32_t );
					device->CreateCommandSignature( &signature_desc, nullptr, IID_PPV_ARGS( dev.DrawIndirectSignature.GetAddressOf() ) );

					argument_desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
					signature_desc.ByteStride = 3 * sizeof( uint32_t );
					device->CreateCommandSignature( &signature_desc, nullptr, IID_PPV_ARGS( dev.DispatchIndirectSignature.GetAddressOf() ) );

					argument_desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
					signature_desc.ByteStride = 5 * sizeof( uint32_t );
					device->CreateCommandSignature( &signature_desc, nullptr, IID_PPV_ARGS( dev.DrawIndexedIndirectSignature.GetAddressOf() ) );
				}

				for ( size_t i = 0; i < GRAPHICS_MAX_COMMAND_ALLOCATOR; i++ )
				{
					D3D12CommandAllocatorPtr allocator;
					if ( SUCCEEDED( device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( allocator.GetAddressOf() ) ) ) )
					{
						dev.CommandAllocators.push( allocator );
					}
				}

				break;
			}
		}

		if ( ada.Device )
		{
			callback( XE::GraphicsRequestDeviceStatus::SUCCESS, ada.Device );
		}
		else
		{
			callback( XE::GraphicsRequestDeviceStatus::ERROR, ada.Device );
		}
	}
}

XE::GraphicsBindGroupHandle XE::GraphicsService::DeviceCreateBindGroup( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		if ( auto & layout = _p->_BindGroupLayouts[descriptor.Layout] )
		{
			auto & group = _p->_BindGroups.Alloc();

			group.Desc = descriptor;
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

					dev.Device->CreateDescriptorHeap( &desc, IID_PPV_ARGS( group.ViewHeap.GetAddressOf() ) );
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

					dev.Device->CreateDescriptorHeap( &desc, IID_PPV_ARGS( group.SamplerHeap.GetAddressOf() ) );
				}
			}

			XE::uint32 view_count = 0;
			XE::uint32 sampler_count = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE view_handle = group.ViewHeap->GetCPUDescriptorHandleForHeapStart();
			D3D12_CPU_DESCRIPTOR_HANDLE sampler_handle = group.SamplerHeap->GetCPUDescriptorHandleForHeapStart();
			XE::uint32 view_item_size = dev.Device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
			XE::uint32 sampler_item_size = dev.Device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );

			for ( size_t i = 0; i < descriptor.Entries.size(); i++ )
			{
				const auto & group_entry = descriptor.Entries[i];
				const auto & layout_entry = layout.Desc.Entries[i];

				if ( group_entry.Buffer )
				{
					if ( auto & buf = _p->_Buffers[group_entry.Buffer] )
					{
						D3D12_GPU_VIRTUAL_ADDRESS gpu_address = buf.Resource->GetGPUVirtualAddress();
						D3D12_CPU_DESCRIPTOR_HANDLE cpu_address = { view_handle.ptr + ( view_count++ * view_item_size ) };

						if ( layout_entry.Buffer.HasDynamicOffset )
						{
							group.DynamicBuffers.push( gpu_address + group_entry.Offset );
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

								dev.Device->CreateConstantBufferView( &desc, cpu_address );
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

								dev.Device->CreateUnorderedAccessView( buf.Resource.Get(), nullptr, &desc, cpu_address );
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

									dev.Device->CreateShaderResourceView( buf.Resource.Get(), &desc, cpu_address );
								}
							}
								break;
							default:
								break;
							}
						}
					}
					else if ( auto & sampler = _p->_Samplers[group_entry.Sampler] )
					{
						D3D12_CPU_DESCRIPTOR_HANDLE cpu_address = { sampler_handle.ptr + ( sampler_count++ * sampler_item_size ) };

						dev.Device->CopyDescriptorsSimple( 1, cpu_address, sampler.CPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );
					}
					else if ( auto & texture = _p->_TextureViews[group_entry.TextureView] )
					{
						D3D12_CPU_DESCRIPTOR_HANDLE cpu_address = { view_handle.ptr + ( view_count++ * view_item_size ) };

						if ( layout_entry.StorageTexture.Access == XE::GraphicsStorageTextureAccess::UNDEFINED )
						{
							dev.Device->CopyDescriptorsSimple( 1, cpu_address, texture.SRVCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
						}
						else
						{
							dev.Device->CopyDescriptorsSimple( 1, cpu_address, texture.UAVCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
						}
					}
				}
			}

			return group;
		}
	}
	
	return {};
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::DeviceCreateBindGroupLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & layout = _p->_BindGroupLayouts.Alloc();

		layout.Desc = descriptor;

		return layout;
	}

	return {};
}

XE::GraphicsBufferHandle XE::GraphicsService::DeviceCreateBuffer( XE::GraphicsDeviceHandle device, const XE::GraphicsBufferDescriptor & descriptor )
{
	if( auto & dev = _p->_Devices[device] )
	{
		auto & buf = _p->_Buffers.Alloc();

		buf.Desc = descriptor;

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

		if ( SUCCEEDED( dev.Device->CreateCommittedResource( &heap_properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( buf.Resource.GetAddressOf() ) ) ) )
		{
			return buf;
		}
		else
		{
			_p->_Buffers.Free( buf );
		}
	}

	return {};
}

XE::GraphicsCommandEncoderHandle XE::GraphicsService::DeviceCreateCommandEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & encoder = _p->_CommandEncoders.Alloc();

		encoder.Desc = descriptor;

		encoder.Parent = dev.IncHandle();

		D3D12CommandAllocatorPtr allocator;
		if ( dev.CommandAllocators.try_pop( allocator ) )
		{
			encoder.CommandAllocator = allocator;
		}
		else if ( SUCCEEDED( dev.Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( allocator.GetAddressOf() ) ) ) )
		{
			encoder.CommandAllocator = allocator;
		}

		return encoder;
	}

	return {};
}

XE::GraphicsComputePipelineHandle XE::GraphicsService::DeviceCreateComputePipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		if ( auto & layout = _p->_PipelineLayouts[descriptor.Layout] )
		{
			if ( auto & cs = _p->_ShaderModules[descriptor.Compute.Shader] )
			{
				auto & pipe = _p->_ComputePipelines.Alloc();

				pipe.Desc = descriptor;
				pipe.ShaderCode = LoadShader( cs.Desc, descriptor.Compute.EntryPoint, XE::GraphicsShaderStage::COMPUTE );

				D3D12_COMPUTE_PIPELINE_STATE_DESC desc;
				{
					desc.pRootSignature = layout.RootSignature.Get();
					desc.CS = { pipe.ShaderCode->GetBufferPointer(), pipe.ShaderCode->GetBufferSize() };
					desc.NodeMask = 0;
					desc.CachedPSO = { nullptr, 0 };
					desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

					D3D12PipelineStatePtr state;
					if ( SUCCEEDED( dev.Device->CreateComputePipelineState( &desc, IID_PPV_ARGS( state.GetAddressOf() ) ) ) )
					{
						pipe.PipelineState = state;
					}
				}

				return pipe;
			}
		}
	}

	return {};
}

XE::GraphicsPipelineLayoutHandle XE::GraphicsService::DeviceCreatePipelineLayout( XE::GraphicsDeviceHandle device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & layout = _p->_PipelineLayouts.Alloc();

		layout.Desc = descriptor;

		// TODO: 

		return layout;
	}

	return {};
}

XE::GraphicsQuerySetHandle XE::GraphicsService::DeviceCreateQuerySet( XE::GraphicsDeviceHandle device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & query_set = _p->_QuerySets.Alloc();

		query_set.Desc = descriptor;

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
			dev.Device->CreateQueryHeap( &query_heap_desc, IID_PPV_ARGS( query_set.QueryHeap.GetAddressOf() ) );
		}

		return query_set;
	}

	return {};
}

XE::GraphicsRenderBundleEncoderHandle XE::GraphicsService::DeviceCreateRenderBundleEncoder( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & bundle = _p->_RenderBundleEncoders.Alloc();

		bundle.Desc = descriptor;

		// TODO: 

		return bundle;
	}

	return {};
}

XE::GraphicsRenderPipelineHandle XE::GraphicsService::DeviceCreateRenderPipeline( XE::GraphicsDeviceHandle device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		if ( auto & layout = _p->_PipelineLayouts[descriptor.Layout] )
		{
			if ( auto & vs = _p->_ShaderModules[descriptor.Vertex.Module] )
			{
				if ( auto & fs = _p->_ShaderModules[descriptor.Fragment.Module] )
				{
					auto & pipe = _p->_RenderPipelines.Alloc();

					pipe.Desc = descriptor;
					
					pipe.VSCode = LoadShader( vs.Desc, descriptor.Vertex.EntryPoint, XE::GraphicsShaderStage::VERTEX );
					pipe.FSCode = LoadShader( fs.Desc, descriptor.Fragment.EntryPoint, XE::GraphicsShaderStage::FRAGMENT );

					D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
					{
						desc.pRootSignature = layout.RootSignature.Get();
						desc.VS = { pipe.VSCode->GetBufferPointer(), pipe.VSCode->GetBufferSize() };
						desc.PS = { pipe.FSCode->GetBufferPointer(), pipe.FSCode->GetBufferSize() };
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
									pipe.Elements.push_back( element_desc );
								}
							}
							desc.InputLayout.NumElements = pipe.Elements.size();
							desc.InputLayout.pInputElementDescs = pipe.Elements.data();
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

					D3D12PipelineStatePtr state;
					if ( SUCCEEDED( dev.Device->CreateGraphicsPipelineState( &desc, IID_PPV_ARGS( state.GetAddressOf() ) ) ) )
					{
						pipe.PipelineState = state;
					}

					return pipe;
				}
			}
		}
	}

	return {};
}

XE::GraphicsSamplerHandle XE::GraphicsService::DeviceCreateSampler( XE::GraphicsDeviceHandle device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & sampler = _p->_Samplers.Alloc();

		sampler.Desc = descriptor;

		sampler.Parent = dev.IncHandle();

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

		sampler.SamplerHeapHandle = dev.SamplerAllocator.Alloc();
		sampler.CPUHandle.ptr = dev.SmaplerHeap->GetCPUDescriptorHandleForHeapStart().ptr + sampler.SamplerHeapHandle.GetValue() * dev.Device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER );

		dev.Device->CreateSampler( &desc, sampler.CPUHandle );

		return sampler;
	}

	return {};
}

XE::GraphicsShaderModuleHandle XE::GraphicsService::DeviceCreateShaderModule( XE::GraphicsDeviceHandle device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & shader = _p->_ShaderModules.Alloc();

		shader.Desc = descriptor;

		return shader;
	}

	return {};
}

XE::GraphicsSwapChainHandle XE::GraphicsService::DeviceCreateSwapChain( XE::GraphicsDeviceHandle device, XE::GraphicsSurfaceHandle surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & swap_chain = _p->_SwapChains.Alloc();

		swap_chain.Desc = descriptor;

		// TODO: 

		return swap_chain;
	}

	return {};
}

XE::GraphicsTextureHandle XE::GraphicsService::DeviceCreateTexture( XE::GraphicsDeviceHandle device, const XE::GraphicsTextureDescriptor & descriptor )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		auto & texture = _p->_Textures.Alloc();

		texture.Desc = descriptor;

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
		if ( SUCCEEDED( dev.Device->CreateCommittedResource( &heap_properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( resource.GetAddressOf() ) ) ) )
		{
			texture.Resource = resource;
		}

		return texture;
	}

	return {};
}

XE::GraphicsTextureViewHandle XE::GraphicsService::TextureCreateView( XE::GraphicsTextureHandle texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	if ( auto & tex = _p->_Textures[texture] )
	{
		auto & view = _p->_TextureViews.Alloc();

		view.Desc = descriptor;

		// TODO: 

		return view;
	}

	return {};
}

void XE::GraphicsService::DeviceEnumerateFeatures( XE::GraphicsDeviceHandle device, XE::Array< XE::GraphicsFeatureName > & features )
{
	auto & dev = _p->_Devices[device];
	{

	}
}

bool XE::GraphicsService::DeviceGetLimits( XE::GraphicsDeviceHandle device, XE::GraphicsSupportedLimits & limits )
{
	auto & dev = _p->_Devices[device];
	{

	}

	return {};
}

XE::GraphicsQueueHandle XE::GraphicsService::DeviceGetQueue( XE::GraphicsDeviceHandle device )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		return dev.Queue;
	}

	return {};
}

bool XE::GraphicsService::DeviceHasFeature( XE::GraphicsDeviceHandle device, XE::GraphicsFeatureName feature )
{
	auto & dev = _p->_Devices[device];
	{

	}

	return {};
}

bool XE::GraphicsService::DevicePopErrorScope( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	auto & dev = _p->_Devices[device];
	{

	}

	return {};
}

void XE::GraphicsService::DevicePushErrorScope( XE::GraphicsDeviceHandle device, XE::GraphicsErrorFilter filter )
{
	auto & dev = _p->_Devices[device];
	{

	}
}

void XE::GraphicsService::DeviceSetDeviceLostCallback( XE::GraphicsDeviceHandle device, DeviceLostCallback callback )
{
	auto & dev = _p->_Devices[device];
	{

	}
}

void XE::GraphicsService::DeviceSetUncapturedErrorCallback( XE::GraphicsDeviceHandle device, ErrorCallback callback )
{
	auto & dev = _p->_Devices[device];
	{

	}
}

void XE::GraphicsService::QueueOnSubmittedWorkDone( XE::GraphicsQueueHandle queue, QueueWorkDoneCallback callback )
{
	if ( auto & que = _p->_Queues[queue] )
	{

	}
}

void XE::GraphicsService::QueueSubmit( XE::GraphicsQueueHandle queue, const XE::Array< XE::GraphicsCommandBufferHandle > & commands )
{
	if ( auto & que = _p->_Queues[queue] )
	{

	}
}

void XE::GraphicsService::QueueWriteBuffer( XE::GraphicsQueueHandle queue, XE::GraphicsBufferHandle buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{
	auto & que = _p->_Queues[queue];
	{

	}
}

void XE::GraphicsService::QueueWriteTexture( XE::GraphicsQueueHandle queue, const XE::GraphicsImageCopyTexture & dst, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{
	auto & que = _p->_Queues[queue];
	{

	}
}

void XE::GraphicsService::BufferMapAsync( XE::GraphicsBufferHandle buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, BufferMapCallback callback, void * userdata )
{

}

XE::Span< const XE::uint8 > XE::GraphicsService::BufferGetConstMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	auto data = BufferGetMappedRange( buffer, offset, size );
	return { data.data(), data.size() };
}

XE::Span< XE::uint8 > XE::GraphicsService::BufferGetMappedRange( XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	if ( auto & buf = _p->_Buffers[buffer] )
	{
		buf.MapRange.Begin = offset;
		buf.MapRange.End = offset + size;

		void * data = nullptr;
		if ( SUCCEEDED( buf.Resource->Map( 0, &buf.MapRange, &data ) ) )
		{
			return { reinterpret_cast<XE::uint8 *>( data ), size };
		}
	}

	return {};
}

void XE::GraphicsService::BufferUnmap( XE::GraphicsBufferHandle buffer )
{
	if( auto & buf = _p->_Buffers[buffer] )
	{
		buf.Resource->Unmap( 0, &buf.MapRange );
	}
}

XE::GraphicsComputePassEncoderHandle XE::GraphicsService::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( auto & dev = _p->_Devices[cmd.Parent] )
		{
			D3D12CommandListPtr list;
			if ( cmd.CurrentList != nullptr )
			{
				list = cmd.CurrentList;
			}
			else if ( !cmd.FreeLists.try_pop( list ) )
			{
				if ( auto & dev = _p->_Devices[cmd.Parent] )
				{
					dev.Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmd.CommandAllocator.Get(), nullptr, IID_PPV_ARGS( list.GetAddressOf() ) );
					list->Close();
				}
			}

			ID3D12DescriptorHeap * heaps[2] = { dev.CSUHeap.Get(), dev.SmaplerHeap.Get() };

			list.Reset();
			list->BeginEvent( 0, cmd.Desc.Label.c_str(), cmd.Desc.Label.size() );
			list->SetDescriptorHeaps( 2, heaps );

			auto & pass = _p->_ComputePassEncoders.Alloc();

			pass.List = list;
			pass.Desc = descriptor;
			pass.Parent = cmd.IncHandle();

			return pass;
		}
	}

	return {};
}

XE::GraphicsRenderPassEncoderHandle XE::GraphicsService::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	if( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( auto & dev = _p->_Devices[cmd.Parent] )
		{
			D3D12CommandListPtr list;
			if ( cmd.CurrentList != nullptr )
			{
				list = cmd.CurrentList;
			}
			else if ( !cmd.FreeLists.try_pop( list ) )
			{
				if ( auto & dev = _p->_Devices[cmd.Parent] )
				{
					dev.Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmd.CommandAllocator.Get(), nullptr, IID_PPV_ARGS( list.GetAddressOf() ) );
					list->Close();
				}
			}

			ID3D12DescriptorHeap * heaps[2] = { dev.CSUHeap.Get(), dev.SmaplerHeap.Get() };

			list.Reset();
			list->BeginEvent( 0, cmd.Desc.Label.c_str(), cmd.Desc.Label.size() );
			list->SetDescriptorHeaps( 2, heaps );

			auto & pass = _p->_RenderPassEncoders.Alloc();

			pass.List = list;
			pass.Desc = descriptor;
			pass.Parent = cmd.IncHandle();

			return pass;
		}
	}

	return {};
}

void XE::GraphicsService::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsBufferHandle src, XE::uint64 src_offset, XE::GraphicsBufferHandle dst, XE::uint64 dst_offset, XE::uint64 size )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( auto & src_buf = _p->_Buffers[src] )
		{
			if ( auto & dst_buf = _p->_Buffers[dst] )
			{
				cmd.CurrentList->CopyBufferRegion( dst_buf.Resource.Get(), dst_offset, src_buf.Resource.Get(), src_offset, size );
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyBuffer & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( auto & src_buf = _p->_Buffers[src.Buffer] )
		{
			if ( auto & dst_tex = _p->_Textures[dst.Texture] )
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
					src_location.pResource = src_buf.Resource.Get();
					src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
					src_location.PlacedFootprint.Offset = src.Layout.Offset;
					src_location.PlacedFootprint.Footprint.Format = Cast( dst_tex.Desc.Format );
					src_location.PlacedFootprint.Footprint.Width = copy_size.x;
					src_location.PlacedFootprint.Footprint.Height = src.Layout.RowsPerImage;
					src_location.PlacedFootprint.Footprint.Depth = copy_size.z;
					src_location.PlacedFootprint.Footprint.RowPitch = src.Layout.BytesPerRow;
				}

				D3D12_TEXTURE_COPY_LOCATION dst_location = {};
				{
					dst_location.pResource = dst_tex.Resource.Get();
					dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
					int array_layer_count = dst_tex.Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : dst_tex.Desc.Size.z;
					dst_location.SubresourceIndex = dst.MipLevel + ( dst.Origin.z * array_layer_count ) * dst_tex.Desc.MipLevelCount;
				}

				cmd.CurrentList->CopyTextureRegion( &dst_location, dst.Origin.x, dst.Origin.y, dst.Origin.z, &src_location, &box );
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyBuffer & dst, const XE::Vec3f & copy_size )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( auto & src_tex = _p->_Textures[src.Texture] )
		{
			if ( auto & dst_buf = _p->_Buffers[dst.Buffer] )
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
					src_location.pResource = src_tex.Resource.Get();
					src_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
					int array_layer_count = src_tex.Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : src_tex.Desc.Size.z;
					src_location.SubresourceIndex = src.MipLevel + ( src.Origin.z * array_layer_count ) * src_tex.Desc.MipLevelCount;
				}

				D3D12_TEXTURE_COPY_LOCATION dst_location = {};
				{
					dst_location.pResource = dst_buf.Resource.Get();
					dst_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
					dst_location.PlacedFootprint.Offset = dst.Layout.Offset;
					dst_location.PlacedFootprint.Footprint.Format = Cast( src_tex.Desc.Format );
					dst_location.PlacedFootprint.Footprint.Width = copy_size.x;
					dst_location.PlacedFootprint.Footprint.Height = dst.Layout.RowsPerImage;
					dst_location.PlacedFootprint.Footprint.Depth = copy_size.z;
					dst_location.PlacedFootprint.Footprint.RowPitch = dst.Layout.BytesPerRow;
				}

				cmd.CurrentList->CopyTextureRegion( &dst_location, 0, 0, 0, &src_location, &box );
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( auto & src_tex = _p->_Textures[src.Texture] )
		{
			if ( auto & dst_tex = _p->_Textures[dst.Texture] )
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
					src_location.pResource = src_tex.Resource.Get();
					src_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
					int array_layer_count = src_tex.Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : src_tex.Desc.Size.z;
					src_location.SubresourceIndex = src.MipLevel + ( src.Origin.z * array_layer_count ) * src_tex.Desc.MipLevelCount;
				}

				D3D12_TEXTURE_COPY_LOCATION dst_location = {};
				{
					dst_location.pResource = dst_tex.Resource.Get();
					dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
					int array_layer_count = dst_tex.Desc.Dimension == XE::GraphicsTextureDimension::D3 ? 1 : dst_tex.Desc.Size.z;
					dst_location.SubresourceIndex = dst.MipLevel + ( dst.Origin.z * array_layer_count ) * dst_tex.Desc.MipLevelCount;
				}

				cmd.CurrentList->CopyTextureRegion( &dst_location, dst.Origin.x, dst.Origin.y, dst.Origin.z, &src_location, &box );
			}
		}
	}
}

XE::GraphicsCommandBufferHandle XE::GraphicsService::CommandEncoderFinish( XE::GraphicsCommandEncoderHandle command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		if ( cmd.CurrentList != nullptr )
		{
			cmd.CurrentList->Close();
			auto list = cmd.CurrentList;
			cmd.CurrentList = nullptr;

			auto & buf = _p->_CommandBuffers.Alloc();

			buf.Desc = descriptor;
			buf.CommandList = list;
			buf.Parent = cmd.IncHandle();

			return buf;
		}
	}

	return {};
}

void XE::GraphicsService::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & marker_label )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		cmd.CurrentList->SetMarker( 0, marker_label.c_str(), marker_label.size() );
	}
}

void XE::GraphicsService::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		cmd.CurrentList->EndEvent();
	}
}

void XE::GraphicsService::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderHandle command_encoder, const XE::String & group_label )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		cmd.CurrentList->BeginEvent( 0, group_label.c_str(), group_label.size() );
	}
}

void XE::GraphicsService::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferHandle dst, XE::uint64 dst_offset )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		auto & query = _p->_QuerySets[query_set];
		{
			auto & buf = _p->_Buffers[dst];
			{
				cmd.CurrentList->ResolveQueryData( query.QueryHeap.Get(), Cast( query.Desc.Type ), first_query, query_count, buf.Resource.Get(), dst_offset );
			}
		}
	}
}

void XE::GraphicsService::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderHandle command_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	if ( auto & cmd = _p->_CommandEncoders[command_encoder] )
	{
		auto & query = _p->_QuerySets[query_set];
		{
			cmd.CurrentList->EndQuery( query.QueryHeap.Get(), D3D12_QUERY_TYPE_TIMESTAMP, query_index );
		}
	}
}

void XE::GraphicsService::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			if( auto & query = _p->_QuerySets[query_set] )
			{
				pass.QuerySet = query_set;
				pass.QueryIndex = query_index;
				cmd.CurrentList->BeginQuery( query.QueryHeap.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, query_index );
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			cmd.CurrentList->Dispatch( workgroup_count_x, workgroup_count_y, workgroup_count_z );
		}
	}
}

void XE::GraphicsService::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			if ( auto & dev = _p->_Devices[cmd.Parent] )
			{
				if ( auto & buf = _p->_Buffers[indirect_buffer] )
				{
					cmd.CurrentList->ExecuteIndirect( dev.DispatchIndirectSignature.Get(), 1, buf.Resource.Get(), indirect_offset, nullptr, 0 );
				}
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			cmd.CurrentList->SetDescriptorHeaps( 0, nullptr );
			cmd.CurrentList->EndEvent();
		}
	}
}

void XE::GraphicsService::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			if ( auto & query = _p->_QuerySets[pass.QuerySet] )
			{
				cmd.CurrentList->EndQuery( query.QueryHeap.Get(), D3D12_QUERY_TYPE_PIPELINE_STATISTICS, pass.QueryIndex );

				pass.QuerySet = {};
				pass.QueryIndex = 0;
			}
		}
	}
}

void XE::GraphicsService::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & marker_label )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			cmd.CurrentList->SetMarker( 0, marker_label.c_str(), marker_label.size() );
		}
	}
}

void XE::GraphicsService::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			cmd.CurrentList->EndEvent();
		}
	}
}

void XE::GraphicsService::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, const XE::String & group_label )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			cmd.CurrentList->BeginEvent( 0, group_label.c_str(), group_label.size() );
		}
	}
}

void XE::GraphicsService::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	// TODO: 
}

void XE::GraphicsService::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderHandle compute_pass_encoder, XE::GraphicsComputePipelineHandle pipeline )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( auto & cmd = _p->_CommandEncoders[pass.Parent] )
		{
			if ( auto & pipe = _p->_ComputePipelines[pipeline] )
			{
				if ( auto & layout = _p->_PipelineLayouts[pipe.Desc.Layout] )
				{
					cmd.CurrentList->SetComputeRootSignature( layout.RootSignature.Get() );
					cmd.CurrentList->SetPipelineState( pipe.PipelineState.Get() );
				}
			}
		}
	}
}

void XE::GraphicsService::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 query_index )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsQuerySetHandle query_set, XE::uint32 query_index )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Array< XE::GraphicsRenderBundleHandle > & bundles )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & marker_label )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::String & group_label )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Color & color )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::GraphicsRenderPipelineHandle pipeline )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, const XE::Recti & rect )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 reference )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderHandle render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{
	auto & bundle_encoder = _p->_RenderPassEncoders[render_pass_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle indirect_buffer, XE::uint64 indirect_offset )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

XE::GraphicsRenderBundleHandle XE::GraphicsService::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}

	return {};
}

void XE::GraphicsService::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & marker_label )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, const XE::String & group_label )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupHandle group, XE::uint32 dynamic_offset_count, XE::uint32 & dynamic_offsets )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsBufferHandle buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::GraphicsRenderPipelineHandle pipeline )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

void XE::GraphicsService::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferHandle buffer, XE::uint64 offset, XE::uint64 size )
{
	auto & bundle_encoder = _p->_RenderBundleEncoders[render_bundle_encoder];
	{

	}
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelineHandle compute_pipeline, XE::uint32 group_index )
{
	if ( auto & pipe = _p->_ComputePipelines[compute_pipeline] )
	{
		// TODO: 
	}

	return {};
}

XE::GraphicsBindGroupLayoutHandle XE::GraphicsService::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelineHandle render_pipeline, XE::uint32 group_index )
{
	auto & pipe = _p->_RenderPipelines[render_pipeline];
	{

	}

	return {};
}

void XE::GraphicsService::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModuleHandle shader_module, CompilationInfoCallback callback )
{
	auto & shader = _p->_ShaderModules[shader_module];
	{

	}
}

void XE::GraphicsService::ShaderModuleSetLabel( XE::GraphicsShaderModuleHandle shader_module, const XE::String & label )
{
	auto & shader = _p->_ShaderModules[shader_module];
	{

	}
}

XE::GraphicsTextureFormat XE::GraphicsService::SurfaceGetPreferredFormat( XE::GraphicsSurfaceHandle surface, XE::GraphicsAdapterHandle adapter )
{
	auto & sur = _p->_Surfaces[surface];
	{

	}

	return {};
}

XE::GraphicsTextureViewHandle XE::GraphicsService::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainHandle swap_chain )
{
	auto & swap = _p->_SwapChains[swap_chain];
	{

	}

	return {};
}

void XE::GraphicsService::SwapChainPresent( XE::GraphicsSwapChainHandle swap_chain )
{
	auto & swap = _p->_SwapChains[swap_chain];
	{

	}
}

void XE::GraphicsService::DeviceDestroy( XE::GraphicsDeviceHandle device )
{
	if ( auto & dev = _p->_Devices[device] )
	{
		if ( dev.Dec() == 0 )
		{
			// TODO: 

			_p->_Devices.Free( dev );
		}
	}
}

void XE::GraphicsService::AdapterDestroy( XE::GraphicsAdapterHandle adapter )
{
	if ( auto & ada = _p->_Adapters[adapter] )
	{
		if ( ada.Dec() == 0 )
		{
			// TODO: 

			_p->_Adapters.Free( ada );
		}
	}
}

void XE::GraphicsService::BindGroupDestroy( XE::GraphicsBindGroupHandle bind_group )
{
	if ( auto & group = _p->_BindGroups[bind_group] )
	{
		if ( group.Dec() == 0 )
		{
			// TODO: 

			_p->_BindGroups.Free( group );
		}
	}
}

void XE::GraphicsService::BindGroupLayoutDestroy( XE::GraphicsBindGroupLayoutHandle bind_group_layout )
{
	if ( auto & layout = _p->_BindGroupLayouts[bind_group_layout] )
	{
		if ( layout.Dec() == 0 )
		{
			// TODO: 

			_p->_BindGroupLayouts.Free( layout );
		}
	}
}

void XE::GraphicsService::BufferDestroy( XE::GraphicsBufferHandle buffer )
{
	if ( auto & buf = _p->_Buffers[buffer] )
	{
		if ( buf.Dec() == 0 )
		{
			// TODO: 

			_p->_Buffers.Free( buf );
		}
	}
}

void XE::GraphicsService::CommandBufferDestroy( XE::GraphicsCommandBufferHandle cmd_buf )
{
	if ( auto & cmd = _p->_CommandBuffers[cmd_buf] )
	{
		if ( cmd.Dec() == 0 )
		{
			// TODO: 

			_p->_CommandBuffers.Free( cmd );
		}
	}
}

void XE::GraphicsService::CommandEncoderDestroy( XE::GraphicsCommandEncoderHandle cmd_encoder )
{
	if ( auto & encoder = _p->_CommandEncoders[cmd_encoder] )
	{
		if ( encoder.Dec() == 0 )
		{
			// TODO: 

			_p->_CommandEncoders.Free( encoder );
		}
	}
}

void XE::GraphicsService::ComputePassEncoderDestroy( XE::GraphicsComputePassEncoderHandle compute_pass_encoder )
{
	if ( auto & pass = _p->_ComputePassEncoders[compute_pass_encoder] )
	{
		if ( pass.Dec() == 0 )
		{
			// TODO: 

			_p->_ComputePassEncoders.Free( pass );
		}
	}
}

void XE::GraphicsService::ComputePipelineDestroy( XE::GraphicsComputePipelineHandle compute_pipeline )
{
	if ( auto & pipe = _p->_ComputePipelines[compute_pipeline] )
	{
		if ( pipe.Dec() == 0 )
		{
			// TODO: 

			_p->_ComputePipelines.Free( pipe );
		}
	}
}

void XE::GraphicsService::PipelineLayoutDestroy( XE::GraphicsPipelineLayoutHandle pipeline_layout )
{
	if ( auto & layout = _p->_PipelineLayouts[pipeline_layout] )
	{
		if ( layout.Dec() == 0 )
		{
			// TODO: 

			_p->_PipelineLayouts.Free( layout );
		}
	}
}

void XE::GraphicsService::QuerySetDestroy( XE::GraphicsQuerySetHandle query_set )
{
	if ( auto & set = _p->_QuerySets[query_set] )
	{
		if ( set.Dec() == 0 )
		{
			// TODO: 

			_p->_QuerySets.Free( set );
		}
	}
}

void XE::GraphicsService::QueueDestroy( XE::GraphicsQueueHandle queue )
{
	if ( auto & q = _p->_Queues[queue] )
	{
		if ( q.Dec() == 0 )
		{
			// TODO: 

			_p->_Queues.Free( q );
		}
	}
}

void XE::GraphicsService::RenderBundleDestroy( XE::GraphicsRenderBundleHandle render_bundle )
{
	if ( auto & bundle = _p->_RenderBundles[render_bundle] )
	{
		if ( bundle.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderBundles.Free( bundle );
		}
	}
}

void XE::GraphicsService::RenderBundleEncoderDestroy( XE::GraphicsRenderBundleEncoderHandle render_bundle_encoder )
{
	if ( auto & encoder = _p->_RenderBundleEncoders[render_bundle_encoder] )
	{
		if ( encoder.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderBundleEncoders.Free( encoder );
		}
	}
}

void XE::GraphicsService::RenderPassEncoderDestroy( XE::GraphicsRenderPassEncoderHandle render_pass_encoder )
{
	if ( auto & pass = _p->_RenderPassEncoders[render_pass_encoder] )
	{
		if ( pass.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderPassEncoders.Free( pass );
		}
	}
}

void XE::GraphicsService::RenderPipelineDestroy( XE::GraphicsRenderPipelineHandle render_pipeline )
{
	if ( auto & pipe = _p->_RenderPipelines[render_pipeline] )
	{
		if ( pipe.Dec() == 0 )
		{
			// TODO: 

			_p->_RenderPipelines.Free( pipe );
		}
	}
}

void XE::GraphicsService::SamplerDestroy( XE::GraphicsSamplerHandle sampler )
{
	if ( auto & sam = _p->_Samplers[sampler] )
	{
		if ( sam.Dec() == 0 )
		{
			// TODO: 

			_p->_Samplers.Free( sam );
		}
	}
}

void XE::GraphicsService::ShaderModuleDestroy( XE::GraphicsShaderModuleHandle shader_module )
{
	if ( auto & shader = _p->_ShaderModules[shader_module] )
	{
		if ( shader.Dec() == 0 )
		{
			// TODO: 

			_p->_ShaderModules.Free( shader );
		}
	}
}

void XE::GraphicsService::SurfaceDestroy( XE::GraphicsSurfaceHandle surface )
{
	if ( auto & sur = _p->_Surfaces[surface] )
	{
		if ( sur.Dec() == 0 )
		{
			// TODO: 

			_p->_Surfaces.Free( sur );
		}
	}
}

void XE::GraphicsService::SwapChainDestroy( XE::GraphicsSwapChainHandle swap_chain )
{
	if ( auto & swap = _p->_SwapChains[swap_chain] )
	{
		if ( swap.Dec() == 0 )
		{
			// TODO: 

			_p->_SwapChains.Free( swap );
		}
	}
}

void XE::GraphicsService::TextureDestroy( XE::GraphicsTextureHandle texture )
{
	if ( auto & tex = _p->_Textures[texture] )
	{
		if ( tex.Dec() == 0 )
		{
			// TODO: 

			_p->_Textures.Free( tex );
		}
	}
}

void XE::GraphicsService::TextureViewDestroy( XE::GraphicsTextureViewHandle texture_view )
{
	if ( auto & view = _p->_TextureViews[texture_view] )
	{
		if ( view.Dec() == 0 )
		{
			// TODO: 

			_p->_TextureViews.Free( view );
		}
	}
}

#endif // GRAPHICS_API == GRAPHICS_D3D12