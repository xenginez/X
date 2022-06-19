/*!
 * \file	AudioSource.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AUDIOSOURCE_H__257958B9_2907_4666_8442_0371E54B3689
#define AUDIOSOURCE_H__257958B9_2907_4666_8442_0371E54B3689

#include "Scene/GameObject.h"

#include "AudioBuffer.h"

BEG_XE_NAMESPACE

class XE_API AudioSource : public XE::GameObjectComponent
{
	OBJECT( AudioSource, XE::GameObjectComponent )

public:
	AudioSource();

	~AudioSource() override;

protected:
	void OnStartup() override;

	void OnUpdate( XE::float32 dt ) override;

	void OnClearup() override;

public:
	const XE::AudioBuffer & GetSource() const;

	void SetSource( const XE::AudioBuffer & val );

	bool GetLoop() const;

	void SetLoop( bool val );

	XE::float32 GetGain() const;

	void SetGain( XE::float32 val );

	XE::float32 GetPitch() const;

	void SetPitch( XE::float32 val );

	XE::float32 GetMinGain() const;

	void SetMinGain( XE::float32 val );

	XE::float32 GetMaxGain() const;

	void SetMaxGain( XE::float32 val );

	XE::float32 GetMinDistance() const;

	void SetMinDistance( XE::float32 val );

	XE::float32 GetMaxDistance() const;

	void SetMaxDistance( XE::float32 val );

	XE::float32 GetInnerConeAngle() const;

	void SetInnerConeAngle( XE::float32 val );

	XE::float32 GetOuterConeAngle() const;

	void SetOuterConeAngle( XE::float32 val );

public:
	void Play();

	void Stop();

	void Pause();

	bool IsPlay();

	bool IsStop();

	bool IsPause();

private:
	XE::AudioBuffer _Audio;

	bool _Pause = false;
	bool _Looping = false;
	XE::float32 _Gain = 1.0f;
	XE::float32 _Pitch = 1.0f;
	XE::float32 _MinGain = 0.0f;
	XE::float32 _MaxGain = 1.0f;
	XE::float32 _MinDistance = 0.0f;
	XE::float32 _MaxDistance = 1.0f;
	XE::float32 _InnerConeAngle = 360.0f;
	XE::float32 _OuterConeAngle = 360.0f;

	XE::AudioSourceState _State = XE::AudioSourceState::PLAY;
	XE::AudioBufferHandle _Buffer;
	XE::AudioSourceHandle _Source;
};

END_XE_NAMESPACE

#endif//AUDIOSOURCE_H__257958B9_2907_4666_8442_0371E54B3689
