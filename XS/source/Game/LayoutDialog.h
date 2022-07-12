/*!
 * \file	LayoutDialog.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LAYOUTDIALOG_H__40CFF3B5_C5A3_42FF_B957_FA61244AA0C5
#define LAYOUTDIALOG_H__40CFF3B5_C5A3_42FF_B957_FA61244AA0C5

#include <QSize>

#include "Core/Core.h"

UI_DECL( LayoutDialog );

BEG_XS_NAMESPACE

class XS_API LayoutDialog : public QDialog
{
	Q_OBJECT

public:
	LayoutDialog( QWidget * parent = nullptr );

	~LayoutDialog() override;

public slots:
	void OnLayoutSize( QSize size );

	void OnLayoutCustomSize();

private:
	Ui::LayoutDialog * ui;
	QSize _Size;

};

END_XS_NAMESPACE

#endif//LAYOUTDIALOG_H__40CFF3B5_C5A3_42FF_B957_FA61244AA0C5
