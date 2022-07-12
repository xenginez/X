#include "LayoutDialog.h"

#include "ui_LayoutDialog.h"
#include "ui_SizeDialog.h"

XS::LayoutDialog::LayoutDialog( QWidget * parent /*= nullptr */ )
	:ui( new Ui::LayoutDialog )
{
	ui->setupUi( this );
	setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );

	ui->layout_1->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_1.png" ) );
	ui->layout_4->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_4.png" ) );
	ui->layout_9->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_9.png" ) );
	ui->layout_custom->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_custom.png" ) );

	connect( ui->layout_1, &QToolButton::clicked, [this]() { OnLayoutSize( { 1, 1 } ); } );
	connect( ui->layout_4, &QToolButton::clicked, [this]() { OnLayoutSize( { 2, 2 } ); } );
	connect( ui->layout_9, &QToolButton::clicked, [this]() { OnLayoutSize( { 3, 3 } ); } );
	connect( ui->layout_custom, &QToolButton::clicked, [this]() { OnLayoutCustomSize(); } );
}

XS::LayoutDialog::~LayoutDialog()
{

}

void XS::LayoutDialog::OnLayoutSize( QSize size )
{

}

void XS::LayoutDialog::OnLayoutCustomSize()
{
	QDialog dialog( this );
	Ui::SizeDialog ui;
	ui.setupUi( &dialog );
	dialog.setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );

	if ( dialog.exec() == QDialog::Accepted )
	{
		_Size.setWidth( ui.col->value() );
		_Size.setHeight( ui.row->value() );

		// color comp
		//abs( r1 * r1 - r2 * r2 ) * 0.299 + abs( g1 * g1 - g2 * g2 ) + 0.587 + abs( b1 * b1 - b2 * b2 ) + 0.114
	}
}
