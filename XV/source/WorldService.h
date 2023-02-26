/*!
 * \file	WorldService.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WORLDSERVICE_H__159FEC5B_4756_469C_804E_FA563A91CE87
#define WORLDSERVICE_H__159FEC5B_4756_469C_804E_FA563A91CE87

#include "Type.h"

BEG_XV_NAMESPACE

class XV_API WorldService : public XE::Service
{
	OBJECT( WorldService, XE::Service )

public:
	WorldService();

	~WorldService() override;

public:

};

END_XV_NAMESPACE

#endif//WORLDSERVICE_H__159FEC5B_4756_469C_804E_FA563A91CE87
