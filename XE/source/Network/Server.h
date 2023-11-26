/*!
 * \file	Server.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SERVER_H__BAC42B2F_9C7C_445E_919D_21BD53777A6F
#define SERVER_H__BAC42B2F_9C7C_445E_919D_21BD53777A6F

#include "Base/Endian.h"
#include "Base/ConcurrentHashMap.hpp"

#include "HttpRequest.h"
#include "HttpResponse.h"

BEG_XE_NAMESPACE

class XE_API Server : public XE::NonCopyable, public XE::EnableSharedFromThis< XE::Server >
{
	friend class NetworkService;

public:
	using AcceptCallbackType = XE::Delegate< void( XE::ServerHandle, XE::SessionHandle, std::error_code ) >;
	using HandshakeCallbackType = XE::Delegate< void( XE::ServerHandle, XE::SessionHandle, std::error_code ) >;
	using SendCallbackType = XE::Delegate< void( XE::ServerHandle, XE::SessionHandle, std::error_code, XE::uint64 ) >;
	using ReceiveCallbackType = XE::Delegate< void( XE::ServerHandle, XE::SessionHandle, std::error_code, XE::MemoryView ) >;

public:
	Server();

	virtual ~Server();

public:
	XE::ServerHandle GetHandle() const;

	XE::Endpoint GetLocalEndpoint() const;

	XE::Endpoint GetRemoteEndpoint( XE::SessionHandle val ) const;

public:
	virtual void BindAcceptCallback( const AcceptCallbackType & callback );

	virtual void BindHandshakeCallback( const HandshakeCallbackType & callback );

	virtual void BindSendCallback( const SendCallbackType & callback );

	virtual void BindReceiveCallback( const ReceiveCallbackType & callback );

public:
	virtual void Send( XE::SessionHandle session, XE::MemoryView data ) = 0;

	virtual void Close( XE::SessionHandle session ) = 0;

	virtual void Close() = 0;

protected:
	XE::ServerHandle _Handle;
	void * _IOService = nullptr;

	XE::Endpoint _LocalEndpoint;

	SendCallbackType _SendCB = nullptr;
	AcceptCallbackType _AcceptCB = nullptr;
	ReceiveCallbackType _ReceiveCB = nullptr;
	HandshakeCallbackType _HandshakeCB = nullptr;

	XE::Array< XE::UniquePtr< XE::Session > > _Sessions;
	XE::QueueHandleAllocator< XE::SessionHandle, std::numeric_limits< XE::uint32 >::max() > _SessionAllocator;
};

class XE_API TCPServer : public Server
{
private:
	struct Private;

public:
	TCPServer();

	~TCPServer() override;

public:
	void Start( XE::uint16 bind, const XE::EndlCondition & endl );

	void Start( XE::uint16 bind, const XE::RegexCondition & regex );

	void Start( XE::uint16 bind, const XE::MatchCondition & match );

	void Start( XE::uint16 bind, const XE::TransferCondition & trans );

	void Start( XE::uint16 bind, const XE::DataGramCondition & dgram );

public:
	void Send( XE::SessionHandle session, XE::MemoryView view ) override;

	void Close( XE::SessionHandle session ) override;

	void Close() override;

private:
	void Accept();

	void Read( XE::SessionHandle session );

	void Write( XE::SessionHandle session );

private:
	Private * _p;
};

class XE_API UDPServer : public Server
{
private:
	struct Private;

public:
	UDPServer();

	~UDPServer() override;

public:
	void Start( XE::uint16 bind, XE::uint64 buf_size = 1400 );

public:
	void Send( XE::SessionHandle session, XE::MemoryView view ) override;

	void Close( XE::SessionHandle session ) override;

	void Close() override;

private:
	void Accept();

	void Write( XE::SessionHandle session );

private:
	Private * _p;
};

class XE_API KCPServer : public Server
{
private:
	struct Private;

public:
	KCPServer();

	~KCPServer() override;

public:
	void Start( XE::uint16 bind, XE::uint64 buf_size = 1400 );

public:
	void Send( XE::SessionHandle session, XE::MemoryView view ) override;

	void Close( XE::SessionHandle session ) override;

	void Close() override;

private:
	void Accept();

	void Write( XE::SessionHandle session );

private:
	Private * _p;
};

template< typename Protocol > class HttpServer : public Protocol
{
public:
	using ProtocolType = Protocol;
	using CallbackType = XE::Delegate< XE::HttpResponse( const XE::HttpRequest & )>;

public:
	HttpServer()
	{
		ProtocolType::BindReceiveCallback( { &HttpServer<Protocol>::OnProtocolReceive, this } );
	}

	~HttpServer() override = default;

public:
	void BindReceiveCallback( const Server::ReceiveCallbackType & callback ) override
	{
		_HttpReceiveCB = callback;
	}

public:
	void Bind( XE::HttpMethodType method, const XE::String & path, const CallbackType & callback )
	{
		_Callbacks[(int)method].insert( { path, callback } );
	}

private:
	void OnProtocolReceive( XE::ServerHandle server, XE::SessionHandle session, std::error_code code, XE::MemoryView view )
	{
		if ( !code )
		{
			XE::HttpRequest requst;
			requst.FromString( { view.data(), view.size() } );

			int method = (int)requst.GetHttpMethod();

			auto it = _Callbacks[method].find( requst.GetRequest() );
			if ( it != _Callbacks[method].end() )
			{
				auto response = it( requst ).ToString();
				ProtocolType::Send( { response.data(), response.size() } );
			}
			else
			{
				XE::HttpResponse response;
				{
					response.SetHttpVersion( requst.GetHttpVersion() );
					response.SetCode( 404 );
					response.SetMsg( "Not Found" );
				}
				XE::String data = response.ToString();
				ProtocolType::Send( { data.data(), data.size() } );
			}
		}
		else
		{
			_HttpReceiveCB( server, session, code, view );
		}
	}

private:
	Server::ReceiveCallbackType _HttpReceiveCB = nullptr;
	std::array<XE::Map<XE::String, CallbackType>, 8> _Callbacks;
};

template< typename Protocol, typename IArchive, typename OArchive > class RPCServer : public Protocol
{
public:
	using ProtocolType = Protocol;
	using IArchiveType = IArchive;
	using OArchiveType = OArchive;
	using ResultCallbackType = XE::Delegate< void( std::error_code, XE::MemoryView ) >;
	using FunctionCallbackType = XE::Delegate< void( XE::MemoryView, ResultCallbackType ) >;

private:
	using ProtocolType::Send;
	using ProtocolType::BindReceiveCallback;

public:
	RPCServer()
	{
		ProtocolType::BindReceiveCallback( { &RPCServer<Protocol>::OnProtocolReceive, this } );
	}

	~RPCServer() = default;

public:
	template< typename F > bool Bind( const XE::String & name, F && f )
	{
		auto it = _Callbacks.find( name );
		if( it != _Callbacks.end() )
		{
			return false;
		}

		_Callbacks[name] = [f = std::move( f )]( XE::MemoryView view, ResultCallbackType result )
		{
			using traits = XE::FunctionTraits< F >;

			typename traits::pod_tuple_type tuple;

			{
				XE::MemoryStream in( XE::MemoryResource::GetFrameMemoryResource() );

				in.view( view );

				IArchiveType arch( in );

				arch & tuple;
			}

			if constexpr( std::is_void_v< traits::result_type > )
			{
				std::apply( [f = std::move( f )]( auto && ... args )
				{
					XE::MemoryStream out( XE::MemoryResource::GetFrameMemoryResource() );
					OArchiveType arch( out );

					arch & f( args... );

					result( {}, out.view() );
				}, tuple );
			}
			else
			{
				std::apply( [f = std::move( f )]( auto && ... args )
				{
					f( args... );

					result( {}, {} );
				}, tuple );
			}
		};
	}

public:
	template< typename F, typename ... Args > void LocalInvoke( const XE::String & name, F && f, Args &&... args )
	{
		using traits = XE::FunctionTraits< F >;

		auto it = _Callbacks.find( name );
		if( it != _Callbacks.end() )
		{
			XE::MemoryStream out( XE::MemoryResource::GetFrameMemoryResource() );
			OArchiveType arch( out );

			XE::ApplyTuple( [&arch]( auto && arg ) { arch & std::forward<decltype( arg )>( arg ); }, std::make_tuple( args... ) );

			it->second( out.view(), [f = std::move( f )]( std::error_code code, XE::MemoryView view )
			{
				if( !code )
				{
					if constexpr( traits::args_size == 2 )
					{
						typename std::tuple_element< 1, typename traits::pod_tuple_type >::type result;

						XE::MemoryStream in( XE::MemoryResource::GetFrameMemoryResource() ); in.view( view );

						IArchiveType arch( in );

						arch & result;

						f( code, result );
					}
					else
					{
						f( code );
					}
				}
				else
				{
					if constexpr( traits::args_size == 2 )
					{
						f( code, {} );
					}
					else
					{
						f( code );
					}
				}
			} );
		}
		else
		{
			if constexpr( traits::args_size == 2 )
			{
				f( std::make_error_code( std::errc::function_not_supported ), {} );
			}
			else
			{
				f( std::make_error_code( std::errc::function_not_supported ) );
			}
		}
	}

	template< typename F, typename ... Args > void RemoteInvoke( XE::SessionHandle session, const XE::String & name, F && f, Args &&... args )
	{
		using traits = XE::FunctionTraits< F >;

		auto handle = _Allocator.Alloc();

		XE::int8 buf[sizeof( XE::uint64 )] = {};
		XE::WriteBigEndian( buf, handle.GetValue() );

		XE::MemoryStream out( XE::MemoryResource::GetFrameMemoryResource() );

		out << static_cast<XE::uint8>( XE::RPCProtocolType::INVOKE );
		out.write( buf, sizeof( XE::uint64 ) );
		out.write( name.c_str(), name.size() + 1 );
		OArchiveType arch( out );

		XE::ApplyTuple( [&arch]( auto && arg ) { arch & std::forward<decltype( arg )>( arg ); }, std::make_tuple( args... ) );

		Send( session, out.view() );

		_Results.insert( handle, [f = std::move( f )]( std::error_code code, XE::MemoryView view )
		{
			if( !code )
			{
				if constexpr( traits::args_size == 2 )
				{
					typename std::tuple_element< 1, typename traits::pod_tuple_type >::type result;

					XE::MemoryStream in( XE::MemoryResource::GetFrameMemoryResource() ); in.view( view );

					IArchiveType arch( in );

					arch & result;

					f( code, result );
				}
				else
				{
					f( code );
				}
			}
			else
			{
				if constexpr( traits::args_size == 2 )
				{
					f( code, {} );
				}
				else
				{
					f( code );
				}
			}
		} );
	}

private:
	void OnProtocolReceive( XE::ServerHandle handle, XE::SessionHandle session, std::error_code code, XE::MemoryView view )
	{
		if( !code )
		{
			auto buf = view.data();

			XE::RPCProtocolType flag = *reinterpret_cast<const XE::RPCProtocolType *>( buf++ );

			switch( flag )
			{
			case XE::RPCProtocolType::INVOKE:
			{
				XE::uint64 handle;
				buf += XE::ReadBigEndian( buf, handle );

				XE::String name;
				while( *buf != 0 ) name.push_back( *buf++ );
				buf++;

				auto result_callback = [handle, session]( std::error_code code, XE::MemoryView view )
				{
					XE::MemoryStream out( XE::MemoryResource::GetFrameMemoryResource() );

					out << static_cast<XE::uint8>( XE::RPCProtocolType::RESULT ) << code.value();

					if( !code )
					{
						XE::int8 buf[sizeof( XE::uint64 )] = {};
						XE::WriteBigEndian( buf, handle.GetValue() );
						out.write( buf, sizeof( XE::uint64 ) );
						out.write( view.data(), view.size() );
					}

					Send( session, out.view() );
				};

				auto it = _Callbacks.find( name );
				if( it != _Callbacks.end() )
				{
					it->second( { buf, view.size() - ( buf - view.data() ) }, result_callback );
				}
				else
				{
					result_callback( std::make_error_code( std::errc::function_not_supported ), {} );
				}
			}
			break;
			case XE::RPCProtocolType::RESULT:
			{
				XE::uint64 handle;
				buf += XE::ReadBigEndian( buf, handle );

				ResultCallbackType result;
				if( _Results.find( XE::HandleCast< XE::RPC >( handle ), result ) )
				{
					result( code, { buf, view.size() - ( buf - view.data() ) } );

					_Results.erase( XE::HandleCast< XE::RPC >( handle ) );
				}
			}
			break;
			default:
				break;
			}
		}
	}

private:
	XE::QueueHandleAllocator< XE::RPC > _Allocator;
	XE::Map< XE::String, FunctionCallbackType > _Callbacks;
	XE::ConcurrentHashMap< XE::RPCHandle, ResultCallbackType > _Results;
};

END_XE_NAMESPACE

#endif//SERVER_H__BAC42B2F_9C7C_445E_919D_21BD53777A6F
