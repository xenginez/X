/*!
 * \file	CodeEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CODEEDITOR_H__96837112_AD11_44A5_AF31_B49805E9BFA2
#define CODEEDITOR_H__96837112_AD11_44A5_AF31_B49805E9BFA2

#include "Core/AssetEditorWindow.h"

BEG_XS_NAMESPACE

class XS_API CodeEditor : public AssetEditor
{
	Q_OBJECT

public:
	CodeEditor( QObject * parent = nullptr );

	~CodeEditor() override;

public:
	QIcon icon() override;

	QString name() override;

	QStringList extensionName() override;

public:
	QUuid assetCreate( const QFileInfo & path ) override;

	QIcon assetIcon( const QUuid & uuid ) override;

	void assetRemove( const QUuid & uuid ) override;

	void assetRename( const QUuid & uuid, const QFileInfo & old_path, const QFileInfo & new_path ) override;

	const QMetaObject * assetEditor() override;

	void assetBuild( const QUuid & uuid ) override;

};

END_XS_NAMESPACE

#endif//CODEEDITOR_H__96837112_AD11_44A5_AF31_B49805E9BFA2
