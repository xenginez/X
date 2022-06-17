/*!
 * \file	Font.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FONT_H__0F93B0F4_8D25_41D3_9A2D_EF230A894499
#define FONT_H__0F93B0F4_8D25_41D3_9A2D_EF230A894499

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Font : public XE::Object
{
	OBJECT( Font, XE::Object )

public:
	Font();

	~Font() override;

public:
	const XE::Utf8String & GetName() const;

	void SetName( const XE::Utf8String & val );

	XE::uint32 GetFontSize() const;

	void SetFontSize( XE::uint32 val );

	XE::uint32 GetPixelSize() const;

	void SetPixelSize( XE::uint32 val );

private:
	XE::Utf8String _Name;
	XE::uint32 _FontSize = 12;
	XE::uint32 _PixelSize = 12;
};

END_XE_NAMESPACE

#endif//FONT_H__0F93B0F4_8D25_41D3_9A2D_EF230A894499
