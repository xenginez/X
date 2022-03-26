/*!
 * \file	EditSceneDockWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef EDITSCENEDOCKWIDGET_H__B1A324D4_9A9B_402B_B82A_FBE8CAFA1600
#define EDITSCENEDOCKWIDGET_H__B1A324D4_9A9B_402B_B82A_FBE8CAFA1600

#include "DockWidget.h"

UI_DECL( EditSceneDockWidget );

BEG_XS_NAMESPACE

class XS_API EditSceneDockWidget : public XS::DockWidget
{
	Q_OBJECT

public:
	EditSceneDockWidget( QWidget * parent = nullptr );

	~EditSceneDockWidget();

public:
	WId display() const;

private slots:
	void OnAudioClicked( bool checked = false );
	void OnCameraClicked( bool checked = false );
	void OnGridClicked( bool checked = false );
	void OnIconsClicked( bool checked = false );
	void OnLightClicked( bool checked = false );
	void OnMoveClicked( bool checked = false );
	void OnRotateClicked( bool checked = false );
	void OnScaleClicked( bool checked = false );
	void OnSearchFinished();

private:
	Ui::EditSceneDockWidget * ui;
};

END_XS_NAMESPACE

XS_DECLARE_WIDGET( EditSceneDockWidget );

#endif//EDITSCENEDOCKWIDGET_H__B1A324D4_9A9B_402B_B82A_FBE8CAFA1600
