#include "Logger.h"

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>

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

#if __cplusplus > 202000L
void XE::Logger::Log( const std::source_location & location, XE::LoggerLevel level, const XE::Utf8String & text )
{
	auto time = std::chrono::system_clock::now();

	for( const auto & listener : _p->_Listeners )
	{
		if( listener )
		{
			listener( time, location.file_name(), location.line(), location.function_name(), level, text );
		}
	}
}
#endif

void XE::Logger::Log( const char * file, XE::uint32 line, const char * func, XE::LoggerLevel level, const XE::Utf8String & text )
{
	auto time = std::chrono::system_clock::now();

	for( const auto & listener : _p->_Listeners )
	{
		if( listener )
		{
			listener( time, file, line, func, level, text );
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
