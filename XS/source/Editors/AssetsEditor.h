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

#include "../DockWidget.h"

#include <QSqlDatabase>
#include <QFileSystemWatcher>

UI_DECL( AssetsEditor );

BEG_XS_NAMESPACE

class XS_API AssetsEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	AssetsEditor( QWidget * parent = nullptr );

	~AssetsEditor();

public:
	void Save( QSettings & settings ) override;

	void Load( QSettings & settings ) override;

private slots:
	void OnWatcherFileChanged( const QString & path );

	void OnWatcherDirectoryChanged( const QString & path );

private:
	Ui::AssetsEditor * ui;
	QSqlDatabase _LocalDB;
	QSqlDatabase _RemoteDB;
	QFileSystemWatcher _Watcher;
};

END_XS_NAMESPACE

#endif//ASSETSEDITOR_H__5E9D7DFE_346F_4735_AFAE_DE82F5673EC8
