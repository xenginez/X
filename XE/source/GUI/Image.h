/*!
 * \file	Image.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/17
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef IMAGE_H__851A4860_3D78_4A44_96AA_22B2E4D7BBAC
#define IMAGE_H__851A4860_3D78_4A44_96AA_22B2E4D7BBAC

#include "Core/Asset.h"
#include "Utils/Picture.h"

#include "Widget.h"

BEG_XE_NAMESPACE

class XE_API Image : public XE::Widget
{
	OBJECT( Image, XE::Widget )

public:
	Image();

	~Image() override;

protected:
	void OnStartup() override;

	void OnRender() override;

private:
	XE::Color _Color;
	XE::AssetPtr< XE::Picture > _Picture;

private:
	XE::GraphicsTextureHandle _Texture;
};

END_XE_NAMESPACE

#endif//IMAGE_H__851A4860_3D78_4A44_96AA_22B2E4D7BBAC
