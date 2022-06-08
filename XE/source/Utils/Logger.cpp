#include "Logger.h"

struct XE::Logger::Private
{
	XE::ConcurrentList< XE::Logger::ListenerType > _Listeners;
};

XE::Logger::Logger()
	:_p( XE::New< Private >() )
{
}

XE::Logger::~Logger()
{
	XE::Delete( _p );
}

#define _p XE::Logger::Instance()->_p

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
	auto it = _p->_Listeners.emplace_back( listener );

	return XE::Disposable{ [it]()
	{
		_p->_Listeners.erase( it );
	} };
}
