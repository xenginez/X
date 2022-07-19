/*!
 * \file   Platform.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __PLATFORM_H__BDD7C650_EC23_41CF_A4EA_7CDBFE3B0135
#define __PLATFORM_H__BDD7C650_EC23_41CF_A4EA_7CDBFE3B0135

#include "Handle.hpp"

BEG_XE_NAMESPACE

class XE_API Platform
{
public:
	static XE::String CpuInfo();

	static XE::String CpuName();

	static XE::uint64 CpuCoreSize();

	static XE::uint64 CpuCacheL1Size();

	static XE::uint64 CpuCacheL2Size();

public:
	static Language GetDefaultLanguage();

public:
	static XE::int32 GetSrceenWidth();

	static XE::int32 GetSrceenHeight();
};

END_XE_NAMESPACE

#endif // __PLATFORM_H__BDD7C650_EC23_41CF_A4EA_7CDBFE3B0135
