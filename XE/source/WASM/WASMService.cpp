#include "WASMService.h"

#include "CoreFramework.h"
#include "AssetsService.h"

#include "Utils/UUID.h"
#include "WASM/WASMModule.h"

IMPLEMENT_META( XE::WASMService );

struct XE::WASMService::Private
{
	XE::Array< XE::WASMModulePtr > _Modules;
};

XE::WASMService::WASMService()
	: _p( XE::New< Private >() )
{

}

XE::WASMService::~WASMService()
{
	XE::Delete( _p );
}

void XE::WASMService::Prepare()
{

}

bool XE::WASMService::Startup()
{
	if( auto asset = GetFramework()->GetServiceT < XE::AssetsService >() )
	{
		auto uuids = asset->GetAssetsFromType( "wasm" );

		for( const auto & it : uuids )
		{
			auto view = asset->Load( it );
			if( view.size() != 0 )
			{
				XE::WASMModulePtr mod = XE::MakeShared< XE::WASMModule >();

				mod->Load( view );

				_p->_Modules.push_back( mod );
			}
		}

		for( const auto & it : _p->_Modules )
		{

		}
	}

	return true;
}

void XE::WASMService::Update()
{

}

void XE::WASMService::Clearup()
{

}
