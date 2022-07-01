/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__7D432CD9_EC29_4924_B303_4414FB236264
#define DECLARE_H__7D432CD9_EC29_4924_B303_4414FB236264

#include "Reflect/Reflect.h"

BEG_XE_NAMESPACE

class MD5;
class UUID;
class Order;
class CRC32;
class Layer;
class Event;
class BASE64;
class Buffer;
class Logger;
class Window;
class Package;
class Library;
class Platform;
class TaskGuard;
class Disposable;
class RingBuffer;

template< XE::uint64 T > class OrderGroup;

template< typename T > class Promise;
template< typename ... Args > class Slot;
template< typename ... Args > class Signal;
template< typename ... Args > class Connect;
template< typename ... Args > class Disconnect;

template< typename T > class Flags;
template< typename T > class Handle;
template< typename T > class HandleAllocator;
template< typename T > struct BufferReadable;
template< typename T > struct BufferWirteable;
template< typename T, typename M > class Concurrent;
template< typename T > class HandleAllocator;
template< typename T, XE::uint64 _Max = std::numeric_limits< XE::uint64 >::max() > class RingHandleAllocator;
template< typename T, XE::uint64 _Max = std::numeric_limits< XE::uint64 >::max() > class QueueHandleAllocator;


END_XE_NAMESPACE

#endif // DECLARE_H__7D432CD9_EC29_4924_B303_4414FB236264
