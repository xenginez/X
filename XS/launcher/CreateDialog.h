/*!
 * \file	CreateDialog.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CREATEDIALOG_H__38E16280_D43B_4F74_9574_1C3E6821FB4E
#define CREATEDIALOG_H__38E16280_D43B_4F74_9574_1C3E6821FB4E

#include "Global.h"

namespace Ui
{
	class CreateDialog;
}

namespace Launcher
{
	class CreateDialog : public QDialog
	{
		Q_OBJECT

	public:
		CreateDialog( QWidget * parent = nullptr );

		~CreateDialog();

	public:
		QString dir() const;

	private slots:
		void OnDirButtonClicked();

		void OnCancelButtonClicked();

		void OnConfirmButtonClicked();

	private slots:
		void CreateProject();

		void FlushTemplate();

		void DownloadTemplate( QListWidgetItem * item );

	private:
		Ui::CreateDialog * ui;
	};
}

#endif//CREATEDIALOG_H__38E16280_D43B_4F74_9574_1C3E6821FB4E
