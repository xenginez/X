#include "Exception.h"

XE::Exception::Exception( const XE::String & message )
	:_Message( message )
{

}

XE::Exception::~Exception()
{

}

char const * XE::Exception::what() const
{
	return _Message.c_str();
}

XE::RuntimeException::RuntimeException( const XE::String & message )
	:Exception( message )
{

}

XE::RuntimeException::~RuntimeException()
{

}
