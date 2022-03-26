/*!
 * \file   Exception.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __EXCEPTION_H__F46ED19F_D50F_495C_9BD6_1ADAFEBA5700
#define __EXCEPTION_H__F46ED19F_D50F_495C_9BD6_1ADAFEBA5700

#include "String.h"

BEG_XE_NAMESPACE

class XE_API Exception : public std::exception
{
public:
	Exception( const XE::String & message );

	~Exception() override;

public:
	char const * what() const override;

private:
	XE::String _Message;
};

class XE_API RuntimeException : public XE::Exception
{
public:
	RuntimeException( const XE::String & message );

	~RuntimeException() override;
};

END_XE_NAMESPACE

#endif // __EXCEPTION_H__F46ED19F_D50F_495C_9BD6_1ADAFEBA5700
