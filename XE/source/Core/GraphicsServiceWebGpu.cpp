#include "GraphicsService.h"

#if !defined( NULL_RENDER ) && ( PLATFORM_OS & OS_WASM )

#include <webgpu/webgpu.h>

IMPLEMENT_META( XE::GraphicsService );

XE::GraphicsService::GraphicsService()
	:_p( nullptr )
{

}

XE::GraphicsService::~GraphicsService()
{

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

#endif // !defined( NULL_RENDER ) && ( PLATFORM_OS & OS_EMSCRIPTEN )

