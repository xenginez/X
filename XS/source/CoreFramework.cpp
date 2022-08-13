#include "CoreFramework.h"

IMPLEMENT_META( XS::CoreFramework );

struct XS::CoreFramework::Private
{
	std::filesystem::path _ProjectPath;
	static XS::CoreFramework * _Framework;
	XE::SharedPtr< XS::AssetDatabase > _AssetDatabase;
};
XS::CoreFramework * XS::CoreFramework::Private::_Framework = nullptr;

XS::CoreFramework::CoreFramework()
	:_p( XE::New<Private>() )
{
	Private::_Framework = this;
}

XS::CoreFramework::~CoreFramework()
{
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
	return GetApplicationPath() / XE::MODULES_DIRECTORY;
}

std::filesystem::path XS::CoreFramework::GetAssetsPath() const
{
	return _p->_ProjectPath / XE::ASSETS_DIRECTORY;
}

std::filesystem::path XS::CoreFramework::GetCachesPath() const
{
	return _p->_ProjectPath / XE::CACHES_DIRECTORY;
}

std::filesystem::path XS::CoreFramework::GetProjectPath() const
{
	return _p->_ProjectPath;
}

std::filesystem::path XS::CoreFramework::GetUserDataPath() const
{
	return _p->_ProjectPath / XE::USERDATAS_DIRECTORY;
}

std::filesystem::path XS::CoreFramework::GetApplicationPath() const
{
	return Super::GetApplicationPath();
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
	_p->_ProjectPath = project_path.std_str();

	std::thread( [this, window]() { Exec( window ); } ).detach();
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

XS::AssetDatabase * XS::CoreFramework::GetAssetDatabase() const
{
	return _p->_AssetDatabase.get();
}
