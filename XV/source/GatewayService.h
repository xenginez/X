/*!
 * \file	GatewayService.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GATEWAYSERVICE_H__35450FC0_661E_4241_AD6D_5669F81B3368
#define GATEWAYSERVICE_H__35450FC0_661E_4241_AD6D_5669F81B3368

#include "Type.h"

BEG_XV_NAMESPACE

class XV_API GatewayService : public XE::Service
{
	OBJECT( GatewayService, XE::Service )

public:
	GatewayService();

	~GatewayService() override;

public:

};

END_XV_NAMESPACE

#endif//GATEWAYSERVICE_H__35450FC0_661E_4241_AD6D_5669F81B3368
