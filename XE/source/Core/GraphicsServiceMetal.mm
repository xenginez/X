#include "GraphicsService.h"

#if GRAPHICS_API == GRAPHICS_METAL

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

#ifdef PLATFORM_OS == OS_IOS
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

#import <Foundation/Foundation.h>


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

#endif // GRAPHICS_API == GRAPHICS_METAL
