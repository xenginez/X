/*!
 * \file   TextAssetInspector.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/03
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef TEXTASSETINSPECTOR_H__609BD32F_9D18_484F_832E_9447ADD17A77
#define TEXTASSETINSPECTOR_H__609BD32F_9D18_484F_832E_9447ADD17A77

#include "../AssetInspector.h"

BEG_XS_NAMESPACE

class TextAssetInspector : public AssetInspector
{
	Q_OBJECT

public:
	TextAssetInspector( QObject * parent = nullptr );

	~TextAssetInspector() override;

public:
	QList< QMimeType > SupportedAssetFormats() override;

	QWidget * ImportAsset( QSqlDatabase & db, const QMimeData & data ) override;

	QWidget * EditorAsset( QSqlDatabase & db, const QMimeData & data ) override;

	QWidget * ChangeAsset( QSqlDatabase & db, const QMimeData & data ) override;

	QWidget * DeleteAsset( QSqlDatabase & db, const QMimeData & data ) override;
};

END_XS_NAMESPACE

#endif // TEXTASSETINSPECTOR_H__609BD32F_9D18_484F_832E_9447ADD17A77
