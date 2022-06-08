/*!
 * \file	Logger.h
 *
 * \author	ZhengYuanQing
 * \date	2020/05/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef LOGGER_H__FABB4435_9587_4290_9EA3_18733C722EE2
#define LOGGER_H__FABB4435_9587_4290_9EA3_18733C722EE2

#if __cplusplus > 202000L
#include <source_location>
#endif

#include "Disposable.h"

BEG_XE_NAMESPACE

class XE_API Logger : public XE::Singleton< XE::Logger >
{
public:
	struct Private;

	using ListenerType = XE::Delegate< void( std::chrono::system_clock::time_point, const char *, XE::uint32, const char *, XE::LoggerLevel, XE::Utf8String ) >;

public:
	Logger();

	~Logger();

public:
	static void Log( const char * file, XE::uint32 line, const char * func, XE::LoggerLevel level, const XE::Utf8String & text );

public:
	static XE::Disposable RegisterListener( ListenerType listener );

private:
	Private * _p;
};

XE_INLINE XE::String ToString(XE::LoggerLevel _Val )
{
	switch( _Val )
	{
	case XE::LoggerLevel::Error:
		return "Error";
	case XE::LoggerLevel::Warning:
		return "Warning";
	case XE::LoggerLevel::Message:
		return "Message";
	}

	return "";
}

END_XE_NAMESPACE

#define XE_LOG(LEVEL, FMT, ...) XE::Logger::Log( __FILE__, __LINE__, __FUNCTION__, LEVEL, XE::Format( FMT, __VA_ARGS__ ) )

#define XE_ERROR( FMT, ... ) XE_LOG( XE::LoggerLevel::Error, FMT, __VA_ARGS__ )
#define XE_WARNING( FMT, ... ) XE_LOG( XE::LoggerLevel::Warning, FMT, __VA_ARGS__ )
#define XE_MESSAGE( FMT, ... ) XE_LOG( XE::LoggerLevel::Message, FMT, __VA_ARGS__ )

#endif // LOGGER_H__FABB4435_9587_4290_9EA3_18733C722EE2
