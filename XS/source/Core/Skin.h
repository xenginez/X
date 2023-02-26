/*!
 * \file	Skin.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SKIN_H__7ECE8068_69E4_4067_B50F_F46CE136C35B
#define SKIN_H__7ECE8068_69E4_4067_B50F_F46CE136C35B

#include <QVector>
#include <QProxyStyle>

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API Skin : public QProxyStyle
{
	Q_OBJECT

public:
	Skin();

	Skin( const QString & path );

	~Skin();

public:
	QStyle * baseStyle() const;

	void polish( QPalette & palette ) override;

	void polish( QApplication * app ) override;

private:
	QStyle * styleBase( QStyle * style = Q_NULLPTR ) const;

private:
	QString _QSS;
	QString _ImagePath;
	QVector< std::tuple< QPalette::ColorRole, QPalette::ColorGroup, QColor > > _PaletteColors;
};

END_XS_NAMESPACE

#endif//SKIN_H__7ECE8068_69E4_4067_B50F_F46CE136C35B
