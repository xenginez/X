/*!
 * \file	PadInput.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/06
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PADINPUT_H__521F2F7A_5CAC_4A6C_A35F_2C5557708AE7
#define PADINPUT_H__521F2F7A_5CAC_4A6C_A35F_2C5557708AE7

#include "InputItem.h"

BEG_XE_NAMESPACE

class XE_API PadInput : public XE::InputItem
{
	OBJECT( PadInput, XE::InputItem );

private:
	struct Private;

public:
	PadInput();

	~PadInput() override;

public:
	void Startup() override;

	void Update() override;

	void Clearup() override;

	void ClearStatus() override;

	const XE::InputState & GetState() const override;

public:
	bool Vibrate( XE::uint32 index, XE::float32 left, XE::float32 right ) override;

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//PADINPUT_H__521F2F7A_5CAC_4A6C_A35F_2C5557708AE7
