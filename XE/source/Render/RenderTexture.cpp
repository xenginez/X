#include "RenderTexture.h"

#include "Core/CoreFramework.h"

#include "RenderService.h"

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

XE::GraphicsTextureFormat XE::RenderTexture::GetFormat() const
{
	return _Format;
}

XE::GraphicsTextureViewPtr XE::RenderTexture::GetTextureView() const
{
	return _TextureView;
}

XE::RenderTexturePtr XE::RenderTexture::Create( XE::int32 width, XE::int32 height, XE::int32 depth, XE::GraphicsTextureFormat format )
{
	XE::RenderTexturePtr texture = XE::MakeShared< XE::RenderTexture >();

	texture->_Width = width;
	texture->_Height = height;
	texture->_Format = format;

	if ( auto service = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::RenderService >() )
	{
		service->GetRenderTextureFromPool( width, height, format );
	}

	return texture;
}

void XE::RenderTexture::ResetTextureView( XE::int32 width, XE::int32 height, XE::int32 depth, XE::GraphicsTextureFormat format, const XE::GraphicsTextureViewPtr & view )
{
	_Width = width;
	_Height = height;
	_Format = format;
	_TextureView = view;
}
