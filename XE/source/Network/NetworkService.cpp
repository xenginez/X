#include "NetworkService.h"

#if PLATFORM_OS & OS_WINDOWS
#include <sdkddkver.h>
#endif

#include <kcp/ikcp.h>
#include <kcp/ikcp.c>
#include <asio/asio.hpp>

#include "Core/CoreFramework.h"
#include "Core/ThreadService.h"

#include "Client.h"
#include "Server.h"

namespace
{
	using io_service_ptr = std::shared_ptr< asio::io_service >;
}

BEG_META( XE::NetworkService )
END_META()

struct XE::NetworkService::Private
{
	XE::Array<io_service_ptr> _IOServices;
	XE::ClientHandleAllocator _ClientAllocator;
	XE::ServerHandleAllocator _ServerAllocator;
};

XE::NetworkService::NetworkService()
	:_p( XE::New< Private >() )
{

}

XE::NetworkService::~NetworkService()
{
	XE::Delete( _p );
}

void XE::NetworkService::Prepare()
{
	for ( size_t i = 0; i < std::thread::hardware_concurrency(); i++ )
	{
		_p->_IOServices.push_back( std::make_shared< asio::io_service >() );
	}
}

void XE::NetworkService::Startup()
{
	GetFramework()->GetServiceT< XE::ThreadService >()->ParallelTask( _p->_IOServices.begin(), _p->_IOServices.end(), []( XE::Array<io_service_ptr>::iterator it )
	{
		( *it )->poll();
	} );
}

void XE::NetworkService::Update()
{
	GetFramework()->GetServiceT< XE::ThreadService >()->ParallelTask( _p->_IOServices.begin(), _p->_IOServices.end(), []( XE::Array<io_service_ptr>::iterator it )
	{
		( *it )->poll();
	} );
}

void XE::NetworkService::Clearup()
{
	for ( auto & it : _p->_IOServices )
	{
		it->stop();
	}
	_p->_IOServices.clear();
}

void XE::NetworkService::RegisterClient( XE::Client * val )
{
	val->_Handle = _p->_ClientAllocator.Alloc();
	val->_IOService = &_p->_IOServices[val->_Handle.GetValue() % _p->_IOServices.size()];
}

void XE::NetworkService::RegisterServer( XE::Server * val )
{
	val->_Handle = _p->_ServerAllocator.Alloc();
	val->_IOService = &_p->_IOServices[val->_Handle.GetValue() % _p->_IOServices.size()];
}
