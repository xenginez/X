#include "GameSceneEditor.h"

#include "ui_GameSceneEditor.h"

REG_WIDGET( XS::GameSceneEditor );

XS::GameSceneEditor::GameSceneEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::GameSceneEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_game.png" ) );

	setTitleBar( ui->title_bar );

	ui->fps->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_fps.png" ) );
	ui->audio->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_audio.png" ) );
	ui->tools->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_tools.png" ) );

	connect( ui->fps, &QToolButton::clicked, this, &GameSceneEditor::OnFpsClicked );
	connect( ui->audio, &QToolButton::clicked, this, &GameSceneEditor::OnAudioClicked );
	connect( ui->tools, &QToolButton::clicked, this, &GameSceneEditor::OnToolsClicked );
	connect( ui->ratio, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &GameSceneEditor::OnRatioCurrentIndexChanged );
	connect( ui->camera, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &GameSceneEditor::OnCameraCurrentIndexChanged );
}

XS::GameSceneEditor::~GameSceneEditor()
{
	delete ui;
}

WId XS::GameSceneEditor::display() const
{
	return ui->display->winId();
}

void XS::GameSceneEditor::OnFpsClicked( bool checked /*= false */ )
{

}

void XS::GameSceneEditor::OnAudioClicked( bool checked /*= false */ )
{

}

void XS::GameSceneEditor::OnToolsClicked( bool checked /*= false */ )
{

}

void XS::GameSceneEditor::OnRatioCurrentIndexChanged( int index )
{

}

void XS::GameSceneEditor::OnCameraCurrentIndexChanged( int index )
{

}
