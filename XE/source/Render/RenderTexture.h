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

private:
	void ResetTextureView( const XE::GraphicsTextureViewPtr & val );
};

END_XE_NAMESPACE

#endif//RENDERTEXTURE_H__90FD6768_55DE_47C9_9733_86656FC5926E
