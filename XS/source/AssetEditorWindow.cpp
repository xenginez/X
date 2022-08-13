#include "AssetEditorWindow.h"

#include "CoreFramework.h"

XS::AssetEditorWindow::AssetEditorWindow( QWidget * parent /*= nullptr */ )
	: XS::DockWidget( parent )
{

}

XS::AssetEditorWindow::~AssetEditorWindow()
{

}

QIcon XS::AssetEditorWindow::icon()
{
	return {};
}

QString XS::AssetEditorWindow::name()
{
	return {};
}

QString XS::AssetEditorWindow::extensionName()
{
	return {};
}

QUuid XS::AssetEditorWindow::assetCreate( const QDir & path )
{
	return {};
}

QIcon XS::AssetEditorWindow::assetIcon( const QUuid & uuid )
{
	return {};
}

void XS::AssetEditorWindow::assetRemove( const QUuid & uuid )
{

}

XS::AssetDatabasePtr XS::AssetEditorWindow::assetDatabase()
{
	return XS::CoreFramework::GetCurrentFramework()->GetAssetDatabase();
}

void XS::AssetEditorWindow::assetRename( const QUuid & uuid, const QDir & old_dir, const QDir & new_dir )
{

}
