/*!
 * \file	ObjectsEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/04/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef OBJECTSEDITOR_H__7B7C0B22_7F47_4FB6_9D83_949A00BD8BD6
#define OBJECTSEDITOR_H__7B7C0B22_7F47_4FB6_9D83_949A00BD8BD6

#include "Core/ToolEditorWindow.h"

class QTreeWidgetItem;

UI_DECL( WorldEditor );

BEG_XS_NAMESPACE

class XS_API WorldEditor : public XS::ToolEditorWindow
{
	Q_OBJECT

public:
	WorldEditor( QWidget * parent = nullptr );

	~WorldEditor();

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnLogicSystemClicked();

	void OnLogicTreeWidgetItemClicked( QTreeWidgetItem * item, int column );

	void OnRenderTreeWidgetItemClicked( QTreeWidgetItem * item, int column );

private:
	Ui::WorldEditor * ui;
};

END_XS_NAMESPACE

#endif//OBJECTSEDITOR_H__7B7C0B22_7F47_4FB6_9D83_949A00BD8BD6
