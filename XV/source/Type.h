/*!
* \file   Type.h
*
* \author ZhengYuanQing
* \date   2022/05/23
* \email  zhengyuanqing.95@gmail.com
*
*/
#ifndef TYPE_H__E160E0BD_C7F5_4309_A1FA_511ACCFC2BD9
#define TYPE_H__E160E0BD_C7F5_4309_A1FA_511ACCFC2BD9

#include "Declare.h"

BEG_XV_NAMESPACE

enum class AgentIdent : XE::uint8
{
	NONE = 0,
	CLIENT = 1 << 0,
	SERVICE = 1 << 1,
	GATEWAY = 1 << 2,
	REGISTRY = 1 << 3,
};
DECL_FLAGS( AgentIdent ); DECL_XV_ENUM( AgentIdent );

enum class AgentStatus : XE::uint8
{
	ONLINE,
	OFFLINE,
};
DECL_XV_ENUM( AgentStatus );

struct XV_API AgentInfo
{
	XE::Endpoint Addr;
	XV::AgentStatus Status;
	XV::AgentIdentFlags Ident;
};
DECL_XV_CLASS( AgentInfo );

#if XE_DEBUG
using TCP_RPCServer = XE::RPCServer< XE::TCPServer, XE::JsonIArchive, XE::JsonOArchive >;
using UDP_RPCServer = XE::RPCServer< XE::UDPServer, XE::JsonIArchive, XE::JsonOArchive >;
using KCP_RPCServer = XE::RPCServer< XE::KCPServer, XE::JsonIArchive, XE::JsonOArchive >;
#else
using TCP_RPCServer = XE::RPCServer< XE::TCPServer, XE::BinaryIArchive, XE::BinaryOArchive >;
using UDP_RPCServer = XE::RPCServer< XE::UDPServer, XE::BinaryIArchive, XE::BinaryOArchive >;
using KCP_RPCServer = XE::RPCServer< XE::KCPServer, XE::BinaryIArchive, XE::BinaryOArchive >;
#endif // XE_DEBUG

END_XV_NAMESPACE

#endif//TYPE_H__E160E0BD_C7F5_4309_A1FA_511ACCFC2BD9