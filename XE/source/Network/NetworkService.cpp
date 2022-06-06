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

BEG_META( XE::NetworkService )
END_META()

struct XE::NetworkService::Private
{
	std::thread _Thread;
	asio::io_service _IOService;
	XE::ClientHandleAllocator _ClientAllocator;
	XE::ServerHandleAllocator _ServerAllocator;
	XE::UniquePtr< asio::io_service::work > _Work;
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
	_p->_Work = XE::MakeUnique< asio::io_service::work >( _p->_IOService );
}

void XE::NetworkService::Startup()
{
	_p->_Thread = std::thread( [this]() { _p->_IOService.run(); } );
}

void XE::NetworkService::Update()
{

}

void XE::NetworkService::Clearup()
{
	_p->_Work = nullptr;

	if( _p->_Thread.joinable() )
	{
		_p->_Thread.join();
	}
}

void * XE::NetworkService::GetIOService()
{
	return &_p->_IOService;
}

XE::ClientHandle XE::NetworkService::AllocClientHandle()
{
	return _p->_ClientAllocator.Alloc();
}

XE::ServerHandle XE::NetworkService::AllocServerHandle()
{
	return _p->_ServerAllocator.Alloc();
}

void XE::NetworkService::Dispatch( const XE::Delegate< void() > & callback )
{
	_p->_IOService.dispatch( callback );
}
