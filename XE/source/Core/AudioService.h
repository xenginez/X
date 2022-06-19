/*!
 * \file	AudioService.h
 *
 * \author	ZhengYuanQing
 * \date	2021/10/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AUDIOSERVICE_H__6493958D_0974_419F_9E77_7AC16A850EDB
#define AUDIOSERVICE_H__6493958D_0974_419F_9E77_7AC16A850EDB

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API AudioService : public XE::Service
{
	OBJECT( AudioService, Service )

private:
	struct Private;

public:
	AudioService();

	~AudioService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	void Play();

	void Pause();

	bool IsPause() const;

public:
	XE::float32 GetGain() const;

	void SetGain( XE::float32 val );

	XE::float32 GetSpeedOfSound() const;

	void SetSpeedOfSound( XE::float32 val );

	XE::float32 GetDopplerFactor() const;

	void SetDopplerFactor( XE::float32 val );

public:
	AudioBufferHandle Create( const XE::AudioBufferDesc & desc );

	AudioSourceHandle Create( const XE::AudioSourceDesc & desc );

public:
	const XE::AudioBufferDesc & GetDesc( XE::AudioBufferHandle handle ) const;

	const XE::AudioSourceDesc & GetDesc( XE::AudioSourceHandle handle ) const;

public:
	void ListenerTransform( const XE::Vec3f & position, const XE::Vec3f & velocity, const XE::Vec3f & orientation, const XE::Vec3f & up = XE::Vec3f::Up );

public:
	void BufferRefill( AudioBufferHandle handle, XE::MemoryView data );

public:
	void SourceQueueBuffer( AudioSourceHandle handle, AudioBufferHandle buffer );

	void SourceUnqueBuffer( AudioSourceHandle handle, void * userdata, UnqueBufferCallbackType callback );

	void SourcePlay( AudioSourceHandle handle );

	void SourcePause( AudioSourceHandle handle );

	void SourceStop( AudioSourceHandle handle );

	void SourceRelative( AudioSourceHandle handle, bool relative );

	void SourceConeAngle( AudioSourceHandle handle, XE::float32 inner, XE::float32 outer );

	void SourcePitch( AudioSourceHandle handle, XE::float32 pitch );

	void SourceGain( AudioSourceHandle handle, XE::float32 min, XE::float32 max );

	void SourceDistance( AudioSourceHandle handle, XE::float32 min, XE::float32 max );

	void SourceTransform( AudioSourceHandle handle, const XE::Vec3f & position, const XE::Vec3f & verlocity, const XE::Vec3f & orientation, const XE::Vec3f & up = XE::Vec3f::Up );

public:
	void Destroy( AudioBufferHandle handle );

	void Destroy( AudioSourceHandle handle );

public:
	bool IsCapture() const;

	void CaptureStart( XE::uint32 Bits, XE::uint32 Channels, XE::uint32 Frequency, ReadBufferCallbackType callback );

	void CaptureStop();

protected:
	void OnUpdate( XE::AudioFrame * frame );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // AUDIOSERVICE_H__6493958D_0974_419F_9E77_7AC16A850EDB
