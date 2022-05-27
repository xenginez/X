#include "GraphicsService.h"

#if !defined( NULL_RENDER ) && ( PLATFORM_OS & ( OS_WINDOWS | OS_XBOX ) )

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <D3DX12/d3dx12.h>

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

IMPLEMENT_META( XE::GraphicsService );

namespace
{
	template< typename T > T * HandleCast( XE::Handle< T > handle )
	{
		return reinterpret_cast<T *>( handle.GetValue() );
	}
	template< typename T > XE::Handle< T > HandleCast( T * ptr )
	{
		return XE::HandleCast< T >( reinterpret_cast<XE::uint64>( ptr ) );
	}
}

namespace XE
{
	class GraphicsInstance
	{

	};
	class GraphicsAdapter
	{
		IDXGIAdapter4 * Adapter = nullptr;
	};
	class GraphicsDevice
	{
		ID3D12Device * Device = nullptr;
	};
	class GraphicsBindGroup
	{

	};
	class GraphicsBindGroupLayout
	{

	};
	class GraphicsBuffer
	{
		ID3D12Resource * Resource = nullptr;
	};
	class GraphicsCommandBuffer
	{
		ID3D12CommandQueue * CommandQueue = nullptr;
		ID3D12CommandAllocator * CommandAllocator = nullptr;
	};
	class GraphicsCommandEncoder
	{
		ID3D12CommandList * CommandList = nullptr;
	};
	class GraphicsComputePassEncoder
	{
		ID3D12GraphicsCommandList * CommandList = nullptr;
	};
	class GraphicsComputePipeline
	{
		ID3D12PipelineState * PipelineState = nullptr;
	};
	class GraphicsPipelineLayout
	{

	};
	class GraphicsQuerySet
	{
		ID3D12QueryHeap * QueryHeap = nullptr;
	};
	class GraphicsQueue
	{

	};
	class GraphicsRenderBundle
	{

	};
	class GraphicsRenderBundleEncoder
	{

	};
	class GraphicsRenderPassEncoder
	{
		ID3D12GraphicsCommandList * CommandList = nullptr;
	};
	class GraphicsRenderPipeline
	{
		ID3D12PipelineState * PipelineState = nullptr;
	};
	class GraphicsSampler
	{

	};
	class GraphicsShaderModule
	{
		D3D12_SHADER_BYTECODE ShaderCode;
	};
	class GraphicsSurface
	{
		XE::WindowPtr Window;
	};
	class GraphicsSwapChain
	{
		IDXGISwapChain4 * SwapChain = nullptr;
	};
	class GraphicsTexture
	{

	};
	class GraphicsTextureView
	{

	};
}

struct XE::GraphicsService::Private
{

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

}

bool XE::GraphicsService::Startup()
{
	return true;
}

void XE::GraphicsService::Update()
{

}

void XE::GraphicsService::Clearup()
{

}

#endif // !defined( NULL_RENDER ) && ( PLATFORM_OS & ( OS_WINDOWS | OS_XBOX ) )