#include "TextAssetInspector.h"

XS::TextAssetInspector::TextAssetInspector( QObject * parent /*= nullptr */ )
	: AssetInspector( parent )
{

}

XS::TextAssetInspector::~TextAssetInspector()
{

}

QList< QMimeType > XS::TextAssetInspector::SupportedAssetFormats()
{
	QMimeDatabase db;
	QList< QMimeType > result;

	result.push_back( db.mimeTypeForName( "text/html" ) );
	result.push_back( db.mimeTypeForName( "text/plain" ) );

	return result;
}

QWidget * XS::TextAssetInspector::ImportAsset( QSqlDatabase & db, const QMimeData & data )
{
	return nullptr;
}

QWidget * XS::TextAssetInspector::EditorAsset( QSqlDatabase & db, const QMimeData & data )
{

}

QWidget * XS::TextAssetInspector::ChangeAsset( QSqlDatabase & db, const QMimeData & data )
{

}

QWidget * XS::TextAssetInspector::DeleteAsset( QSqlDatabase & db, const QMimeData & data )
{

}
