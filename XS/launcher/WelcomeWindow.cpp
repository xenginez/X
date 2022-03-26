#include "WelcomeWindow.h"
#include "ui_WelcomeWindow.h"

#include <map>
#include <vector>

#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QDateTime>
#include <QFileInfo>
#include <QSettings>
#include <QJsonArray>
#include <QMouseEvent>
#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>

#include "CloneDialog.h"
#include "CreateDialog.h"
#include "ProjectListItem.h"

Launcher::WelcomeWindow::WelcomeWindow( QWidget * parent /*= nullptr */ )
	:ui( new Ui::WelcomeWindow )
{
	ui->setupUi( this );

	ui->open_frame->setAttribute( Qt::WA_Hover );
	ui->clone_frame->setAttribute( Qt::WA_Hover );
	ui->create_frame->setAttribute( Qt::WA_Hover );

	ui->open_frame->installEventFilter( this );
	ui->clone_frame->installEventFilter( this );
	ui->create_frame->installEventFilter( this );

	ui->title->setPixmap( QPixmap( "SkinIcons:/images/common/icon_title.png" ) );
	ui->open_icon->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_open.png" ) );
	ui->clone_icon->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_clone.png" ) );
	ui->create_icon->setPixmap( QPixmap( "SkinIcons:/images/welcome/icon_welcome_create.png" ) );

	ReadProjectList();

	connect( ui->project_list, QOverload<QListWidgetItem *>::of( &QListWidget::itemDoubleClicked ), this, &WelcomeWindow::OnProjectListItemDoubleClicked );
}

Launcher::WelcomeWindow::~WelcomeWindow()
{
	WriteProjectList();

	delete ui;
}

bool Launcher::WelcomeWindow::eventFilter( QObject * obj, QEvent * event )
{
	if( obj == ui->open_frame || obj == ui->clone_frame || obj == ui->create_frame )
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

			if( obj == ui->open_frame )
			{
				OnOpenClicked();
			}
			else if( obj == ui->clone_frame )
			{
				OnCloneClicked();
			}
			else if( obj == ui->create_frame )
			{
				OnCreateClicked();
			}
		}
	}

	return QWidget::eventFilter( obj, event );
}

void Launcher::WelcomeWindow::OnOpenClicked()
{
	QString path = QFileDialog::getExistingDirectory( this, "open x studio project", QDir::homePath() );

	if ( !path.isEmpty() )
	{
		OpenProject( path );
	}
}

void Launcher::WelcomeWindow::OnCloneClicked()
{
	CloneDialog dialog( this );

	if ( dialog.exec() == QDialog::Accepted )
	{
		OpenProject( dialog.dir() );
	}
}

void Launcher::WelcomeWindow::OnCreateClicked()
{
	CreateDialog dialog( this );

	if( dialog.exec() == QDialog::Accepted )
	{
		OpenProject( dialog.dir() );
	}
}

void Launcher::WelcomeWindow::OnProjectListItemFixed( QListWidgetItem * item )
{
	QString path = item->data( Qt::UserRole + 1 ).toString();
	{
		auto it = _Projects.find( path );
		if( it != _Projects.end() )
		{
			auto obj = it->toObject();
			obj["fixed"] = item->data( Qt::UserRole + 3 ).toBool();
			_Projects[path] = obj;
		}
	}
	QTimer::singleShot( 10, [this]() { FlushProjectList(); } );
}

void Launcher::WelcomeWindow::OnProjectListItemDoubleClicked( QListWidgetItem * item )
{
	OpenProject( item->data( Qt::UserRole + 1 ).value<QString>() );
}

void Launcher::WelcomeWindow::OpenProject( const QString & path )
{
	bool exists = false;

	for( int i = 0; i < ui->project_list->count(); i++ )
	{
		auto item = ui->project_list->item( i );
		if( item->data( Qt::UserRole + 1 ).toString() == path )
		{
			auto it = _Projects.find( path );
			if( it != _Projects.end() )
			{
				auto obj = it->toObject();
				obj["time"] = QDateTime::currentDateTime().toString( Qt::ISODate );
				_Projects[path] = obj;
			}

			exists = true;
			break;
		}
	}

	if( !exists )
	{
		QJsonObject object;
		object["time"] = QDateTime::currentDateTime().toString( Qt::ISODate );
		object["fixed"] = false;

		_Projects[path] = object;
	}

	QTimer::singleShot( 10, [this]() { FlushProjectList(); } );

	QProcess process;

	QString cmd = "./XStudio.exe " + path;

	process.startDetached( cmd );

	QTimer::singleShot( 1000, []() { QApplication::exit(); } );
}

void Launcher::WelcomeWindow::ReadProjectList()
{
	QFile file( "./launcher.conf" );

	file.open( QIODevice::ReadOnly );

	_Projects = QJsonDocument::fromJson( file.readAll() ).object();

	FlushProjectList();
}

void Launcher::WelcomeWindow::WriteProjectList()
{
	QJsonObject projects;

	for( int i = 0; i < ui->project_list->count(); i++ )
	{
		QJsonObject object;

		auto item = ui->project_list->item( i );

		object["time"] = item->data( Qt::UserRole + 2 ).toDateTime().toString( Qt::ISODate );
		object["fixed"] = item->data( Qt::UserRole + 3 ).toBool();

		projects.insert( item->data( Qt::UserRole + 1 ).toString(), object );
	}

	QJsonDocument doc;

	doc.setObject( projects );

	QFile file( "./launcher.conf" );

	file.open( QIODevice::ReadWrite | QIODevice::Truncate );

	file.write( doc.toJson( QJsonDocument::Indented ) );
}

void Launcher::WelcomeWindow::FlushProjectList()
{
	ui->project_list->clear();

	std::vector<std::tuple<QString, QDateTime, bool>> items;
	for( auto it = _Projects.begin(); it != _Projects.end(); it++ )
	{
		QJsonObject object = it.value().toObject();

		QString path = it.key();
		QDateTime time = QDateTime::fromString( object["time"].toString(), Qt::ISODate );
		bool fixed = object["fixed"].toBool();
		if( QFileInfo::exists( path ) )
		{
			items.push_back( { path, time, fixed } );
		}
		else
		{
			_Projects.remove( path );
		}
	}

	std::sort( items.begin(), items.end(), []( const auto & left, const auto & right )
	{
		if( std::get<2>( left ) == std::get<2>( right ) )
		{
			return std::get<1>( left ) > std::get<1>( right );
		}

		return std::get<2>( left );
	} );

	for( int i = 0; i < items.size(); i++ )
	{
		QListWidgetItem * item = new QListWidgetItem( ui->project_list );
		item->setData( Qt::UserRole + 1, std::get<0>( items[i] ) );
		item->setData( Qt::UserRole + 2, std::get<1>( items[i] ) );
		item->setData( Qt::UserRole + 3, std::get<2>( items[i] ) );

		ProjectListItem * custom = new ProjectListItem( item, ui->project_list );
		item->setSizeHint( QSize( 0, custom->sizeHint().height() ) );
		item->setFlags( item->flags() & ( ~Qt::ItemIsSelectable ) );

		ui->project_list->addItem( item );
		ui->project_list->setItemWidget( item, custom );

		connect( custom, &ProjectListItem::itemFixed, this, &WelcomeWindow::OnProjectListItemFixed );
		connect( custom, &ProjectListItem::itemUnfixed, this, &WelcomeWindow::OnProjectListItemFixed );
	}
}
