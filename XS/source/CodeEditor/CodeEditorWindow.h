/*!
 * \file	CodeEditorWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CODEEDITORWINDOW_H__28AB8D25_3DA1_4D6F_8CB7_173980864DE2
#define CODEEDITORWINDOW_H__28AB8D25_3DA1_4D6F_8CB7_173980864DE2

#include "Core/AssetEditorWindow.h"

UI_DECL( CodeEditorWindow );

BEG_XS_NAMESPACE

class XS_API CodeEditorWindow : public XS::AssetEditorWindow
{
	Q_OBJECT

public:
	CodeEditorWindow( QWidget * parent = nullptr );

	~CodeEditorWindow();

public:
	void assetOpen( const QUuid & uuid ) override;

private:
	Ui::CodeEditorWindow * ui;
};

END_XS_NAMESPACE

#endif//CODEEDITORWINDOW_H__28AB8D25_3DA1_4D6F_8CB7_173980864DE2