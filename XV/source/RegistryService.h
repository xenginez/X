/*!
 * \file	RegistryService.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef REGISTRYSERVICE_H__175C7E1F_0DF7_443F_A9D3_35EC458153DC
#define REGISTRYSERVICE_H__175C7E1F_0DF7_443F_A9D3_35EC458153DC

#include "Type.h"

BEG_XV_NAMESPACE

class XV_API RegistryService : public XE::Service
{
	OBJECT( RegistryService, XE::Service )

public:
	RegistryService();

	~RegistryService() override;

public:

};

END_XV_NAMESPACE

#endif//REGISTRYSERVICE_H__175C7E1F_0DF7_443F_A9D3_35EC458153DC
