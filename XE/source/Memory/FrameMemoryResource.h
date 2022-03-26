/*!
 * \file	FrameMemoryResource.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef FRAMEMEMORYRESOURCE_H__3898E7DA_4906_45B6_9121_62FF417AE3DD
#define FRAMEMEMORYRESOURCE_H__3898E7DA_4906_45B6_9121_62FF417AE3DD

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API FrameMemoryResource : public std::pmr::memory_resource
{
private:
	struct Node
	{
		Node * prev = nullptr;
		Node * next = nullptr;
		XE::uint64 size = 0;
	};

private:
	friend class MemoryResource;

private:
	FrameMemoryResource();

	~FrameMemoryResource() noexcept override;

private:
	void * do_allocate( size_t _Bytes, size_t _Align ) override;

	void do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align ) override;

	bool do_is_equal( const std::pmr::memory_resource & _That ) const noexcept override;

private:
	XE::uint64 _Size = 0;
	void * _Data = nullptr;
	Node * _Current = nullptr;
};

END_XE_NAMESPACE

#endif // FRAMEMEMORYRESOURCE_H__3898E7DA_4906_45B6_9121_62FF417AE3DD
