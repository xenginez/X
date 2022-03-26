/*!
 * \file	GitCloneDialog.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GITCLONEDIALOG_H__18DF5665_0349_4488_9D09_DEBE430708DB
#define GITCLONEDIALOG_H__18DF5665_0349_4488_9D09_DEBE430708DB

#include "Global.h"

namespace Ui
{
	class CloneDialog;
}

namespace Launcher
{
	class CloneDialog : public QDialog
	{
		Q_OBJECT

	public:
		CloneDialog( QWidget * parent = nullptr );

		~CloneDialog();

	public:
		QString url() const;

		QString dir() const;

	private slots:
		void OnUrlButtonClicked();

		void OnDirButtonClicked();

		void OnCancelButtonClicked();

		void OnFinishButtonClicked();

		void OnConfirmButtonClicked();

	private:
		Ui::CloneDialog * ui;
	};
}

#endif//GITCLONEDIALOG_H__18DF5665_0349_4488_9D09_DEBE430708DB
