#include "WorldEditor.h"

#include "ui_WorldEditor.h"

#include <QTimer>

REG_WIDGET( XS::WorldEditor );

XS::WorldEditor::WorldEditor( QWidget * parent /*= nullptr */ )
	: XS::ToolEditorWindow( parent ), ui( new Ui::WorldEditor )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( new QWidget( this ) );

	ui->logic_icon->setPixmap( QPixmap( "SkinIcons:/images/world/icon_world_logic.png" ) );
	ui->logic_sys->setIcon( QIcon( "SkinIcons:/images/world/icon_world_system.png" ) );
	ui->logic_add->setIcon( QIcon( "SkinIcons:/images/world/icon_world_add.png" ) );
	ui->logic_type->setIcon( QIcon( "SkinIcons:/images/world/icon_world_type.png" ) );
	ui->logic_expand->setIcon( QIcon( "SkinIcons:/images/world/icon_world_expand.png" ) );
	ui->logic_collapse->setIcon( QIcon( "SkinIcons:/images/world/icon_world_collapse.png" ) );
	ui->logic_search->addAction( QIcon( "SkinIcons:/images/world/icon_world_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->render_icon->setPixmap( QPixmap( "SkinIcons:/images/world/icon_world_render.png" ) );
	ui->render_add->setIcon( QIcon( "SkinIcons:/images/world/icon_world_add.png" ) );
	ui->render_type->setIcon( QIcon( "SkinIcons:/images/world/icon_world_type.png" ) );
	ui->render_expand->setIcon( QIcon( "SkinIcons:/images/world/icon_world_expand.png" ) );
	ui->render_collapse->setIcon( QIcon( "SkinIcons:/images/world/icon_world_collapse.png" ) );
	ui->render_search->addAction( QIcon( "SkinIcons:/images/world/icon_world_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->inspector_icon->setPixmap( QPixmap( "SkinIcons:/images/world/icon_world_inspector.png" ) );
	ui->inspector_expand->setIcon( QIcon( "SkinIcons:/images/world/icon_world_expand.png" ) );
	ui->inspector_collapse->setIcon( QIcon( "SkinIcons:/images/world/icon_world_collapse.png" ) );

	connect( ui->logic_sys, &QToolButton::clicked, this, &WorldEditor::OnLogicSystemClicked );
	connect( ui->logic_tree, &QTreeWidget::itemClicked, this, &WorldEditor::OnLogicTreeWidgetItemClicked );
	connect( ui->render_tree, &QTreeWidget::itemClicked, this, &WorldEditor::OnRenderTreeWidgetItemClicked );

	connect( ui->inspector_expand, &QToolButton::clicked, [this]() { if ( _Inspector != nullptr ) _Inspector->Expand(); } );
	connect( ui->inspector_collapse, &QToolButton::clicked, [this]() { if ( _Inspector != nullptr ) _Inspector->Collapse(); } );

	ui->splitter->setSizes( { 10000, 10000 } );
	ui->splitter_2->setSizes( { 10000, 10000 } );
}

XS::WorldEditor::~WorldEditor()
{
	delete ui;
}

QIcon XS::WorldEditor::icon()
{
	return QIcon( "SkinIcons:/images/icons/icon_world.png" );
}

QString XS::WorldEditor::name()
{
	return tr( "World" );
}

void XS::WorldEditor::SaveLayout( QSettings & settings )
{
	XS::ToolEditorWindow::SaveLayout( settings );
	
	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter_horizontal_state", ui->splitter->saveState() );
		settings.setValue( "splitter_vertical_state", ui->splitter_2->saveState() );
	}
	settings.endGroup();
}

void XS::WorldEditor::LoadLayout( QSettings & settings )
{
	XS::ToolEditorWindow::LoadLayout( settings );
	
	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreState( settings.value( "splitter_horizontal_state" ).toByteArray() );
		ui->splitter_2->restoreState( settings.value( "splitter_vertical_state" ).toByteArray() );
	}
	settings.endGroup();
}

void XS::WorldEditor::OnLogicSystemClicked()
{

}

void XS::WorldEditor::OnLogicTreeWidgetItemClicked( QTreeWidgetItem * item, int column )
{
	ui->render_tree->setCurrentItem( nullptr );
	ui->inspector_name->setText( item->text( 0 ) );

}

void XS::WorldEditor::OnRenderTreeWidgetItemClicked( QTreeWidgetItem * item, int column )
{
	ui->logic_tree->setCurrentItem( nullptr );
	ui->inspector_name->setText( item->text( 0 ) );

}

void XS::WorldEditor::OnInspectorClicked( const XE::Variant & val )
{
	if ( ui->inspector_layout->count() != 0 )
	{
		ui->inspector_layout->removeItem( ui->inspector_layout->itemAt( 0 ) );
	}

	_Inspector = XS::Inspector::Create( new XS::VariantObjectProxy( val ), this );

	ui->inspector_layout->addWidget( _Inspector );
}
