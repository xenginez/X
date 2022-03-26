/*!
 * \file	KeyboardInput.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef KEYBOARDINPUT_H__1752AAC6_7327_48B2_8D15_2B5A8BEF631C
#define KEYBOARDINPUT_H__1752AAC6_7327_48B2_8D15_2B5A8BEF631C

#include "Input.h"

BEG_XE_NAMESPACE

class XE_API KeyboardInput : public XE::Input
{
	OBJECT( KeyboardInput, XE::Input );

private:
	struct Private;

public:
	KeyboardInput();

	~KeyboardInput() override;

public:
	void Startup() override;

	void Update() override;

	void Clearup() override;

	void ClearStatus() override;

	const XE::InputState & GetState() const override;

public:
	bool Vibrate( XE::uint32 index, XE::float32 left, XE::float32 right ) override { return false; }

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//KEYBOARDINPUT_H__1752AAC6_7327_48B2_8D15_2B5A8BEF631C
