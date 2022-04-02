/*!
 * \file	MenuBar.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/17
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MENUBAR_H__27F5708F_6731_4113_81AD_3DF76EC85399
#define MENUBAR_H__27F5708F_6731_4113_81AD_3DF76EC85399

#include <QLabel>
#include <QMenuBar>
#include <QToolButton>

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API MenuBar : public QMenuBar
{
	Q_OBJECT

public:
	MenuBar( QWidget * parent = nullptr );

	~MenuBar();

public:
	void showRestoreButton();
	void hideRestoreButton();
	bool RestoreButtonVisable() const;
	void setRestoreButtonVisable( bool val );

	void showMinimizeButton();
	void hideMinimizeButton();
	bool MinimizeButtonVisable() const;
	void setMinimizeButtonVisable( bool val );

	void showMaximizeButton();
	void hideMaximizeButton();
	bool MaximizeButtonVisable() const;
	void setMaximizeButtonVisable( bool val );

signals:
	void CloseButtonClicked();
	void MenuBarDoubleClicked();
	void RestoreButtonClicked();
	void MinimizeButtonClicked();
	void MaximizeButtonClicked();

protected:
	void paintEvent( QPaintEvent * event ) override;
	void mousePressEvent( QMouseEvent * event ) override;
	void mouseMoveEvent( QMouseEvent * event ) override;
	void mouseReleaseEvent( QMouseEvent * event ) override;
	void mouseDoubleClickEvent( QMouseEvent * event ) override;

private:
	bool _MoveFlag = false;
	QPoint _MovePos = QPoint( 0, 0 );

	QLabel * _Title = nullptr;
	QToolButton * _Close = nullptr;
	QToolButton * _Restore = nullptr;
	QToolButton * _Minimize = nullptr;
	QToolButton * _Maximize = nullptr;
};

END_XS_NAMESPACE

#endif//MENUBAR_H__27F5708F_6731_4113_81AD_3DF76EC85399
