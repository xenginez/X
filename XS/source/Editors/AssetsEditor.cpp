#include "AssetsEditor.h"

#include "ui_AssetsEditor.h"

#include <QSqlError>
#include <QSqlQuery>

#include "CoreFramework.h"

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

	auto project_path = QString::fromStdString( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string() );

	_Watcher.addPath( project_path );

	_LocalDB = QSqlDatabase::addDatabase( "QSQLITE" );
	_LocalDB.setHostName( project_path + "/FileWatcher.db" );
	_LocalDB.open();

	connect( &_Watcher, &QFileSystemWatcher::fileChanged, this, &AssetsEditor::OnWatcherFileChanged );
	connect( &_Watcher, &QFileSystemWatcher::directoryChanged, this, &AssetsEditor::OnWatcherDirectoryChanged );
}

XS::AssetsEditor::~AssetsEditor()
{
	if ( _LocalDB.isOpen() )
	{
		_LocalDB.close();
	}
	if ( _RemoteDB.isOpen() )
	{
		_RemoteDB.close();
	}

	delete ui;
}

void XS::AssetsEditor::Save( QSettings & settings )
{
	DockWidget::Save( settings );

	settings.beginGroup( objectName() );
	{
		if ( _RemoteDB.isOpen() )
		{
			QString data = _RemoteDB.databaseName() + ":" + _RemoteDB.hostName() + ":" + QString::number( _RemoteDB.port() ) + ":" + _RemoteDB.userName() + ":" + _RemoteDB.password();
			settings.setValue( "remote_db", data.toLocal8Bit().toBase64() );
		}

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

		QByteArray base64 = settings.value( "remote_db", QByteArray() ).toByteArray();
		if ( !base64.isEmpty() )
		{
			QString data = QByteArray::fromBase64( base64 );
			auto list = data.split( ":" );
			_RemoteDB = QSqlDatabase::addDatabase( list[0] );
			_RemoteDB.setHostName( list[1] );
			_RemoteDB.setPort( list[2].toInt() );
			_RemoteDB.open( list[3], list[4] );
		}
	}
	settings.endGroup();
}

void XS::AssetsEditor::OnWatcherFileChanged( const QString & path )
{

}

void XS::AssetsEditor::OnWatcherDirectoryChanged( const QString & path )
{

}
