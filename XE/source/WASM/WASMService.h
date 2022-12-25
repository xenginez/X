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
	static constexpr XE::uint64 npos = std::numeric_limits< XE::uint64 >::max();

public:
	WASMService();

	~WASMService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::uint64 FindGlobal( const XE::String & val );

	XE::uint64 FindFunction( const XE::String & val );

public:
	XE::Variant GetGlobal( XE::uint64 idx );

	void SetGlobal( XE::uint64 idx, const XE::Variant & val );

	XE::Variant InvokeFunction( XE::uint64 idx, XE::InvokeStack * params );

public:
	XE::WASMContext * GetContext() const;

private:
	void Instantiate();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//WASMSERVICE_H__E368C1E5_7F84_4D47_986A_4F78083E3D5D
