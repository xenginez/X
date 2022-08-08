/*!
 * \file	RenderGraphEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143
#define RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143

#include "Core/Core.h"

UI_DECL( RenderGraphEditor );

BEG_XS_NAMESPACE

class RenderGraphEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	RenderGraphEditor( QWidget * parent = nullptr );

	~RenderGraphEditor() override;

public:

private:
	Ui::RenderGraphEditor * ui;
};

END_XS_NAMESPACE

#endif//RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143
