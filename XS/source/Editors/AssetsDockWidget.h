/*!
 * \file	AssetsDockWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSETSDOCKWIDGET_H__583A67AC_F146_4C5C_8692_1DE921DD72A4
#define ASSETSDOCKWIDGET_H__583A67AC_F146_4C5C_8692_1DE921DD72A4

#include "../DockWidget.h"

UI_DECL( AssetsDockWidget );

BEG_XS_NAMESPACE

class XS_API AssetsDockWidget : public XS::DockWidget
{
	Q_OBJECT

public:
	AssetsDockWidget( QWidget * parent = nullptr );

	~AssetsDockWidget();

public:
	void Save( QSettings & settings ) override;

	void Load( QSettings & settings ) override;

private:
	Ui::AssetsDockWidget * ui;
};

END_XS_NAMESPACE

#endif//ASSETSDOCKWIDGET_H__583A67AC_F146_4C5C_8692_1DE921DD72A4
