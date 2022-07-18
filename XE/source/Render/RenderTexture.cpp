#include "RenderTexture.h"

#include "Core/CoreFramework.h"
#include "Core/GraphicsService.h"

BEG_META( XE::RenderTexture )
END_META()

XE::RenderTexture::RenderTexture()
{

}

XE::RenderTexture::~RenderTexture()
{

}

XE::int32 XE::RenderTexture::GetWidth() const
{
	return _Width;
}

XE::int32 XE::RenderTexture::GetHeight() const
{
	return _Height;
}

XE::int32 XE::RenderTexture::GetDepth() const
{
	return _Depth;
}

XE::GraphicsTextureFormat XE::RenderTexture::GetFormat() const
{
	return _Format;
}

XE::GraphicsTextureDimension XE::RenderTexture::GetDimension() const
{
	return _Dimension;
}

void XE::RenderTexture::ResetTextureView( XE::int32 width, XE::int32 height, XE::int32 depth, XE::GraphicsTextureFormat format, XE::GraphicsTextureDimension dimension, const XE::GraphicsTextureViewPtr & view )
{
	_Width = width;
	_Height = height;
	_Depth = depth;
	_Format = format;
	_Dimension = dimension;
	_TextureView = view;
}
