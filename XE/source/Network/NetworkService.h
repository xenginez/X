/*!
 * \file	Service.h
 *
 * \author	ZhengYuanQing
 * \date	2022/01/22
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SERVICE_H__613AEC00_F8C2_4C3A_8612_B15EA97F0B61
#define SERVICE_H__613AEC00_F8C2_4C3A_8612_B15EA97F0B61

#include "Type.h"

#include "Core/Service.h"

BEG_XE_NAMESPACE

class XE_API NetworkService : public XE::Service
{
	OBJECT( Service, XE::Service );

private:
	friend class XE::Client;
	friend class XE::Server;

private:
	struct Private;

public:
	NetworkService();

	~NetworkService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

private:
	void * GetIOService();

	XE::ClientHandle AllocClientHandle();

	XE::ServerHandle AllocServerHandle();

	void Dispatch( const XE::Delegate< void() > & callback );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//SERVICE_H__613AEC00_F8C2_4C3A_8612_B15EA97F0B61
