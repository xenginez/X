/*!
 * \file	CodeStyle.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/26
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CODESTYLE_H__27318DF2_2E82_4A35_82F8_34047CBAB186
#define CODESTYLE_H__27318DF2_2E82_4A35_82F8_34047CBAB186

#include "Core/Global.h"

BEG_XS_NAMESPACE

class CodeStyleItem : public QObject
{
	Q_OBJECT

	Q_PROPERTY( QFont font READ font WRITE setFont );
	Q_PROPERTY( QColor backColor READ backColor WRITE setBackColor );
	Q_PROPERTY( QColor foreColor READ foreColor WRITE setForeColor );

public:
	QFont font() const { return _font; }
	void setFont( const QFont & val ) { _font = val; }
	QColor backColor() const { return _backColor; }
	void setBackColor( QColor val ) { _backColor = val; }
	QColor foreColor() const { return _foreColor; }
	void setForeColor( QColor val ) { _foreColor = val; }

private:
	QFont _font;
	QColor _backColor;
	QColor _foreColor;
};

class CodeStyle : public QObject
{
	Q_OBJECT

public:
	CodeStyle( QObject * parent );
	~CodeStyle();

public:
	QString save() const;

	void load( const QString & val );

public:
	QColor backColor;
	QColor foreColor;
};

END_XS_NAMESPACE

#endif//CODESTYLE_H__27318DF2_2E82_4A35_82F8_34047CBAB186