/*!
 * \file	StringInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef STRINGINSPECTOR_H__F1A46D38_BDC8_4FC9_B8D4_9A2FADD989F0
#define STRINGINSPECTOR_H__F1A46D38_BDC8_4FC9_B8D4_9A2FADD989F0

#include <QLineEdit>

#include "Inspector.h"

BEG_XS_NAMESPACE

class XS_API StringInspector : public XS::Inspector
{
	Q_OBJECT

public:
	StringInspector( QWidget * parent = nullptr );

	~StringInspector() override;

public:
	void Refresh() override;

private:
	QLineEdit * _QLineEdit;
};

END_XS_NAMESPACE

#endif//STRINGINSPECTOR_H__F1A46D38_BDC8_4FC9_B8D4_9A2FADD989F0
