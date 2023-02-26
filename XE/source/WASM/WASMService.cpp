#include "WASMService.h"

#include "Core/CoreFramework.h"
#include "Core/AssetsService.h"

#include "WASMModule.h"
#include "WASMContext.h"
#include "WASMInterpreter.h"

IMPLEMENT_META( XE::WASMService );

struct XE::WASMService::Private
{
	XE::WASMModulePtr _Module;
	XE::Array< XE::uint8 > _Memory;
	XE::Array< XE::uint64 > _Tables;
	XE::Array< XE::WASMValue > _Globals;
	XE::Array< XE::WASMFunc > _Functions;
	XE::Map< XE::String, XE::uint64 > _GlobalMap;
	XE::Map< XE::String, XE::uint64 > _FunctionMap;
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

void XE::WASMService::Startup()
{
	if( auto asset = GetFramework()->GetServiceT< XE::AssetsService >() )
	{
		auto view = asset->Load( "XE.wasm" );
		if ( view.size() != 0 )
		{
			_p->_Module = XE::MakeShared< XE::WASMModule >();
			_p->_Module->Load( view );

			Instantiate();
		}
	}
}

void XE::WASMService::Update()
{

}

void XE::WASMService::Clearup()
{
	_p->_Functions.clear();
	_p->_Globals.clear();
	_p->_Tables.clear();
	_p->_Memory.clear();

	_p->_Module = nullptr;
}

void XE::WASMService::Instantiate()
{

}

XE::uint64 XE::WASMService::FindGlobal( const XE::String & val )
{
	auto it = _p->_GlobalMap.find( val );
	if ( it != _p->_GlobalMap.end() )
	{
		return it->second;
	}
	return npos;
}

XE::uint64 XE::WASMService::FindFunction( const XE::String & val )
{
	auto it = _p->_FunctionMap.find( val );
	if ( it != _p->_FunctionMap.end() )
	{
		return it->second;
	}
	return npos;
}

XE::Variant XE::WASMService::GetGlobal( XE::uint64 idx )
{
	return _p->_Globals[idx];
}

void XE::WASMService::SetGlobal( XE::uint64 idx, const XE::Variant & val )
{
	_p->_Globals[idx] = val.Value< XE::WASMValue >();
}

XE::Variant XE::WASMService::InvokeFunction( XE::uint64 idx, XE::InvokeStack * params )
{
	return {};
}

XE::WASMContext * XE::WASMService::GetContext() const
{
	thread_local WASMContext ctx;
	return &ctx;
}
