#include "GraphicsServiceWebGpu.h"

#if GRAPHICS_API & GRAPHICS_WEBGPU

#include <webgpu/webgpu.h>

namespace
{
	DECL_PTR( WGPUGraphicsSurface );
	DECL_PTR( WGPUGraphicsSwapChain );
	DECL_PTR( WGPUGraphicsAdapter );
	DECL_PTR( WGPUGraphicsDevice );
	DECL_PTR( WGPUGraphicsQueue );
	DECL_PTR( WGPUGraphicsBindGroup );
	DECL_PTR( WGPUGraphicsBindGroupLayout );
	DECL_PTR( WGPUGraphicsBuffer );
	DECL_PTR( WGPUGraphicsCommandBuffer );
	DECL_PTR( WGPUGraphicsCommandEncoder );
	DECL_PTR( WGPUGraphicsComputePassEncoder );
	DECL_PTR( WGPUGraphicsComputePipeline );
	DECL_PTR( WGPUGraphicsPipelineLayout );
	DECL_PTR( WGPUGraphicsQuerySet );
	DECL_PTR( WGPUGraphicsRenderBundle );
	DECL_PTR( WGPUGraphicsRenderBundleEncoder );
	DECL_PTR( WGPUGraphicsRenderPassEncoder );
	DECL_PTR( WGPUGraphicsRenderPipeline );
	DECL_PTR( WGPUGraphicsSampler );
	DECL_PTR( WGPUGraphicsShaderModule );
	DECL_PTR( WGPUGraphicsTexture );
	DECL_PTR( WGPUGraphicsTextureView );

	class WGPUGraphicsSurface : public XE::GraphicsSurface
	{
	public:
		WGPUSurface Raw;
	};
	class WGPUGraphicsSwapChain : public XE::GraphicsSwapChain
	{
	public:
		WGPUSwapChain Raw;
	};
	class WGPUGraphicsAdapter : public XE::GraphicsAdapter
	{
	public:
		WGPUAdapter Raw;
	};
	class WGPUGraphicsDevice : public XE::GraphicsDevice
	{
	public:
		WGPUDevice Raw;
	};
	class WGPUGraphicsQueue : public XE::GraphicsQueue
	{
	public:
		WGPUQueue Raw;
	};
	class WGPUGraphicsBindGroup : public XE::GraphicsBindGroup
	{
	public:
		WGPUBindGroup Raw;
	};
	class WGPUGraphicsBindGroupLayout : public XE::GraphicsBindGroupLayout
	{
	public:
		WGPUBindGroupLayout Raw;
	};
	class WGPUGraphicsBuffer : public XE::GraphicsBuffer
	{
	public:
		WGPUBuffer Raw;
	};
	class WGPUGraphicsCommandBuffer : public XE::GraphicsCommandBuffer
	{
	public:
		WGPUCommandBuffer Raw;
	};
	class WGPUGraphicsCommandEncoder : public XE::GraphicsCommandEncoder
	{
	public:
		WGPUCommandEncoder Raw;
	};
	class WGPUGraphicsComputePassEncoder : public XE::GraphicsComputePassEncoder
	{
	public:
		WGPUComputePassEncoder Raw;
	};
	class WGPUGraphicsComputePipeline : public XE::GraphicsComputePipeline
	{
	public:
		WGPUComputePipeline Raw;
	};
	class WGPUGraphicsPipelineLayout : public XE::GraphicsPipelineLayout
	{
	public:
		WGPUPipelineLayout Raw;
	};
	class WGPUGraphicsQuerySet : public XE::GraphicsQuerySet
	{
	public:
		WGPUQuerySet Raw;
	};
	class WGPUGraphicsRenderBundle : public XE::GraphicsRenderBundle
	{
	public:
		WGPURenderBundle Raw;
	};
	class WGPUGraphicsRenderBundleEncoder : public XE::GraphicsRenderBundleEncoder
	{
	public:
		WGPURenderBundleEncoder Raw;
	};
	class WGPUGraphicsRenderPassEncoder : public XE::GraphicsRenderPassEncoder
	{
	public:
		WGPURenderPassEncoder Raw;
	};
	class WGPUGraphicsRenderPipeline : public XE::GraphicsRenderPipeline
	{
	public:
		WGPURenderPipeline Raw;
	};
	class WGPUGraphicsSampler : public XE::GraphicsSampler
	{
	public:
		WGPUSampler Raw;
	};
	class WGPUGraphicsShaderModule : public XE::GraphicsShaderModule
	{
	public:
		WGPUShaderModule Raw;
	};
	class WGPUGraphicsTexture : public XE::GraphicsTexture
	{
	public:
		WGPUTexture Raw;
	};
	class WGPUGraphicsTextureView : public XE::GraphicsTextureView
	{
	public:
		WGPUTextureView Raw;
	};

#define CAST( NAME ) \
	WGPU##NAME * Cast( const XE::##NAME##Ptr & val ) \
	{ \
		return static_cast<WGPU##NAME *>( val.get() ); \
	}

	CAST( GraphicsSurface );
	CAST( GraphicsSwapChain );
	CAST( GraphicsAdapter );
	CAST( GraphicsDevice );
	CAST( GraphicsQueue );
	CAST( GraphicsBindGroup );
	CAST( GraphicsBindGroupLayout );
	CAST( GraphicsBuffer );
	CAST( GraphicsCommandBuffer );
	CAST( GraphicsCommandEncoder );
	CAST( GraphicsComputePassEncoder );
	CAST( GraphicsComputePipeline );
	CAST( GraphicsPipelineLayout );
	CAST( GraphicsQuerySet );
	CAST( GraphicsRenderBundle );
	CAST( GraphicsRenderBundleEncoder );
	CAST( GraphicsRenderPassEncoder );
	CAST( GraphicsRenderPipeline );
	CAST( GraphicsSampler );
	CAST( GraphicsShaderModule );
	CAST( GraphicsTexture );
	CAST( GraphicsTextureView );

#undef CAST

}

struct XE::GraphicsServiceWebGpu::Private
{
	WGPUInstance _Instance = nullptr;
};

XE::GraphicsServiceWebGpu::GraphicsServiceWebGpu( bool debug /*= false*/ )
	:_p( XE::New< Private >() )
{

}

XE::GraphicsServiceWebGpu::~GraphicsServiceWebGpu()
{
	XE::Delete( _p );
}

void XE::GraphicsServiceWebGpu::Prepare()
{
	WGPUInstanceDescriptor desc = {};

	_p->_Instance = wgpuCreateInstance( &desc );
}

void XE::GraphicsServiceWebGpu::Startup()
{

}

void XE::GraphicsServiceWebGpu::Update()
{

}

void XE::GraphicsServiceWebGpu::Clearup()
{
	_p->_Instance = nullptr;
}

XE::GraphicsSurfacePtr XE::GraphicsServiceWebGpu::CreateSurface( const XE::GraphicsSurfaceDescriptor & descriptor )
{
	auto surface = XE::MakeShared< WGPUGraphicsSurface >();
	{
		WGPUSurfaceDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();

			desc.nextInChain;
		}

		surface->Raw = wgpuInstanceCreateSurface( _p->_Instance, &desc );
	}
	return surface;
}

void XE::GraphicsServiceWebGpu::RequestAdapter( const XE::GraphicsRequestAdapterOptions & options, XE::GraphicsService::RequestAdapterCallback callback )
{
	WGPURequestAdapterOptions option = {};
	{
		option.compatibleSurface = Cast( options.CompatibleSurface )->Raw;
		option.forceFallbackAdapter = options.ForceFallbackAdapter;
		option.powerPreference = (WGPUPowerPreference)options.PowerPreference;
	}

	wgpuInstanceRequestAdapter( _p->_Instance, &option, []( WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * userdata )
	{
		XE::GraphicsService::RequestAdapterCallback * callback = (XE::GraphicsService::RequestAdapterCallback *)userdata;

		auto ada = XE::MakeShared< WGPUGraphicsAdapter >();
		ada->Raw = adapter;

		( *callback )( XE::GraphicsRequestAdapterStatus( status ), ada );

	}, &callback );
}

void XE::GraphicsServiceWebGpu::AdapterEnumerateFeatures( XE::GraphicsAdapterPtr adapter, XE::Array< XE::GraphicsFeatureName > & features )
{
	WGPUFeatureName names[10];
	auto sz = wgpuAdapterEnumerateFeatures( Cast( adapter )->Raw, names );
	for ( size_t i = 0; i < sz; i++ )
	{
		features.push_back( (XE::GraphicsFeatureName)names[i] );
	}
}

bool XE::GraphicsServiceWebGpu::AdapterGetLimits( XE::GraphicsAdapterPtr adapter, XE::GraphicsSupportedLimits & limits )
{
	WGPUSupportedLimits limit;
	
	if ( wgpuAdapterGetLimits( Cast( adapter )->Raw, &limit ) )
	{
		limits.Limits.MaxBindGroups = limit.limits.maxBindGroups;
		limits.Limits.MaxComputeInvocationsPerWorkgroup = limit.limits.maxBindGroups;
		limits.Limits.MaxComputeWorkgroupSizeX = limit.limits.maxComputeWorkgroupSizeX;
		limits.Limits.MaxComputeWorkgroupSizeY = limit.limits.maxComputeWorkgroupSizeY;
		limits.Limits.MaxComputeWorkgroupSizeZ = limit.limits.maxComputeWorkgroupSizeZ;
		limits.Limits.MaxComputeWorkgroupsPerDimension = limit.limits.maxComputeWorkgroupsPerDimension;
		limits.Limits.MaxComputeWorkgroupStorageSize = limit.limits.maxComputeWorkgroupStorageSize;
		limits.Limits.MaxDynamicStorageBuffersPerPipelineLayout = limit.limits.maxDynamicStorageBuffersPerPipelineLayout;
		limits.Limits.MaxDynamicUniformBuffersPerPipelineLayout = limit.limits.maxDynamicUniformBuffersPerPipelineLayout;
		limits.Limits.MaxInterStageShaderComponents = limit.limits.maxInterStageShaderComponents;
		limits.Limits.MaxSampledTexturesPerShaderStage = limit.limits.maxSampledTexturesPerShaderStage;
		limits.Limits.MaxSamplersPerShaderStage = limit.limits.maxSamplersPerShaderStage;
		limits.Limits.MaxStorageBufferBindingSize = limit.limits.maxStorageBufferBindingSize;
		limits.Limits.MaxStorageBuffersPerShaderStage = limit.limits.maxStorageBuffersPerShaderStage;
		limits.Limits.MaxStorageTexturesPerShaderStage = limit.limits.maxStorageTexturesPerShaderStage;
		limits.Limits.MaxTextureArrayLayers = limit.limits.maxTextureArrayLayers;
		limits.Limits.MaxTextureDimension1D = limit.limits.maxTextureDimension1D;
		limits.Limits.MaxTextureDimension2D = limit.limits.maxTextureDimension2D;
		limits.Limits.MaxTextureDimension3D = limit.limits.maxTextureDimension3D;
		limits.Limits.MaxUniformBufferBindingSize = limit.limits.maxUniformBufferBindingSize;
		limits.Limits.MaxUniformBuffersPerShaderStage = limit.limits.maxUniformBuffersPerShaderStage;
		limits.Limits.MaxVertexAttributes = limit.limits.maxVertexAttributes;
		limits.Limits.MaxVertexBufferArrayStride = limit.limits.maxVertexBufferArrayStride;
		limits.Limits.MaxVertexBuffers = limit.limits.maxVertexBuffers;
		limits.Limits.MinStorageBufferOffsetAlignment = limit.limits.minStorageBufferOffsetAlignment;
		limits.Limits.MinUniformBufferOffsetAlignment = limit.limits.minUniformBufferOffsetAlignment;

		return true;
	}

	return false;
}

void XE::GraphicsServiceWebGpu::AdapterGetProperties( XE::GraphicsAdapterPtr adapter, XE::GraphicsAdapterProperties & properties )
{
	WGPUAdapterProperties prop = {};

	wgpuAdapterGetProperties( Cast( adapter )->Raw, &prop );

	properties.AdapterType = (XE::GraphicsAdapterType)prop.adapterType;
	properties.BackendType = (XE::GraphicsBackendType)prop.backendType;
	properties.DeviceID = prop.deviceID;
	properties.DriverDescription = prop.driverDescription;
	properties.Name = prop.name;
	properties.VendorID = prop.vendorID;
}

bool XE::GraphicsServiceWebGpu::AdapterHasFeature( XE::GraphicsAdapterPtr adapter, XE::GraphicsFeatureName feature )
{
	return wgpuAdapterHasFeature( Cast( adapter )->Raw, (WGPUFeatureName)feature );
}

void XE::GraphicsServiceWebGpu::AdapterRequestDevice( XE::GraphicsAdapterPtr adapter, const XE::GraphicsDeviceDescriptor & descriptor, XE::GraphicsService::RequestDeviceCallback callback )
{
	WGPURequiredLimits limits = {};
	{
		limits.limits.maxBindGroups = descriptor.RequiredLimits.Limits.MaxBindGroups;
		limits.limits.maxComputeInvocationsPerWorkgroup = descriptor.RequiredLimits.Limits.MaxBindGroups;
		limits.limits.maxComputeWorkgroupSizeX = descriptor.RequiredLimits.Limits.MaxComputeWorkgroupSizeX;
		limits.limits.maxComputeWorkgroupSizeY = descriptor.RequiredLimits.Limits.MaxComputeWorkgroupSizeY;
		limits.limits.maxComputeWorkgroupSizeZ = descriptor.RequiredLimits.Limits.MaxComputeWorkgroupSizeZ;
		limits.limits.maxComputeWorkgroupsPerDimension = descriptor.RequiredLimits.Limits.MaxComputeWorkgroupsPerDimension;
		limits.limits.maxComputeWorkgroupStorageSize = descriptor.RequiredLimits.Limits.MaxComputeWorkgroupStorageSize;
		limits.limits.maxDynamicStorageBuffersPerPipelineLayout = descriptor.RequiredLimits.Limits.MaxDynamicStorageBuffersPerPipelineLayout;
		limits.limits.maxDynamicUniformBuffersPerPipelineLayout = descriptor.RequiredLimits.Limits.MaxDynamicUniformBuffersPerPipelineLayout;
		limits.limits.maxInterStageShaderComponents = descriptor.RequiredLimits.Limits.MaxInterStageShaderComponents;
		limits.limits.maxSampledTexturesPerShaderStage = descriptor.RequiredLimits.Limits.MaxSampledTexturesPerShaderStage;
		limits.limits.maxSamplersPerShaderStage = descriptor.RequiredLimits.Limits.MaxSamplersPerShaderStage;
		limits.limits.maxStorageBufferBindingSize = descriptor.RequiredLimits.Limits.MaxStorageBufferBindingSize;
		limits.limits.maxStorageBuffersPerShaderStage = descriptor.RequiredLimits.Limits.MaxStorageBuffersPerShaderStage;
		limits.limits.maxStorageTexturesPerShaderStage = descriptor.RequiredLimits.Limits.MaxStorageTexturesPerShaderStage;
		limits.limits.maxTextureArrayLayers = descriptor.RequiredLimits.Limits.MaxTextureArrayLayers;
		limits.limits.maxTextureDimension1D = descriptor.RequiredLimits.Limits.MaxTextureDimension1D;
		limits.limits.maxTextureDimension2D = descriptor.RequiredLimits.Limits.MaxTextureDimension2D;
		limits.limits.maxTextureDimension3D = descriptor.RequiredLimits.Limits.MaxTextureDimension3D;
		limits.limits.maxUniformBufferBindingSize = descriptor.RequiredLimits.Limits.MaxUniformBufferBindingSize;
		limits.limits.maxUniformBuffersPerShaderStage = descriptor.RequiredLimits.Limits.MaxUniformBuffersPerShaderStage;
		limits.limits.maxVertexAttributes = descriptor.RequiredLimits.Limits.MaxVertexAttributes;
		limits.limits.maxVertexBufferArrayStride = descriptor.RequiredLimits.Limits.MaxVertexBufferArrayStride;
		limits.limits.maxVertexBuffers = descriptor.RequiredLimits.Limits.MaxVertexBuffers;
		limits.limits.minStorageBufferOffsetAlignment = descriptor.RequiredLimits.Limits.MinStorageBufferOffsetAlignment;
		limits.limits.minUniformBufferOffsetAlignment = descriptor.RequiredLimits.Limits.MinUniformBufferOffsetAlignment;
	}
	WGPUDeviceDescriptor desc = {};
	{
		desc.defaultQueue.label = descriptor.DefaultQueue.Label.c_str();
		desc.label = descriptor.Label.c_str();
		desc.requiredLimits = &limits;
		desc.requiredFeatures = (WGPUFeatureName *)descriptor.RequiredFeatures.data();
		desc.requiredFeaturesCount = descriptor.RequiredFeatures.size();
	}

	wgpuAdapterRequestDevice( Cast( adapter )->Raw, &desc, []( WGPURequestDeviceStatus status, WGPUDevice device, char const * message, void * userdata )
	{
		XE::GraphicsService::RequestDeviceCallback * callback = (XE::GraphicsService::RequestDeviceCallback *)( userdata );

		auto dev = XE::MakeShared< WGPUGraphicsDevice >();
		dev->Raw = device;

		( *callback )( (XE::GraphicsRequestDeviceStatus)status, dev );

	}, &callback );
}

XE::GraphicsBindGroupPtr XE::GraphicsServiceWebGpu::DeviceCreateBindGroup( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupDescriptor & descriptor )
{
	auto bind_group = XE::MakeShared< WGPUGraphicsBindGroup >();
	{
		XE::Array< WGPUBindGroupEntry > entries( XE::MemoryResource::GetFrameMemoryResource() ); entries.resize( descriptor.Entries.size() );
		for ( size_t i = 0; i < descriptor.Entries.size(); i++ )
		{
			entries[i].binding = descriptor.Entries[i].Binding;
			entries[i].buffer = Cast( descriptor.Entries[i].Buffer )->Raw;
			entries[i].offset = descriptor.Entries[i].Offset;
			entries[i].sampler = Cast( descriptor.Entries[i].Sampler )->Raw;
			entries[i].size = descriptor.Entries[i].Size;
			entries[i].textureView = Cast( descriptor.Entries[i].TextureView )->Raw;
		}

		WGPUBindGroupDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.layout = Cast( descriptor.Layout )->Raw;
			desc.entries = entries.data();
			desc.entryCount = entries.size();
		}
		bind_group->Raw = wgpuDeviceCreateBindGroup( Cast( device )->Raw, &desc );
	}
	return bind_group;
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceWebGpu::DeviceCreateBindGroupLayout( XE::GraphicsDevicePtr device, const XE::GraphicsBindGroupLayoutDescriptor & descriptor )
{
	auto layout = XE::MakeShared< WGPUGraphicsBindGroupLayout >();
	{
		XE::Array< WGPUBindGroupLayoutEntry > entries( XE::MemoryResource::GetFrameMemoryResource() ); entries.resize( descriptor.Entries.size() );
		for ( size_t i = 0; i < descriptor.Entries.size(); i++ )
		{
			entries[i].binding = descriptor.Entries[i].Binding;
			entries[i].buffer.hasDynamicOffset = descriptor.Entries[i].Buffer.HasDynamicOffset;
			entries[i].buffer.minBindingSize = descriptor.Entries[i].Buffer.MinBindingSize;
			entries[i].buffer.type = (WGPUBufferBindingType)descriptor.Entries[i].Buffer.Type;
			entries[i].sampler.type = (WGPUSamplerBindingType)descriptor.Entries[i].Sampler.Type;
			entries[i].storageTexture.access = (WGPUStorageTextureAccess)descriptor.Entries[i].StorageTexture.Access;
			entries[i].storageTexture.format = (WGPUTextureFormat)descriptor.Entries[i].StorageTexture.Format;
			entries[i].storageTexture.viewDimension = (WGPUTextureViewDimension)descriptor.Entries[i].StorageTexture.ViewDimension;
			entries[i].texture.multisampled = descriptor.Entries[i].Texture.Multisampled;
			entries[i].texture.sampleType = (WGPUTextureSampleType)descriptor.Entries[i].Texture.SampleType;
			entries[i].texture.viewDimension = (WGPUTextureViewDimension)descriptor.Entries[i].Texture.ViewDimension;
			entries[i].visibility = descriptor.Entries[i].Visibility.GetValue();
		}

		WGPUBindGroupLayoutDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.entries = entries.data();
			desc.entryCount = entries.size();
		}
		layout->Raw = wgpuDeviceCreateBindGroupLayout( Cast( device )->Raw, &desc );
	}
	return layout;
}

XE::GraphicsBufferPtr XE::GraphicsServiceWebGpu::DeviceCreateBuffer( XE::GraphicsDevicePtr device, const XE::GraphicsBufferDescriptor & descriptor )
{
	auto buffer = XE::MakeShared< WGPUGraphicsBuffer >();
	{
		WGPUBufferDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.mappedAtCreation = descriptor.MappedAtCreation;
			desc.size = descriptor.Size;
			desc.usage = descriptor.Usage.GetValue();
		}
		buffer->Raw = wgpuDeviceCreateBuffer( Cast( device )->Raw, &desc );
	}
	return buffer;
}

XE::GraphicsCommandEncoderPtr XE::GraphicsServiceWebGpu::DeviceCreateCommandEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsCommandEncoderDescriptor & descriptor )
{
	auto encoder = XE::MakeShared< WGPUGraphicsCommandEncoder >();
	{
		WGPUCommandEncoderDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
		}
		encoder->Raw = wgpuDeviceCreateCommandEncoder( Cast( device )->Raw, &desc );
	}
	return encoder;
}

XE::GraphicsComputePipelinePtr XE::GraphicsServiceWebGpu::DeviceCreateComputePipeline( XE::GraphicsDevicePtr device, const XE::GraphicsComputePipelineDescriptor & descriptor )
{
	auto pipeline = XE::MakeShared< WGPUGraphicsComputePipeline >();
	{
		XE::Array< WGPUConstantEntry > entries( XE::MemoryResource::GetFrameMemoryResource() ); entries.resize( descriptor.Compute.Constants.size() );
		for ( size_t i = 0; i < descriptor.Compute.Constants.size(); i++ )
		{
			entries[i].key = descriptor.Compute.Constants[i].Key.c_str();
			entries[i].value = descriptor.Compute.Constants[i].Value;
		}
		WGPUComputePipelineDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.layout = Cast( descriptor.Layout )->Raw;
			desc.compute.module = Cast( descriptor.Compute.Shader )->Raw;
			desc.compute.entryPoint = descriptor.Compute.EntryPoint.c_str();
			desc.compute.constants = entries.data();
			desc.compute.constantCount = entries.size();
		}
		pipeline->Raw = wgpuDeviceCreateComputePipeline( Cast( device )->Raw, &desc );
	}
	return pipeline;
}

XE::GraphicsPipelineLayoutPtr XE::GraphicsServiceWebGpu::DeviceCreatePipelineLayout( XE::GraphicsDevicePtr device, const XE::GraphicsPipelineLayoutDescriptor & descriptor )
{
	auto layout = XE::MakeShared< WGPUGraphicsPipelineLayout >();
	{
		XE::Array< WGPUBindGroupLayout > entries( XE::MemoryResource::GetFrameMemoryResource() ); entries.resize( descriptor.BindGroupLayouts.size() );
		for ( size_t i = 0; i < descriptor.BindGroupLayouts.size(); i++ )
		{
			entries[i] = Cast( descriptor.BindGroupLayouts[i] )->Raw;
		}

		WGPUPipelineLayoutDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.bindGroupLayouts = entries.data();
			desc.bindGroupLayoutCount = entries.size();
		}
		layout->Raw = wgpuDeviceCreatePipelineLayout( Cast( device )->Raw, &desc );
	}
	return layout;
}

XE::GraphicsQuerySetPtr XE::GraphicsServiceWebGpu::DeviceCreateQuerySet( XE::GraphicsDevicePtr device, const XE::GraphicsQuerySetDescriptor & descriptor )
{
	auto set = XE::MakeShared< WGPUGraphicsQuerySet >();
	{
		WGPUQuerySetDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.count = descriptor.Count;
			desc.pipelineStatistics = (WGPUPipelineStatisticName *)descriptor.PipelineStatistics.data();
			desc.pipelineStatisticsCount = descriptor.PipelineStatistics.size();
		}
		set->Raw = wgpuDeviceCreateQuerySet( Cast( device )->Raw, &desc );
	}
	return set;
}

XE::GraphicsRenderBundleEncoderPtr XE::GraphicsServiceWebGpu::DeviceCreateRenderBundleEncoder( XE::GraphicsDevicePtr device, const XE::GraphicsRenderBundleEncoderDescriptor & descriptor )
{
	auto encoder = XE::MakeShared< WGPUGraphicsRenderBundleEncoder >();
	{
		WGPURenderBundleEncoderDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.colorFormats = (WGPUTextureFormat *)descriptor.ColorFormats.data();
			desc.colorFormatsCount = descriptor.ColorFormats.size();
			desc.depthReadOnly = descriptor.DepthReadOnly;
			desc.depthStencilFormat = WGPUTextureFormat( descriptor.DepthStencilFormat );
			desc.sampleCount = descriptor.SampleCount;
			desc.stencilReadOnly = descriptor.StencilReadOnly;
		}
		encoder->Raw = wgpuDeviceCreateRenderBundleEncoder( Cast( device )->Raw, &desc );
	}
	return encoder;
}

XE::GraphicsRenderPipelinePtr XE::GraphicsServiceWebGpu::DeviceCreateRenderPipeline( XE::GraphicsDevicePtr device, const XE::GraphicsRenderPipelineDescriptor & descriptor )
{
	auto pipeline = XE::MakeShared< WGPUGraphicsRenderPipeline >();
	{
		WGPUVertexState vertex = {};
		{

		}
		WGPUFragmentState fragment = {};
		{

		}
		WGPUPrimitiveState primitive = {};
		{

		}
		WGPUDepthStencilState depthStencil = {};
		{

		}
		WGPURenderPipelineDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.layout = Cast( descriptor.Layout )->Raw;
			desc.vertex = vertex;
			desc.fragment = &fragment;
			desc.primitive = primitive;
			desc.depthStencil = &depthStencil;
		}
		pipeline->Raw = wgpuDeviceCreateRenderPipeline( Cast( device )->Raw, &desc );
	}
	return pipeline;
}

XE::GraphicsSamplerPtr XE::GraphicsServiceWebGpu::DeviceCreateSampler( XE::GraphicsDevicePtr device, const XE::GraphicsSamplerDescriptor & descriptor )
{
	auto sampler = XE::MakeShared< WGPUGraphicsSampler >();
	{
		WGPUSamplerDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.addressModeU = WGPUAddressMode( descriptor.AddressModeU );
			desc.addressModeV = WGPUAddressMode( descriptor.AddressModeV );
			desc.addressModeW = WGPUAddressMode( descriptor.AddressModeW );
			desc.compare = WGPUCompareFunction( descriptor.Compare );
			desc.magFilter = WGPUFilterMode( descriptor.MagFilter );
			desc.minFilter = WGPUFilterMode( descriptor.MinFilter );
			desc.mipmapFilter = WGPUMipmapFilterMode( descriptor.MipmapFilter );
			desc.lodMaxClamp = descriptor.LodMaxClamp;
			desc.lodMinClamp = descriptor.LodMinClamp;
			desc.maxAnisotropy = descriptor.MaxAnisotropy;
		}
		sampler->Raw = wgpuDeviceCreateSampler( Cast( device )->Raw, &desc );
	}
	return sampler;
}

XE::GraphicsShaderModulePtr XE::GraphicsServiceWebGpu::DeviceCreateShaderModule( XE::GraphicsDevicePtr device, const XE::GraphicsShaderModuleDescriptor & descriptor )
{
	auto shader = XE::MakeShared< WGPUGraphicsShaderModule >();
	{
		XE::Array< WGPUShaderModuleCompilationHint > hints( XE::MemoryResource::GetFrameMemoryResource() ); hints.resize( descriptor.Hints.size() );
		for ( size_t i = 0; i < descriptor.Hints.size(); i++ )
		{
			hints[i].layout = Cast( descriptor.Hints[i].Layout )->Raw;
			hints[i].entryPoint = descriptor.Hints[i].EntryPoint.c_str();
		}
		WGPUShaderModuleDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.hints = hints.data();
			desc.hintCount = hints.size();
		}
		shader->Raw = wgpuDeviceCreateShaderModule( Cast( device )->Raw, &desc );
	}
	return shader;
}

XE::GraphicsSwapChainPtr XE::GraphicsServiceWebGpu::DeviceCreateSwapChain( XE::GraphicsDevicePtr device, XE::GraphicsSurfacePtr surface, const XE::GraphicsSwapChainDescriptor & descriptor )
{
	auto swapchain = XE::MakeShared< WGPUGraphicsSwapChain >();
	{
		WGPUSwapChainDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.width = descriptor.Width;
			desc.height = descriptor.Height;
			desc.usage = descriptor.Usage.GetValue();
			desc.format = WGPUTextureFormat( descriptor.Format );
			desc.presentMode = WGPUPresentMode( descriptor.PresentMode );
		}
		swapchain->Raw = wgpuDeviceCreateSwapChain( Cast( device )->Raw, Cast( surface )->Raw, &desc );
	}
	return swapchain;
}

XE::GraphicsTexturePtr XE::GraphicsServiceWebGpu::DeviceCreateTexture( XE::GraphicsDevicePtr device, const XE::GraphicsTextureDescriptor & descriptor )
{
	auto texture = XE::MakeShared< WGPUGraphicsTexture >();
	{
		WGPUTextureDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.size.width = descriptor.Size.x;
			desc.size.height = descriptor.Size.y;
			desc.size.depthOrArrayLayers = descriptor.Size.z;
			desc.usage = descriptor.Usage.GetValue();
			desc.format = WGPUTextureFormat( descriptor.Format );
			desc.dimension = WGPUTextureDimension( descriptor.Dimension );
			desc.sampleCount = descriptor.SampleCount;
			desc.mipLevelCount = descriptor.MipLevelCount;
			desc.viewFormats = (WGPUTextureFormat *)descriptor.ViewFormats.data();
			desc.viewFormatCount = descriptor.ViewFormats.size();
		}
		texture->Raw = wgpuDeviceCreateTexture( Cast( device )->Raw, &desc );
	}
	return texture;
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceWebGpu::TextureCreateView( XE::GraphicsTexturePtr texture, const XE::GraphicsTextureViewDescriptor & descriptor )
{
	auto view = XE::MakeShared< WGPUGraphicsTextureView >();
	{
		WGPUTextureViewDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.aspect = WGPUTextureAspect( descriptor.Aspect );
			desc.format = WGPUTextureFormat( descriptor.Format );
			desc.dimension = WGPUTextureViewDimension( descriptor.Dimension );
			desc.baseMipLevel = descriptor.BaseMipLevel;
			desc.baseArrayLayer = descriptor.BaseArrayLayer;
			desc.mipLevelCount = descriptor.MipLevelCount;
			desc.arrayLayerCount = descriptor.ArrayLayerCount;
		}
		view->Raw = wgpuTextureCreateView( Cast( texture )->Raw, &desc );
	}
	return view;
}

void XE::GraphicsServiceWebGpu::DeviceEnumerateFeatures( XE::GraphicsDevicePtr device, XE::Array< XE::GraphicsFeatureName > & features )
{
	WGPUFeatureName names[10];

	auto sz = wgpuDeviceEnumerateFeatures( Cast( device )->Raw, names );

	for ( size_t i = 0; i < sz; i++ )
	{
		features.push_back( (XE::GraphicsFeatureName)names[i] );
	}
}

bool XE::GraphicsServiceWebGpu::DeviceGetLimits( XE::GraphicsDevicePtr device, XE::GraphicsSupportedLimits & limits )
{
	WGPUSupportedLimits limit;

	if ( wgpuDeviceGetLimits( Cast( device )->Raw, &limit ) )
	{
		limits.Limits.MaxBindGroups = limit.limits.maxBindGroups;
		limits.Limits.MaxComputeInvocationsPerWorkgroup = limit.limits.maxBindGroups;
		limits.Limits.MaxComputeWorkgroupSizeX = limit.limits.maxComputeWorkgroupSizeX;
		limits.Limits.MaxComputeWorkgroupSizeY = limit.limits.maxComputeWorkgroupSizeY;
		limits.Limits.MaxComputeWorkgroupSizeZ = limit.limits.maxComputeWorkgroupSizeZ;
		limits.Limits.MaxComputeWorkgroupsPerDimension = limit.limits.maxComputeWorkgroupsPerDimension;
		limits.Limits.MaxComputeWorkgroupStorageSize = limit.limits.maxComputeWorkgroupStorageSize;
		limits.Limits.MaxDynamicStorageBuffersPerPipelineLayout = limit.limits.maxDynamicStorageBuffersPerPipelineLayout;
		limits.Limits.MaxDynamicUniformBuffersPerPipelineLayout = limit.limits.maxDynamicUniformBuffersPerPipelineLayout;
		limits.Limits.MaxInterStageShaderComponents = limit.limits.maxInterStageShaderComponents;
		limits.Limits.MaxSampledTexturesPerShaderStage = limit.limits.maxSampledTexturesPerShaderStage;
		limits.Limits.MaxSamplersPerShaderStage = limit.limits.maxSamplersPerShaderStage;
		limits.Limits.MaxStorageBufferBindingSize = limit.limits.maxStorageBufferBindingSize;
		limits.Limits.MaxStorageBuffersPerShaderStage = limit.limits.maxStorageBuffersPerShaderStage;
		limits.Limits.MaxStorageTexturesPerShaderStage = limit.limits.maxStorageTexturesPerShaderStage;
		limits.Limits.MaxTextureArrayLayers = limit.limits.maxTextureArrayLayers;
		limits.Limits.MaxTextureDimension1D = limit.limits.maxTextureDimension1D;
		limits.Limits.MaxTextureDimension2D = limit.limits.maxTextureDimension2D;
		limits.Limits.MaxTextureDimension3D = limit.limits.maxTextureDimension3D;
		limits.Limits.MaxUniformBufferBindingSize = limit.limits.maxUniformBufferBindingSize;
		limits.Limits.MaxUniformBuffersPerShaderStage = limit.limits.maxUniformBuffersPerShaderStage;
		limits.Limits.MaxVertexAttributes = limit.limits.maxVertexAttributes;
		limits.Limits.MaxVertexBufferArrayStride = limit.limits.maxVertexBufferArrayStride;
		limits.Limits.MaxVertexBuffers = limit.limits.maxVertexBuffers;
		limits.Limits.MinStorageBufferOffsetAlignment = limit.limits.minStorageBufferOffsetAlignment;
		limits.Limits.MinUniformBufferOffsetAlignment = limit.limits.minUniformBufferOffsetAlignment;

		return true;
	}

	return false;
}

XE::GraphicsQueuePtr XE::GraphicsServiceWebGpu::DeviceGetQueue( XE::GraphicsDevicePtr device )
{
	auto queue = XE::MakeShared< WGPUGraphicsQueue >();
	{
		queue->Raw = wgpuDeviceGetQueue( Cast( device )->Raw );
	}
	return queue;
}

bool XE::GraphicsServiceWebGpu::DeviceHasFeature( XE::GraphicsDevicePtr device, XE::GraphicsFeatureName feature )
{
	return wgpuDeviceHasFeature( Cast( device )->Raw, WGPUFeatureName( feature ) );
}

bool XE::GraphicsServiceWebGpu::DevicePopErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	return wgpuDevicePopErrorScope( Cast( device )->Raw, []( WGPUErrorType type, char const * message, void * userdata )
	{
		XE::GraphicsService::ErrorCallback * callback = (XE::GraphicsService::ErrorCallback *)userdata;

		( *callback )( XE::GraphicsErrorType( type ) );

	}, &callback );
}

void XE::GraphicsServiceWebGpu::DevicePushErrorScope( XE::GraphicsDevicePtr device, XE::GraphicsErrorFilter filter )
{
	wgpuDevicePushErrorScope( Cast( device )->Raw, WGPUErrorFilter( filter ) );
}

void XE::GraphicsServiceWebGpu::DeviceSetDeviceLostCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::DeviceLostCallback callback )
{
	wgpuDeviceSetDeviceLostCallback( Cast( device )->Raw, []( WGPUDeviceLostReason reason, char const * message, void * userdata )
	{
		XE::GraphicsService::DeviceLostCallback * callback = (XE::GraphicsService::DeviceLostCallback *)userdata;

		( *callback )( XE::GraphicsDeviceLostReason( reason ) );

	}, &callback );
}

void XE::GraphicsServiceWebGpu::DeviceSetUncapturedErrorCallback( XE::GraphicsDevicePtr device, XE::GraphicsService::ErrorCallback callback )
{
	wgpuDeviceSetUncapturedErrorCallback( Cast( device )->Raw, []( WGPUErrorType type, char const * message, void * userdata )
	{
		XE::GraphicsService::ErrorCallback * callback = (XE::GraphicsService::ErrorCallback *)userdata;

		( *callback )( XE::GraphicsErrorType( type ) );

	}, &callback );
}

void XE::GraphicsServiceWebGpu::QueueOnSubmittedWorkDone( XE::GraphicsQueuePtr queue, XE::GraphicsService::QueueWorkDoneCallback callback )
{
	wgpuQueueOnSubmittedWorkDone( Cast( queue )->Raw, []( WGPUQueueWorkDoneStatus status, void * userdata )
	{
		XE::GraphicsService::QueueWorkDoneCallback * callback = (XE::GraphicsService::QueueWorkDoneCallback *)userdata;

		( *callback )( XE::GraphicsQueueWorkDoneStatus( status ) );

	}, &callback );
}

void XE::GraphicsServiceWebGpu::QueueSubmit( XE::GraphicsQueuePtr queue, const XE::Array< XE::GraphicsCommandBufferPtr > & commands )
{
	XE::Array< WGPUCommandBuffer > buffers( XE::MemoryResource::GetFrameMemoryResource() ); buffers.resize( commands.size() );
	for ( size_t i = 0; i < commands.size(); i++ )
	{
		buffers[i] = Cast( commands[i] )->Raw;
	}

	wgpuQueueSubmit( Cast( queue )->Raw, buffers.size(), buffers.data() );
}

void XE::GraphicsServiceWebGpu::QueueWriteBuffer( XE::GraphicsQueuePtr queue, XE::GraphicsBufferPtr buffer, XE::uint64 buffer_offset, XE::MemoryView data )
{
	wgpuQueueWriteBuffer( Cast( queue )->Raw, Cast( buffer )->Raw, buffer_offset, data.data(), data.size() );
}

void XE::GraphicsServiceWebGpu::QueueWriteTexture( XE::GraphicsQueuePtr queue, const XE::GraphicsImageCopyTexture & dst, XE::MemoryView data, const XE::GraphicsTextureDataLayout & data_layout, const XE::Vec3f & write_size )
{
	WGPUExtent3D ext = {};
	{
		ext.width = write_size.x;
		ext.height = write_size.y;
		ext.depthOrArrayLayers = write_size.z;
	}
	WGPUImageCopyTexture cpy_dst = {};
	{
		cpy_dst.aspect = WGPUTextureAspect( dst.Aspect );
		cpy_dst.mipLevel = dst.MipLevel;
		cpy_dst.origin.x = dst.Origin.x;
		cpy_dst.origin.y = dst.Origin.y;
		cpy_dst.origin.z = dst.Origin.z;
		cpy_dst.texture = Cast( dst.Texture )->Raw;
	}
	WGPUTextureDataLayout layout = {};
	{
		layout.offset = data_layout.Offset;
		layout.bytesPerRow = data_layout.BytesPerRow;
		layout.rowsPerImage = data_layout.RowsPerImage;
	}

	wgpuQueueWriteTexture( Cast( queue )->Raw, &cpy_dst, data.data(), data.size(), &layout, &ext );
}

void XE::GraphicsServiceWebGpu::BufferMapAsync( XE::GraphicsBufferPtr buffer, XE::GraphicsMapModeFlags mode, size_t offset, size_t size, XE::GraphicsService::BufferMapCallback callback )
{
	wgpuBufferMapAsync( Cast( buffer )->Raw, mode.GetValue(), offset, size, []( WGPUBufferMapAsyncStatus status, void * userdata )
	{
		XE::GraphicsService::BufferMapCallback * callback = (XE::GraphicsService::BufferMapCallback *)userdata;

		( *callback )( XE::GraphicsBufferMapAsyncStatus( status ) );

	}, &callback );
}

XE::Span< const XE::uint8 > XE::GraphicsServiceWebGpu::BufferGetConstMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return { (const XE::uint8 *)wgpuBufferGetConstMappedRange( Cast( buffer )->Raw, offset, size ), size };
}

XE::Span< XE::uint8 > XE::GraphicsServiceWebGpu::BufferGetMappedRange( XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	return { (XE::uint8 *)wgpuBufferGetMappedRange( Cast( buffer )->Raw, offset, size ), size };
}

void XE::GraphicsServiceWebGpu::BufferUnmap( XE::GraphicsBufferPtr buffer )
{
	wgpuBufferUnmap( Cast( buffer )->Raw );
}

XE::GraphicsComputePassEncoderPtr XE::GraphicsServiceWebGpu::CommandEncoderBeginComputePass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsComputePassDescriptor & descriptor )
{
	auto pass = XE::MakeShared< WGPUGraphicsComputePassEncoder >();
	{
		XE::Array< WGPUComputePassTimestampWrite > times( XE::MemoryResource::GetFrameMemoryResource() ); times.resize( descriptor.TimestampWrites.size() );
		for ( size_t i = 0; i < descriptor.TimestampWrites.size(); i++ )
		{
			times[i].querySet = Cast( descriptor.TimestampWrites[i].QuerySet )->Raw;
			times[i].queryIndex = descriptor.TimestampWrites[i].QueryIndex;
			times[i].location = WGPUComputePassTimestampLocation( descriptor.TimestampWrites[i].Location );
		}
		WGPUComputePassDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.timestampWrites = times.data();
			desc.timestampWriteCount = times.size();
		}
		pass->Raw = wgpuCommandEncoderBeginComputePass( Cast( command_encoder )->Raw, &desc );
	}
	return pass;
}

XE::GraphicsRenderPassEncoderPtr XE::GraphicsServiceWebGpu::CommandEncoderBeginRenderPass( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsRenderPassDescriptor & descriptor )
{
	auto pass = XE::MakeShared< WGPUGraphicsRenderPassEncoder >();
	{
		XE::Array< WGPURenderPassTimestampWrite > times( XE::MemoryResource::GetFrameMemoryResource() ); times.resize( descriptor.TimestampWrites.size() );
		for ( size_t i = 0; i < descriptor.TimestampWrites.size(); i++ )
		{
			times[i].querySet = Cast( descriptor.TimestampWrites[i].QuerySet )->Raw;
			times[i].queryIndex = descriptor.TimestampWrites[i].QueryIndex;
			times[i].location = WGPURenderPassTimestampLocation( descriptor.TimestampWrites[i].Location );
		}
		XE::Array< WGPURenderPassColorAttachment > attachs( XE::MemoryResource::GetFrameMemoryResource() ); times.resize( descriptor.ColorAttachments.size() );
		for ( size_t i = 0; i < descriptor.ColorAttachments.size(); i++ )
		{
			attachs[i].view = Cast( descriptor.ColorAttachments[i].View )->Raw;
			attachs[i].loadOp = WGPULoadOp( descriptor.ColorAttachments[i].LoadOp );
			attachs[i].storeOp = WGPUStoreOp( descriptor.ColorAttachments[i].StoreOp );
			attachs[i].clearValue.r = descriptor.ColorAttachments[i].ClearValue.r;
			attachs[i].clearValue.g = descriptor.ColorAttachments[i].ClearValue.g;
			attachs[i].clearValue.b = descriptor.ColorAttachments[i].ClearValue.b;
			attachs[i].clearValue.a = descriptor.ColorAttachments[i].ClearValue.a;
			attachs[i].resolveTarget = Cast( descriptor.ColorAttachments[i].ResolveTarget )->Raw;
		}
		WGPURenderPassDepthStencilAttachment depth = {};
		{
			depth.view = Cast( descriptor.DepthStencilAttachment.View )->Raw;
			depth.depthLoadOp = WGPULoadOp( descriptor.DepthStencilAttachment.DepthLoadOp );
			depth.depthStoreOp = WGPUStoreOp( descriptor.DepthStencilAttachment.DepthStoreOp );
			depth.depthReadOnly = descriptor.DepthStencilAttachment.DepthReadOnly;
			depth.depthClearValue = descriptor.DepthStencilAttachment.DepthClearValue;
			depth.stencilLoadOp = WGPULoadOp( descriptor.DepthStencilAttachment.StencilLoadOp );
			depth.stencilStoreOp = WGPUStoreOp( descriptor.DepthStencilAttachment.StencilStoreOp );
			depth.stencilReadOnly = descriptor.DepthStencilAttachment.StencilReadOnly;
			depth.stencilClearValue = descriptor.DepthStencilAttachment.StencilClearValue;
		}

		WGPURenderPassDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
			desc.occlusionQuerySet = Cast( descriptor.OcclusionQuerySet )->Raw;
			desc.timestampWrites = times.data();
			desc.timestampWriteCount = times.size();
			desc.colorAttachments = attachs.data();
			desc.colorAttachmentCount = attachs.size();
			desc.depthStencilAttachment = &depth;
		}
		pass->Raw = wgpuCommandEncoderBeginRenderPass( Cast( command_encoder )->Raw, &desc );
	}
	return pass;
}

void XE::GraphicsServiceWebGpu::CommandEncoderCopyBufferToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsBufferPtr src, XE::uint64 src_offset, XE::GraphicsBufferPtr dst, XE::uint64 dst_offset, XE::uint64 size )
{
	wgpuCommandEncoderCopyBufferToBuffer( Cast( command_encoder )->Raw, Cast( src )->Raw, src_offset, Cast( dst )->Raw, dst_offset, size );
}

void XE::GraphicsServiceWebGpu::CommandEncoderCopyBufferToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyBuffer & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	WGPUImageCopyBuffer csrc = {};
	{
		csrc.buffer = Cast( src.Buffer )->Raw;
		csrc.layout.offset = src.Layout.Offset;
		csrc.layout.bytesPerRow = src.Layout.BytesPerRow;
		csrc.layout.rowsPerImage = src.Layout.RowsPerImage;
	}
	WGPUImageCopyTexture cdst = {};
	{
		cdst.aspect = WGPUTextureAspect( dst.Aspect );
		cdst.mipLevel = dst.MipLevel;
		cdst.origin.x = dst.Origin.x;
		cdst.origin.y = dst.Origin.y;
		cdst.origin.z = dst.Origin.z;
		cdst.texture = Cast( dst.Texture )->Raw;
	}
	WGPUExtent3D ext = {};
	{
		ext.width = copy_size.x;
		ext.height = copy_size.y;
		ext.depthOrArrayLayers = copy_size.z;
	}

	wgpuCommandEncoderCopyBufferToTexture( Cast( command_encoder )->Raw, &csrc, &cdst, &ext );
}

void XE::GraphicsServiceWebGpu::CommandEncoderCopyTextureToBuffer( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyBuffer & dst, const XE::Vec3f & copy_size )
{
	WGPUImageCopyTexture csrc = {};
	{
		csrc.aspect = WGPUTextureAspect( src.Aspect );
		csrc.mipLevel = src.MipLevel;
		csrc.origin.x = src.Origin.x;
		csrc.origin.y = src.Origin.y;
		csrc.origin.z = src.Origin.z;
		csrc.texture = Cast( src.Texture )->Raw;
	}
	WGPUImageCopyBuffer cdst = {};
	{
		cdst.buffer = Cast( dst.Buffer )->Raw;
		cdst.layout.offset = dst.Layout.Offset;
		cdst.layout.bytesPerRow = dst.Layout.BytesPerRow;
		cdst.layout.rowsPerImage = dst.Layout.RowsPerImage;
	}
	WGPUExtent3D ext = {};
	{
		ext.width = copy_size.x;
		ext.height = copy_size.y;
		ext.depthOrArrayLayers = copy_size.z;
	}

	wgpuCommandEncoderCopyTextureToBuffer( Cast( command_encoder )->Raw, &csrc, &cdst, &ext );
}

void XE::GraphicsServiceWebGpu::CommandEncoderCopyTextureToTexture( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsImageCopyTexture & src, const XE::GraphicsImageCopyTexture & dst, const XE::Vec3f & copy_size )
{
	WGPUImageCopyTexture csrc = {};
	{
		csrc.aspect = WGPUTextureAspect( src.Aspect );
		csrc.mipLevel = src.MipLevel;
		csrc.origin.x = src.Origin.x;
		csrc.origin.y = src.Origin.y;
		csrc.origin.z = src.Origin.z;
		csrc.texture = Cast( src.Texture )->Raw;
	}
	WGPUImageCopyTexture cdst = {};
	{
		cdst.aspect = WGPUTextureAspect( dst.Aspect );
		cdst.mipLevel = dst.MipLevel;
		cdst.origin.x = dst.Origin.x;
		cdst.origin.y = dst.Origin.y;
		cdst.origin.z = dst.Origin.z;
		cdst.texture = Cast( dst.Texture )->Raw;
	}
	WGPUExtent3D ext = {};
	{
		ext.width = copy_size.x;
		ext.height = copy_size.y;
		ext.depthOrArrayLayers = copy_size.z;
	}

	wgpuCommandEncoderCopyTextureToTexture( Cast( command_encoder )->Raw, &csrc, &cdst, &ext );
}

XE::GraphicsCommandBufferPtr XE::GraphicsServiceWebGpu::CommandEncoderFinish( XE::GraphicsCommandEncoderPtr command_encoder, const XE::GraphicsCommandBufferDescriptor & descriptor )
{
	auto buf = XE::MakeShared< WGPUGraphicsCommandBuffer >();
	{
		WGPUCommandBufferDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
		}
		buf->Raw = wgpuCommandEncoderFinish( Cast( command_encoder )->Raw, &desc );
	}
	return buf;
}

void XE::GraphicsServiceWebGpu::CommandEncoderInsertDebugMarker( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & marker_label )
{
	wgpuCommandEncoderInsertDebugMarker( Cast( command_encoder )->Raw, marker_label.c_str() );
}

void XE::GraphicsServiceWebGpu::CommandEncoderPopDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder )
{
	wgpuCommandEncoderPopDebugGroup( Cast( command_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::CommandEncoderPushDebugGroup( XE::GraphicsCommandEncoderPtr command_encoder, const XE::String & group_label )
{
	wgpuCommandEncoderPushDebugGroup( Cast( command_encoder )->Raw, group_label.c_str() );
}

void XE::GraphicsServiceWebGpu::CommandEncoderResolveQuerySet( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 first_query, XE::uint32 query_count, XE::GraphicsBufferPtr dst, XE::uint64 dst_offset )
{
	wgpuCommandEncoderResolveQuerySet( Cast( command_encoder )->Raw, Cast( query_set )->Raw, first_query, query_count, Cast( dst )->Raw, dst_offset );
}

void XE::GraphicsServiceWebGpu::CommandEncoderWriteTimestamp( XE::GraphicsCommandEncoderPtr command_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	wgpuCommandEncoderWriteTimestamp( Cast( command_encoder )->Raw, Cast( query_set )->Raw, query_index );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderBeginPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	wgpuComputePassEncoderBeginPipelineStatisticsQuery( Cast( compute_pass_encoder )->Raw, Cast( query_set )->Raw, query_index );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderDispatch( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 workgroup_count_x, XE::uint32 workgroup_count_y, XE::uint32 workgroup_count_z )
{
	wgpuComputePassEncoderDispatch( Cast( compute_pass_encoder )->Raw, workgroup_count_x, workgroup_count_y, workgroup_count_z );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderDispatchIndirect( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	wgpuComputePassEncoderDispatchIndirect( Cast( compute_pass_encoder )->Raw, Cast( indirect_buffer )->Raw, indirect_offset );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderEnd( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	wgpuComputePassEncoderEnd( Cast( compute_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderEndPipelineStatisticsQuery( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	wgpuComputePassEncoderEndPipelineStatisticsQuery( Cast( compute_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderInsertDebugMarker( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & marker_label )
{
	wgpuComputePassEncoderInsertDebugMarker( Cast( compute_pass_encoder )->Raw, marker_label.c_str() );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderPopDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder )
{
	wgpuComputePassEncoderPopDebugGroup( Cast( compute_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderPushDebugGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, const XE::String & group_label )
{
	wgpuComputePassEncoderPushDebugGroup( Cast( compute_pass_encoder )->Raw, group_label.c_str() );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderSetBindGroup( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	wgpuComputePassEncoderSetBindGroup( Cast( compute_pass_encoder )->Raw, group_index, Cast( group )->Raw, dynamic_offsets.size(), dynamic_offsets.data() );
}

void XE::GraphicsServiceWebGpu::ComputePassEncoderSetPipeline( XE::GraphicsComputePassEncoderPtr compute_pass_encoder, XE::GraphicsComputePipelinePtr pipeline )
{
	wgpuComputePassEncoderSetPipeline( Cast( compute_pass_encoder )->Raw, Cast( pipeline )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderBeginOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	wgpuRenderPassEncoderBeginOcclusionQuery( Cast( render_pass_encoder )->Raw, query_index );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderBeginPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsQuerySetPtr query_set, XE::uint32 query_index )
{
	wgpuRenderPassEncoderBeginPipelineStatisticsQuery( Cast( render_pass_encoder )->Raw, Cast( query_set )->Raw, query_index );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderDraw( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	wgpuRenderPassEncoderDraw( Cast( render_pass_encoder )->Raw, vertex_count, instance_count, first_vertex, first_instance );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderDrawIndexed( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	wgpuRenderPassEncoderDrawIndexed( Cast( render_pass_encoder )->Raw, index_count, instance_count, first_index, base_vertex, first_instance );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderDrawIndexedIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	wgpuRenderPassEncoderDrawIndexedIndirect( Cast( render_pass_encoder )->Raw, Cast( indirect_buffer )->Raw, indirect_offset );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderDrawIndirect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	wgpuRenderPassEncoderDrawIndirect( Cast( render_pass_encoder )->Raw, Cast( indirect_buffer )->Raw, indirect_offset );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderEnd( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	wgpuRenderPassEncoderEnd( Cast( render_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderEndOcclusionQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	wgpuRenderPassEncoderEndOcclusionQuery( Cast( render_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderEndPipelineStatisticsQuery( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	wgpuRenderPassEncoderEndPipelineStatisticsQuery( Cast( render_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderExecuteBundles( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Array< XE::GraphicsRenderBundlePtr > & bundles )
{
	XE::Array< WGPURenderBundle > bunds( XE::MemoryResource::GetFrameMemoryResource() ); bunds.resize( bundles.size() );
	for ( size_t i = 0; i < bundles.size(); i++ )
	{
		bunds[i] = Cast( bundles[i] )->Raw;
	}
	wgpuRenderPassEncoderExecuteBundles( Cast( render_pass_encoder )->Raw, bunds.size(), bunds.data() );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderInsertDebugMarker( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & marker_label )
{
	wgpuRenderPassEncoderInsertDebugMarker( Cast( render_pass_encoder )->Raw, marker_label.c_str() );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderPopDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder )
{
	wgpuRenderPassEncoderPopDebugGroup( Cast( render_pass_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderPushDebugGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::String & group_label )
{
	wgpuRenderPassEncoderPushDebugGroup( Cast( render_pass_encoder )->Raw, group_label.c_str() );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetBindGroup( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	wgpuRenderPassEncoderSetBindGroup( Cast( render_pass_encoder )->Raw, group_index, Cast( group )->Raw, dynamic_offsets.size(), dynamic_offsets.data() );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetBlendConstant( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Color & color )
{
	XE::FColor f( color );

	WGPUColor c;
	c.r = f.r; c.g = f.g; c.b = f.b; c.a = f.a;

	wgpuRenderPassEncoderSetBlendConstant( Cast( render_pass_encoder )->Raw, &c );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetIndexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderPassEncoderSetIndexBuffer( Cast( render_pass_encoder )->Raw, Cast( buffer )->Raw, WGPUIndexFormat( format ), offset, size );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetPipeline( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{
	wgpuRenderPassEncoderSetPipeline( Cast( render_pass_encoder )->Raw, Cast( pipeline )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetScissorRect( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, const XE::Recti & rect )
{
	wgpuRenderPassEncoderSetScissorRect( Cast( render_pass_encoder )->Raw, rect.x, rect.y, rect.width, rect.height );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetStencilReference( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 reference )
{
	wgpuRenderPassEncoderSetStencilReference( Cast( render_pass_encoder )->Raw, reference );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetVertexBuffer( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderPassEncoderSetVertexBuffer( Cast( render_pass_encoder )->Raw, slot, Cast( buffer )->Raw, offset, size );
}

void XE::GraphicsServiceWebGpu::RenderPassEncoderSetViewport( XE::GraphicsRenderPassEncoderPtr render_pass_encoder, XE::float32 x, XE::float32 y, XE::float32 width, XE::float32 height, XE::float32 min_depth, XE::float32 max_depth )
{
	wgpuRenderPassEncoderSetViewport( Cast( render_pass_encoder )->Raw, x, y, width, height, min_depth, max_depth );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderDraw( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 vertex_count, XE::uint32 instance_count, XE::uint32 first_vertex, XE::uint32 first_instance )
{
	wgpuRenderBundleEncoderDraw( Cast( render_bundle_encoder )->Raw, vertex_count, instance_count, first_vertex, first_instance );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderDrawIndexed( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 index_count, XE::uint32 instance_count, XE::uint32 first_index, XE::int32 base_vertex, XE::uint32 first_instance )
{
	wgpuRenderBundleEncoderDrawIndexed( Cast( render_bundle_encoder )->Raw, index_count, instance_count, first_index, base_vertex, first_instance );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderDrawIndexedIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	wgpuRenderBundleEncoderDrawIndexedIndirect( Cast( render_bundle_encoder )->Raw, Cast( indirect_buffer )->Raw, indirect_offset );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderDrawIndirect( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr indirect_buffer, XE::uint64 indirect_offset )
{
	wgpuRenderBundleEncoderDrawIndirect( Cast( render_bundle_encoder )->Raw, Cast( indirect_buffer )->Raw, indirect_offset );
}

XE::GraphicsRenderBundlePtr XE::GraphicsServiceWebGpu::RenderBundleEncoderFinish( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::GraphicsRenderBundleDescriptor & descriptor )
{
	auto bundle = XE::MakeShared< WGPUGraphicsRenderBundle >();
	{
		WGPURenderBundleDescriptor desc = {};
		{
			desc.label = descriptor.Label.c_str();
		}
		bundle->Raw = wgpuRenderBundleEncoderFinish( Cast( render_bundle_encoder )->Raw, &desc );
	}
	return bundle;
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderInsertDebugMarker( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & marker_label )
{
	wgpuRenderBundleEncoderInsertDebugMarker( Cast( render_bundle_encoder )->Raw, marker_label.c_str() );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderPopDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder )
{
	wgpuRenderBundleEncoderPopDebugGroup( Cast( render_bundle_encoder )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderPushDebugGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, const XE::String & group_label )
{
	wgpuRenderBundleEncoderPushDebugGroup( Cast( render_bundle_encoder )->Raw, group_label.c_str() );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderSetBindGroup( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 group_index, XE::GraphicsBindGroupPtr group, const XE::Array< XE::uint32 > & dynamic_offsets )
{
	wgpuRenderBundleEncoderSetBindGroup( Cast( render_bundle_encoder )->Raw, group_index, Cast( group )->Raw, dynamic_offsets.size(), dynamic_offsets.data() );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderSetIndexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsBufferPtr buffer, XE::GraphicsIndexFormat format, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderBundleEncoderSetIndexBuffer( Cast( render_bundle_encoder )->Raw, Cast( buffer )->Raw, WGPUIndexFormat( format ), offset, size );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderSetPipeline( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::GraphicsRenderPipelinePtr pipeline )
{
	wgpuRenderBundleEncoderSetPipeline( Cast( render_bundle_encoder )->Raw, Cast( pipeline )->Raw );
}

void XE::GraphicsServiceWebGpu::RenderBundleEncoderSetVertexBuffer( XE::GraphicsRenderBundleEncoderPtr render_bundle_encoder, XE::uint32 slot, XE::GraphicsBufferPtr buffer, XE::uint64 offset, XE::uint64 size )
{
	wgpuRenderBundleEncoderSetVertexBuffer( Cast( render_bundle_encoder )->Raw, slot, Cast( buffer )->Raw, offset, size );
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceWebGpu::ComputePipelineGetBindGroupLayout( XE::GraphicsComputePipelinePtr compute_pipeline, XE::uint32 group_index )
{
	auto layout = XE::MakeShared< WGPUGraphicsBindGroupLayout >();
	{
		layout->Raw = wgpuComputePipelineGetBindGroupLayout( Cast( compute_pipeline )->Raw, group_index );
	}
	return layout;
}

XE::GraphicsBindGroupLayoutPtr XE::GraphicsServiceWebGpu::RenderPipelineGetBindGroupLayout( XE::GraphicsRenderPipelinePtr render_pipeline, XE::uint32 group_index )
{
	auto layout = XE::MakeShared< WGPUGraphicsBindGroupLayout >();
	{
		layout->Raw = wgpuRenderPipelineGetBindGroupLayout( Cast( render_pipeline )->Raw, group_index );
	}
	return layout;
}

void XE::GraphicsServiceWebGpu::ShaderModuleGetCompilationInfo( XE::GraphicsShaderModulePtr shader_module, XE::GraphicsService::CompilationInfoCallback callback )
{
	wgpuShaderModuleGetCompilationInfo( Cast( shader_module )->Raw, []( WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const * compilationInfo, void * userdata )
	{
		XE::GraphicsService::CompilationInfoCallback * callback = (XE::GraphicsService::CompilationInfoCallback *)userdata;

		XE::GraphicsCompilationInfo info;
		for ( size_t i = 0; i < compilationInfo->messageCount; i++ )
		{
			XE::GraphicsCompilationMessage msg;

			msg.LineNum = compilationInfo->messages[i].lineNum;
			msg.LinePos = compilationInfo->messages[i].linePos;
			msg.Message = compilationInfo->messages[i].message;
			msg.Offset = compilationInfo->messages[i].offset;
			msg.Type = GraphicsCompilationMessageType( compilationInfo->messages[i].type );

			info.Messages.push_back( msg );
		}

		( *callback )( XE::GraphicsCompilationInfoRequestStatus( status ), info );

	}, &callback );
}

void XE::GraphicsServiceWebGpu::ShaderModuleSetLabel( XE::GraphicsShaderModulePtr shader_module, const XE::String & label )
{
	wgpuShaderModuleSetLabel( Cast( shader_module )->Raw, label.c_str() );
}

XE::GraphicsTextureFormat XE::GraphicsServiceWebGpu::SurfaceGetPreferredFormat( XE::GraphicsSurfacePtr surface, XE::GraphicsAdapterPtr adapter )
{
	return XE::GraphicsTextureFormat( wgpuSurfaceGetPreferredFormat( Cast( surface )->Raw, Cast( adapter )->Raw ) );
}

XE::GraphicsTextureViewPtr XE::GraphicsServiceWebGpu::SwapChainGetCurrentTextureView( XE::GraphicsSwapChainPtr swap_chain )
{
	auto view = XE::MakeShared< WGPUGraphicsTextureView >();
	{
		view->Raw = wgpuSwapChainGetCurrentTextureView( Cast( swap_chain )->Raw );
	}
	return view;
}

void XE::GraphicsServiceWebGpu::SwapChainPresent( XE::GraphicsSwapChainPtr swap_chain )
{
	wgpuSwapChainPresent( Cast( swap_chain )->Raw );
}

#endif // GRAPHICS_API & GRAPHICS_WEBGPU

