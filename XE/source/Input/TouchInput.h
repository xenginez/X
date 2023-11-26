/*!
 * \file	TouchInput.h
 *
 * \author	ZhengYuanQing
 * \date	2023/11/26
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef TOUCHINPUT_H__FEDDCF4F_BC57_434A_9F3A_673287A45038
#define TOUCHINPUT_H__FEDDCF4F_BC57_434A_9F3A_673287A45038

#include "InputItem.h"

BEG_XE_NAMESPACE

class XE_API TouchInput : XE::InputItem
{
	OBJECT( TouchInput, InputItem )

private:
	struct Private;

public:
	TouchInput();

	~TouchInput() override;

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

#endif//TOUCHINPUT_H__FEDDCF4F_BC57_434A_9F3A_673287A45038