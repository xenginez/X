/*!
 * \file	framelesswindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FRAMELESSWINDOW_H__60C1E323_2D82_4DE8_999B_76CEECE65608
#define FRAMELESSWINDOW_H__60C1E323_2D82_4DE8_999B_76CEECE65608

#include <QMap>
#include <QShortcut>
#include <QSettings>
#include <QUndoGroup>
#include <QMainWindow>
#include <QKeySequence>

#include "Global.h"

namespace Ui
{
	class MainWindow;
}

BEG_XS_NAMESPACE

class XS_API MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow( QWidget * parent = Q_NULLPTR );

	virtual ~MainWindow();

public:
	std::thread::id GetEditorThreadId() const;

public:
	bool SetContent( QWidget * w );
	void SetWindowIcon( const QIcon & ico );
	void SetWindowTitle( const QString & text );
	void RemoveCentralwidget();

public:
	void Save();
	void Load();

public:
	void AddUndoStack( QUndoStack * val );
	void RemoveUndoStack( QUndoStack * val );
	QShortcut * AddShortcuts( const QString & name, const QKeySequence & key, QWidget * widget );

public:
	void enableRestoreButton();
	void disableRestoreButton();
	void enableMinimizeButton();
	void disableMinimizeButton();
	void enableMaximizeButton();
	void disableMaximizeButton();

public:
	void show();
	void showMinimized();
	void showMaximized();

private slots:
	void OnCloseButtonClicked();
	void OnMenuBarDoubleClicked();
	void OnRestoreButtonClicked();
	void OnMinimizeButtonClicked();
	void OnMaximizeButtonClicked();

private:
	void SaveLayout();
	void LoadLayout();
	void SaveShortcuts();
	void LoadShortcuts();

private:
	Ui::MainWindow * ui;

private:
	std::thread::id _MainThreadID;
	QUndoGroup * _UndoGroup = nullptr;
	QMap< QString, QShortcut * > _Shortcuts;
	QMap< QString, QKeySequence > _KeySequences;
};

END_XS_NAMESPACE

#endif//FRAMELESSWINDOW_H__60C1E323_2D82_4DE8_999B_76CEECE65608
