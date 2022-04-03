#include "EditSceneEditor.h"

#include "ui_EditSceneEditor.h"

REG_WIDGET( XS::EditSceneEditor );

XS::EditSceneEditor::EditSceneEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::EditSceneEditor )
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

	connect( ui->audio, &QToolButton::clicked, this, &EditSceneEditor::OnAudioClicked );
	connect( ui->camera, &QToolButton::clicked, this, &EditSceneEditor::OnCameraClicked );
	connect( ui->icons, &QToolButton::clicked, this, &EditSceneEditor::OnIconsClicked );
	connect( ui->light, &QToolButton::clicked, this, &EditSceneEditor::OnLightClicked );
	connect( ui->move, &QToolButton::clicked, this, &EditSceneEditor::OnMoveClicked );
	connect( ui->rotate, &QToolButton::clicked, this, &EditSceneEditor::OnRotateClicked );
	connect( ui->scale, &QToolButton::clicked, this, &EditSceneEditor::OnScaleClicked );
	connect( ui->search, &QLineEdit::editingFinished, this, &EditSceneEditor::OnSearchFinished );
}

XS::EditSceneEditor::~EditSceneEditor()
{
	delete ui;
}

WId XS::EditSceneEditor::display() const
{
	return ui->display->winId();
}

void XS::EditSceneEditor::OnAudioClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnCameraClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnGridClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnIconsClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnLightClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnMoveClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnRotateClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnScaleClicked( bool checked /*= false */ )
{

}

void XS::EditSceneEditor::OnSearchFinished()
{

}
