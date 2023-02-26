/*!
 * \file	AssetEditorWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSETEDITORWINDOW_H__306E3902_D212_46D2_8C1E_02AB97F37CF9
#define ASSETEDITORWINDOW_H__306E3902_D212_46D2_8C1E_02AB97F37CF9

#include "DockWidget.h"

BEG_XS_NAMESPACE

class XS_API AssetEditor : public QObject
{
	Q_OBJECT

public:
	AssetEditor( QObject * parent = nullptr );

	~AssetEditor() override;

public:
	virtual QIcon icon() = 0;

	virtual QString name() = 0;

	virtual QStringList extensionName() = 0;

public:
	virtual QUuid assetCreate( const QFileInfo & path ) = 0;

	virtual QIcon assetIcon( const QUuid & uuid ) = 0;

	virtual void assetRemove( const QUuid & uuid ) = 0;

	virtual void assetRename( const QUuid & uuid, const QFileInfo & old_path, const QFileInfo & new_path ) = 0;

	virtual const QMetaObject * assetEditor() = 0;

	virtual void assetBuild( const QUuid & uuid ) = 0;

public:
	XS::AssetDatabasePtr assetDatabase();
};

class XS_API AssetEditorWindow : public XS::DockWidget
{
	Q_OBJECT

public:
	AssetEditorWindow( QWidget * parent = nullptr );

	~AssetEditorWindow() override;

public:
	virtual void assetOpen( const QUuid & uuid ) = 0;
};

END_XS_NAMESPACE

#define REG_ASSET( TYPE ) REG_OBJECT( TYPE ); REG_WIDGET( TYPE##Window );

#endif//ASSETEDITORWINDOW_H__306E3902_D212_46D2_8C1E_02AB97F37CF9
