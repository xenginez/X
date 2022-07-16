#include "GameScene.h"

#include "ui_GameScene.h"

XS::GameScene::GameScene( QWidget * parent /*= nullptr */ )
	: ui( new Ui::GameScene )
{
	setFrameShape( QFrame::Box );
	setFrameShadow( QFrame::Sunken );

	ui->setupUi( this );

	ui->fps->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_fps.png" ) );
	ui->audio->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_audio.png" ) );
	ui->tools->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_tools.png" ) );
	ui->ratio_icon->setPixmap( QPixmap( "SkinIcons:/images/gamescene/icon_gamescene_ratio.png" ) );
	ui->camera_icon->setPixmap( QPixmap( "SkinIcons:/images/gamescene/icon_gamescene_camera.png" ) );

	ui->display->installEventFilter( this );
}

XS::GameScene::~GameScene()
{
	delete ui;
}

bool XS::GameScene::eventFilter( QObject * watched, QEvent * event )
{
	if ( watched == ui->display )
	{
	}

	return QFrame::eventFilter( watched, event );
}
