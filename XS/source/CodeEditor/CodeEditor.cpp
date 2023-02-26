#include "CodeEditor.h"

#include "CodeEditorWindow.h"

REG_ASSET( XS::CodeEditor );

XS::CodeEditor::CodeEditor( QObject * parent /*= nullptr */ )
{

}

XS::CodeEditor::~CodeEditor()
{

}

QIcon XS::CodeEditor::icon()
{
	return {};
}

QString XS::CodeEditor::name()
{
	return {};
}

QStringList XS::CodeEditor::extensionName()
{
	return {};
}

QUuid XS::CodeEditor::assetCreate( const QFileInfo & path )
{
	return {};
}

QIcon XS::CodeEditor::assetIcon( const QUuid & uuid )
{
	return {};
}

void XS::CodeEditor::assetRemove( const QUuid & uuid )
{

}

void XS::CodeEditor::assetRename( const QUuid & uuid, const QFileInfo & old_path, const QFileInfo & new_path )
{

}

const QMetaObject * XS::CodeEditor::assetEditor()
{
	return {};
}

void XS::CodeEditor::assetBuild( const QUuid & uuid )
{

}

