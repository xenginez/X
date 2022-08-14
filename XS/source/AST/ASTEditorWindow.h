/*!
 * \file	ASTEditorWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/14
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTEDITORWINDOW_H__D176DB34_7428_4CA5_AEB8_DDC3C66944EA
#define ASTEDITORWINDOW_H__D176DB34_7428_4CA5_AEB8_DDC3C66944EA

#include "AssetEditorWindow.h"

UI_DECL( ASTEditorWindow );

BEG_XS_NAMESPACE

class ASTEditorWindow;

class ASTEditor : public XS::AssetEditor
{
	Q_OBJECT

	friend class ASTEditorWindow;

public:
	ASTEditor( QObject * parent = nullptr );

	~ASTEditor() override;

public:
	QIcon icon() override;

	QString name() override;

	QString extensionName() override;

public:
	QUuid assetCreate( const QFileInfo & path ) override;

	QIcon assetIcon( const QUuid & uuid ) override;

	void assetRemove( const QUuid & uuid ) override;

	void assetRename( const QUuid & uuid, const QFileInfo & old_dir, const QFileInfo & new_dir ) override;

public:
	void assetOpen( const QUuid & uuid ) override;

	void assetBuild( const QUuid & uuid ) override;
};

class ASTEditorWindow : public XS::AssetEditorWindow
{
	Q_OBJECT

	friend class ASTEditor;

public:
	ASTEditorWindow( QWidget * parent = nullptr );

	~ASTEditorWindow() override;

public:
	void assetOpen( const QUuid & uuid ) override;

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private:
	XS::ASTEditor * _Editor;
	Ui::ASTEditorWindow * ui;
};

END_XS_NAMESPACE

#endif//ASTEDITORWINDOW_H__D176DB34_7428_4CA5_AEB8_DDC3C66944EA
