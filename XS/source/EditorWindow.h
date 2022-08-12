/*!
 * \file	EditorWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef EDITORWINDOW_H__93FDDFE5_AFB4_406F_936E_DC98531748AB
#define EDITORWINDOW_H__93FDDFE5_AFB4_406F_936E_DC98531748AB

#include "DockWidget.h"

BEG_XS_NAMESPACE

class XS_API EditorWindow : public XS::DockWidget
{
	Q_OBJECT

public:
	EditorWindow( QWidget * parent = nullptr );

	~EditorWindow() override;
};

END_XS_NAMESPACE

#endif//EDITORWINDOW_H__93FDDFE5_AFB4_406F_936E_DC98531748AB
