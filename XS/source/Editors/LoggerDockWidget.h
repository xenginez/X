/*!
 * \file	LoggerDockWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LOGGERDOCKWIDGET_H__D7F0249E_2F0C_4385_8D68_B12BF0596FF2
#define LOGGERDOCKWIDGET_H__D7F0249E_2F0C_4385_8D68_B12BF0596FF2

#include <QStandardItemModel>

#include "../DockWidget.h"

UI_DECL( LoggerDockWidget );

BEG_XS_NAMESPACE

class XS_API LoggerDockWidget : public XS::DockWidget
{
	Q_OBJECT

public:
	LoggerDockWidget( QWidget * parent = nullptr );

	~LoggerDockWidget();

public:
	void Save( QSettings & settings ) override;

	void Load( QSettings & settings ) override;

private slots:
	void OnErrorButtonClicked( bool checked );
	void OnWarningButtonClicked( bool checked );
	void OnInfoButtonClicked( bool checked );
	void OnClearButtonClicked( bool checked );
	void OnMergeButtonClicked( bool checked );
	void OnSearchEditingFinished();
	void OnListViewItemDoubleClicked( const QModelIndex & index );

private:
	void OnLoggerListener( std::chrono::system_clock::time_point time, const char * file, XE::uint32 line, XE::LoggerLevel level, XE::String msg );

private:
	XE::Disposable _Logger;
	Ui::LoggerDockWidget * ui;
	QStandardItemModel * _Model = nullptr;
};

END_XS_NAMESPACE

#endif//LOGGERDOCKWIDGET_H__D7F0249E_2F0C_4385_8D68_B12BF0596FF2
