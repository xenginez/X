/*!
 * \file	EnumInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ENUMINSPECTOR_H__492D3989_8359_4ED9_A14A_EF5939AF48C0
#define ENUMINSPECTOR_H__492D3989_8359_4ED9_A14A_EF5939AF48C0

#include <QComboBox>

#include "../Inspector.h"

BEG_XS_NAMESPACE

class XS_API EnumInspector : public Inspector
{
	Q_OBJECT

public:
	EnumInspector( QWidget * parent = nullptr );

	~EnumInspector() override;

public:
	void Refresh() override;

private:
	QComboBox * _QComboBox;
};

END_XS_NAMESPACE

#endif//ENUMINSPECTOR_H__492D3989_8359_4ED9_A14A_EF5939AF48C0
