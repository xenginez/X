#include "AssetsEditor.h"

#include "ui_AssetsEditor.h"

REG_WIDGET( XS::AssetsEditor );

XS::AssetsEditor::AssetsEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::AssetsEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_assets.png" ) );

	setTitleBar( ui->title_bar );

	ui->add->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_add.png" ) );
	ui->type->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_type.png" ) );
	ui->label->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_label.png" ) );
	ui->invisible->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_invisible.png" ) );
	ui->icon->setPixmap( QPixmap( "SkinIcons:/images/assets/icon_assets_folder.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/assets/icon_assets_search.png" ), QLineEdit::ActionPosition::LeadingPosition );
}

XS::AssetsEditor::~AssetsEditor()
{
	delete ui;
}

void XS::AssetsEditor::Save( QSettings & settings )
{
	DockWidget::Save( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter_geometry", ui->splitter->saveGeometry() );
		settings.setValue( "splitter_state", ui->splitter->saveState() );
	}
	settings.endGroup();
}

void XS::AssetsEditor::Load( QSettings & settings )
{
	DockWidget::Load( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreGeometry( settings.value( "splitter_geometry" ).toByteArray() );
		ui->splitter->restoreState( settings.value( "splitter_state" ).toByteArray() );
	}
	settings.endGroup();
}
