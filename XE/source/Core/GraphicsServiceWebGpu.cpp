#include "GraphicsService.h"

#if GRAPHICS_API == GRAPHICS_WEBGPU

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

#endif // GRAPHICS_API == GRAPHICS_WEBGPU

