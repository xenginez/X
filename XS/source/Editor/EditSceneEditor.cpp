#include "EditSceneEditor.h"

#include "ui_EditSceneEditor.h"

#include <QMenu>

REG_WIDGET( XS::EditSceneEditor );

XS::EditSceneEditor::EditSceneEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::EditSceneEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_edit.png" ) );

	setTitleBar( ui->title_bar );

	ui->icons->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_icons.png" ) );
	ui->move->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_move.png" ) );
	ui->scale->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_scale.png" ) );
	ui->rotate->setIcon( QIcon( "SkinIcons:/images/editscene/icon_editscene_rotate.png" ) );

	connect( ui->d23, &QToolButton::clicked, this, &EditSceneEditor::OnD23Clicked );
	connect( ui->icons, &QToolButton::clicked, this, &EditSceneEditor::OnIconsClicked );
	connect( ui->move, &QToolButton::clicked, this, &EditSceneEditor::OnTransformClicked );
	connect( ui->scale, &QToolButton::clicked, this, &EditSceneEditor::OnTransformClicked );
	connect( ui->rotate, &QToolButton::clicked, this, &EditSceneEditor::OnTransformClicked );
}

XS::EditSceneEditor::~EditSceneEditor()
{
	delete ui;
}

WId XS::EditSceneEditor::display() const
{
	return ui->display->winId();
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

void XS::EditSceneEditor::OnIconsClicked( bool checked /*= false */ )
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

void XS::EditSceneEditor::paintEvent( QPaintEvent * event )
{
	XS::DockWidget::paintEvent( event );

	if ( ui->d23->text() == "2D" )
	{

	}
	else
	{

	}

}
