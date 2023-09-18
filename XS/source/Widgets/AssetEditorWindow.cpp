#include "AssetEditorWindow.h"

#include "Core/CoreFramework.h"

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
