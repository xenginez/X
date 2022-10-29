#include "Server.h"

#include "Core/CoreFramework.h"
#include "Core/TimerService.h"

#if PLATFORM_OS & OS_WINDOWS
#include <sdkddkver.h>
#endif

#include <kcp/ikcp.h>
#include <asio/asio.hpp>

#include "NetworkService.h"

static constexpr XE::uint64 udp_timeout = 10;

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

namespace XE
{
	class Session
	{
	public:
		XE::SessionHandle _Handle;
		XE::Endpoint _RemoteEndpoint;
	};

	class TCPSession : public Session
	{
	public:
		std::mutex _Mutex;
		asio::streambuf _Read;
		asio::streambuf _Write;
		XE::UniquePtr< asio::ip::tcp::socket > _Socket;
	};

	class UDPSession : public Session
	{
	public:
		asio::streambuf _Write;
	};

	class KCPSession : public Session
	{
	public:
		asio::streambuf _Write;
		ikcpcb * _KCP = nullptr;
		XE::Disposable _UpdateTimer;
	};
}

XE::Server::Server()
{
	XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >()->RegisterServer( this );
}

XE::Server::~Server()
{

}

XE::ServerHandle XE::Server::GetHandle() const
{
	return _Handle;
}

XE::Endpoint XE::Server::GetLocalEndpoint() const
{
	return _LocalEndpoint;
}

XE::Endpoint XE::Server::GetRemoteEndpoint( XE::SessionHandle val ) const
{
	return _Sessions[val.GetValue()]->_RemoteEndpoint;
}

void XE::Server::BindAcceptCallback( const AcceptCallbackType & callback )
{
	_AcceptCB = callback;
}

void XE::Server::BindHandshakeCallback( const HandshakeCallbackType & callback )
{
	_HandshakeCB = callback;
}

void XE::Server::BindSendCallback( const SendCallbackType & callback )
{
	_SendCB = callback;
}

void XE::Server::BindReceiveCallback( const ReceiveCallbackType & callback )
{
	_ReceiveCB = callback;
}


struct XE::TCPServer::Private
{
	XE::UniquePtr< asio::ip::tcp::acceptor > _Acceptor;
	std::variant<XE::EndlCondition, XE::RegexCondition, XE::MatchCondition, XE::TransferCondition, XE::DataGramCondition> _Condition;
};

XE::TCPServer::TCPServer()
	:_p( XE::New< Private >() )
{

}

XE::TCPServer::~TCPServer()
{
	Close();

	XE::Delete( _p );
}

void XE::TCPServer::Start( XE::uint16 bind, const XE::EndlCondition & endl )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );

	_p->_Condition = endl;

	_p->_Acceptor = XE::MakeUnique< asio::ip::tcp::acceptor >( *reinterpret_cast<asio::io_service *>( _IOService ), endpoint );

	Accept();
}

void XE::TCPServer::Start( XE::uint16 bind, const XE::RegexCondition & regex )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );

	_p->_Condition = regex;

	_p->_Acceptor = XE::MakeUnique< asio::ip::tcp::acceptor >( *reinterpret_cast<asio::io_service *>( _IOService ), endpoint );

	Accept();
}

void XE::TCPServer::Start( XE::uint16 bind, const XE::MatchCondition & match )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );

	_p->_Condition = match;

	_p->_Acceptor = XE::MakeUnique< asio::ip::tcp::acceptor >( *reinterpret_cast<asio::io_service *>( _IOService ), endpoint );

	Accept();
}

void XE::TCPServer::Start( XE::uint16 bind, const XE::TransferCondition & trans )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );

	_p->_Condition = trans;

	_p->_Acceptor = XE::MakeUnique< asio::ip::tcp::acceptor >( *reinterpret_cast<asio::io_service *>( _IOService ), endpoint );

	Accept();
}

void XE::TCPServer::Start( XE::uint16 bind, const XE::DataGramCondition & dgram )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );

	_p->_Condition = dgram;

	_p->_Acceptor = XE::MakeUnique< asio::ip::tcp::acceptor >( *reinterpret_cast<asio::io_service *>( _IOService ), endpoint );

	Accept();
}

void XE::TCPServer::Send( XE::SessionHandle session, XE::MemoryView view )
{
	if ( auto ptr = static_cast<TCPSession *>( _Sessions[session.GetValue()].get() ) )
	{
		std::visit( XE::Overloaded{
					[&]( const XE::EndlCondition & val )
					{
						std::unique_lock<std::mutex> lock( ptr->_Mutex );

						std::ostream os( &ptr->_Write );
						os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
						os.write( val.endl.c_str(), val.endl.size() );
					},
					[&]( const XE::RegexCondition & val )
					{
						std::ostream os( &ptr->_Write );
						os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
					},
					[&]( const XE::MatchCondition & val )
					{
						std::ostream os( &ptr->_Write );
						os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
					},
					[&]( const XE::TransferCondition & val )
					{
						std::ostream os( &ptr->_Write );
						os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
					},
					[&]( const XE::DataGramCondition & val )
					{
						std::unique_lock<std::mutex> lock( ptr->_Mutex );

						char buf[sizeof( XE::uint64 )];
						XE::WriteBigEndian( buf, view.size() );

						std::ostream os( &ptr->_Write );
						os.write( buf, sizeof( XE::uint64 ) );
						os.write( reinterpret_cast<const char *>( view.data() ), view.size() );
					} }, _p->_Condition );

		Write( session );
	}
}

void XE::TCPServer::Close( XE::SessionHandle session )
{
	if ( auto ptr = static_cast<TCPSession *>( _Sessions[session.GetValue()].get() ) )
	{
		if ( ptr->_Socket != nullptr )
		{
			ptr->_Socket->close();
			ptr->_Socket = nullptr;
		}

		_Sessions[session.GetValue()] = nullptr;

		_SessionAllocator.Free( session );
	}
}

void XE::TCPServer::Close()
{
	if ( _p->_Acceptor != nullptr )
	{
		_p->_Acceptor->close();
		_p->_Acceptor = nullptr;

		for ( auto & it : _Sessions )
		{
			if ( auto session = SP_CAST< TCPSession >( it.get() ) )
			{
				session->_Socket->close();
			}
		}

		_Sessions.clear();
		_SessionAllocator.Reset();
	}
}

void XE::TCPServer::Accept()
{
	_p->_Acceptor->async_accept( [this]( std::error_code code, asio::ip::tcp::socket socket )
								 {
									 if( !code )
									 {
										 XE::SessionHandle handle;

										 handle = _SessionAllocator.Alloc();
										 if( handle.GetValue() >= _Sessions.size() )
										 {
											 _Sessions.push_back( nullptr );
										 }

										 auto session = XE::MakeUnique< TCPSession >();
										 {
											 session->_Handle = handle;
											 session->_RemoteEndpoint.Addr = socket.remote_endpoint().address().to_string();
											 session->_RemoteEndpoint.Port = XE::ToString( socket.remote_endpoint().port() );
											 session->_Socket = XE::MakeUnique< asio::ip::tcp::socket >( std::move( socket ) );

											 _Sessions[handle.GetValue()] = std::move( session );
										 }

										 if ( _AcceptCB )
										 {
											 _AcceptCB( _Handle, handle, code );
										 }

										 if ( _HandshakeCB )
										 {
											 _HandshakeCB( _Handle, handle, code );
										 }

										 Read( handle );
									 }
									 else
									 {
										 if ( _AcceptCB )
										 {
											 _AcceptCB( _Handle, {}, code );
										 }
									 }

									 Accept();
								 } );
}

void XE::TCPServer::Read( XE::SessionHandle session )
{
	if ( auto ptr = static_cast<TCPSession *>( _Sessions[session.GetValue()].get() ) )
	{
		auto cb = [this, ptr]( std::error_code code, std::size_t bytes )
		{
			if ( !code )
			{
				XE::Array<char> buf( XE::MemoryResource::GetFrameMemoryResource() ); buf.resize( bytes );
				std::istream is( &ptr->_Read );
				is.read( buf.data(), bytes );
				if ( _ReceiveCB )
				{
					_ReceiveCB( _Handle, ptr->_Handle, code, { buf.data(), bytes } );
				}

				Read( ptr->_Handle );
			}
			else
			{
				if ( _ReceiveCB )
				{
					_ReceiveCB( _Handle, ptr->_Handle, code, {} );
				}
			}
		};

		std::visit( XE::Overloaded{ [&]( auto & val ) { asio::async_read_until( *ptr->_Socket.get(), ptr->_Read, val, cb ); } }, _p->_Condition );
	}
}

void XE::TCPServer::Write( XE::SessionHandle session )
{
	if ( auto ptr = static_cast<TCPSession *>( _Sessions[session.GetValue()].get() ) )
	{
		if ( ptr->_Write.size() > 0 )
		{
			asio::async_write( *ptr->_Socket.get(), asio::buffer( ptr->_Write.data(), ptr->_Write.size() ), [this, ptr]( std::error_code code, size_t bytes )
			{
				ptr->_Write.consume( bytes );

				if ( !code )
				{
					if ( _SendCB )
					{
						_SendCB( _Handle, ptr->_Handle, code, bytes );
					}

					Write( ptr->_Handle );
				}
				else
				{
					if ( _SendCB )
					{
						_SendCB( _Handle, ptr->_Handle, code, bytes );
					}
				}
			} );
		}
	}
}


struct XE::UDPServer::Private
{
	asio::streambuf _Read;
	XE::uint64 _BufSize;
	asio::ip::udp::endpoint _Acceptor;
	XE::UniquePtr< asio::ip::udp::socket > _Socket;
	XE::ConcurrentHashMap<asio::ip::udp::endpoint, XE::SessionHandle> _SessionMap;
};

XE::UDPServer::UDPServer()
	:_p( XE::New< Private >() )
{

}

XE::UDPServer::~UDPServer()
{
	Close();

	XE::Delete( _p );
}

void XE::UDPServer::Start( XE::uint16 bind, XE::uint64 buf_size /*= 1400*/ )
{
	_p->_Acceptor = asio::ip::udp::endpoint( asio::ip::udp::v4(), bind );
	_LocalEndpoint.Addr = _p->_Acceptor.address().to_string();
	_LocalEndpoint.Port = XE::ToString( _p->_Acceptor.port() );

	_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( _IOService ), _p->_Acceptor );

	Accept();
}

void XE::UDPServer::Send( XE::SessionHandle session, XE::MemoryView view )
{
	if ( auto ptr = static_cast<UDPSession *>( _Sessions[session.GetValue()].get() ) )
	{
		std::ostream os( &ptr->_Write );
		os.write( reinterpret_cast<const char *>( view.data() ), view.size() );

		Write( session );
	}
}

void XE::UDPServer::Close( XE::SessionHandle session )
{
	if ( auto ptr = SP_CAST<UDPSession>( _Sessions[session.GetValue()].get() ) )
	{
		_p->_SessionMap.erase( ToEndpoint<asio::ip::udp::endpoint>( ptr->_RemoteEndpoint ) );
		_Sessions[session.GetValue()] = nullptr;
		_SessionAllocator.Free( session );
	}
}

void XE::UDPServer::Close()
{
	if ( _p->_Socket != nullptr )
	{
		_p->_Socket->close();
		_p->_Socket = nullptr;

		_p->_SessionMap.clear();

		_Sessions.clear();
		_SessionAllocator.Reset();
	}
}

void XE::UDPServer::Accept()
{
	_p->_Socket->async_receive_from( _p->_Read.prepare( _p->_BufSize ), _p->_Acceptor, [this]( std::error_code code, std::size_t bytes )
	{
		_p->_Read.consume( bytes );

		if ( !code )
		{
			XE::SessionHandle session;
			auto remote = _p->_Socket->remote_endpoint();

			if ( !_p->_SessionMap.find( remote, session ) )
			{
				session = _SessionAllocator.Alloc();
				if ( session.GetValue() >= _Sessions.size() )
				{
					_Sessions.resize( session.GetValue() + 1 );
				}

				auto ptr = XE::MakeUnique<UDPSession>();
				{
					ptr->_Handle = session;
					ptr->_RemoteEndpoint.Addr = remote.address().to_string();
					ptr->_RemoteEndpoint.Port = XE::ToString( remote.port() );

					_p->_SessionMap.insert( remote, session );
					_Sessions[session.GetValue()] = std::move( ptr );
				}

				if ( _AcceptCB )
				{
					_AcceptCB( _Handle, session, code );
				}
				if ( _HandshakeCB )
				{
					_HandshakeCB( _Handle, session, code );
				}
			}

			XE::Array<char> buf( XE::MemoryResource::GetFrameMemoryResource() ); buf.resize( bytes );
			std::istream is( &_p->_Read );
			is.read( buf.data(), bytes );
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, session, code, { buf.data(), bytes } );
			}

			Accept();
		}
		else
		{
			if ( _AcceptCB )
			{
				_AcceptCB( _Handle, {}, code );
			}
		}
	} );
}

void XE::UDPServer::Write( XE::SessionHandle session )
{
	if ( auto ptr = SP_CAST<UDPSession>( _Sessions[session.GetValue()].get() ) )
	{
		if ( ptr->_Write.size() > 0 )
		{
			_p->_Socket.get()->async_send_to( asio::buffer( ptr->_Write.data(), ptr->_Write.size() ), ToEndpoint<asio::ip::udp::endpoint>( ptr->_RemoteEndpoint ), [this, ptr]( std::error_code code, size_t bytes )
			{
				ptr->_Write.consume( bytes );

				if ( !code )
				{
					if ( _SendCB )
					{
						_SendCB( _Handle, ptr->_Handle, code, bytes );
					}

					Write( ptr->_Handle );
				}
				else
				{
					if ( _SendCB )
					{
						_SendCB( _Handle, ptr->_Handle, code, bytes );
					}
				}
			} );
		}
	}
}


struct XE::KCPServer::Private
{
	asio::streambuf _Read;
	XE::uint64 _BufSize;
	asio::ip::udp::endpoint _Endpoint;
	XE::UniquePtr< asio::ip::udp::socket > _Socket;
	XE::ConcurrentHashMap<asio::ip::udp::endpoint, XE::SessionHandle> _SessionMap;
};

XE::KCPServer::KCPServer()
	:_p( XE::New< Private >() )
{

}

XE::KCPServer::~KCPServer()
{
	Close();

	XE::Delete( _p );
}

void XE::KCPServer::Start( XE::uint16 bind, XE::uint64 buf_size /*= 1400*/ )
{
	_p->_Endpoint = asio::ip::udp::endpoint( asio::ip::udp::v4(), bind );
	_LocalEndpoint.Addr = _p->_Endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( _p->_Endpoint.port() );

	_p->_BufSize = buf_size;
	_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( _IOService ), _p->_Endpoint );

	Accept();
}

void XE::KCPServer::Send( XE::SessionHandle session, XE::MemoryView view )
{
	if ( auto ptr = SP_CAST<KCPSession>( _Sessions[session.GetValue()].get() ) )
	{
		ikcp_send( ptr->_KCP, view.data(), view.size() );

		Write( session );
	}
}

void XE::KCPServer::Close( XE::SessionHandle session )
{
	if ( auto ptr = static_cast<KCPSession *>( _Sessions[session.GetValue()].get() ) )
	{
		ptr->_UpdateTimer.Dispose();

		ikcp_release( ptr->_KCP );

		_p->_SessionMap.erase( ToEndpoint<asio::ip::udp::endpoint>( ptr->_RemoteEndpoint ) );

		_Sessions[session.GetValue()] = nullptr;
		_SessionAllocator.Free( session );
	}
}

void XE::KCPServer::Close()
{
	if ( _p->_Socket != nullptr )
	{
		_p->_Socket->close();
		_p->_Socket = nullptr;

		_p->_SessionMap.clear();
		for ( auto & it : _Sessions )
		{
			if ( auto ptr = SP_CAST<KCPSession>( it.get() ) )
			{
				ptr->_UpdateTimer.Dispose();

				ikcp_release( ptr->_KCP );
			}
		}

		_Sessions.clear();
		_SessionAllocator.Reset();
	}
}

void XE::KCPServer::Accept()
{
	_p->_Socket->async_receive_from( _p->_Read.prepare( _p->_BufSize ), _p->_Endpoint, [this]( std::error_code code, std::size_t bytes )
	{
		_p->_Read.consume( bytes );

		if ( !code )
		{
			XE::SessionHandle session;
			auto remote = _p->_Socket->remote_endpoint();

			if ( !_p->_SessionMap.find( remote, session ) )
			{
				session = _SessionAllocator.Alloc();
				if ( session.GetValue() >= _Sessions.size() )
				{
					_Sessions.resize( session.GetValue() + 1 );
				}

				auto ptr = XE::MakeUnique<KCPSession>();
				{
					ptr->_Handle = session;
					ptr->_RemoteEndpoint.Addr = remote.address().to_string();
					ptr->_RemoteEndpoint.Port = XE::ToString( remote.port() );

					ptr->_KCP = ikcp_create( static_cast<XE::uint32>( session.GetValue() ), ptr.get() );
					{
						ikcp_setmtu( ptr->_KCP, ( (int)_p->_BufSize ) - 1 );
						ikcp_wndsize( ptr->_KCP, 128, 128 );
						ikcp_nodelay( ptr->_KCP, 1, 10, 2, 1 );
						ikcp_setoutput( ptr->_KCP, []( const char * buf, int len, ikcpcb * kcp, void * user ) -> int
						{
							std::ostream os( &( static_cast<XE::KCPSession *>( user )->_Write ) );
							os.write( buf, len );

							return 0;
						} );
					}

					ptr->_UpdateTimer = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->StartUnscaleTimer( std::chrono::milliseconds( 10 ), [p = ptr.get()]()
					{
						ikcp_update( p->_KCP, 10 );
						return true;
					} );

					_p->_SessionMap.insert( remote, session );
					_Sessions[session.GetValue()] = std::move( ptr );
				}

				if ( _AcceptCB )
				{
					_AcceptCB( _Handle, session, code );
				}
				if ( _HandshakeCB )
				{
					_HandshakeCB( _Handle, session, code );
				}
			}

			XE::Array<char> buf( XE::MemoryResource::GetFrameMemoryResource() ); buf.resize( bytes );
			std::istream is( &_p->_Read );
			is.read( buf.data(), bytes );
			if ( _ReceiveCB )
			{
				_ReceiveCB( _Handle, session, code, { buf.data(), bytes } );
			}

			Accept();
		}
		else
		{
			if ( _AcceptCB )
			{
				_AcceptCB( _Handle, {}, code );
			}
		}
	} );
}

void XE::KCPServer::Write( XE::SessionHandle session )
{
	if ( auto ptr = SP_CAST<KCPSession>( _Sessions[session.GetValue()].get() ) )
	{
		if ( ptr->_Write.size() > 0 )
		{
			_p->_Socket->async_send_to( asio::buffer( ptr->_Write.data(), ptr->_Write.size() ), ToEndpoint<asio::ip::udp::endpoint>( ptr->_RemoteEndpoint ), [this, ptr]( std::error_code code, std::size_t bytes )
			{
				ptr->_Write.consume( bytes );

				if ( !code )
				{
					if ( _SendCB )
					{
						_SendCB( _Handle, ptr->_Handle, code, bytes );
					}

					Write( ptr->_Handle );
				}
				else
				{
					if ( _SendCB )
					{
						_SendCB( _Handle, ptr->_Handle, code, bytes );
					}
				}
			} );
		}
	}
}
