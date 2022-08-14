#include "AssetEditorWindow.h"

#include "CoreFramework.h"

XS::AssetEditor::AssetEditor( QObject * parent /*= nullptr */ )
	: QObject( parent )
{

}

XS::AssetEditor::~AssetEditor()
{

}

XS::AssetDatabasePtr XS::AssetEditor::assetDatabase()
{
	return XS::CoreFramework::GetCurrentFramework()->GetAssetDatabase();
}

XS::AssetEditorWindow::AssetEditorWindow( QWidget * parent /*= nullptr */ )
	: XS::DockWidget( parent )
{

}

XS::AssetEditorWindow::~AssetEditorWindow()
{

}
