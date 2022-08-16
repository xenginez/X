/*!
 * \file	ContainerInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONTAINERINSPECTOR_H__470DFEDA_AEE3_4775_ADAF_190A02C7E7D5
#define CONTAINERINSPECTOR_H__470DFEDA_AEE3_4775_ADAF_190A02C7E7D5

#include "Inspector.h"

UI_DECL( ContainerInspector );
BEG_XS_NAMESPACE

class ContainerInspector : public XS::Inspector
{
	Q_OBJECT

public:
	ContainerInspector( QWidget * parent = nullptr );

	~ContainerInspector() override;

public:
	void Refresh() override;

private slots:
	void OnAddToolButtonClicked();

	void OnSubToolButtonClicked();

private:
	XE::VariantArray _Array;
	Ui::ContainerInspector * ui;
};

END_XS_NAMESPACE

#endif//CONTAINERINSPECTOR_H__470DFEDA_AEE3_4775_ADAF_190A02C7E7D5
