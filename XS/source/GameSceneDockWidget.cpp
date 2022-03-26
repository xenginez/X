#include "GameSceneDockWidget.h"

#include "ui_GameSceneDockWidget.h"

#include <Skin.h>

XS::GameSceneDockWidget::GameSceneDockWidget( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::GameSceneDockWidget )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_game.png" ) );

	setTitleBar( ui->title_bar );

	ui->fps->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_fps.png" ) );
	ui->audio->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_audio.png" ) );
	ui->tools->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_tools.png" ) );

	connect( ui->fps, &QToolButton::clicked, this, &GameSceneDockWidget::OnFpsClicked );
	connect( ui->audio, &QToolButton::clicked, this, &GameSceneDockWidget::OnAudioClicked );
	connect( ui->tools, &QToolButton::clicked, this, &GameSceneDockWidget::OnToolsClicked );
	connect( ui->ratio, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &GameSceneDockWidget::OnRatioCurrentIndexChanged );
	connect( ui->camera, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &GameSceneDockWidget::OnCameraCurrentIndexChanged );
}

XS::GameSceneDockWidget::~GameSceneDockWidget()
{
	delete ui;
}

WId XS::GameSceneDockWidget::display() const
{
	return ui->display->winId();
}

void XS::GameSceneDockWidget::OnFpsClicked( bool checked /*= false */ )
{

}

void XS::GameSceneDockWidget::OnAudioClicked( bool checked /*= false */ )
{

}

void XS::GameSceneDockWidget::OnToolsClicked( bool checked /*= false */ )
{

}

void XS::GameSceneDockWidget::OnRatioCurrentIndexChanged( int index )
{

}

void XS::GameSceneDockWidget::OnCameraCurrentIndexChanged( int index )
{

}
