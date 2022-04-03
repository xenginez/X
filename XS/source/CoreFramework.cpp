#include "CoreFramework.h"

IMPLEMENT_META( XS::CoreFramework );

struct XS::CoreFramework::Private
{
	static XS::CoreFramework * _Framework;
};
XS::CoreFramework * XS::CoreFramework::Private::_Framework = nullptr;

XS::CoreFramework::CoreFramework()
	:_p( XE::New<Private>() )
{
	Private::_Framework = this;
}

XS::CoreFramework::~CoreFramework()
{
	WaitExit();

	XE::Delete( _p );

	Private::_Framework = nullptr;
}

XS::CoreFramework * XS::CoreFramework::GetCurrentFramework()
{
	return Private::_Framework;
}

bool XS::CoreFramework::RegisterService( const XE::MetaClassPtr & val )
{
	return Super::RegisterService( val );
}

void XS::CoreFramework::UnregisterService( const XE::MetaClassPtr & val )
{
	Super::UnregisterService( val );
}

void XS::CoreFramework::Exit()
{
	Super::Exit();
}

void XS::CoreFramework::WaitExit()
{
	Super::WaitExit();
}

bool XS::CoreFramework::IsExit() const
{
	return Super::IsExit();
}

XE::Language XS::CoreFramework::GetSystemLanguage() const
{
	return Super::GetSystemLanguage();
}

std::filesystem::path XS::CoreFramework::GetModulePath() const
{
	return "";
}

std::filesystem::path XS::CoreFramework::GetAssetsPath() const
{
	return "";
}

std::filesystem::path XS::CoreFramework::GetCachesPath() const
{
	return "";
}

std::filesystem::path XS::CoreFramework::GetProjectPath() const
{
	return "";
}

std::filesystem::path XS::CoreFramework::GetUserDataPath() const
{
	return "";
}

std::filesystem::path XS::CoreFramework::GetApplicationPath() const
{
	return "";
}

XE::WindowPtr XS::CoreFramework::GetMainWindow() const
{
	return nullptr;
}

void XS::CoreFramework::Save()
{

}

void XS::CoreFramework::Reload()
{

}

int XS::CoreFramework::Exec( XE::WindowPtr window )
{
	return 0;
}

void XS::CoreFramework::Exec( XE::WindowPtr window, const XE::String & project_path )
{
	std::thread th( [this, window]() { Exec( window ); } );
	th.detach();
}

void XS::CoreFramework::Prepare()
{

}

void XS::CoreFramework::Startup()
{

}

void XS::CoreFramework::Update()
{

}

void XS::CoreFramework::Clearup()
{

}

XE::String XS::CoreFramework::GetValue( const XE::String & key )
{
	return "";
}

void XS::CoreFramework::SetValue( const XE::String & key, const XE::String & val )
{

}
