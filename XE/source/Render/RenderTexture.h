/*!
 * \file	RenderTexture.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERTEXTURE_H__90FD6768_55DE_47C9_9733_86656FC5926E
#define RENDERTEXTURE_H__90FD6768_55DE_47C9_9733_86656FC5926E

#include "Core/Type.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderTexture : public XE::Object
{
	OBJECT( RenderTexture, XE::Object )

private:
	friend class RenderService;

public:
	RenderTexture();

	~RenderTexture() override;

public:
	XE::int32 GetWidth() const;

	XE::int32 GetHeight() const;

	XE::int32 GetDepth() const;

	XE::GraphicsTextureFormat GetFormat() const;

	XE::GraphicsTextureDimension GetDimension() const;

private:
	void ResetTextureView( XE::int32 width, XE::int32 height, XE::int32 depth, XE::GraphicsTextureFormat format, XE::GraphicsTextureDimension dimension, const XE::GraphicsTextureViewPtr & view );

private:
	XE::int32 _Width;
	XE::int32 _Height;
	XE::int32 _Depth;
	XE::GraphicsTextureFormat _Format;
	XE::GraphicsTextureDimension _Dimension;
	XE::GraphicsTextureViewPtr _TextureView;
};

OBJECT_POOL_ALLOCATOR( XE::RenderTexture );

END_XE_NAMESPACE

#endif//RENDERTEXTURE_H__90FD6768_55DE_47C9_9733_86656FC5926E
