/*!
 * \file	Event.h
 *
 * \author	ZhengYuanQing
 * \date	2019/09/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef EVENT_H__297F9FF1_F984_41D3_8EFF_6E87D3E265B5
#define EVENT_H__297F9FF1_F984_41D3_8EFF_6E87D3E265B5

#include "Order.h"
#include "Handle.hpp"

BEG_XE_NAMESPACE

DECL_HANDLE( Event );

class XE_API Event
{
	OBJECT( Event )

public:
	Event();

	Event( XE::EventHandle handle, const XE::Variant & parameter = XE::Variant() );

public:
	static XE::EventPtr Create( XE::EventHandle handle, const XE::Variant & parameter = XE::Variant() );

public:
	bool accept;
	XE::Variant parameter;
	XE::EventHandle handle;
};

END_XE_NAMESPACE

OBJECT_POOL_ALLOCATOR( XE::Event );

DECL_GROUP( XE_API, 1, Event );

#define DECL_EVENT( NAME, DESC, ... ) \
DLL_VAR_WEAK extern const XE::Handle< XE::Event > EVENT_##NAME = XE::HandleCast< XE::Event >( XE::Order::RegisterOrder< XE::EventGroup >( #NAME, DESC, TypeID< __VA_ARGS__ >::Get() ) )

#endif // EVENT_H__297F9FF1_F984_41D3_8EFF_6E87D3E265B5
