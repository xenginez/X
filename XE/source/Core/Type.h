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
#include "Utils/Concurrent.hpp"

#include "Declare.h"

BEG_XE_NAMESPACE

DECL_PTR( Service );
DECL_PTR( EventService );
DECL_PTR( TimerService );
DECL_PTR( AudioService );
DECL_PTR( InputService );
DECL_PTR( CacheService );
DECL_PTR( AssetsService );
DECL_PTR( ConfigService );
DECL_PTR( RenderService );
DECL_PTR( ThreadService );
DECL_PTR( PhysicsService );
DECL_PTR( LocalizationService );
class CoreFramework;  using CoreFrameworkPtr = CoreFramework * ;

static constexpr char CONFIG_FILE_NAME[] = "Config.json";
static constexpr char LANGUAGE_FILE_NAME[] = "Language.csv";
static constexpr char CACHEDATA_FILE_NAME[] = "Cache.cache";

static constexpr char ASSETS_DIRECTORY[] = "Assets";
static constexpr char CACHES_DIRECTORY[] = "Caches";
static constexpr char MODULES_DIRECTORY[] = "Modules";
static constexpr char USERDATAS_DIRECTORY[] = "UserDatas";

static constexpr char CONFIG_RENDER_VSYNC[] = "Render/VSync";
static constexpr char CONFIG_RENDER_DEBUG[] = "Render/Debug";

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

using ReadBufferCallbackType = void( * )( void * userdata, XE::MemoryView view );


//////////////////////////////////////////////////////////////////////////
// Audio

static constexpr XE::uint64 AUDIO_MAX_FRAME = 2;
static constexpr XE::uint64 AUDIO_MAX_SOURCE = 512;
static constexpr XE::uint64 AUDIO_MAX_BUFFER = 1024;
static constexpr XE::uint64 AUDIO_MAX_COMMAND = 2048;

DECL_HANDLE( AudioBuffer );
DECL_HANDLE( AudioSource );

struct XE_API AudioBufferDesc
{
	XE::String Name;
	XE::uint32 Bits = 8;		// 8 16 32
	XE::uint32 Channels = 1;		// 1 2
	XE::uint32 Frequency = 44100;	// 8000 11025 16000 22050 37800 44100 48000
};

struct XE_API AudioSourceDesc
{
	XE::String Name;

	bool Pause = true;		// 暂停,默认暂停
	bool Relative = false;	// 指示Position是否是相对于Listener
	bool Looping = false;	// 循环播放
	XE::float32 InnerConeAngle = 360.0f;	// 内锥覆盖的角度
	XE::float32 OuterConeAngle = 360.0f;	// 外锥覆盖的角度
	XE::float32 Pitch = 1.0f;		// 采样倍增,应为正数
	XE::float32 MinGain = 0.0f;		// 最小音量,应为正数
	XE::float32 MaxGain = 1.0f;		// 最大音量,应为正数
	XE::float32 MinDistance = 0.0f;		// 音量开始衰减的距离
	XE::float32 MaxDistance = 1.0f;		// 音量彻底消失的距离
};


enum class AudioCommandType
{
	PLAY,

	GAIN,
	SPEED_OF_SOUND,
	DOPPLER_FACTOR,

	CREATE_BUFFER,
	CREATE_SOURCE,

	LISTENER_TRANSFORM,

	BUFFER_FILL,

	SOURCE_QUEUE_BUFFER,
	SOURCE_UNQUE_BUFFER,
	SOURCE_PLAY,
	SOURCE_RELATIVE,
	SOURCE_CONE_ANGLE,
	SOURCE_PITCH,
	SOURCE_TRANSFORM,
	SOURCE_GAIN,
	SOURCE_DISTANCE,
	SOURCE_PAUSE,

	CAPTURE_START,
	CAPTURE_SAMPLES,

	SOURCE_STOP,
	CAPTURE_STOP,

	PAUSE,

	DESTROY_BUFFER,
	DESTROY_SOURCE,
};

struct XE_API AudioItem
{
	AudioCommandType Type;
	XE::uint64 Handle;
	XE::uint64 Code;
	XE::uint64 Size;
};

struct XE_API AudioFrame
{
public:
	void Reset()
	{
		CommandSize = 0;
		Buffers.unsafe().Clear();
	}

public:
	std::mutex Mutex;
	std::atomic< XE::uint64 > CommandSize = 0;
	std::array< AudioItem, AUDIO_MAX_COMMAND > Command;
	XE::Concurrent< XE::Buffer, std::mutex > Buffers;
};

using UnqueBufferCallbackType = void( * )( void * userdata, XE::AudioBufferHandle buffer );

// Audio
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Render

static constexpr XE::uint64 RENDER_MAX_PASS = 512;
static constexpr XE::uint64 RENDER_MAX_QUERY = 512;
static constexpr XE::uint64 RENDER_MAX_BUFFER = 512;
static constexpr XE::uint64 RENDER_MAX_TARGET = 512;
static constexpr XE::uint64 RENDER_MAX_SHADER = 512;
static constexpr XE::uint64 RENDER_MAX_TEXTURE = 512;
static constexpr XE::uint64 RENDER_MAX_SAMPLER = 512;
static constexpr XE::uint64 RENDER_MAX_COMMAND_BUFFER = 32;
static constexpr XE::uint64 RENDER_MAX_COMMAND_BUNDLE = 64;
static constexpr XE::uint64 RENDER_MAX_SHADER_PROGRAM = 1024;
static constexpr XE::uint64 RENDER_MAX_DESCRIPTOR_SET = 1024;
static constexpr XE::uint64 RENDER_MAX_PIPELINE_STATE = 1024;
static constexpr XE::uint64 RENDER_MAX_VIRTUAL_BUFFER = 1024;

static constexpr XE::uint64 RENDER_MAX_SWAP_CHAIN_BACK_BUFFER = 2;

static constexpr XE::uint64 RENDER_MAX_RTV_DESC_HEAP_CACHE = 512;
static constexpr XE::uint64 RENDER_MAX_DSV_DESC_HEAP_CACHE = 512;
static constexpr XE::uint64 RENDER_MAX_SAMPLER_DESC_HEAP_CACHE = 512;
static constexpr XE::uint64 RENDER_MAX_CBV_SRV_UAV_DESC_HEAP_CACHE = 512;

DECL_HANDLE( RenderPass );
DECL_HANDLE( RenderQuery );
DECL_HANDLE( RenderBuffer );
DECL_HANDLE( RenderTarget );
DECL_HANDLE( RenderShader );
DECL_HANDLE( RenderTexture );
DECL_HANDLE( RenderSampler );
DECL_HANDLE( RenderContext );
DECL_HANDLE( RenderDescriptorSet );
DECL_HANDLE( RenderCommandBuffer );
DECL_HANDLE( RenderCommandBundle );
DECL_HANDLE( RenderPipelineState );
DECL_HANDLE( RenderShaderProgram );
DECL_HANDLE( RenderVirtualBuffer );


enum class RenderApiType
{
	NIL,
	D3D12,
	METAL,
	VULKAN,
	WEBGPU,
};

enum class RenderCullType
{
	DISABLED,
	FRONT,
	BACK,
};

enum class RenderQueryType
{ 
	SAMPLES_PASSED,
	ANY_SAMPLES_PASSED,
	ANY_SAMPLES_PASSED_CONSERVATIVE,
	TIME_ELAPSED,
	STREAMOUT_PRIMITIVES_WRITTEN,
	STREAMOUT_OVERFLOW,
	PIPELINE_STATISTICS,
};

enum class RenderAccessType
{
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE,
};

enum class RenderShaderType
{
	CS,
	VS,
	PS,
	HS,
	DS,
	GS,
};

enum class RenderFormatType
{
	UNKNOWN,
	R8_UNORM,
	R8_SNORM,
	R8_UINT,
	R8_SINT,
	R16_UNORM,
	R16_SNORM,
	R16_UINT,
	R16_SINT,
	R16_SFLOAT,
	R32_UINT,
	R32_SINT,
	R32_SFLOAT,

	R8G8_UNORM,
	R8G8_SNORM,
	R8G8_UINT,
	R8G8_SINT,
	R16G16_UNORM,
	R16G16_SNORM,
	R16G16_UINT,
	R16G16_SINT,
	R16G16_SFLOAT,
	R32G32_UINT,
	R32G32_SINT,
	R32G32_SFLOAT,

	R32G32B32_UINT,
	R32G32B32_SINT,
	R32G32B32_SFLOAT,

	R8G8B8A8_UNORM,
	R8G8B8A8_SNORM,
	R8G8B8A8_UINT,
	R8G8B8A8_SINT,
	R8G8B8A8_SRGB,
	R16G16B16A16_UNORM,
	R16G16B16A16_SNORM,
	R16G16B16A16_UINT,
	R16G16B16A16_SINT,
	R16G16B16A16_SFLOAT,
	R32G32B32A32_UINT,
	R32G32B32A32_SINT,
	R32G32B32A32_SFLOAT,

	B8G8R8A8_UNORM,
	B8G8R8A8_SRGB,

	B5G6R5_UNORM_PACK16,
	B5G5R5A1_UNORM_PACK16,
	R10G10B10A2_UNORM_PACK32,
	R10G10B10A2_UINT_PACK32,
	R9G9B9E5_UFLOAT_PACK32,
	R11G11B10_UFLOAT_PACK32,

	D16_UNORM,
	D32_SFLOAT,
	D24_UNORM_S8_UINT,
	D32_SFLOAT_S8_UINT,

	BC1_UNORM_BLOCK,
	BC1_SRGB_BLOCK,
	BC2_UNORM_BLOCK,
	BC2_SRGB_BLOCK,
	BC3_UNORM_BLOCK,
	BC3_SRGB_BLOCK,
	BC4_UNORM_BLOCK,
	BC4_SNORM_BLOCK,
	BC5_UNORM_BLOCK,
	BC5_SNORM_BLOCK,
	BC6_UFLOAT_BLOCK,
	BC6_SFLOAT_BLOCK,
	BC7_UNORM_BLOCK,
	BC7_SRGB_BLOCK,
};

enum class RenderTextureType
{
	TEXTURE_1D,
	TEXTURE_1D_ARRAY,
	TEXTURE_2D,
	TEXTURE_2D_MS,
	TEXTURE_2D_ARRAY,
	TEXTURE_3D,
	TEXTURE_CUBE,
	TEXTURE_2D_MS_ARRAY,
};

enum class RenderPolygonType
{
	FILL,
	WIREFRAME,
	POINTS,
};

enum class RenderLogicOpType
{
	DISABLED,
	CLEAR,
	SET,
	COPY,
	COPY_INVERTED,
	NOOP,
	INVERT,
	AND,
	AND_REVERSE,
	AND_INVERTED,
	NAND,
	OR,
	OR_REVERSE,
	OR_INVERTED,
	NOR,
	XOR,
	EQUIV,
};

enum class RenderBlendOpType
{
	ZERO,
	ONE,
	SRC_COLOR,
	INV_SRC_COLOR,
	SRC_ALPHA,
	INV_SRC_ALPHA,
	DST_COLOR,
	INV_DST_COLOR,
	DST_ALPHA,
	INV_DST_ALPHA,
	SRC_ALPHA_SATURATE,
	BLEND_FACTOR,
	INV_BLEND_FACTOR,
	SRC1_COLOR,
	INV_SRC1_COLOR,
	SRC1_ALPHA,
	INV_SRC1_ALPHA,
};

enum class RenderResourceType
{
	BUFFER,
	TEXTURE,
	SAMPLER,
};

enum class RenderStencilOpType
{
	KEEP,
	ZERO,
	REPLACE,
	INC_CLAMP,
	DEC_CLAMP,
	INVERT,
	INC_WRAP,
	DEC_WRAP,
};

enum class RenderCompareOpType
{
	NEVER_PASS,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS_PASS,
};

enum class RenderConditionType
{
	WAIT,
	NO_WAIT,
	BY_REGION_WAIT,
	BY_REGION_NO_WAIT,
	WAIT_INVERTED,
	NO_WAIT_INVERTED,
	BY_REGION_WAIT_INVERTED,
	BY_REGION_NO_WAIT_INVERTED,
};

enum class RenderAttributeType
{
	UNDEFINED,
	CLIP_DISTANCE,
	COLOR,
	CULL_DISTANCE,
	DEPTH,
	DEPTH_GREATER,
	DEPTH_LESS,
	FRONT_FACING,
	INSTANCE_ID,
	POSITION,
	PRIMITIVE_ID,
	RENDER_TARGET_INDEX,
	SAMPLE_MASK,
	SAMPLE_ID,
	STENCIL,
	VERTEX_ID,
	VIEWPORT_INDEX,
};

enum class RenderAttachmentType
{
	COLOR,
	DEPTH,
	STENCIL,
	DEPTH_STENCIL,
};

enum class RenderStencilFaceType
{
	FRONT = 1,
	BACK = 2,
	FRONT_BACK = FRONT | BACK,
};

enum class RenderSamplerFilterType
{
	NEAREST,
	LINEAR,
};

enum class RenderPipelineStateType
{
	COMPUTE,
	GRAPHICS,
};

enum class RenderResourceStatesType
{
	COMMON,
	VERTEX_AND_CONSTANT_BUFFER,
	INDEX_BUFFER,
	RENDER_TARGET,
	UNORDERED_ACCESS,
	DEPTH_WRITE,
	DEPTH_READ,
	NON_PIXEL_SHADER_RESOURCE,
	PIXEL_SHADER_RESOURCE,
	STREAM_OUT,
	INDIRECT_ARGUMENT,
	COPY_DEST,
	COPY_SOURCE,
	RESOLVE_DEST,
	RESOLVE_SOURCE,
	RAYTRACING_ACCELERATION_STRUCTURE,
	SHADING_RATE_SOURCE,
	GENERIC_READ,
	PRESENT,
	PREDICATION,
};

enum class RenderTextureSwizzleType
{
	ZERO,
	ONE,
	RED,
	GREEN,
	BLUE,
	ALPHA,
};

enum class RenderBlendArithmeticType
{
	ADD,           
	SUBTRACT,      
	REV_SUBTRACT,   
	MIN,           
	MAX,           
};

enum class RenderAttachmentLoadOpType
{
	DISCARD,
	PRESERVE,
	CLEAR,
	NO_ACCESS,
};

enum class RenderAttachmentStoreOpType
{
	DISCARD,
	PRESERVE,
	RESOLVE,
	NO_ACCESS,
};

enum class RenderPrimitiveTopologyType
{
	POINT_LIST,
	LINE_LIST,
	LINE_STRIP,
	LINE_LOOP,
	LINE_LIST_ADJACENCY,
	LINE_STRIP_ADJACENCY,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	TRIANGLE_LIST_ADJACENCY,
	TRIANGLE_STRIP_ADJACENCY,
	PATCHES_1,
	PATCHES_2,
	PATCHES_3,
	PATCHES_4,
	PATCHES_5,
	PATCHES_6,
	PATCHES_7,
	PATCHES_8,
	PATCHES_9,
	PATCHES_10,
	PATCHES_11,
	PATCHES_12,
	PATCHES_13,
	PATCHES_14,
	PATCHES_15,
	PATCHES_16,
	PATCHES_17,
	PATCHES_18,
	PATCHES_19,
	PATCHES_20,
	PATCHES_21,
	PATCHES_22,
	PATCHES_23,
	PATCHES_24,
	PATCHES_25,
	PATCHES_26,
	PATCHES_27,
	PATCHES_28,
	PATCHES_29,
	PATCHES_30,
	PATCHES_31,
	PATCHES_32,
};

enum class RenderSamplerAddressModeType
{
	REPEAT,
	MIRROR,
	CLAMP,
	BORDER,
	MIRRORONCE,
};

enum class RenderTessellationPartitionType
{
	UNDEFINED,
	INTEGER,
	POW2,
	FRACTIONAL_ODD,
	FRACTIONAL_EVEN,
};


enum class RenderPassFlag
{
	NONE = 0,
	ALLOW_UAV_WRITES = 1 << 0,
	SUSPENDING_PASS = 1 << 1,
	RESUMING_PASS = 1 << 2,
};
DECL_FLAGS( RenderPassFlag, RenderPassFlags );

enum class RenderMiscFlag
{
	DYNAMIC_USAGE =1 << 0,
	FIXED_SAMPLES =1 << 1,
	GENERATE_MIPS =1 << 2,
	NOINITIAL_DATA =1 << 3,
	APPEND =1 << 4,
	COUNTER =1 << 5,
};
DECL_FLAGS( RenderMiscFlag, RenderMiscFlags );

enum class RenderBindFlag
{
	VERTEX_BUFFER = 1 << 0,
	INDEX_BUFFER = 1 << 1,
	CONSTANT_BUFFER = 1 << 2,
	STREAM_OUTPUT_BUFFER = 1 << 3,
	INDIRECT_BUFFER = 1 << 4,
	SAMPLED = 1 << 5,
	STORAGE = 1 << 6,
	COLOR_ATTACHMENT = 1 << 7,
	DEPTH_STENCIL_ATTACHMENT = 1 << 8,
	COMBINED_SAMPLER = 1 << 9,
	COPY_SRC = 1 << 10,
	COPY_DST = 1 << 11,
};
DECL_FLAGS( RenderBindFlag, RenderBindFlags );

enum class RenderStageFlag
{
	VERTEX = 1 << 0,
	TESS_CONTROL = 1 << 1,
	TESS_EVALUATION = 1 << 2,
	GEOMETRY = 1 << 3,
	FRAGMENT = 1 << 4,
	COMPUTE = 1 << 5,
	ALL_TESS = TESS_CONTROL | TESS_EVALUATION,
	ALL_GRAPHICS = VERTEX | ALL_TESS | GEOMETRY | FRAGMENT,
	ALL = ALL_GRAPHICS | COMPUTE,
};
DECL_FLAGS( RenderStageFlag, RenderStageFlags );

enum class RenderClearFlag
{
	DEPTH = 1 << 1,
	STENCIL = 1 << 2,
	COLOR = 1 << 3,
};
DECL_FLAGS( RenderClearFlag, RenderClearFlags );


struct XE_API RenderClear
{
	XE::RenderClearFlags Flags;
	XE::FColor Color;
	XE::float32 Depth;
	XE::uint32 Stencil;
};

struct XE_API RenderPassDesc
{
	struct AttachmentDesc
	{
		XE::RenderFormatType Format;
		XE::RenderTargetHandle RenderTarget;
		XE::RenderAttachmentLoadOpType LoadOp;
		XE::RenderAttachmentStoreOpType StoreOp;
	};

	XE::String Name;
	XE::uint32 Samples;
	XE::RenderClear Clear;
	AttachmentDesc DepthStencil;
	XE::Array< AttachmentDesc > Colors;
};

struct XE_API RenderQueryDesc
{
	XE::String Name;
	RenderQueryType Type;
	XE::uint32 NumQueries;
	bool RenderCondition;
};

struct XE_API RenderBufferDesc
{
	XE::String Name;
	XE::uint64 Size;
	XE::uint32 Stride;
	XE::RenderFormatType Format;
	XE::RenderBindFlags BindFlags;
	XE::RenderAccessType CpuAccess;
};

struct XE_API RenderTargetDesc
{
	XE::String Name;
	XE::Vec2i Resolution;
	XE::RenderAttachmentType Type;
	XE::RenderTextureHandle Texture;

	bool CustomMultiSampling = false;
};

struct XE_API RenderShaderDesc
{
	XE::RenderShaderType Type;
	XE::String Name;
	XE::String Enter;
	XE::String Source;
};

struct XE_API RenderTextureDesc
{
	XE::String Name;
	XE::RenderTextureType Type;
	XE::RenderBindFlags BindFlags;
	XE::RenderFormatType Format;
	XE::uint32 Width, Height, Depth;
	XE::uint32 MipLevels;
	XE::RenderClear Clear;
};

struct XE_API RenderSamplerDesc
{
	XE::String Name;
	XE::RenderSamplerAddressModeType AddressModeU;
	XE::RenderSamplerAddressModeType AddressModeV;
	XE::RenderSamplerAddressModeType AddressModeW;
	XE::RenderSamplerFilterType MinFilter;
	XE::RenderSamplerFilterType MagFilter;
	XE::RenderSamplerFilterType MipMapFilter;
	bool MipMapping;
	XE::float32 MipMapLODBias;
	XE::float32 MinLOD;
	XE::float32 MaxLOD;
	std::uint32_t MaxAnisotropy;
	bool CompareEnabled;
	XE::RenderCompareOpType CompareOp;
	XE::FColor BorderColor;
};

struct XE_API RenderContextDesc
{
	XE::String Name;
	XE::uint64 Vendor;
	XE::uint64 Device;
	XE::uint64 VideoMemory;
	XE::uint64 SystemMemory;
	XE::uint64 SharedMemory;
};

struct XE_API RenderDescriptorSetDesc
{
	struct BindingDesc
	{
		XE::String Name;
		XE::uint32 Slot;
		XE::uint32 ArraySize;
		XE::RenderBindFlags Bind;
		XE::RenderStageFlags Stage;
		XE::RenderResourceType Type;
	};

	struct RenderResourceViewDesc
	{
		XE::String Name;
		std::variant< std::monostate, XE::RenderBufferHandle, XE::RenderTextureHandle > Resource;

		union
		{
			struct BufferView
			{
				XE::RenderFormatType Format;
				XE::uint64 Offset;
				XE::uint64 Size;
			} Buffer;

			struct TextureView
			{
				XE::RenderTextureType Type;
				XE::RenderFormatType Format;
				XE::uint32 BaseMipLevel = 0;
				XE::uint32 NumMipLevels = 1;
				XE::uint32 BaseArrayLayer = 0;
				XE::uint32 NumArrayLayers = 1;
				RenderTextureSwizzleType R;
				RenderTextureSwizzleType G;
				RenderTextureSwizzleType B;
				RenderTextureSwizzleType A;
			} Texture;
		};
	};

	XE::String Name;
	XE::Array< BindingDesc > Bindings;
	XE::Array< RenderResourceViewDesc > ResourceViews;
};

struct XE_API RenderShaderProgramDesc
{
	XE::String Name;
	XE::RenderShaderHandle CS;
	XE::RenderShaderHandle VS;
	XE::RenderShaderHandle PS;
	XE::RenderShaderHandle HS;
	XE::RenderShaderHandle DS;
	XE::RenderShaderHandle GS;
};

struct XE_API RenderCommandBundleDesc
{
	XE::String Name;
};

struct XE_API RenderVirtualBufferDesc
{
	XE::String Name;
	XE::uint64 Size;
	XE::uint64 Stride;
};

struct XE_API RenderPipelineStateDesc
{
	XE::String Name;
	XE::RenderPipelineStateType Type;
	XE::RenderDescriptorSetHandle DescriptorSet;
	XE::RenderShaderProgramHandle ShaderProgram;
};

struct XE_API RenderGraphicsPipelineStateDesc : public RenderPipelineStateDesc
{
	struct RenderDepthStateDesc
	{
		bool TestEnabled;
		bool WriteEnabled;

		XE::RenderCompareOpType Compare;
	};

	struct RenderBlendStateDesc
	{
		struct RenderBlendTargetDesc
		{
			bool BlendEnabled = false;
			bool LogicOpEnable = false;
			XE::RenderLogicOpType LogicOp = XE::RenderLogicOpType::NOOP;
			XE::RenderBlendOpType SrcColor = XE::RenderBlendOpType::SRC_ALPHA;
			XE::RenderBlendOpType DstColor = XE::RenderBlendOpType::INV_SRC_ALPHA;
			XE::RenderBlendArithmeticType ColorArithmetic = XE::RenderBlendArithmeticType::ADD;
			XE::RenderBlendOpType SrcAlpha = XE::RenderBlendOpType::SRC_ALPHA;
			XE::RenderBlendOpType DstAlpha = XE::RenderBlendOpType::INV_SRC_ALPHA;
			XE::RenderBlendArithmeticType AlphaArithmetic = XE::RenderBlendArithmeticType::ADD;
			XE::Color ColorMask = { 1, 1, 1, 1 };
		};

		bool AlphaToCoverageEnabled = false;
		bool IndependentBlendEnabled = false;
		XE::uint32 SampleMask = ~0u;
		XE::RenderLogicOpType LogicOp = XE::RenderLogicOpType::DISABLED;
		XE::FColor BlendFactor = { 0, 0, 0, 0 };
		bool BlendFactorDynamic = false;
		RenderBlendTargetDesc BlendTargets[8];
	};

	struct RenderInputLayoutDesc
	{
		struct AttributeDesc
		{
			XE::String Name;
			XE::RenderFormatType Format;
			XE::uint32 Location;
			XE::uint32 SemanticIndex;
			XE::RenderAttributeType Type;
			XE::uint32 Slot;
			XE::uint32 Offset;
			XE::uint32 Stride;
			XE::uint32 InstanceDivisor;
		};

		XE::String Name;
		XE::Array< AttributeDesc > Attributes;
	};

	struct RenderStencilStateDesc
	{
		bool TestEnabled;
		bool ReferenceDynamic;

		XE::uint32 FrontReadMask;
		XE::uint32 FrontWriteMask;
		XE::uint32 FrontReference;
		XE::RenderStencilOpType FrontStencilFail;
		XE::RenderStencilOpType FrontDepthFail;
		XE::RenderStencilOpType FrontDepthPass;
		XE::RenderCompareOpType FrontCompare;
		XE::uint32 BackReadMask;
		XE::uint32 BackWriteMask;
		XE::uint32 BackReference;
		XE::RenderStencilOpType BackStencilFail;
		XE::RenderStencilOpType BackDepthFail;
		XE::RenderStencilOpType BackDepthPass;
		XE::RenderCompareOpType BackCompare;
	};

	struct RenderRasterizerStateDesc
	{
		XE::RenderCullType CullMode;
		XE::RenderPolygonType PolygonMode;

		XE::float32 DepthBiasClamp = 0.0f;
		XE::float32 DepthBiasSlopeFactor = 0.0f;
		XE::float32 DepthBiasConstantFactor = 0.0f;

		bool FrontCCW = false;
		bool DiscardEnabled = false;
		bool DepthClampEnabled = false;
		bool ScissorTestEnabled = false;
		bool MultiSampleEnabled = false;
		bool AntiAliasedLineEnabled = false;
		bool ConservativeRasterization = false;
		XE::float32 LineWidth = 1.0f;
	};

	struct RenderTessellationStateDesc
	{
		XE::RenderTessellationPartitionType Partition;
		XE::RenderFormatType IndexFormat;
		XE::uint32 MaxTessFactor;
		bool OutputWindingCCW;
	};

	XE::RenderPrimitiveTopologyType Topology;

	XE::uint32 SampleCount;
	XE::RenderFormatType DepthStencil;
	XE::Array< XE::RenderFormatType > RenderTargets;

	RenderDepthStateDesc DepthState;
	RenderBlendStateDesc BlendState;
	RenderInputLayoutDesc InputLayout;
	RenderStencilStateDesc StencilState;
	RenderRasterizerStateDesc RasterizerState;
	RenderTessellationStateDesc TessellationState;
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