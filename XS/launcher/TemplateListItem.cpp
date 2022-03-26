#include "TemplateListItem.h"

#include "ui_TemplateListItem.h"

Launcher::TemplateListItem::TemplateListItem( QListWidgetItem * item, QWidget * parent /*= nullptr */ )
	:QWidget( parent ), ui( new Ui::TemplateListItem ), item( item )
{
	ui->setupUi( this );

	ui->frame->setAttribute( Qt::WA_Hover );

	ui->download->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_create_download.png" ) );

	ui->icon->setPixmap( QPixmap( item->data( Qt::UserRole + 2 ).toString() ) );
	ui->name->setText( item->data( Qt::UserRole + 3 ).toString() );
	ui->tooltip->setText( item->data( Qt::UserRole + 4 ).toString() );
	if( !( item->data( Qt::UserRole + 5 ).toBool() ) ) ui->download->hide();

	ui->frame->installEventFilter( this );
	ui->download->installEventFilter( this );
}

Launcher::TemplateListItem::~TemplateListItem()
{
	delete ui;
}

bool Launcher::TemplateListItem::eventFilter( QObject * obj, QEvent * event )
{
	if( obj == ui->download && event->type() == QEvent::MouseButtonRelease )
	{
		downloadClicked( item );
	}
	else if( obj == ui->frame )
	{
		if ( item->isSelected() )
		{
			if( ui->frame->styleSheet() != "" )
			{
				ui->frame->setStyleSheet( "" );
			}
		}
		else if( event->type() == QEvent::HoverEnter )
		{
			ui->frame->setStyleSheet( "background-color:palette(highlight);" );
		}
		else if( event->type() == QEvent::HoverLeave )
		{
			ui->frame->setStyleSheet( "background-color:none;" );
		}
	}

	return QWidget::eventFilter( obj, event );
}
