/*!
 * \file	RenderGraphEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143
#define RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143

#include "AssetEditorWindow.h"

UI_DECL( ASTEditor );

BEG_XS_NAMESPACE

class ASTEditor : public XS::AssetEditorWindow
{
	Q_OBJECT

public:
	ASTEditor( QWidget * parent = nullptr );

	~ASTEditor() override;

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

	Q_INVOKABLE static QString extensionName();

public:
	Q_INVOKABLE static QUuid assetCreate( const QDir & path );

	Q_INVOKABLE static QIcon assetIcon( const QUuid & uuid );

	Q_INVOKABLE static void assetRemove( const QUuid & uuid );

public:
	void assetOpen( const QUuid & uuid ) override;

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnAddToolButtonClicked( bool clicked );

	void OnToolToolButtonClicked( bool clicked );


private:
	Ui::ASTEditor * ui;
};

END_XS_NAMESPACE

#endif//RENDERGRAPHEDITOR_H__ABDD9F06_A753_4C14_8FF2_8BC1C2C64143
