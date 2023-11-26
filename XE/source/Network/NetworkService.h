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

#include "Core/Service.h"

#include "Type.h"

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
	void RegisterClient( XE::Client * val );

	void RegisterServer( XE::Server * val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//SERVICE_H__613AEC00_F8C2_4C3A_8612_B15EA97F0B61
