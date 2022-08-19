/*!
 * \file	GameScene.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/14
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GAMESCENE_H__28F1025F_BA22_4F52_AA5C_A9112428279E
#define GAMESCENE_H__28F1025F_BA22_4F52_AA5C_A9112428279E

#include "Widget.h"

UI_DECL( GameScene );

BEG_XS_NAMESPACE

class XS_API GameScene : public XS::Widget
{
	Q_OBJECT

public:
	GameScene( QWidget * parent = nullptr );

	~GameScene() override;

protected:
	bool eventFilter( QObject * watched, QEvent * event ) override;

private:
	Ui::GameScene * ui;
};

END_XS_NAMESPACE

#endif//GAMESCENE_H__28F1025F_BA22_4F52_AA5C_A9112428279E
