/*!
 * \file	Inspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INSPECTOR_H__D0D642BB_FD51_49D3_BDCA_AF020BFAE231
#define INSPECTOR_H__D0D642BB_FD51_49D3_BDCA_AF020BFAE231

#include <XE/XE.h>
#include <QWidget>
#include <QVariant>

#include "../Widget.h"

BEG_XS_NAMESPACE

class XS_API Inspector : public Widget
{
	Q_OBJECT

public:
	Inspector( QWidget * parent = nullptr );

	~Inspector() override;

public:
	static Inspector * Create( const XE::MetaTypeCPtr & type, QWidget * parent = nullptr );

public:
	const XE::Variant & GetVariant() const;

	void SetVariant( const XE::Variant & var );

public:
	virtual void Refresh();

private:
	XE::Variant _Variant;
};

END_XS_NAMESPACE

XS_DECLARE_WIDGET( Inspector );

#endif//INSPECTOR_H__D0D642BB_FD51_49D3_BDCA_AF020BFAE231
