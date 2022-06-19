/*!
 * \file	AudioListener.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AUDIOLISTENER_H__2711A0C0_2A2B_4D48_969C_C725E1B1BCD0
#define AUDIOLISTENER_H__2711A0C0_2A2B_4D48_969C_C725E1B1BCD0

#include "Scene/GameObject.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API AudioListener : public XE::GameObjectComponent
{
	OBJECT( AudioListener, XE::GameObjectComponent )

public:
	AudioListener();

	~AudioListener() override;

protected:
	void OnStartup();

	void OnUpdate( XE::float32 dt );

	void OnClearup();

};

END_XE_NAMESPACE

#endif//AUDIOLISTENER_H__2711A0C0_2A2B_4D48_969C_C725E1B1BCD0
