/*!
 * \file	CacheService.h
 *
 * \author	ZhengYuanQing
 * \date	2021/12/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CACHESERVICE_H__B2942684_27F6_4582_9828_92B8D8F344C8
#define CACHESERVICE_H__B2942684_27F6_4582_9828_92B8D8F344C8

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API CacheService : public XE::Service
{
	OBJECT( CacheService, XE::Service )

private:
	struct Private;

public:
	CacheService();

	~CacheService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	void InsertCache( XE::uint64 hash, XE::MemoryView data );

	void RemoveCache( XE::uint64 hash );

	void UpdateCache( XE::uint64 hash, XE::MemoryView data );

	XE::MemoryView FindCache( XE::uint64 hash );

private:
	void Reset();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//CACHESERVICE_H__B2942684_27F6_4582_9828_92B8D8F344C8
