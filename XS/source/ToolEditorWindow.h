/*!
 * \file	ToolEditorWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TOOLEDITORWINDOW_H__1C449FAF_DDE5_4037_8E06_0AFB3F301245
#define TOOLEDITORWINDOW_H__1C449FAF_DDE5_4037_8E06_0AFB3F301245

#include "DockWidget.h"

BEG_XS_NAMESPACE

class XS_API ToolEditorWindow : public XS::DockWidget
{
	Q_OBJECT

public:
	ToolEditorWindow( QWidget * parent = nullptr );

	~ToolEditorWindow() override;

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();
};

END_XS_NAMESPACE

#endif//TOOLEDITORWINDOW_H__1C449FAF_DDE5_4037_8E06_0AFB3F301245
