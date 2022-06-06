#include "SceneService.h"

BEG_META( XE::SceneService )
END_META()

struct XE::SceneService::Private
{

};

XE::SceneService::SceneService()
	:_p( XE::New< Private >() )
{

}

XE::SceneService::~SceneService()
{
	XE::Delete( _p );
}

void XE::SceneService::Prepare()
{

}

void XE::SceneService::Startup()
{

}

void XE::SceneService::Update()
{

}

void XE::SceneService::Clearup()
{

}

void XE::SceneService::LoadWorld( const XE::String & val )
{

}

void XE::SceneService::UnloadWorld( const XE::String & val )
{

}

void XE::SceneService::AsyncLoadWorld( const XE::String & val )
{

}

bool XE::SceneService::GetWorldEnable( const XE::String & val ) const
{
	return false;
}

void XE::SceneService::SetWorldEnable( const XE::String & name, bool enable )
{

}

