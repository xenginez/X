#include "GraphicsService.h"

#if !defined( NULL_RENDER ) && ( PLATFORM_OS & ( OS_LINUX | OS_ANDROID ) )

#if PLATFORM_OS == OS_ANDROID
#	define VK_USE_PLATFORM_ANDROID_KHR
#	define KHR_SURFACE_EXTENSION_NAME VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#	define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_ANDROID
#elif PLATFORM_OS == OS_LINUX
#	define VK_USE_PLATFORM_XCB_KHR
#	define KHR_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME
#	define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_LINUX
#elif PLATFORM_OS == OS_WINDOWS
#	define VK_USE_PLATFORM_WIN32_KHR
#	define KHR_SURFACE_EXTENSION_NAME  VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#	define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_WINDOWS
#endif

#define VK_NO_STDDEF_H
#define VK_NO_STDINT_H
#define VK_NO_PROTOTYPES

#include <vulkan/vulkan.h>

#define VOLK_IMPLEMENTATION
#include <volk/volk.h>


IMPLEMENT_META( XE::GraphicsService );

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

#endif // !defined( NULL_RENDER ) && ( PLATFORM_OS & ( OS_LINUX | OS_ANDROID ) )

