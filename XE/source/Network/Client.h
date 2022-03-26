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

#include "Type.h"

#include "Utils/Disposable.h"

BEG_XE_NAMESPACE

class XE_API Client : public XE::NonCopyable, public XE::EnableSharedFromThis< Client >
{
public:
	using HandshakeCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code ) >;
	using ConnectCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code ) >;
	using DisconnectCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code ) >;
	using SendCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code, XE::uint64 ) >;
	using ReceiveCallbackType = XE::Delegate< void( XE::ClientHandle, std::error_code, XE::MemoryView ) >;

public:
	Client( XE::ProtocolType type );

	virtual ~Client();

public:
	XE::ClientHandle GetHandle() const;

	XE::ProtocolType GetProtocol() const;

	XE::Endpoint GetLocalEndpoint() const;

	XE::Endpoint GetRemoteEndpoint() const;

public:
	const std::chrono::steady_clock::duration & GetReconnect() const;

	void SetReconnect( const std::chrono::steady_clock::duration & val );

public:
	void BindConnectCallback( const ConnectCallbackType & callback );

	void BindHandshakeCallback( const HandshakeCallbackType & callback );

	void BindDisconnectCallback( const DisconnectCallbackType & callback );

	void BindSendCallback( const SendCallbackType & callback );

	void BindReceiveCallback( const ReceiveCallbackType & callback );

public:
	void Send( XE::MemoryView data );

public:
	bool Close();

	void WaitClose();

protected:
	virtual void Read() = 0;

	virtual void Wirte() = 0;

	virtual void Clear() = 0;

	virtual void Connect() = 0;

protected:
	void AllocHandle();

	void * GetIOService();

protected:
	bool _Close = true;

	XE::ClientHandle _Handle;
	XE::ProtocolType _Protocol;

	XE::Endpoint _LocalEndpoint;
	XE::Endpoint _RemoteEndpoint;

	SendCallbackType _SendCB = nullptr;
	ReceiveCallbackType _ReceiveCB = nullptr;
	ConnectCallbackType _ConnectCB = nullptr;
	HandshakeCallbackType _HandshakeCB = nullptr;
	DisconnectCallbackType _DisconnectCB = nullptr;

	std::mutex _Mutex;
	XE::ConcurrentQueue< XE::Array< XE::int8 > > _Wirte;
	XE::ConcurrentQueue< XE::Array< XE::int8 > > _Buffers;

	XE::Disposable _ReconnectTimer;
	std::chrono::steady_clock::duration _Reconnect = std::chrono::steady_clock::duration::zero();
};

class XE_API TCPClient : public Client
{
private:
	struct Private;

public:
	TCPClient();

	~TCPClient() override;

public:
	void Start( const XE::Endpoint & endpoint, const XE::String & endl );

	void Start( const XE::Endpoint & endpoint, const std::regex & regex );

	void Start( const XE::Endpoint & endpoint, const XE::Match & match );

	void Start( const XE::Endpoint & endpoint, const XE::Transfer & trans );

	void Start( const XE::Endpoint & endpoint, const XE::DataGram & dgram );

private:
	void Read() override;

	void Wirte() override;

	void Clear() override;

	void Connect() override;

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
	void Start( XE::int16 bind, const XE::Endpoint & endpoint, XE::uint64 buf_size = 1400 );

private:
	void Read() override;

	void Wirte() override;

	void Clear() override;

	void Connect() override;

private:
	void Handshake();

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
	void Start( XE::int16 bind, const XE::Endpoint & endpoint, XE::uint64 buf_size = 1400 );

private:
	void Read() override;

	void Wirte() override;

	void Clear() override;

	void Connect() override;

private:
	void Handshake();

private:
	Private * _p;
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
		BindReceiveCallback( { &RPCClient<Protocol>::OnReceive, this } );
	}

	~RPCClient() = default;

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

			// load args
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

	template< typename F, typename ... Args > void RemoteInvoke( const XE::String & name, F && f, Args &&... args )
	{
		using traits = XE::FunctionTraits< F >;

		auto handle = _Allocator.Alloc();

		XE::int8 buf[sizeof( XE::uint64 )] = {};
		XE::WriteBigEndian( buf, handle.GetValue() );

		XE::MemoryStream out( XE::MemoryResource::GetFrameMemoryResource() );

		out << static_cast<XE::uint8>( XE::RPCProtocolFlag::INVOKE );
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
	void OnReceive( XE::ClientHandle handle, std::error_code code, XE::MemoryView view )
	{
		if( !code )
		{
			auto buf = view.data();

			XE::RPCProtocolFlag flag = *reinterpret_cast<const XE::RPCProtocolFlag *>( buf++ );
			
			switch( flag )
			{
			case XE::RPCProtocolFlag::INVOKE:
			{
				XE::uint64 handle;
				buf += XE::ReadBigEndian( buf, handle );

				XE::String name;
				while( *buf != 0 ) name.push_back( *buf++ );
				buf++;

				auto result_callback = [handle]( std::error_code code, XE::MemoryView view )
				{
					XE::MemoryStream out( XE::MemoryResource::GetFrameMemoryResource() );

					out << static_cast<XE::uint8>( XE::RPCProtocolFlag::RESULT ) << code.value();

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
			case XE::RPCProtocolFlag::RESULT:
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
	XE::Map< XE::String, FunctionCallbackType > _Callbacks;
	XE::ConcurrentHandleAllocator< XE::RPC > _Allocator;
	XE::ConcurrentHashMap< XE::RPCHandle, ResultCallbackType > _Results;
};

END_XE_NAMESPACE

#endif//CLIENT_H__A413619C_1137_4FB0_A2C3_41408D07E9AA
