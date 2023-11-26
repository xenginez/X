/*!
 * \file	Vec4fInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/31
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef VEC4FINSPECTOR_H__4D5AAA93_14D9_40B2_8CE7_7187F16AE630
#define VEC4FINSPECTOR_H__4D5AAA93_14D9_40B2_8CE7_7187F16AE630

#include <QLabel>
#include <QDoubleSpinBox>

#include "Widgets/InspectorWidget.h"

BEG_XS_NAMESPACE

class XS_API Vec4fInspector : public XS::InspectorWidget
{
	Q_OBJECT

public:
	Vec4fInspector( QWidget * parent = nullptr );

	~Vec4fInspector() override;

public:
	void Refresh() override;

private:
	QDoubleSpinBox * _X;
	QDoubleSpinBox * _Y;
	QDoubleSpinBox * _Z;
	QDoubleSpinBox * _W;
};

END_XS_NAMESPACE

#endif//VEC4FINSPECTOR_H__4D5AAA93_14D9_40B2_8CE7_7187F16AE630
