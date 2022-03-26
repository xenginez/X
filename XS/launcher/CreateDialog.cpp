#include "CreateDialog.h"

#include "ui_CreateDialog.h"

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "TemplateListItem.h"

Launcher::CreateDialog::CreateDialog( QWidget * parent /*= nullptr */ )
	:QDialog( parent ), ui( new Ui::CreateDialog )
{
	ui->setupUi( this );

	QDir dir( "./engines" );
	if( dir.exists() )
	{
		QStringList list = dir.entryList( QDir::Filter::Dirs | QDir::NoDotAndDotDot, QDir::SortFlag::Name );

		ui->version_comboBox->addItems( list );

		ui->version_comboBox->setCurrentIndex( list.size() - 1 );
	}

	QTimer::singleShot( 10, [this]() { FlushTemplate(); } );

	connect( ui->dir_button, &QToolButton::clicked, this, &CreateDialog::OnDirButtonClicked );
	connect( ui->cancel_button, &QPushButton::clicked, this, &CreateDialog::OnCancelButtonClicked );
	connect( ui->confirm_button, &QPushButton::clicked, this, &CreateDialog::OnConfirmButtonClicked );
}

Launcher::CreateDialog::~CreateDialog()
{
	delete ui;
}

QString Launcher::CreateDialog::dir() const
{
	return ui->dir_lineEdit->text();
}

void Launcher::CreateDialog::OnDirButtonClicked()
{
	QString path = QFileDialog::getExistingDirectory( this, "create project", QDir::homePath() );
	if( path != "" )
	{
		ui->dir_lineEdit->setText( path );
	}
}

void Launcher::CreateDialog::OnCancelButtonClicked()
{
	reject();
}

void Launcher::CreateDialog::OnConfirmButtonClicked()
{
	if( ui->name_lineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this, "error", "name is empty!" );
		return;
	}
	if( ui->dir_lineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this, "error", "save path is empty!" );
		return;
	}

	CreateProject();

	accept();
}

void Launcher::CreateDialog::CreateProject()
{

}

void Launcher::CreateDialog::FlushTemplate()
{
	ui->template_list->clear();

	{
		QListWidgetItem * item = new QListWidgetItem( ui->template_list );
		item->setData( Qt::UserRole + 1, "" ); // url
		item->setData( Qt::UserRole + 2, "SkinIcons:/images/welcome/icon_welcome_create_template_2d.png" ); // icon
		item->setData( Qt::UserRole + 3, "2D" ); // name
		item->setData( Qt::UserRole + 4, "2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d" ); // tooltip
		item->setData( Qt::UserRole + 5, true ); // download

		TemplateListItem * custom = new TemplateListItem( item, ui->template_list );

		item->setSizeHint( QSize( 0, custom->sizeHint().height() ) );

		ui->template_list->addItem( item );
		ui->template_list->setItemWidget( item, custom );

		connect( custom, &TemplateListItem::downloadClicked, this, &CreateDialog::DownloadTemplate );
	}
	{
		QListWidgetItem * item = new QListWidgetItem( ui->template_list );
		item->setData( Qt::UserRole + 1, "" ); // url
		item->setData( Qt::UserRole + 2, "SkinIcons:/images/welcome/icon_welcome_create_template_3d.png" ); // icon
		item->setData( Qt::UserRole + 3, "3D" ); // name
		item->setData( Qt::UserRole + 4, "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d" ); // tooltip
		item->setData( Qt::UserRole + 5, false ); // download

		TemplateListItem * custom = new TemplateListItem( item, ui->template_list );

		item->setSizeHint( QSize( 0, custom->sizeHint().height() ) );

		ui->template_list->addItem( item );
		ui->template_list->setItemWidget( item, custom );

		connect( custom, &TemplateListItem::downloadClicked, this, &CreateDialog::DownloadTemplate );
	}
	{
		QListWidgetItem * item = new QListWidgetItem( ui->template_list );
		item->setData( Qt::UserRole + 1, "" ); // url
		item->setData( Qt::UserRole + 2, "SkinIcons:/images/welcome/icon_welcome_create_template_2d.png" ); // icon
		item->setData( Qt::UserRole + 3, "2D" ); // name
		item->setData( Qt::UserRole + 4, "2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d" ); // tooltip
		item->setData( Qt::UserRole + 5, false ); // download

		TemplateListItem * custom = new TemplateListItem( item, ui->template_list );

		item->setSizeHint( QSize( 0, custom->sizeHint().height() ) );

		ui->template_list->addItem( item );
		ui->template_list->setItemWidget( item, custom );

		connect( custom, &TemplateListItem::downloadClicked, this, &CreateDialog::DownloadTemplate );
	}
	{
		QListWidgetItem * item = new QListWidgetItem( ui->template_list );
		item->setData( Qt::UserRole + 1, "" ); // url
		item->setData( Qt::UserRole + 2, "SkinIcons:/images/welcome/icon_welcome_create_template_3d.png" ); // icon
		item->setData( Qt::UserRole + 3, "3D" ); // name
		item->setData( Qt::UserRole + 4, "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d" ); // tooltip
		item->setData( Qt::UserRole + 5, true ); // download

		TemplateListItem * custom = new TemplateListItem( item, ui->template_list );

		item->setSizeHint( QSize( 0, custom->sizeHint().height() ) );

		ui->template_list->addItem( item );
		ui->template_list->setItemWidget( item, custom );

		connect( custom, &TemplateListItem::downloadClicked, this, &CreateDialog::DownloadTemplate );
	}
}

void Launcher::CreateDialog::DownloadTemplate( QListWidgetItem * item )
{
	qDebug() << "download";
}
