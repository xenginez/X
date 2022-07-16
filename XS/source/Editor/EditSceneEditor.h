/*!
 * \file	EditSceneEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/04/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef EDITSCENEEDITOR_H__2C2FB7BC_1E8B_4ADF_9296_1F4802B4CADB
#define EDITSCENEEDITOR_H__2C2FB7BC_1E8B_4ADF_9296_1F4802B4CADB

#include "Core/Core.h"

UI_DECL( EditSceneEditor );

BEG_XS_NAMESPACE

class XS_API EditSceneEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	EditSceneEditor( QWidget * parent = nullptr );

	~EditSceneEditor();

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnD23Clicked( bool checked = false );
	void OnToolsClicked( bool checked = false );
	void OnTransformClicked( bool checked = false );

protected:
	bool eventFilter( QObject * watched, QEvent * event ) override;

private:
	Ui::EditSceneEditor * ui;
};

END_XS_NAMESPACE

#endif//EDITSCENEEDITOR_H__2C2FB7BC_1E8B_4ADF_9296_1F4802B4CADB
