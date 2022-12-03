/*!
 * \file	Input.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INPUT_H__9CFA0460_9BAF_40D3_8CEE_09031B242A54
#define INPUT_H__9CFA0460_9BAF_40D3_8CEE_09031B242A54

#include "InputState.h"

BEG_XE_NAMESPACE

class XE_API Input : public XE::EnableSharedFromThis< Input >
{
	OBJECT( Input );

public:
	Input();

	virtual ~Input();

public:
	virtual void Startup() = 0;

	virtual void Update() = 0;

	virtual void Clearup() = 0;

	virtual void ClearStatus() = 0;

	virtual const XE::InputState & GetState() const = 0;

public:
	virtual bool Vibrate( XE::uint32 index, XE::float32 left, XE::float32 right ) = 0;
};

END_XE_NAMESPACE

#endif//INPUT_H__9CFA0460_9BAF_40D3_8CEE_09031B242A54
