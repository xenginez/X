/*!
 * \file	TypeInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/22
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPEINSPECTOR_H__DDCE7BE4_4D47_4B3B_98DA_5DB8D8753739
#define TYPEINSPECTOR_H__DDCE7BE4_4D47_4B3B_98DA_5DB8D8753739

#include <QLineEdit>
#include <QToolButton>

#include "Inspector.h"

BEG_XS_NAMESPACE

class XS_API TypeInspector : public XS::Inspector
{
	Q_OBJECT

public:
	TypeInspector( QWidget * parent = nullptr );

	~TypeInspector() override;

public:
	void Refresh() override;

private slots:
	void OnToolButtonClicked();

private:
	QLineEdit * _Edit;
	QToolButton * _Button;
};

END_XS_NAMESPACE

#endif//TYPEINSPECTOR_H__DDCE7BE4_4D47_4B3B_98DA_5DB8D8753739
