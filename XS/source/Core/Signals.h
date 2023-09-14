/*!
 * \file	Signals.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef SIGNALS_H__24D9B1CB_E318_4F40_A6CA_7F0334DDCD81
#define SIGNALS_H__24D9B1CB_E318_4F40_A6CA_7F0334DDCD81

#include "Global.h"

BEG_XS_NAMESPACE

class Signals : public QObject
{
	Q_OBJECT

public:
	Signals();

	~Signals();

signals:
	void Inspector( const QString & name, const XE::Variant & val );
};
using SignalsPtr = Signals *;

END_XS_NAMESPACE

#endif//SIGNALS_H__24D9B1CB_E318_4F40_A6CA_7F0334DDCD81
