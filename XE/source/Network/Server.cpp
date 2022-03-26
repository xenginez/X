#include "Server.h"

#include "Core/CoreFramework.h"
#include "Core/TimerService.h"
#include "Core/NetworkService.h"

#if PLATFORM_OS & OS_WINDOWS
#include <sdkddkver.h>
#endif

#include <kcp/ikcp.h>
#include <asio/asio.hpp>

static constexpr XE::uint64 udp_timeout = 10;

namespace asio
{
	template <> struct is_match_condition< XE::Match > : public std::true_type
	{
	};
	template <> struct is_match_condition< XE::Regex > : public std::true_type
	{
	};
	template <> struct is_match_condition< XE::DataGram > : public std::true_type
	{
	};
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
		std::mutex _Mutex;
		XE::SessionHandle _Handle;
		XE::Endpoint _RemoteEndpoint;
		XE::ConcurrentQueue< XE::Array< XE::int8 > > _Wirte;
	};

	class TCPSession : public Session
	{
	public:
		asio::streambuf _Read;
		XE::UniquePtr< asio::ip::tcp::socket > _Socket;
	};

	class UDPSession : public Session
	{
	public:
		XE::Disposable _KeepliveTimer;
		std::chrono::high_resolution_clock::time_point _Keeplive;
	};

	class KCPSession : public Session
	{
	public:
		ikcpcb * _KCP = nullptr;
		XE::Disposable _UpdateTimer;
		XE::KCPServer * _Server = nullptr;
	};

	class WEBSession : public Session
	{
	public:
		asio::streambuf _Read;
		XE::UniquePtr< asio::ip::tcp::socket > _Socket;
	};
}


XE::Server::Server()
{

}

XE::Server::~Server()
{

}

XE::ProtocolType XE::Server::GetProtocol() const
{
	return _Protocol;
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

void XE::Server::BindConnectCallback( const ConnectCallbackType & callback )
{
	_ConnectCB = callback;
}

void XE::Server::BindHandshakeCallback( const HandshakeCallbackType & callback )
{
	_HandshakeCB = callback;
}

void XE::Server::BindDisconnectCallback( const DisconnectCallbackType & callback )
{
	_DisconnectCB = callback;
}

void XE::Server::BindSendCallback( const SendCallbackType & callback )
{
	_SendCB = callback;
}

void XE::Server::BindReceiveCallback( const ReceiveCallbackType & callback )
{
	_ReceiveCB = callback;
}

void XE::Server::Send( XE::SessionHandle session, XE::MemoryView data )
{
	DataGram::SizeType sz = static_cast< DataGram::SizeType >( data.size() );

	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	_Buffers.try_pop( buffer ); buffer.resize( sizeof( DataGram::SizeType ) + data.size() );

	::memcpy( buffer.data(), reinterpret_cast< const char * >( &sz ), sizeof( sz ) );

	::memcpy( buffer.data() + sizeof( DataGram::SizeType ), data.data(), data.size() );

	if( _Sessions[session.GetValue()]->_Mutex.try_lock() )
	{
		_Sessions[session.GetValue()]->_Wirte.push( std::move( buffer ) );

		Wirte( session );
	}
}

bool XE::Server::Close()
{
	if( _Close == false )
	{
		_Close = true;

		if( _Mutex.try_lock() )
		{
			for( auto & it : _Sessions )
			{
				if( it != nullptr )
				{
					Clear( it->_Handle );
				}
			}

			_Mutex.unlock();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void XE::Server::WaitClose()
{
	if( _Close == false )
	{
		_Close = true;
	}

	std::unique_lock< std::mutex > lock( _Mutex );

	for( auto & it : _Sessions )
	{
		if( it != nullptr )
		{
			Clear( it->_Handle );
		}
	}
}

void XE::Server::Disconnect( XE::SessionHandle session )
{
	Clear( session );
}

void XE::Server::AllocHandle()
{
	_Handle = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >()->AllocServerHandle();
}

void * XE::Server::GetIOService()
{
	return XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >()->GetIOService();
}

struct XE::TCPServer::Private
{
	XE::uint64 _Size = 0;
	XE::String _Endl = {};
	std::regex _Regex = {};
	XE::Match _Match = {};
	XE::ReadType _Type = XE::ReadType::ENDL;

	XE::UniquePtr< asio::ip::tcp::acceptor > _Acceptor;
};

XE::TCPServer::TCPServer()
	:_p( XE::New< Private >() )
{

}

XE::TCPServer::~TCPServer()
{
	XE::Delete( _p );
}

void XE::TCPServer::Start( XE::int16 bind, const XE::String & endl )
{
	_p->_Type = XE::ReadType::ENDL;

	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_p->_Endl = endl;
	_Close = false;

	Accept();
}

void XE::TCPServer::Start( XE::int16 bind, const std::regex & regex )
{
	_p->_Type = XE::ReadType::REGEX;

	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_p->_Regex = regex;
	_Close = false;

	Accept();
}

void XE::TCPServer::Start( XE::int16 bind, const XE::Match & match )
{
	_p->_Type = XE::ReadType::MATCH;

	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_p->_Match = match;
	_Close = false;

	Accept();
}

void XE::TCPServer::Start( XE::int16 bind, const XE::Transfer & trans )
{
	_p->_Type = XE::ReadType::TRANSFER;

	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_p->_Size = trans.size;
	_Close = false;

	Accept();
}

void XE::TCPServer::Start( XE::int16 bind, const XE::DataGram & dgram )
{
	_p->_Type = XE::ReadType::DATAGRAM;

	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_Close = false;

	Accept();
}

void XE::TCPServer::Start()
{
	if( auto network = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::NetworkService >() )
	{
		AllocHandle();

		asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), std::stoi( _LocalEndpoint.Port.c_str() ) );
		_p->_Acceptor = XE::MakeUnique< asio::ip::tcp::acceptor >( *reinterpret_cast< asio::io_service * >( GetIOService() ), endpoint );

		Accept();
	}
}

void XE::TCPServer::Accept()
{
	_p->_Acceptor->async_accept( [this]( std::error_code code, asio::ip::tcp::socket socket )
								 {
									 XE::SessionHandle handle;

									 if( !code )
									 {
										 std::unique_lock< std::mutex > lock( _Mutex );

										 handle = _SessionAllocator.Alloc();
										 if( handle.GetValue() >= _Sessions.size() )
										 {
											 _Sessions.push_back( nullptr );
										 }

										 auto session = XE::MakeUnique< TCPSession >();

										 session->_Handle = handle;
										 session->_RemoteEndpoint.Addr = socket.remote_endpoint().address().to_string();
										 session->_RemoteEndpoint.Port = XE::ToString( socket.remote_endpoint().port() );
										 session->_Socket = XE::MakeUnique< asio::ip::tcp::socket >( std::move( socket ) );

										 _Sessions[handle.GetValue()].reset( session.release() );
									 }

									 if( _AcceptCB )
									 {
										 _AcceptCB( _Handle, handle, code );
									 }

									 if( _HandshakeCB )
									 {
										 _HandshakeCB( _Handle, handle, code );
									 }

									 if( !code )
									 {
										 if( _ConnectCB )
										 {
											 _ConnectCB( _Handle, handle, code );
										 }

										 Read( handle );
									 }

									 Accept();
								 } );
}

void XE::TCPServer::Read( XE::SessionHandle session )
{
	auto ptr = static_cast< TCPSession * >( _Sessions[session.GetValue()].get() );

	switch( _p->_Type )
	{
	case XE::ReadType::ENDL:
	{
		asio::async_read_until( *ptr->_Socket.get(), ptr->_Read, std::string_view( _p->_Endl.c_str(), _p->_Endl.size() ), [this, ptr]( std::error_code code, std::size_t bytes )
								{
									if( !code )
									{
										std::istream is( &ptr->_Read );

										XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
										_Buffers.try_pop( buffer ); buffer.resize( bytes );

										is.read( buffer.data(), bytes );

										if( _ReceiveCB )
										{
											XE::MemoryView view( buffer.data(), buffer.size() );

											_ReceiveCB( _Handle, ptr->_Handle, code, view );
										}

										buffer.clear();
										_Buffers.push( std::move( buffer ) );

										Read( ptr->_Handle );
									}
									else if( _DisconnectCB )
									{
										_DisconnectCB( _Handle, ptr->_Handle, code );
									}
								} );
	}
	break;
	case XE::ReadType::REGEX:
	{
		asio::async_read_until( *ptr->_Socket.get(), ptr->_Read, Regex( _p->_Regex ), [this, ptr]( std::error_code code, std::size_t bytes )
								{
									if( !code )
									{
										std::istream is( &ptr->_Read );

										XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
										_Buffers.try_pop( buffer ); buffer.resize( bytes );

										is.read( buffer.data(), bytes );

										if( _ReceiveCB )
										{
											XE::MemoryView view( buffer.data(), buffer.size() );

											_ReceiveCB( _Handle, ptr->_Handle, code, view );
										}

										buffer.clear();
										_Buffers.push( std::move( buffer ) );

										Read( ptr->_Handle );
									}
									else if( _DisconnectCB )
									{
										_DisconnectCB( _Handle, ptr->_Handle, code );
									}
								} );
	}
	break;
	case XE::ReadType::MATCH:
	{
		asio::async_read_until( *ptr->_Socket.get(), ptr->_Read, Match( _p->_Match ), [this, ptr]( std::error_code code, std::size_t bytes )
								{
									if( !code )
									{
										std::istream is( &ptr->_Read );

										XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
										_Buffers.try_pop( buffer ); buffer.resize( bytes );

										is.read( buffer.data(), bytes );

										if( _ReceiveCB )
										{
											XE::MemoryView view( buffer.data(), buffer.size() );

											_ReceiveCB( _Handle, ptr->_Handle, code, view );
										}

										buffer.clear();
										_Buffers.push( std::move( buffer ) );

										Read( ptr->_Handle );
									}
									else if( _DisconnectCB )
									{
										_DisconnectCB( _Handle, ptr->_Handle, code );
									}
								}, 0 );
	}
	break;
	case XE::ReadType::TRANSFER:
	{
		asio::async_read( *ptr->_Socket.get(), ptr->_Read, asio::transfer_exactly( _p->_Size ), [this, ptr]( std::error_code code, std::size_t bytes )
						  {
							  if( !code )
							  {
								  std::istream is( &ptr->_Read );

								  XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
								  _Buffers.try_pop( buffer ); buffer.resize( bytes );

								  is.read( buffer.data(), bytes );

								  if( _ReceiveCB )
								  {
									  XE::MemoryView view( buffer.data(), buffer.size() );

									  _ReceiveCB( _Handle, ptr->_Handle, code, view );
								  }

								  buffer.clear();
								  _Buffers.push( std::move( buffer ) );

								  Read( ptr->_Handle );
							  }
							  else if( _DisconnectCB )
							  {
								  _DisconnectCB( _Handle, ptr->_Handle, code );
							  }
						  } );
	}
	break;
	case XE::ReadType::DATAGRAM:
	{
		asio::async_read_until( *ptr->_Socket.get(), ptr->_Read, DataGram(), [this, ptr]( std::error_code code, std::size_t bytes )
								{
									if( !code )
									{
										std::istream is( &ptr->_Read );

										XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
										_Buffers.try_pop( buffer ); buffer.resize( bytes );

										is.read( buffer.data(), bytes );

										if( _ReceiveCB )
										{
											XE::MemoryView view( buffer.data(), buffer.size() );

											_ReceiveCB( _Handle, ptr->_Handle, code, view );
										}

										buffer.clear();
										_Buffers.push( std::move( buffer ) );

										Read( ptr->_Handle );
									}
									else if( _DisconnectCB )
									{
										_DisconnectCB( _Handle, ptr->_Handle, code );
									}
								} );
	}
	break;
	default:
		break;
	}
}

void XE::TCPServer::Wirte( XE::SessionHandle session )
{
	auto ptr = static_cast< TCPSession * >( _Sessions[session.GetValue()].get() );

	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	if( ptr->_Wirte.try_pop( buffer ) )
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

		asio::async_write( *ptr->_Socket.get(), asio::buffer( view.data(), view.size() ), asio::transfer_exactly( view.size() ),
						   [this, ptr, buf = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			if( !code )
			{
				if( _SendCB )
				{
					_SendCB( _Handle, ptr->_Handle, code, bytes );
				}

				Wirte( ptr->_Handle );
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, ptr->_Handle, code );
			}

			buf.clear();
			_Buffers.push( std::move( buf ) );
		} );
	}
	else
	{
		ptr->_Mutex.unlock();
	}
}

void XE::TCPServer::Clear( XE::SessionHandle session )
{
	auto ptr = static_cast< TCPSession * >( _Sessions[session.GetValue()].get() );

	if( ptr != nullptr )
	{
		if( ptr->_Socket != nullptr )
		{
			ptr->_Socket->close();
			ptr->_Socket = nullptr;
		}

		_Sessions[session.GetValue()] = nullptr;

		_SessionAllocator.Free( session );
	}
}


struct XE::UDPServer::Private
{
	XE::uint64 _BufferSize;
	asio::ip::udp::endpoint _Endpoint;
	XE::UniquePtr< asio::ip::udp::socket > _Socket;
};

XE::UDPServer::UDPServer()
	:_p( XE::New< Private >() )
{

}

XE::UDPServer::~UDPServer()
{
	XE::Delete( _p );
}

void XE::UDPServer::Start( XE::int16 bind, XE::uint64 buf_size /*= 1400*/ )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_Close = false;

	_p->_BufferSize = buf_size + sizeof( XE::UDPProtocolFlag ) + sizeof( XE::SessionHandle );

	Accept();
}

void XE::UDPServer::Read()
{
	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	_Buffers.try_pop( buffer ); buffer.resize( _p->_BufferSize );

	_p->_Socket->async_receive_from( asio::buffer( buffer ), _p->_Endpoint, [this, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
	{
		if( !code && bytes >= sizeof( XE::UDPProtocolFlag ) )
		{
			auto buf = buffer.data();

			XE::UDPProtocolFlag flag = *( reinterpret_cast<XE::UDPProtocolFlag *>( buf++ ) );

			switch( flag )
			{
			case XE::UDPProtocolFlag::START:
			{
				XE::SessionHandle handle = _SessionAllocator.Alloc();

				{
					std::unique_lock< std::mutex > lock( _Mutex );

					if( handle.GetValue() >= _Sessions.size() )
					{
						_Sessions.resize( handle.GetValue() );
					}
				}

				auto session = XE::UniquePtr< UDPSession >();

				session->_Handle = handle;
				session->_Keeplive = std::chrono::high_resolution_clock::now();
				session->_RemoteEndpoint.Addr = _p->_Endpoint.address().to_string();
				session->_RemoteEndpoint.Port = XE::ToString( _p->_Endpoint.port() );
				session->_KeepliveTimer = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->
					StartUnscaleTimer( std::chrono::seconds( udp_timeout ), [this, handle]()
				{
					auto now = std::chrono::high_resolution_clock::now();

					auto session = reinterpret_cast<XE::UDPSession *>( _Sessions[handle].get() );

					if( ( now - session->_Keeplive ) > std::chrono::seconds( udp_timeout ) )
					{
						_Sessions[handle] = nullptr;

						_SessionAllocator.Free( XE::HandleCast< XE::Session >( handle ) );

						return true;
					}

					return false;
				} );

				_Sessions[handle.GetValue()] = std::move( session );

				buffer.resize( sizeof( XE::UDPProtocolFlag ) + sizeof( XE::uint64 ) );
				buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::START );
				XE::WriteBigEndian( buffer.data() + sizeof( XE::UDPProtocolFlag ), handle.GetValue() );

				_p->_Socket->async_send_to( asio::buffer( buffer ), _p->_Endpoint, [this, handle, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
				{
					if( _HandshakeCB )
					{
						_HandshakeCB( _Handle, handle, code );
					}

					buffer.clear();
					_Buffers.push( std::move( buffer ) );
				} );

				if( _ConnectCB )
				{
					_ConnectCB( _Handle, handle, code );
				}
			}
			break;
			case XE::UDPProtocolFlag::KEEPLIVE:
			{
				XE::uint64 handle = 0;
				
				buf += XE::ReadBigEndian( buf, handle );

				static_cast<XE::UDPSession *>( _Sessions[handle].get() )->_Keeplive = std::chrono::high_resolution_clock::now();

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			case XE::UDPProtocolFlag::DATAGRAM:
			{
				XE::uint64 handle = 0;

				buf += XE::ReadBigEndian( buf, handle );

				if( _ReceiveCB )
				{
					_ReceiveCB( _Handle, XE::HandleCast< XE::Session >( handle ), code, { buf, buffer.size() - ( buf - buffer.data() ) } );
				}

				static_cast<XE::UDPSession *>( _Sessions[handle].get() )->_Keeplive = std::chrono::high_resolution_clock::now();

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			case XE::UDPProtocolFlag::STOP:
			{
				XE::uint64 handle = 0;

				buf += XE::ReadBigEndian( buf, handle );

				_Sessions[handle] = nullptr;

				_SessionAllocator.Free( XE::HandleCast< XE::Session >( handle ) );

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			default:
				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			break;
			}
		}

		Read();
	} );
}

void XE::UDPServer::Wirte( XE::SessionHandle session )
{
	asio::ip::udp::endpoint endpoint(
		asio::ip::address::from_string( _Sessions[session.GetValue()]->_RemoteEndpoint.Addr.c_str() ),
		std::stoi( _Sessions[session.GetValue()]->_RemoteEndpoint.Port.c_str() ) );

	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	if( _Sessions[session.GetValue()]->_Wirte.try_pop( buffer ) )
	{
		auto beg = sizeof( DataGram::SizeType ) - sizeof( XE::UDPProtocolFlag );
		
		buffer[beg] = static_cast<XE::int8>( XE::UDPProtocolFlag::DATAGRAM );

		XE::MemoryView view = { buffer.data() + beg, buffer.size() - beg };

		_p->_Socket.get()->async_send_to( asio::buffer( view.data(), view.size() ), endpoint, [this, session, buf = std::move( buffer )]( std::error_code code, size_t bytes ) mutable
		{
			if( !code )
			{
				if( _SendCB )
				{
					_SendCB( _Handle, session, code, bytes );
				}

				Wirte( session );
			}
			else if( _DisconnectCB )
			{
				_DisconnectCB( _Handle, session, code );
			}

			buf.clear();
			_Buffers.push( std::move( buf ) );
		} );
	}
	else
	{
		_Sessions[session.GetValue()]->_Mutex.unlock();
	}
}

void XE::UDPServer::Clear( XE::SessionHandle session )
{
	if( _Sessions[session.GetValue()] != nullptr )
	{
		_Sessions[session.GetValue()] = nullptr;
		_SessionAllocator.Free( session );
	}
}

void XE::UDPServer::Accept()
{
	if( auto network = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::Service >() )
	{
		AllocHandle();

		asio::ip::udp::endpoint local_endpoint( asio::ip::udp::v4(), std::stoi( _LocalEndpoint.Port.c_str() ) );

		_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast< asio::io_service * >( GetIOService() ), local_endpoint );

		Read();
	}
}


struct XE::KCPServer::Private
{
	XE::uint64 _BufferSize;
	asio::ip::udp::endpoint _Endpoint;
	XE::UniquePtr< asio::ip::udp::socket > _Socket;
};

XE::KCPServer::KCPServer()
	:_p( XE::New< Private >() )
{

}

XE::KCPServer::~KCPServer()
{
	XE::Delete( _p );
}

void XE::KCPServer::Start( XE::int16 bind, XE::uint64 buf_size /*= 1400*/ )
{
	asio::ip::tcp::endpoint endpoint( asio::ip::tcp::v4(), bind );
	_LocalEndpoint.Addr = endpoint.address().to_string();
	_LocalEndpoint.Port = XE::ToString( endpoint.port() );
	_Close = false;

	_p->_BufferSize = buf_size + sizeof( XE::UDPProtocolFlag );

	Accept();
}

void XE::KCPServer::Read()
{
	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	_Buffers.try_pop( buffer ); buffer.resize( _p->_BufferSize );

	_p->_Socket->async_receive_from( asio::buffer( buffer ), _p->_Endpoint, [this, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
	{
		if( !code && bytes >= sizeof( XE::UDPProtocolFlag ) )
		{
			auto buf = buffer.data();

			XE::UDPProtocolFlag flag = *( reinterpret_cast<XE::UDPProtocolFlag *>( buf++ ) );

			switch( flag )
			{
			case XE::UDPProtocolFlag::START:
			{
				XE::SessionHandle handle = _SessionAllocator.Alloc();

				{
					std::unique_lock< std::mutex > lock( _Mutex );

					if( handle.GetValue() >= _Sessions.size() )
					{
						_Sessions.resize( handle.GetValue() );
					}
				}

				auto session = XE::UniquePtr< XE::KCPSession >();

				session->_Server = this;
				session->_Handle = handle;
				session->_RemoteEndpoint.Addr = _p->_Endpoint.address().to_string();
				session->_RemoteEndpoint.Port = XE::ToString( _p->_Endpoint.port() );
				session->_KCP = ikcp_create( static_cast<XE::uint32>( handle.GetValue() ), session.get() );

				ikcp_setmtu( session->_KCP, ( (int)_p->_BufferSize ) - 1 );
				ikcp_wndsize( session->_KCP, 128, 128 );
				ikcp_nodelay( session->_KCP, 1, 10, 2, 1 );
				ikcp_setoutput( session->_KCP, []( const char * buf, int len, ikcpcb * kcp, void * user ) -> int
				{
					auto session = static_cast<XE::KCPSession *>( user );
					auto server = session->_Server;

					XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
					server->_Buffers.try_pop( buffer ); buffer.resize( len + sizeof( XE::UDPProtocolFlag ) );

					buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::DATAGRAM );
					::memcpy( buffer.data() + sizeof( XE::UDPProtocolFlag ), buf, len );

					auto endpoint = ToEndpoint< asio::ip::udp::endpoint >( session->_RemoteEndpoint );

					server->_p->_Socket->async_send_to( asio::buffer( buffer ), endpoint, [server, session, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
					{
						if( !code )
						{
							if( server->_SendCB )
							{
								server->_SendCB( server->_Handle, session->_Handle, code, bytes );
							}

							buffer.clear();
							server->_Buffers.push( std::move( buffer ) );
							server->Wirte( session->_Handle );
						}
						else if( server->_DisconnectCB )
						{
							server->_DisconnectCB( server->_Handle, session->_Handle, code );
						}
					} );

					return 0;
				} );

				XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::TimerService >()->StartUnscaleTimer( std::chrono::milliseconds( 10 ), [session = session.get()]()
				{
					ikcp_update( session->_KCP, 10 );

					return false;
				} );

				_Sessions[handle.GetValue()] = std::move( session );

				buffer.resize( sizeof( XE::UDPProtocolFlag ) + sizeof( XE::uint64 ) );
				buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::START );
				XE::WriteBigEndian( buffer.data() + sizeof( XE::UDPProtocolFlag ), handle.GetValue() );

				_p->_Socket->async_send_to( asio::buffer( buffer ), _p->_Endpoint, [this, handle, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
				{
					if( _HandshakeCB )
					{
						_HandshakeCB( _Handle, handle, code );
					}

					buffer.clear();
					_Buffers.push( std::move( buffer ) );
				} );

				if( _ConnectCB )
				{
					_ConnectCB( _Handle, handle, code );
				}
			}
			break;
			case XE::UDPProtocolFlag::DATAGRAM:
			{
				XE::uint64 handle = 0;

				buf += XE::ReadBigEndian( buf, handle );

				auto session = reinterpret_cast<XE::KCPSession *>( _Sessions[handle].get() );

				if( ikcp_input( session->_KCP, buf, buffer.size() - ( buf - buffer.data() ) ) != 0 )
				{
					if( _DisconnectCB )
					{
						_DisconnectCB( _Handle, XE::HandleCast< XE::Session >( handle ), std::make_error_code( std::errc::message_size ) );
					}
				}
				else
				{
					buffer.resize( _p->_BufferSize );

					for (;;)
					{
						int len = ikcp_recv( session->_KCP, buffer.data(), buffer.size() );
						if( len >= 0 )
						{
							if( _ReceiveCB )
							{
								_ReceiveCB( _Handle, XE::HandleCast< XE::Session >( handle ), code, { buffer.data(), (XE::uint64)len } );
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

					static_cast<XE::UDPSession *>( _Sessions[handle].get() )->_Keeplive = std::chrono::high_resolution_clock::now();
				}

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			case XE::UDPProtocolFlag::STOP:
			{
				XE::uint64 handle = 0;

				buf += XE::ReadBigEndian( buf, handle );

				auto session = reinterpret_cast<XE::KCPSession *>( _Sessions[handle].get() );

				ikcp_release( session->_KCP );

				_Sessions[handle] = nullptr;

				_SessionAllocator.Free( XE::HandleCast< XE::Session >( handle ) );

				buffer.clear();
				_Buffers.push( std::move( buffer ) );
			}
			break;
			default:
				buffer.clear();
				_Buffers.push( std::move( buffer ) );
				break;
			}
		}

		Read();
	} );
}

void XE::KCPServer::Wirte( XE::SessionHandle session )
{
	XE::Array< XE::int8 > buffer( XE::MemoryResource::GetDefaultMemoryResource() );
	if( _Sessions[session.GetValue()]->_Wirte.try_pop( buffer ) )
	{
		auto kcp = reinterpret_cast<XE::KCPSession *>( _Sessions[session].get() )->_KCP;
		ikcp_send( kcp, buffer.data() + sizeof( DataGram::SizeType ), buffer.size() - sizeof( DataGram::SizeType ) );

		buffer.clear();
		_Buffers.push( std::move( buffer ) );
	}
	else
	{
		_Sessions[session.GetValue()]->_Mutex.unlock();
	}
}

void XE::KCPServer::Clear( XE::SessionHandle session )
{
	if( _Sessions[session.GetValue()] != nullptr )
	{
		XE::Array< XE::int8> buffer( XE::MemoryResource::GetDefaultMemoryResource() );
		_Buffers.try_pop( buffer ); buffer.resize( sizeof( XE::UDPProtocolFlag ) );

		buffer[0] = static_cast<XE::int8>( XE::UDPProtocolFlag::STOP );

		_p->_Socket->async_send_to( asio::buffer( buffer ), _p->_Endpoint, [this, session, buffer = std::move( buffer )]( std::error_code code, std::size_t bytes ) mutable
		{
			auto kcp = reinterpret_cast<XE::KCPSession *>( _Sessions[session.GetValue()].get() );

			ikcp_release( kcp->_KCP );

			_Sessions[session.GetValue()] = nullptr;
			_SessionAllocator.Free( session );

			buffer.clear();
			_Buffers.push( std::move( buffer ) );
		} );
	}
}

void XE::KCPServer::Accept()
{
	if( auto network = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::Service >() )
	{
		AllocHandle();

		asio::ip::udp::endpoint local_endpoint( asio::ip::udp::v4(), std::stoi( _LocalEndpoint.Port.c_str() ) );

		_p->_Socket = XE::MakeUnique< asio::ip::udp::socket >( *reinterpret_cast<asio::io_service *>( GetIOService() ), local_endpoint );

		Read();
	}
}
