#include "Client.h"

#include "Core/CoreFramework.h"
#include "Core/TimerService.h"

#if PLATFORM_OS & OS_WINDOWS
#include <sdkddkver.h>
#endif

#include <kcp/ikcp.h>
#include <asio/asio.hpp>

#include "NetworkService.h"

namespace asio
{
	template <> struct is_match_condition< XE::Match > : public std::true_type {};
	template <> struct is_match_condition< XE::Regex > : public std::true_type {};
	template <> struct is_match_condition< XE::DataGram > : public std::true_type {};
}

template< typename T > T ToEndpoint( const XE::Endpoint & endpoint )
{
	return { asio::ip::address::from_string( endpoint.Addr.c_str() ), static_cast<asio::ip::port_type>( std::atoi( endpoint.Port.c_str() ) ) };
}


XE::Client::Client( XE::ProtocolTypeFlags type )
	:_Protocol( type )
{

}

XE::Client::~Client()
{
	WaitClose();
}

XE::ClientHandle XE::Client::GetHandle() const
{
	return _Handle;
}

XE::ProtocolTypeFlags XE::Client::GetProtocol() const
{
	return _Protocol;
}

XE::Endpoint XE::Client::GetLocalEndpoint() const
{
	return _LocalEndpoint;
}

XE::Endpoint XE::Client::GetRemoteEndpoint() const
{
	return _RemoteEndpoint;
}

const std::chrono::steady_clock::duration & XE::Client::GetReconnect() const
{
	return _Reconnect;
}

void XE::Client::SetReconnect( const std::chrono::steady_clock::duration & val )
{
	_Reconnect = val;
}

void XE::Client::BindConnectCallback( const ConnectCallbackType & callback )
{
	_ConnectCB = callback;
}

void XE::Client::BindHandshakeCallback( const HandshakeCallbackType & callback )
{
	_HandshakeCB = callback;
}

void XE::Client::BindDisconnectCallback( const DisconnectCallbackType & callback )
{
	_DisconnectCB = [this, callback]( XE::ClientHandle handle, std::error_code code )
	{
		if( _Reconnect != std::chrono::steady_clock::duration::zero() )
		{
			_ReconnectTimer = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->
				StartUnscaleTimer( _Reconnect, [this]()
			{
				Connect();
				_ReconnectTimer.Dispose();
				return false;
			} );
		}
	};
}

void XE::Client::BindSendCallback( const SendCallbackType & callback )
{
	_SendCB = callback;
}

void XE::Client::BindReceiveCallback( const ReceiveCallbackType & callback )
{
	_ReceiveCB = callback;
}

void XE::Client::Send( XE::MemoryView data )
{
	DataGram::SizeType sz = static_cast<DataGram::SizeType>( data.size() );

	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	_Buffers.try_pop( buffer ); buffer.resize( sizeof( DataGram::SizeType ) + data.size() );

	XE::WriteBigEndian( buffer.data(), sz );

	::memcpy( buffer.data() + sizeof( DataGram::SizeType ), data.data(), data.size() );

	if( _Mutex.try_lock() )
	{
		_Wirte.push( std::move( buffer ) );

		Wirte();
	}
}

bool XE::Client::Close()
{
	if( _Close == false )
	{
		_Close = true;

		if( _Mutex.try_lock() )
		{
			Clear();

			_Mutex.unlock();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void XE::Client::WaitClose()
{
	if( _Close == false )
	{
		_Close = true;
	}

	std::unique_lock< std::mutex > lock( _Mutex );

	Clear();
}

void XE::Client::AllocHandle()
{
	_Handle = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >()->AllocClientHandle();
}

void * XE::Client::GetIOService()
{
	return XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >()->GetIOService();
}

struct XE::TCPClient::Private
{
	XE::uint64 _Size = 0;
	XE::String _Endl = {};
	std::regex _Regex = {};
	XE::Match _Match = {};
	XE::ReadType _Type = XE::ReadType::ENDL;

	asio::streambuf _Read;
	XE::UniquePtr < asio::ip::tcp::socket > _Socket;
};

XE::TCPClient::TCPClient()
	:Client( XE::ProtocolType::TCP ), _p( XE::New< Private >() )
{

}

XE::TCPClient::~TCPClient()
{
	XE::Delete( _p );
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::String & endl )
{
	_p->_Type = XE::ReadType::ENDL;

	_RemoteEndpoint = endpoint;
	_p->_Endl = endl;
	_Close = false;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const std::regex & regex )
{
	_p->_Type = XE::ReadType::REGEX;

	_RemoteEndpoint = endpoint;
	_p->_Regex = regex;
	_Close = false;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::Match & match )
{
	_p->_Type = XE::ReadType::MATCH;

	_RemoteEndpoint = endpoint;
	_p->_Match = match;
	_Close = false;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::Transfer & trans )
{
	_p->_Type = XE::ReadType::TRANSFER;

	_RemoteEndpoint = endpoint;
	_p->_Size = trans.size;
	_Close = false;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::DataGram & dgram )
{
	_p->_Type = XE::ReadType::DATAGRAM;

	_RemoteEndpoint = endpoint;
	_Close = false;

	Connect();
}

void XE::TCPClient::Read()
{
	switch( _p->_Type )
	{
	case XE::ReadType::ENDL:
	{
		asio::async_read_until( *_p->_Socket.get(), _p->_Read, std::string_view( _p->_Endl.c_str(), _p->_Endl.size() ), [this]( std::error_code code, std::size_t bytes )
		{
			if( !code )
			{
				std::istream is( &_p->_Read );

				XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
				_Buffers.try_pop( buffer ); buffer.resize( bytes );

				is.read( buffer.data(), bytes );

				if( _ReceiveCB )
				{
					XE::MemoryView view( buffer.data(), buffer.size() );

					_ReceiveCB( _Handle, code, view );
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );

				Read();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		} );
	}
	break;
	case XE::ReadType::REGEX:
	{
		asio::async_read_until( *_p->_Socket.get(), _p->_Read, Regex( _p->_Regex ), [this]( std::error_code code, std::size_t bytes )
		{
			if( !code )
			{
				std::istream is( &_p->_Read );

				XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
				_Buffers.try_pop( buffer ); buffer.resize( bytes );

				is.read( buffer.data(), bytes );

				if( _ReceiveCB )
				{
					XE::MemoryView view( buffer.data(), buffer.size() );

					_ReceiveCB( _Handle, code, view );
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );

				Read();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		} );
	}
	break;
	case XE::ReadType::MATCH:
	{
		asio::async_read_until( *_p->_Socket.get(), _p->_Read, Match( _p->_Match ), [this]( std::error_code code, std::size_t bytes )
		{
			if( !code )
			{
				std::istream is( &_p->_Read );

				XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
				_Buffers.try_pop( buffer ); buffer.resize( bytes );

				is.read( buffer.data(), bytes );

				if( _ReceiveCB )
				{
					XE::MemoryView view( buffer.data(), buffer.size() );

					_ReceiveCB( _Handle, code, view );
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );

				Read();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		}, 0 );
	}
	break;
	case XE::ReadType::TRANSFER:
	{
		asio::async_read( *_p->_Socket.get(), _p->_Read, asio::transfer_exactly( _p->_Size ), [this]( std::error_code code, std::size_t bytes )
		{
			if( !code )
			{
				std::istream is( &_p->_Read );

				XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
				_Buffers.try_pop( buffer ); buffer.resize( bytes );

				is.read( buffer.data(), bytes );

				if( _ReceiveCB )
				{
					XE::MemoryView view( buffer.data(), buffer.size() );

					_ReceiveCB( _Handle, code, view );
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );

				Read();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		} );
	}
	break;
	case XE::ReadType::DATAGRAM:
	{
		asio::async_read_until( *_p->_Socket.get(), _p->_Read, DataGram(), [this]( std::error_code code, std::size_t bytes )
		{
			if( !code )
			{
				std::istream is( &_p->_Read );

				XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
				_Buffers.try_pop( buffer ); buffer.resize( bytes );

				is.read( buffer.data(), bytes );

				if( _ReceiveCB )
				{
					XE::MemoryView view( buffer.data(), buffer.size() );

					_ReceiveCB( _Handle, code, view );
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );

				Read();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		} );
	}
	break;
	default:
		break;
	}
}

void XE::TCPClient::Wirte()
{
	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	if( _Wirte.try_pop( buffer ) )
	{
		XE::MemoryView view;

		switch( _p->_Type )
		{
		case XE::ReadType::ENDL:
			view = { buffer.data() + sizeof( DataGram::SizeType ), buffer.size() - sizeof( DataGram::SizeType ) };
			break;
		case XE::ReadType::REGEX:
			view = { buffer.data() + sizeof( DataGram::SizeType ), buffer.size() - sizeof( DataGram::SizeType ) };
			break;
		case XE::ReadType::TRANSFER:
			view = { buffer.data() + sizeof( DataGram::SizeType ), buffer.size() - sizeof( DataGram::SizeType ) };
			break;
		case XE::ReadType::DATAGRAM:
			view = { buffer.data(), buffer.size() };
			break;
		default:
			break;
		}

		asio::async_write( *_p->_Socket.get(), asio::buffer( view.data(), view.size() ), asio::transfer_exactly( view.size() ),
						   [this, buf = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			if( !code )
			{
				if( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}

				Wirte();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}

			buf.clear();
			_Buffers.push( std::move( buf ) );
		} );
	}
	else
	{
		_Mutex.unlock();
	}
}

void XE::TCPClient::Clear()
{
	if( _p->_Socket != nullptr )
	{
		_p->_Socket->close();
		_p->_Socket = nullptr;
	}
}

void XE::TCPClient::Connect()
{
	if( auto network = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >() )
	{
		AllocHandle();

		_p->_Socket = XE::MakeUnique< asio::ip::tcp::socket >( *reinterpret_cast<asio::io_service *>( GetIOService() ) );

		auto resolver = XE::MakeShared< asio::ip::tcp::resolver >( *reinterpret_cast<asio::io_service *>( GetIOService() ) );

		resolver->async_resolve( _RemoteEndpoint.Addr.std_str(), _RemoteEndpoint.Port.std_str(), [this, resolver]( std::error_code code, asio::ip::tcp::resolver::results_type endpoints )
		{
			asio::async_connect( *_p->_Socket.get(), endpoints, [this]( std::error_code code, asio::ip::tcp::endpoint endpoint )
			{
				if( !code )
				{
					auto local = _p->_Socket->local_endpoint();
					auto remote = _p->_Socket->remote_endpoint();

					_LocalEndpoint.Addr = local.address().to_string();
					_LocalEndpoint.Port = XE::ToString( local.port() );

					_RemoteEndpoint.Addr = remote.address().to_string();
					_RemoteEndpoint.Port = XE::ToString( remote.port() );
				}

				if( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
				if( _HandshakeCB )
				{
					_HandshakeCB( _Handle, code );
				}

				if( !code )
				{
					Read();
				}
			} );
		} );
	}
}

XE::uint64 XE::TCPClient::NativeHandle()
{
	return ( XE::uint64 ) _p->_Socket->native_handle();
}

struct XE::UDPClient::Private
{
	XE::int16 _Bind;
	XE::uint64 _BufferSize;
	XE::SessionHandle _Session;
	XE::UniquePtr < asio::ip::udp::socket > _Socket;
};

XE::UDPClient::UDPClient()
	:Client( XE::ProtocolType::UDP ), _p( XE::New< Private >() )
{

}

XE::UDPClient::~UDPClient()
{
	XE::Delete( _p );
}

void XE::UDPClient::Start( XE::int16 bind, const XE::Endpoint & endpoint, XE::uint64 buf_size /*= 1400 */ )
{
	_RemoteEndpoint = endpoint;
	_p->_BufferSize = buf_size + sizeof( XE::UDPProtocolFlag ) + sizeof( XE::SessionHandle );
	_p->_Bind = bind;
	_Close = false;

	Connect();
}

void XE::UDPClient::Read()
{
	XE::Array< XE::int8 > buffer;
	_Buffers.try_pop( buffer ); buffer.resize( _p->_BufferSize );
	_p->_Socket->async_receive( asio::buffer( buffer ), [this, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
	{
		if( !code )
		{
			auto buf = buffer.data();
			auto flag = *reinterpret_cast<XE::UDPProtocolFlag *>( buf++ );

			switch( flag )
			{
			case XE::UDPProtocolFlag::START:
			{
				XE::uint64 handle = XE::SessionHandle::Invalid.GetValue();

				XE::ReadBigEndian( buf, handle );

				_p->_Session = XE::HandleCast< XE::Session >( handle );

				if( _HandshakeCB )
				{
					_HandshakeCB( _Handle, code );
				}
			}
			break;
			case XE::UDPProtocolFlag::DATAGRAM:
			{
				if( _ReceiveCB )
				{
					_ReceiveCB( _Handle, code, { buf, bytes - sizeof( XE::UDPProtocolFlag ) } );
				}
			}
			break;
			case XE::UDPProtocolFlag::STOP:
			{
				Close();
			}
			break;
			default:
				break;
			}

			buffer.clear();
			_Buffers.push( std::move( buffer ) );

			Read();
		}
		else if( _DisconnectCB )
		{
			_DisconnectCB( _Handle, code );
		}
	} );
}

void XE::UDPClient::Wirte()
{
	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	if( _Wirte.try_pop( buffer ) )
	{
		XE::WriteBigEndian( buffer.data(), _p->_Session.GetValue() ); // replace DataGram::SizeType memory
		buffer.insert( buffer.begin(), static_cast<XE::int8>( XE::UDPProtocolFlag::DATAGRAM ) );

		XE::MemoryView view = { buffer.data(), buffer.size() };

		_p->_Socket.get()->async_send( asio::buffer( view.data(), view.size() ), [this, buf = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			if( !code )
			{
				if( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}

				Wirte();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}

			buf.clear();
			_Buffers.push( std::move( buf ) );
		} );
	}
	else
	{
		_Mutex.unlock();
	}
}

void XE::UDPClient::Clear()
{
	if( _p->_Socket != nullptr )
	{
		XE::Array< XE::int8 > buffer;
		_Buffers.try_pop( buffer ); buffer.resize( sizeof( XE::UDPProtocolFlag ) + sizeof( XE::SessionHandle ) );

		buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::STOP );
		XE::WriteBigEndian( buffer.data() + sizeof( XE::UDPProtocolFlag ), _p->_Session.GetValue() );

		_p->_Socket.get()->async_send( asio::buffer( buffer ), [this, buffer = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			_p->_Socket->close();
			_p->_Socket = nullptr;
		} );
	}
}

void XE::UDPClient::Connect()
{
	if( auto network = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::Service >() )
	{
		AllocHandle();

		asio::ip::udp::endpoint local_endpoint( asio::ip::udp::v4(), _p->_Bind );
		_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( GetIOService() ), local_endpoint );

		auto resolver = XE::MakeShared< asio::ip::udp::resolver >( *reinterpret_cast<asio::io_service *>( GetIOService() ) );

		resolver->async_resolve( _RemoteEndpoint.Addr.std_str(), _RemoteEndpoint.Port.std_str(), [this, resolver]( std::error_code code, asio::ip::udp::resolver::results_type endpoints )
		{
			_p->_Socket->async_connect( endpoints->endpoint(), [this]( auto code )
			{
				if( !code )
				{
					auto local = _p->_Socket->local_endpoint();
					auto remote = _p->_Socket->remote_endpoint();

					_LocalEndpoint.Addr = local.address().to_string();
					_LocalEndpoint.Port = XE::ToString( local.port() );

					_RemoteEndpoint.Addr = remote.address().to_string();
					_RemoteEndpoint.Port = XE::ToString( remote.port() );
				}

				if( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}

				if( !code )
				{
					Handshake();
				}
			} );
		} );
	}
}

void XE::UDPClient::Handshake()
{
	XE::Array< XE::int8 > buffer;
	_Buffers.try_pop( buffer ); buffer.resize( 1 );

	buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::START );

	_p->_Socket.get()->async_send( asio::buffer( buffer ), [this, buffer = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
	{
		if( code )
		{
			if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		}
	} );

	Read();
}

XE::uint64 XE::UDPClient::NativeHandle()
{
	return ( XE::uint64 ) _p->_Socket->native_handle();
}


struct XE::KCPClient::Private
{
	XE::int16 _Bind;
	XE::uint64 _BufferSize;
	ikcpcb * _KCP = nullptr;
	XE::Disposable _KeepliveTimer;
	XE::SessionHandle _Session;
	XE::UniquePtr < asio::ip::udp::socket > _Socket;
};

XE::KCPClient::KCPClient()
	:Client( XE::ProtocolType::KCP ), _p( XE::New< Private >() )
{

}

XE::KCPClient::~KCPClient()
{
	XE::Delete( _p );
}

void XE::KCPClient::Start( XE::int16 bind, const XE::Endpoint & endpoint, XE::uint64 buf_size /*= 1400 */ )
{
	_RemoteEndpoint = endpoint;
	_p->_BufferSize = buf_size + sizeof( XE::UDPProtocolFlag ) + sizeof( XE::SessionHandle );
	_p->_Bind = bind;
	_Close = false;

	Connect();
}

void XE::KCPClient::Read()
{
	XE::Array< XE::int8 > buffer;
	_Buffers.try_pop( buffer ); buffer.resize( _p->_BufferSize );
	_p->_Socket->async_receive( asio::buffer( buffer ), [this, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
	{
		if( !code )
		{
			auto buf = buffer.data();
			auto flag = *reinterpret_cast<XE::UDPProtocolFlag *>( buf++ );

			switch( flag )
			{
			case XE::UDPProtocolFlag::START:
			{
				XE::uint64 handle = XE::SessionHandle::Invalid.GetValue();

				XE::ReadBigEndian( buf, handle );

				_p->_Session = XE::HandleCast< XE::Session >( handle );

				_p->_KCP = ikcp_create( static_cast<XE::uint32>( handle ), this );

				ikcp_setmtu( _p->_KCP, ( (int)_p->_BufferSize ) - 1 );
				ikcp_wndsize( _p->_KCP, 128, 128 );
				ikcp_nodelay( _p->_KCP, 1, 10, 2, 1 );
				ikcp_setoutput( _p->_KCP, []( const char * buf, int len, ikcpcb * kcp, void * user ) -> int
				{
					auto client = static_cast<XE::KCPClient *>( user );

					XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
					client->_Buffers.try_pop( buffer ); buffer.resize( len + sizeof( XE::UDPProtocolFlag ) );

					buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::DATAGRAM );
					::memcpy( buffer.data() + sizeof( XE::UDPProtocolFlag ), buf, len );

					auto endpoint = ToEndpoint< asio::ip::udp::endpoint >( client->_RemoteEndpoint );

					client->_p->_Socket->async_send_to( asio::buffer( buffer ), endpoint, [client, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
					{
						if( !code )
						{
							if( client->_SendCB )
							{
								client->_SendCB( client->_Handle, code, bytes );
							}

							buffer.clear();
							client->_Buffers.push( std::move( buffer ) );
							client->Wirte();
						}
						else if( client->_DisconnectCB )
						{
							client->_DisconnectCB( client->_Handle, code );
						}
					} );

					return 0;
				} );

				_p->_KeepliveTimer = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->StartUnscaleTimer( std::chrono::milliseconds( 10 ), [this]()
				{
					XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
					_Buffers.try_pop( buffer ); buffer.resize( sizeof( XE::UDPProtocolFlag ) );

					buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::KEEPLIVE );

					auto endpoint = ToEndpoint< asio::ip::udp::endpoint >( _RemoteEndpoint );

					_p->_Socket->async_send_to( asio::buffer( buffer ), endpoint, [this, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
					{
						buffer.clear();
						_Buffers.push( std::move( buffer ) );
					} );

					return false;
				} );

				if( _HandshakeCB )
				{
					_HandshakeCB( _Handle, code );
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			case XE::UDPProtocolFlag::DATAGRAM:
			{
				if( ikcp_input( _p->_KCP, buf, buffer.size() - ( buf - buffer.data() ) ) != 0 )
				{
					if( _DisconnectCB )
					{
						_DisconnectCB( _Handle, std::make_error_code( std::errc::message_size ) );
					}
				}
				else
				{
					buffer.resize( _p->_BufferSize );

					for( ;;)
					{
						int len = ikcp_recv( _p->_KCP, buffer.data(), buffer.size() );
						if( len >= 0 )
						{
							if( _ReceiveCB )
							{
								_ReceiveCB( _Handle, code, { buffer.data(), (XE::uint64)len } );
							}

							break;
						}
						else if( len == -3 )
						{
							buffer.resize( buffer.size() * 2 );
						}
						else
						{
							break;
						}
					}
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			case XE::UDPProtocolFlag::STOP:
			{
				Close();

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			default:
				buffer.clear();
				_Buffers.push( std::move( buffer ) );
				break;
			}

			Read();
		}
		else if( _DisconnectCB )
		{
			_DisconnectCB( _Handle, code );
		}
	} );
}

void XE::KCPClient::Wirte()
{
	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	if( _Wirte.try_pop( buffer ) )
	{
		XE::WriteBigEndian( buffer.data(), _p->_Session.GetValue() ); // replace DataGram::SizeType memory
		buffer.insert( buffer.begin(), static_cast<XE::int8>( XE::UDPProtocolFlag::DATAGRAM ) );

		XE::MemoryView view = { buffer.data(), buffer.size() };

		_p->_Socket.get()->async_send( asio::buffer( view.data(), view.size() ), [this, buf = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			if( !code )
			{
				if( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}

				Wirte();
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}

			buf.clear();
			_Buffers.push( std::move( buf ) );
		} );
	}
	else
	{
		_Mutex.unlock();
	}
}

void XE::KCPClient::Clear()
{
	if( _p->_Socket != nullptr )
	{
		_p->_KeepliveTimer.Dispose();

		XE::Array< XE::int8 > buffer;
		_Buffers.try_pop( buffer ); buffer.resize( sizeof( XE::UDPProtocolFlag ) + sizeof( XE::SessionHandle ) );

		buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::STOP );
		XE::WriteBigEndian( buffer.data() + sizeof( XE::UDPProtocolFlag ), _p->_Session.GetValue() );

		_p->_Socket.get()->async_send( asio::buffer( buffer ), [this, buffer = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			_p->_Socket->close();
			ikcp_release( _p->_KCP );

			_p->_KCP = nullptr;
			_p->_Socket = nullptr;
		} );
	}
}

void XE::KCPClient::Connect()
{
	if( auto network = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::Service >() )
	{
		AllocHandle();

		asio::ip::udp::endpoint local_endpoint( asio::ip::udp::v4(), _p->_Bind );
		_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( GetIOService() ), local_endpoint );

		auto resolver = XE::MakeShared< asio::ip::udp::resolver >( *reinterpret_cast<asio::io_service *>( GetIOService() ) );

		resolver->async_resolve( _RemoteEndpoint.Addr.std_str(), _RemoteEndpoint.Port.std_str(), [this, resolver]( std::error_code code, asio::ip::udp::resolver::results_type endpoints )
		{
			_p->_Socket->async_connect( endpoints->endpoint(), [this]( auto code )
			{
				if( !code )
				{
					auto local = _p->_Socket->local_endpoint();
					auto remote = _p->_Socket->remote_endpoint();

					_LocalEndpoint.Addr = local.address().to_string();
					_LocalEndpoint.Port = XE::ToString( local.port() );

					_RemoteEndpoint.Addr = remote.address().to_string();
					_RemoteEndpoint.Port = XE::ToString( remote.port() );
				}

				if( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}

				if( !code )
				{
					Handshake();
				}
			} );
		} );
	}
}

void XE::KCPClient::Handshake()
{
	XE::Array< XE::int8 > buffer;
	_Buffers.try_pop( buffer ); buffer.resize( 1 );

	buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::START );

	_p->_Socket.get()->async_send( asio::buffer( buffer ), [this, buffer = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
	{
		if( code )
		{
			if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, code );
			}
		}
	} );

	Read();
}

XE::uint64 XE::KCPClient::NativeHandle()
{
	return ( XE::uint64 ) _p->_Socket->native_handle();
}
