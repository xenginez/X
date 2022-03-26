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

#include <QSettings>
#include <QUndoGroup>
#include <QMainWindow>

#include "Global.h"

namespace Ui
{
	class FramelessWindow;
}

BEG_XS_NAMESPACE

class XS_API FramelessWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit FramelessWindow( QWidget * parent = Q_NULLPTR );

	virtual ~FramelessWindow();

public:
	bool setContent( QWidget * w );

	void removeCentralwidget();

public:
	virtual void Save( QSettings & settings );
	virtual void Load( QSettings & settings );

public:
	void AddUndoStack( QUndoStack * val );
	void RemoveUndoStack( QUndoStack * val );

public:
	void enableWindowIcon();
	void disableWindowIcon();
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

public slots:
	void setWindowIcon( const QIcon & ico );
	void setWindowTitle( const QString & text );

private slots:
	void OnCloseButtonClicked();
	void OnMenuBarDoubleClicked();
	void OnRestoreButtonClicked();
	void OnMinimizeButtonClicked();
	void OnMaximizeButtonClicked();

private:
	Ui::FramelessWindow * ui;

private:
	QUndoGroup * _UndoGroup = nullptr;
};
END_XS_NAMESPACE

#endif//FRAMELESSWINDOW_H__60C1E323_2D82_4DE8_999B_76CEECE65608
