#include "GameSceneEditor.h"

#include "ui_GameSceneEditor.h"

#include <QComboBox>

#include "LayoutDialog.h"

REG_WIDGET( XS::GameSceneEditor );

XS::GameSceneEditor::GameSceneEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::GameSceneEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_game.png" ) );

	setTitleBar( ui->title_bar );

	ui->layout->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout.png" ) );
	connect( ui->layout, &QToolButton::clicked, this, &GameSceneEditor::OnLayoutClicked );
}

XS::GameSceneEditor::~GameSceneEditor()
{
	delete ui;
}

void XS::GameSceneEditor::OnLayoutClicked( bool checked /*= false */ )
{
	XS::LayoutDialog dialog( _LayoutSize, this );

	if ( dialog.exec() == QDialog::Accepted )
	{
		_Layouts = dialog.GetRebuildRects();
		_LayoutSize = dialog.GetRebuildSize();

		ReLayout();
	}
}

XS::GameScene * XS::GameSceneEditor::CreateGameScene()
{
	XS::GameScene * scene = new XS::GameScene( QString( "view_%1" ).arg( _GameScenes.size() ), this );

	_GameScenes.push_back( scene );

	return scene;
}

void XS::GameSceneEditor::ReLayout()
{

}
