/*!
 * \file	LoggerEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2022/04/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LOGGEREDITOR_H__2E487B5E_13F2_48E6_99A3_8A273A3A0695
#define LOGGEREDITOR_H__2E487B5E_13F2_48E6_99A3_8A273A3A0695

#include <QStandardItemModel>

#include "EditorWindow.h"

UI_DECL( LoggerEditor );

BEG_XS_NAMESPACE

class XS_API LoggerEditor : public XS::EditorWindow
{
	Q_OBJECT

public:
	LoggerEditor( QWidget * parent = nullptr );

	~LoggerEditor();

public:
	void SaveLayout( QSettings & settings ) override;

	void LoadLayout( QSettings & settings ) override;

private slots:
	void OnErrorButtonClicked( bool checked );
	void OnWarningButtonClicked( bool checked );
	void OnInfoButtonClicked( bool checked );
	void OnClearButtonClicked( bool checked );
	void OnMergeButtonClicked( bool checked );
	void OnSearchEditingFinished();
	void OnListViewItemDoubleClicked( const QModelIndex & index );

private:
	void OnLoggerListener( std::chrono::system_clock::time_point time, const char * file, XE::uint32 line, const char * func, XE::LoggerLevel level, const XE::String & msg );

private:
	Ui::LoggerEditor * ui;
	XE::Disposable _Logger;
	QStandardItemModel * _Model = nullptr;
};

END_XS_NAMESPACE

#endif//LOGGEREDITOR_H__2E487B5E_13F2_48E6_99A3_8A273A3A0695
