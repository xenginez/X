#include "EventService.h"

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_hash_map.h>

BEG_META( XE::EventService )
END_META()

using ListenerMap = tbb::concurrent_hash_map< XE::EventHandle, tbb::concurrent_vector< XE::EventService::ListenerType > >;
using GroupMap = XE::Map< XE::String, ListenerMap >;
using EventPair = XE::Pair< XE::String, XE::EventPtr >;

struct XE::EventService::Private
{
	GroupMap _Listeners;
	tbb::concurrent_queue< EventPair > _Events;
};

XE::EventService::EventService()
	:_p( XE::New< Private >() )
{

}

XE::EventService::~EventService()
{
	XE::Delete( _p );
}

void XE::EventService::Prepare()
{

}

void XE::EventService::Startup()
{

}

void XE::EventService::Update()
{
	EventPair event;
	while( _p->_Events.try_pop( event ) )
	{
		auto it = _p->_Listeners.find( event.first );
		if( it != _p->_Listeners.end() )
		{
			ListenerMap::accessor accessor;
			if( it->second.find( accessor, event.second->handle ) )
			{
				for( auto var : accessor->second )
				{
					if( var )
					{
						var( event.second );

						if( event.second->accept )
						{
							return;
						}
					}
				}
			}
		}
	}
}

void XE::EventService::Clearup()
{
	_p->_Listeners.clear();
}

void XE::EventService::PostEvent( const XE::EventPtr & val, const XE::String & group/* = ""*/ )
{
	_p->_Events.push( { group, val } );
}

void XE::EventService::PostEvent( XE::EventHandle handle, const XE::Variant & parameter /*= XE::Variant()*/, const XE::String & group /*= "" */ )
{
	XE::EventPtr event = XE::MakeShared< XE::Event >( handle, parameter );

	PostEvent( event, group );
}

XE::Disposable XE::EventService::RegisterListener( XE::EventHandle handle, ListenerType listener, const XE::String & group/* = ""*/ )
{
	GroupMap::iterator it = _p->_Listeners.find( group );
	if( it == _p->_Listeners.end() )
	{
		it = _p->_Listeners.insert( { group, ListenerMap() } ).first;
	}

	ListenerMap::accessor accessor;

	if ( it->second.find( accessor, handle ) )
	{
		auto index = accessor->second.size();

		accessor->second.push_back( listener );

		return { [this, group, handle, index]()
		{
			ListenerMap::accessor accessor;

			if( _p->_Listeners[group].find( accessor, handle ) )
			{
				accessor->second[index] = nullptr;
			}
		} };
	}

	if ( it->second.insert( accessor, handle ) )
	{
		accessor->second.push_back( listener );

		return { [this, group, handle]()
		{
			ListenerMap::accessor accessor;

			if( _p->_Listeners[group].find( accessor, handle ) )
			{
				accessor->second[0] = nullptr;
			}
		} };
	}

	return {};
}
