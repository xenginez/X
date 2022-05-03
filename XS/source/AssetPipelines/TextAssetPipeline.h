/*!
 * \file   TextAssetPipeline.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/03
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef TEXTASSETPIPELINE_H__C5A19B4E_3DF2_4B22_9913_B5D466C68675
#define TEXTASSETPIPELINE_H__C5A19B4E_3DF2_4B22_9913_B5D466C68675

#include "../AssetPipeline.h"

BEG_XS_NAMESPACE

class TextAssetPipeline : public AssetPipeline
{
	Q_OBJECT

public:
	TextAssetPipeline( QObject * parent = nullptr );

	~TextAssetPipeline() override;

public:
	QStringList ContextMenuNames() const override;

	QList< QMimeType > SupportedAssetFormats() const override;

	QIcon AssetIcon( QSqlDatabase & db, const QMimeData & data ) const override;

	bool CreateAsset( QSqlDatabase & db, const QMimeData & data ) const override;

	bool ImportAsset( QSqlDatabase & db, const QMimeData & data ) const override;

	bool EditorAsset( QSqlDatabase & db, const QMimeData & data ) const override;

	bool ChangeAsset( QSqlDatabase & db, const QMimeData & data ) const override;

	bool DeleteAsset( QSqlDatabase & db, const QMimeData & data ) const override;
};

END_XS_NAMESPACE

#endif // TEXTASSETPIPELINE_H__C5A19B4E_3DF2_4B22_9913_B5D466C68675
