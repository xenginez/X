#include "GameScene.h"

#include "ui_GameScene.h"

XS::GameScene::GameScene( const QString & title, QWidget * parent /*= nullptr */ )
	: ui( new Ui::GameScene )
{
	ui->setupUi( this );

	ui->title->setText( title );
	ui->fps->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_fps.png" ) );
	ui->audio->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_audio.png" ) );
	ui->tools->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_tools.png" ) );

}

XS::GameScene::~GameScene()
{
	delete ui;
}
