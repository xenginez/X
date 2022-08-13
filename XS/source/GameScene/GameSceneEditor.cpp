#include "GameSceneEditor.h"

#include "ui_GameSceneEditor.h"

#include <QComboBox>

#include "LayoutDialog.h"

REG_WIDGET( XS::GameSceneEditor );

XS::GameSceneEditor::GameSceneEditor( QWidget * parent /*= nullptr */ )
	: XS::ToolEditorWindow( parent ), ui( new Ui::GameSceneEditor )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );

	ui->layout->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout.png" ) );
	connect( ui->layout, &QToolButton::clicked, this, &GameSceneEditor::OnLayoutClicked );
}

XS::GameSceneEditor::~GameSceneEditor()
{
	delete ui;
}

QIcon XS::GameSceneEditor::icon()
{
	return  QIcon( "SkinIcons:/images/icons/icon_game.png" );
}

QString XS::GameSceneEditor::name()
{
	return tr( "Game" );
}

void XS::GameSceneEditor::SaveLayout( QSettings & settings )
{
	XS::ToolEditorWindow::SaveLayout( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "layout_size", _LayoutSize );
		settings.beginWriteArray( "layouts", _Layouts.size() );
		for ( size_t i = 0; i < _Layouts.size(); i++ )
		{
			settings.setArrayIndex( i );
			settings.setValue( "rect", _Layouts[i] );
		}
		settings.endArray();
	}
	settings.endGroup();
}

void XS::GameSceneEditor::LoadLayout( QSettings & settings )
{
	XS::ToolEditorWindow::LoadLayout( settings );

	settings.beginGroup( objectName() );
	{
		if ( settings.contains( "layout_size" ) )
		{
			_LayoutSize = settings.value( "layout_size" ).value<QSize>();
			_Layouts.resize( settings.beginReadArray( "layouts" ) );
			for ( size_t i = 0; i < _Layouts.size(); i++ )
			{
				settings.setArrayIndex( i );
				_Layouts[i] = settings.value( "rect" ).value<QRect>();
			}
			settings.endArray();
		}
	}
	settings.endGroup();

	ReLayout();
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

void XS::GameSceneEditor::ReLayout()
{
	for ( auto it : _GameScenes )
	{
		it->deleteLater();
	}
	_GameScenes.clear();

	ui->gridLayout;

	for ( size_t i = 0; i < _Layouts.size(); i++ )
	{
		const auto & rect = _Layouts[i];
		XS::GameScene * scene = new XS::GameScene( this );

		ui->gridLayout->addWidget( scene, rect.y(), rect.x(), rect.height(), rect.width() );

		_GameScenes.push_back( scene );
	}
}
