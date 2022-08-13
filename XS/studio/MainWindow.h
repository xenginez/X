/*!
 * \file	MainWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MAINWINDOW_H__6C6EAC24_A5DF_4472_8D75_72505C174B1D
#define MAINWINDOW_H__6C6EAC24_A5DF_4472_8D75_72505C174B1D

#include "Global.h"

namespace XStudio
{
	class MainWindow : public XS::MainWindow
	{
		Q_OBJECT

	public:
		MainWindow( const QString & project, QWidget * parent = nullptr );

		~MainWindow();

	protected:
		void showEvent( QShowEvent * e ) override;

		void closeEvent( QCloseEvent * e ) override;
	};
}

#endif//MAINWINDOW_H__6C6EAC24_A5DF_4472_8D75_72505C174B1D
