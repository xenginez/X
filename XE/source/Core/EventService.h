/*!
 * \file   EventService.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/04
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __EVENTSERVICE_H__DBDF5D9A_0DF3_4FAA_8A61_54560543A0FA
#define __EVENTSERVICE_H__DBDF5D9A_0DF3_4FAA_8A61_54560543A0FA

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API EventService : public XE::Service
{
	OBJECT( EventService, Service )

public:
	typedef XE::Delegate< void( const EventPtr & ) > ListenerType;

private:
	struct Private;

public:
	EventService();

	~EventService() override;

public:
	void Prepare()override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	virtual void PostEvent( const XE::EventPtr & val );

	void PostEvent( XE::EventHandle handle, const XE::Variant & parameter = XE::Variant() );

public:
	virtual XE::Disposable RegisterListener( XE::EventHandle handle, ListenerType listener );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __EVENTSERVICE_H__DBDF5D9A_0DF3_4FAA_8A61_54560543A0FA
