#include "AudioListener.h"

#include "AudioService.h"

IMPLEMENT_META( XE::AudioListener );

XE::AudioListener::AudioListener()
{

}

XE::AudioListener::~AudioListener()
{

}

void XE::AudioListener::OnStartup()
{

}

void XE::AudioListener::OnUpdate( XE::float32 dt )
{
	GetService< XE::AudioService >()->ListenerTransform( GetTransform() );
}

void XE::AudioListener::OnClearup()
{

}

