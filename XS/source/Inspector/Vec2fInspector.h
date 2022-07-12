/*!
 * \file	Vec2fInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/31
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef VEC2FINSPECTOR_H__71BEADD7_389B_415C_9ED4_B7F28EE27F72
#define VEC2FINSPECTOR_H__71BEADD7_389B_415C_9ED4_B7F28EE27F72

#include <QLabel>
#include <QDoubleSpinBox>

#include "Core/Core.h"

BEG_XS_NAMESPACE

class XS_API Vec2fInspector : public XS::Inspector
{
	Q_OBJECT

public:
	Vec2fInspector( QWidget * parent = nullptr );

	~Vec2fInspector() override;

public:
	void Refresh() override;

private:
	QDoubleSpinBox * _X;
	QDoubleSpinBox * _Y;
};

END_XS_NAMESPACE

#endif//VEC2FINSPECTOR_H__71BEADD7_389B_415C_9ED4_B7F28EE27F72
