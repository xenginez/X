/*!
 * \file	HttpResponse.h
 *
 * \author	ZhengYuanQing
 * \date	2022/01/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef HTTPRESPONSE_H__1EE75445_B432_43F2_850C_06094234B7CC
#define HTTPRESPONSE_H__1EE75445_B432_43F2_850C_06094234B7CC

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API HttpResponse
{
public:
	HttpResponse();

	~HttpResponse();

public:
	XE::HttpVersionType GetHttpVersion() const;

	void SetHttpVersion( XE::HttpVersionType val );

	XE::int32 GetCode() const;

	void SetCode( XE::int32 val );

	const XE::String & GetMsg() const;

	void SetMsg( const XE::String & val );

public:
	const XE::String & GetHeader( XE::HttpHeaderFieldType field ) const;

	void SetHeader( XE::HttpHeaderFieldType field, const XE::String & val );

public:
	const XE::String & GetContent() const;

	void SetContent( const XE::String & val );

public:
	XE::String ToString() const;

	bool FromString( const XE::String & val );

private:
	XE::HttpVersionType _Version;
	XE::int32 _Code;
	XE::String _Msg;
	XE::Map< XE::HttpHeaderFieldType, XE::String > _Header;

	XE::String _Content;
};

END_XE_NAMESPACE

#endif//HTTPRESPONSE_H__1EE75445_B432_43F2_850C_06094234B7CC
