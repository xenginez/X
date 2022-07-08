/*!
 * \file   Type.h
 *
 * \author ZhengYuanQing
 * \date   2019/02/26
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __TYPE_H__14214677_E236_47F9_92B5_48295B609787
#define __TYPE_H__14214677_E236_47F9_92B5_48295B609787

#include "Math/Math.h"

#include "Utils/Layer.h"
#include "Utils/Window.h"
#include "Utils/Buffer.h"
#include "Utils/Flags.hpp"
#include "Utils/Handle.hpp"
#include "Utils/RefCount.h"
#include "Utils/Concurrent.hpp"

#include "Declare.h"

BEG_XE_NAMESPACE

DECL_PTR( Service );
DECL_PTR( EventService );
DECL_PTR( TimerService );
DECL_PTR( AudioService );
DECL_PTR( CacheService );
DECL_PTR( AssetsService );
DECL_PTR( ThreadService );
DECL_PTR( PhysicsService );
DECL_PTR( GraphicsService );
DECL_PTR( LocalizationService );
class CoreFramework;  using CoreFrameworkPtr = CoreFramework * ;

static constexpr char CONFIG_FILE_NAME[] = "Config.json";
static constexpr char LANGUAGE_FILE_NAME[] = "Language.csv";
static constexpr char CACHEDATA_FILE_NAME[] = "Cache.cache";

static constexpr char ASSETS_DIRECTORY[] = "Assets";
static constexpr char CACHES_DIRECTORY[] = "Caches";
static constexpr char MODULES_DIRECTORY[] = "Modules";
static constexpr char USERDATAS_DIRECTORY[] = "UserDatas";

static constexpr char CONFIG_GRAPHICS_VSYNC[] = "Graphics/VSync";
static constexpr char CONFIG_GRAPHICS_DEBUG[] = "Graphics/Debug";

enum class ThreadType
{
	IO,
	GAME,
	AUDIO,
	RENDER,
	PHYSICS,
	WORKS,
	UNKNOWN,
};
DECL_XE_ENUM( ThreadType );

//////////////////////////////////////////////////////////////////////////
// Render

static constexpr XE::uint64 GRAPHICS_MAX_SURFACE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_SWAP_CHAIN = 0;
static constexpr XE::uint64 GRAPHICS_MAX_ADAPTER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_DEVICE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_QUEUE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_BIND_GROUP = 0;
static constexpr XE::uint64 GRAPHICS_MAX_BIND_GROUP_LAYOUT = 0;
static constexpr XE::uint64 GRAPHICS_MAX_BUFFER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_COMMAND_BUFFER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_COMMAND_ENCODER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_COMPUTE_PASS_ENCODER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_COMPUTE_PIPELINE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_PIPELINE_LAYOUT = 0;
static constexpr XE::uint64 GRAPHICS_MAX_QUERY_SET = 0;
static constexpr XE::uint64 GRAPHICS_MAX_RENDER_BUNDLE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_RENDER_BUNDLE_ENCODER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_RENDER_PASS_ENCODER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_RENDER_PIPELINE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_SAMPLER = 0;
static constexpr XE::uint64 GRAPHICS_MAX_SHADER_MODULE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_TEXTURE = 0;
static constexpr XE::uint64 GRAPHICS_MAX_TEXTURE_VIEW = 0;

DECL_PTR( GraphicsAdapter );
DECL_PTR( GraphicsDevice );
DECL_PTR( GraphicsBindGroup );
DECL_PTR( GraphicsBindGroupLayout );
DECL_PTR( GraphicsBuffer );
DECL_PTR( GraphicsCommandBuffer );
DECL_PTR( GraphicsCommandEncoder );
DECL_PTR( GraphicsComputePassEncoder );
DECL_PTR( GraphicsComputePipeline );
DECL_PTR( GraphicsPipelineLayout );
DECL_PTR( GraphicsQuerySet );
DECL_PTR( GraphicsQueue );
DECL_PTR( GraphicsRenderBundle );
DECL_PTR( GraphicsRenderBundleEncoder );
DECL_PTR( GraphicsRenderPassEncoder );
DECL_PTR( GraphicsRenderPipeline );
DECL_PTR( GraphicsSampler );
DECL_PTR( GraphicsShaderModule );
DECL_PTR( GraphicsSurface );
DECL_PTR( GraphicsSwapChain );
DECL_PTR( GraphicsTexture );
DECL_PTR( GraphicsTextureView );

enum class GraphicsAdapterType
{
    DISCRETE_GPU = 0x00000000,
    INTEGRATED_GPU = 0x00000001,
    SOFTWARE_CPU = 0x00000002,
    UNKNOWN = 0x00000003,
};
DECL_XE_ENUM( GraphicsAdapterType );

enum class GraphicsAddressMode
{
    REPEAT = 0x00000000,
    MIRROR_REPEAT = 0x00000001,
    CLAMP_TO_EDGE = 0x00000002,
};
DECL_XE_ENUM( GraphicsAddressMode );

enum class GraphicsBackendType
{
    NIL = 0x00000000,
    WEBGPU = 0x00000001,
    D3D11 = 0x00000002,
    D3D12 = 0x00000003,
    METAL = 0x00000004,
    VULKAN = 0x00000005,
    OPENGL = 0x00000006,
    OPENGLES = 0x00000007,
};
DECL_XE_ENUM( GraphicsBackendType );

enum class GraphicsBlendFactor
{
    ZERO = 0x00000000,
    ONE = 0x00000001,
    SRC = 0x00000002,
    ONE_MINUS_SRC = 0x00000003,
    SRC_ALPHA = 0x00000004,
    ONE_MINUS_SRC_ALPHA = 0x00000005,
    DST = 0x00000006,
    ONE_MINUS_DST = 0x00000007,
    DST_ALPHA = 0x00000008,
    ONE_MINUS_DST_ALPHA = 0x00000009,
    SRC_ALPHA_SATURATED = 0x0000000A,
    CONSTANT = 0x0000000B,
    ONE_MINUS_CONSTANT = 0x0000000C,
};
DECL_XE_ENUM( GraphicsBlendFactor );

enum class GraphicsBlendOperation
{
    ADD = 0x00000000,
    SUBTRACT = 0x00000001,
    REVERSE_SUBTRACT = 0x00000002,
    MIN = 0x00000003,
    MAX = 0x00000004,
};
DECL_XE_ENUM( GraphicsBlendOperation );

enum class GraphicsBufferBindingType
{
    UNDEFINED = 0x00000000,
    UNIFORM = 0x00000001,
    STORAGE = 0x00000002,
    READ_ONLY_STORAGE = 0x00000003,
};
DECL_XE_ENUM( GraphicsBufferBindingType );

enum class GraphicsBufferMapAsyncStatus
{
    SUCCESS = 0x00000000,
    ERROR = 0x00000001,
    UNKNOWN = 0x00000002,
    DEVICE_LOST = 0x00000003,
    DESTROYED_BEFORE_CALLBACK = 0x00000004,
    UNMAPPED_BEFORE_CALLBACK = 0x00000005,
};
DECL_XE_ENUM( GraphicsBufferMapAsyncStatus );

enum class GraphicsCompareFunction
{
    UNDEFINED = 0x00000000,
    NEVER = 0x00000001,
    LESS = 0x00000002,
    LESS_EQUAL = 0x00000003,
    GREATER = 0x00000004,
    GREATER_EQUAL = 0x00000005,
    EQUAL = 0x00000006,
    NOT_EQUAL = 0x00000007,
    ALWAYS = 0x00000008,
};
DECL_XE_ENUM( GraphicsCompareFunction );

enum class GraphicsCompilationInfoRequestStatus
{
	SUCCESS = 0x00000000,
	ERROR = 0x00000001,
	DEVICE_LOST = 0x00000002,
	UNKNOWN = 0x00000003,
};
DECL_XE_ENUM( GraphicsCompilationInfoRequestStatus );

enum class GraphicsCompilationMessageType
{
    ERROR = 0x00000000,
    WARNING = 0x00000001,
    INFO = 0x00000002,
};
DECL_XE_ENUM( GraphicsCompilationMessageType );

enum class GraphicsComputePassTimestampLocation
{
	BEGINNING = 0x00000000,
	END = 0x00000001,
};
DECL_XE_ENUM( GraphicsComputePassTimestampLocation );

enum class GraphicsCreatePipelineAsyncStatus
{
    SUCCESS = 0x00000000,
    ERROR = 0x00000001,
    DEVICE_LOST = 0x00000002,
    DEVICE_DESTROYED = 0x00000003,
    UNKNOWN = 0x00000004,
};
DECL_XE_ENUM( GraphicsCreatePipelineAsyncStatus );

enum class GraphicsCullMode
{
    NONE = 0x00000000,
    FRONT = 0x00000001,
    BACK = 0x00000002,
};
DECL_XE_ENUM( GraphicsCullMode );

enum class GraphicsDeviceLostReason
{
    UNDEFINED = 0x00000000,
    DESTROYED = 0x00000001,
};
DECL_XE_ENUM( GraphicsDeviceLostReason );

enum class GraphicsErrorFilter
{
	VALIDATION = 0x00000000,
	OUT_OF_MEMORY = 0x00000001,
};
DECL_XE_ENUM( GraphicsErrorFilter );

enum class GraphicsErrorType
{
    NO_ERROR = 0x00000000,
    VALIDATION = 0x00000001,
    OUT_OF_MEMORY = 0x00000002,
    UNKNOWN = 0x00000003,
    DEVICE_LOST = 0x00000004,
};
DECL_XE_ENUM( GraphicsErrorType );

enum class GraphicsFeatureName
{
    UNDEFINED = 0x00000000,
    DEPTH_CLIP_CONTROL = 0x00000001,
    DEPTH24UNORM_STENCIL8 = 0x00000002,
    DEPTH32FLOAT_STENCIL8 = 0x00000003,
    TIMESTAMP_QUERY = 0x00000004,
    PIPELINE_STATISTICS_QUERY = 0x00000005,
    TEXTURE_COMPRESSION_BC = 0x00000006,
    TEXTURE_COMPRESSION_ETC2 = 0x00000007,
    TEXTURE_COMPRESSION_ASTC = 0x00000008,
    INDIRECT_FIRST_INSTANCE = 0x00000009,
};
DECL_XE_ENUM( GraphicsFeatureName );

enum class GraphicsFilterMode
{
    NEAREST = 0x00000000,
    LINEAR = 0x00000001,
};
DECL_XE_ENUM( GraphicsFilterMode );

enum class GraphicsFrontFace
{
    CCW = 0x00000000,
    CW = 0x00000001,
};
DECL_XE_ENUM( GraphicsFrontFace );

enum class GraphicsIndexFormat
{
    UNDEFINED = 0x00000000,
    UINT16 = 0x00000001,
	UINT32 = 0x00000002,
};
DECL_XE_ENUM( GraphicsIndexFormat );

enum class GraphicsLoadOp
{
    UNDEFINED = 0x00000000,
    CLEAR = 0x00000001,
    LOAD = 0x00000002,
};
DECL_XE_ENUM( GraphicsLoadOp );

enum class GraphicsMipmapFilterMode
{
    NEAREST = 0x00000000,
    LINEAR = 0x00000001,
};
DECL_XE_ENUM( GraphicsMipmapFilterMode );

enum class GraphicsPipelineStatisticName
{
    VERTEX_SHADER_INVOCATIONS = 0x00000000,
    CLIPPER_INVOCATIONS = 0x00000001,
    CLIPPER_PRIMITIVES_OUT = 0x00000002,
    FRAGMENT_SHADER_INVOCATIONS = 0x00000003,
    COMPUTE_SHADER_INVOCATIONS = 0x00000004,
};
DECL_XE_ENUM( GraphicsPipelineStatisticName );

enum class GraphicsPowerPreference
{
    UNDEFINED = 0x00000000,
    LOW_POWER = 0x00000001,
    HIGH_PERFORMANCE = 0x00000002,
};
DECL_XE_ENUM( GraphicsPowerPreference );

enum class GraphicsPredefinedColorSpace
{
	UNDEFINED = 0x00000000,
	SRGB = 0x00000001,
};
DECL_XE_ENUM( GraphicsPredefinedColorSpace );

enum class GraphicsPresentMode
{
    IMMEDIATE = 0x00000000,
    MAILBOX = 0x00000001,
    FIFO = 0x00000002,
};
DECL_XE_ENUM( GraphicsPresentMode );

enum class GraphicsPrimitiveTopology
{
    POINT_LIST = 0x00000000,
    LINE_LIST = 0x00000001,
    LINE_STRIP = 0x00000002,
    TRIANGLE_LIST = 0x00000003,
    TRIANGLE_STRIP = 0x00000004,
};
DECL_XE_ENUM( GraphicsPrimitiveTopology );

enum class GraphicsQueryType
{
    OCCLUSION = 0x00000000,
    PIPELINE_STATISTICS = 0x00000001,
    TIMESTAMP = 0x00000002,
};
DECL_XE_ENUM( GraphicsQueryType );

enum class GraphicsQueueWorkDoneStatus
{
    SUCCESS = 0x00000000,
    ERROR = 0x00000001,
    UNKNOWN = 0x00000002,
    DEVICE_LOST = 0x00000003,
};
DECL_XE_ENUM( GraphicsQueueWorkDoneStatus );

enum class GraphicsRenderPassTimestampLocation
{
	BEGINNING = 0x00000000,
	END = 0x00000001,
};
DECL_XE_ENUM( GraphicsRenderPassTimestampLocation );

enum class GraphicsRequestAdapterStatus
{
    SUCCESS = 0x00000000,
    UNAVAILABLE = 0x00000001,
    ERROR = 0x00000002,
    UNKNOWN = 0x00000003,
};
DECL_XE_ENUM( GraphicsRequestAdapterStatus );

enum class GraphicsRequestDeviceStatus
{
	SUCCESS = 0x00000000,
	ERROR = 0x00000001,
	UNKNOWN = 0x00000002,
};
DECL_XE_ENUM( GraphicsRequestDeviceStatus );

enum class GraphicsSamplerBindingType
{
    UNDEFINED = 0x00000000,
    FILTERING = 0x00000001,
    NON_FILTERING = 0x00000002,
    COMPARISON = 0x00000003,
};
DECL_XE_ENUM( GraphicsSamplerBindingType );

enum class GraphicsStencilOperation
{
    KEEP = 0x00000000,
    ZERO = 0x00000001,
    REPLACE = 0x00000002,
    INVERT = 0x00000003,
    INCREMENT_CLAMP = 0x00000004,
    DECREMENT_CLAMP = 0x00000005,
    INCREMENT_WRAP = 0x00000006,
    DECREMENT_WRAP = 0x00000007,
};
DECL_XE_ENUM( GraphicsStencilOperation );

enum class GraphicsStorageTextureAccess
{
    UNDEFINED = 0x00000000,
    WRITE_ONLY = 0x00000001,
};
DECL_XE_ENUM( GraphicsStorageTextureAccess );

enum class GraphicsStoreOp
{
	UNDEFINED = 0x00000000,
	STORE = 0x00000001,
	DISCARD = 0x00000002,
};
DECL_XE_ENUM( GraphicsStoreOp );

enum class GraphicsTextureAspect
{
	ALL = 0x00000000,
	STENCIL_ONLY = 0x00000001,
	DEPTH_ONLY = 0x00000002,
};
DECL_XE_ENUM( GraphicsTextureAspect );

enum class GraphicsTextureComponentType
{
    FLOAT = 0x00000000,
    SINT = 0x00000001,
    UINT = 0x00000002,
    DEPTH_COMPARISON = 0x00000003,
};
DECL_XE_ENUM( GraphicsTextureComponentType );

enum class GraphicsTextureDimension
{
	D1 = 0x00000000,
	D2 = 0x00000001,
	D3 = 0x00000002,
};
DECL_XE_ENUM( GraphicsTextureDimension );

enum class GraphicsTextureFormat
{
    UNDEFINED = 0x00000000,
    R8UNORM = 0x00000001,
    R8SNORM = 0x00000002,
    R8UINT = 0x00000003,
    R8SINT = 0x00000004,
    R16UINT = 0x00000005,
    R16SINT = 0x00000006,
    R16FLOAT = 0x00000007,
    RG8UNORM = 0x00000008,
    RG8SNORM = 0x00000009,
    RG8UINT = 0x0000000A,
    RG8SINT = 0x0000000B,
    R32FLOAT = 0x0000000C,
    R32UINT = 0x0000000D,
    R32SINT = 0x0000000E,
    RG16UINT = 0x0000000F,
    RG16SINT = 0x00000010,
    RG16FLOAT = 0x00000011,
    RGBA8UNORM = 0x00000012,
    RGBA8UNORMSRGB = 0x00000013,
    RGBA8SNORM = 0x00000014,
    RGBA8UINT = 0x00000015,
    RGBA8SINT = 0x00000016,
    BGRA8UNORM = 0x00000017,
    BGRA8UNORMSRGB = 0x00000018,
    RGB10A2UNORM = 0x00000019,
    RG11B10UFLOAT = 0x0000001A,
    RGB9E5UFLOAT = 0x0000001B,
    RG32FLOAT = 0x0000001C,
    RG32UINT = 0x0000001D,
    RG32SINT = 0x0000001E,
    RGBA16UINT = 0x0000001F,
    RGBA16SINT = 0x00000020,
    RGBA16FLOAT = 0x00000021,
    RGBA32FLOAT = 0x00000022,
    RGBA32UINT = 0x00000023,
    RGBA32SINT = 0x00000024,
    STENCIL8 = 0x00000025,
    DEPTH16UNORM = 0x00000026,
    DEPTH24PLUS = 0x00000027,
    DEPTH24PLUSSTENCIL8 = 0x00000028,
    DEPTH24UNORMSTENCIL8 = 0x00000029,
    DEPTH32FLOAT = 0x0000002A,
    DEPTH32FLOATSTENCIL8 = 0x0000002B,
    BC1RGBAUNORM = 0x0000002C,
    BC1RGBAUNORMSRGB = 0x0000002D,
    BC2RGBAUNORM = 0x0000002E,
    BC2RGBAUNORMSRGB = 0x0000002F,
    BC3RGBAUNORM = 0x00000030,
    BC3RGBAUNORMSRGB = 0x00000031,
    BC4RUNORM = 0x00000032,
    BC4RSNORM = 0x00000033,
    BC5RGUNORM = 0x00000034,
    BC5RGSNORM = 0x00000035,
    BC6HRGBUFLOAT = 0x00000036,
    BC6HRGBFLOAT = 0x00000037,
    BC7RGBAUNORM = 0x00000038,
    BC7RGBAUNORMSRGB = 0x00000039,
    ETC2RGB8UNORM = 0x0000003A,
    ETC2RGB8UNORMSRGB = 0x0000003B,
    ETC2RGB8A1UNORM = 0x0000003C,
    ETC2RGB8A1UNORMSRGB = 0x0000003D,
    ETC2RGBA8UNORM = 0x0000003E,
    ETC2RGBA8UNORMSRGB = 0x0000003F,
    EACR11UNORM = 0x00000040,
    EACR11SNORM = 0x00000041,
    EACRG11UNORM = 0x00000042,
    EACRG11SNORM = 0x00000043,
    ASTC4X4UNORM = 0x00000044,
    ASTC4X4UNORMSRGB = 0x00000045,
    ASTC5X4UNORM = 0x00000046,
    ASTC5X4UNORMSRGB = 0x00000047,
    ASTC5X5UNORM = 0x00000048,
    ASTC5X5UNORMSRGB = 0x00000049,
    ASTC6X5UNORM = 0x0000004A,
    ASTC6X5UNORMSRGB = 0x0000004B,
    ASTC6X6UNORM = 0x0000004C,
    ASTC6X6UNORMSRGB = 0x0000004D,
    ASTC8X5UNORM = 0x0000004E,
    ASTC8X5UNORMSRGB = 0x0000004F,
    ASTC8X6UNORM = 0x00000050,
    ASTC8X6UNORMSRGB = 0x00000051,
    ASTC8X8UNORM = 0x00000052,
    ASTC8X8UNORMSRGB = 0x00000053,
    ASTC10x5UNORM = 0x00000054,
    ASTC10x5UNORMSRGB = 0x00000055,
    ASTC10x6UNORM = 0x00000056,
    ASTC10x6UNORMSRGB = 0x00000057,
    ASTC10x8UNORM = 0x00000058,
    ASTC10x8UNORMSRGB = 0x00000059,
    ASTC10x10UNORM = 0x0000005A,
    ASTC10x10UNORMSRGB = 0x0000005B,
    ASTC12X10UNORM = 0x0000005C,
    ASTC12X10UNORMSRGB = 0x0000005D,
    ASTC12X12UNORM = 0x0000005E,
    ASTC12X12UNORMSRGB = 0x0000005F,
};
DECL_XE_ENUM( GraphicsTextureFormat );

enum class GraphicsTextureSampleType
{
    UNDEFINED = 0x00000000,
    FLOAT = 0x00000001,
    UNFILTERABLE_FLOAT = 0x00000002,
    DEPTH = 0x00000003,
    SINT = 0x00000004,
    UINT = 0x00000005,
};
DECL_XE_ENUM( GraphicsTextureSampleType );

enum class GraphicsTextureViewDimension
{
	UNDEFINED = 0x00000000,
	D1 = 0x00000001,
	D2 = 0x00000002,
	D2ARRAY = 0x00000003,
	CUBE = 0x00000004,
	CUBEARRAY = 0x00000005,
	D3 = 0x00000006,
};
DECL_XE_ENUM( GraphicsTextureViewDimension );

enum class GraphicsVertexFormat
{
    UNDEFINED = 0x00000000,
    UINT8X2 = 0x00000001,
    UINT8X4 = 0x00000002,
    SINT8X2 = 0x00000003,
    SINT8X4 = 0x00000004,
    UNORM8X2 = 0x00000005,
    UNORM8X4 = 0x00000006,
    SNORM8X2 = 0x00000007,
    SNORM8X4 = 0x00000008,
    UINT16X2 = 0x00000009,
    UINT16X4 = 0x0000000A,
    SINT16X2 = 0x0000000B,
    SINT16X4 = 0x0000000C,
    UNORM16X2 = 0x0000000D,
    UNORM16X4 = 0x0000000E,
    SNORM16X2 = 0x0000000F,
    SNORM16X4 = 0x00000010,
    FLOAT16X2 = 0x00000011,
    FLOAT16X4 = 0x00000012,
    FLOAT32 = 0x00000013,
    FLOAT32X2 = 0x00000014,
    FLOAT32X3 = 0x00000015,
    FLOAT32X4 = 0x00000016,
    UINT32 = 0x00000017,
    UINT32X2 = 0x00000018,
    UINT32X3 = 0x00000019,
    UINT32X4 = 0x0000001A,
    SINT32 = 0x0000001B,
    SINT32X2 = 0x0000001C,
    SINT32X3 = 0x0000001D,
    SINT32X4 = 0x0000001E,
};
DECL_XE_ENUM( GraphicsVertexFormat );

enum class GraphicsVertexStepMode
{
    VERTEX = 0x00000000,
    INSTANCE = 0x00000001,
};
DECL_XE_ENUM( GraphicsVertexStepMode );

enum class GraphicsBufferUsage
{
    NONE = 0x00000000,
    MAP_READ = 0x00000001,
    MAP_WRITE = 0x00000002,
    COPY_SRC = 0x00000004,
    COPY_DST = 0x00000008,
    INDEX = 0x00000010,
    VERTEX = 0x00000020,
    UNIFORM = 0x00000040,
    STORAGE = 0x00000080,
    INDIRECT = 0x00000100,
    QUERY_RESOLVE = 0x00000200,
};
DECL_FLAGS( GraphicsBufferUsage ); DECL_XE_ENUM( GraphicsBufferUsage );

enum class GraphicsColorWriteMask
{
    NONE = 0x00000000,
    RED = 0x00000001,
    GREEN = 0x00000002,
    BLUE = 0x00000004,
    ALPHA = 0x00000008,
    ALL = 0x0000000F,
};
DECL_FLAGS( GraphicsColorWriteMask ); DECL_XE_ENUM( GraphicsColorWriteMask );

enum class GraphicsMapMode
{
	NONE = 0x00000000,
	READ = 0x00000001,
	WRITE = 0x00000002,
};
DECL_FLAGS( GraphicsMapMode ); DECL_XE_ENUM( GraphicsMapMode );

enum class GraphicsShaderStage
{
    NONE = 0x00000000,
    VERTEX = 0x00000001,
    FRAGMENT = 0x00000002,
    COMPUTE = 0x00000004,
};
DECL_FLAGS( GraphicsShaderStage ); DECL_XE_ENUM( GraphicsShaderStage );

enum class GraphicsTextureUsage
{
    NONE = 0x00000000,
    COPY_SRC = 0x00000001,
    COPY_DST = 0x00000002,
    TEXTURE_BINDING = 0x00000004,
    STORAGE_BINDING = 0x00000008,
    RENDER_ATTACHMENT = 0x00000010,
};
DECL_FLAGS( GraphicsTextureUsage ); DECL_XE_ENUM( GraphicsTextureUsage );


struct XE_API GraphicsAdapterProperties
{
    XE::uint32 VendorID;
    XE::uint32 DeviceID;
    XE::String Name;
    XE::String DriverDescription;
    XE::GraphicsAdapterType AdapterType;
    XE::GraphicsBackendType BackendType;
};

struct XE_API GraphicsBindGroupEntry
{
    XE::uint32 Binding;
    XE::GraphicsBufferPtr Buffer;
    XE::uint64 Offset;
    XE::uint64 Size;
    XE::GraphicsSamplerPtr Sampler;
    XE::GraphicsTextureViewPtr TextureView;
};

struct XE_API GraphicsBlendComponent
{
    XE::GraphicsBlendOperation Operation;
    XE::GraphicsBlendFactor SrcFactor;
    XE::GraphicsBlendFactor DstFactor;
};

struct XE_API GraphicsBufferBindingLayout
{
    XE::GraphicsBufferBindingType Type;
    bool HasDynamicOffset;
    XE::uint64 MinBindingSize;
};

struct XE_API GraphicsBufferDescriptor
{
    XE::String Label;
    XE::GraphicsBufferUsageFlags Usage;
    XE::uint64 Size;
    bool MappedAtCreation;
};

struct XE_API GraphicsCommandBufferDescriptor
{
    XE::String Label;
};

struct XE_API GraphicsCommandEncoderDescriptor
{
    XE::String Label;
};

struct XE_API GraphicsCompilationMessage
{
    XE::String Message;
    XE::uint64 LineNum;
    XE::uint64 LinePos;
    XE::uint64 Offset;
	XE::uint64 Length;
	XE::GraphicsCompilationMessageType Type;
};

struct XE_API GraphicsComputePassTimestampWrite
{
    XE::GraphicsQuerySetPtr QuerySet;
    XE::uint32 QueryIndex;
    XE::GraphicsComputePassTimestampLocation Location;
};

struct XE_API GraphicsConstantEntry
{
    XE::String Key;
    double Value;
};

struct XE_API GraphicsLimits
{
    XE::uint32 MaxTextureDimension1D = 8192;
    XE::uint32 MaxTextureDimension2D = 8192;
    XE::uint32 MaxTextureDimension3D = 2048;
    XE::uint32 MaxTextureArrayLayers = 256;
    XE::uint32 MaxBindGroups = 4;
    XE::uint32 MaxDynamicUniformBuffersPerPipelineLayout = 8;
    XE::uint32 MaxDynamicStorageBuffersPerPipelineLayout = 4;
    XE::uint32 MaxSampledTexturesPerShaderStage = 16;
    XE::uint32 MaxSamplersPerShaderStage = 16;
    XE::uint32 MaxStorageBuffersPerShaderStage = 8;
    XE::uint32 MaxStorageTexturesPerShaderStage = 8;
    XE::uint32 MaxUniformBuffersPerShaderStage = 12;
    XE::uint64 MaxUniformBufferBindingSize = 64 << 10;
    XE::uint64 MaxStorageBufferBindingSize = 128 << 20;
    XE::uint32 MinUniformBufferOffsetAlignment = 256;
	XE::uint32 MinStorageBufferOffsetAlignment = 256;
    XE::uint32 MaxVertexBuffers = 8;
    XE::uint32 MaxVertexAttributes = 16;
    XE::uint32 MaxVertexBufferArrayStride = 2048;
    XE::uint32 MaxInterStageShaderComponents = 60;
    XE::uint32 MaxComputeWorkgroupStorageSize = 16352;
    XE::uint32 MaxComputeInvocationsPerWorkgroup = 256;
    XE::uint32 MaxComputeWorkgroupSizeX = 256;
    XE::uint32 MaxComputeWorkgroupSizeY = 256;
    XE::uint32 MaxComputeWorkgroupSizeZ = 64;
    XE::uint32 MaxComputeWorkgroupsPerDimension = 65535;
};

struct XE_API GraphicsMultisampleState
{
    XE::uint32 Count;
    XE::uint32 Mask;
    bool AlphaToCoverageEnabled;
};

struct XE_API GraphicsPipelineLayoutDescriptor
{
    XE::String Label;
    XE::Array<XE::GraphicsBindGroupLayoutPtr> BindGroupLayouts;
};

struct XE_API GraphicsPrimitiveState
{
    XE::GraphicsPrimitiveTopology Topology;
    XE::GraphicsIndexFormat StripIndexFormat;
    XE::GraphicsFrontFace FrontFace;
	XE::GraphicsCullMode CullMode;
	bool UnclippedDepth;
};

struct XE_API GraphicsQuerySetDescriptor
{
    XE::String Label;
    XE::GraphicsQueryType Type;
	XE::uint32 Count;
	XE::Array< XE::GraphicsPipelineStatisticName > PipelineStatistics;
};

struct XE_API GraphicsQueueDescriptor
{
    XE::String Label;
};

struct XE_API GraphicsRenderBundleDescriptor
{
    XE::String Label;
};

struct XE_API GraphicsRenderBundleEncoderDescriptor
{
    XE::String Label;
    XE::Array< XE::GraphicsTextureFormat > ColorFormats;
    XE::GraphicsTextureFormat DepthStencilFormat;
    XE::uint32 SampleCount;
    bool DepthReadOnly;
    bool StencilReadOnly;
};

struct XE_API GraphicsRenderPassDepthStencilAttachment
{
    XE::GraphicsTextureViewPtr View;
    XE::GraphicsLoadOp DepthLoadOp;
    XE::GraphicsStoreOp DepthStoreOp;
    XE::float32 DepthClearValue;
    bool DepthReadOnly;
    XE::GraphicsLoadOp StencilLoadOp;
    XE::GraphicsStoreOp StencilStoreOp;
    XE::uint32 StencilClearValue;
    bool StencilReadOnly;
};

struct XE_API GraphicsRenderPassTimestampWrite
{
    XE::GraphicsQuerySetPtr QuerySet;
    XE::uint32 QueryIndex;
    XE::GraphicsRenderPassTimestampLocation Location;
};

struct XE_API GraphicsRequestAdapterOptions
{
//    XE::GraphicsSurfacePtr CompatibleSurface;
    XE::GraphicsPowerPreference PowerPreference;
    bool ForceFallbackAdapter;
};

struct XE_API GraphicsSamplerBindingLayout
{
    XE::GraphicsSamplerBindingType Type;
};

struct XE_API GraphicsSamplerDescriptor
{
    XE::String Label;
    XE::GraphicsAddressMode AddressModeU;
    XE::GraphicsAddressMode AddressModeV;
    XE::GraphicsAddressMode AddressModeW;
    XE::GraphicsFilterMode MagFilter;
    XE::GraphicsFilterMode MinFilter;
    XE::GraphicsMipmapFilterMode MipmapFilter;
    XE::float32 LodMinClamp;
    XE::float32 LodMaxClamp;
    XE::GraphicsCompareFunction Compare;
    XE::uint16 MaxAnisotropy;
};

struct XE_API GraphicsShaderModuleCompilationHint
{
    XE::String EntryPoint;
    XE::GraphicsPipelineLayoutPtr Layout;
};

struct XE_API GraphicsShaderModuleCodeDescriptor
{
    XE::MemoryView Code;
};

struct XE_API GraphicsStencilFaceState
{
    XE::GraphicsCompareFunction Compare;
    XE::GraphicsStencilOperation FailOp;
    XE::GraphicsStencilOperation DepthFailOp;
    XE::GraphicsStencilOperation PassOp;
};

struct XE_API GraphicsStorageTextureBindingLayout
{
    XE::GraphicsStorageTextureAccess Access;
    XE::GraphicsTextureFormat Format;
    XE::GraphicsTextureViewDimension ViewDimension;
};

struct XE_API GraphicsSurfaceDescriptor
{
    XE::String Label;
    XE::WindowPtr Window;
};

struct XE_API GraphicsSwapChainDescriptor
{
    XE::String Label;
    XE::GraphicsTextureUsageFlags Usage;
    XE::GraphicsTextureFormat Format;
    XE::uint32 Width;
    XE::uint32 Height;
    XE::GraphicsPresentMode PresentMode;
};

struct XE_API GraphicsTextureBindingLayout
{
    XE::GraphicsTextureSampleType SampleType;
    XE::GraphicsTextureViewDimension ViewDimension;
    bool Multisampled;
};

struct XE_API GraphicsTextureDataLayout
{
    XE::uint64 Offset;
    XE::uint32 BytesPerRow;
    XE::uint32 RowsPerImage;
};

struct XE_API GraphicsTextureViewDescriptor
{
    XE::String Label;
    XE::GraphicsTextureFormat Format;
    XE::GraphicsTextureViewDimension Dimension;
    XE::uint32 BaseMipLevel;
    XE::uint32 MipLevelCount;
    XE::uint32 BaseArrayLayer;
    XE::uint32 ArrayLayerCount;
    XE::GraphicsTextureAspect Aspect;
};

struct XE_API GraphicsVertexAttribute
{
    XE::GraphicsVertexFormat Format;
    XE::uint64 Offset;
    XE::uint32 ShaderLocation;
};

struct XE_API GraphicsBindGroupDescriptor
{
    XE::String Label;
    XE::GraphicsBindGroupLayoutPtr Layout;
    XE::Array< XE::GraphicsBindGroupEntry > Entries;
};

struct XE_API GraphicsBindGroupLayoutEntry
{
    XE::uint32 Binding;
    XE::GraphicsShaderStageFlags Visibility;
    XE::GraphicsBufferBindingLayout Buffer;
    XE::GraphicsSamplerBindingLayout Sampler;
    XE::GraphicsTextureBindingLayout Texture;
    XE::GraphicsStorageTextureBindingLayout StorageTexture;
};

struct XE_API GraphicsBlendState
{
    bool Enable;
	XE::GraphicsBlendComponent Color;
	XE::GraphicsBlendComponent Alpha;
};

struct XE_API GraphicsCompilationInfo
{
    XE::Array< XE::GraphicsCompilationMessage > Messages;
};

struct XE_API GraphicsComputePassDescriptor
{
    XE::String Label;
    XE::Array< XE::GraphicsComputePassTimestampWrite > TimestampWrites;
};

struct XE_API GraphicsDepthStencilState
{
    XE::GraphicsTextureFormat Format;
    bool DepthWriteEnabled;
    XE::GraphicsCompareFunction DepthCompare;
    XE::GraphicsStencilFaceState StencilFront;
    XE::GraphicsStencilFaceState StencilBack;
    XE::uint32 StencilReadMask;
    XE::uint32 StencilWriteMask;
    int32_t DepthBias;
    XE::float32 DepthBiasSlopeScale;
    XE::float32 DepthBiasClamp;
};

struct XE_API GraphicsImageCopyBuffer
{
    XE::GraphicsTextureDataLayout Layout;
    XE::GraphicsBufferPtr Buffer;
};

struct XE_API GraphicsImageCopyTexture
{
    XE::GraphicsTexturePtr Texture;
    XE::uint32 MipLevel;
    XE::Vec3i Origin;
    XE::GraphicsTextureAspect Aspect;
};

struct XE_API GraphicsProgrammableStageDescriptor
{
    XE::GraphicsShaderModulePtr Shader;
    XE::String EntryPoint;
    XE::Array< XE::GraphicsConstantEntry > Constants;
};

struct XE_API GraphicsRenderPassColorAttachment
{
    XE::GraphicsTextureViewPtr View;
    XE::GraphicsTextureViewPtr ResolveTarget;
    XE::GraphicsLoadOp LoadOp;
    XE::GraphicsStoreOp StoreOp;
    XE::Color ClearValue;
};

struct XE_API GraphicsRequiredLimits
{
    XE::GraphicsLimits Limits;
};

struct XE_API GraphicsShaderModuleDescriptor
{
	XE::String Label;
    bool Debug;
	XE::MemoryView Code;
    XE::Array< XE::GraphicsShaderModuleCompilationHint > Hints;
};

struct XE_API GraphicsSupportedLimits
{
    XE::GraphicsLimits Limits;
};

struct XE_API GraphicsTextureDescriptor
{
    XE::String Label;
    XE::GraphicsTextureUsageFlags Usage;
    XE::GraphicsTextureDimension Dimension;
    XE::Vec3i Size;
    XE::GraphicsTextureFormat Format;
    XE::uint32 MipLevelCount;
    XE::uint32 SampleCount;
    XE::Array< XE::GraphicsTextureFormat > ViewFormats;
};

struct XE_API GraphicsVertexBufferLayout
{
    XE::uint64 ArrayStride;
    XE::GraphicsVertexStepMode StepMode;
    XE::Array < XE::GraphicsVertexAttribute > Attributes;
};

struct XE_API GraphicsBindGroupLayoutDescriptor
{
    XE::String Label;
    XE::Array < XE::GraphicsBindGroupLayoutEntry > Entries;
};

struct XE_API GraphicsColorTargetState
{
    XE::GraphicsTextureFormat Format;
    XE::GraphicsBlendState Blend;
    XE::GraphicsColorWriteMaskFlags WriteMask;
};

struct XE_API GraphicsComputePipelineDescriptor
{
    XE::String Label;
    XE::GraphicsPipelineLayoutPtr Layout;
    XE::GraphicsProgrammableStageDescriptor Compute;
};

struct XE_API GraphicsDeviceDescriptor
{
    XE::String Label;
    XE::Array < XE::GraphicsFeatureName > RequiredFeatures;
    XE::GraphicsRequiredLimits RequiredLimits;
    XE::GraphicsQueueDescriptor DefaultQueue;
};

struct XE_API GraphicsRenderPassDescriptor
{
    XE::String Label;
    XE::Array < XE::GraphicsRenderPassColorAttachment > ColorAttachments;
    XE::GraphicsRenderPassDepthStencilAttachment DepthStencilAttachment;
    XE::GraphicsQuerySetPtr OcclusionQuerySet;
    XE::Array < XE::GraphicsRenderPassTimestampWrite > TimestampWrites;
};

struct XE_API GraphicsVertexState
{
    XE::GraphicsShaderModulePtr Module;
    XE::String EntryPoint;
    XE::Array < XE::GraphicsConstantEntry > Constants;
    XE::Array < XE::GraphicsVertexBufferLayout > Buffers;
};

struct XE_API GraphicsFragmentState
{
    XE::GraphicsShaderModulePtr Module;
    XE::String EntryPoint;
    XE::Array < XE::GraphicsConstantEntry > Constants;
    XE::Array < XE::GraphicsColorTargetState > Targets;
};

struct XE_API GraphicsRenderPipelineDescriptor
{
    XE::String Label;
    XE::GraphicsPipelineLayoutPtr Layout;
    XE::GraphicsVertexState Vertex;
    XE::GraphicsPrimitiveState Primitive;
    XE::GraphicsDepthStencilState DepthStencil;
    XE::GraphicsMultisampleState Multisample;
    XE::GraphicsFragmentState Fragment;
};


// Render
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// PHI

DECL_HANDLE( PhysicsBody );
DECL_HANDLE( PhysicsWorld );
DECL_HANDLE( PhysicsShape );
DECL_HANDLE( PhysicsMaterial );
DECL_HANDLE( PhysicsConstraint );

static constexpr XE::uint64 PHYSICS_MAX_FRAME = 2;
static constexpr XE::uint64 PHYSICS_MAX_WORLD = 16;
static constexpr XE::uint64 PHYSICS_MAX_BODY = 1024;
static constexpr XE::uint64 PHYSICS_MAX_SHAPE = 2048;
static constexpr XE::uint64 PHYSICS_MAX_COMMAND = 2048;
static constexpr XE::uint64 PHYSICS_MAX_MATERIAL = 2048;
static constexpr XE::uint64 PHYSICS_MAX_CONSTRAINT = 2048;

enum class PhysicsBodyType
{
	SOFT_BODY,
	CLOTH_BODY,
	FLUID_BODY,
	STATIC_RIGID_BODY,
	DYNAMIC_RIGID_BODY,
};

enum class PhysicsShapeType
{
	BOX,
	PLANE,
	SPHERE,
	CAPSULE,
	CONVEXHULL,
	HEIGHTFIELD,
	TRIANGLEMESH,
};

enum class PhysicsLockAxisType
{
	LINEAR_X = 1 << 0,
	LINEAR_Y = 1 << 1,
	LINEAR_Z = 1 << 2,
	ANGULAR_X = 1 << 3,
	ANGULAR_Y = 1 << 4,
	ANGULAR_Z = 1 << 5,
};
DECL_FLAGS( PhysicsLockAxisType );

struct XE_API PhysicsWorldDesc
{
	XE::Vec3f Gravity;
	XE::AABB BoundBox;
	XE::Variant UserData;
	XE::Delegate< void( const XE::EventPtr & ) > EventCallback;
};

struct XE_API PhysicsShapeDesc
{
	XE::Variant UserData;
	PhysicsShapeType Type;
	XE::Mat4x4f LocalTransform;
	XE::PhysicsMaterialHandle Material;
	std::variant< std::monostate, XE::AABB, XE::Plane, XE::Sphere, XE::Capsule, XE::ConvexHull, XE::HeightField, XE::Mesh > Data;
};

struct XE_API PhysicsMaterialDesc
{
	XE::String Name;
	XE::float32 Restitution;
	XE::float32 StaticFriction;
	XE::float32 DynamicFriction;
};

struct XE_API PhysicsSoftBodyDesc
{

	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mesh & ) > SyncMeshCallback;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsClothBodyDesc
{

	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mesh & ) > SyncMeshCallback;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsFluidBodyDesc
{

	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mesh & ) > SyncMeshCallback;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsStaticRigidBodyDesc
{
	XE::Vec3f CMass;
	XE::float32 Mass;
	XE::float32 LinearDamping;
	XE::float32 AngularDamping;
	XE::Vec3f MaxLinearVelocity;
	XE::Vec3f MaxAngularVelocity;
};

struct XE_API PhysicsDynamicRigidBodyDesc : public PhysicsStaticRigidBodyDesc
{
	XE::uint32 IterationCount;
	XE::float32 SleepThreshold;
	XE::float32 StabilizationThreshold;
	XE::float32 ContactReportsThreshold;
	PhysicsLockAxisTypeFlags LockAxisFlags;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsBodyDesc
{
	XE::Layer Mask;
	XE::String Name;
	XE::Variant UserData;
	XE::PhysicsBodyType Type;
	XE::Mat4x4f WorldTransform;
	std::variant< std::monostate, XE::PhysicsSoftBodyDesc, XE::PhysicsClothBodyDesc, XE::PhysicsFluidBodyDesc, XE::PhysicsStaticRigidBodyDesc, XE::PhysicsDynamicRigidBodyDesc > Data;
};

struct XE_API PhysicsConstraintDesc
{
	XE::Vec3f Local0, Local1;
	XE::PhysicsBodyHandle Body0, Body1;

	XE::AxisType DriveLockAxis;
	XE::float32 DriveStiffness;
	XE::float32 DriveDamping;
	XE::float32 DriveMaxForce;
	bool DriveIsAcceleration;

	XE::AxisType LinearLockAxis;
	XE::Pair< XE::float32, XE::float32 > LinearXLimit;
	XE::Pair< XE::float32, XE::float32 > LinearYLimit;
	XE::Pair< XE::float32, XE::float32 > LinearZLimit;

	XE::AxisType AngularLockAxis;
	XE::Pair< XE::float32, XE::float32 > AngularXLimit;
	XE::Pair< XE::float32, XE::float32 > AngularYLimit;
	XE::Pair< XE::float32, XE::float32 > AngularZLimit;
};

struct XE_API PhysicsRaycastHit
{
	XE::Vec3f Normal;
	XE::Vec3f Position;
	XE::float32 Distance;
	XE::PhysicsBodyHandle Body;
	XE::PhysicsShapeHandle Shape;
};


enum class PhysicsCommandType
{
	CREATE_WORLD,
	CREATE_BODY,
	CREATE_SHAPE,
	CREATE_MATERIAL,
	CREATE_CONSTRAINT,

	SET_GRAVITY,
	SET_WORLD_TRANSFORM,
	SET_LOCAL_TRANSFROM,
	SET_LINEAR_DAMPING,
	SET_ANGULAR_DAMPING,
	SET_LINEAR_VELOCITY,
	SET_ANGULAR_VELOCITY,

	ATTACH_SHAPE,
	DETACH_SHAPE,

	ADD_FORCE,
	ADD_TORQUE,
	CLEAR_FORCE,
	CLEAR_TORQUE,
	SLEEP,
	WAKEUP,

	DESTROY_CONSTRAINT,
	DESTROY_MATERIAL,
	DESTROY_SHAPE,
	DESTROY_BODY,
	DESTROY_WORLD,

	END,
};

struct XE_API PhysicsItem
{
	XE::PhysicsCommandType Type;
	XE::uint64 Handle;
	XE::uint64 Code;
	XE::uint64 Size;
};

struct XE_API PhysicsFrame
{
public:
	void Reset()
	{
		CommandSize = 0;
		Buffers.unsafe().Clear();
	}

	std::mutex Mutex;
	std::atomic< XE::uint64 > CommandSize = 0;
	XE::Concurrent< XE::Buffer, std::mutex > Buffers;
	std::array< XE::PhysicsItem, PHYSICS_MAX_COMMAND > Command;
};

// PHI
//////////////////////////////////////////////////////////////////////////

END_XE_NAMESPACE


#endif // __TYPE_H__14214677_E236_47F9_92B5_48295B609787