/*!
 * \file	Label.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LABEL_H__18151F8A_49A1_4E79_BA4A_1323B4A987FC
#define LABEL_H__18151F8A_49A1_4E79_BA4A_1323B4A987FC

#include "Font.h"
#include "Widget.h"

BEG_XE_NAMESPACE

class XE_API Label : public XE::Widget
{
	OBJECT( Label, XE::Widget )

public:
	Label();

	~Label() override;

protected:
	virtual void OnStartup();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClearup();

public:
	const XE::Color & GetColor() const;

	void SetColor( const XE::Color & val );

	const XE::Utf8String & GetFont() const;

	void SetFont( const XE::Utf8String & val );

	const XE::Utf8String & GetText() const;

	void SetText( const XE::Utf8String & val );

	XE::VAlignment GetVerticalAlignment() const;

	void SetVerticalAlignment( XE::VAlignment val );

	XE::HAlignment GetHorizontalAlignment() const;

	void SetHorizontalAlignment( XE::HAlignment val );

	bool GetWordWrap() const;

	void SetWordWrap( bool val );

private:
	XE::Color _Color;
	XE::Utf8String _Font;
	XE::Utf8String _Text;
	bool _WordWarp = false;
	XE::VAlignment _VAlignment = XE::VAlignment::CENTER;
	XE::HAlignment _HAlignment = XE::HAlignment::LEFT;
};

END_XE_NAMESPACE

#endif//LABEL_H__18151F8A_49A1_4E79_BA4A_1323B4A987FC
