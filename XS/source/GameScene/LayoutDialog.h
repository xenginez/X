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

#include <random>
#include <QSize>
#include <QLabel>
#include <QVector>

#include "Global.h"

UI_DECL( LayoutDialog );

BEG_XS_NAMESPACE

class XS_API LayoutDialog : public QDialog
{
	Q_OBJECT

public:
	LayoutDialog( QSize size = { 1,1 }, QWidget * parent = nullptr );

	~LayoutDialog() override;

public:
	const QSize & GetRebuildSize() const;

	const QVector< QRect > & GetRebuildRects() const;

private slots:
	void OnLayoutSize( QSize size );

	void OnLayoutCustomSize();

private:
	void ResizeLayout( bool select );

	QColor RandomColor();

	void Rebuild();

protected:
	void showEvent( QShowEvent * e ) override;

	bool eventFilter( QObject * o, QEvent * e ) override;

private:
	Ui::LayoutDialog * ui;
	
	QSize _Size = { 1, 1 };
	QVector< QRect > _Rects;

	bool _MousePress = false;
	QPoint _MousePos = { 0, 0 };

	int _ColorIdx = 0;
	QColor _MouseColor = Qt::black;
	std::random_device _RandomDevice;
	QVector< QVector< QLabel * > > _Labels;
};

END_XS_NAMESPACE

#endif//LAYOUTDIALOG_H__40CFF3B5_C5A3_42FF_B957_FA61244AA0C5
