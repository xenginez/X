/*!
 * \file   AssetImporter.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/29
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSETIMPORTER_H__7E31FA17_9404_4F75_9BF2_B6717612F3A3
#define ASSETIMPORTER_H__7E31FA17_9404_4F75_9BF2_B6717612F3A3

#include <QMimeType>
#include <QMimeData>
#include <QSqlDatabase>
#include <QMimeDatabase>

#include "Widget.h"

BEG_XS_NAMESPACE

class XS_API AssetInspector : public QObject
{
	Q_OBJECT

public:
	AssetInspector( QObject * parent = nullptr );

	~AssetInspector() override;

public:
	virtual QList< QMimeType > SupportedAssetFormats() = 0;

	virtual QWidget * ImportAsset( QSqlDatabase & db, const QMimeData & data ) = 0;

	virtual QWidget * EditorAsset( QSqlDatabase & db, const QMimeData & data ) = 0;

	virtual QWidget * ChangeAsset( QSqlDatabase & db, const QMimeData & data ) = 0;

	virtual QWidget * DeleteAsset( QSqlDatabase & db, const QMimeData & data ) = 0;
};

END_XS_NAMESPACE

#endif // ASSETIMPORTER_H__7E31FA17_9404_4F75_9BF2_B6717612F3A3
