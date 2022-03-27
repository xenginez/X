#include "AssetsDockWidget.h"

#include "ui_AssetsDockWidget.h"

#include <Skin.h>

REG_WIDGET( XS::AssetsDockWidget );

XS::AssetsDockWidget::AssetsDockWidget( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::AssetsDockWidget )
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

	ui->splitter->setStretchFactor( 0, 2 );
	ui->splitter->setStretchFactor( 1, 8 );
}

XS::AssetsDockWidget::~AssetsDockWidget()
{
	delete ui;
}

void XS::AssetsDockWidget::Save( QSettings & settings )
{
	DockWidget::Save( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter", ui->splitter->saveState() );
	}
	settings.endGroup();
}

void XS::AssetsDockWidget::Load( QSettings & settings )
{
	DockWidget::Load( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreState( settings.value( "splitter" ).toByteArray() );
	}
	settings.endGroup();
}
