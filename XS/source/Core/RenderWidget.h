/*!
 * \file	RenderWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERWIDGET_H__F71A92F7_0594_4CF0_9E3E_8DC00F333AF7
#define RENDERWIDGET_H__F71A92F7_0594_4CF0_9E3E_8DC00F333AF7

#include "Widget.h"

BEG_XS_NAMESPACE

class XS_API RenderWidget : public XS::Widget
{
	Q_OBJECT

public:
	RenderWidget( QWidget * parent = nullptr );

	~RenderWidget() override;

public:
	const XE::GraphicsSurfacePtr & GetSurface() const;

protected:
	void showEvent( QShowEvent * event ) override;

	void closeEvent( QCloseEvent * event ) override;

	void resizeEvent( QResizeEvent * event ) override;

	void paintEvent( QPaintEvent * event ) override;

private:
	XE::GraphicsSurfacePtr _Surface;
};

END_XS_NAMESPACE

#endif//RENDERWIDGET_H__F71A92F7_0594_4CF0_9E3E_8DC00F333AF7
