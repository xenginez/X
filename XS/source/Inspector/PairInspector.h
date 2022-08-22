/*!
 * \file	PairInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PAIRINSPECTOR_H__09EAE13A_13DC_4CBF_8819_C0741AA34B19
#define PAIRINSPECTOR_H__09EAE13A_13DC_4CBF_8819_C0741AA34B19

#include <QFormLayout>

#include "Inspector.h"

UI_DECL( PairInspector );

BEG_XS_NAMESPACE

class XS_API PairInspector : public XS::Inspector
{
	Q_OBJECT

public:
	PairInspector( QWidget * parent = nullptr );

	~PairInspector() override;

public:
	void Refresh() override;

private:
	QFormLayout * _Layout;
};

END_XS_NAMESPACE

#endif//PAIRINSPECTOR_H__09EAE13A_13DC_4CBF_8819_C0741AA34B19
