#include "AssetDatabase.h"

#include <QCache>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#include "CoreFramework.h"

struct XS::AssetDatabase::Private
{
	Private()
		: _Cache( 1000 )
	{}

	QSqlDatabase _Database;
	QCache< QUuid, QPair<QFileInfo, QJsonDocument > > _Cache;
};

XS::AssetDatabase::AssetDatabase()
	:_p( XE::New< Private>() )
{

}

XS::AssetDatabase::~AssetDatabase()
{
	XE::Delete( _p );
}

bool XS::AssetDatabase::Open( const QUrl & url )
{
	_p->_Database = QSqlDatabase::addDatabase( url.scheme().toUpper() );
	{
		if ( !url.host().isEmpty() ) _p->_Database.setHostName( url.host() );
		if ( url.port() != -1 ) _p->_Database.setPort( url.port() );
		if ( !url.path().isEmpty() ) _p->_Database.setDatabaseName( QString::fromStdString( ( XS::CoreFramework::GetCurrentFramework()->GetProjectPath() / ( "." + url.path().toStdString() ) ).string() ) );
		if ( !url.userName().isEmpty() ) _p->_Database.setUserName( url.userName() );
		if ( !url.password().isEmpty() ) _p->_Database.setPassword( url.password() );
	}

	if ( _p->_Database.open() )
	{
		QSqlQuery query( _p->_Database );
		
		QString cmd = QString( "CREATE TABLE IF NOT EXISTS %1 ( uuid CHAR(50) PRIMARY KEY NOT NULL UNIQUE, path VARCHAR(255) NOT NULL, data VARCHAR(5000) NOT NULL );" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().c_str() );

		bool result = query.exec( cmd );

		if ( result == false ) _p->_Database.close();

		return result;
	}

	return false;
}

bool XS::AssetDatabase::Insert( const QUuid & uuid, const QFileInfo & path, const QJsonDocument & json )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		QDir dir( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string().c_str() );

		QString cmd = QString( R"(INSERT INTO %1 VALUES ( '%2', '%3', '%4' );)" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
			.arg( uuid.toString() )
			.arg( dir.relativeFilePath( path.absoluteFilePath() ) )
			.arg( QString( json.toJson( QJsonDocument::Compact ) ) );

		if ( query.exec( cmd ) )
		{
			_p->_Cache.insert( uuid, new QPair<QFileInfo, QJsonDocument>( path, json ) );

			return true;
		}

		qDebug() << query.lastError().databaseText();
	}

	return false;
}

bool XS::AssetDatabase::Remove( const QUuid & uuid )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		QString cmd = QString( R"(DELETE FROM %1 WHERE uuid = '%2';)" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
			.arg( uuid.toString() );

		if ( query.exec( cmd ) )
		{
			if ( _p->_Cache.contains( uuid ) )
			{
				_p->_Cache.remove( uuid );
			}

			return true;
		}

		qDebug() << query.lastError().databaseText();
	}

	return false;
}

bool XS::AssetDatabase::Update( const QUuid & uuid, const QFileInfo & path )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		QDir dir( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string().c_str() );

		QString cmd = QString( R"(UPDATE %1 SET path = '%2' WHERE uuid = '%3';)" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
			.arg( dir.relativeFilePath( path.absoluteFilePath() ) )
			.arg( uuid.toString() );

		if ( query.exec( cmd ) )
		{
			_p->_Cache.object( uuid )->first = path;

			return true;
		}

		qDebug() << query.lastError().databaseText();
	}

	return false;
}

bool XS::AssetDatabase::Update( const QUuid & uuid, const QJsonDocument & json )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		QString cmd = QString( R"(UPDATE %1 SET data = '%2' WHERE uuid = '%3';)" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
			.arg( QString( json.toJson( QJsonDocument::Compact ) ) )
			.arg( uuid.toString() );

		if ( query.exec( cmd ) )
		{
			_p->_Cache.object( uuid )->second = json;

			return true;
		}

		qDebug() << query.lastError().databaseText();
	}

	return false;
}

QPair<QFileInfo, QJsonDocument > XS::AssetDatabase::Query( const QUuid & uuid )
{
	QPair< QFileInfo, QJsonDocument > result;

	if ( _p->_Database.isOpen() )
	{
		if ( _p->_Cache.contains( uuid ) )
		{
			return *_p->_Cache.object( uuid );
		}

		QSqlQuery query( _p->_Database );

		QDir dir( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string().c_str() );

		QString cmd = QString( R"(SELECT path, data FROM %1 WHERE uuid = '%2';)" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
			.arg( uuid.toString() );

		if ( query.exec( cmd ) )
		{
			if ( query.next() )
			{
				result.first = dir.absoluteFilePath( query.value( 0 ).toString() );
				result.second = QJsonDocument::fromJson( query.value( 1 ).toByteArray() );

				_p->_Cache.insert( uuid, new QPair<QFileInfo, QJsonDocument >( result ) );
			}
		}
		else
		{
			qDebug() << query.lastError().databaseText();
		}
	}

	return result;
}

QUuid XS::AssetDatabase::Query( const QFileInfo & path )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		QDir dir( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string().c_str() );

		QString cmd = QString( R"(SELECT uuid FROM %1 WHERE path = '%2';)" )
			.arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
			.arg( dir.relativeFilePath( path.absoluteFilePath() ) );

		if ( query.exec( cmd ) )
		{
			if ( query.next() )
			{
				QString s = query.value( 0 ).toString();
				return QUuid::fromString( query.value( 0 ).toString() );
			}
		}
		else
		{
			qDebug() << query.lastError().databaseText();
		}
	}

	return {};
}

QFileInfo XS::AssetDatabase::QueryPath( const QUuid & uuid )
{
	return Query( uuid ).first;
}

QJsonDocument XS::AssetDatabase::QueryData( const QUuid & uuid )
{
	return Query( uuid ).second;
}
