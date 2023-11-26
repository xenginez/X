/*!
 * \file	IntegerInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INTEGERINSPECTOR_H__00781C6F_84CD_41BD_B9C5_B22B97D44D00
#define INTEGERINSPECTOR_H__00781C6F_84CD_41BD_B9C5_B22B97D44D00

#include <QSpinBox>

#include "Widgets/InspectorWidget.h"

BEG_XS_NAMESPACE

class XS_API IntegerInspector : public XS::InspectorWidget
{
	Q_OBJECT

public:
	IntegerInspector( QWidget * parent = nullptr );

	~IntegerInspector() override;

public:
	void Refresh() override;

private:
	QSpinBox * _QSpinBox;
};

END_XS_NAMESPACE

#endif//INTEGERINSPECTOR_H__00781C6F_84CD_41BD_B9C5_B22B97D44D00
