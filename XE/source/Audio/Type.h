/*!
 * \file	Type.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__A935D3BC_1222_4AE4_97BD_7B25C63E0BFF
#define TYPE_H__A935D3BC_1222_4AE4_97BD_7B25C63E0BFF

#include <array>

#include "Utils/Buffer.h"
#include "Utils/Handle.hpp"
#include "Utils/Concurrent.hpp"

#include "Declare.h"

BEG_XE_NAMESPACE

static constexpr XE::uint64 AUDIO_MAX_FRAME = 2;
static constexpr XE::uint64 AUDIO_MAX_SOURCE = 512;
static constexpr XE::uint64 AUDIO_MAX_BUFFER = 1024;
static constexpr XE::uint64 AUDIO_MAX_COMMAND = 2048;


DECL_HANDLE( AudioBuffer );
DECL_HANDLE( AudioSource );

enum class AudioSourceState
{
	PLAY,
	STOP,
	PAUSE,
};

enum class AudioCommandType
{
	PLAY,

	GAIN,
	SPEED_OF_SOUND,
	DOPPLER_FACTOR,

	CREATE_BUFFER,
	CREATE_SOURCE,

	LISTENER_TRANSFORM,

	BUFFER_FILL,

	SOURCE_QUEUE_BUFFER,
	SOURCE_UNQUE_BUFFER,
	SOURCE_PLAY,
	SOURCE_GAIN,
	SOURCE_LOOP,
	SOURCE_PITCH,
	SOURCE_TRANSFORM,
	SOURCE_GAIN_RANGE,
	SOURCE_DISTANCE_RANGE,
	SOURCE_CONEANGLE_RANGE,
	SOURCE_PAUSE,

	CAPTURE_START,
	CAPTURE_SAMPLES,

	SOURCE_STOP,
	CAPTURE_STOP,

	PAUSE,

	DESTROY_BUFFER,
	DESTROY_SOURCE,
};


struct XE_API AudioCommand
{
	AudioCommandType Type;
	XE::uint64 Handle;
	XE::uint64 Code;
	XE::uint64 Size;
};

struct XE_API AudioFrame
{
public:
	void Reset()
	{
		CommandSize = 0;
		Buffers.unsafe().Clear();
	}

public:
	std::mutex Mutex;
	std::atomic< XE::uint64 > CommandSize = 0;
	std::array< AudioCommand, AUDIO_MAX_COMMAND > Command;
	XE::Concurrent< XE::Buffer, std::mutex > Buffers;
};

struct XE_API AudioBufferDesc
{
	XE::String Name;
	XE::uint32 Bits = 8;
	XE::uint32 Channels = 1;
	XE::uint32 Frequency = 44100;
};

struct XE_API AudioSourceDesc
{
	XE::String Name;

	bool Pause = true;
	bool Looping = false;
	XE::float32 Gain = 0.0f;
	XE::float32 Pitch = 1.0f;
	XE::float32 MinGain = 0.0f;
	XE::float32 MaxGain = 1.0f;
	XE::float32 MinDistance = 0.0f;
	XE::float32 MaxDistance = 1.0f;
	XE::float32 InnerConeAngle = 360.0f;
	XE::float32 OuterConeAngle = 360.0f;
};


using UnqueBufferCallbackType = void( * )( void * userdata, XE::AudioBufferHandle buffer );
using CaptureBufferCallbackType = void( * )( void * userdata, XE::MemoryView view );

END_XE_NAMESPACE

#endif//TYPE_H__A935D3BC_1222_4AE4_97BD_7B25C63E0BFF
