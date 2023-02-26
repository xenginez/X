/*!
 * \file	AgentService.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AGENTSERVICE_H__D7CF4222_5677_4F11_95CD_2F7F7403E547
#define AGENTSERVICE_H__D7CF4222_5677_4F11_95CD_2F7F7403E547

#include "Type.h"

BEG_XV_NAMESPACE

class XV_API AgentService : public XE::Service
{
	OBJECT( AgentService, XE::Service )

public:
	AgentService();

	~AgentService() override;

public:

};

END_XV_NAMESPACE

#endif//AGENTSERVICE_H__D7CF4222_5677_4F11_95CD_2F7F7403E547
