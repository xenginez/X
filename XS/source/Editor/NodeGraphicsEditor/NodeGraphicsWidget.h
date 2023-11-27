/*!
 * \file	NodeGraphicsWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2023/11/19
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSWIDGET_H__B51CF07A_C9E8_4D9E_B53B_CE8B14A5565F
#define NODEGRAPHICSWIDGET_H__B51CF07A_C9E8_4D9E_B53B_CE8B14A5565F

#include "Global.h"

UI_DECL( NodeGraphicsWidget );

BEG_XS_NAMESPACE

class XS_API NodeGraphicsWidget : public QWidget
{
	Q_OBJECT

public:
	NodeGraphicsWidget( QWidget * parent = nullptr );

	~NodeGraphicsWidget();

public:

private:
	Ui::NodeGraphicsWidget * ui;
};

END_XS_NAMESPACE

#endif//NODEGRAPHICSWIDGET_H__B51CF07A_C9E8_4D9E_B53B_CE8B14A5565F