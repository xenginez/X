/*!
 * \file	Service.h
 *
 * \author	ZhengYuanQing
 * \date	2021/11/06
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SERVICE_H__BABD7EC6_4C87_4CC3_B914_98C949300CEB
#define SERVICE_H__BABD7EC6_4C87_4CC3_B914_98C949300CEB

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Service : public XE::Object
{
	OBJECT( Service, Object )

public:
	Service();

	~Service() override;

public:
	XE::CoreFrameworkPtr GetFramework() const;

	void SetFramework( XE::CoreFrameworkPtr val );

public:
	virtual void Prepare() = 0;

	virtual void Startup() = 0;

	virtual void Update() = 0;

	virtual void Clearup() = 0;

private:
	XE::CoreFrameworkPtr _Framework;
};

END_XE_NAMESPACE

#endif // SERVICE_H__BABD7EC6_4C87_4CC3_B914_98C949300CEB
