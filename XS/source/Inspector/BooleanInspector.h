/*!
 * \file	BooleanInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/28
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef BOOLEANINSPECTOR_H__94C48987_86EF_4B3F_8555_5B2B81875D23
#define BOOLEANINSPECTOR_H__94C48987_86EF_4B3F_8555_5B2B81875D23

#include <QCheckBox>

#include "Core/Inspector.h"

BEG_XS_NAMESPACE

class XS_API BooleanInspector : public XS::Inspector
{
	Q_OBJECT

public:
	BooleanInspector( QWidget * parent = nullptr );

	~BooleanInspector() override;

public:
	void Refresh() override;

private:
	QCheckBox * _CheckBox;
};

END_XS_NAMESPACE

#endif//BOOLEANINSPECTOR_H__94C48987_86EF_4B3F_8555_5B2B81875D23
