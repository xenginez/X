#include "AudioBuffer.h"

BEG_META( XE::AudioBuffer )
type->Property( "UUID", &XE::AudioBuffer::_UUID );
type->Property( "Bits", &XE::AudioBuffer::_Bits );
type->Property( "Channels", &XE::AudioBuffer::_Channels );
type->Property( "Frequency", &XE::AudioBuffer::_Frequency );
END_META()

XE::AudioBuffer::AudioBuffer()
{

}

XE::AudioBuffer::~AudioBuffer()
{

}

XE::UUID XE::AudioBuffer::GetUUID() const
{
	return _UUID;
}

void XE::AudioBuffer::SetUUID( XE::UUID val )
{
	_UUID = val;
}

XE::uint32 XE::AudioBuffer::GetBits() const
{
	return _Bits;
}

void XE::AudioBuffer::SetBits( XE::uint32 val )
{
	_Bits = val;
}

XE::uint32 XE::AudioBuffer::GetChannels() const
{
	return _Channels;
}

void XE::AudioBuffer::SetChannels( XE::uint32 val )
{
	_Channels = val;
}

XE::uint32 XE::AudioBuffer::GetFrequency() const
{
	return _Frequency;
}

void XE::AudioBuffer::SetFrequency( XE::uint32 val )
{
	_Frequency = val;
}
