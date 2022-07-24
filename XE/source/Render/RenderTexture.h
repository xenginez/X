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

	XE::GraphicsTextureFormat GetFormat() const;

	XE::GraphicsTextureViewPtr GetTextureView() const;

public:
	static XE::RenderTexturePtr Create( XE::int32 width, XE::int32 height, XE::int32 depth, XE::GraphicsTextureFormat format );

private:
	void ResetTextureView( XE::int32 width, XE::int32 height, XE::int32 depth, XE::GraphicsTextureFormat format, const XE::GraphicsTextureViewPtr & view );

private:
	XE::int32 _Width = 0;
	XE::int32 _Height = 0;
	XE::GraphicsTextureFormat _Format = XE::GraphicsTextureFormat::RGBA8SNORM;
	XE::GraphicsTextureViewPtr _TextureView = nullptr;
};

OBJECT_POOL_ALLOCATOR( XE::RenderTexture );

END_XE_NAMESPACE

#endif//RENDERTEXTURE_H__90FD6768_55DE_47C9_9733_86656FC5926E
