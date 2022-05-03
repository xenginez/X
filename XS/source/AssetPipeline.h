/*!
 * \file   AssetPipeline.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/03
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSETPIPELINE_H__6EA5B576_420B_4B05_A62C_F45A79123C87
#define ASSETPIPELINE_H__6EA5B576_420B_4B05_A62C_F45A79123C87

#include <QIcon>
#include <QMimeType>
#include <QMimeData>
#include <QSqlDatabase>
#include <QMimeDatabase>

#include "Widget.h"

BEG_XS_NAMESPACE

class XS_API AssetPipeline : public QObject
{
	Q_OBJECT

public:
	AssetPipeline( QObject * parent = nullptr );

	~AssetPipeline() override;

public:
	virtual QStringList ContextMenuNames() const = 0;

	virtual QList< QMimeType > SupportedAssetFormats() const = 0;

	virtual QIcon AssetIcon( QSqlDatabase & db, const QMimeData & data ) const = 0;

	virtual bool CreateAsset( QSqlDatabase & db, const QMimeData & data ) const = 0;

	virtual bool ImportAsset( QSqlDatabase & db, const QMimeData & data ) const = 0;
	
	virtual bool EditorAsset( QSqlDatabase & db, const QMimeData & data ) const = 0;
	
	virtual bool ChangeAsset( QSqlDatabase & db, const QMimeData & data ) const = 0;
	
	virtual bool DeleteAsset( QSqlDatabase & db, const QMimeData & data ) const = 0;
};

END_XS_NAMESPACE

#endif // ASSETPIPELINE_H__6EA5B576_420B_4B05_A62C_F45A79123C87
