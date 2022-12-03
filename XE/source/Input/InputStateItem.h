/*!
 * \file	InputStateItem.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/06
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INPUTSTATEITEM_H__42025678_2B4B_4501_8DEA_C049CB385797
#define INPUTSTATEITEM_H__42025678_2B4B_4501_8DEA_C049CB385797

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API InputStateItem : public XE::EnableSharedFromThis< InputStateItem >
{
	OBJECT( InputStateItem );

public:
	InputStateItem();

	~InputStateItem();

public:
	XE::float32 GetTimeout() const;

	const XE::String & GetInputName() const;

	XE::InputStateItemTimeoutType GetTimeoutType() const;

public:
	virtual bool Condition( const XE::Variant & val ) const = 0;

private:
	XE::String _Input;
	XE::float32 _Timeout;
	XE::InputStateItemTimeoutType _Type;
};

class XE_API InputStateRangeItem : public XE::InputStateItem
{
	OBJECT( InputStateRangeItem, XE::InputStateItem );

public:
	InputStateRangeItem();

	~InputStateRangeItem();

public:
	XE::float32 GetMin() const;

	XE::float32 GetMax() const;

public:
	bool Condition( const XE::Variant & val ) const override;

private:
	XE::float32 _Min = 0.0f;
	XE::float32 _Max = 1.0f;
};

class XE_API InputStateButtonItem : public XE::InputStateItem
{
	OBJECT( InputStateButtonItem, XE::InputStateItem );

public:
	InputStateButtonItem();

	~InputStateButtonItem();

public:
	bool GetPressed() const;

public:
	bool Condition( const XE::Variant & val ) const override;

private:
	bool _Pressed = false;
};

END_XE_NAMESPACE

#endif//INPUTSTATEITEM_H__42025678_2B4B_4501_8DEA_C049CB385797
