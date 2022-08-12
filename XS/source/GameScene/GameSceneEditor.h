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

#include "EditorWindow.h"

#include "GameScene.h"

UI_DECL( GameSceneEditor );

BEG_XS_NAMESPACE

class XS_API GameSceneEditor : public XS::EditorWindow
{
	Q_OBJECT

public:
	GameSceneEditor( QWidget * parent = nullptr );

	~GameSceneEditor();

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnLayoutClicked( bool checked = false );

private:
	void ReLayout();

private:
	Ui::GameSceneEditor * ui;

	QSize _LayoutSize = { 1,1 };
	QVector< QRect > _Layouts = { QRect( 0,0,1,1 ) };

	QVector< XS::GameScene * > _GameScenes;
};

END_XS_NAMESPACE

#endif//GAMESCENEEDITOR_H__391B86FC_35F4_4404_B14D_AB5418BE3285
