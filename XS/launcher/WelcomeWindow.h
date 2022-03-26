/*!
 * \file	WelcomeWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/14
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WELCOMEWINDOW_H__2FE39C30_E3E0_4956_9C5C_62B20DC94240
#define WELCOMEWINDOW_H__2FE39C30_E3E0_4956_9C5C_62B20DC94240

#include "Global.h"

namespace Ui
{
	class WelcomeWindow;
}

namespace Launcher
{
	class WelcomeWindow : public QWidget
	{
		Q_OBJECT

	public:
		WelcomeWindow( QWidget * parent = nullptr );

		~WelcomeWindow();

	public:
		bool eventFilter( QObject * obj, QEvent * event ) override;

	private slots:
		void OnOpenClicked();

		void OnCloneClicked();

		void OnCreateClicked();

		void OnProjectListItemFixed( QListWidgetItem * item );

		void OnProjectListItemDoubleClicked( QListWidgetItem * item );

	private:
		void OpenProject( const QString & path );

		void ReadProjectList();

		void WriteProjectList();

		void FlushProjectList();

	private:
		Ui::WelcomeWindow * ui;
		QJsonObject _Projects;
		QListWidgetItem * _SelectItem = nullptr;
	};
}

#endif//WELCOMEWINDOW_H__2FE39C30_E3E0_4956_9C5C_62B20DC94240
