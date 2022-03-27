#include "ObjectsDockWidget.h"

#include "ui_ObjectsDockWidget.h"

#include <Skin.h>

REG_WIDGET( XS::ObjectsDockWidget );

XS::ObjectsDockWidget::ObjectsDockWidget( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::ObjectsDockWidget )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_objects.png" ) );

	setTitleBar( new QWidget( this ) );

	ui->logic_icon->setPixmap( QPixmap( "SkinIcons:/images/objects/icon_objects_logic.png" ) );
	ui->logic_add->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_add.png" ) );
	ui->logic_type->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_type.png" ) );
	ui->logic_expand->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_expand.png" ) );
	ui->logic_collapse->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_collapse.png" ) );
	ui->logic_search->addAction( QIcon( "SkinIcons:/images/objects/icon_objects_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->render_icon->setPixmap( QPixmap( "SkinIcons:/images/objects/icon_objects_render.png" ) );
	ui->render_add->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_add.png" ) );
	ui->render_type->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_type.png" ) );
	ui->render_expand->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_expand.png" ) );
	ui->render_collapse->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_collapse.png" ) );
	ui->render_search->addAction( QIcon( "SkinIcons:/images/objects/icon_objects_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->inspector_icon->setPixmap( QPixmap( "SkinIcons:/images/objects/icon_objects_inspector.png" ) );

	ui->splitter_2->setStretchFactor( 0, 5 );
	ui->splitter_2->setStretchFactor( 1, 5 );

	connect( ui->logic_tree, &QTreeWidget::itemClicked, this, &ObjectsDockWidget::OnLogicTreeWidgetItemClicked );
	connect( ui->render_tree, &QTreeWidget::itemClicked, this, &ObjectsDockWidget::OnRenderTreeWidgetItemClicked );
}

XS::ObjectsDockWidget::~ObjectsDockWidget()
{
	delete ui;
}

void XS::ObjectsDockWidget::Save( QSettings & settings )
{
	DockWidget::Save( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter_horizontal", ui->splitter->saveState() );
		settings.setValue( "splitter_vertical", ui->splitter_2->saveState() );
	}
	settings.endGroup();
}

void XS::ObjectsDockWidget::Load( QSettings & settings )
{
	DockWidget::Load( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter_2->restoreState( settings.value( "splitter_vertical" ).toByteArray() );
		ui->splitter->restoreState( settings.value( "splitter_horizontal" ).toByteArray() );
	}
	settings.endGroup();
}

void XS::ObjectsDockWidget::OnLogicTreeWidgetItemClicked( QTreeWidgetItem * item, int column )
{
	ui->render_tree->setCurrentItem( nullptr );
	ui->inspector_name->setText( item->text( 0 ) );

}

void XS::ObjectsDockWidget::OnRenderTreeWidgetItemClicked( QTreeWidgetItem * item, int column )
{
	ui->logic_tree->setCurrentItem( nullptr );
	ui->inspector_name->setText( item->text( 0 ) );

}

void XS::ObjectsDockWidget::OnInspectorClicked()
{

}
