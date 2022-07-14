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

#include "Core/Core.h"

UI_DECL( GameScene );

BEG_XS_NAMESPACE

class XS_API GameScene : public QWidget
{
	Q_OBJECT

public:
	GameScene( const QString & title, QWidget * parent = nullptr );

	~GameScene() override;

public:


private:
	Ui::GameScene * ui;
};

END_XS_NAMESPACE

#endif//GAMESCENE_H__28F1025F_BA22_4F52_AA5C_A9112428279E
