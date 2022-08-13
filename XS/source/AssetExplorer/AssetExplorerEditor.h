/*!
 * \file	AssetsEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/04/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSETSEDITOR_H__5E9D7DFE_346F_4735_AFAE_DE82F5673EC8
#define ASSETSEDITOR_H__5E9D7DFE_346F_4735_AFAE_DE82F5673EC8

#include "ToolEditorWindow.h"

UI_DECL( AssetExplorerEditor );

BEG_XS_NAMESPACE

class AssetsItemModel;

class XS_API AssetExplorerEditor : public XS::ToolEditorWindow
{
	Q_OBJECT

public:
	AssetExplorerEditor( QWidget * parent = nullptr );

	~AssetExplorerEditor();

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnWatcherFileChanged( const QString & path );

	void OnWatcherDirectoryChanged( const QString & path );

	void OnScaleValueChanged( int value );

	void OnTreeViewClicked( const QModelIndex & index );

	void OnTreeViewCustomContextMenuRequested( const QPoint & pos );

private:
	Ui::AssetExplorerEditor * ui;
	XS::AssetsItemModel * _Model;
};

END_XS_NAMESPACE

#endif//ASSETSEDITOR_H__5E9D7DFE_346F_4735_AFAE_DE82F5673EC8
