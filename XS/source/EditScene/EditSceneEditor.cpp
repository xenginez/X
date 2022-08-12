#include "EditSceneEditor.h"

#include "ui_EditSceneEditor.h"

#include <QMenu>

REG_WIDGET( XS::EditSceneEditor );

XS::EditSceneEditor::EditSceneEditor( QWidget * parent /*= nullptr */ )
	: XS::EditorWindow( parent ), ui( new Ui::EditSceneEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_edit.png" ) );

	setTitleBar( ui->title_bar );

	ui->tools->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_tools.png" ) );
	ui->move->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_move.png" ) );
	ui->scale->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_scale.png" ) );
	ui->rotate->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_rotate.png" ) );

	connect( ui->d23, &QToolButton::clicked, this, &EditSceneEditor::OnD23Clicked );
	connect( ui->tools, &QToolButton::clicked, this, &EditSceneEditor::OnToolsClicked );
	connect( ui->move, &QToolButton::clicked, this, &EditSceneEditor::OnTransformClicked );
	connect( ui->scale, &QToolButton::clicked, this, &EditSceneEditor::OnTransformClicked );
	connect( ui->rotate, &QToolButton::clicked, this, &EditSceneEditor::OnTransformClicked );

	ui->display->installEventFilter( this );
}

XS::EditSceneEditor::~EditSceneEditor()
{
	delete ui;
}

void XS::EditSceneEditor::SaveLayout( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		settings.setValue( "2d_3d", ui->d23->text() );

		int checked = 0;
		if ( ui->move->isChecked() ) checked = 1;
		else if ( ui->scale->isChecked() ) checked = 2;
		else if ( ui->rotate->isChecked() ) checked = 3;
		settings.setValue( "transform_checked", checked );
	}
	settings.endGroup();
}

void XS::EditSceneEditor::LoadLayout( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		ui->d23->setText( settings.value( "2d_3d", "2D" ).toString() );
		switch ( settings.value( "transform_checked", 0 ).toInt() )
		{
		case 1: ui->move->setChecked( true ); break;
		case 2: ui->scale->setChecked( true ); break;
		case 3: ui->rotate->setChecked( true ); break;
		}
	}
	settings.endGroup();
}

void XS::EditSceneEditor::OnD23Clicked( bool checked /*= false */ )
{
	ui->d23->setText( ( ui->d23->text() == "2D" ) ? "3D" : "2D" );
}

void XS::EditSceneEditor::OnToolsClicked( bool checked /*= false */ )
{
	QMenu menu( this );


	menu.exec( cursor().pos() );
}

void XS::EditSceneEditor::OnTransformClicked( bool checked /*= false */ )
{
	if ( sender() != ui->move ) ui->move->setChecked( false );
	if ( sender() != ui->scale ) ui->scale->setChecked( false );
	if ( sender() != ui->rotate ) ui->rotate->setChecked( false );
}

bool XS::EditSceneEditor::eventFilter( QObject * watched, QEvent * event )
{
	if ( watched == ui->display )
	{


		//ui->display->repaint();
	}

	return XS::DockWidget::eventFilter( watched, event );
}
