#include "Logger.h"

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>

#if PLATFORM_OS == OS_ANDROID
#include <android/log.h>
#endif

struct XE::Logger::Private
{
	tbb::concurrent_queue< XE::uint64 > _FreeIndex;
	tbb::concurrent_vector< XE::Logger::ListenerType > _Listeners;
};

XE::Logger::Logger()
	:_p( XE::New< Private >() )
{
}

XE::Logger::~Logger()
{
	XE::Delete( _p );
}

XE::Logger * XE::Logger::Instance()
{
	static XE::Logger instance;
	return &instance;
}

#define _p XE::Logger::Instance()->_p

void XE::Logger::Log( const char * file, XE::uint32 line, LoggerLevel level, const XE::String & text )
{
	auto time = std::chrono::system_clock::now();

	for( const auto & listener : _p->_Listeners )
	{
		if( listener )
		{
			listener( time, file, line, level, text );
		}
	}
}

XE::Disposable XE::Logger::RegisterListener( ListenerType listener )
{
	XE::uint64 index = 0;

	if( !_p->_FreeIndex.try_pop( index ) )
	{
		index = _p->_Listeners.push_back( listener ) - _p->_Listeners.begin();
	}

	return XE::Disposable{ [index]()
	{
		_p->_Listeners[index] = nullptr;
		_p->_FreeIndex.push( index );
	} };
}
