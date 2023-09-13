/*!
 * \file	NodeEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/13
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEEDITOR_H__F9578566_72B5_4583_9B30_F74EEBD01F27
#define NODEEDITOR_H__F9578566_72B5_4583_9B30_F74EEBD01F27

#include "Core/Inspector.h"
#include "Core/ToolEditorWindow.h"

UI_DECL( NodeEditor );

BEG_XS_NAMESPACE

class XS_API NodeEditor : public XS::ToolEditorWindow
{
	Q_OBJECT

public:
	NodeEditor( QWidget * parent = nullptr );

	~NodeEditor();

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

private:
	Ui::NodeEditor * ui;
};

END_XS_NAMESPACE

#endif//NODEEDITOR_H__F9578566_72B5_4583_9B30_F74EEBD01F27
