#include "AudioService.h"

#include <openal-soft/AL/al.h>
#include <openal-soft/AL/alc.h>

#include "Utils/Logger.h"
#include "Utils/RefCount.h"
#include "Core/CoreFramework.h"
#include "Core/ThreadService.h"

BEG_META( XE::AudioService )
type->Property( "SpeedOfSound", &XE::AudioService::GetSpeedOfSound, &XE::AudioService::SetSpeedOfSound );
type->Property( "DopplerFactor", &XE::AudioService::GetDopplerFactor, &XE::AudioService::SetDopplerFactor )->Attribute( XE::RangeAttribute( 0.0f, 1.0f ) );
END_META()

namespace
{
	ALCenum AL_Format( XE::uint32 bits, XE::uint32 channels )
	{
		ALCenum format = AL_FORMAT_MONO8;

		if ( bits == 8 )
		{
			format = channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		}
		else if ( bits == 16 )
		{
			format = channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		}

		return format;
	}

	void AL_Error()
	{
		ALenum err = alGetError();
		if ( err != AL_NO_ERROR )
		{
			switch ( err )
			{
			case AL_NO_ERROR:
				break;
			case AL_INVALID_NAME:
				XE_ERROR( "Invalid name paramater passed to AL call." );
				break;
			case AL_INVALID_ENUM:
				XE_ERROR( "Invalid enum parameter passed to AL call." );
				break;
			case AL_INVALID_VALUE:
				XE_ERROR( "Invalid value parameter passed to AL call." );
				break;
			case AL_INVALID_OPERATION:
				XE_ERROR( "Illegal AL call." );
				break;
			case AL_OUT_OF_MEMORY:
				XE_ERROR( "Not enough memory." );
				break;
			default:
				break;
			}
		}
	}

	struct AHIBuffer : public XE::RefCount
	{
		ALuint Buffer;
		XE::AudioBufferDesc Desc;
		XE::AudioBufferHandle Handle;
	};

	struct AHISource : public XE::RefCount
	{
		ALuint Source;
		XE::AudioSourceDesc Desc;
		XE::AudioSourceHandle Handle;
		std::deque<XE::AudioBufferHandle> UnqueBuffers;
	};
}

struct XE::AudioService::Private
{
	std::mutex _FrameMutex;
	XE::uint64 _SubmitFrame = 0;
	std::array< XE::AudioFrame, XE::AUDIO_MAX_FRAME > _Frames;

	bool _Pause = false;
	XE::float32 _Gain = 1.0f;
	XE::float32 _SpeedOfSound = 343.3f;
	XE::float32 _DopplerFactor = 1.0f;

	ALCdevice * _Device = nullptr;
	ALCcontext * _Context = nullptr;
	ALCdevice * _CaptureDevice = nullptr;

	bool _Capture = false;
	ALCint _CaptureSample = 0;
	XE::Array< XE::int8 > _CaptureBuffer;

	std::array<AHIBuffer, XE::AUDIO_MAX_BUFFER > _Buffers;
	std::array<AHISource, XE::AUDIO_MAX_SOURCE > _Sources;
	XE::QueueHandleAllocator< XE::AudioBufferHandle, AUDIO_MAX_BUFFER > _BufferHandleAlloc;
	XE::QueueHandleAllocator< XE::AudioSourceHandle, AUDIO_MAX_SOURCE > _SourceHandleAlloc;
};

XE::AudioService::AudioService()
	:_p( XE::New< Private >() )
{

}

XE::AudioService::~AudioService()
{
	XE::Delete( _p );
}

void XE::AudioService::Prepare()
{

}

void XE::AudioService::Startup()
{
	auto submit = _p->_SubmitFrame;

	auto index = _p->_Frames[submit].CommandSize++;
	_p->_Frames[submit].Command[index].Type = XE::AudioCommandType::GAIN;
	_p->_Frames[submit].Command[index].Code = XE::BitCast< XE::uint64 >( _p->_Gain );

	index = _p->_Frames[submit].CommandSize++;
	_p->_Frames[submit].Command[index].Type = XE::AudioCommandType::SPEED_OF_SOUND;
	_p->_Frames[submit].Command[index].Code = XE::BitCast< XE::uint64 >( _p->_SpeedOfSound );

	index = _p->_Frames[submit].CommandSize++;
	_p->_Frames[submit].Command[index].Type = XE::AudioCommandType::DOPPLER_FACTOR;
	_p->_Frames[submit].Command[index].Code = XE::BitCast< XE::uint64 >( _p->_DopplerFactor );

	_p->_Device = alcOpenDevice( nullptr );
	AL_Error();

	_p->_Context = alcCreateContext( _p->_Device, nullptr );
	AL_Error();

	alcMakeContextCurrent( _p->_Context );
	AL_Error();

	std::array<ALuint, XE::AUDIO_MAX_BUFFER> buffers;
	alGenBuffers( XE::AUDIO_MAX_BUFFER, buffers.data() );
	AL_Error();
	for ( size_t i = 0; i < buffers.size(); i++ )
	{
		_p->_Buffers[i].Buffer = buffers[i];
	}

	std::array<ALuint, XE::AUDIO_MAX_SOURCE> sources;
	alGenSources( XE::AUDIO_MAX_SOURCE, sources.data() );
	AL_Error();
	for ( size_t i = 0; i < sources.size(); i++ )
	{
		_p->_Sources[i].Source = sources[i];
	}
}

void XE::AudioService::Update()
{
	XE::uint64 frame = _p->_SubmitFrame;

	{
		XE::SharedPtr< std::unique_lock< std::mutex > >lock = XE::MakeShared< std::unique_lock< std::mutex > >( _p->_Frames[frame].Mutex );

		GetFramework()->GetServiceT< XE::ThreadService >()->PostTask
		( XE::ThreadType::AUDIO, [this, frame, lock]()
		  {
			  OnUpdate( &_p->_Frames[frame] );
		  } );
	}

	{
		std::unique_lock< std::mutex > lock( _p->_Frames[( frame + 1 ) % AUDIO_MAX_FRAME].Mutex );

		_p->_SubmitFrame = ( frame + 1 ) % AUDIO_MAX_FRAME;

		_p->_Frames[_p->_SubmitFrame].Reset();
	}
}

void XE::AudioService::Clearup()
{
	std::array<ALuint, XE::AUDIO_MAX_SOURCE> sources;
	for ( size_t i = 0; i < sources.size(); i++ )
	{
		sources[i] = _p->_Sources[i].Source;
		_p->_Sources[i].Source = 0;
	}
	alSourceStopv( XE::AUDIO_MAX_SOURCE, sources.data() );
	AL_Error();

	alDeleteSources( XE::AUDIO_MAX_SOURCE, sources.data() );
	AL_Error();

	std::array<ALuint, XE::AUDIO_MAX_BUFFER> buffers;
	for ( size_t i = 0; i < buffers.size(); i++ )
	{
		buffers[i] = _p->_Buffers[i].Buffer;
		_p->_Buffers[i].Buffer = 0;
	}
	alDeleteBuffers( XE::AUDIO_MAX_BUFFER, buffers.data() );
	AL_Error();

	alcMakeContextCurrent( nullptr );
	AL_Error();

	alcDestroyContext( _p->_Context );
	AL_Error();
	_p->_Context = nullptr;

	alcCloseDevice( _p->_Device );
	AL_Error();
	_p->_Device = nullptr;

	if ( _p->_CaptureDevice != nullptr )
	{
		alcCaptureStop( _p->_CaptureDevice );

		alcCaptureCloseDevice( _p->_CaptureDevice );

		_p->_CaptureDevice = nullptr;
	}

	for( auto & it : _p->_Frames )
	{
		std::unique_lock< std::mutex > lock( it.Mutex );

		it.Reset();
	}

	_p->_SubmitFrame = 0;
}

void XE::AudioService::Play()
{
	_p->_Pause = false;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::PLAY;
}

void XE::AudioService::Pause()
{
	_p->_Pause = true;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::PAUSE;
}

bool XE::AudioService::IsPause() const
{
	return _p->_Pause;
}

XE::float32 XE::AudioService::GetGain() const
{
	return _p->_Gain;
}

void XE::AudioService::SetGain( XE::float32 val )
{
	_p->_Gain = val;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::GAIN;
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( _p->_Gain );
}

XE::float32 XE::AudioService::GetSpeedOfSound() const
{
	return _p->_SpeedOfSound;
}

void XE::AudioService::SetSpeedOfSound( XE::float32 val )
{
	_p->_SpeedOfSound = val;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SPEED_OF_SOUND;
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( _p->_SpeedOfSound );
}

XE::float32 XE::AudioService::GetDopplerFactor() const
{
	return _p->_DopplerFactor;
}

void XE::AudioService::SetDopplerFactor( XE::float32 val )
{
	_p->_DopplerFactor = val;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::DOPPLER_FACTOR;
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( _p->_DopplerFactor );
}

XE::AudioBufferHandle XE::AudioService::Create( const XE::AudioBufferDesc & desc )
{
	auto it = std::find_if( _p->_Buffers.begin(), _p->_Buffers.end(), [&]( const AHIBuffer & val ) { return val.Count() != 0 && desc.Name == val.Desc.Name; } );
	auto handle = it != _p->_Buffers.end() ? it->Handle : XE::AudioBufferHandle();

	if( !handle )
	{
		handle = _p->_BufferHandleAlloc.Alloc();

		_p->_Buffers[handle.GetValue()].Handle = handle;
		_p->_Buffers[handle.GetValue()].Desc = desc;

		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CREATE_BUFFER;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	}

	_p->_Buffers[handle.GetValue()].Inc();

	return handle;
}

XE::AudioSourceHandle XE::AudioService::Create( const XE::AudioSourceDesc & desc )
{
	auto it = std::find_if( _p->_Sources.begin(), _p->_Sources.end(), [&]( const AHISource & val ) { return val.Count() != 0 && desc.Name == val.Desc.Name; } );
	auto handle = it != _p->_Sources.end() ? it->Handle : XE::AudioSourceHandle();

	if( !handle )
	{
		handle = _p->_SourceHandleAlloc.Alloc();

		_p->_Sources[handle.GetValue()].Handle = handle;
		_p->_Sources[handle.GetValue()].Desc = desc;

		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CREATE_SOURCE;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	}

	_p->_Sources[handle.GetValue()].Inc();

	return handle;
}

const XE::AudioBufferDesc & XE::AudioService::GetDesc( XE::AudioBufferHandle handle ) const
{
	return _p->_Buffers[handle.GetValue()].Desc;
}

const XE::AudioSourceDesc & XE::AudioService::GetDesc( XE::AudioSourceHandle handle ) const
{
	return _p->_Sources[handle.GetValue()].Desc;
}

void XE::AudioService::ListenerTransform( const XE::Transform & transform )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::LISTENER_TRANSFORM;

	_p->_Frames[_p->_SubmitFrame].Buffers( [&]( XE::Buffer & cmd )
										   {
											   _p->_Frames[frame].Command[index].Code = ( XE::uint64 )cmd.WirtePos();

											   XE::Vec3f pos = transform.GetWorldPosition();
											   XE::Vec3f ori = transform.GetForward();

											   cmd.Wirte( pos );
											   cmd.Wirte( ori );
										   } );
}

void XE::AudioService::BufferRefill( AudioBufferHandle handle, XE::MemoryView data )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::BUFFER_FILL;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();

	_p->_Frames[_p->_SubmitFrame].Buffers( [&]( XE::Buffer & cmd )
										   {
											   _p->_Frames[frame].Command[index].Code = cmd.WirtePos();
											   _p->_Frames[frame].Command[index].Size = data.size();

											   _p->_Frames[_p->_SubmitFrame].Buffers( [&data]( XE::Buffer & buf )
																					  {
																						  buf.Wirte( data.data(), data.size() );
																					  } );
										   } );
}

void XE::AudioService::SourceQueueBuffer( AudioSourceHandle handle, AudioBufferHandle buffer )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_QUEUE_BUFFER;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = buffer.GetValue();
}

void XE::AudioService::SourceUnqueBuffer( AudioSourceHandle handle, UnqueBufferCallbackType callback, void * userdata )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_UNQUE_BUFFER;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = ( XE::uint64 )userdata;
	_p->_Frames[frame].Command[index].Size = ( XE::uint64 )callback;
}

void XE::AudioService::SourcePlay( AudioSourceHandle handle )
{
	_p->_Sources[handle.GetValue()].Desc.Pause = false;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_PLAY;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::AudioService::SourcePause( AudioSourceHandle handle )
{
	_p->_Sources[handle.GetValue()].Desc.Pause = true;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_PAUSE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::AudioService::SourceStop( AudioSourceHandle handle )
{
	_p->_Sources[handle.GetValue()].Desc.Pause = true;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_STOP;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::AudioService::SourceLoop( AudioSourceHandle handle, bool loop )
{
	_p->_Sources[handle.GetValue()].Desc.Looping = loop;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_LOOP;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::AudioService::SourceGain( AudioSourceHandle handle, XE::float32 gain )
{
	_p->_Sources[handle.GetValue()].Desc.Gain = gain;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_GAIN;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::AudioService::SourcePitch( AudioSourceHandle handle, XE::float32 pitch )
{
	_p->_Sources[handle.GetValue()].Desc.Pitch = pitch;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_PITCH;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( pitch );
}

void XE::AudioService::SourceTransform( AudioSourceHandle handle, const XE::Transform & transform )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_TRANSFORM;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[_p->_SubmitFrame].Buffers( [&]( XE::Buffer & cmd )
	{
		_p->_Frames[frame].Command[index].Code = cmd.WirtePos();

		XE::Vec3f pos = transform.GetWorldPosition();
		XE::Vec3f ori = transform.GetForward();

		cmd.Wirte( pos );
		cmd.Wirte( ori );
	} );
}

void XE::AudioService::SourceGainRange( AudioSourceHandle handle, XE::float32 min, XE::float32 max )
{
	_p->_Sources[handle.GetValue()].Desc.MinGain = min;
	_p->_Sources[handle.GetValue()].Desc.MaxGain = max;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_GAIN_RANGE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( min );
	_p->_Frames[frame].Command[index].Size = XE::BitCast< XE::uint64 >( max );
}

void XE::AudioService::SourceDistanceRange( AudioSourceHandle handle, XE::float32 min, XE::float32 max )
{
	_p->_Sources[handle.GetValue()].Desc.MinDistance = min;
	_p->_Sources[handle.GetValue()].Desc.MaxDistance = max;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_DISTANCE_RANGE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( min );
	_p->_Frames[frame].Command[index].Size = XE::BitCast< XE::uint64 >( max );
}

void XE::AudioService::SourceConeAngleRange( AudioSourceHandle handle, XE::float32 inner, XE::float32 outer )
{
	_p->_Sources[handle.GetValue()].Desc.InnerConeAngle = inner;
	_p->_Sources[handle.GetValue()].Desc.OuterConeAngle = outer;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_CONEANGLE_RANGE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( inner );
	_p->_Frames[frame].Command[index].Size = XE::BitCast< XE::uint64 >( outer );
}

void XE::AudioService::Destroy( AudioBufferHandle handle )
{
	if( _p->_Buffers[handle.GetValue()].Dec() == 0 )
	{
		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::DESTROY_BUFFER;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_BufferHandleAlloc.Free( handle );
		_p->_Buffers[handle.GetValue()].Reset();
	}
}

void XE::AudioService::Destroy( AudioSourceHandle handle )
{
	if( _p->_Sources[handle.GetValue()].Dec() == 0 )
	{
		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::DESTROY_SOURCE;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_SourceHandleAlloc.Free( handle );
		_p->_Sources[handle.GetValue()].Reset();
	}
}

void XE::AudioService::CaptureStart( XE::uint32 Bits, XE::uint32 Channels, XE::uint32 Frequency )
{
	_p->_Capture = true;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CAPTURE_START;
	_p->_Frames[frame].Command[index].Handle = Bits;
	_p->_Frames[frame].Command[index].Code = Channels;
	_p->_Frames[frame].Command[index].Size = Frequency;
}

void XE::AudioService::CaptureSample( CaptureBufferCallbackType callback, void * userdata )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CAPTURE_SAMPLES;
	_p->_Frames[frame].Command[index].Code = (XE::uint64)userdata;
	_p->_Frames[frame].Command[index].Size = (XE::uint64)callback;
}

void XE::AudioService::CaptureStop()
{
	_p->_Capture = false;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CAPTURE_STOP;
}

bool XE::AudioService::IsCapture() const
{
	return _p->_Capture;
}

void XE::AudioService::OnUpdate( XE::AudioFrame * frame )
{
	XE::uint64 size = frame->CommandSize;

	std::sort( frame->Command.begin(), frame->Command.begin() + size, []( const XE::AudioCommand & left, const XE::AudioCommand & right )
	{
		return left.Type < right.Type;
	} );

	XE::MemoryView view = frame->Buffers.unsafe().View();

	for ( size_t i = 0; i < size; i++ )
	{
		auto & cmd = frame->Command[i];

		switch ( cmd.Type )
		{
		case XE::AudioCommandType::PLAY:
		{
			for ( size_t i = 0; i < XE::AUDIO_MAX_SOURCE; i++ )
			{
				if ( _p->_Sources[i].Desc.Pause == false )
				{
					alSourcePlay( _p->_Sources[i].Source );
				}
			}
		}
		break;
		case XE::AudioCommandType::PAUSE:
		{
			for ( size_t i = 0; i < XE::AUDIO_MAX_SOURCE; i++ )
			{
				if ( _p->_Sources[i].Desc.Pause == false )
				{
					alSourcePause( _p->_Sources[i].Source );
				}
			}
		}
		break;
		case XE::AudioCommandType::GAIN:
		{
			XE::float32 gain = XE::BitCast< XE::float32 >( cmd.Code );

			alListenerf( AL_GAIN, gain );
		}
		break;
		case XE::AudioCommandType::SPEED_OF_SOUND:
		{
			XE::float32 speed = XE::BitCast< XE::float32 >( cmd.Code );

			alSpeedOfSound( speed );
		}
		break;
		case XE::AudioCommandType::DOPPLER_FACTOR:
		{
			XE::float32 factor = XE::BitCast< XE::float32 >( cmd.Code );

			alDopplerFactor( factor );
		}
		break;
		case XE::AudioCommandType::CREATE_BUFFER:
		{
			XE::AudioBufferHandle handle = XE::HandleCast< XE::AudioBuffer >( cmd.Handle );

			const XE::AudioBufferDesc & desc = GetDesc( handle );

			alBufferi( _p->_Buffers[handle.GetValue()].Buffer, AL_BITS, desc.Bits );
			alBufferi( _p->_Buffers[handle.GetValue()].Buffer, AL_CHANNELS, desc.Channels );
			alBufferi( _p->_Buffers[handle.GetValue()].Buffer, AL_FREQUENCY, desc.Frequency );
		}
		break;
		case XE::AudioCommandType::CREATE_SOURCE:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			const XE::AudioSourceDesc & desc = GetDesc( handle );

			alSourcei( _p->_Sources[handle.GetValue()].Source, AL_SOURCE_RELATIVE, 0 );
			alSourcei( _p->_Sources[handle.GetValue()].Source, AL_SOURCE_TYPE, AL_STATIC );

			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_GAIN, desc.Gain );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_PITCH, desc.Pitch );
			alSourcei( _p->_Sources[handle.GetValue()].Source, AL_LOOPING, desc.Looping );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_MIN_GAIN, desc.MinGain );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_MAX_GAIN, desc.MaxGain );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_REFERENCE_DISTANCE, desc.MinDistance );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_MAX_DISTANCE, desc.MaxDistance );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_CONE_INNER_ANGLE, desc.InnerConeAngle );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_CONE_OUTER_ANGLE, desc.OuterConeAngle );

			if ( desc.Pause )
			{
				alSourcePause( _p->_Sources[handle.GetValue()].Source );
			}
			else
			{
				alSourcePlay( _p->_Sources[handle.GetValue()].Source );
			}
		}
		break;
		case XE::AudioCommandType::LISTENER_TRANSFORM:
		{
			alListenerfv( AL_POSITION, (const ALfloat *)( view.data() + cmd.Code ) );
			alListenerfv( AL_ORIENTATION, (const ALfloat *)( view.data() + cmd.Code + sizeof( XE::Vec3f ) ) );

			alListenerfv( AL_VELOCITY,  XE::Vec3f::Zero.d );
		}
		break;
		case XE::AudioCommandType::BUFFER_FILL:
		{
			XE::AudioBufferHandle handle = XE::HandleCast< XE::AudioBuffer >( cmd.Handle );

			const auto & desc = GetDesc( handle );

			alBufferData( _p->_Buffers[handle.GetValue()].Buffer, AL_Format( desc.Bits, desc.Channels ), view.data() + cmd.Code, cmd.Size, desc.Frequency );
			AL_Error();
		}
		break;
		case XE::AudioCommandType::SOURCE_QUEUE_BUFFER:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );
			XE::AudioBufferHandle buffer = XE::HandleCast< XE::AudioBuffer >( cmd.Code );

			alSourceQueueBuffers( _p->_Sources[handle.GetValue()].Source, 1, &_p->_Buffers[buffer.GetValue()].Buffer );
		}
		break;
		case XE::AudioCommandType::SOURCE_UNQUE_BUFFER:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );
			std::intptr_t userdata = cmd.Code;
			std::intptr_t callback = cmd.Size;

			if ( _p->_Sources[handle.GetValue()].UnqueBuffers.empty() )
			{
				ALint num;
				std::array<ALuint, 10> buffers;
				alGetSourcei( _p->_Sources[handle.GetValue()].Source, AL_BUFFERS_PROCESSED, &num );
				alSourceUnqueueBuffers( _p->_Sources[handle.GetValue()].Source, std::min<XE::uint64>( num, 10 ), buffers.data() );

				for ( XE::uint64 i = 0; i < std::min<XE::uint64>( num, 10 ); i++ )
				{
					auto it = std::find_if( _p->_Buffers.begin(), _p->_Buffers.end(), [&]( const auto & val ) { return val.Buffer == buffers[i]; } );
					if ( it != _p->_Buffers.end() )
					{
						_p->_Sources[handle.GetValue()].UnqueBuffers.push_back( XE::HandleCast< XE::AudioBuffer >( it - _p->_Buffers.begin() ) );
					}
				}
			}

			XE::AudioBufferHandle buffer;

			if ( !_p->_Sources[handle.GetValue()].UnqueBuffers.empty() )
			{
				buffer = _p->_Sources[handle.GetValue()].UnqueBuffers.front();
				_p->_Sources[handle.GetValue()].UnqueBuffers.pop_front();
			}

			( (XE::UnqueBufferCallbackType)callback )( (void *)userdata, buffer );
		}
		break;
		case XE::AudioCommandType::SOURCE_PLAY:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourcePlay( _p->_Sources[handle.GetValue()].Source );
		}
		break;
		case XE::AudioCommandType::SOURCE_PAUSE:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourcePause( _p->_Sources[handle.GetValue()].Source );
		}
		break;
		case XE::AudioCommandType::SOURCE_LOOP:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourcei( _p->_Sources[handle.GetValue()].Source, AL_LOOPING, _p->_Sources[handle.GetValue()].Desc.Looping );
		}
		break;
		case XE::AudioCommandType::SOURCE_GAIN:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_GAIN, _p->_Sources[handle.GetValue()].Desc.Gain );
		}
		break;
		case XE::AudioCommandType::SOURCE_PITCH:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );
			XE::float32 pitch = XE::BitCast< XE::float32 >( cmd.Code );

			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_PITCH, pitch );
		}
		break;
		case XE::AudioCommandType::SOURCE_TRANSFORM:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourcefv( _p->_Sources[handle.GetValue()].Source, AL_POSITION, (const ALfloat *)( view.data() + cmd.Code ) );
			alSourcefv( _p->_Sources[handle.GetValue()].Source, AL_DIRECTION, (const ALfloat *)( view.data() + cmd.Code + sizeof( XE::Vec3f ) ) );
			alSourcefv( _p->_Sources[handle.GetValue()].Source, AL_VELOCITY, XE::Vec3f::Zero.d );
		}
		break;
		case XE::AudioCommandType::SOURCE_GAIN_RANGE:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );
			XE::float32 min = XE::BitCast< XE::float32 >( cmd.Code );
			XE::float32 max = XE::BitCast< XE::float32 >( cmd.Size );

			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_MIN_GAIN, min );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_MAX_GAIN, max );
		}
		break;
		case XE::AudioCommandType::SOURCE_DISTANCE_RANGE:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );
			XE::float32 min = XE::BitCast< XE::float32 >( cmd.Code );
			XE::float32 max = XE::BitCast< XE::float32 >( cmd.Size );

			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_REFERENCE_DISTANCE, min );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_MAX_DISTANCE, max );
		}
		break;
		case XE::AudioCommandType::SOURCE_CONEANGLE_RANGE:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );
			XE::float32 inner = XE::BitCast< XE::float32 >( cmd.Code );
			XE::float32 outer = XE::BitCast< XE::float32 >( cmd.Size );

			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_CONE_INNER_ANGLE, inner );
			alSourcef( _p->_Sources[handle.GetValue()].Source, AL_CONE_OUTER_ANGLE, outer );
		}
		break;
		case XE::AudioCommandType::CAPTURE_START:
		{
			XE::uint32 Bits = cmd.Handle;
			XE::uint32 Channels = cmd.Code;
			XE::uint32 Frequency = cmd.Size;

			if ( _p->_CaptureDevice == nullptr )
			{
				_p->_CaptureDevice = alcCaptureOpenDevice( nullptr, Frequency, AL_Format( Bits, Channels ), (ALCsizei)( (XE::uint64)Frequency * (XE::uint64)Bits * (XE::uint64)Channels ) );
				AL_Error();

				alcCaptureStart( _p->_CaptureDevice );
				AL_Error();

				alcGetIntegerv( _p->_CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &_p->_CaptureSample );
				AL_Error();

				_p->_CaptureBuffer.resize( (XE::uint64)Frequency * (XE::uint64)Bits * (XE::uint64)Channels );
			}
			else
			{
				XE_ERROR( "The capture is turned on" );
			}
		}
		break;
		case XE::AudioCommandType::CAPTURE_SAMPLES:
		{
			XE::uint64 userdata = cmd.Code;
			XE::uint64 callback = cmd.Size;

			if ( _p->_CaptureDevice != nullptr )
			{
				alcCaptureSamples( _p->_CaptureDevice, _p->_CaptureBuffer.data(), _p->_CaptureSample );
				AL_Error();

				( (XE::CaptureBufferCallbackType)callback )( (void *)userdata, { _p->_CaptureBuffer.data(), _p->_CaptureBuffer.size() } );
			}
			else
			{
				( (XE::CaptureBufferCallbackType)callback )( (void *)userdata, {} );
			}
		}
		break;
		case XE::AudioCommandType::SOURCE_STOP:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourceStop( _p->_Sources[handle.GetValue()].Source );

			_p->_Sources[handle.GetValue()].UnqueBuffers.clear();
		}
		break;
		case XE::AudioCommandType::CAPTURE_STOP:
		{
			if ( _p->_CaptureDevice != nullptr )
			{
				alcCaptureStop( _p->_CaptureDevice );

				alcCaptureCloseDevice( _p->_CaptureDevice );

				_p->_CaptureDevice = nullptr;
				_p->_CaptureBuffer.clear();
				_p->_CaptureBuffer.shrink_to_fit();
				_p->_CaptureSample = 0;
			}
		}
		break;
		case XE::AudioCommandType::DESTROY_BUFFER:
		{
			XE::AudioBufferHandle handle = XE::HandleCast< XE::AudioBuffer >( cmd.Handle );

			alBufferData( _p->_Buffers[handle.GetValue()].Buffer, AL_FORMAT_MONO8, nullptr, 0, 0 );
			AL_Error();
		}
		break;
		case XE::AudioCommandType::DESTROY_SOURCE:
		{
			XE::AudioSourceHandle handle = XE::HandleCast< XE::AudioSource >( cmd.Handle );

			alSourceStop( _p->_Sources[handle.GetValue()].Source );
		}
		break;
		}

	}
}
