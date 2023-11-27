/*!
 * \file	NodeGraphicsEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2023/11/27
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSEDITOR_H__93B274AA_01EB_4129_A933_0C12E7E31C84
#define NODEGRAPHICSEDITOR_H__93B274AA_01EB_4129_A933_0C12E7E31C84

#include "Widgets/ToolEditorWindow.h"

UI_DECL( NodeGraphicsEditor );

BEG_XS_NAMESPACE

class XS_API NodeGraphicsEditor : public XS::ToolEditorWindow
{
	Q_OBJECT

public:
	NodeGraphicsEditor( QWidget * parent = nullptr );

	~NodeGraphicsEditor();

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

private:
	Ui::NodeGraphicsEditor * ui;
};

END_XS_NAMESPACE

#endif//NODEGRAPHICSEDITOR_H__93B274AA_01EB_4129_A933_0C12E7E31C84