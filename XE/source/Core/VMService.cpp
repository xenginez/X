#include "VMService.h"

#include "CoreFramework.h"
#include "AssetsService.h"

#include "VM/Module.h"
#include "Utils/UUID.h"

IMPLEMENT_META( XE::VMService );

struct XE::VMService::Private
{
	XE::Array< XE::ModulePtr > _Modules;
};

XE::VMService::VMService()
	: _p( XE::New< Private >() )
{

}

XE::VMService::~VMService()
{
	XE::Delete( _p );
}

void XE::VMService::Prepare()
{

}

bool XE::VMService::Startup()
{
	if( auto asset = GetFramework()->GetServiceT < XE::AssetsService >() )
	{
		auto uuids = asset->GetAssetsFromType( "wasm" );

		for( const auto & it : uuids )
		{
			auto view = asset->Load( it );
			if( view.size() != 0 )
			{
				XE::ModulePtr mod = XE::MakeShared< XE::Module >();

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

void XE::VMService::Update()
{

}

void XE::VMService::Clearup()
{

}
