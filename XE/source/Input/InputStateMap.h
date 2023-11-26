/*!
 * \file	InputStateMap.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/06
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INPUTSTATEMAP_H__B5863FE2_70B8_477B_994A_67E2CAE93A5C
#define INPUTSTATEMAP_H__B5863FE2_70B8_477B_994A_67E2CAE93A5C

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API InputStateMap : public XE::EnableSharedFromThis< InputStateMap >
{
	OBJECT( InputStateMap );

private:
	struct Private;

public:
	InputStateMap();

	~InputStateMap();

public:
	void Startup();

	void Update( const XE::InputState & state );

	void Clearup();

	void ClearStatus();

public:
	const XE::InputState & GetState() const;

private:
	const XE::Map< XE::String, XE::Array< XE::InputStateItemPtr > > & GetStateItems() const;

	void SetStateItems( const XE::Map< XE::String, XE::Array< XE::InputStateItemPtr > > & val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//INPUTSTATEMAP_H__B5863FE2_70B8_477B_994A_67E2CAE93A5C
