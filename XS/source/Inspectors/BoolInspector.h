/*!
 * \file	BoolInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/25
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef BOOLINSPECTOR_H__24EE26F5_4C25_4624_B1D1_31EB4C01E611
#define BOOLINSPECTOR_H__24EE26F5_4C25_4624_B1D1_31EB4C01E611

#include <QCheckBox>

#include "../Inspector.h"

BEG_XS_NAMESPACE

class XS_API BoolInspector : public Inspector
{
	Q_OBJECT

public:
	BoolInspector( QWidget * parent = nullptr );

	~BoolInspector() override;

public:
	void Refresh() override;

private:
	QCheckBox * _CheckBox;
};

END_XS_NAMESPACE

#endif//BOOLINSPECTOR_H__24EE26F5_4C25_4624_B1D1_31EB4C01E611
