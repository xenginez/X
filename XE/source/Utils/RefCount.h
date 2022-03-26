/*!
 * \file	RefCount.h
 *
 * \author	ZhengYuanQing
 * \date	2021/11/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef REFCOUNT_H__416B3227_FC29_474C_B2CC_243CC7F99FD1
#define REFCOUNT_H__416B3227_FC29_474C_B2CC_243CC7F99FD1

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RefCount : public XE::NonCopyable
{
public:
	RefCount() = default;

	virtual ~RefCount() = default;

public:
	void Reset();

	XE::uint64 Inc();

	XE::uint64 Dec();
	
	XE::uint64 Count() const;

private:
	std::atomic< XE::uint64 > _Count = 0;
};

END_XE_NAMESPACE

#endif // REFCOUNT_H__416B3227_FC29_474C_B2CC_243CC7F99FD1
