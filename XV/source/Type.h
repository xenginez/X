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

enum class ServiceIdent : XE::uint8
{
	NONE = 0,
	SERVICE = 1 << 0,
	GATEWAY = 1 << 1,
	REGISTRY = 1 << 2,
};
DECL_FLAGS( ServiceIdent ); DECL_XV_ENUM( ServiceIdent );

enum class ServiceStatus : XE::uint8
{
	ONLINE,
	OFFLINE,
};
DECL_XV_ENUM( ServiceStatus );

struct XV_API ServiceInfo
{
	XE::String Ip;
	XE::uint16 Port;
	ServiceStatus Status;
};
DECL_XV_CLASS( ServiceInfo );

using ServiceMap = XE::MultiMap< XE::String, XV::ServiceInfo >;

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