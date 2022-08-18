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

class ContainerInspector : public XS::GroupInspector
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

protected:
	void OnRedo() override;

	void OnUndo() override;

	void OnSave() override;

private:
	XE::Variant _Container;
	Ui::ContainerInspector * ui;
	XE::MetaEnumeratorPtr _Enumerator;
};

END_XS_NAMESPACE

#endif//CONTAINERINSPECTOR_H__470DFEDA_AEE3_4775_ADAF_190A02C7E7D5
