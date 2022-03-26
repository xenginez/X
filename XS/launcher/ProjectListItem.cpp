#include "ProjectListItem.h"

#include "ui_ProjectListItem.h"

#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <QDateTime>
#include <QFileInfo>
#include <QListWidget>
#include <QResizeEvent>

Launcher::ProjectListItem::ProjectListItem( QListWidgetItem * item, QWidget * parent /*= nullptr */ )
	:QWidget( parent ), ui( new Ui::ProjectListItem ), item( item )
{
	ui->setupUi( this );

	ui->fixed->setAttribute( Qt::WA_Hover );
	ui->frame->setAttribute( Qt::WA_Hover );

	QString path = item->data( Qt::UserRole + 1 ).value<QString>();

	QDir dir( path );

	ui->path->setText( path );
	ui->name->setText( dir.dirName() );
	ui->time->setText( item->data( Qt::UserRole + 2 ).toDateTime().toString( Qt::LocalDate ) );

	ui->icon->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_item.png" ) );
	ui->fixed->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_unfixed.png" ) );

	if( item->data( Qt::UserRole + 3 ).toBool() )
	{
		ui->fixed->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_fixed.png" ) );
	}
	else
	{
		ui->fixed->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_unfixed.png" ) );
	}

	ui->fixed->installEventFilter( this );
	ui->frame->installEventFilter( this );
}

Launcher::ProjectListItem::~ProjectListItem()
{
	delete ui;
}

bool Launcher::ProjectListItem::eventFilter( QObject * obj, QEvent * event )
{
	if( obj == ui->fixed )
	{
		if( event->type() == QEvent::HoverEnter )
		{
			static_cast<QWidget *>( obj )->setStyleSheet( "background-color:palette(alternate-base);" );
		}
		else if( event->type() == QEvent::HoverLeave )
		{
			static_cast<QWidget *>( obj )->setStyleSheet( "background-color:none;" );
		}
		else if( event->type() == QEvent::MouseButtonPress )
		{
			static_cast<QWidget *>( obj )->setStyleSheet( "background-color:palette(highlight);" );
		}
		else if( event->type() == QEvent::MouseButtonRelease )
		{
			static_cast<QWidget *>( obj )->setStyleSheet( "background-color:none;" );

			if( item->data( Qt::UserRole + 3 ).toBool() )
			{
				item->setData( Qt::UserRole + 3, false );
				ui->fixed->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_unfixed.png" ) );

				itemUnfixed( item );
			}
			else
			{
				item->setData( Qt::UserRole + 3, true );
				ui->fixed->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_fixed.png" ) );

				itemFixed( item );
			}
		}
	}
	else if( obj == ui->frame )
	{
		if( event->type() == QEvent::HoverEnter )
		{
			ui->frame->setStyleSheet( "background-color:palette(highlight);" );
		}
		if( event->type() == QEvent::HoverLeave )
		{
			ui->frame->setStyleSheet( "background-color:none;" );
		}
	}

	return QWidget::eventFilter( obj, event );
}
