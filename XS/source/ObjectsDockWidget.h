/*!
 * \file	SceneDockWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SCENEDOCKWIDGET_H__17295ACA_C889_4EA8_B419_18E18B7BD0B8
#define SCENEDOCKWIDGET_H__17295ACA_C889_4EA8_B419_18E18B7BD0B8

#include <QTreeWidget>

#include "DockWidget.h"

UI_DECL( ObjectsDockWidget );

BEG_XS_NAMESPACE

class XS_API ObjectsDockWidget : public XS::DockWidget
{
	Q_OBJECT

public:
	ObjectsDockWidget( QWidget * parent = nullptr );

	~ObjectsDockWidget();

public:
	void Save( QSettings & settings ) override;

	void Load( QSettings & settings ) override;

private slots:
	void OnLogicTreeWidgetItemClicked( QTreeWidgetItem * item, int column );

	void OnRenderTreeWidgetItemClicked( QTreeWidgetItem * item, int column );

private:
	void OnInspectorClicked();

private:
	Ui::ObjectsDockWidget * ui;
};

END_XS_NAMESPACE

XS_DECLARE_WIDGET( ObjectsDockWidget );

#endif//SCENEDOCKWIDGET_H__17295ACA_C889_4EA8_B419_18E18B7BD0B8
