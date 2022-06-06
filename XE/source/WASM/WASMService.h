/*!
 * \file	WASMService.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WASMSERVICE_H__E368C1E5_7F84_4D47_986A_4F78083E3D5D
#define WASMSERVICE_H__E368C1E5_7F84_4D47_986A_4F78083E3D5D

#include "Type.h"

#include "Core/Service.h"

BEG_XE_NAMESPACE

class XE_API WASMService : public XE::Service
{
	OBJECT( WASMService, XE::Service )

private:
	struct Private;

public:
	WASMService();

	~WASMService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//WASMSERVICE_H__E368C1E5_7F84_4D47_986A_4F78083E3D5D
