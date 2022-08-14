/*!
 * \file	ASTTabWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F
#define ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F

#include "Widget.h"
#include "NodeWidget.h"

UI_DECL( ASTTabWidget );

BEG_XS_NAMESPACE

class XS_API ASTTabWidget : public XS::Widget
{
	Q_OBJECT

public:
	ASTTabWidget( QWidget * parent = nullptr );

	~ASTTabWidget() override;

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnAddToolButtonClicked( bool clicked );

	void OnToolToolButtonClicked( bool clicked );

private:
	Ui::ASTTabWidget * ui;
};

END_XS_NAMESPACE

#endif//ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F
