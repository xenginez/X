#include "AudioSource.h"

#include "Core/AssetsService.h"

#include "AudioBuffer.h"
#include "AudioService.h"

BEG_META( XE::AudioSource )
END_META()

XE::AudioSource::AudioSource()
{

}

XE::AudioSource::~AudioSource()
{

}

void XE::AudioSource::OnStartup()
{
	if ( auto audio = GetService< XE::AudioService >() )
	{
		{
			XE::AudioBufferDesc desc;

			desc.Bits = _Audio.GetBits();
			desc.Channels = _Audio.GetChannels();
			desc.Frequency = _Audio.GetFrequency();
			desc.Name = _Audio.GetUUID().ToString();

			_Buffer = audio->Create( desc );
		}

		{
			XE::AudioSourceDesc desc;

			desc.Gain = _Gain;
			desc.Pitch = _Pitch;
			desc.Pause = _Pause;
			desc.Looping = _Looping;
			desc.MinGain = _MinGain;
			desc.MaxGain = _MaxGain;
			desc.MinDistance = _MinDistance;
			desc.MaxDistance = _MaxDistance;
			desc.InnerConeAngle = _InnerConeAngle;
			desc.OuterConeAngle = _OuterConeAngle;

			_Source = audio->Create( desc );
		}

		if ( auto asset = GetService< XE::AssetsService >() )
		{
			audio->BufferRefill( _Buffer, asset->Load( _Audio.GetUUID() ) );
		}
	}

	_State = _Pause ? XE::AudioSourceState::PAUSE : XE::AudioSourceState::PLAY;
}

void XE::AudioSource::OnUpdate( XE::float32 dt )
{
	
}

void XE::AudioSource::OnClearup()
{
	if ( auto audio = GetService< XE::AudioService >() )
	{
		audio->Destroy( _Source );
		audio->Destroy( _Buffer );
	}
}

const XE::AudioBuffer & XE::AudioSource::GetSource() const
{
	return _Audio;
}

void XE::AudioSource::SetSource( const XE::AudioBuffer & val )
{
	_Audio = val;
}

bool XE::AudioSource::GetLoop() const
{
	return _Looping;
}

void XE::AudioSource::SetLoop( bool val )
{
	_Looping = val;

	GetService< XE::AudioService >()->SourceLoop( _Source, _Looping );
}

XE::float32 XE::AudioSource::GetGain() const
{
	return _Gain;
}

void XE::AudioSource::SetGain( XE::float32 val )
{
	_Gain = val;

	GetService< XE::AudioService >()->SourceGain( _Source, _Gain );
}

XE::float32 XE::AudioSource::GetPitch() const
{
	return _Pitch;
}

void XE::AudioSource::SetPitch( XE::float32 val )
{
	_Pitch = val;

	GetService< XE::AudioService >()->SourcePitch( _Source, _Pitch );
}

XE::float32 XE::AudioSource::GetMinGain() const
{
	return _MinGain;
}

void XE::AudioSource::SetMinGain( XE::float32 val )
{
	_MinGain = val;

	GetService< XE::AudioService >()->SourceGainRange( _Source, _MinGain, _MaxGain );
}

XE::float32 XE::AudioSource::GetMaxGain() const
{
	return _MaxGain;
}

void XE::AudioSource::SetMaxGain( XE::float32 val )
{
	_MaxGain = val;

	GetService< XE::AudioService >()->SourceGainRange( _Source, _MinGain, _MaxGain );
}

XE::float32 XE::AudioSource::GetMinDistance() const
{
	return _MinDistance;
}

void XE::AudioSource::SetMinDistance( XE::float32 val )
{
	_MinDistance = val;

	GetService< XE::AudioService >()->SourceDistanceRange( _Source, _MinDistance, _MaxDistance );
}

XE::float32 XE::AudioSource::GetMaxDistance() const
{
	return _MaxDistance;
}

void XE::AudioSource::SetMaxDistance( XE::float32 val )
{
	_MaxDistance = val;

	GetService< XE::AudioService >()->SourceDistanceRange( _Source, _MinDistance, _MaxDistance );
}

XE::float32 XE::AudioSource::GetInnerConeAngle() const
{
	return _InnerConeAngle;
}

void XE::AudioSource::SetInnerConeAngle( XE::float32 val )
{
	_InnerConeAngle = val;

	GetService< XE::AudioService >()->SourceConeAngleRange( _Source, _InnerConeAngle, _OuterConeAngle );
}

XE::float32 XE::AudioSource::GetOuterConeAngle() const
{
	return _OuterConeAngle;
}

void XE::AudioSource::SetOuterConeAngle( XE::float32 val )
{
	_OuterConeAngle = val;

	GetService< XE::AudioService >()->SourceConeAngleRange( _Source, _InnerConeAngle, _OuterConeAngle );
}

void XE::AudioSource::Play()
{
	if ( _State != AudioSourceState::PLAY )
	{
		GetService< XE::AudioService >()->SourcePlay( _Source );

		_State = AudioSourceState::PLAY;
	}
}

void XE::AudioSource::Stop()
{
	if ( _State != AudioSourceState::STOP )
	{
		GetService< XE::AudioService >()->SourceStop( _Source );

		_State = AudioSourceState::STOP;
	}
}

void XE::AudioSource::Pause()
{
	if ( _State == AudioSourceState::PLAY )
	{
		GetService< XE::AudioService >()->SourcePause( _Source );

		_State = AudioSourceState::PAUSE;
	}
}

bool XE::AudioSource::IsPlay()
{
	return _State == AudioSourceState::PLAY;
}

bool XE::AudioSource::IsStop()
{
	return _State == AudioSourceState::STOP;
}

bool XE::AudioSource::IsPause()
{
	return _State == AudioSourceState::PAUSE;
}
