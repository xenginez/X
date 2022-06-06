#include "LocalizationService.h"

#include <fstream>

#include <csv2/csv2.hpp>

#undef CreateWindow

#include "CoreFramework.h"

BEG_META( XE::LocalizationService )
END_META()

struct XE::LocalizationService::Private
{
	XE::Language _Language;
	std::array< XE::UnorderedMap< XE::String, XE::String >, ( XE::uint64 )XE::Language::UNKNOWN > _Maps;
};

XE::LocalizationService::LocalizationService()
	:_p( XE::New< Private >() )
{

}

XE::LocalizationService::~LocalizationService()
{
	XE::Delete( _p );
}

void XE::LocalizationService::Prepare()
{
	String LanguageName = GetFramework()->GetString( "System/Language" );

	if( LanguageName == "" )
	{
		_p->_Language = GetFramework()->GetSystemLanguage();
	}
	else
	{
		_p->_Language = EnumID< XE::Language >::Get()->FindValue( LanguageName ).Value< XE::Language >();
	}

	LoadLocalized();
}

void XE::LocalizationService::Startup()
{

}

void XE::LocalizationService::Update()
{

}

void XE::LocalizationService::Clearup()
{
	for( auto & i : _p->_Maps )
	{
		i.clear();
	}
}

XE::Language XE::LocalizationService::GetCurrentLanguage() const
{
	return _p->_Language;
}

void XE::LocalizationService::SetCurrentLanguage( XE::Language val )
{
	_p->_Language = val;

	GetFramework()->SetString( "System/Language", EnumID< XE::Language >::Get()->FindName( _p->_Language ) );
}

const XE::String & XE::LocalizationService::LocalizedString( const XE::String & key ) const
{
	auto it = _p->_Maps[( XE::uint64 )_p->_Language].find( key );

	if ( it != _p->_Maps[( XE::uint64 )_p->_Language].end() && it->second != "" )
	{
		return it->second;
	}

	return key;
}

void XE::LocalizationService::LoadLocalized()
{
	std::string path = ( GetFramework()->GetUserDataPath() / LANGUAGE_FILE_NAME ).string();
	
	csv2::Reader< csv2::delimiter< ',' >, csv2::quote_character< '"' >, csv2::first_row_is_header< true >, csv2::trim_policy::trim_whitespace > csv;

	if( csv.mmap( path ) )
	{
		const auto header = csv.header();

		XE::Array< XE::uint64 > indexs( XE::MemoryResource::GetFrameMemoryResource() );
		for( auto col = ++header.begin(); col != header.end(); ++col )
		{
			std::string cell;
			( *col ).read_value( cell );
			indexs.push_back( ( XE::uint64 )( EnumID< XE::Language >::Get()->FindValue( std::move( cell ) ).Value< XE::Language >() ) );
		}

		for( const auto row : csv )
		{
			std::string key;
			auto col = row.begin();
			( *col ).read_value( key );
			++col;

			for( int i = 1; col != row.end(); ++col, ++i )
			{
				std::string cell;
				( *col ).read_value( cell );
				_p->_Maps[indexs[i]].insert( { key, std::move( cell ) } );
			}
		}
	}
}
