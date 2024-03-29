#include "InputService.h"

#include "Core/CoreFramework.h"

#include "InputItem.h"
#include "InputState.h"
#include "InputStateMap.h"

#if PLATFORM_OS == OS_WINDOWS
#pragma comment(lib, "xinput.lib")
#endif

BEG_META( XE::InputService )
END_META()

struct XE::InputService::Private
{
	XE::InputState _State;
	XE::InputStateMapPtr _StateMap;
	XE::Array< XE::InputItemPtr > _InputItems;
};

XE::InputService::InputService()
	:_p( XE::New< Private >() )
{

}

XE::InputService::~InputService()
{
	XE::Delete( _p );
}

void XE::InputService::Prepare()
{

}

void XE::InputService::Startup()
{
	for ( auto & it : _p->_InputItems )
	{
		it->Startup();
	}
	_p->_StateMap->Startup();
}

void XE::InputService::Update()
{
	_p->_State.ClearChanges();

	for ( auto & it : _p->_InputItems )
	{
		it->Update();
		_p->_State.Merge( it->GetState() );
	}
	_p->_StateMap->Update( _p->_State );

	_p->_State.Merge( _p->_StateMap->GetState() );
}

void XE::InputService::Clearup()
{
	for ( auto & it : _p->_InputItems )
	{
		it->Clearup();
	}
	_p->_StateMap->Clearup();

	_p->_InputItems.clear();
	_p->_State.Clear();
}

bool XE::InputService::GetButton( const XE::String & val ) const
{
	return GetValue( val ).Value< bool >();
}

bool XE::InputService::GetButtonUp( const XE::String & val ) const
{
	const auto & changes = _p->_State.GetChanges();

	auto it = changes.find( val );
	if ( it != changes.end() )
	{
		return !it->second.Value< bool >();
	}

	return false;
}

bool XE::InputService::GetButtonDown( const XE::String & val ) const
{
	const auto & changes = _p->_State.GetChanges();

	auto it = changes.find( val );
	if ( it != changes.end() )
	{
		return it->second.Value< bool >();
	}

	return false;
}

XE::float32 XE::InputService::GetAxis( const XE::String & val ) const
{
	return GetValue( val ).Value< XE::float32 >();
}

XE::Variant XE::InputService::GetValue( const XE::String & val ) const
{
	return _p->_State.GetVariant( val );
}

XE::BasicMemoryView< XE::CodePoint > XE::InputService::GetUnicodes() const
{
	return _p->_State.GetUnicodes();
}

void XE::InputService::Vibrate( XE::float32 left, XE::float32 right, XE::uint32 index /*= 0*/ )
{
	for ( auto & it : _p->_InputItems )
	{
		it->Vibrate( index, left, right );
	}
}

void XE::InputService::ClearStatus()
{
	for ( auto & it : _p->_InputItems )
	{
		it->ClearStatus();
	}
	_p->_StateMap->ClearStatus();

	_p->_State.Clear();
}

const XE::Map< XE::String, XE::Variant > & XE::InputService::GetChangeStatus() const
{
	return _p->_State.GetChanges();
}
