#include "HttpResponse.h"

XE::HttpResponse::HttpResponse()
{

}

XE::HttpResponse::~HttpResponse()
{

}

XE::HttpVersionType XE::HttpResponse::GetHttpVersion() const
{
	return _Version;
}

void XE::HttpResponse::SetHttpVersion( XE::HttpVersionType val )
{
	_Version = val;
}

XE::int32 XE::HttpResponse::GetCode() const
{
	return _Code;
}

void XE::HttpResponse::SetCode( XE::int32 val )
{
	_Code = val;
}

const XE::String & XE::HttpResponse::GetMsg() const
{
	return _Msg;
}

void XE::HttpResponse::SetMsg( const XE::String & val )
{
	_Msg = val;
}

const XE::String & XE::HttpResponse::GetHeader( XE::HttpHeaderFieldType field ) const
{
	auto it = _Header.find( field );
	if( it != _Header.end() )
	{
		return it->second;
	}
	return XE::String::empty_string;
}

void XE::HttpResponse::SetHeader( XE::HttpHeaderFieldType field, const XE::String & val )
{
	_Header[field] = val;
}

const XE::String & XE::HttpResponse::GetContent() const
{
	return _Content;
}

void XE::HttpResponse::SetContent( const XE::String & val )
{
	_Content = val;
}

XE::String XE::HttpResponse::ToString() const
{
	XE::String result;

	// response line
	{
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

		result += " " + XE::ToString( _Code ) + " " + _Msg;

		result += "\r\n";
	}

	// response headers
	{
		for( const auto & it : _Header )
		{
			result += ::EnumID< XE::HttpHeaderFieldType >::Get()->FindName( it.first ) + ": " + it.second + "\r\n";
		}

		result += "\r\n";
	}

	// response content
	{
		result += _Content;

		result += "\r\n\r\n";
	}

	return result;
}

bool XE::HttpResponse::FromString( const XE::String & val )
{
	auto beg = val.begin();
	auto end = val.begin() + val.find( "\r\n" );

	// response line
	{
		auto sec = XE::String( beg, end ).split( " " );

		switch( XE::Hash( sec[0] ) )
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

		XE::FromString( sec[1], _Code );

		_Msg = sec[2];
	}

	// response headers
	{
		while( *beg != '\r' && *( beg + 1 ) != '\n' )
		{
			beg = end + 2;
			end = beg + val.find( "\r\n", beg - val.begin() );

			auto pair = XE::String( beg, end ).split( "*:*" );

			_Header.insert( { ::EnumID< XE::HttpHeaderFieldType >::Get()->FindValue( pair[0] ).Value< XE::HttpHeaderFieldType >(), pair[1] } );
		}
	}

	// response content
	{
		beg = end + 2;
		end = val.end() - 4;

		_Content.assign( beg, end );
	}

	return *beg++ == '\r' && *beg++ == '\n' && *beg++ == '\r' && *beg++ == '\n';
}
