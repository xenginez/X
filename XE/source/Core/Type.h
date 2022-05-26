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

DECL_HANDLE( GraphicsInstance );
DECL_HANDLE( GraphicsAdapter );
DECL_HANDLE( GraphicsDevice );
DECL_HANDLE( GraphicsBindGroup );
DECL_HANDLE( GraphicsBindGroupLayout );
DECL_HANDLE( GraphicsBuffer );
DECL_HANDLE( GraphicsCommandBuffer );
DECL_HANDLE( GraphicsCommandEncoder );
DECL_HANDLE( GraphicsComputePassEncoder );
DECL_HANDLE( GraphicsComputePipeline );
DECL_HANDLE( GraphicsPipelineLayout );
DECL_HANDLE( GraphicsQuerySet );
DECL_HANDLE( GraphicsQueue );
DECL_HANDLE( GraphicsRenderBundle );
DECL_HANDLE( GraphicsRenderBundleEncoder );
DECL_HANDLE( GraphicsRenderPassEncoder );
DECL_HANDLE( GraphicsRenderPipeline );
DECL_HANDLE( GraphicsSampler );
DECL_HANDLE( GraphicsShaderModule );
DECL_HANDLE( GraphicsSurface );
DECL_HANDLE( GraphicsSwapChain );
DECL_HANDLE( GraphicsTexture );
DECL_HANDLE( GraphicsTextureView );

enum class GraphicsSType
{
    Invalid = 0x00000000,
    SurfaceDescriptorFromMetalLayer = 0x00000001,
    SurfaceDescriptorFromWindowsHWND = 0x00000002,
    SurfaceDescriptorFromXlibWindow = 0x00000003,
    SurfaceDescriptorFromCanvasHTMLSelector = 0x00000004,
    ShaderModuleSPIRVDescriptor = 0x00000005,
    ShaderModuleWGSLDescriptor = 0x00000006,
    PrimitiveDepthClipControl = 0x00000007,
    SurfaceDescriptorFromWaylandSurface = 0x00000008,
    SurfaceDescriptorFromAndroidNativeWindow = 0x00000009,
    SurfaceDescriptorFromXcbWindow = 0x0000000A,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsSType );

enum class GraphicsAdapterType
{
    DiscreteGPU = 0x00000000,
    IntegratedGPU = 0x00000001,
    SoftwareCPU = 0x00000002,
    Unknown = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsAdapterType );

enum class GraphicsAddressMode
{
    Repeat = 0x00000000,
    MirrorRepeat = 0x00000001,
    ClampToEdge = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsAddressMode );

enum class GraphicsBackendType
{
    Null = 0x00000000,
    WebGPU = 0x00000001,
    D3D11 = 0x00000002,
    D3D12 = 0x00000003,
    Metal = 0x00000004,
    Vulkan = 0x00000005,
    OpenGL = 0x00000006,
    OpenGLES = 0x00000007,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsBackendType );

enum class GraphicsBlendFactor
{
    Zero = 0x00000000,
    One = 0x00000001,
    Src = 0x00000002,
    OneMinusSrc = 0x00000003,
    SrcAlpha = 0x00000004,
    OneMinusSrcAlpha = 0x00000005,
    Dst = 0x00000006,
    OneMinusDst = 0x00000007,
    DstAlpha = 0x00000008,
    OneMinusDstAlpha = 0x00000009,
    SrcAlphaSaturated = 0x0000000A,
    Constant = 0x0000000B,
    OneMinusConstant = 0x0000000C,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsBlendFactor );

enum class GraphicsBlendOperation
{
    Add = 0x00000000,
    Subtract = 0x00000001,
    ReverseSubtract = 0x00000002,
    Min = 0x00000003,
    Max = 0x00000004,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsBlendOperation );

enum class GraphicsBufferBindingType
{
    Undefined = 0x00000000,
    Uniform = 0x00000001,
    Storage = 0x00000002,
    ReadOnlyStorage = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsBufferBindingType );

enum class GraphicsBufferMapAsyncStatus
{
    Success = 0x00000000,
    Error = 0x00000001,
    Unknown = 0x00000002,
    DeviceLost = 0x00000003,
    DestroyedBeforeCallback = 0x00000004,
    UnmappedBeforeCallback = 0x00000005,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsBufferMapAsyncStatus );

enum class GraphicsCompareFunction
{
    Undefined = 0x00000000,
    Never = 0x00000001,
    Less = 0x00000002,
    LessEqual = 0x00000003,
    Greater = 0x00000004,
    GreaterEqual = 0x00000005,
    Equal = 0x00000006,
    NotEqual = 0x00000007,
    Always = 0x00000008,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsCompareFunction );

enum class GraphicsCompilationInfoRequestStatus
{
    Success = 0x00000000,
    Error = 0x00000001,
    DeviceLost = 0x00000002,
    Unknown = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsCompilationInfoRequestStatus );

enum class GraphicsCompilationMessageType
{
    Error = 0x00000000,
    Warning = 0x00000001,
    Info = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsCompilationMessageType );

enum class GraphicsComputePassTimestampLocation
{
    Beginning = 0x00000000,
    End = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsComputePassTimestampLocation );

enum class GraphicsCreatePipelineAsyncStatus
{
    Success = 0x00000000,
    Error = 0x00000001,
    DeviceLost = 0x00000002,
    DeviceDestroyed = 0x00000003,
    Unknown = 0x00000004,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsCreatePipelineAsyncStatus );

enum class GraphicsCullMode
{
    None = 0x00000000,
    Front = 0x00000001,
    Back = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsCullMode );

enum class GraphicsDeviceLostReason
{
    Undefined = 0x00000000,
    Destroyed = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsDeviceLostReason );

enum class GraphicsErrorFilter
{
    Validation = 0x00000000,
    OutOfMemory = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsErrorFilter );

enum class GraphicsErrorType
{
    NoError = 0x00000000,
    Validation = 0x00000001,
    OutOfMemory = 0x00000002,
    Unknown = 0x00000003,
    DeviceLost = 0x00000004,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsErrorType );

enum class GraphicsFeatureName
{
    Undefined = 0x00000000,
    DepthClipControl = 0x00000001,
    Depth24UnormStencil8 = 0x00000002,
    Depth32FloatStencil8 = 0x00000003,
    TimestampQuery = 0x00000004,
    PipelineStatisticsQuery = 0x00000005,
    TextureCompressionBC = 0x00000006,
    TextureCompressionETC2 = 0x00000007,
    TextureCompressionASTC = 0x00000008,
    IndirectFirstInstance = 0x00000009,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsFeatureName );

enum class GraphicsFilterMode
{
    Nearest = 0x00000000,
    Linear = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsFilterMode );

enum class GraphicsFrontFace
{
    CCW = 0x00000000,
    CW = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsFrontFace );

enum class GraphicsIndexFormat
{
    Undefined = 0x00000000,
    Uint16 = 0x00000001,
    Uint32 = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsIndexFormat );

enum class GraphicsLoadOp
{
    Undefined = 0x00000000,
    Clear = 0x00000001,
    Load = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsLoadOp );

enum class GraphicsMipmapFilterMode
{
    Nearest = 0x00000000,
    Linear = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsMipmapFilterMode );

enum class GraphicsPipelineStatisticName
{
    VertexShaderInvocations = 0x00000000,
    ClipperInvocations = 0x00000001,
    ClipperPrimitivesOut = 0x00000002,
    FragmentShaderInvocations = 0x00000003,
    ComputeShaderInvocations = 0x00000004,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsPipelineStatisticName );

enum class GraphicsPowerPreference
{
    Undefined = 0x00000000,
    LowPower = 0x00000001,
    HighPerformance = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsPowerPreference );

enum class GraphicsPredefinedColorSpace
{
    Undefined = 0x00000000,
    Srgb = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsPredefinedColorSpace );

enum class GraphicsPresentMode
{
    Immediate = 0x00000000,
    Mailbox = 0x00000001,
    Fifo = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsPresentMode );

enum class GraphicsPrimitiveTopology
{
    PointList = 0x00000000,
    LineList = 0x00000001,
    LineStrip = 0x00000002,
    TriangleList = 0x00000003,
    TriangleStrip = 0x00000004,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsPrimitiveTopology );

enum class GraphicsQueryType
{
    Occlusion = 0x00000000,
    PipelineStatistics = 0x00000001,
    Timestamp = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsQueryType );

enum class GraphicsQueueWorkDoneStatus
{
    Success = 0x00000000,
    Error = 0x00000001,
    Unknown = 0x00000002,
    DeviceLost = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsQueueWorkDoneStatus );

enum class GraphicsRenderPassTimestampLocation
{
    Beginning = 0x00000000,
    End = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsRenderPassTimestampLocation );

enum class GraphicsRequestAdapterStatus
{
    Success = 0x00000000,
    Unavailable = 0x00000001,
    Error = 0x00000002,
    Unknown = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsRequestAdapterStatus );

enum class GraphicsRequestDeviceStatus
{
    Success = 0x00000000,
    Error = 0x00000001,
    Unknown = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsRequestDeviceStatus );

enum class GraphicsSamplerBindingType
{
    Undefined = 0x00000000,
    Filtering = 0x00000001,
    NonFiltering = 0x00000002,
    Comparison = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsSamplerBindingType );

enum class GraphicsStencilOperation
{
    Keep = 0x00000000,
    Zero = 0x00000001,
    Replace = 0x00000002,
    Invert = 0x00000003,
    IncrementClamp = 0x00000004,
    DecrementClamp = 0x00000005,
    IncrementWrap = 0x00000006,
    DecrementWrap = 0x00000007,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsStencilOperation );

enum class GraphicsStorageTextureAccess
{
    Undefined = 0x00000000,
    WriteOnly = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsStorageTextureAccess );

enum class GraphicsStoreOp
{
    Undefined = 0x00000000,
    Store = 0x00000001,
    Discard = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsStoreOp );

enum class GraphicsTextureAspect
{
    All = 0x00000000,
    StencilOnly = 0x00000001,
    DepthOnly = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsTextureAspect );

enum class GraphicsTextureComponentType
{
    Float = 0x00000000,
    Sint = 0x00000001,
    Uint = 0x00000002,
    DepthComparison = 0x00000003,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsTextureComponentType );

enum class GraphicsTextureDimension
{
    TextureDimension_1D = 0x00000000,
    TextureDimension_2D = 0x00000001,
    TextureDimension_3D = 0x00000002,
    TextureDimension_Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsTextureDimension );

enum class GraphicsTextureFormat
{
    Undefined = 0x00000000,
    R8Unorm = 0x00000001,
    R8Snorm = 0x00000002,
    R8Uint = 0x00000003,
    R8Sint = 0x00000004,
    R16Uint = 0x00000005,
    R16Sint = 0x00000006,
    R16Float = 0x00000007,
    RG8Unorm = 0x00000008,
    RG8Snorm = 0x00000009,
    RG8Uint = 0x0000000A,
    RG8Sint = 0x0000000B,
    R32Float = 0x0000000C,
    R32Uint = 0x0000000D,
    R32Sint = 0x0000000E,
    RG16Uint = 0x0000000F,
    RG16Sint = 0x00000010,
    RG16Float = 0x00000011,
    RGBA8Unorm = 0x00000012,
    RGBA8UnormSrgb = 0x00000013,
    RGBA8Snorm = 0x00000014,
    RGBA8Uint = 0x00000015,
    RGBA8Sint = 0x00000016,
    BGRA8Unorm = 0x00000017,
    BGRA8UnormSrgb = 0x00000018,
    RGB10A2Unorm = 0x00000019,
    RG11B10Ufloat = 0x0000001A,
    RGB9E5Ufloat = 0x0000001B,
    RG32Float = 0x0000001C,
    RG32Uint = 0x0000001D,
    RG32Sint = 0x0000001E,
    RGBA16Uint = 0x0000001F,
    RGBA16Sint = 0x00000020,
    RGBA16Float = 0x00000021,
    RGBA32Float = 0x00000022,
    RGBA32Uint = 0x00000023,
    RGBA32Sint = 0x00000024,
    Stencil8 = 0x00000025,
    Depth16Unorm = 0x00000026,
    Depth24Plus = 0x00000027,
    Depth24PlusStencil8 = 0x00000028,
    Depth24UnormStencil8 = 0x00000029,
    Depth32Float = 0x0000002A,
    Depth32FloatStencil8 = 0x0000002B,
    BC1RGBAUnorm = 0x0000002C,
    BC1RGBAUnormSrgb = 0x0000002D,
    BC2RGBAUnorm = 0x0000002E,
    BC2RGBAUnormSrgb = 0x0000002F,
    BC3RGBAUnorm = 0x00000030,
    BC3RGBAUnormSrgb = 0x00000031,
    BC4RUnorm = 0x00000032,
    BC4RSnorm = 0x00000033,
    BC5RGUnorm = 0x00000034,
    BC5RGSnorm = 0x00000035,
    BC6HRGBUfloat = 0x00000036,
    BC6HRGBFloat = 0x00000037,
    BC7RGBAUnorm = 0x00000038,
    BC7RGBAUnormSrgb = 0x00000039,
    ETC2RGB8Unorm = 0x0000003A,
    ETC2RGB8UnormSrgb = 0x0000003B,
    ETC2RGB8A1Unorm = 0x0000003C,
    ETC2RGB8A1UnormSrgb = 0x0000003D,
    ETC2RGBA8Unorm = 0x0000003E,
    ETC2RGBA8UnormSrgb = 0x0000003F,
    EACR11Unorm = 0x00000040,
    EACR11Snorm = 0x00000041,
    EACRG11Unorm = 0x00000042,
    EACRG11Snorm = 0x00000043,
    ASTC4x4Unorm = 0x00000044,
    ASTC4x4UnormSrgb = 0x00000045,
    ASTC5x4Unorm = 0x00000046,
    ASTC5x4UnormSrgb = 0x00000047,
    ASTC5x5Unorm = 0x00000048,
    ASTC5x5UnormSrgb = 0x00000049,
    ASTC6x5Unorm = 0x0000004A,
    ASTC6x5UnormSrgb = 0x0000004B,
    ASTC6x6Unorm = 0x0000004C,
    ASTC6x6UnormSrgb = 0x0000004D,
    ASTC8x5Unorm = 0x0000004E,
    ASTC8x5UnormSrgb = 0x0000004F,
    ASTC8x6Unorm = 0x00000050,
    ASTC8x6UnormSrgb = 0x00000051,
    ASTC8x8Unorm = 0x00000052,
    ASTC8x8UnormSrgb = 0x00000053,
    ASTC10x5Unorm = 0x00000054,
    ASTC10x5UnormSrgb = 0x00000055,
    ASTC10x6Unorm = 0x00000056,
    ASTC10x6UnormSrgb = 0x00000057,
    ASTC10x8Unorm = 0x00000058,
    ASTC10x8UnormSrgb = 0x00000059,
    ASTC10x10Unorm = 0x0000005A,
    ASTC10x10UnormSrgb = 0x0000005B,
    ASTC12x10Unorm = 0x0000005C,
    ASTC12x10UnormSrgb = 0x0000005D,
    ASTC12x12Unorm = 0x0000005E,
    ASTC12x12UnormSrgb = 0x0000005F,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsTextureFormat );

enum class GraphicsTextureSampleType
{
    Undefined = 0x00000000,
    Float = 0x00000001,
    UnfilterableFloat = 0x00000002,
    Depth = 0x00000003,
    Sint = 0x00000004,
    Uint = 0x00000005,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsTextureSampleType );

enum class GraphicsTextureViewDimension
{
    TextureViewDimension_Undefined = 0x00000000,
    TextureViewDimension_1D = 0x00000001,
    TextureViewDimension_2D = 0x00000002,
    TextureViewDimension_2DArray = 0x00000003,
    TextureViewDimension_Cube = 0x00000004,
    TextureViewDimension_CubeArray = 0x00000005,
    TextureViewDimension_3D = 0x00000006,
    TextureViewDimension_Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsTextureViewDimension );

enum class GraphicsVertexFormat
{
    Undefined = 0x00000000,
    Uint8x2 = 0x00000001,
    Uint8x4 = 0x00000002,
    Sint8x2 = 0x00000003,
    Sint8x4 = 0x00000004,
    Unorm8x2 = 0x00000005,
    Unorm8x4 = 0x00000006,
    Snorm8x2 = 0x00000007,
    Snorm8x4 = 0x00000008,
    Uint16x2 = 0x00000009,
    Uint16x4 = 0x0000000A,
    Sint16x2 = 0x0000000B,
    Sint16x4 = 0x0000000C,
    Unorm16x2 = 0x0000000D,
    Unorm16x4 = 0x0000000E,
    Snorm16x2 = 0x0000000F,
    Snorm16x4 = 0x00000010,
    Float16x2 = 0x00000011,
    Float16x4 = 0x00000012,
    Float32 = 0x00000013,
    Float32x2 = 0x00000014,
    Float32x3 = 0x00000015,
    Float32x4 = 0x00000016,
    Uint32 = 0x00000017,
    Uint32x2 = 0x00000018,
    Uint32x3 = 0x00000019,
    Uint32x4 = 0x0000001A,
    Sint32 = 0x0000001B,
    Sint32x2 = 0x0000001C,
    Sint32x3 = 0x0000001D,
    Sint32x4 = 0x0000001E,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsVertexFormat );

enum class GraphicsVertexStepMode
{
    Vertex = 0x00000000,
    Instance = 0x00000001,
    Force32 = 0x7FFFFFFF
};
DECL_XE_ENUM( GraphicsVertexStepMode );

enum class GraphicsBufferUsage
{
    None = 0x00000000,
    MapRead = 0x00000001,
    MapWrite = 0x00000002,
    CopySrc = 0x00000004,
    CopyDst = 0x00000008,
    Index = 0x00000010,
    Vertex = 0x00000020,
    Uniform = 0x00000040,
    Storage = 0x00000080,
    Indirect = 0x00000100,
    QueryResolve = 0x00000200,
    Force32 = 0x7FFFFFFF
};
DECL_FLAGS( GraphicsBufferUsage ); DECL_XE_ENUM( GraphicsBufferUsage );

enum class GraphicsColorWriteMask
{
    None = 0x00000000,
    Red = 0x00000001,
    Green = 0x00000002,
    Blue = 0x00000004,
    Alpha = 0x00000008,
    All = 0x0000000F,
    Force32 = 0x7FFFFFFF
};
DECL_FLAGS( GraphicsColorWriteMask ); DECL_XE_ENUM( GraphicsColorWriteMask );

enum class GraphicsMapMode
{
    None = 0x00000000,
    Read = 0x00000001,
    Write = 0x00000002,
    Force32 = 0x7FFFFFFF
};
DECL_FLAGS( GraphicsMapMode ); DECL_XE_ENUM( GraphicsMapMode );

enum class GraphicsShaderStage
{
    None = 0x00000000,
    Vertex = 0x00000001,
    Fragment = 0x00000002,
    Compute = 0x00000004,
    Force32 = 0x7FFFFFFF
};
DECL_FLAGS( GraphicsShaderStage ); DECL_XE_ENUM( GraphicsShaderStage );

enum class GraphicsTextureUsage
{
    None = 0x00000000,
    CopySrc = 0x00000001,
    CopyDst = 0x00000002,
    TextureBinding = 0x00000004,
    StorageBinding = 0x00000008,
    RenderAttachment = 0x00000010,
    Force32 = 0x7FFFFFFF
};
DECL_FLAGS( GraphicsTextureUsage ); DECL_XE_ENUM( GraphicsTextureUsage );


struct XE_API GraphicsAdapterProperties
{
    XE::uint32 vendorID;
    XE::uint32 deviceID;
    XE::String name;
    XE::String driverDescription;
    XE::GraphicsAdapterType adapterType;
    XE::GraphicsBackendType backendType;
};

struct XE_API GraphicsBindGroupEntry
{
    XE::uint32 binding;
    XE::GraphicsBufferHandle buffer;
    XE::uint64 offset;
    XE::uint64 size;
    XE::GraphicsSamplerHandle sampler;
    XE::GraphicsTextureViewHandle textureView;
};

struct XE_API GraphicsBlendComponent
{
    XE::GraphicsBlendOperation operation;
    XE::GraphicsBlendFactor srcFactor;
    XE::GraphicsBlendFactor dstFactor;
};

struct XE_API GraphicsBufferBindingLayout
{
    XE::GraphicsBufferBindingType type;
    bool hasDynamicOffset;
    XE::uint64 minBindingSize;
};

struct XE_API GraphicsBufferDescriptor
{
    XE::String label;
    XE::GraphicsBufferUsageFlags usage;
    XE::uint64 size;
    bool mappedAtCreation;
};

struct XE_API GraphicsCommandBufferDescriptor
{
    XE::String label;
};

struct XE_API GraphicsCommandEncoderDescriptor
{
    XE::String label;
};

struct XE_API GraphicsCompilationMessage
{
    XE::String message;
    XE::GraphicsCompilationMessageType type;
    XE::uint64 lineNum;
    XE::uint64 linePos;
    XE::uint64 offset;
    XE::uint64 length;
};

struct XE_API GraphicsComputePassTimestampWrite
{
    XE::GraphicsQuerySetHandle querySet;
    XE::uint32 queryIndex;
    XE::GraphicsComputePassTimestampLocation location;
};

struct XE_API GraphicsConstantEntry
{
    XE::String key;
    double value;
};

struct XE_API GraphicsExtent3D
{
    XE::uint32 width;
    XE::uint32 height;
    XE::uint32 depthOrArrayLayers;
};

struct XE_API GraphicsLimits
{
    XE::uint32 maxTextureDimension1D;
    XE::uint32 maxTextureDimension2D;
    XE::uint32 maxTextureDimension3D;
    XE::uint32 maxTextureArrayLayers;
    XE::uint32 maxBindGroups;
    XE::uint32 maxDynamicUniformBuffersPerPipelineLayout;
    XE::uint32 maxDynamicStorageBuffersPerPipelineLayout;
    XE::uint32 maxSampledTexturesPerShaderStage;
    XE::uint32 maxSamplersPerShaderStage;
    XE::uint32 maxStorageBuffersPerShaderStage;
    XE::uint32 maxStorageTexturesPerShaderStage;
    XE::uint32 maxUniformBuffersPerShaderStage;
    XE::uint64 maxUniformBufferBindingSize;
    XE::uint64 maxStorageBufferBindingSize;
    XE::uint32 minUniformBufferOffsetAlignment;
    XE::uint32 minStorageBufferOffsetAlignment;
    XE::uint32 maxVertexBuffers;
    XE::uint32 maxVertexAttributes;
    XE::uint32 maxVertexBufferArrayStride;
    XE::uint32 maxInterStageShaderComponents;
    XE::uint32 maxComputeWorkgroupStorageSize;
    XE::uint32 maxComputeInvocationsPerWorkgroup;
    XE::uint32 maxComputeWorkgroupSizeX;
    XE::uint32 maxComputeWorkgroupSizeY;
    XE::uint32 maxComputeWorkgroupSizeZ;
    XE::uint32 maxComputeWorkgroupsPerDimension;
};

struct XE_API GraphicsMultisampleState
{
    XE::uint32 count;
    XE::uint32 mask;
    bool alphaToCoverageEnabled;
};

struct XE_API GraphicsOrigin3D
{
    XE::uint32 x;
    XE::uint32 y;
    XE::uint32 z;
};

struct XE_API GraphicsPipelineLayoutDescriptor
{
    XE::String label;
    XE::uint32 bindGroupLayoutCount;
    XE::GraphicsBindGroupLayout const * bindGroupLayouts;
};

struct XE_API GraphicsPrimitiveDepthClipControl
{
    bool unclippedDepth;
};

struct XE_API GraphicsPrimitiveState
{
    XE::GraphicsPrimitiveTopology topology;
    XE::GraphicsIndexFormat stripIndexFormat;
    XE::GraphicsFrontFace frontFace;
    XE::GraphicsCullMode cullMode;
};

struct XE_API GraphicsQuerySetDescriptor
{
    XE::String label;
    XE::GraphicsQueryType type;
    XE::uint32 count;
    XE::GraphicsPipelineStatisticName const * pipelineStatistics;
    XE::uint32 pipelineStatisticsCount;
};

struct XE_API GraphicsQueueDescriptor
{
    XE::String label;
};

struct XE_API GraphicsRenderBundleDescriptor
{
    XE::String label;
};

struct XE_API GraphicsRenderBundleEncoderDescriptor
{
    XE::String label;
    XE::uint32 colorFormatsCount;
    XE::GraphicsTextureFormat const * colorFormats;
    XE::GraphicsTextureFormat depthStencilFormat;
    XE::uint32 sampleCount;
    bool depthReadOnly;
    bool stencilReadOnly;
};

struct XE_API GraphicsRenderPassDepthStencilAttachment
{
    XE::GraphicsTextureViewHandle view;
    XE::GraphicsLoadOp depthLoadOp;
    XE::GraphicsStoreOp depthStoreOp;
    float depthClearValue;
    bool depthReadOnly;
    XE::GraphicsLoadOp stencilLoadOp;
    XE::GraphicsStoreOp stencilStoreOp;
    XE::uint32 stencilClearValue;
    bool stencilReadOnly;
};

struct XE_API GraphicsRenderPassTimestampWrite
{
    XE::GraphicsQuerySetHandle querySet;
    XE::uint32 queryIndex;
    XE::GraphicsRenderPassTimestampLocation location;
};

struct XE_API GraphicsRequestAdapterOptions
{
    XE::GraphicsSurfaceHandle compatibleSurface;
    XE::GraphicsPowerPreference powerPreference;
    bool forceFallbackAdapter;
};

struct XE_API GraphicsSamplerBindingLayout
{
    XE::GraphicsSamplerBindingType type;
};

struct XE_API GraphicsSamplerDescriptor
{
    XE::String label;
    XE::GraphicsAddressMode addressModeU;
    XE::GraphicsAddressMode addressModeV;
    XE::GraphicsAddressMode addressModeW;
    XE::GraphicsFilterMode magFilter;
    XE::GraphicsFilterMode minFilter;
    XE::GraphicsMipmapFilterMode mipmapFilter;
    float lodMinClamp;
    float lodMaxClamp;
    XE::GraphicsCompareFunction compare;
    XE::uint16 maxAnisotropy;
};

struct XE_API GraphicsShaderModuleCompilationHint
{
    XE::String entryPoint;
    XE::GraphicsPipelineLayoutHandle layout;
};

struct XE_API GraphicsShaderModuleCodeDescriptor
{
    XE::MemoryView code;
};

struct XE_API GraphicsStencilFaceState
{
    XE::GraphicsCompareFunction compare;
    XE::GraphicsStencilOperation failOp;
    XE::GraphicsStencilOperation depthFailOp;
    XE::GraphicsStencilOperation passOp;
};

struct XE_API GraphicsStorageTextureBindingLayout
{
    XE::GraphicsStorageTextureAccess access;
    XE::GraphicsTextureFormat format;
    XE::GraphicsTextureViewDimension viewDimension;
};

struct XE_API GraphicsSurfaceDescriptor
{
    XE::String label;
};

struct XE_API GraphicsSurfaceDescriptorFromAndroidNativeWindow
{
    void * window;
};

struct XE_API GraphicsSurfaceDescriptorFromCanvasHTMLSelector
{
    XE::String selector;
};

struct XE_API GraphicsSurfaceDescriptorFromMetalLayer
{
    void * layer;
};

struct XE_API GraphicsSurfaceDescriptorFromWaylandSurface
{
    void * display;
    void * surface;
};

struct XE_API GraphicsSurfaceDescriptorFromWindowsHWND
{
    void * hinstance;
    void * hwnd;
};

struct XE_API GraphicsSurfaceDescriptorFromXcbWindow
{
    void * connection;
    XE::uint32 window;
};

struct XE_API GraphicsSurfaceDescriptorFromXlibWindow
{
    void * display;
    XE::uint32 window;
};

struct XE_API GraphicsSwapChainDescriptor
{
    XE::String label;
    XE::GraphicsTextureUsageFlags usage;
    XE::GraphicsTextureFormat format;
    XE::uint32 width;
    XE::uint32 height;
    XE::GraphicsPresentMode presentMode;
};

struct XE_API GraphicsTextureBindingLayout
{
    XE::GraphicsTextureSampleType sampleType;
    XE::GraphicsTextureViewDimension viewDimension;
    bool multisampled;
};

struct XE_API GraphicsTextureDataLayout
{
    XE::uint64 offset;
    XE::uint32 bytesPerRow;
    XE::uint32 rowsPerImage;
};

struct XE_API GraphicsTextureViewDescriptor
{
    XE::String label;
    XE::GraphicsTextureFormat format;
    XE::GraphicsTextureViewDimension dimension;
    XE::uint32 baseMipLevel;
    XE::uint32 mipLevelCount;
    XE::uint32 baseArrayLayer;
    XE::uint32 arrayLayerCount;
    XE::GraphicsTextureAspect aspect;
};

struct XE_API GraphicsVertexAttribute
{
    XE::GraphicsVertexFormat format;
    XE::uint64 offset;
    XE::uint32 shaderLocation;
};

struct XE_API GraphicsBindGroupDescriptor
{
    XE::String label;
    XE::GraphicsBindGroupLayoutHandle layout;
    XE::uint32 entryCount;
    XE::GraphicsBindGroupEntry const * entries;
};

struct XE_API GraphicsBindGroupLayoutEntry
{
    XE::uint32 binding;
    XE::GraphicsShaderStageFlags visibility;
    XE::GraphicsBufferBindingLayout buffer;
    XE::GraphicsSamplerBindingLayout sampler;
    XE::GraphicsTextureBindingLayout texture;
    XE::GraphicsStorageTextureBindingLayout storageTexture;
};

struct XE_API GraphicsBlendState
{
    XE::GraphicsBlendComponent color;
    XE::GraphicsBlendComponent alpha;
};

struct XE_API GraphicsCompilationInfo
{
    XE::uint32 messageCount;
    XE::GraphicsCompilationMessage const * messages;
};

struct XE_API GraphicsComputePassDescriptor
{
    XE::String label;
    XE::uint32 timestampWriteCount;
    XE::GraphicsComputePassTimestampWrite const * timestampWrites;
};

struct XE_API GraphicsDepthStencilState
{
    XE::GraphicsTextureFormat format;
    bool depthWriteEnabled;
    XE::GraphicsCompareFunction depthCompare;
    XE::GraphicsStencilFaceState stencilFront;
    XE::GraphicsStencilFaceState stencilBack;
    XE::uint32 stencilReadMask;
    XE::uint32 stencilWriteMask;
    int32_t depthBias;
    float depthBiasSlopeScale;
    float depthBiasClamp;
};

struct XE_API GraphicsImageCopyBuffer
{
    XE::GraphicsTextureDataLayout layout;
    XE::GraphicsBufferHandle buffer;
};

struct XE_API GraphicsImageCopyTexture
{
    XE::GraphicsTextureHandle texture;
    XE::uint32 mipLevel;
    XE::GraphicsOrigin3D origin;
    XE::GraphicsTextureAspect aspect;
};

struct XE_API GraphicsProgrammableStageDescriptor
{
    XE::GraphicsShaderModuleHandle module;
    XE::String entryPoint;
    XE::uint32 constantCount;
    XE::GraphicsConstantEntry const * constants;
};

struct XE_API GraphicsRenderPassColorAttachment
{
    XE::GraphicsTextureViewHandle view;
    XE::GraphicsTextureViewHandle resolveTarget;
    XE::GraphicsLoadOp loadOp;
    XE::GraphicsStoreOp storeOp;
    XE::Color clearValue;
};

struct XE_API GraphicsRequiredLimits
{
    XE::GraphicsLimits limits;
};

struct XE_API GraphicsShaderModuleDescriptor
{
    XE::String label;
    XE::uint32 hintCount;
    XE::GraphicsShaderModuleCompilationHint const * hints;
};

struct XE_API GraphicsSupportedLimits
{
    XE::GraphicsLimits limits;
};

struct XE_API GraphicsTextureDescriptor
{
    XE::String label;
    XE::GraphicsTextureUsageFlags usage;
    XE::GraphicsTextureDimension dimension;
    XE::GraphicsExtent3D size;
    XE::GraphicsTextureFormat format;
    XE::uint32 mipLevelCount;
    XE::uint32 sampleCount;
    XE::uint32 viewFormatCount;
    XE::GraphicsTextureFormat const * viewFormats;
};

struct XE_API GraphicsVertexBufferLayout
{
    XE::uint64 arrayStride;
    XE::GraphicsVertexStepMode stepMode;
    XE::uint32 attributeCount;
    XE::GraphicsVertexAttribute const * attributes;
};

struct XE_API GraphicsBindGroupLayoutDescriptor
{
    XE::String label;
    XE::uint32 entryCount;
    XE::GraphicsBindGroupLayoutEntry const * entries;
};

struct XE_API GraphicsColorTargetState
{
    XE::GraphicsTextureFormat format;
    XE::GraphicsBlendState const * blend;
    XE::GraphicsColorWriteMaskFlags writeMask;
};

struct XE_API GraphicsComputePipelineDescriptor
{
    XE::String label;
    XE::GraphicsPipelineLayoutHandle layout;
    XE::GraphicsProgrammableStageDescriptor compute;
};

struct XE_API GraphicsDeviceDescriptor
{
    XE::String label;
    XE::uint32 requiredFeaturesCount;
    XE::GraphicsFeatureName const * requiredFeatures;
    XE::GraphicsRequiredLimits const * requiredLimits;
    XE::GraphicsQueueDescriptor defaultQueue;
};

struct XE_API GraphicsRenderPassDescriptor
{
    XE::String label;
    XE::uint32 colorAttachmentCount;
    XE::GraphicsRenderPassColorAttachment const * colorAttachments;
    XE::GraphicsRenderPassDepthStencilAttachment const * depthStencilAttachment;
    XE::GraphicsQuerySetHandle occlusionQuerySet;
    XE::uint32 timestampWriteCount;
    XE::GraphicsRenderPassTimestampWrite const * timestampWrites;
};

struct XE_API GraphicsVertexState
{
    XE::GraphicsShaderModuleHandle module;
    XE::String entryPoint;
    XE::uint32 constantCount;
    XE::GraphicsConstantEntry const * constants;
    XE::uint32 bufferCount;
    XE::GraphicsVertexBufferLayout const * buffers;
};

struct XE_API GraphicsFragmentState
{
    XE::GraphicsShaderModuleHandle module;
    XE::String entryPoint;
    XE::uint32 constantCount;
    XE::GraphicsConstantEntry const * constants;
    XE::uint32 targetCount;
    XE::GraphicsColorTargetState const * targets;
};

struct XE_API GraphicsRenderPipelineDescriptor
{
    XE::String label;
    XE::GraphicsPipelineLayoutHandle layout;
    XE::GraphicsVertexState vertex;
    XE::GraphicsPrimitiveState primitive;
    XE::GraphicsDepthStencilState const * depthStencil;
    XE::GraphicsMultisampleState multisample;
    XE::GraphicsFragmentState const * fragment;
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