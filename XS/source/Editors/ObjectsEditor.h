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

#include "../Inspector.h"
#include "../DockWidget.h"

class QTreeWidgetItem;

UI_DECL( ObjectsEditor );

BEG_XS_NAMESPACE

class XS_API ObjectsEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	ObjectsEditor( QWidget * parent = nullptr );

	~ObjectsEditor();

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnLogicTreeWidgetItemClicked( QTreeWidgetItem * item, int column );

	void OnRenderTreeWidgetItemClicked( QTreeWidgetItem * item, int column );

private:
	void OnInspectorClicked( const XE::Variant & val );

private:
	Ui::ObjectsEditor * ui;
	XS::Inspector * _Inspector = nullptr;
};

END_XS_NAMESPACE

#endif//OBJECTSEDITOR_H__7B7C0B22_7F47_4FB6_9D83_949A00BD8BD6
