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

XS::AssetDatabasePtr XS::CoreFramework::GetAssetDatabase() const
{
	return _p->_AssetDatabase;
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

void XS::CoreFramework::Save()
{
	Super::Save();


}

void XS::CoreFramework::Reload()
{
	Super::Reload();


}

int XS::CoreFramework::Exec( XE::WindowPtr window )
{
	return Super::Exec( window );
}

void XS::CoreFramework::Exec( XE::WindowPtr window, const XE::String & project_path )
{
	_p->_ProjectPath = project_path.std_str();

	std::thread( [this, window]() { Exec( window ); } ).detach();
}

void XS::CoreFramework::Prepare()
{
	Super::Prepare();

	QUrl url( GetString( ASSET_DATABASE ).c_str() );

	_p->_AssetDatabase = XE::MakeShared< XS::AssetDatabase >();

	if ( !_p->_AssetDatabase->Open( url ) )
	{
		XE_ERROR( "asset database open failure: %{1}", GetString( ASSET_DATABASE ) );
		_p->_AssetDatabase = nullptr;
	}
}

void XS::CoreFramework::Startup()
{
	Super::Startup();
}

void XS::CoreFramework::Update()
{
	Super::Update();
}

void XS::CoreFramework::Clearup()
{
	Super::Clearup();
}

void XS::CoreFramework::LoadModules()
{
	Super::LoadModules();
}

void XS::CoreFramework::LoadServices()
{
	Super::LoadServices();
}

XE::String XS::CoreFramework::GetValue( const XE::String & key )
{
	if ( key.find( "System" ) == 0 )
	{
		return Super::GetValue( key );
	}
	else
	{

	}

	return "";
}

void XS::CoreFramework::SetValue( const XE::String & key, const XE::String & val )
{
	if ( key.find( "System" ) == 0 )
	{
		Super::SetValue( key, val );
	}
	else
	{

	}
}
