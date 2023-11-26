#include "EventService.h"

#include "Base/ConcurrentList.hpp"
#include "Base/ConcurrentHashMap.hpp"

BEG_META( XE::EventService )
END_META()

namespace
{
	using ListenerList = XE::ConcurrentList< XE::EventService::ListenerType >;
	using ListenerListPtr = XE::SharedPtr< ListenerList >;
	using ListenerMap = XE::ConcurrentHashMap< XE::EventHandle, ListenerListPtr >;
}

struct XE::EventService::Private
{
	ListenerMap _Listeners;
	XE::ConcurrentQueue< XE::EventPtr > _Events;
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
	XE::EventPtr event;
	while( _p->_Events.try_pop( event ) )
	{
		ListenerListPtr list;
		if ( _p->_Listeners.find( event->handle, list ) )
		{
			for ( auto var : *list )
			{
				if ( var )
				{
					var( event );

					if ( event->accept )
					{
						return;
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

void XE::EventService::PostEvent( const XE::EventPtr & val )
{
	_p->_Events.push( val );
}

void XE::EventService::PostEvent( XE::EventHandle handle, const XE::Variant & parameter /*= XE::Variant()*/ )
{
	PostEvent( XE::MakeShared< XE::Event >( handle, parameter ) );
}

XE::Disposable XE::EventService::RegisterListener( XE::EventHandle handle, ListenerType listener )
{
	ListenerListPtr list;
	ListenerList::iterator index;

	if ( _p->_Listeners.find( handle, list ) )
	{
		index = list->emplace( list->end(), listener );
	}
	else
	{
		auto list = XE::MakeShared<ListenerList>();
		index = list->emplace( list->end(), listener );
		_p->_Listeners.insert( handle, list );
	}

	return { [this, handle, index]()
	{
		ListenerListPtr list;
		if ( _p->_Listeners.find( handle, list ) )
		{
			list->erase( index );
		}
	} };
}
