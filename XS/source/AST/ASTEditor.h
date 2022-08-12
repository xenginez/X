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

#include "EditorWindow.h"

UI_DECL( ASTEditor );

BEG_XS_NAMESPACE

class ASTEditor : public XS::EditorWindow
{
	Q_OBJECT

public:
	ASTEditor( QWidget * parent = nullptr );

	~ASTEditor() override;

public:

private:
	Ui::ASTEditor * ui;
};

END_XS_NAMESPACE

#endif//RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143
