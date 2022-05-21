/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__0C5A5E3A_A31A_4C32_A45A_D4E1EFBE2985
#define DECLARE_H__0C5A5E3A_A31A_4C32_A45A_D4E1EFBE2985

#include "Utils/Declare.h"

BEG_XE_NAMESPACE

class RPC;

class TCPClient;
class UDPClient;
class KCPClient;

class TCPServer;
class UDPServer;
class KCPServer;

class HttpRequest;
class HttpResponse;

class NetworkService;

template< typename Protocol, typename IArchive = XE::BinaryIArchive, typename OArchive = XE::BinaryOArchive > class RPCClient;
template< typename Protocol, typename IArchive = XE::BinaryIArchive, typename OArchive = XE::BinaryOArchive > class RPCServer;

DECL_PTR( Client );
DECL_PTR( Server );
DECL_PTR( Service );

END_XE_NAMESPACE

#endif//DECLARE_H__0C5A5E3A_A31A_4C32_A45A_D4E1EFBE2985
