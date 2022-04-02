/*!
 * \file	GameSceneDockWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GAMESCENEDOCKWIDGET_H__0DE254A8_4B79_43D4_8DA5_EA35B0AB4F17
#define GAMESCENEDOCKWIDGET_H__0DE254A8_4B79_43D4_8DA5_EA35B0AB4F17

#include "../DockWidget.h"

UI_DECL( GameSceneDockWidget );

BEG_XS_NAMESPACE

class XS_API GameSceneDockWidget : public XS::DockWidget
{
	Q_OBJECT

public:
	GameSceneDockWidget( QWidget * parent = nullptr );

	~GameSceneDockWidget();

public:
	WId display() const;

private slots:
	void OnFpsClicked( bool checked = false );
	void OnAudioClicked( bool checked = false );
	void OnToolsClicked( bool checked = false );
	void OnRatioCurrentIndexChanged( int index );
	void OnCameraCurrentIndexChanged( int index );

private:
	Ui::GameSceneDockWidget * ui;
};

END_XS_NAMESPACE

#endif//GAMESCENEDOCKWIDGET_H__0DE254A8_4B79_43D4_8DA5_EA35B0AB4F17
