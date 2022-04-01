/*!
 * \file	FlagInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FLAGINSPECTOR_H__1513F0F7_1D29_4627_86DA_D804E90866CF
#define FLAGINSPECTOR_H__1513F0F7_1D29_4627_86DA_D804E90866CF

#include <QComboBox>

#include "../Inspector.h"

BEG_XS_NAMESPACE

class XS_API FlagInspector : public Inspector
{
	Q_OBJECT

public:
	FlagInspector( QWidget * parent = nullptr );

	~FlagInspector() override;

public:
	void Refresh() override;

protected:
	bool eventFilter( QObject * watched, QEvent * event ) override;

private:
	void RefreshComboBox( XE::uint64 flag );

private:
	QComboBox * _QComboBox;
};

END_XS_NAMESPACE

#endif//FLAGINSPECTOR_H__1513F0F7_1D29_4627_86DA_D804E90866CF
