/*!
 * \file	HttpRequest.h
 *
 * \author	ZhengYuanQing
 * \date	2022/01/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef HTTPREQUEST_H__D4C47B1A_E99D_44AD_A126_F034A8A3A7DF
#define HTTPREQUEST_H__D4C47B1A_E99D_44AD_A126_F034A8A3A7DF

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API HttpRequest
{
public:
	HttpRequest();

	~HttpRequest();

public:
	XE::HttpMethodType GetHttpMethod() const;

	void SetHttpMethod( XE::HttpMethodType val );

	XE::HttpVersionType GetHttpVersion() const;

	void SetHttpVersion( XE::HttpVersionType val );

	const XE::String & GetRequest() const;

	void SetRequest( const XE::String & val );

public:
	const XE::String & GetHeader( XE::HttpHeaderFieldType field ) const;

	void SetHeader( XE::HttpHeaderFieldType field, const XE::String & val );

public:
	const XE::String & GetContent() const;

	void SetContent( const XE::String & val );

public:
	XE::String ToString() const;

	bool FromString( const XE::String & val );

public:
	XE::HttpMethodType _Method = XE::HttpMethodType::HTTP_GET;
	XE::HttpVersionType _Version = XE::HttpVersionType::HTTP_1_1;
	XE::String _Request;
	XE::Map< XE::HttpHeaderFieldType, XE::String > _Header;

	XE::String _Content;
};

END_XE_NAMESPACE

#endif//HTTPREQUEST_H__D4C47B1A_E99D_44AD_A126_F034A8A3A7DF
