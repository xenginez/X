#include "Client.h"

#include "Core/TimerService.h"
#include "Core/CoreFramework.h"

#if PLATFORM_OS & OS_WINDOWS
#include <sdkddkver.h>
#endif

#include <kcp/ikcp.h>
#include <asio/asio.hpp>

#include "NetworkService.h"

namespace asio
{
	template <> struct is_match_condition< XE::EndlCondition > : public std::true_type {};
	template <> struct is_match_condition< XE::RegexCondition > : public std::true_type {};
	template <> struct is_match_condition< XE::MatchCondition > : public std::true_type {};
	template <> struct is_match_condition< XE::TransferCondition > : public std::true_type {};
	template <> struct is_match_condition< XE::DataGramCondition > : public std::true_type {};
}

template< typename T > T ToEndpoint( const XE::Endpoint & endpoint )
{
	return { asio::ip::address::from_string( endpoint.Addr.c_str() ), static_cast<asio::ip::port_type>( std::atoi( endpoint.Port.c_str() ) ) };
}

XE::Client::Client()
{
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >()->RegisterClient( this );
}

XE::Client::~Client()
{

}

XE::ClientHandle XE::Client::GetHandle() const
{
	return _Handle;
}

XE::Endpoint XE::Client::GetLocalEndpoint() const
{
	return _LocalEndpoint;
}

XE::Endpoint XE::Client::GetRemoteEndpoint() const
{
	return _RemoteEndpoint;
}

void XE::Client::BindConnectCallback( const ConnectCallbackType & callback )
{
	_ConnectCB = callback;
}

void XE::Client::BindHandshakeCallback( const HandshakeCallbackType & callback )
{
	_HandshakeCB = callback;
}

void XE::Client::BindSendCallback( const SendCallbackType & callback )
{
	_SendCB = callback;
}

void XE::Client::BindReceiveCallback( const ReceiveCallbackType & callback )
{
	_ReceiveCB = callback;
}


struct XE::TCPClient::Private
{
	std::mutex _Mutex;
	asio::streambuf _Read;
	asio::streambuf _Write;
	XE::UniquePtr < asio::ip::tcp::socket > _Socket;
	std::variant<XE::EndlCondition, XE::RegexCondition, XE::MatchCondition, XE::TransferCondition, XE::DataGramCondition> _Condition;
};

XE::TCPClient::TCPClient()
	:_p( XE::New< Private >() )
{

}

XE::TCPClient::~TCPClient()
{
	Close();

	XE::Delete( _p );
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::EndlCondition & endl )
{
	_p->_Condition = endl;
	_RemoteEndpoint = endpoint;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::RegexCondition & regex )
{
	_p->_Condition = regex;
	_RemoteEndpoint = endpoint;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::MatchCondition & match )
{
	_p->_Condition = match;
	_RemoteEndpoint = endpoint;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::TransferCondition & trans )
{
	_p->_Condition = trans;
	_RemoteEndpoint = endpoint;

	Connect();
}

void XE::TCPClient::Start( const XE::Endpoint & endpoint, const XE::DataGramCondition & dgram )
{
	_p->_Condition = dgram;
	_RemoteEndpoint = endpoint;

	Connect();
}

void XE::TCPClient::Send( XE::MemoryView view )
{
	std::visit( XE::Overloaded{
				[&]( const XE::EndlCondition & val )
				{
					std::unique_lock<std::mutex> lock( _p->_Mutex );

					std::ostream os( &_p->_Write );
					os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
					os.write( val.endl.c_str(), val.endl.size() );
				},
				[&]( const XE::RegexCondition & val )
				{
					std::ostream os( &_p->_Write );
					os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
				},
				[&]( const XE::MatchCondition & val )
				{
					std::ostream os( &_p->_Write );
					os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
				},
				[&]( const XE::TransferCondition & val )
				{
					std::ostream os( &_p->_Write );
					os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
				},
				[&]( const XE::DataGramCondition & val )
				{
					std::unique_lock<std::mutex> lock( _p->_Mutex );

					char buf[sizeof( XE::uint64 )];
					XE::WriteBigEndian( buf, view.size() );

					std::ostream os( &_p->_Write );
					os.write( buf, sizeof( XE::uint64 ) );
					os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
				} }, _p->_Condition );

	Write();
}

void XE::TCPClient::Read()
{
	auto cb = [this]( std::error_code code, std::size_t bytes )
	{
		if ( !code )
		{
			XE::Array<char> buf( XE::MemoryResource::GetFrameMemoryResource() ); buf.resize( bytes );
			std::istream is( &_p->_Read );
			is.read( buf.data(), bytes );
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, code, { buf.data(), bytes } );
			}

			Read();
		}
		else
		{
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, code, {} );
			}
		}
	};

	std::visit( XE::Overloaded{ [&]( auto & val ) { asio::async_read_until( *_p->_Socket.get(), _p->_Read, val, cb ); } }, _p->_Condition );
}

void XE::TCPClient::Write()
{
	if ( _p->_Write.size() > 0 )
	{
		asio::async_write( *_p->_Socket.get(), asio::buffer( _p->_Write.data(), _p->_Write.size() ), [this]( std::error_code code, size_t bytes ) mutable
		{
			_p->_Write.consume( bytes );

			if ( !code )
			{
				if ( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}

				Write();
			}
			else
			{
				if ( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}
			}
		} );
	}
}

void XE::TCPClient::Close()
{
	if ( _p->_Socket != nullptr )
	{
		_p->_Socket->close();
		_p->_Socket = nullptr;
	}
}

void XE::TCPClient::Connect()
{
	_p->_Socket = XE::MakeUnique< asio::ip::tcp::socket >( *reinterpret_cast<asio::io_service *>( _IOService ) );

	auto resolver = XE::MakeShared< asio::ip::tcp::resolver >( *reinterpret_cast<asio::io_service *>( _IOService ) );

	resolver->async_resolve( _RemoteEndpoint.Addr.std_str(), _RemoteEndpoint.Port.std_str(), [this, resolver]( std::error_code code, asio::ip::tcp::resolver::results_type endpoints )
	{
		asio::async_connect( *_p->_Socket.get(), endpoints, [this]( std::error_code code, asio::ip::tcp::endpoint endpoint )
		{
			if ( !code )
			{
				auto local = _p->_Socket->local_endpoint();
				auto remote = _p->_Socket->remote_endpoint();

				_LocalEndpoint.Addr = local.address().to_string();
				_LocalEndpoint.Port = XE::ToString( local.port() );

				_RemoteEndpoint.Addr = remote.address().to_string();
				_RemoteEndpoint.Port = XE::ToString( remote.port() );

				if ( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
				if ( _HandshakeCB )
				{
					_HandshakeCB( _Handle, code );
				}

				Read();
			}
			else
			{
				if ( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
			}
		} );
	} );
}


struct XE::UDPClient::Private
{
	XE::uint64 _BufSize;
	asio::streambuf _Read;
	asio::streambuf _Write;
	XE::UniquePtr < asio::ip::udp::socket > _Socket;
};

XE::UDPClient::UDPClient()
	:_p( XE::New< Private >() )
{

}

XE::UDPClient::~UDPClient()
{
	Close();

	XE::Delete( _p );
}

void XE::UDPClient::Start( const XE::Endpoint & endpoint, XE::uint64 buf_size /*= 1400*/ )
{
	_p->_BufSize = buf_size;
	_RemoteEndpoint = endpoint;

	Connect();
}

void XE::UDPClient::Send( XE::MemoryView view )
{
	std::ostream os( &_p->_Write );
	os.write( reinterpret_cast<const char *>( view.data() ), view.size() );

	Write();
}

void XE::UDPClient::Read()
{
	_p->_Socket->async_receive( _p->_Read.prepare( _p->_BufSize ), [this]( std::error_code code, std::size_t bytes )
	{
		_p->_Read.consume( bytes );

		if ( !code )
		{
			XE::Array<char> buf( XE::MemoryResource::GetFrameMemoryResource() ); buf.resize( bytes );
			std::istream is( &_p->_Read );
			is.read( buf.data(), bytes );
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, code, { buf.data(), bytes } );
			}

			Read();
		}
		else
		{
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, code, {} );
			}
		}
	} );
}

void XE::UDPClient::Write()
{
	if ( _p->_Write.size() > 0 )
	{
		_p->_Socket.get()->async_send( asio::buffer( _p->_Write.data(), _p->_Write.size() ), [this]( std::error_code code, size_t bytes )
		{
			_p->_Write.consume( bytes );

			if ( !code )
			{
				if ( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}

				Write();
			}
			else
			{
				if ( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}
			}
		} );
	}
}

void XE::UDPClient::Close()
{
	if ( _p->_Socket != nullptr )
	{
		_p->_Socket->close();
		_p->_Socket = nullptr;
	}
}

void XE::UDPClient::Connect()
{
	asio::ip::udp::endpoint local_endpoint( asio::ip::udp::v4(), 0 );
	_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( _IOService ), local_endpoint );

	auto resolver = XE::MakeShared< asio::ip::udp::resolver >( *reinterpret_cast<asio::io_service *>( _IOService ) );

	resolver->async_resolve( _RemoteEndpoint.Addr.std_str(), _RemoteEndpoint.Port.std_str(), [this, resolver]( std::error_code code, asio::ip::udp::resolver::results_type endpoints )
	{
		_p->_Socket->async_connect( endpoints->endpoint(), [this]( auto code )
		{
			if ( !code )
			{
				auto local = _p->_Socket->local_endpoint();
				auto remote = _p->_Socket->remote_endpoint();

				_LocalEndpoint.Addr = local.address().to_string();
				_LocalEndpoint.Port = XE::ToString( local.port() );

				_RemoteEndpoint.Addr = remote.address().to_string();
				_RemoteEndpoint.Port = XE::ToString( remote.port() );

				if ( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
				if ( _HandshakeCB )
				{
					_HandshakeCB( _Handle, code );
				}
			}
			else
			{
				if ( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
			}
		} );
	} );
}


struct XE::KCPClient::Private
{
	asio::streambuf _Read;
	asio::streambuf _Write;
	ikcpcb * _KCP = nullptr;
	XE::Disposable _KCPUpdateTimer;
	XE::UniquePtr < asio::ip::udp::socket > _Socket;
};

XE::KCPClient::KCPClient()
	:_p( XE::New< Private >() )
{

}

XE::KCPClient::~KCPClient()
{
	Close();

	XE::Delete( _p );
}

void XE::KCPClient::Start( const XE::Endpoint & endpoint, XE::uint64 buf_size /*= 1400 */ )
{
	_RemoteEndpoint = endpoint;

	_p->_KCP = ikcp_create( static_cast<XE::uint32>( _Handle.GetValue() ), this );
	{
		ikcp_setmtu( _p->_KCP, (int)buf_size - 1 );
		ikcp_wndsize( _p->_KCP, 128, 128 );
		ikcp_nodelay( _p->_KCP, 1, 10, 2, 1 );
		ikcp_setoutput( _p->_KCP, []( const char * buf, int len, ikcpcb * kcp, void * user ) -> int
		{
			auto client = static_cast<XE::KCPClient *>( user );
			std::ostream os( &client->_p->_Write );
			os.write( buf, len );

			return 0;
		} );
	}

	Connect();
}

void XE::KCPClient::Send( XE::MemoryView view )
{
	ikcp_send( _p->_KCP, view.data(), view.size() );

	Write();
}

void XE::KCPClient::Read()
{
	
	_p->_Socket->async_receive( _p->_Read.prepare( _p->_KCP->mtu ), [this]( std::error_code code, std::size_t bytes )
	{
		_p->_Read.consume( bytes );

		if ( !code )
		{
			XE::Array<char> buf( XE::MemoryResource::GetFrameMemoryResource() ); buf.resize( bytes );
			std::istream is( &_p->_Read );
			is.read( buf.data(), bytes );
			if ( ikcp_input( _p->_KCP, buf.data(), bytes ) == 0 )
			{
				for ( ;;)
				{
					char buf[1024];
					int len = ikcp_recv( _p->_KCP, buf, 1024 );
					if ( len >= 0 )
					{
						if ( _ReceiveCB )
						{
							_ReceiveCB( _Handle, code, { buf, (XE::uint64)len } );
						}
					}
					else
					{
						break;
					}
				}

				Read();
			}
		}
		else
		{
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, code, {} );
			}
		}
	} );
}

void XE::KCPClient::Write()
{
	if ( _p->_Write.size() > 0 )
	{
		_p->_Socket.get()->async_send( asio::buffer( _p->_Write.data(), _p->_Write.size() ), [this]( std::error_code code, size_t bytes )
		{
			_p->_Write.consume( bytes );

			if ( !code )
			{
				if ( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}

				Write();
			}
			else
			{
				if ( _SendCB )
				{
					_SendCB( _Handle, code, bytes );
				}
			}
		} );
	}
}

void XE::KCPClient::Close()
{
	if ( _p->_Socket != nullptr )
	{
		_p->_Socket->close();
		_p->_Socket = nullptr;

		_p->_KCPUpdateTimer.Dispose();

		ikcp_release( _p->_KCP );
		_p->_KCP = nullptr;
	}
}

void XE::KCPClient::Connect()
{
	asio::ip::udp::endpoint local_endpoint( asio::ip::udp::v4(), 0 );
	_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( _IOService ), local_endpoint );

	auto resolver = XE::MakeShared< asio::ip::udp::resolver >( *reinterpret_cast<asio::io_service *>( _IOService ) );

	resolver->async_resolve( _RemoteEndpoint.Addr.std_str(), _RemoteEndpoint.Port.std_str(), [this, resolver]( std::error_code code, asio::ip::udp::resolver::results_type endpoints )
	{
		_p->_Socket->async_connect( endpoints->endpoint(), [this]( auto code )
		{
			if ( !code )
			{
				auto local = _p->_Socket->local_endpoint();
				auto remote = _p->_Socket->remote_endpoint();

				_LocalEndpoint.Addr = local.address().to_string();
				_LocalEndpoint.Port = XE::ToString( local.port() );

				_RemoteEndpoint.Addr = remote.address().to_string();
				_RemoteEndpoint.Port = XE::ToString( remote.port() );

				if ( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
				if ( _HandshakeCB )
				{
					_HandshakeCB( _Handle, code );
				}

				if ( auto timer = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >() )
				{
					_p->_KCPUpdateTimer = timer->StartUnscaleTimer( std::chrono::milliseconds( 10 ), [this]() { ikcp_update( _p->_KCP, 10 ); return true; } );
				}

				Read();
			}
			else
			{
				if ( _ConnectCB )
				{
					_ConnectCB( _Handle, code );
				}
			}
		} );
	} );
}
