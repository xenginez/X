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

#include "../DockWidget.h"

UI_DECL( EditSceneEditor );

BEG_XS_NAMESPACE

class XS_API EditSceneEditor : public XS::DockWidget
{
	Q_OBJECT

public:
	EditSceneEditor( QWidget * parent = nullptr );

	~EditSceneEditor();

public:
	WId display() const;

private slots:
	void OnAudioClicked( bool checked = false );
	void OnCameraClicked( bool checked = false );
	void OnGridClicked( bool checked = false );
	void OnIconsClicked( bool checked = false );
	void OnLightClicked( bool checked = false );
	void OnMoveClicked( bool checked = false );
	void OnRotateClicked( bool checked = false );
	void OnScaleClicked( bool checked = false );
	void OnSearchFinished();

private:
	Ui::EditSceneEditor * ui;
};

END_XS_NAMESPACE

#endif//EDITSCENEEDITOR_H__2C2FB7BC_1E8B_4ADF_9296_1F4802B4CADB
