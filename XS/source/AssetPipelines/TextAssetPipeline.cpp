#include "TextAssetPipeline.h"

XS::TextAssetPipeline::TextAssetPipeline( QObject * parent /*= nullptr */ )
	: AssetPipeline( parent )
{

}

XS::TextAssetPipeline::~TextAssetPipeline()
{

}

QStringList XS::TextAssetPipeline::ContextMenuNames() const
{
	return { "Text/txt", "Text/csv", "Text/xml", "Text/json" };
}

QList< QMimeType > XS::TextAssetPipeline::SupportedAssetFormats() const
{
	QMimeDatabase db;
	QList< QMimeType > result;

	result.push_back( db.mimeTypeForName( "text/csv" ) );
	result.push_back( db.mimeTypeForName( "text/plain" ) );
	result.push_back( db.mimeTypeForName( "application/xml" ) );
	result.push_back( db.mimeTypeForName( "application/json" ) );

	return result;
}

QIcon XS::TextAssetPipeline::AssetIcon( QSqlDatabase & db, const QMimeData & data ) const
{
	if( data.hasFormat( "text/csv" ) )
	{
		return QIcon( "SkinIcons:/images/icons/icon_asset_csv.svg" );
	}
	else if( data.hasFormat( "text/plain" ) )
	{
		return QIcon( "SkinIcons:/images/icons/icon_asset_txt.svg" );
	}
	else if( data.hasFormat( "application/xml" ) )
	{
		return QIcon( "SkinIcons:/images/icons/icon_asset_xml.svg" );
	}
	else if( data.hasFormat( "application/json" ) )
	{
		return QIcon( "SkinIcons:/images/icons/icon_asset_json.svg" );
	}

	return {};
}

bool XS::TextAssetPipeline::CreateAsset( QSqlDatabase & db, const QMimeData & data ) const
{
	return false;
}

bool XS::TextAssetPipeline::ImportAsset( QSqlDatabase & db, const QMimeData & data ) const
{
	return false;
}

bool XS::TextAssetPipeline::EditorAsset( QSqlDatabase & db, const QMimeData & data ) const
{
	return false;
}

bool XS::TextAssetPipeline::ChangeAsset( QSqlDatabase & db, const QMimeData & data ) const
{
	return false;
}

bool XS::TextAssetPipeline::DeleteAsset( QSqlDatabase & db, const QMimeData & data ) const
{
	return false;
}
