#include "InputStateMap.h"

#include "Core/TimerService.h"
#include "Core/CoreFramework.h"

BEG_META( XE::InputStateMap )
type->Property( "StateItems", &XE::InputStateMap::GetStateItems, &XE::InputStateMap::SetStateItems );
END_META()

struct XE::InputStateMap::Private
{
	XE::InputState * _State;
	XE::InputState _State1, _State2;
	XE::Map< XE::String, XE::Array< XE::InputStateItemPtr > > _Items;
	XE::Map< XE::String, XE::Pair< XE::uint64, XE::uint64 > > _Caches;
};

XE::InputStateMap::InputStateMap()
	:_p( XE::New< Private >() )
{

}

XE::InputStateMap::~InputStateMap()
{
	XE::Delete( _p );
}

void XE::InputStateMap::Startup()
{
	_p->_State = &_p->_State1;
	for ( const auto & it : _p->_Items )
	{
		_p->_Caches.insert( { it.first, { 0.0f, 0 } } );
	}
}

void XE::InputStateMap::Update( const XE::InputState & state )
{
	XE::float32 time = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->GetTime();
	XE::float32 unscale = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->GetTime();
	XE::uint64 frame = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->GetFrameCount();

	auto prev = _p->_State; _p->_State = _p->_State == &_p->_State1 ? &_p->_State2 : &_p->_State1;
	{
		for ( const auto & list : _p->_Items )
		{
			auto cache = _p->_Caches.find( list.first );
			const auto & item = list.second[cache->second.second];

			XE::uint64 now = 0;
			XE::uint64 timeout = 0;

			switch ( item->GetTimeoutType() )
			{
			case XE::InputStateItemTimeoutType::TIME:
				now = static_cast<XE::uint64>( time * 1000 );
				timeout = static_cast<XE::uint64>( item->GetTimeout() * 1000 );
				break;
			case XE::InputStateItemTimeoutType::FRAME:
				now = frame;
				timeout = static_cast<XE::uint64>( item->GetTimeout() );
				break;
			case XE::InputStateItemTimeoutType::UNSCALE_TIME:
				now = static_cast<XE::uint64>( unscale * 1000 );
				timeout = static_cast<XE::uint64>( item->GetTimeout() * 1000 );
				break;
			}

			if ( cache->second.second == 0 || ( now - cache->second.first ) <= timeout )
			{
				auto value = state.GetVariant( item->GetInputName() );
				if ( !value.IsInvalid() )
				{
					if ( item->Condition( value ) )
					{
						cache->second.first = now;
						cache->second.second += 1;

						if ( cache->second.second == list.second.size() )
						{
							_p->_State->PutVariant( list.first, true );
							cache->second.second = 0;
						}
					}
				}
			}
			else
			{
				cache->second.second = 0;
			}
		}
	}
	prev->Clear();
}

void XE::InputStateMap::Clearup()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
}

void XE::InputStateMap::ClearStatus()
{
	_p->_State1.Clear();
	_p->_State2.Clear();
}

const XE::InputState & XE::InputStateMap::GetState() const
{
	return *_p->_State;
}

const XE::Map< XE::String, XE::Array< XE::InputStateItemPtr > > & XE::InputStateMap::GetStateItems() const
{
	return _p->_Items;
}

void XE::InputStateMap::SetStateItems( const XE::Map< XE::String, XE::Array< XE::InputStateItemPtr > > & val )
{
	_p->_Items = val;
}
