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

#include "Core/Core.h"
#include "GameScene.h"

UI_DECL( GameSceneEditor );

BEG_XS_NAMESPACE

class XS_API GameSceneEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	GameSceneEditor( QWidget * parent = nullptr );

	~GameSceneEditor();

private slots:
	void OnLayoutClicked( bool checked = false );

private:
	XS::GameScene * CreateGameScene();

private:
	void ReLayout();

private:
	Ui::GameSceneEditor * ui;

	QSize _LayoutSize;
	QVector< QRect > _Layouts;
	QVector< XS::GameScene * > _GameScenes;
};

END_XS_NAMESPACE

#endif//GAMESCENEEDITOR_H__391B86FC_35F4_4404_B14D_AB5418BE3285
