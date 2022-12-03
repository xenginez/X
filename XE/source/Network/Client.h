/*!
 * \file	Client.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CLIENT_H__A413619C_1137_4FB0_A2C3_41408D07E9AA
#define CLIENT_H__A413619C_1137_4FB0_A2C3_41408D07E9AA

#include "HttpRequest.h"
#include "HttpResponse.h"

BEG_XE_NAMESPACE

class XE_API Client : public XE::NonCopyable, public XE::EnableSharedFromThis< Client >
{
	friend class NetworkService;

public:
	using HandshakeCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code ) >;
	using ConnectCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code ) >;
	using SendCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code, XE::uint64 ) >;
	using ReceiveCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code, XE::MemoryView ) >;

public:
	Client();

	virtual ~Client();

public:
	XE::ClientHandle GetHandle() const;

	XE::Endpoint GetLocalEndpoint() const;

	XE::Endpoint GetRemoteEndpoint() const;

public:
	virtual void BindConnectCallback( const ConnectCallbackType & callback );

	virtual void BindHandshakeCallback( const HandshakeCallbackType & callback );

	virtual void BindSendCallback( const SendCallbackType & callback );

	virtual void BindReceiveCallback( const ReceiveCallbackType & callback );

public:
	virtual void Send( XE::MemoryView view ) = 0;

	virtual void Close() = 0;

protected:
	XE::ClientHandle _Handle;
	void * _IOService = nullptr;

	XE::Endpoint _LocalEndpoint;
	XE::Endpoint _RemoteEndpoint;

	SendCallbackType _SendCB = nullptr;
	ReceiveCallbackType _ReceiveCB = nullptr;
	ConnectCallbackType _ConnectCB = nullptr;
	HandshakeCallbackType _HandshakeCB = nullptr;
};

class XE_API TCPClient : public Client
{
private:
	struct Private;

public:
	TCPClient();

	~TCPClient() override;

public:
	void Start( const XE::Endpoint & endpoint, const XE::EndlCondition & endl );

	void Start( const XE::Endpoint & endpoint, const XE::RegexCondition & regex );

	void Start( const XE::Endpoint & endpoint, const XE::MatchCondition & match );

	void Start( const XE::Endpoint & endpoint, const XE::TransferCondition & trans );

	void Start( const XE::Endpoint & endpoint, const XE::DataGramCondition & dgram );

public:
	void Send( XE::MemoryView view ) override;

	void Close() override;

private:
	void Read();

	void Write();

	void Connect();

private:
	Private * _p;
};

class XE_API UDPClient : public Client
{
private:
	struct Private;

public:
	UDPClient();

	~UDPClient() override;

public:
	void Start( const XE::Endpoint & endpoint, XE::uint64 buf_size = 1400 );

public:
	void Send( XE::MemoryView view ) override;

	void Close() override;

private:
	void Read();

	void Write();

	void Connect();

private:
	Private * _p;
};

class XE_API KCPClient : public Client
{
private:
	struct Private;

public:
	KCPClient();

	~KCPClient() override;

public:
	void Start( const XE::Endpoint & endpoint, XE::uint64 buf_size = 1400 );

public:
	void Send( XE::MemoryView view ) override;

	void Close() override;

private:
	void Read();

	void Write();

	void Connect();

private:
	Private * _p;
};

template< typename Protocol > class HttpClient : public Protocol
{
public:
	using ProtocolType = Protocol;
	using HttpReceiveCallbackType = XE::Delegate<void( const XE::HttpResponse & )>;

public:
	HttpClient()
	{
		ProtocolType::BindReceiveCallback( { &HttpClient<Protocol>::OnProtocolReceive, this } );
	}

	~HttpClient() override = default;

private:
	using ProtocolType::Send;
	using ProtocolType::BindReceiveCallback;

public:
	void BindReceiveCallback( const HttpReceiveCallbackType & callback )
	{
		_HttpReceiveCB = callback;
	}

public:
	void Send( const XE::HttpRequest & request )
	{
		auto data = request.ToString();

		ProtocolType::Send( { data.data(), data.size() } );
	}

private:
	void OnProtocolReceive( XE::ClientHandle handle, std::error_code code, XE::MemoryView view )
	{
		if ( !code )
		{
			XE::HttpResponse response;
			response.FromString( { view.data(), view.size() } );

			if ( _HttpReceiveCB )
			{
				_HttpReceiveCB( response );
			}
		}
	}

private:
	HttpReceiveCallbackType _HttpReceiveCB;
};

template< typename Protocol, typename IArchive, typename OArchive > class RPCClient : public Protocol
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
	RPCClient()
	{
		ProtocolType::BindReceiveCallback( { &RPCClient<Protocol, IArchive, OArchive>::OnProtocolReceive, this } );
	}

	~RPCClient() override = default;

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

			if constexpr( !std::is_void_v< traits::result_type > )
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

	template< typename F, typename ... Args > void RemoteInvoke( const XE::String & name, F && f, Args &&... args )
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

		Send( out.view() );

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
	void OnProtocolReceive( XE::ClientHandle handle, std::error_code code, XE::MemoryView view )
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

				auto result_callback = [handle]( std::error_code code, XE::MemoryView view )
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

					Send( out.view() );
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

#endif//CLIENT_H__A413619C_1137_4FB0_A2C3_41408D07E9AA
