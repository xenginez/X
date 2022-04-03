/*!
 * \file	GameSceneEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/04/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GAMESCENEEDITOR_H__391B86FC_35F4_4404_B14D_AB5418BE3285
#define GAMESCENEEDITOR_H__391B86FC_35F4_4404_B14D_AB5418BE3285

#include "../DockWidget.h"

UI_DECL( GameSceneEditor );

BEG_XS_NAMESPACE

class XS_API GameSceneEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	GameSceneEditor( QWidget * parent = nullptr );

	~GameSceneEditor();

public:
	WId display() const;

private slots:
	void OnFpsClicked( bool checked = false );
	void OnAudioClicked( bool checked = false );
	void OnToolsClicked( bool checked = false );
	void OnRatioCurrentIndexChanged( int index );
	void OnCameraCurrentIndexChanged( int index );

private:
	Ui::GameSceneEditor * ui;
};

END_XS_NAMESPACE

#endif//GAMESCENEEDITOR_H__391B86FC_35F4_4404_B14D_AB5418BE3285
