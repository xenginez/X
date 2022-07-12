/*!
 * \file   StatusBar.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/27
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef STATUSBAR_H__04C24BF7_8371_4271_A4AF_F91B69785F7D
#define STATUSBAR_H__04C24BF7_8371_4271_A4AF_F91B69785F7D

#include <QStatusBar>

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API StatusBar : public QStatusBar
{
	Q_OBJECT

public:
	StatusBar( QWidget * parent = nullptr );

	~StatusBar() override;

public:

};

END_XS_NAMESPACE

#endif // STATUSBAR_H__04C24BF7_8371_4271_A4AF_F91B69785F7D
