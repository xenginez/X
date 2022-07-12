/*!
 * \file	FloatingInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FLOATINGINSPECTOR_H__C2A2212D_70EE_4666_BB15_439BDA467BB6
#define FLOATINGINSPECTOR_H__C2A2212D_70EE_4666_BB15_439BDA467BB6

#include <QDoubleSpinBox>

#include "Core/Core.h"

BEG_XS_NAMESPACE

class XS_API FloatingInspector : public Inspector
{
	Q_OBJECT

public:
	FloatingInspector( QWidget * parent = nullptr );

	~FloatingInspector() override;

public:
	void Refresh() override;

private:
	QDoubleSpinBox * _QDoubleSpinBox;
};

END_XS_NAMESPACE

#endif//FLOATINGINSPECTOR_H__C2A2212D_70EE_4666_BB15_439BDA467BB6
