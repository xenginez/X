/*!
 * \file	MouseInput.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MOUSEINPUT_H__88723973_0CD0_4167_BEDE_461BD84ED43E
#define MOUSEINPUT_H__88723973_0CD0_4167_BEDE_461BD84ED43E

#include "InputItem.h"

BEG_XE_NAMESPACE

class XE_API MouseInput : public XE::InputItem
{
	OBJECT( MouseInput, XE::InputItem );

private:
	struct Private;

public:
	MouseInput();

	~MouseInput() override;

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

#endif//MOUSEINPUT_H__88723973_0CD0_4167_BEDE_461BD84ED43E
