/*!
 * \file	TestNodeEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/30
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TESTNODEEDITOR_H__6F1E2658_1F67_4BB8_8B49_BBA3CA4F9368
#define TESTNODEEDITOR_H__6F1E2658_1F67_4BB8_8B49_BBA3CA4F9368

#include "Core/DockWidget.h"

UI_DECL( TestNodeEditor );

BEG_XS_NAMESPACE

class XS_API TestNodeEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	TestNodeEditor( QWidget * parent = nullptr );

	~TestNodeEditor() override;

private:
	Ui::TestNodeEditor * ui;
};

END_XS_NAMESPACE

#endif//TESTNODEEDITOR_H__6F1E2658_1F67_4BB8_8B49_BBA3CA4F9368
