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

#include <QDir>

#include "DockWidget.h"
#include "AssetDatabase.h"

BEG_XS_NAMESPACE

class XS_API AssetEditorWindow : public XS::DockWidget
{
	Q_OBJECT

public:
	AssetEditorWindow( QWidget * parent = nullptr );

	~AssetEditorWindow() override;

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

	Q_INVOKABLE static QString extensionName();

public:
	Q_INVOKABLE static QUuid assetCreate( const QDir & path );

	Q_INVOKABLE static QIcon assetIcon( const QUuid & uuid );

	Q_INVOKABLE static void assetRemove( const QUuid & uuid );

public:
	static XS::AssetDatabase * assetDatabase();

public:
	virtual void assetOpen( const QUuid & uuid ) = 0;

};

END_XS_NAMESPACE

#endif//ASSETEDITORWINDOW_H__306E3902_D212_46D2_8C1E_02AB97F37CF9
