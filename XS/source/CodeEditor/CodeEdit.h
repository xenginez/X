/*!
 * \file	CodeEdit.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CODEEDIT_H__09E23A0C_8BE5_4C57_867E_3174788CB12A
#define CODEEDIT_H__09E23A0C_8BE5_4C57_867E_3174788CB12A

#include <QTextEdit>

#include "Core/Global.h"

BEG_XS_NAMESPACE

class CodeEdit : public QTextEdit
{
	Q_OBJECT

public:
	CodeEdit( QWidget * parent );

	~CodeEdit();

public:
	void Open( const QString & file );

	void Save() const;

	void SaveAs( const QString & file ) const;

private slots:
	void OnCustomContextMenuRequested( const QPoint & pos );

protected:
	void paintEvent( QPaintEvent * e ) override;

	void closeEvent( QCloseEvent * e ) override;
};

END_XS_NAMESPACE

#endif//CODEEDIT_H__09E23A0C_8BE5_4C57_867E_3174788CB12A
