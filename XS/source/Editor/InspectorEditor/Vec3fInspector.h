/*!
 * \file	Vec3fInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/31
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef VEC3FINSPECTOR_H__10CD918B_AA19_4BF4_97C9_3C895036FE02
#define VEC3FINSPECTOR_H__10CD918B_AA19_4BF4_97C9_3C895036FE02

#include <QLabel>
#include <QDoubleSpinBox>

#include "Widgets/InspectorWidget.h"

BEG_XS_NAMESPACE

class XS_API Vec3fInspector : public XS::InspectorWidget
{
	Q_OBJECT

public:
	Vec3fInspector( QWidget * parent = nullptr );

	~Vec3fInspector() override;

public:
	void Refresh() override;

private:
	QDoubleSpinBox * _X;
	QDoubleSpinBox * _Y;
	QDoubleSpinBox * _Z;
};

END_XS_NAMESPACE

#endif//VEC3FINSPECTOR_H__10CD918B_AA19_4BF4_97C9_3C895036FE02
