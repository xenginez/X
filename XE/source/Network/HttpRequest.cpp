#include "HttpRequest.h"

XE::HttpRequest::HttpRequest()
{

}

XE::HttpRequest::~HttpRequest()
{

}

XE::HttpMethodType XE::HttpRequest::GetHttpMethod() const
{
	return _Method;
}

void XE::HttpRequest::SetHttpMethod( XE::HttpMethodType val )
{
	_Method = val;
}

XE::HttpVersionType XE::HttpRequest::GetHttpVersion() const
{
	return _Version;
}

void XE::HttpRequest::SetHttpVersion( XE::HttpVersionType val )
{
	_Version = val;
}

const XE::String & XE::HttpRequest::GetRequest() const
{
	return _Request;
}

void XE::HttpRequest::SetRequest( const XE::String & val )
{
	_Request = val;
}

const XE::String & XE::HttpRequest::GetHeader( XE::HttpHeaderFieldType field ) const
{
	auto it = _Header.find( field );
	if( it != _Header.end() )
	{
		return it->second;
	}
	return XE::String::empty_string;
}

void XE::HttpRequest::SetHeader( XE::HttpHeaderFieldType field, const XE::String & val )
{
	_Header[field] = val;
}

const XE::String & XE::HttpRequest::GetContent() const
{
	return _Content;
}

void XE::HttpRequest::SetContent( const XE::String & val )
{
	_Content = val;
}

XE::String XE::HttpRequest::ToString() const
{
	XE::String result;

	// request line
	{
		result += ::EnumID< XE::HttpMethodType >::Get()->FindName( _Method );

		if( _Request.empty() )
		{
			return "";
		}

		result += " " + _Request + " ";

		switch( _Version )
		{
		case XE::HttpVersionType::HTTP_1_0:
			result += "HTTP/1.0";
			break;
		case XE::HttpVersionType::HTTP_1_1:
			result += "HTTP/1.1";
			break;
		default:
			return "";
		}

		result += "\r\n";
	}

	// request headers
	{
		for( const auto & it : _Header )
		{
			result += ::EnumID< XE::HttpHeaderFieldType >::Get()->FindName( it.first ) + ": " + it.second + "\r\n";
		}

		result += "\r\n";
	}

	// request content
	{
		result += _Content;

		result += "\r\n\r\n";
	}

	return result;
}

bool XE::HttpRequest::FromString( const XE::String & val )
{
	auto beg = val.begin();
	auto end = val.begin() + val.find( "\r\n" );

	// request line
	{
		auto sec = XE::String( beg, end ).split( " " );

		_Method = ::EnumID< XE::HttpMethodType >::Get()->FindValue( sec[0] ).Value< XE::HttpMethodType >();

		_Request = sec[1];

		switch( XE::Hash(sec[2]) )
		{
		case "HTTP/1.0"_hash:
			_Version = XE::HttpVersionType::HTTP_1_0;
			break;
		case "HTTP/1.1"_hash:
			_Version = XE::HttpVersionType::HTTP_1_1;
			break;
		default:
			break;
		}
	}

	// request headers
	{
		while( *beg != '\r' && *( beg + 1 ) != '\n' )
		{
			beg = end + 2;
			end = beg + val.find( "\r\n", beg - val.begin() );

			auto pair = XE::String( beg, end ).split( "*:*" );

			_Header.insert( { ::EnumID< XE::HttpMethodType >::Get()->FindValue( pair[0] ).Value< XE::HttpHeaderFieldType >(), pair[1] } );
		}
	}

	// request content
	{
		beg = end + 2;
		end = val.end() - 4;

		_Content.assign( beg, end );
	}

	return *beg++ == '\r' && *beg++ == '\n' && *beg++ == '\r' && *beg++ == '\n';
}
