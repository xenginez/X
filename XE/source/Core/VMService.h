/*!
 * \file   VMService.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/09
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef VMSERVICE_H__43C943FB_AAA5_40CA_ABDC_BBBC96B4FF10
#define VMSERVICE_H__43C943FB_AAA5_40CA_ABDC_BBBC96B4FF10

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API VMService : public XE::Service
{
	OBJECT( VMService, XE::Service )

private:
	struct Private;

public:
	VMService();

	~VMService() override;

public:
	void Prepare() override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	
private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // VMSERVICE_H__43C943FB_AAA5_40CA_ABDC_BBBC96B4FF10
