#include "AudioService.h"

#include "Utils/RefCount.h"

#include "CoreFramework.h"
#include "ThreadService.h"

BEG_META( XE::AudioService )
type->Property( "SpeedOfSound", &XE::AudioService::GetSpeedOfSound, &XE::AudioService::SetSpeedOfSound );
type->Property( "DopplerFactor", &XE::AudioService::GetDopplerFactor, &XE::AudioService::SetDopplerFactor )->Attribute( XE::RangeAttribute( 0.0f, 1.0f ) );
END_META()

BEG_XE_NAMESPACE

struct AHIBuffer : public XE::RefCount
{
	XE::AudioBufferDesc Desc;
	XE::AudioBufferHandle Handle;
};

struct AHISource : public XE::RefCount
{
	XE::AudioSourceDesc Desc;
	XE::AudioSourceHandle Handle;
};

END_XE_NAMESPACE

struct XE::AudioService::Private
{
	std::mutex _FrameMutex;
	XE::uint64 _SubmitFrame = 0;
	std::array< XE::AudioFrame, XE::AUDIO_MAX_FRAME > _Frames;

	bool _Capture = false;

	bool _Pause = false;
	XE::float32 _Gain = 1.0f;
	XE::float32 _SpeedOfSound = 343.3f;
	XE::float32 _DopplerFactor = 1.0f;

	std::array<XE::AHIBuffer, XE::AUDIO_MAX_BUFFER > _Buffers;
	std::array<XE::AHISource, XE::AUDIO_MAX_SOURCE > _Sources;
	XE::ConcurrentHandleAllocator< XE::AudioBufferHandle, AUDIO_MAX_BUFFER > _BufferHandleAlloc;
	XE::ConcurrentHandleAllocator< XE::AudioSourceHandle, AUDIO_MAX_SOURCE > _SourceHandleAlloc;
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

void XE::AudioService::ListenerTransform( const XE::Vec3f & position, const XE::Vec3f & velocity, const XE::Vec3f & orientation, const XE::Vec3f & up /*= XE::Vec3f::Up */ )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::LISTENER_TRANSFORM;

	_p->_Frames[_p->_SubmitFrame].Buffers( [&]( XE::Buffer & cmd )
										   {
											   _p->_Frames[frame].Command[index].Code = ( XE::uint64 )cmd.WirtePos();

											   cmd.Wirte( position );
											   cmd.Wirte( velocity );
											   cmd.Wirte( orientation );
											   cmd.Wirte( up );
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

void XE::AudioService::SourceUnqueBuffer( AudioSourceHandle handle, void * userdata, UnqueBufferCallbackType callback )
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

void XE::AudioService::SourceRelative( AudioSourceHandle handle, bool relative )
{
	_p->_Sources[handle.GetValue()].Desc.Relative = relative;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_RELATIVE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = relative;
}

void XE::AudioService::SourceConeAngle( AudioSourceHandle handle, XE::float32 inner, XE::float32 outer )
{
	_p->_Sources[handle.GetValue()].Desc.InnerConeAngle = inner;
	_p->_Sources[handle.GetValue()].Desc.OuterConeAngle = outer;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_CONE_ANGLE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( inner );
	_p->_Frames[frame].Command[index].Size = XE::BitCast< XE::uint64 >( outer );
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

void XE::AudioService::SourceGain( AudioSourceHandle handle, XE::float32 min, XE::float32 max )
{
	_p->_Sources[handle.GetValue()].Desc.MinGain = min;
	_p->_Sources[handle.GetValue()].Desc.MaxGain = max;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_GAIN;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( min );
	_p->_Frames[frame].Command[index].Size = XE::BitCast< XE::uint64 >( max );
}

void XE::AudioService::SourceDistance( AudioSourceHandle handle, XE::float32 min, XE::float32 max )
{
	_p->_Sources[handle.GetValue()].Desc.MinDistance = min;
	_p->_Sources[handle.GetValue()].Desc.MaxDistance = max;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_DISTANCE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( min );
	_p->_Frames[frame].Command[index].Size = XE::BitCast< XE::uint64 >( max );
}

void XE::AudioService::SourceTransform( AudioSourceHandle handle, const XE::Vec3f & position, const XE::Vec3f & verlocity, const XE::Vec3f & orientation, const XE::Vec3f & up /*= XE::Vec3f::Up */ )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::SOURCE_GAIN;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[_p->_SubmitFrame].Buffers( [&]( XE::Buffer & cmd )
										   {
											   _p->_Frames[frame].Command[index].Code = cmd.WirtePos();

											   cmd.Wirte( position );
											   cmd.Wirte( verlocity );
											   cmd.Wirte( orientation );
											   cmd.Wirte( up );
										   } );
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

void XE::AudioService::CaptureStart( XE::uint32 Bits, XE::uint32 Channels, XE::uint32 Frequency, ReadBufferCallbackType callback )
{
	_p->_Capture = true;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CAPTURE_START;
	_p->_Frames[frame].Command[index].Handle = Bits;
	_p->_Frames[frame].Command[index].Code = Channels;
	_p->_Frames[frame].Command[index].Size = Frequency;

	index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::AudioCommandType::CAPTURE_SAMPLES;
	_p->_Frames[frame].Command[index].Code = (XE::uint64)callback;
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
	for ( size_t i = 0; i < frame->CommandSize; i++ )
	{
		switch ( frame->Command[i].Type )
		{
		case XE::AudioCommandType::PLAY:
			break;
		case XE::AudioCommandType::GAIN:
			break;
		case XE::AudioCommandType::SPEED_OF_SOUND:
			break;
		case XE::AudioCommandType::DOPPLER_FACTOR:
			break;
		case XE::AudioCommandType::CREATE_BUFFER:
			break;
		case XE::AudioCommandType::CREATE_SOURCE:
			break;
		case XE::AudioCommandType::LISTENER_TRANSFORM:
			break;
		case XE::AudioCommandType::BUFFER_FILL:
			break;
		case XE::AudioCommandType::SOURCE_QUEUE_BUFFER:
			break;
		case XE::AudioCommandType::SOURCE_UNQUE_BUFFER:
			break;
		case XE::AudioCommandType::SOURCE_PLAY:
			break;
		case XE::AudioCommandType::SOURCE_RELATIVE:
			break;
		case XE::AudioCommandType::SOURCE_CONE_ANGLE:
			break;
		case XE::AudioCommandType::SOURCE_PITCH:
			break;
		case XE::AudioCommandType::SOURCE_TRANSFORM:
			break;
		case XE::AudioCommandType::SOURCE_GAIN:
			break;
		case XE::AudioCommandType::SOURCE_DISTANCE:
			break;
		case XE::AudioCommandType::SOURCE_PAUSE:
			break;
		case XE::AudioCommandType::CAPTURE_START:
			break;
		case XE::AudioCommandType::CAPTURE_SAMPLES:
			break;
		case XE::AudioCommandType::SOURCE_STOP:
			break;
		case XE::AudioCommandType::CAPTURE_STOP:
			break;
		case XE::AudioCommandType::PAUSE:
			break;
		case XE::AudioCommandType::DESTROY_BUFFER:
			break;
		case XE::AudioCommandType::DESTROY_SOURCE:
			break;
		default:
			break;
		}
	}
}
