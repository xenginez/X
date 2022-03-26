#include "EditSceneDockWidget.h"

#include "ui_EditSceneDockWidget.h"

#include <Skin.h>

XS::EditSceneDockWidget::EditSceneDockWidget( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::EditSceneDockWidget )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_edit.png" ) );

	setTitleBar( ui->title_bar );

	ui->audio->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_audio.png" ) );
	ui->camera->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_camera.png" ) );
	ui->grid->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_grid.png" ) );
	ui->icons->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_icons.png" ) );
	ui->light->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_light.png" ) );
	ui->move->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_move.png" ) );
	ui->rotate->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_rotate.png" ) );
	ui->scale->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_scale.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/editscene/icon_editscene_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	connect( ui->audio, &QToolButton::clicked, this, &EditSceneDockWidget::OnAudioClicked );
	connect( ui->camera, &QToolButton::clicked, this, &EditSceneDockWidget::OnCameraClicked );
	connect( ui->icons, &QToolButton::clicked, this, &EditSceneDockWidget::OnIconsClicked );
	connect( ui->light, &QToolButton::clicked, this, &EditSceneDockWidget::OnLightClicked );
	connect( ui->move, &QToolButton::clicked, this, &EditSceneDockWidget::OnMoveClicked );
	connect( ui->rotate, &QToolButton::clicked, this, &EditSceneDockWidget::OnRotateClicked );
	connect( ui->scale, &QToolButton::clicked, this, &EditSceneDockWidget::OnScaleClicked );
	connect( ui->search, &QLineEdit::editingFinished, this, &EditSceneDockWidget::OnSearchFinished );
}

XS::EditSceneDockWidget::~EditSceneDockWidget()
{
	delete ui;
}

WId XS::EditSceneDockWidget::display() const
{
	return ui->display->winId();
}

void XS::EditSceneDockWidget::OnAudioClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnCameraClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnGridClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnIconsClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnLightClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnMoveClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnRotateClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnScaleClicked( bool checked /*= false */ )
{

}

void XS::EditSceneDockWidget::OnSearchFinished()
{

}
