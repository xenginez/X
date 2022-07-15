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

	// XS::CoreFramework::GetCurrentFramework()->GetServiceT< XE::RenderService >();
}

XS::GameScene::~GameScene()
{
	delete ui;
}

void XS::GameScene::showEvent( QShowEvent * event )
{
	QFrame::showEvent( event );

	// TODO: 
}

void XS::GameScene::closeEvent( QCloseEvent * event )
{
	// TODO: 

	QFrame::closeEvent( event );
}

void XS::GameScene::resizeEvent( QResizeEvent * event )
{
	QFrame::resizeEvent( event );

	// TODO: 
}
