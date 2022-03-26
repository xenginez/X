#include "CoreFramework.h"

#include <mutex>
#include <atomic>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include "Utils/Logger.h"
#include "Utils/Library.h"
#include "Utils/Platform.h"

#include "Service.h"

BEG_META( XE::CoreFramework )
END_META()

void nest_json( rapidjson::Value & parent, std::vector< XE::String >::const_iterator beg, std::vector< XE::String >::const_iterator end, const XE::String & str, rapidjson::MemoryPoolAllocator< > & allocator )
{
	if( beg != end )
	{
		auto it = parent.FindMember( beg->c_str() );
		if( it == parent.MemberEnd() )
		{
			parent.AddMember(
				rapidjson::Value().SetString( beg->c_str(), allocator ).Move(),
				rapidjson::Value( rapidjson::kObjectType ).Move(),
				allocator
			);

			it = parent.FindMember( beg->c_str() );
		}

		nest_json( it->value, beg + 1, end, str, allocator );
	}
	else
	{
		parent.AddMember(
			rapidjson::Value().SetString( beg->c_str(), allocator ).Move(),
			rapidjson::Value().SetString( str.c_str(), allocator ).Move(),
			allocator
		);
	}
}

struct XE::CoreFramework::Private
{
	XE::WindowPtr _MainWindow;
	
	std::atomic< bool > _Exit = false;
	std::mutex _ExitMutex;
	XE::Map < XE::String, XE::String > Values;
	XE::Array < XE::ServicePtr > _Services;

	static XE::CoreFrameworkPtr _CurrentFramework;

	XE::Array< XE::uint64 > _StartServices, _UpdateServices, _ClearupServices;
};
XE::CoreFrameworkPtr XE::CoreFramework::Private::_CurrentFramework = nullptr;

XE::CoreFramework::CoreFramework()
	:_p( XE::New< Private >() )
{
	XE_ASSERT( Private::_CurrentFramework == nullptr && "framework is not unique!" );

	Private::_CurrentFramework = this;
}

XE::CoreFramework::~CoreFramework()
{
	XE::Delete( _p );

	Private::_CurrentFramework = nullptr;
}

XE::CoreFrameworkPtr XE::CoreFramework::GetCurrentFramework()
{
	return Private::_CurrentFramework;
}

int XE::CoreFramework::Exec( XE::WindowPtr window )
{
	std::unique_lock< std::mutex > lock( _p->_ExitMutex );

	_p->_MainWindow = window;

	Prepare();

	Startup();

	while (!_p->_Exit)
	{
		Update();
	}

	Clearup();

	return 0;
}

XE::ServicePtr XE::CoreFramework::GetService( const XE::MetaClassPtr & val ) const
{
	for( auto service : _p->_Services )
	{
		if( service->GetMetaClass()->CanConvert( val ) )
		{
			return service;
		}
	}

	return nullptr;
}

bool XE::CoreFramework::RegisterService( const XE::MetaClassPtr & val )
{
	if( GetService( val ) == nullptr )
	{
		if( XE::ServicePtr service = val->ConstructPtr().Value< XE::ServicePtr >() )
		{
			service->SetFramework( this );

			service->Prepare();

			if( !service->Startup() )
			{
				XE_LOG( LoggerLevel::Error, "startup {%0} error!", service->GetMetaClass()->GetFullName() );
				return false;
			}

			_p->_Services.push_back( service );

			return true;
		}
	}

	return false;
}

void XE::CoreFramework::UnregisterService( const XE::MetaClassPtr & val )
{
	auto it = std::find_if( _p->_Services.begin(), _p->_Services.end(), [val]( const XE::ServicePtr & srv )
							{
								return srv->GetMetaClass()->CanConvert( val );
							} );

	if( it != _p->_Services.end() )
	{
		( *it )->Clearup();
		( *it ) = nullptr;
	}
}

void XE::CoreFramework::Exit()
{
	_p->_Exit = true;
}

void XE::CoreFramework::WaitExit()
{
	Exit();

	std::unique_lock< std::mutex > lock( _p->_ExitMutex );
}

bool XE::CoreFramework::IsExit() const
{
	return _p->_Exit;
}

XE::Language XE::CoreFramework::GetSystemLanguage() const
{
	return Platform::GetDefaultLanguage();
}

std::filesystem::path XE::CoreFramework::GetModulePath() const
{
	return GetApplicationPath() / MODULES_DIRECTORY;
}

std::filesystem::path XE::CoreFramework::GetAssetsPath() const
{
	return GetApplicationPath().parent_path() / ASSETS_DIRECTORY;
}

std::filesystem::path XE::CoreFramework::GetCachesPath() const
{
	return GetApplicationPath().parent_path() / CACHES_DIRECTORY;
}

std::filesystem::path XE::CoreFramework::GetUserDataPath() const
{
	return GetApplicationPath().parent_path() / USERDATAS_DIRECTORY;
}

std::filesystem::path XE::CoreFramework::GetApplicationPath() const
{
	return std::filesystem::absolute( std::filesystem::current_path() );
}

XE::WindowPtr XE::CoreFramework::GetMainWindow() const
{
	return _p->_MainWindow;
}

void XE::CoreFramework::Prepare()
{
	Library::RegisterEnvPath( GetModulePath() );

	Reload();

	LoadModules();

	LoadServices();

	for( auto service : _p->_Services )
	{
		service->SetFramework( this );
		service->Prepare();
	}
}

void XE::CoreFramework::Startup()
{
	_p->_Exit = false;

	for( auto i : _p->_StartServices )
	{
		if (i != std::numeric_limits< XE::uint64 >::max() )
		{
			if( !_p->_Services[i]->Startup() )
			{
				XE_LOG( LoggerLevel::Error, "startup {%0} error!", _p->_Services[i]->GetMetaClass()->GetFullName() );
				_p->_Exit = true;
				return;
			}
		}
	}
}

void XE::CoreFramework::Update()
{
	_p->_MainWindow->MessageLoop();

	for( auto i : _p->_UpdateServices )
	{
		if( i != std::numeric_limits< XE::uint64 >::max() )
		{
			_p->_Services[i]->Update();
		}
	}
}

void XE::CoreFramework::Clearup()
{
	for( auto i : _p->_ClearupServices )
	{
		if( i != std::numeric_limits< XE::uint64 >::max() )
		{
			_p->_Services[i]->Clearup();
		}
	}

	_p->_Services.clear();

	_p->_StartServices.clear();
	_p->_UpdateServices.clear();
	_p->_ClearupServices.clear();

	_p->Values.clear();
}

void XE::CoreFramework::LoadModules()
{
	auto path = GetModulePath();

	std::filesystem::directory_iterator end;
	for( std::filesystem::directory_iterator iter( path ); iter != end; ++iter )
	{
		if( std::filesystem::is_directory( *iter ) )
		{
			auto module = ( *iter ).path() / ( *iter ).path().stem();
			if( Library::Open( module.string() ) == LibraryHandle::Invalid )
			{
				XE_ERROR( "load module ${1} failed!", module.string().c_str() );
			}
		}
	}
}

void XE::CoreFramework::LoadServices()
{
	XE::Set< XE::String > Services;
	XE::Array< XE::String > StartupServices, UpdateServices, ClearupServices;

	StartupServices = GetStringArray( "System/SortStartupServices" );
	UpdateServices = GetStringArray( "System/SortUpdateServices" );
	ClearupServices = GetStringArray( "System/SortClearupServices" );

	for( const auto & i : StartupServices )
	{
		Services.insert( i );
	}
	for( const auto & i : UpdateServices )
	{
		Services.insert( i );
	}
	for( const auto & i : ClearupServices )
	{
		Services.insert( i );
	}

	for( auto service : Services )
	{
		if( auto meta = Reflection::FindClass( service ) )
		{
			if( auto ser = meta->ConstructPtr().Value< XE::ServicePtr >() )
			{
				_p->_Services.push_back( ser );
			}
		}
	}

	for( const auto & i : StartupServices )
	{
		auto it = std::find_if( _p->_Services.begin(), _p->_Services.end(), [i](const XE::ServicePtr & service )
								{
									return service->GetMetaClass()->GetFullName() == i;
								} );

		if( it != _p->_Services.end() )
		{
			_p->_StartServices.push_back( it - _p->_Services.begin() );
		}
		else
		{
			_p->_StartServices.push_back( std::numeric_limits< XE::uint64 >::max() );
		}
	}
	for( const auto & i : UpdateServices )
	{
		auto it = std::find_if( _p->_Services.begin(), _p->_Services.end(), [i]( const XE::ServicePtr & service )
								{
									return service->GetMetaClass()->GetFullName() == i;
								} );

		if( it != _p->_Services.end() )
		{
			_p->_UpdateServices.push_back( it - _p->_Services.begin() );
		}
		else
		{
			_p->_UpdateServices.push_back( std::numeric_limits< XE::uint64 >::max() );
		}
	}
	for( const auto & i : ClearupServices )
	{
		auto it = std::find_if( _p->_Services.begin(), _p->_Services.end(), [i]( const XE::ServicePtr & service )
								{
									return service->GetMetaClass()->GetFullName() == i;
								} );

		if( it != _p->_Services.end() )
		{
			_p->_ClearupServices.push_back( it - _p->_Services.begin() );
		}
		else
		{
			_p->_ClearupServices.push_back( std::numeric_limits< XE::uint64 >::max() );
		}
	}
}

void XE::CoreFramework::Save()
{
	auto path = GetUserDataPath() / CONFIG_FILE_NAME;

	Save( path, _p->Values );
}

void XE::CoreFramework::Save( const std::filesystem::path & path, const XE::Map < XE::String, XE::String > & values ) const
{
	rapidjson::Document doc;
	auto & allocator = doc.GetAllocator();
	doc.SetObject();

	for( const auto & it : values )
	{
		auto list = it.first.split( "/" );

		auto json_it = doc.FindMember( list.front().c_str() );
		if( json_it == doc.MemberEnd() )
		{
			doc.AddMember(
				rapidjson::Value().SetString( list.front().c_str(), allocator ).Move(),
				rapidjson::Value( rapidjson::kObjectType ).Move(),
				allocator );
			json_it = doc.FindMember( list.front().c_str() );
		}

		nest_json( json_it->value, list.begin() + 1, list.end() - 1, it.second, allocator );
	}

	std::ofstream ofs( path.string(), std::ios::out );
	if( ofs.is_open() )
	{
		rapidjson::OStreamWrapper wrapper( ofs );
		rapidjson::PrettyWriter< rapidjson::OStreamWrapper > writer( wrapper );
		doc.Accept( writer );
	}
	ofs.close();
}

void XE::CoreFramework::Reload()
{
	_p->Values.clear();

	auto path = GetUserDataPath() / CONFIG_FILE_NAME;

	Reload( path, _p->Values );
}

void XE::CoreFramework::Reload( const std::filesystem::path & path, XE::Map < XE::String, XE::String > & values ) const
{
	std::ifstream ifs( path.string() );
	if( ifs.is_open() )
	{
		rapidjson::Document doc;
		rapidjson::IStreamWrapper wrapper( ifs );
		doc.ParseStream( wrapper );
		if( !doc.HasParseError() )
		{
			Stack< Pair< XE::String, rapidjson::Value::ConstMemberIterator>> stack;

			for( rapidjson::Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter )
			{
				stack.push( { iter->name.GetString(), iter } );
			}

			while( !stack.empty() )
			{
				Pair< XE::String, rapidjson::Value::ConstMemberIterator > pair = stack.top();
				stack.pop();

				if( !pair.second->value.IsObject() )
				{
					values.insert( { pair.first, pair.second->value.GetString() } );
				}
				else
				{
					for( rapidjson::Value::ConstMemberIterator it = pair.second->value.MemberBegin(); it != pair.second->value.MemberEnd(); ++it )
					{
						stack.push( { pair.first + "/" + it->name.GetString(), it } );
					}
				}
			}
		}
	}
	ifs.close();
}

bool XE::CoreFramework::GetBool( const String & key, bool def )
{
	String s = GetValue( key );

	return s.empty() ? def : s == "true";
}

void XE::CoreFramework::SetBool( const String & key, bool val )
{
	SetValue( key, val ? "true" : "false" );
}

XE::int8 XE::CoreFramework::GetInt8( const String & key, XE::int8 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stoi( s.c_str() );
}

void XE::CoreFramework::SetInt8( const String & key, XE::int8 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::int16 XE::CoreFramework::GetInt16( const String & key, XE::int16 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stoi( s.c_str() );
}

void XE::CoreFramework::SetInt16( const String & key, XE::int16 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::int32 XE::CoreFramework::GetInt32( const String & key, XE::int32 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stoi( s.c_str() );
}

void XE::CoreFramework::SetInt32( const String & key, XE::int32 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::int64 XE::CoreFramework::GetInt64( const String & key, XE::int64 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stoll( s.c_str() );
}

void XE::CoreFramework::SetInt64( const String & key, XE::int64 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::uint8 XE::CoreFramework::GetUInt8( const String & key, XE::uint8 def )
{
	String s = GetValue( key );

	return s.empty() ? def : ( XE::uint8 )std::stoul( s.c_str() );
}

void XE::CoreFramework::SetUInt8( const String & key, XE::uint8 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::uint16 XE::CoreFramework::GetUInt16( const String & key, XE::uint16 def )
{
	String s = GetValue( key );

	return s.empty() ? def : ( XE::uint16 )std::stoul( s.c_str() );
}

void XE::CoreFramework::SetUInt16( const String & key, XE::uint16 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::uint32 XE::CoreFramework::GetUInt32( const String & key, XE::uint32 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stoul( s.c_str() );
}

void XE::CoreFramework::SetUInt32( const String & key, XE::uint32 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::uint64 XE::CoreFramework::GetUInt64( const String & key, XE::uint64 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stoull( s.c_str() );
}

void XE::CoreFramework::SetUInt64( const String & key, XE::uint64 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::float32 XE::CoreFramework::GetFloat32( const String & key, XE::float32 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stof( s.c_str() );
}

void XE::CoreFramework::SetFloat32( const String & key, XE::float32 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::float64 XE::CoreFramework::GetFloat64( const String & key, XE::float64 def )
{
	String s = GetValue( key );

	return s.empty() ? def : std::stod( s.c_str() );
}

void XE::CoreFramework::SetFloat64( const String & key, XE::float64 val )
{
	SetValue( key, XE::ToString( val ) );
}

XE::String XE::CoreFramework::GetString( const String & key, const String & def )
{
	String s = GetValue( key );

	return s.empty() ? def : s;
}

void XE::CoreFramework::SetString( const String & key, const String & val )
{
	SetValue( key, val );
}

void XE::CoreFramework::SetStringArray( const String & key, const Array< String > & val )
{
	rapidjson::Document doc;

	rapidjson::Value & strs = doc.SetArray();

	for( size_t i = 0; i < val.size(); i++ )
	{
		strs.PushBack( rapidjson::StringRef( val[i].c_str() ), doc.GetAllocator() );
	}

	std::stringstream str;

	rapidjson::OStreamWrapper wrapper( str );
	rapidjson::PrettyWriter< rapidjson::OStreamWrapper > writer( wrapper );
	doc.Accept( writer );

	SetValue( key, str.str() );
}

XE::Array< XE::String > XE::CoreFramework::GetStringArray( const String & key, const Array< String > & def /*= Array< String >() */ )
{
	XE::Array< XE::String > res;

	rapidjson::Document doc;
	doc.Parse( GetValue( key ).c_str() );

	auto strs = doc.GetArray();

	for( size_t i = 0; i < strs.Size(); i++ )
	{
		res.push_back( strs[i].GetString() );
	}

	return res;
}

XE::String XE::CoreFramework::GetValue( const XE::String & key )
{
	auto it = _p->Values.find( key );

	if( it != _p->Values.end() )
	{
		return it->second;
	}
	else
	{
		_p->Values.insert( { key, "" } );
	}

	return "";
}

void XE::CoreFramework::SetValue( const XE::String & key, const XE::String & val )
{
	_p->Values[key] = val;
}
