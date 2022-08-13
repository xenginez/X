#include "AssetDatabase.h"

#include <QSqlQuery>
#include <QSqlDatabase>

#include "CoreFramework.h"

struct XS::AssetDatabase::Private
{
	QSqlDatabase _Database;
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
	_p->_Database = QSqlDatabase::addDatabase( url.scheme() );
	{
		if ( !url.host().isEmpty() ) _p->_Database.setHostName( url.host() );
		if ( url.port() != -1 ) _p->_Database.setPort( url.port() );
		if ( !url.path().isEmpty() ) _p->_Database.setDatabaseName( url.path() );
		if ( !url.userName().isEmpty() ) _p->_Database.setUserName( url.userName() );
		if ( !url.password().isEmpty() ) _p->_Database.setPassword( url.password() );
	}

	if ( _p->_Database.open() )
	{
		QSqlQuery query( _p->_Database );

		bool result = query.exec( QString( "CREATE TABLE IF NOT EXISTS %1 ( uuid CHAR(128) PRIMARY KEY NOT NULL, path VARCHAR(255) NOT NULL, data VARCHAR(60000) ) NOT NULL;" )
								  .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().c_str() ) );

		if ( result == false ) _p->_Database.close();

		return result;
	}

	return false;
}

bool XS::AssetDatabase::Insert( const QUuid & uuid, const QDir & path, const QJsonDocument & json )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		return query.exec( QString( "INSERT INTO %1 VALUES ( %2, %3, %4 );" )
						   .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						   .arg( uuid.toString( QUuid::Id128 ) )
						   .arg( path.path() )
						   .arg( QString( json.toJson( QJsonDocument::Compact ) ) ) );
	}

	return false;
}

bool XS::AssetDatabase::Remove( const QUuid & uuid )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		return query.exec( QString( "DELETE FROM %1 WHERE uuid = %2;" )
						   .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						   .arg( uuid.toString( QUuid::Id128 ) ) );
	}

	return false;
}

bool XS::AssetDatabase::Update( const QUuid & uuid, const QDir & path )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		return query.exec( QString( "UPDATE %1 SET path = %2 WHERE uuid = %3;" )
						   .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						   .arg( path.path() )
						   .arg( uuid.toString( QUuid::Id128 ) ) );
	}

	return false;
}

bool XS::AssetDatabase::Update( const QUuid & uuid, const QJsonDocument & json )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		return query.exec( QString( "UPDATE %1 SET data = %2 WHERE uuid = %3;" )
						   .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						   .arg( QString( json.toJson( QJsonDocument::Compact ) ) )
						   .arg( uuid.toString( QUuid::Id128 ) ) );
	}

	return false;
}

QPair<QDir, QJsonDocument > XS::AssetDatabase::Query( const QUuid & uuid )
{
	QPair< QDir, QJsonDocument > result;

	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		if ( query.exec( QString( "SELECT path, data FROM %1 WHERE uuid = %2;" )
						 .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						 .arg( uuid.toString( QUuid::Id128 ) ) ) )
		{
			if ( query.next() )
			{
				result.first = query.value( "path" ).toString();
				result.second = QJsonDocument::fromJson( query.value( "data" ).toByteArray() );
			}
		}
	}

	return result;
}

QUuid XS::AssetDatabase::Query( const QDir & path )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		if ( query.exec( QString( "SELECT uuid FROM %1 WHERE path = %2;" )
						 .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						 .arg( path.path() ) ) )
		{
			if ( query.next() )
			{
				return QUuid::fromString( query.value( "uuid" ).toString() );
			}
		}
	}

	return {};
}

QDir XS::AssetDatabase::QueryPath( const QUuid & uuid )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		if ( query.exec( QString( "SELECT path FROM %1 WHERE uuid = %2;" )
						 .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						 .arg( uuid.toString( QUuid::Id128 ) ) ) )
		{
			if ( query.next() )
			{
				return query.value( "path" ).toString();
			}
		}
	}

	return {};
}

QJsonDocument XS::AssetDatabase::QueryData( const QUuid & uuid )
{
	if ( _p->_Database.isOpen() )
	{
		QSqlQuery query( _p->_Database );

		if ( query.exec( QString( "SELECT data FROM %1 WHERE uuid = %2;" )
						 .arg( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().stem().string().c_str() )
						 .arg( uuid.toString( QUuid::Id128 ) ) ) )
		{
			if ( query.next() )
			{
				QJsonParseError error;
				QJsonDocument json = QJsonDocument::fromJson( query.value( "data" ).toByteArray(), &error );
				if ( error.error == QJsonParseError::NoError )
				{
					return json;
				}
			}
		}
	}

	return {};
}
